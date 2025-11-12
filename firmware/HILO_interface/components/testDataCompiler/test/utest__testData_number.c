/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   utest__testData_number.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	Unit-tests for the testData_number sub-modules
//
------------------------------------------------------------------------------------------------------------------------------*/

#include <testDataCompiler.h>
#include <debugTools.h>
#include <testData_number.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

char *jsonMsg = "{                        \
	\"type\":   \"number\",             \
	\"pins\":   [8,9,10,11,12,13,14,15],\
	\"period\": 10,                     \
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

char *false_jsonMsg = "{                  \
	\"type\":   \"number312\",          \
	\"pins\":   [8,9,10,11,12,13,14,15],\
	\"period\": 10,                     \
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";


TEST (T1, testData_number_check) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;
	
	//
	// Test target: wrong JSON data
	// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
	// 	code
	//
	if ((j_number = cJSON_Parse(false_jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISWARNING(err));

		free(j_number);
		j_number = NULL;
	}

	
	//
	// Test target: correct JSON-data
	//
	if ((j_number = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISSUCCESS(err));

		free(j_number);
		j_number = NULL;
	}


	//
	// Test target: incoherent JSON-data
	//	
	if ((j_number = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		// Field removing
		cJSON_DeleteItemFromObject(j_number, "start");

		if (cJSON_GetObjectItemCaseSensitive(j_number, "start") != NULL) {
			// ERROR!
			err = WERROR_ERRUTEST_CORRUPTDATA;
			ERRORBANNER (err);
			fprintf(stderr, "ERROR! JSON message manipulation failed\n");

		} else {
			err = testData_number_check(j_number);
			ASSERT_TRUE (WERROR_ISERROR(err));
		}

		free(j_number);
		j_number = NULL;
	}

	return;
}


int main() {
	T1__testData_number_check();

	return(0);
}
