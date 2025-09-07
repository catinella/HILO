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
esp_err_t spi_device_polling_transmit(spi_device_handle_t handle, spi_transaction_t *tDesc);

#endif
