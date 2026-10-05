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
// Filename: main.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This it the firmware of the engine component. This component implements the real interaction with the DUT.
//	The test can be splitted in sequential steps, for everyone of them, it loads the HILO's output pins configuration and
//	use it to stimuate the DUT. Then HILO will read the (digital and analogical) DUT response and store the results to
//	create a test report, later.
//
//	in order to obtain a precise schedule of the steps comprising the test, they are executed by a timer interrupt, as shown
//	in the following diagram:
//
//		         [TIMER]
//		            |
//		            ▼
//		     +-------------+
//		     | ADC trigger |
//		     +------+------+
//		            |
//		            ▼
//		       +---------+
//		       |  Output |
//		       | loading |
//		       +----+----+
//		            |
//		            ▼
//		    +--------------+
//		    | GPIO writing |
//		    +-------+------+
//		            |
//		            ▼
//		   +-----------------+
//		   | ADC Acknowledge |
//		   +--------+--------+
//		            |
//		            ▼
//		 +----------------------+
//		 | FUSEs status reading |
//		 +----------+-----------+
//		            |
//		            ▼
//		    +--------------+
//		    | GPIO reading |
//		    +-------+------+
//		            |
//		            ▼
//		       +---------+
//		       | Results |
//		       | storing |
//		       +----+----+
//		            |
//		            ▼
//		          [END]          
//
//
//	In order to reduce the time required by a single step, the configurations pins are moved in from the external SRAM to the
//	M4's RAM using a DMA channel. Also the test's results are moved out to another SRAM memory bank using another DMA channel.
//
//		         16 MB SRAM
//		             |
//		             | SPI(1)
//		             | DMA(1)
//		+--------+   |
//		| ARM M4 |   |
//		+--------+---|-----------+
//		|            ▼           |
//		|  +------------------+  |
//		|  │ INPUT RINGBUFFER │  |
//		|  +---------+--------+  |
//		|            │           |
//		|            │           |
//		|            ▼           |
//		|  +------------------+  |
//		|  |  HILO processing |  |
//		|  +---------+--------+  |
//		|            │           |
//		|            │           |
//		|            ▼           |
//		|  +------------------+  |
//		|  │ OUTPUT RINGBUFFER│  |
//		|  +---------+--------+  |
//		|            |           |
//		+------------|-----------+
//		             | DMA(2)
//		             | SPI(2)
//		             |
//		             ▼
//		         64 MB SRAM
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
#include "main.h"
#include <stdbool.h>
#include <stdint.h>
#include <ringBuffer.h>


UART_HandleTypeDef huart1;

       void SystemClock_Config  (void);
static void MX_GPIO_Init        (void);
static void MX_USART1_UART_Init (void);
static void retProc             (uint8_t);

int main(void) {
	uint8_t  cmd          = 0;            // Received command/package
	uint8_t  freq         = 0;            // Sampling frequency for not-interactive test
	uint32_t size         = 0;            // Number of sammpling for not-interactive test (1-1M)
	bool     confFlag     = false;        // Test has been configured?
	bool     interact     = false;        // Test type
	uint8_t  consoleIp[4] = {0, 0, 0, 0}; // Console's IPv4 address
	uint8_t  consolePort  = 0;            // Console's port
	bool     testInProg   = false;        // Test in progress?

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	
	/* Infinite loop */
	while (1) {
		// cmd = 1 byte from I2C

		if ((cmd == HENG_TEST_STCONF || cmd == HENG_TEST_STCONF) && testInProg) {
			// ERROR! (Illegal command while test is running)
			retProc(HENG_ERROR_ILLEGALCMD);
			
		if (cmd == HENG_TEST_STCONF) {
			interract = false;
			
			// freq = 1 byte from I2C
			// Set the timer
			
			// size = 3 bytes from I2C

			confFlag = true;
		
		} else if (cmd == HENG_TEST_ITCONF) {
			// consoleIp = 4 bytes I2C
			// consolePort = 1 byte I2C

			confFlag = true;

		} else if (cmd == HENG_TEST_START) {
			if (confFlag == false) {
				// ERROR! (Not yet configured test)
				retProc(HENG_ERROR_CONIGMISSING);

			} else {
				testInProg = true;

				if (interact == false) {
					// Interactive mode starts

				} else {
					// Configured test starts!!!
				}
			}

		} else if (cmd = HENG_TEST_STOP) {
			if (testInProg == false) {
				// WARNING! (test is not running)
				retProc(HENG_WARNING_NOTHINGTODO);
			
			} else if (interact == false) {
				// Interactive session will be stopped
				testInProg = false;

			} else {
				// Test will be stopped
				testInProg = false;
			}
		}

	}
}

//------------------------------------------------------------------------------------------------------------------------------
//                                        P R I V A T E   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------
void SystemClock_Config(void) {
	//
	// Description:
	//	System Clock Configuration
	//
	
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	//
	// Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.
	//
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	//
	// Initializes the CPU, AHB and APB buses clocks
	//
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

static void MX_USART1_UART_Init(void) {
	//
	// Description:
	//	USART1 Initialization Function
	//

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 38400;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

static void MX_GPIO_Init(void) {
	//
	// Description:
	//	GPIO Initialization Function
	//
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

void Error_Handler(void) {
	//
	// Description:
	//	This function is executed in case of error occurrence.
	//
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
	//
	// Description:
	//	Reports the name of the source file and the source line number
	//
	// Arguments:
	//	file: pointer to the source file name
	//	line: assert_param error line source number
	//
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif
