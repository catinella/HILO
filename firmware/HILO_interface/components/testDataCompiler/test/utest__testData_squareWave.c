/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   utest__testData_squareWave.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	Unit-tests for the testData_squareWave module
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
------------------------------------------------------------------------------------------------------------------------------*/

#include <testDataCompiler.h>
#include <debugTools.h>
#include <testData_squareWave.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

char *jsonMsg = "{                   \
	\"type\":      \"squareWave\", \
	\"pin\":       5,              \
	\"period\":    100,            \
	\"dutyCycle\": 50,             \
	\"start\":     20,             \
	\"stop\":      40              \
}";

char *false_jsonMsg = "{             \
	\"type\":      \"squarave\",   \
	\"pin\":       5,              \
	\"period\":    100,            \
	\"dutyCycle\": 50,             \
	\"start\":     20,             \
	\"stop\":      40              \
}";

TEST (T1, testData_squareWave_check) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_squareWave = NULL;

	
	//
	// Test target: wrong JSON-data type
	// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
	// 	code
	//
	if ((j_squareWave = cJSON_Parse(false_jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_squareWave_check(j_squareWave);
		ASSERT_TRUE (WERROR_ISWARNING(err));

		free(j_squareWave);
		j_squareWave = NULL;
	}


	//
	// Test target: correct JSON-data
	//
	if ((j_squareWave = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		//
		// Test target: right JSON-data
		//
		err = testData_squareWave_check(j_squareWave);
		ASSERT_TRUE (WERROR_ISSUCCESS(err));
		
		free(j_squareWave);
		j_squareWave = NULL;
	}

	
	//
	// Test target: incoherent JSON-data
	//	
	if ((j_squareWave = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else  {
		// Field removing
		cJSON_DeleteItemFromObject(j_squareWave, "period");

		if (cJSON_GetObjectItemCaseSensitive(j_squareWave, "period") != NULL) {
			// ERROR!
			err = WERROR_ERRUTEST_CORRUPTDATA;
			ERRORBANNER (err);
			fprintf(stderr, "ERROR! JSON message manipulation failed\n");

		} else {
			//
			// Test target: right JSON data
			//
			err = testData_squareWave_check(j_squareWave);
			ASSERT_TRUE (WERROR_ISERROR(err));
		}

		free(j_squareWave);
		j_squareWave = NULL;
	}
	return;
}

int main() {
	T1__testData_squareWave_check();
	return(0);
}
