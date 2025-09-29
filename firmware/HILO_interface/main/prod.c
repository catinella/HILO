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
//
// Filename: prod.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This firmware allows the HILO's interface subsystem to orchestrate the test procedure.
//		For more info, please, read the README file
//
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
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <sram_manager.h>

typedef enum {
	HILO_SCEDMODE,
	HILO_RTMODE,
	HILO_CONFMODE
} modeType;


void app_main(void) {
	
	// Initialization
	
	// Mode selection acknowledge

	// Bluetooth initialization

	// Bluetooth starting

	// Checking for the network configuration

	// SRAM memories resetting

//		2) it configures the test parameters sending the proper commands to the (STM32M4) realtime micro
//		3) it close the SPI link to the SRAMs
//		4) it sends the test start message to the micro
//		5) it waits for one of the following events:
//			5.1) a test step log from the micro
//			5.2) a critical test-end message from the micro
//			5.3) a test-stop message from the user

}
