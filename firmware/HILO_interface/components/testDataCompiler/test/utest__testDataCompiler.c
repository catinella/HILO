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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


char *jsonMsgFxp1 = "{                                   \
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
		}                                            \
	]                                                  \
}";

char *jsonMsgFxp2 = "{                                   \
	\"configuration\": {                               \
		\"freq\": 5                                  \
	},                                                 \
	\"test_outputData\": [                             \
		{                                            \
			\"type\":      \"fixedTimePeriod\",    \
			\"pin\":       5,                      \
			\"value\":     \"high\",               \
			\"start\":     0,                      \
			\"stop\":      30                      \
		}                                            \
	]                                                  \
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


TEST (T1, testDataCompiler_generate) {
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;

	testDataCompiler_clean();
	
	//
	// Simple test for fixedTimePeriod() module
	//
	if ((jmsg = cJSON_Parse(jsonMsgFxp1)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testDataCompiler_generate(jmsg);
		ASSERT_TRUE ((WERROR_ISERROR(err) == false));

		if (WERROR_ISERROR(err) == false) {
			//
			// Checking for the virtual-SRAM content
			//	Because the sampling rate is 5Khz, the period is 0.2ms.
			//	In the test definition the bit number 5 will be HIGH only after 10ms. It means after (10/0.2) 50 samples.
			//	The signal must return LOW after 15ms from the test beginning. It means it must stay HIGH for 25 samples.
			//

			FILE     *fh    = NULL;
			uint16_t buffer = 0;
			
			if ((fh = fopen(UTFILE, "r")) == NULL) {
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
				ERRORBANNER (err);
				fprintf(stderr, "ERROR! I cannot open the virtual-SRAM (%s)\n", UTFILE);

			} else if (fseek(fh, 0, SEEK_SET) < 0) {
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
				ERRORBANNER (err);
				fprintf(stderr, "ERROR! I cannot index the virtual-SRAM\n");

			} else {
				uint8_t st = 0;
				uint16_t t = 0;
				
				while (WERROR_ISERROR(err) == false) {
					if      (t == 50)   st = 1;
					else if (t == 75)   st = 2;
					else if (t == 1024) break;
					
					if (fread(&buffer, sizeof(buffer), 1, fh) != 1) {
						// ERROR!
						err = WERROR_ERRUTEST_IOERROR;
						ERRORBANNER (err);
						fprintf(stderr, "ERROR! I cannot read the virtual-SRAM\n");
						break;
	
					} else if (buffer != 0 && st == 0) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
						break;

					} else if (buffer != 0x20 && st == 1) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
						break;
					
					} else if (buffer != 0 && st == 2) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
						break;
					}

					t++;
				}
				
				ASSERT_TRUE ((WERROR_ISERROR(err) == false));
			}
		}
	}

	return;
}


TEST (T2, testDataCompiler_generate) {
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;

	testDataCompiler_clean();
	
	//
	// Simple test for fixedTimePeriod() module
	//
	if ((jmsg = cJSON_Parse(jsonMsgFxp2)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testDataCompiler_generate(jmsg);
		ASSERT_TRUE ((WERROR_ISERROR(err) == false));

		if (WERROR_ISERROR(err) == false) {
			//
			// Checking for the virtual-SRAM content
			//	Because the sampling rate is 5Khz, the period is 0.2ms.
			//	In the test definition the bit number 5 will be HIGH for 30ms by the beginning.
			//	It means after (30/0.2) 60 samples.
			//	The signal must return LOW after that time
			//

			FILE     *fh    = NULL;
			uint16_t buffer = 0;
			
			if ((fh = fopen(UTFILE, "r")) == NULL) {
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
				ERRORBANNER (err);
				fprintf(stderr, "ERROR! I cannot open the virtual-SRAM (%s)\n", UTFILE);

			} else if (fseek(fh, 0, SEEK_SET) < 0) {
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
				ERRORBANNER (err);
				fprintf(stderr, "ERROR! I cannot index the virtual-SRAM\n");

			} else {
				uint8_t st = 0;
				uint16_t t = 0;
				
				while (WERROR_ISERROR(err) == false) {
					if      (t == 60)   st = 1;
					else if (t == 1024) break;
					
					if (fread(&buffer, sizeof(buffer), 1, fh) != 1) {
						// ERROR!
						err = WERROR_ERRUTEST_IOERROR;
						ERRORBANNER (err);
						fprintf(stderr, "ERROR! I cannot read the virtual-SRAM\n");
						break;
	
					} else if (buffer != 0x20 && st == 0) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
						break;

					} else if (buffer != 0 && st == 1) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
						break;
					}

					t++;
				}
				
				ASSERT_TRUE ((WERROR_ISERROR(err) == false));
			}
		}
	}

	return;
}

TEST (T1, testDataCompiler_getParams) {
	wError     err = WERROR_SUCCESS;
	configDB_t cdata; 
		
	err = testDataCompiler_getParams(&cdata);
	ASSERT_TRUE ((WERROR_ISERROR(err) == false) && cdata.freq == 5);
	
	return;
}

int main() {
	T1__testDataCompiler_init();
	T1__testDataCompiler_generate();
	T2__testDataCompiler_generate();
	T1__testDataCompiler_getParams();
	
	return(0);
}
