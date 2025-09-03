//------------------------------------------------------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include "driver/spi_master.h"
#include <sram_manager.h>

#ifndef UTEST
#define UTEST 1
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LITTLE_ENDIAN
#undef  BIG_ENDIAN

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef  LITTLE_ENDIAN
#define BIG_ENDIAN
#endif

typedef enum {
	SRAMMAN_DIRTY,
	SRAMMAN_EMPTY,
	SRAMMAN_WRSTARTED,
	SRAMMAN_RDAVAILAB,
	SRAMMAN_RDSTARTED
} sramFsmType;

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
static sramFsmType         sram_fsm        = false;              // Finite State Machine
static uint32_t            sram_noi        = 0;                  // Number Of (written) Items
static uint8_t             sram_rrIndex    = 0;                  // SRAM chips Round Robin index
static bool_t              sram_spiCfgFlag = false;


static wError _spi_initialization() {
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
}


void _recordToArray (ssRecord src, uint8_t *arr) {
	//
	// Description:
	//	In order to send/receive data to/from the SRAM using DMA the ssRecord data must be splitted in bytes array
	//
	for (uint8_t t = 0; t < sizeof(ssRecord); t++)
#ifdef LITTLE_ENDIAN
		arr[t] = (uint8_t)((src >> (t*8)) & 0xFF);
#else
		arr[7 - t] = (uint8_t)((src >> (t*8)) & 0xFF);
#endif
	return;
}


wError _sram_rndOp (spi_device_handle_t dev, uint32_t addr, uint64_t *data, sramRwOpsType opt) {
	//
	// Description:
	//	this function allows you to write/read a single ssRecord data-packet into/from the specified chip, at the
	//	specidied address
	//	[!] SPI flags:
	//		SPI_TRANS_VARIABLE_CMD    it enables the cmd_bits field
	//		SPI_TRANS_VARIABLE_ADDR   it enables the addr_bits field
	//
	//	Generated data on the SPI bus:
	//		+---------------------+-----------+----------+
	//		| cmd=PSRAM_CMD_WRITE | ADDR=addr | DATA=0x0 |
	//		+---------------------+-----------+----------+
	//		|         8bit        |   24bit   |    8bit  |
	//		+---------------------+-----------+----------+
	//
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERROR_ILLEGALARG
	//	WERROR_ERROR_SPIBUSERROR
	//
	wError                err = WERROR_SUCCESS;

	if (data == NULL)
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;

	else {	
		spi_transaction_ext_t t = {0};
		uint8_t               arrData[8];
		
		t.base.flags   = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
		t.base.addr    = addr;               // 24 bit
		t.command_bits = 8;
		t.address_bits = SRAM_ATTR_ADDRSIZE;
	
		if (opt == SRAMMAN_OP_WRITE) {
			_recordToArray(*data, arrData);
			t.base.cmd = SRAM_CMD_WRITE;
			t.base.tx_buffer = arrData;
			t.base.length = 8 * sizeof(ssRecord);    // Expressed in bits
			
		} else if (opt == SRAMMAN_OP_READ) {
			t.base.cmd = SRAM_CMD_READ;
			t.base.rx_buffer = data;                 // Possible compatibility problem {little|big}-endian
			t.base.rxlength = 8 * sizeof(ssRecord);  // Expressed in bits

		} else
			// ERROR!
			err = WERROR_ERROR_ILLEGALARG;
			
		// The following call sends a polling transaction and wait for it to complete
		if (err == WERROR_SUCCESS && spi_device_polling_transmit(dev, (spi_transaction_t*)&t) != ESP_OK)
			// ERROR!
			err = WERROR_ERROR_SPIBUSERROR;
	}
	return(err);
}


wError _sram_seqWrite (spi_device_handle_t dev, uint32_t addr, uint8_t data, bool_t chClosingFlag) {
	//
	// Description:
	//	If you don't set true the chClosingFlag argument, this function will write one byte in the argument defined
	//	address of a SRAM chip, and will not close che channel. In this way you will be able to write the next bytes
	//	sequentially. When you will have to store the last data, setting chClosingFlag argument to true, the function
	//	will close the channel.
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
	//
	wError                     err = WERROR_SUCCESS;
	static bool_t              activeChFlag = false;
	static spi_device_handle_t activeDev = NULL;
	spi_transaction_ext_t      t0 = {0};

	t0.base.length    = 8;
	t0.base.tx_buffer = &data;

	if (
		activeChFlag == false  ||
		(activeDev != NULL && activeDev != dev)
	) {
		//
		// Stream head
		//
		t0.command_bits   = 8;
		t0.address_bits   = SRAM_ATTR_ADDRSIZE;
		t0.base.cmd       = PSRAM_CMD_WRITE;
		t0.base.addr      = addr & 0xFFFFFF;
		t0.base.flags     = SPI_TRANS_VARIABLE_CMD  |
		                    SPI_TRANS_VARIABLE_ADDR |
		                    (chClosingFlag ? 0 : SPI_TRANS_CS_KEEP_ACTIVE);

		if (spi_device_polling_transmit(dev, (spi_transaction_t*)&t0) != ESP_OK) {
			// ERROR!
			err = WERROR_ERROR_SPIBUSERROR;
		
			activeChFlag = false;
			activeDev = NULL;
		
		} else if (chClosingFlag == false) {
			// Channel flags updating....
			activeChFlag = true;
			activeDev    = dev;
		}
		
	} else {
		//
		// Stream body
		//
		t0.command_bits   = 0;
		t0.address_bits   = 0;
		t0.base.flags     = chClosingFlag ? 0 : SPI_TRANS_CS_KEEP_ACTIVE;
		
		if (spi_device_polling_transmit(dev, (spi_transaction_t*)&t0) != ESP_OK) {
			// ERROR!
			err = WERROR_ERROR_SPIBUSERROR;
			activeChFlag = false;

		} else if (chClosingFlag) {
			// Channel closing...
			activeChFlag = false;
			activeDev    = NULL;
		}
	}

	return(err);
}


wError _sram_seqRWops (uint64_t *record, sramRwOpsType opt) {
	//
	// Description:
	//	This function allows you to perform sequentially operations (rw) in the whole memory area composed by all SRAM chips,
	//    in transparent way.
	//	As a fifo queue, you can just drop your 8 bytes records and they will be added respecting the items received order,
	//	or you can read them sequentially
	//
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERROR_SPIBUSERROR  // From _sram_seqWrite()
	//
	//
	static uint32_t      totalRWBS = 0;
	static sramRwOpsType lastOp = SRAMMAN_OP_RESET;

	if (lastOp != opt) {
		// RESET
		totalRWBS = 0;
		lastOp == opt;
	}
	
	if (opt == SRAMMAN_OP_WRITE) {
		uint8_t  chipID    = (uint32_t)((totalWBS + 8) / (uint32_t)SRAM_CHIPSIZE);
		uint8_t  tData     = 0;

		for (uint8_t t = 0; t < sizeof(ssRecord); t++) {
			tData = (uint8_t)((*record >> (t*8)) & 0xFF);
			err = _sram_seqWrite (sram_devsHandles[chipID], 0, tData, false);
			if (WERROR_ISERROR(err))
				break;
			else
				totalWBS = totalWBS + sizeof(sRecord);
		}
		
	} else if (opt == SRAMMAN_OP_READ) {
		// TODO: Reading procedure

	} else
		// ERROR!
		err = WERROR_ERROR_ILLEGALOP;
		
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
	static bool_t   sram_spiCfgFlag = false;
	static uint32_t noi = 0;
	wError          err = WERROR_SUCCESS;
	
	//
	// Checking for the SPI port configuration
	//
	if (sram_spiCfgFlag == false) {
		err = _spi_initialization();
		sram_spiCfgFlag = WERROR_ISERROR(err) ? false : true;
	}

	if (sram_spiCfgFlag) {
		if (sram_fsm != SRAMMAN_WRSTARTED) {
			uint64_t tRec = 0;

			_sram_seqRWops(NULL, SRAMMAN_OP_RESET);   // For errors on write op

			// NOI record resetting...
			err = _sram_seqRWops(&tRec, SRAMMAN_OP_WRITE);

			if (!WERROR_ISERROR(err))
				sram_fsm = SRAMMAN_WRSTARTED;
		}

		if (sram_fsm == SRAMMAN_WRSTARTED) {
			if (sram_noi < (SRAMMAN_MAXSIZE / sizeof(ssRecord)) {
				err = _sram_seqRWops(&record, SRAMMAN_OP_WRITE);

				if (WERROR_ISERROR(err) == false)
					sram_noi++;
				else
					sram_fsm = SRAMMAN_DIRTY;
				
		} else
			// ERROR!
			err = WERROR_ERROR_DATAOVERFLOW;
	}
	
	return(err);
}


wError sramManager_stream() {
}


wError sramManager_read  (ssRecord *rec) {
	//
	// Description:
	//	This function is used after the test end to read record-by-record the results.
	//
	// Returned value
	//	WERROR_SUCCESS
	//
	uint64_t item = 0;
	wError   err  = WERROR_SUCCESS;

	if (sram_fsm == SRAMMAN_RDAVAILAB) {
		// TODO: NOI record reading....

		if (WERROR_ISERROR(err) == false) {
			sram_fsm = SRAMMAN_RDSTARTED;

			if (sram_noi > 0) {
				// TODO: record reading
				sram_noi--;
			}
		}
	}	
		
	return(err);
}

