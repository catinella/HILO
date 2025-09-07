/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   sram_manager.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This library provides useful features for the APS6404L-3SQR-SN 8Mbytes SRAM management
//		In order to minimize the used time, the read/write operations are always implemented sequentially, except of the
//		first	record. In fact that record is used to keep track of the stored-items number and it will be updated just at
//		the end.
//
//		Internal data reppresentation:
//		==============================
//		Every record has 64bit size
//		0                                              64MB
//		+---------------+------------------------------+
//		| Num. of items | item1 | item2 |//////////////| 
//		+---------------+-------+-------+--------------+
//
//		Multiple SRAMs access
//		=====================
//		In order to get the advantages of the SRAM solution, it has been needed to use more devices to achieve 64MB size.
//		In fact the APS6404L-3SQR-SN maximum size is just 8MB and bigger SRAM devices cannot be welded by humans!! :-O
//		64MB looks like a huge quantity of memory, but considering every round HILO would save 8 bytes for round and it
//		would be do it 100 000 times per second (at maximum speed). It means the test must be shorter then (64MB/800KB)
//		80 seconds.
//		At the beginning the idea was to store data using a round-robin access to all chips. But, in this solution every
//		one of them require to get the new (24bit) address for every writing operations. It makes the the operation too
//		slow!!!
//		In the new solution data is written in the firts chip unless it is full, then the new data will be written in the
//		next until also this chip will be full, and so on....
//		
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file  "Linuxwoodo/trunk/templates/lgpl-3.0.txt".
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//
------------------------------------------------------------------------------------------------------------------------------*/
//
// Common C libraries
//
#include <stdio.h>
#include <stdbool.h>

#ifdef MOCK
//
// Libraries used for testing purpose
//
#include "./test/sram_manager__mockedFunctions.h"

#else
//
// ESP32 platform dependent libraries
//
#include "driver/spi_master.h"
#endif

// Other libraries
#include <sram_manager.h>


#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LITTLE_ENDIAN
#undef  BIG_ENDIAN

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef  LITTLE_ENDIAN
#define BIG_ENDIAN
#endif

typedef enum {
	SRAMMAN_OP_RESET,
	SRAMMAN_OP_READ,
	SRAMMAN_OP_WRITE
} sramRwOpsType;


#define SRAM_CHIPSIZE (SRAMMAN_MAXSIZE / SRAMMAN_NUMOFBANKS)


//
// Module's variables
//
static spi_device_handle_t sram_devsHandles[SRAMMAN_NUMOFBANKS]; // Handle of every SRAM chip
static uint32_t            sram_noi        = 0;                  // Number Of (written) Items


//------------------------------------------------------------------------------------------------------------------------------
//                                       P R I V A T E   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------
#ifndef MOCK
static wError _spi_initialization();
#endif
static void   _recordToArray     (ssRecord src, uint8_t *arr);
static wError _sram_seqOP        (spi_device_handle_t dev, uint32_t addr, ssRecord *data, bool flag, sramRwOpsType opt);
static wError _sram_mcSeqOP      (uint64_t *record, sramRwOpsType opt);


#ifndef MOCK
wError _spi_initialization() {
	//
	// Description:
	//	It initializes the SPI port used by the SRAM devices to communicate to the ESP32
	//
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERROR_SPIBUSERROR
	//
	wError                        err = WERROR_SUCCESS;
	spi_bus_config_t              buscfg = {};
	spi_device_interface_config_t devcfg = {};
	esp_err_t                     espErr;
	
	buscfg = {
		.mosi_io_num     = SRAMMAN_ESP32PIN_MOSI,
		.miso_io_num     = SRAMMAN_ESP32PIN_MISO,
		.sclk_io_num     = SRAMMAN_ESP32PIN_CLK,
		.quadwp_io_num   = -1,
		.quadhd_io_num   = -1,
		.max_transfer_sz = 4096
	};

	devcfg = {
		.clock_speed_hz = SRAMMAN_ESP32SPI_CLK,
		.mode           = 0,                           // CPOL=0, CPHA=0
		.queue_size     = 4,
	};

	// SPI BUS initialization
	if (spi_bus_initialize(SPI1_HOST, &buscfg, SPI_DMA_CH_AUTO) == ESP_OK) {

		// SRAM devices adding
		uint8_t csPinsList[SRAMMAN_NUMOFBANKS] = SRAMMAN_CSPINSLIST ;

		for (uint8_t t = 0; t < SRAMMAN_NUMOFBANKS; t++) {
			devcfg.spics_io_num = csPinsList[t];
			espErr = spi_bus_add_device(SPI1_HOST, &devcfg, (sram_devsHandles + t));
			if (espErr != ESP_OK) {
				// ERROR!
				// TODO: errorlog
				err = WERROR_ERROR_SPIBUSERROR;
				break;
			}
		}
	} else {
		// ERROR!
		// TODO: errorlog
		err = WERROR_ERROR_SPIBUSERROR;
	}
	return(err);
}
#endif


void _recordToArray (ssRecord src, uint8_t *arr) {
	//
	// Description:
	//	In order to send/receive data to/from the SRAM using DMA the ssRecord data must be splitted in bytes array
	//	[!] Most of CPUs and MCUs use little endian convention
	//
	for (uint8_t t = 0; t < sizeof(ssRecord); t++)
#ifdef LITTLE_ENDIAN
		arr[t] = (uint8_t)((src >> (t*8)) & 0xFF);
#else
		arr[7 - t] = (uint8_t)((src >> (t*8)) & 0xFF);
#endif
	return;
}


wError _sram_seqOP (spi_device_handle_t dev, uint32_t addr, ssRecord *data, bool chClosingFlag, sramRwOpsType opt) {
	//
	// Description:
	//	This function allows you to write/read data to/from SRAM sequentially. In this meaning it transforms the argument
	//	defined SRAM in a big queue (eg. fifo). To close the data-stream set the chClosingFlag argument to true.
	//	The address will be considered just in the data packet that opens the data stream session
	//	Everytime you change SRAM device the stram will be closed and the new one will be open
	//
	//	[!] If you have open a stream in writing mode you cannot change to read mode (and vice versa) before to have
	//	    closed the stram.
	//
	//	[!] This can be used with DMA. It could be useful for bigger ssRecord blobs
	//
	//	Function's commans:
	//		SRAMMAN_OP_READ   Data reading
	//		SRAMMAN_OP_WRITE  Data wtiting
	//		SRAMMAN_OP_RESET  Data stream stopping! (CS = High)
	//
	//	Generated data on the SPI bus:
	//		+---------------------+-----------+----------+  +----------+  +----------+  .... +----------+ 
	//		| cmd=PSRAM_CMD_WRITE | ADDR=addr |  DATA(0) |  |  DATA(1) |  |  DATA(2) |       |  DATA(N) |
	//		+---------------------+-----------+----------+  +----------+  +----------+  .... +----------+ 
	//		|         8bit        |   24bit   |   8bit   |  |   8bit   |  |   8bit   |       |   8bit   |
	//		+---------------------+-----------+----------+  +----------+  +----------+  .... +----------+ 
	//	
	//
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERROR_SPIBUSERROR
	//	WERROR_ERROR_ILLEGALARG
	//
	wError                     err = WERROR_SUCCESS;
	static bool                activeChFlag = false;
	static spi_device_handle_t activeDev = NULL;

	if (opt == SRAMMAN_OP_RESET) {
		//
		// Stop the data streaming!
		//
		spi_transaction_t t = {0};
		t.length = 0;              // NO DATA!
		
		// It forces CS to HIGH
		if (spi_device_polling_transmit((dev != NULL ? dev : sram_devsHandles[0]), &t) != ESP_OK)
			// ERROR
			err = WERROR_ERROR_SPIBUSERROR;
		
		activeChFlag = false;
		activeDev = NULL;

		
	} else if (data == NULL) {
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;

		
	} else {
		//
		// Common fields for read/write operations
		//
		spi_transaction_ext_t cfg = {0};
		cfg.base.length    = 8 * sizeof(ssRecord);    // Expressed in bits
		cfg.base.addr      = addr & 0xFFFFFF;         // 24bits address

		if (
			activeChFlag == false  ||
			(activeDev != NULL && activeDev != dev)
		) {
			//
			// Stream head
			//

			uint8_t arrData[8];

			cfg.command_bits = 8;
			cfg.address_bits = SRAM_ATTR_ADDRSIZE;
			cfg.base.flags   = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR |
				             (chClosingFlag ? 0 : SPI_TRANS_CS_KEEP_ACTIVE);

			if (opt == SRAMMAN_OP_WRITE) {
				_recordToArray(*data, arrData);
				cfg.base.cmd       = SRAM_CMD_WRITE;
				cfg.base.length  = 8 * sizeof(ssRecord);  // Expressed in bits
				cfg.base.tx_buffer = arrData;

			} else if (opt == SRAMMAN_OP_READ) {
				cfg.base.cmd       = SRAM_CMD_READ;
				cfg.base.rxlength  = 8 * sizeof(ssRecord);  // Expressed in bits
				cfg.base.rx_buffer = data;
			} else
				// ERROR!
				err = WERROR_ERROR_ILLEGALARG;
				
			if (err == WERROR_SUCCESS) {
				spi_transaction_t t = {0};
				t.length = 0;              // NO DATA!
				
				if (
					spi_device_polling_transmit(dev, &t)                       != ESP_OK ||  // CS = High
					spi_device_polling_transmit(dev, (spi_transaction_t*)&cfg) != ESP_OK     // Data reading/writing
				) {
					// ERROR!
					err = WERROR_ERROR_SPIBUSERROR;
			
					// Stop the stream!
					activeChFlag = false;
					activeDev = NULL;
		
				} else if (chClosingFlag == false) {
					// Channel flags updating....
					activeChFlag = true;
					activeDev    = dev;
				}
			}

		} else {
			//
			// Stream body
			//

			uint8_t arrData[8];

			cfg.command_bits   = 0;
			cfg.address_bits   = 0;
			cfg.base.flags     = chClosingFlag ? 0 : SPI_TRANS_CS_KEEP_ACTIVE;

			if (opt == SRAMMAN_OP_WRITE) {
				_recordToArray(*data, arrData);
				cfg.base.length    = 8 * sizeof(ssRecord);
				cfg.base.tx_buffer = arrData;
				
			} else if (opt == SRAMMAN_OP_READ) {
				cfg.base.rxlength  = 8 * sizeof(ssRecord);
				cfg.base.rx_buffer = data;
			} else
				// ERROR!
				err = WERROR_ERROR_ILLEGALARG;
				
			if (err == WERROR_SUCCESS) {
				if (spi_device_polling_transmit(dev, (spi_transaction_t*)&cfg) != ESP_OK)
				{
					// ERROR!
					err = WERROR_ERROR_SPIBUSERROR;
					activeChFlag = false;
	
				} else if (chClosingFlag) {
					// Channel closing...
					activeChFlag = false;
					activeDev    = NULL;
				}	
			}
		}
	}

	return(err);
}


wError _sram_mcSeqOP (uint64_t *record, sramRwOpsType opt) {
	//
	// Description:
	//	This function allows you to perform sequentially operations (rw) in the whole memory area composed by all SRAM chips,
	//    in transparent way.
	//
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERROR_SPIBUSERROR 
	//	WERROR_ERROR_DATAOVERFLOW
	//
	wError               err       = WERROR_SUCCESS;
	static uint32_t      totalRWBS = 0;
	static sramRwOpsType lastOp    = SRAMMAN_OP_RESET;
	uint8_t              chipID    = (uint8_t)((uint32_t)((totalRWBS + 8) / (uint32_t)SRAM_CHIPSIZE));

	// SRAMMAN_OP_RESET option
	if (lastOp != opt) {
		// RESET
		err = _sram_seqOP (sram_devsHandles[chipID], 0, NULL, false, SRAMMAN_OP_RESET);
		
		totalRWBS = 0;
		lastOp = opt;
	}

	if (WERROR_ISERROR(err) == false) {
		if (totalRWBS >= SRAMMAN_MAXSIZE)
			// ERROR!
			err = WERROR_ERROR_DATAOVERFLOW;
		
		else {
			err = _sram_seqOP (sram_devsHandles[chipID], 0, record, false, opt);
			if (WERROR_ISERROR(err) == false)
				totalRWBS = totalRWBS + sizeof(ssRecord);
		}
	}
	return(err);
}
//------------------------------------------------------------------------------------------------------------------------------
//                                         P U B L I C   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------

wError sramManager_write (ssRecord rec) {
	//
	// Description:
	//	This function writes the argument defined record in the first available memory area. The writing operations
	//	are performed sequentially
	//	
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERROR_SPIBUSERROR
	//	WERROR_ERROR_DATAOVERFLOW
	//
	static bool sram_spiCfgFlag = false;
	wError      err = WERROR_SUCCESS;
	
	//
	// Checking for the SPI port configuration
	//
	if (sram_spiCfgFlag == false) {
#ifdef MOCK
		err = test_initialization(sram_devsHandles);
#else
		err = _spi_initialization();
#endif
		sram_spiCfgFlag = WERROR_ISERROR(err) ? false : true;
	}

	if (sram_spiCfgFlag) {
		err = _sram_mcSeqOP(&rec, SRAMMAN_OP_WRITE);
	
		if (WERROR_ISERROR(err) == false)
			sram_noi++;
	}
	
	return(err);
}


wError sramManager_resetNOIfiled (ssRecord *oldNOI) {
	//
	// Description:
	//	This function saves ad cleans the first 8 bytes in the first SRAM chip. This memory area will besed to store
	//	the number of written records. This data is very important and it is needed my the HILO-engine (STM32)
	//
	//	[!] After this call, all data previousely recorded will be lost and rewritten
	//
	ssRecord rec = 0;
	wError   err = WERROR_SUCCESS;
	
	err = _sram_seqOP(NULL, 0, NULL, true, SRAMMAN_OP_RESET);

	if (oldNOI != NULL)
		err = _sram_seqOP (sram_devsHandles[0], 0, oldNOI, true, SRAMMAN_OP_READ);

	if (WERROR_ISERROR(err) == false)
		err = _sram_seqOP (sram_devsHandles[0], 0, &rec, true, SRAMMAN_OP_WRITE);

	return(err);
}

wError sramManager_updateNOIfiled () {
	//
	// Description:
	//	It stops the data-stream, and stores the number of records belong to that stream, into the proper memory area
	//
	//	[!] After this call, YOU SHOULD NOT USE the write function or all data previousely recorded will be lost and
	//	    rewritten
	//
	wError   err = WERROR_SUCCESS;
	ssRecord rec = sram_noi;

	// It forces CH = 1
	err = _sram_seqOP (NULL, 0, NULL, true, SRAMMAN_OP_RESET);

	if (WERROR_ISERROR(err) == false)
		err = _sram_seqOP (sram_devsHandles[0], 0, &rec, true, SRAMMAN_OP_WRITE);
		
	sram_noi = 0;

	return(err);
}


wError sramManager_read  (ssRecord *rec) {
	//
	// Description:
	//	This function is used after the test end to read record-by-record the results.
	//
	// Returned value
	//	WERROR_SUCCESS
	//	(read _sram_seqOP() for more info)
	//
	wError   err = WERROR_SUCCESS;
	
	if (rec != NULL) {
		// It forces CH = 1
		err = _sram_seqOP (NULL, 0, NULL, true, SRAMMAN_OP_RESET);
	
		err = _sram_seqOP(sram_devsHandles[0], 0, rec, true, SRAMMAN_OP_READ);
	} else
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;
		
	return(err);
}


