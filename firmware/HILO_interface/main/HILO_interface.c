//------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// Description:
//	This firmware allows the HILO's interface subsystem to orchestrate the test procedure. This HILO device can work in two
//	different ways: sccheduled-mode abd realtime-mode.
//
//	Scheduled test mode:
//	====================
//	Firmware's main steps:
//		1) it resets the whole SRAM memories
//		2) it configures the test parameters sending the proper commands to the (STM32M4) realtime micro
//		3) it close the SPI link to the SRAMs
//		4) it sends the test start message to the micro
//		5) it waits for one of the following events:
//			5.1) a test step log from the micro
//			5.2) a critical test-end message from the micro
//			5.3) a test-stop message from the user
//
//	REST APIs:
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
//	JSON messages:
//		<HILO>/test/set_recepit = {
//			configuration = {
//				pin<number> = {IN|OUT} [,pin<number> = {IN|OUT}, ...]},
//				freq = <n>
//			}
//			test_steps = {<hex number> [,<hex number>, ...]}
//		}
//
//
//
//
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
