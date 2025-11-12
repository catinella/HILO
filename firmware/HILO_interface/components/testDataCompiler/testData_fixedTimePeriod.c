/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:  testData_fixedTimePeriod.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module convert the test-data definition in a stream of bits.
//		In this definition type, user can set a fixed period where a defined bit must have a constant value.
//		This value can be set as "swap", in this case the new value will be the inverted old one 
//
//		The key to select this module is "type = fixedTimePeriod"
//		{
// 			"type":  "fixedTimePeriod",        // char*
//			"pin":   <0-15>,                   // uint8_t
//			"value": {"high"|"low"|"swap"}     // char*
//			"start": <n>                       // float - milliseconds
//			"stop":  <n>                       // float - milliseconds
// 		}
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
#include <testData_fixedTimePeriod.h>
#include <stdbool.h>
#include <string.h>
#include <utilities.h>
#include <math.h>

wError testData_fixedTimePeriod_init() {
	//
	// Description:
	//	This function registers the _check() and _generate() methods in the parent class testDataCompiler
	//
	wError err = WERROR_SUCCESS;
	err = testDataCompiler_register(testData_fixedTimePeriod_check, testData_fixedTimePeriod_generate);

	return(err);
}

wError testData_fixedTimePeriod_check (const cJSON *root) {
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
	cJSON  *type = NULL, *pin = NULL, *value = NULL, *start = NULL, *stop = NULL;

	// Checking for the mandatory "type" field
	if (
		(type = cJSON_GetObjectItem(root, "type")) == NULL  ||
		cJSON_IsString(type)                       == false ||
		type->valuestring                          == NULL
	)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	// Checking for the JSON message ID
	else if (strcmp(type->valuestring, TD_FIXEDTIMEPERIOD_KEYWORD) != 0)
		// WARNING!
		err = WERROR_WARNING_TYPEMISSMATCH;
	
	// Checking for the required fields	
	else if (
		(pin = cJSON_GetObjectItem(root, "pin"))     == NULL  ||
		(value = cJSON_GetObjectItem(root, "value")) == NULL  ||
		(start = cJSON_GetObjectItem(root, "start")) == NULL  ||
		(stop = cJSON_GetObjectItem(root, "stop"))   == NULL  ||
		cJSON_IsNumber(pin)                          == false ||
		cJSON_IsString(value)                        == false || 
		cJSON_IsNumber(start)                        == false ||
		cJSON_IsNumber(stop)                         == false
	)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	// Checking for the field's content
	else if (
		(
			strcmp(strupr(value->valuestring), "HIGH") != 0 &&
			strcmp(strupr(value->valuestring), "LOW")  != 0 &&
			strcmp(strupr(value->valuestring), "SWAP") != 0
		)                                                     || 
		pin->valueint >> TDC_NUMOFPINS                        ||
		stop->valuedouble <= start->valuedouble
	)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;


	return(err);
}

wError testData_fixedTimePeriod_generate (const cJSON *root) {
	//
	// Description:
	//	This generates the test data stream
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_ERROR_INTFAILURE   // The DB has not yet been initialized or the initialization failed
	//
	//
	wError     err = WERROR_SUCCESS;
	configDB_t conf;
	cJSON      *pin = NULL, *value = NULL, *start = NULL, *stop = NULL;
	
	// Frequency setting reading
	err = testDataCompiler_getParams(&conf);
	
	if (WERROR_ISSUCCESS(err)) {
		uint32_t steps_t0 = 0, steps_t1 = 0;
		uint16_t bitConf = 0;
		
		pin   = cJSON_GetObjectItem(root, "pin");
		value = cJSON_GetObjectItem(root, "value");
		start = cJSON_GetObjectItem(root, "start");
		stop  = cJSON_GetObjectItem(root, "stop");
		
		steps_t0 = trunc(conf.freq * (float)(start->valuedouble));
		steps_t1 = trunc(conf.freq * (float)(stop->valuedouble));
		
		if (strcmp(strupr(value->valuestring), "HIGH") == 0) {
			bitConf = 1 << pin->valueint;
			for (uint32_t t = steps_t0; t < steps_t1; t++) {
				if ((err = testDataCompiler_write(bitConf, t, TDC_OROP)) && WERROR_ISERROR(err)) {
					// ERROR!
					break;
				}
			}
		
		} else if (strcmp(strupr(value->valuestring), "LOW")  == 0) {
			bitConf = ~(1 << pin->valueint);
			for (uint32_t t = steps_t0; t < steps_t1; t++) {
				if ((err = testDataCompiler_write(bitConf, t, TDC_ANDOP)) && WERROR_ISERROR(err)) {
					// ERROR!
					break;
				}
			}
			
		} else {
			for (uint32_t t = steps_t0; t < steps_t1; t++) {
				// Old value reading...
				if ((err = testDataCompiler_read(&bitConf, t)) && WERROR_ISERROR(err)) {
					// ERROR!
					break;
				
				} else if ((bitConf & (1 << pin->valueint)) == 0) {
					// Old state = LOW ---> new state HIGH
					bitConf =  (1 << pin->valueint);
					if ((err = testDataCompiler_write(bitConf, t, TDC_OROP)) && WERROR_ISERROR(err)) {
						// ERROR!
						break;
					 }
			
				} else {
					// Old state = HIGH ---> new state LOW
					bitConf = ~(1 << pin->valueint);
					if ((err = testDataCompiler_write(bitConf, t, TDC_ANDOP)) && WERROR_ISERROR(err)) {
						// ERROR!
						break;
					 }
				}
			}
					
		}
	} else
		// ERROR! (The DB has not yet been initialized or the initialization failed)
		err = WERROR_ERROR_INTFAILURE;
		
	return(err);
}
