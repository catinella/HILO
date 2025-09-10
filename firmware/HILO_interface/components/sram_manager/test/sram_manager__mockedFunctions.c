/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
//
// File:   sram_manager__mockedFunctions.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This module containg all the mocked functions and sub-functions used by the unit tests
//
//
------------------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <debugTools.h>
#include <wError.h>
#include <sram_manager.h>
#include <sram_manager__mockedFunctions.h>

static FILE *tPool[SRAMMAN_NUMOFBANKS];
static bool CS = 0;


wError _csHig(spi_device_handle_t dev) {
	//
	// Description:
	//	The following lines of code simulate the CS-HIGH event
	//
	// Returned value
	//	WERROR_SUCCESS
	//	WERROR_ERRUTEST_IOERROR
	//
	wError err = WERROR_SUCCESS;
	if (dev != NULL && fseek(dev, 0, 0) != 0) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;

	} else {
		for (uint8_t t = 0; t < SRAMMAN_NUMOFBANKS; t++) {
			if (fseek(tPool[t], 0, 0) != 0)
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
		}
	}

	if (WERROR_ISERROR(err)) {
		ERRORBANNER(err);
		printf("fseek(addr=0) failed: %s\n", strerror(errno));
	}
	
	return(err);
}

//------------------------------------------------------------------------------------------------------------------------------
//                                            P U B L I C   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------

wError test_initialization(FILE **pool) {
	//
	// Description
	//	It initializes the virtual SRAM chips and save their file-handles in the argument defined array
	//
	wError err = WERROR_SUCCESS;
	char   filename[PATH_MAX];
	
	for (uint8_t t = 0; t < SRAMMAN_NUMOFBANKS; t++) {
		sprintf(filename, SRAM_VCHIP_TEMPLATE, t);
		pool[t]  = fopen(filename, "w+");
		tPool[t] = pool[t];
		
		if (pool[t] == NULL) {
			// ERROR
			err = WERROR_ERRUTEST_IOERROR;
			ERRORBANNER(err);
			printf("I Cannot open the \"%s\" filename: %s\n", filename, strerror(errno));
		}
	}

	return(err);
}

esp_err_t spi_device_polling_transmit(spi_device_handle_t handle, spi_transaction_t *tDesc) {
	//
	// Description
	//	In ESP32 chips, this function is used to send commands/data to the SRAM chips using SPI bus
	//
	wError   err      = WERROR_SUCCESS;
	size_t   dataSize = 0;
	uint16_t cmd      = 0;
	void     *buffer  = NULL;
	uint64_t addr     = 0;
	
	if (tDesc->flags == 0 && tDesc->length == 0) {
		// CS = high
		err = _csHig(handle);
		CS = 1;
		
	} else {
		if (tDesc->flags & SPI_TRANS_VARIABLE_CMD || tDesc->flags & SPI_TRANS_VARIABLE_ADDR) {
			spi_transaction_ext_t *extDesc = (spi_transaction_ext_t*)tDesc;
			cmd = extDesc->base.cmd;
			addr = extDesc->base.addr;
			
			if (cmd == SRAM_CMD_WRITE) {
				dataSize = extDesc->base.length;
				buffer   = (void*)extDesc->base.tx_buffer;
	
			} else if (cmd == SRAM_CMD_READ) {
				dataSize = extDesc->base.rxlength;
				buffer   = (void*)extDesc->base.rx_buffer;
	
			} else {
				// ERROR
				err = WERROR_ERRUTEST_NOTYETIMPL;
				ERRORBANNER(err);
				printf("Not yet implemented option\n");
			}
			
		} else {
			cmd = tDesc->cmd;
			addr = tDesc->addr;
			
			if (cmd == SRAM_CMD_WRITE) {
				dataSize = tDesc->length;
				buffer   = (void*)tDesc->tx_buffer;
				
			} else if (cmd == SRAM_CMD_READ) {
				dataSize = tDesc->rxlength;
				buffer   = tDesc->rx_buffer;
				
			} else {
				// ERROR
				err = WERROR_ERRUTEST_NOTYETIMPL;
				ERRORBANNER(err);
				printf("Not yet implemented option\n");
			}
		}

		if (CS == 1) {
			if (fseek(handle, (long int)addr, 1) != 0) {
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
				ERRORBANNER(err);
				printf("seek() failed: %s\n", strerror(errno));
			}

		} else if (
			(cmd == SRAM_CMD_WRITE && fwrite(buffer, 1, dataSize, handle) != 0) ||
			(cmd == SRAM_CMD_READ  && fread( buffer, 1, dataSize, handle) != 0)
		) {
			// ERROR!
			err = WERROR_ERRUTEST_IOERROR;
			ERRORBANNER(err);
			printf("Reading/writing operation failed: %s\n", strerror(errno));

		} else if (tDesc->flags & SPI_TRANS_CS_KEEP_ACTIVE) {
			CS = 0;

		} else
			CS = 1;
	}
	
	return(WERROR_ISERROR(err) ? ESP_FAIL : ESP_OK);
}


void testEnd() {
	//
	// Description:
	//	Because in test mode the SRAM chips are simulated with files, at the end of the test you need to close them
	//	explicity
	//
	for (uint8_t t = 0; t < SRAMMAN_NUMOFBANKS; t++)
		if (tPool[t]) fclose(tPool[t]);

	return;
}

