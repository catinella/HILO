/* USER CODE BEGIN Header */
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
// Filename: main.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This it the firmware of the engine component. This component implements the real interaction with the DUT.
//	The test can be splitted in sequential steps, for everyone of them, it loads the HILO's output pins configuration and
//	use it to stimuate the DUT. Then HILO will read the (digital and analogical) DUT response and store the results to
//	create a test report, later.
//
//	symbols:
//		OUTCONF_STORAGE_SIZE:
//			In the HILO device there is a memory area where all 16-bit output words are loaded, before the session
//			starts. This area is implemented by two APS6404L-35QR-SN SPI memory banks, so, its size is 16MB
//
//
//		---- The following symbols define the message used to perform an action in the not-interactive test mode ----
//
//		HENG_TEST_START:
//			It triggers the starting test event
//
//		HENG_TEST_GETSTATUS
//			This is the HILO_interface component request code to get the test status
//
//		HENG_TEST_STOP
//			When the HILO_enfine component receives this command the test is terminated without any delay
//
//
//		---- The following symbols define the message used by the HILO_interface component to configure the test ----
//
//		HENG_TEST_STCONF
//			Not interactive test configuration data
//			[8  bits]: freq (0 - 256Hz)
//			[24 bits]: Test size
//
//		HENG_TEST_ITCONF
//			Interactive test configuration data
//			[4 Bytes]: console's address
//			[1 Bytes]: console's port 
//
//
//
//
// License:  LGPL ver 3.0
//
// 	This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU Lesser General
//	Public License as published by the Free Software Foundation; either version 3.0 of the License,	or (at your option)
//	any later version. 
//
//	For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 	You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//		Free Software Foundation, Inc.,
//		59 Temple Place, Suite 330,
//		Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
/* USER CODE END Header */

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx_hal.h"

#define VCP_TX_Pin       GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define SWDIO_Pin        GPIO_PIN_13
#define SWDIO_GPIO_Port  GPIOA
#define SWCLK_Pin        GPIO_PIN_14
#define SWCLK_GPIO_Port  GPIOA
#define VCP_RX_Pin       GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA

#define OUTCONF_STORAGE_SIZE (16 * 1024 * 1024)

//
// Type-1 messages: [OpCopde]
//
#define HENG_TEST_START     8
#define HENG_TEST_STOP      10
#define HENG_TEST_GETSTATUS 12

//
// Type-2 messages: [OpCopde | OpArg]
//
#define HENG_TEST_ITCONF   18
#define HENG_TEST_ITCONF   20

//
// ERROR codes
//
#define HENG_SUCCESS             1
#define HENG_WARNING_NOTHINGTODO 34
#define HENG_ERROR_ILLEGALCMD    130
#define HENG_ERROR_CONIGMISSING  132


void Error_Handler(void);

#endif
