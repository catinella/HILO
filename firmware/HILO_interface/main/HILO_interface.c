//------------------------------------------------------------------------------------------------------------------------------
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
// Description:
//		This firmware allows the HILO's interface subsystem to orchestrate the test procedure. This HILO device can work
//		in two different ways: sccheduled-mode abd realtime-mode.
//
//		Scheduled test mode:
//		====================
//		Firmware's main steps:
//			1) it resets the whole SRAM memories
//			2) it configures the test parameters sending the proper commands to the (STM32M4) realtime micro
//			3) it close the SPI link to the SRAMs
//			4) it sends the test start message to the micro
//			5) it waits for one of the following events:
//				5.1) a test step log from the micro
//				5.2) a critical test-end message from the micro
//				5.3) a test-stop message from the user
//
//		REST APIs:
//		==========
//		<HILO>/test/set_recepit            // the test's recipit will be accepted, converted, and stored in the main menory
//		<HILO>/test/start                  // if a recepit has been configured then the test will be waked up
//		<HILO>/test/stop                   // If a test is running then test procedyre will be stopped
//		<HILO>/test/delete                 // It stops the current test and remove all data in the external bus type
//		<HILO>/test/get_status             // It returns the test status
//		<HILO>/test/get_resuts             // It returns all data produced by the executed test
//
//		<HILO>/realtime/configure_io       // It configures the bits direction
//		<HILO>/realtime/configure_stream   // It configures the network stream
//		<HILO>/realtime/send_data          // It force the HILO's output pins to change their status
//
//		JSON messages:
//			<HILO>/test/set_recepit = {
//				configuration = {
//					pin<number> = {IN|OUT} [,pin<number> = {IN|OUT}, ...]},
//					freq = <n>
//				}
//				test_steps = {<hex number> [,<hex number>, ...]}
//			}
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
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <sram_manager.h>
#include <>
#include <>

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
