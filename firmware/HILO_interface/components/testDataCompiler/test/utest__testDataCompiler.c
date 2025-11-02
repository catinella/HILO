/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   utest__testDataCompiler.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	Unit-tests for the testDataCompiler fabric and its sub-modules
//
------------------------------------------------------------------------------------------------------------------------------*/

#include <testDataCompiler.h>
#include <debugTools.h>
#include <testData_fixedTimePeriod.h>
#include <testData_number.h>
#include <testData_squareWave.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


char *FTP_jsonMsg = "{                \
 	\"type\":  \"fixedTimePeriod\", \
	\"pin\":   5,                   \
	\"value\": \"high\",            \
	\"start\": 20,                  \
	\"stop\":  40                   \
}";

char *NUM_jsonMsg = "{                    \
	\"type\":   \"number\",             \
	\"pins\":   [8,9,10,11,12,13,14,15],\
	\"period\": 10,                     \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

char *SQW_jsonMsg = "{              \
	\"type\":      \"squareWave\",\
	\"pin\":       5,             \
	\"period\":    10,            \
	\"dutyCycle\": 50,            \
	\"start\":     20,            \
	\"stop\":      40             \
}";

char *jsonMsg = "{                                       \
	\"configuration\": {                               \
		\"freq\": 5                                  \
	},                                                 \
	\"test_outputData\": [                             \
		{                                            \
			\"type\":      \"fixedTimePeriod\",    \
			\"pin\":       5,                      \
			\"value\":     \"high\",               \
			\"start\":     10,                     \
			\"stop\":      15                      \
		}, {                                         \
			\"type\":      \"number\",             \
			\"pins\":      [8,9,10,11,12,13,14,15],\
			\"period\":    10,                     \
			\"start\":     20,                     \
			\"stop\":      40                      \
		}, {                                         \
			\"type\":      \"squareWave\",         \
			\"pin\":       7,                      \
			\"period\":    10,                     \
			\"dutyCycle\": 50,                     \
			\"start\":     10,                     \
			\"stop\":      100                     \
		}                                            \
	]                                                  \
}";

//------------------------------------------------------------------------------------------------------------------------------
//                                              U N I T   T E S T S
//------------------------------------------------------------------------------------------------------------------------------

TEST (T1, testDataCompiler_init) {
	wError err = WERROR_SUCCESS;
	
	// All sub-modules initialization...
	err = testDataCompiler_init();
	ASSERT_EQ (WERROR_SUCCESS, err);

	return;
}

TEST (T2, testData_squareWave_check) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_squareWave = NULL;

	
	if ((j_squareWave = cJSON_Parse(NUM_jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		//
		// Test target: wrong JSON data
		// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
		// 	code
		//
		err = testData_squareWave_check(j_squareWave);
		ASSERT_TRUE (WERROR_ISWARNING(err));

		free(j_squareWave);
		j_squareWave = NULL;
		
		if ((j_squareWave = cJSON_Parse(SQW_jsonMsg)) == NULL) {
			// ERROR!
			err = WERROR_ERRUTEST_CORRUPTDATA;
			ERRORBANNER (err);
			fprintf(stderr, "ERROR! JSON message parsing failed\n");

		} else {
			//
			// Test target: right JSON data
			//
			err = testData_squareWave_check(j_squareWave);
			ASSERT_TRUE (WERROR_ISSUCCESS(err));
		}
	}

	return;
}


TEST (T3, testData_number_check) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;
	
	if ((j_number = cJSON_Parse(SQW_jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		//
		// Test target: wrong JSON data
		// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
		// 	code
		//
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISWARNING(err));

		free(j_number);
		j_number = NULL;
		
		if ((j_number = cJSON_Parse(NUM_jsonMsg)) == NULL) {
			// ERROR!
			err = WERROR_ERRUTEST_CORRUPTDATA;
			ERRORBANNER (err);
			fprintf(stderr, "ERROR! JSON message parsing failed\n");

		} else {
			//
			// Test target: right JSON data
			//
			err = testData_number_check(j_number);
			ASSERT_TRUE (WERROR_ISSUCCESS(err));
		}
	}

	return;
}


TEST (T4, testData_fixedTimePeriod_check) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_fixedTimePeriod = NULL;
	
	if ((j_fixedTimePeriod = cJSON_Parse(SQW_jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		//
		// Test target: wrong JSON data
		// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
		// 	code
		//
		err = testData_fixedTimePeriod_check(j_fixedTimePeriod);
		ASSERT_TRUE (WERROR_ISWARNING(err));

		free(j_fixedTimePeriod);
		j_fixedTimePeriod = NULL;
		
		if ((j_fixedTimePeriod = cJSON_Parse(FTP_jsonMsg)) == NULL) {
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
		}
	}

	return;
}

TEST (T5, testDataCompiler_generate) {
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;
	
	// JSON messages creation...
	if ((jmsg = cJSON_Parse(jsonMsg)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testDataCompiler_generate(jmsg);
		ASSERT_TRUE ((WERROR_ISERROR(err) == false));
	}

	return;
}

TEST (T6, testDataCompiler_getParams) {
	wError     err = WERROR_SUCCESS;
	configDB_t cdata; 
		
	err = testDataCompiler_getParams(&cdata);
	ASSERT_TRUE ((WERROR_ISERROR(err) == false) && cdata.freq == 5);
	
	return;
}

int main() {
	T1__testDataCompiler_init();
	T2__testData_squareWave_check();
	T3__testData_number_check();
	T4__testData_fixedTimePeriod_check();
	T5__testDataCompiler_generate();
	T6__testDataCompiler_getParams();
	
	return(0);
}
