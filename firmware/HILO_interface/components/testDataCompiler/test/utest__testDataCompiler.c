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
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


char *jsonMsgCC = "{                                     \
	\"configuration\": {                               \
		\"freq\": 50                                 \
	},                                                 \
	\"test_outputData\": []                            \
}";


char *jsonMsgFxp1 = "{                                   \
	\"configuration\": {                               \
		\"freq\": 50                                 \
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
		\"freq\": 50                                 \
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


char *jsonMsgFxp3 = "{                                   \
	\"configuration\": {                               \
		\"freq\": 50                                 \
	},                                                 \
	\"test_outputData\": [                             \
		{                                            \
			\"type\":      \"fixedTimePeriod\",    \
			\"pin\":       5,                      \
			\"value\":     \"swap\",               \
			\"start\":     10,                     \
			\"stop\":      16                      \
		}                                            \
	]                                                  \
}";


char *jsonMsgNum1 = "{                                   \
	\"configuration\": {                               \
		\"freq\": 50                                 \
	},                                                 \
	\"test_outputData\": [                             \
		{                                            \
			\"type\":      \"number\",             \
			\"pins\":      [8,9,10,11,12,13,14,15],\
			\"value\":     202,                    \
			\"start\":     20,                     \
			\"stop\":      40                      \
		}                                            \
	]                                                  \
}";


char *jsonMsgSQW = "{                                    \
	\"configuration\": {                               \
		\"freq\": 50                                 \
	},                                                 \
	\"test_outputData\": [                             \
		{                                            \
			\"type\":      \"squareWave\",         \
			\"pin\":       7,                      \
			\"period\":    10,                     \
			\"dutyCycle\": 50,                     \
			\"start\":     10,                     \
			\"stop\":      100                     \
		}                                            \
	]                                                  \
}";


char *jsonMsg = "{                                       \
	\"configuration\": {                               \
		\"freq\": 50                                 \
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
	//
	// Description:
	//	This is a test for the fabric functionality of the testDataCompiler super-class. The procedure load all sub-modules
	//	references in the super-class internal DB
	//	If the test fails then at least one sub-module registartion procedure failed
	//
	wError err = WERROR_SUCCESS;
	
	// All sub-modules initialization...
	err = testDataCompiler_init();
	ASSERT_EQ (WERROR_SUCCESS, err);

	return;
}


TEST (T1, testDataCompiler_getParams) {
	//
	// Description:
	//	This procedure check for the function used by the sub-modules to get the active configuration
	//
	wError     err = WERROR_SUCCESS;
	configDB_t cdata; 
	cJSON      *jmsg = NULL;

	// It reset the configuration data too	
	testDataCompiler_clean();

	// When you try to get the config's values but the db is empty, then you should get a warning
	err = testDataCompiler_getParams(&cdata);
	ASSERT_TRUE ((WERROR_ISWARNING(err) == true));
	
	if ((jmsg = cJSON_Parse(jsonMsgCC)) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_CORRUPTDATA;
		ERRORBANNER (err);
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testDataCompiler_generate(jmsg);
		ASSERT_TRUE ((WERROR_ISERROR(err) == false));

		err = testDataCompiler_getParams(&cdata);
		ASSERT_TRUE ((WERROR_ISERROR(err) == false) && cdata.freq == 50);
	}
	
	return;
}


TEST (T1, testDataCompiler_generate) {
	//
	// Description:
	//	This is the simplest test to fixedTimePeriod. It just sets the defined pin to one for a short period.
	//
	
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;

	testDataCompiler_clean();
	
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
	//
	// Description:
	//	It is another easy test for the fixedTimePeriod sub-module. It sets tthe defined bit to one for a longer time
	//	This test has been created to prepare the virtual SRAM's content for the next test, mainly.
	// 
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;

	testDataCompiler_clean();
	
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
			//	It means after (30/0.2) 150 samples.
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
					if      (t == 150)   st = 1;
					else if (t == 1024) break;
					
					if (fread(&buffer, sizeof(buffer), 1, fh) != 1) {
						// ERROR!
						err = WERROR_ERRUTEST_IOERROR;
						ERRORBANNER (err);
						fprintf(stderr, "ERROR! I cannot read the virtual-SRAM\n");
	
					} else if (buffer != 0x20 && st == 0) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;

					} else if (buffer != 0 && st == 1) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
					}

					t++;
				}
				
				ASSERT_TRUE ((WERROR_ISERROR(err) == false));
			}
		}
	}

	return;
}


TEST (T3, testDataCompiler_generate) {
	//
	// Description:
	//	It is a bit more complicated test for fixedTimePeriod sub-module. This procedure reqires the
	//	T2::testDataCompiler_generate test has been executed to fill virtual SRAM correctly.
	//    In this test, thecurrect defined pin's value will be swapped.
	//
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;
	
	//
	// Simple test for fixedTimePeriod() module
	//
	if ((jmsg = cJSON_Parse(jsonMsgFxp3)) == NULL) {
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
			//	In the test definition the bit number 5 will be swapped for 6ms after 10ms by the test beginning.
			//
			//	The defined by JSON-recipit number is 202 and the used bits sequence starts by pin-8
			//
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
					else if (t == 80)   st = 2;
					else if (t == 150)  st = 3;
					else if (t == 1024) break;
					
					if (fread(&buffer, sizeof(buffer), 1, fh) != 1) {
						// ERROR!
						err = WERROR_ERRUTEST_IOERROR;
						ERRORBANNER (err);
						fprintf(stderr, "ERROR! I cannot read the virtual-SRAM\n");
	
					} else if (buffer != 0x20 && st == 0) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;

					} else if (buffer != 0 && st == 1) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;

					} else if (buffer != 0x20 && st == 2) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;

					} else if (buffer != 0 && st == 3) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
					}

					t++;
				}
				
				ASSERT_TRUE ((WERROR_ISERROR(err) == false));
			}
		}
	}

	return;
}

TEST (T4, testDataCompiler_generate) {
	//
	// Description:
	//	It is an easy test for the testData_number sub-module.
	//	It simulate a key-pad where for a given period you still push the defined number
	// 
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;

	testDataCompiler_clean();
	
	if ((jmsg = cJSON_Parse(jsonMsgNum1)) == NULL) {
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
			//	Because the sampling rate (freq:50) is 5Khz, the period is 0.2ms.
			//	In the JSON test recipit, the number must be present on the HILO's output pins, after 20ms, and it
			//	doesn't have to change for other 20ms. It means the number should be recorded in virtual SRAM after 100
			//	records and it should be written in the next 100 records. 
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
				uint16_t expData = 202 << 8;
				
				while (WERROR_ISERROR(err) == false) {
					if      (t == 100)   st = 1;
					else if (t == 200)   st = 2;
					else if (t == 1024) break;
					
					if (fread(&buffer, sizeof(buffer), 1, fh) != 1) {
						// ERROR!
						err = WERROR_ERRUTEST_IOERROR;
						ERRORBANNER (err);
						fprintf(stderr, "ERROR! I cannot read the virtual-SRAM\n");
	
					} else if (buffer != 0x00 && st == 0) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;

					} else if (buffer != expData && st == 1) {
						// TEST FAILED
						err = WERROR_ERRUTEST_TESTFAILED;
					}
					t++;
				}

				ASSERT_TRUE ((WERROR_ISERROR(err) == false));
			}
		}
	}

	return;
}

TEST (T5, testDataCompiler_generate) {
	//
	// Description:
	//	It is an easy test for the testData_squareWave sub-module.
	// 
	wError err = WERROR_SUCCESS;
	cJSON  *jmsg = NULL;

	testDataCompiler_clean();
	
	if ((jmsg = cJSON_Parse(jsonMsgSQW)) == NULL) {
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
//------------------------------------------------------------------------------------------------------------------------------
//                                                       M A I N
//------------------------------------------------------------------------------------------------------------------------------

int main() {
	T1__testDataCompiler_init();
	T1__testDataCompiler_getParams();
	T1__testDataCompiler_generate();
	T2__testDataCompiler_generate();
	T3__testDataCompiler_generate();
	T4__testDataCompiler_generate();
	T5__testDataCompiler_generate();
	
	return(0);
}
