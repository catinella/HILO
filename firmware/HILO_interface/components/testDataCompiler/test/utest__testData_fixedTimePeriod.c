/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   utest__testData_fixedTimePeriod.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	Unit-tests for the testData_fixedTimePeriod sub-modules
//
// License:  LGPL ver 3.0
//
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
------------------------------------------------------------------------------------------------------------------------------*/

#include <testDataCompiler.h>
#include <debugTools.h>
#include <testData_fixedTimePeriod.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

char *jsonMsg = "{                    \
 	\"type\":  \"fixedTimePeriod\", \
	\"pin\":   5,                   \
	\"value\": \"high\",            \
	\"start\": 20,                  \
	\"stop\":  40                   \
}";

char *false_jsonMsg = "{              \
 	\"type\":  \"fixedTeriod\",     \
	\"pin\":   5,                   \
	\"value\": \"high\",            \
	\"start\": 20,                  \
	\"stop\":  40                   \
}";


TEST (T1, testData_fixedTimePeriod_check) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_fixedTimePeriod = NULL;
	
	//
	// Test target: wrong JSON data
	// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
	// 	code
	//
	if ((j_fixedTimePeriod = cJSON_Parse(false_jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_fixedTimePeriod_check(j_fixedTimePeriod);
		ASSERT_TRUE (WERROR_ISWARNING(err));

		free(j_fixedTimePeriod);
		j_fixedTimePeriod = NULL;
	}
	

	//
	// Test target: correct JSON-data
	//
	if ((j_fixedTimePeriod = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		//
		// Test target: right JSON data
		//
		err = testData_fixedTimePeriod_check(j_fixedTimePeriod);
		ASSERT_TRUE (WERROR_ISSUCCESS(err));

		free(j_fixedTimePeriod);
		j_fixedTimePeriod = NULL;
	}

	
	//
	// Test target: incoherent JSON-data
	//	
	if ((j_fixedTimePeriod = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		// Field removing
		cJSON_DeleteItemFromObject(j_fixedTimePeriod, "stop");

		if (cJSON_GetObjectItemCaseSensitive(j_fixedTimePeriod, "stop") != NULL) {
			// ERROR!
			err = WERROR_ERRUTEST_CORRUPTDATA;
			ERRORBANNER (err);
			fprintf(stderr, "ERROR! JSON message manipulation failed\n");

		} else {
			err = testData_fixedTimePeriod_check(j_fixedTimePeriod);
			ASSERT_TRUE (WERROR_ISERROR(err));

			free(j_fixedTimePeriod);
			j_fixedTimePeriod = NULL;
		}
	}

	return;
}


int main() {
	T1__testData_fixedTimePeriod_check();

	return(0);
}
