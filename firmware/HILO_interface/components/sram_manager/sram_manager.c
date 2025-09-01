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
//	This library provides useful features for the APS6404L-3SQR-SN 8Mbytes SRAM management
//	In order to minimize the used time, the read/write operations are always implemented sequentially, except of the first
//	record. In fact that record is used to keep track of the stored-items number and it will be updated just at the end.
//	Every record has 64bit size
//
//	Internal data reppresentation:
//	0                                              64MB
//	+---------------+------------------------------+
//	| Num. of items | item1 | item2 |//////////////| 
//	+---------------+-------+-------+--------------+
//
//	Multiple SRAMs access
//	=====================
//	In order to get the advantages of the SRAM solution, it has been needed to use more devices to achieve 64MB size. In fact
//	the APS6404L-3SQR-SN maximum size is just 8MB and bigger SRAM devices cannot be welded by humans!! :-O
//	64MB looks like a huge quantity of memory, but considering every round HILO would save 8 bytes and it would be do it
//	100 000 times per second (at maximum speed). It means the test must be shorter then (64MB/800KB) 80 seconds.
//	Anyways to store and read data in the SRAMs you have to use a roud-robin access for the chip0 to the chip7
//
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include "driver/spi_master.h"
#include <sram_manager.h>

#ifndef UTEST
#define UTEST 1
#endif

tpedef enum {
	SRAMMAN_DIRTY,
	SRAMMAN_EMPTY,
	SRAMMAN_WRSTARTED,
	SRAMMAN_RDAVAILAB,
	SRAMMAN_RDSTARTED
} sramFsmType;


//
// Module's variables
//
static sramFsmType         sram_fsm = false;                     // Finite State Machine
static uint32_t            sram_noi = 0;                         // Number Of (written) Items
static spi_device_handle_t sram_devsHandles[SRAMMAN_NUMOFBANKS]; // Handle of every SRAM chip
static uint8_t             sram_rrIndex = 0;                     // SRAM chips Round Robin index


wError spi_initialization() {
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


//------------------------------------------------------------------------------------------------------------------------------
//                                         P U B L I C   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------
wError sramManager_deleteAll() {
	//
	// Description:
	//	It deletes all the SRAM's content and initializes the internal flags.
	//
	// Returned value
	//	WERROR_SUCCESS
	//
	wError err = WERROR_SUCCESS;

	//TODO: procedure to delete the sram's content

	sram_fsm     = SRAMMAN_EMPTY;  // It marks the data db is empty and available to be filled
	sram_rrIndex = 0;              // Round Robin index resetting....
	
	return(err);
}


wError sramManager_write (ssRecord rec) {
	//
	// Description:
	//	This function writes the argument defined record in the first available memory area. The writing operations
	//	are performed sequentially
	//	
	// Returned value
	//	WERROR_SUCCESS
	//	ERROR_ERROR_INITFAILED
	//	WERROR_ERROR_DATAOVERFLOW
	//
	wError err = WERROR_SUCCESS;
	
	if (sram_fsm == SRAMMAN_DIRTY || sram_fsm == SRAMMAN_RDAVAILAB) {
		if (WERROR_ISERROR(sramManager_deleteAll()))
			// ERROR!
			err = WERROR_ERROR_INITFAILED;
		else {
			sram_fsm = SRAMMAN_WRSTARTED;
			// Entering in data writing mode
			// TODO: enable the SRAM in seq write mode

			// NOI record space saving...
			// TODO: write 8bytes of 0
		}
	}

	if (err == WERROR_SUCCESS) {
		if (sram_noi < SRAMMAN_MAXSIZE) {
			// TODO: Write the new rec

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

