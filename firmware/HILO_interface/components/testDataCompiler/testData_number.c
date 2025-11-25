/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:  testData_number.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module convert the test-data definition in a stream of bits.
//		In this definition type, user can modify 8 bits at the time. In fact, in order to correctly use this module,
//		he has to define a (0-355) number and the bits used to reppresent that number. Those bits will keep that
//		configuration for the period defined by the JASON.
//
//		The key to select this module is "type = number"
//
//		{
//			"type":   "number",
//			"pins"[]:  <0-15>, <0-15>, <0-15>, <0-15>, <0-15>, <0-15>, <0-15>, <0-15>  # 8bits MSB --> LSB
//			"value":   <0-255>,
//			"start":   <n ms>,
//			"stiop":   <n ms>
//		}
//
//		Usually this method is used by the test-client software numeric-keypad, digital-counter...
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
------------------------------------------------------------------------------------------------------------------------------*/

#include <testDataCompiler.h>
#include <testData_number.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

static wError _isNotUsedPin (uint8_t pin) {
	//
	// Description:
	//	This function returns a success if the pin has not been already used/registered or a warning if it has been.
	//	If the first case, the pin will be added to the used-pins internal list.
	//	The pin must be a number smaller then 16 or 255 to clean the internal data
	//
	// Returned value:
	//	WERROR_SUCCESS             // Ok, the pin is available
	//	WERROR_WARNING_RESBUSY     // Warning, the pin was already used
	//	WERROR_ERROR_ILLEGALARG    // Illegal Pin number
	//	WERROR_ERROR_DATAOVERFLOW  // You are trying to register too many pins numbers
	//
	static uint8_t usedPinsList[TD_NUMBER_RESOLUTION];
	static uint8_t usedPinsList_idx = 0;
	wError         err = WERROR_SUCCESS;
	
	if (pin == 255) {
		// References cleaning
		memset(usedPinsList, 255, sizeof(usedPinsList));
		usedPinsList_idx = 0;

	// HILO has 16 pin only, at the moment
	} else if (pin > 15) {
			// ERROR!
			err = WERROR_ERROR_ILLEGALARG;

	} else {
		bool found = false;
		for (uint8_t t = 0; t < usedPinsList_idx; t++) {
			if (usedPinsList[t] == pin) {
				found = true;
				break;
			}
		}
		if (found) {
			// WARNING! (Pin already in use)
			err = WERROR_WARNING_RESBUSY;

		} else if (usedPinsList_idx >= TD_NUMBER_RESOLUTION) {
			// ERROR!
			err = WERROR_ERROR_DATAOVERFLOW;

		} else {
			usedPinsList[usedPinsList_idx] = pin;
			usedPinsList_idx++;
		}
	}
	return(err);
}

wError testData_number_init() {
	//
	// Description:
	//	This function registers the _check() and _generate() methods in the parent class testDataCompiler
	//
	wError err = WERROR_SUCCESS;
	err = testDataCompiler_register(testData_number_check, testData_number_generate);

	return(err);
}

wError testData_number_check (const cJSON *root) {
	//
	// Description:
	//	This function accepts a JSON message as arguments and returns a success value only if the message can be handled
	//	by this module
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_WARNING_MISSMATCHTYPE
	//	WERROR_ERROR_ILLEGALSYNTAX
	//
	wError err = WERROR_SUCCESS;
	cJSON  *pins = NULL, *type = NULL, *value = NULL, *start = NULL, *stop = NULL;
	
	//
	// Checking for the mandatory "type" field
	//
	if (
		(type = cJSON_GetObjectItem(root, "type")) == NULL  ||
		cJSON_IsString(type)                       == false ||
		type->valuestring                          == NULL
	)
		// ERROR!  JSON:type is the lonely mandatory field by all sub-modules!!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	//
	// Checking for the JSON message ID
	//
	else if (strcmp(type->valuestring, TD_NUMBER_KEYWORD) != 0)
		// WARNING!
		err = WERROR_WARNING_TYPEMISSMATCH;
	
	//
	// Checking for the required fields	
	//
	else if (
		(pins = cJSON_GetObjectItem(root,  "pins"))   == NULL  ||
		(value = cJSON_GetObjectItem(root, "value"))  == NULL  ||
		(start = cJSON_GetObjectItem(root, "start"))  == NULL  ||
		(stop = cJSON_GetObjectItem(root,  "stop"))   == NULL  ||
		cJSON_IsArray(pins)                           == false ||
		cJSON_IsNumber(value)                         == false 
		
	)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	//
	// Checking for the field's content
	//
	else if (value->valueint > 255 || (float)stop->valuedouble <= (float)start->valuedouble)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	else {
		cJSON *item = NULL;
		
		int arraySize = cJSON_GetArraySize(pins);
		
		if (arraySize != TD_NUMBER_RESOLUTION)
			// ERROR!
			err = WERROR_ERROR_ILLEGALSYNTAX;

		else {
			// Function's initialization
			_isNotUsedPin(255);
			
			for (uint8_t i = 0; i < TD_NUMBER_RESOLUTION; i++) {
				item  = cJSON_GetArrayItem(pins, i);
				if (cJSON_IsNumber(item) == false) {
					// ERROR!
					err = WERROR_ERROR_ILLEGALSYNTAX;
					break;
				}
				if (_isNotUsedPin(item->valueint) != WERROR_SUCCESS) {
					// ERROR!
					err = WERROR_ERROR_ILLEGALSYNTAX;
					break;
				}
			}
		}
	}
	
	return(err);
}

wError testData_number_generate (const cJSON *root) {
	//
	// Description:
	//	This generates the test data stream
	//
	wError     err = WERROR_SUCCESS;
	configDB_t conf;

	// Frequency setting reading
	err = testDataCompiler_getParams(&conf);
	
	if (WERROR_ISSUCCESS(err)) {
		cJSON    *pins = NULL, *value = NULL, *start = NULL, *stop = NULL, *item = NULL;
		uint32_t steps_t0 = 0, steps_t1 = 0;
		uint16_t bitConf = 0;
		uint8_t  usedPins[TD_NUMBER_RESOLUTION] = {0,0,0,0, 0,0,0,0};
		uint16_t mask = 0;
		
		pins  = cJSON_GetObjectItem(root, "pins");
		value = cJSON_GetObjectItem(root, "value");
		start = cJSON_GetObjectItem(root, "start");
		stop  = cJSON_GetObjectItem(root, "stop");
		
		// [!] Freq is expressed in 1K/10 Hz
		steps_t0 = trunc((conf.freq/10) * (float)(start->valuedouble));
		steps_t1 = trunc((conf.freq/10) * (float)(stop->valuedouble));

		//
		// PINs list retriving...
		//
		for (uint8_t i = 0; i < TD_NUMBER_RESOLUTION; i++) {
			item  = cJSON_GetArrayItem(pins, i);
			usedPins[i] = item->valueint;
			mask |= 1 << item->valueint;
		}

		//
		// Cleaning the used bits in the stored data
		//
		for (uint32_t t = steps_t0; t < steps_t1; t++) {
			if ((err = testDataCompiler_write(~(mask), t, TDC_ANDOP)) && WERROR_ISERROR(err)) {
				// ERROR!
				break;
			}
		}

		//
		// Bits configuration creation...
		//
		if (WERROR_ISERROR(err) == false) {
			for (uint8_t pos = 0; pos < TD_NUMBER_RESOLUTION; pos++) {
				if ((value->valueint & (1 << pos)) > 0)
					bitConf |= 1 << usedPins[pos];
			}
		}
		
		//
		// Bits configuration saving...
		//
		for (uint32_t t = steps_t0; t < steps_t1; t++) {
			if ((err = testDataCompiler_write(bitConf, t, TDC_OROP)) && WERROR_ISERROR(err)) {
				// ERROR!
				break;
			}
		}
	}

	return(err);
}
