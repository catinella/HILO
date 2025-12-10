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
// File:   sram_manager__mockedFunctions.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This module containg all the mocked functions and sub-functions used by the unit tests
//	The mocked functions and structures belong to the ESP-IDF framework
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MOCKEDFUNCTIONS_
#define _MOCKEDFUNCTIONS_

#include <stdint.h>
#include <wError.h>

typedef int32_t esp_err_t;

typedef FILE*   spi_device_handle_t;

typedef struct spi_transaction_t {
    uint32_t flags;        
    uint16_t cmd;          
    uint64_t addr;         
    size_t length;         
    size_t rxlength;       
    void *user;            
    const void *tx_buffer; 
    void *rx_buffer;       
} spi_transaction_t;

typedef struct {
    spi_transaction_t base;
    uint8_t command_bits;  
    uint8_t address_bits;  
    uint8_t dummy_bits;    
    uint8_t reserved[2];   
} spi_transaction_ext_t;

#define ESP_OK   0
#define ESP_FAIL -1

#define SPI_TRANS_CS_KEEP_ACTIVE (1<<4)
#define SPI_TRANS_VARIABLE_CMD   (1<<5)
#define SPI_TRANS_VARIABLE_ADDR  (1<<6)

#define SRAM_VCHIP_TEMPLATE  "/tmp/SRAM-%d.bin"



void   testEnd();
wError test_initialization(FILE **pool);

//------------------------------------------------------------------------------------------------------------------------------
//                                          M O C K E D   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------
wError    _spi_initialization (FILE **pool);
esp_err_t spi_device_polling_transmit(spi_device_handle_t handle, spi_transaction_t *tDesc);

void      testEnd();

#endif
