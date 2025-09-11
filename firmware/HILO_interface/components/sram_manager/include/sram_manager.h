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
// File:   sram_manager.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This library provides useful functions for the APS6404L-3SQR-SN 8Mbytes SRAM management
//		Using them you will be able to use all SRAM chips ad a single dueue (eg. fifo)
//
//		Symbols:
//		========
//		SRAMMAN_MAXSIZE        the max number of records you can store in the SRAM. (*1)
//		SRAMMAN_ESP32PIN_MISO  ESP32's SPI pin 
//		SRAMMAN_ESP32PIN_MOSI  ""
//		SRAMMAN_ESP32PIN_CLK   ""
//		SRAMMAN_ESP32PIN_CS<n> ESP32's SPI CS pin connected to the proper SRAM chip
//		SRAMMAN_ESP32SPI_CLK   ESP32's SPI clock (*2)
//
//		(*1) At the moment, the allocated SRAM size is 64MB (8 chips of 8MB), and because every item's size is 8B, it means
//		     the max number you can set is 67108864/8 - 1 = 8388607
//
//		(*2) At the moment, the max sampling rate should be 100KHz, it means the SRAMs would require 800KB/s (less then
//		     10Mbit/sec) so 20Mhx (20000000 Hz) could be enough
//
//		Unit tests
//		==========
//		In ordet to build this code for unit testing activities, you have to define the "MOCK" symbol. In this way
//		the code will be compatible with x86 CPU architectures and Linux OS, and you will be able to run the unit tests
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
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#ifndef __SRAMMAN__
#define __SRAMMAN__

#include <wError.h>

typedef uint64_t ssRecord;

#define SRAMMAN_MAXSIZE        8388607
#define SRAMMAN_NUMOFBANKS     8

//
// ESP32's SPI configuration
//
#define SRAMMAN_ESP32PIN_MISO  19
#define SRAMMAN_ESP32PIN_MOSI  23
#define SRAMMAN_ESP32PIN_CLK   18
#define SRAMMAN_ESP32PIN_CS0   5
#define SRAMMAN_ESP32PIN_CS1   12
#define SRAMMAN_ESP32PIN_CS2   13
#define SRAMMAN_ESP32PIN_CS3   14
#define SRAMMAN_ESP32PIN_CS4   15
#define SRAMMAN_ESP32PIN_CS5   16
#define SRAMMAN_ESP32PIN_CS6   17
#define SRAMMAN_ESP32PIN_CS7   21
#define SRAMMAN_ESP32SPI_CLK   20000000

//
// APS6404L-3SQR-SN SRAM's commands and attributes
//
#define SRAM_CMD_READ       0x03
#define SRAM_CMD_WRITE      0x02
#define SRAM_CMD_FASTREAD   0x0b
#define SRAM_CMD_GETDEVID   0x9f
#define SRAM_ATTR_ADDRSIZE  24


#define SRAMMAN_CSPINSLIST { \
	SRAMMAN_ESP32PIN_CS0,  \
	SRAMMAN_ESP32PIN_CS1,  \
	SRAMMAN_ESP32PIN_CS2,  \
	SRAMMAN_ESP32PIN_CS3,  \
	SRAMMAN_ESP32PIN_CS4,  \
	SRAMMAN_ESP32PIN_CS5,  \
	SRAMMAN_ESP32PIN_CS6,  \
	SRAMMAN_ESP32PIN_CS7   \
}


//------------------------------------------------------------------------------------------------------------------------------
//                                        F U N C T I O N S   P R O T O T Y P E S
//------------------------------------------------------------------------------------------------------------------------------
wError sramManager_write          (ssRecord rec);
wError sramManager_read           (ssRecord *rec);
wError sramManager_resetNOIfield  (ssRecord *oldNOI);
wError sramManager_updateNOIfield (); 


#endif
