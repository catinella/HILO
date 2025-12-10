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
#include <testData_number.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//
// It is a correct JSON message for this module
//
char *jsonMsg = "{                        \
	\"type\":   \"number\",             \
	\"pins\":   [8,9,10,11,12,13,14,15],\
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

//
// Wrong JSON message: type mismatch
//
char *false_jsonMsg = "{                  \
	\"type\":   \"number312\",          \
	\"pins\":   [8,9,10,11,12,13,14,15],\
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

//
// Wrong JSON message: illegal number of pins
//

// Not enough pins
char *jsonMsgE1 = "{                      \
	\"type\":   \"number\",             \
	\"pins\":   [8,9,10,11,12,13,15],   \
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

// No pins
char *jsonMsgE2 = "{                      \
	\"type\":   \"number\",             \
	\"pins\":   [],                     \
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

// Repeted pins
char *jsonMsgE3 = "{                      \
	\"type\":   \"number\",             \
	\"pins\":   [8,9,10,11,12,13,15,12],\
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";

// Too many pins
char *jsonMsgE4 = "{                      \
	\"type\":   \"number\",             \
	\"pins\":   [6,7,8,9,10,11,12,13,15],\
	\"value\":  255,                    \
	\"start\":  20,                     \
	\"stop\":   40                      \
}";


//------------------------------------------------------------------------------------------------------------------------------
// n
//------------------------------------------------------------------------------------------------------------------------------

TEST (T1, testData_number_check) {
	//
	// Description:
	//	It checks for a wrong type JSON message. 
	// 	For every JSON that does not match with the expected type, the <sub.module>_check() function returns a warning
	// 	code
	//
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;
	
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
	
	return;
}
	
TEST (T2, testData_number_check) {
	//
	// Description:
	//	It checks for a correct JSON message
	//
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;

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

	return;
}
	

TEST (T3, testData_number_check) {
	//
	// Description:
	//	It checks for an incoherent JSON-message where some field is missing
	//	
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;

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


TEST (T4, testData_number_check) {
	//
	// Description:
	//	Wrong JSON message: not enoufgh defined pins
	//	
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;

	if ((j_number = cJSON_Parse(jsonMsgE1)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISERROR(err));
	}
	return;
}


TEST (T5, testData_number_check) {
	//
	// Description:
	//	Wrong JSON message: no defined pins
	//	
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;

	if ((j_number = cJSON_Parse(jsonMsgE2)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISERROR(err));
	}
	return;
}


TEST (T6, testData_number_check) {
	//
	// Description:
	//	Wrong JSON message: repeated pins
	//	
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;

	if ((j_number = cJSON_Parse(jsonMsgE3)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISERROR(err));
	}
	return;
}

TEST (T7, testData_number_check) {
	//
	// Description:
	//	Wrong JSON message: too many defined pins
	//	
	wError err = WERROR_SUCCESS;
	cJSON  *j_number = NULL;

	if ((j_number = cJSON_Parse(jsonMsgE4)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testData_number_check(j_number);
		ASSERT_TRUE (WERROR_ISERROR(err));
	}
	return;
}

int main() {
	T1__testData_number_check();
	T2__testData_number_check();
	T3__testData_number_check();
	T4__testData_number_check();
	T5__testData_number_check();
	T6__testData_number_check();
	T7__testData_number_check();

	return(0);
}
