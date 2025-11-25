/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:  testData_squareWave.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module convert the test-data definition in a stream of bits.
//		In this definition type, user can set a digital wave as test-data
//
//		The key to select this module is "type = squareWave"
//
//		{
//			"type":       "squareWave",
//			"pin":        <0-15>,
//			"period":     <n>,         # milli seconds
//			"dutyCycle":  <0-100>,     # percent
//			"start":      <n ms>,
//			"stiop":      <n ms>
//		}
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
#include <testData_squareWave.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

wError testData_squareWave_init() {
	//
	// Description:
	//	This function registers the _check() and _generate() methods in the parent class testDataCompiler
	//
	wError err = WERROR_SUCCESS;
	err = testDataCompiler_register(testData_squareWave_check, testData_squareWave_generate);

	return(err);
}

wError testData_squareWave_check (const cJSON *root) {
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
	cJSON *j_type = NULL, *j_pin = NULL, *j_period = NULL, *j_dutyCycle = NULL, *j_start = NULL, *j_stop = NULL;
	
	if (
		(j_type = cJSON_GetObjectItem(root, "type")) == NULL  ||
		cJSON_IsString(j_type)                       == false ||
		j_type->valuestring                          == NULL
	)
		// ERROR!  JSON:type is the lonely mandatory field for all sub-modules!!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	// Checking for the JSON message ID
	else if (strcmp(j_type->valuestring, TD_SQUAREWAVE_KEYWORD) != 0)
		// WARNING!
		err = WERROR_WARNING_TYPEMISSMATCH;
	
	// Checking for the required fields	
	else if (
		(j_pin = cJSON_GetObjectItem(root, "pin"))             == NULL  ||
		(j_period = cJSON_GetObjectItem(root, "period"))       == NULL  ||
		(j_dutyCycle = cJSON_GetObjectItem(root, "dutyCycle")) == NULL  ||
		(j_start = cJSON_GetObjectItem(root, "start"))         == NULL  ||
		(j_stop = cJSON_GetObjectItem(root, "stop"))           == NULL  ||
		cJSON_IsNumber(j_pin)                                  == false ||
		cJSON_IsNumber(j_period)                               == false ||
		cJSON_IsNumber(j_dutyCycle)                            == false ||
		cJSON_IsNumber(j_start)                                == false ||
		cJSON_IsNumber(j_stop)                                 == false
	)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	// Checking for the field's content
	else if (
		j_pin->valueint > 15                                         ||
		j_period->valueint == 0                                      ||
		(j_dutyCycle->valueint == 0 || j_dutyCycle->valueint > 100)  ||
		(float)j_stop->valuedouble <= (float)j_start->valuedouble
	)
		// ERROR!
		err = WERROR_ERROR_ILLEGALSYNTAX;

	return(err);
}

wError testData_squareWave_generate (const cJSON *root) {
	//
	// Description:
	//	This generates the test data stream
	//
	wError err = WERROR_SUCCESS;
	configDB_t conf;
	cJSON      *pin = NULL, *period = NULL, *dCycle = NULL, *start = NULL, *stop = NULL;
	
	// Frequency setting reading
	err = testDataCompiler_getParams(&conf);
	
	if (WERROR_ISSUCCESS(err)) {
		uint32_t steps_t0 = 0, steps_t1 = 0;
		
		pin    = cJSON_GetObjectItem(root, "pin");
		period = cJSON_GetObjectItem(root, "period");
		dCycle = cJSON_GetObjectItem(root, "dutyCycle");
		start  = cJSON_GetObjectItem(root, "start");
		stop   = cJSON_GetObjectItem(root, "stop");
		
		// [!] Freq is expressed in 1K/10 Hz
		steps_t0 = trunc((conf.freq/10) * (float)(start->valuedouble));
		steps_t1 = trunc((conf.freq/10) * (float)(stop->valuedouble));


		{
			uint8_t st = 0;
			uint16_t hs = 0, ls = 0;
			uint16_t hsCounter = trunc(conf.freq * (period->valueint * dCycle->valueint / 100));
			uint16_t lsCounter = trunc(conf.freq * (period->valueint * (100 - dCycle->valueint) / 100));
			uint16_t bitConf = 0;
			
			for (uint32_t t = steps_t0; t < steps_t1; t++) {
				if (st == 0) {
					bitConf = 1 << pin->valueint;
					if (hs > hsCounter) {
						st =1;
						ls = 0;
					} else {
						hs++;
						err = testDataCompiler_write(bitConf, t, TDC_OROP);
					}
					
				} else if (st == 1) {
					bitConf = ~(1 << pin->valueint);
					if (hs > lsCounter) {
						st = 0;
						hs = 0;
					} else {
						ls++;
						err = testDataCompiler_write(bitConf, t, TDC_ANDOP);
					}
				}
				if (WERROR_ISERROR(err))
					// ERROR!
					break;
			}
		} 
	}
	
	return(err);
}
