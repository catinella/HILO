/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   testDataCompiler.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module accepts a particular JSON message as input and write the rendered data into the sram.
//		In order to obtain a software module opens to extensions and close to changes, the module's architecture is based
//		on the Fabric DP. For the UML and more details, please, read the testDataCompiler.h file
//
//		Self-generated source-code files:
//		=================================
//			testDataCompiler_subModsHeaders.h
//				It is a list of "#include" statements. It allows you to include all header-files of all sub-modules,
//				in one single step, without to change the super-class source code
//
//			testDataCompiler_subModsRegCalls.dc
//				It is the magic of the meta-programming approch to the Fabric DP.
//				This file contains a list of the sub-modules registering calls. Executing this dinamically self created
//				code, this super-class will know how to handle every its sub-module
//		
//		
// License:  LGPL ver 3.0
//
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
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
//
------------------------------------------------------------------------------------------------------------------------------*/
#include <debugTools.h>
#include <testDataCompiler.h>
#include <testDataCompiler_subModsHeaders.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef MOCK

// X86 libraries
#include <stdio.h>
#include <string.h>
#include <math.h>

#endif

typedef struct {
	tdc_check    check;
	tdc_generate generate;
} smDbItems;


static smDbItems db[TDC_MAXSUBMODS];
static uint8_t  db_index = 0;

#ifdef MOCK
static FILE* fh = NULL;
#endif

static configDB_t confDB;
static bool       confDB_flag = false;

//------------------------------------------------------------------------------------------------------------------------------
//                                    P R I V A T E   F U N C T I O N S 
//------------------------------------------------------------------------------------------------------------------------------
static wError _setParams (const cJSON *configMessage) {
	//
	// Description:
	//	It accepts a JSON message as argument and tores the "configuration" field' data in the module's static database.
	//	The sub-modules will be able to retrive the data using the testDataCompiler_getParams() function, later. 
	//
	wError err = WERROR_SUCCESS;
	cJSON  *freq = NULL;
	
	if (
		(freq = cJSON_GetObjectItem(configMessage, "freq")) == NULL  ||
		cJSON_IsNumber(freq)                                == false ||
		freq->valuedouble                                   == 0
	) {
		// ERROR!  "freq" field is mandatory
		err = WERROR_ERROR_ILLEGALSYNTAX;
	
	} else 
		confDB.freq = freq->valuedouble;
		
	return(err);
}

static wError _getPin (uint16_t word, uint8_t pos, bool *value) {
	//
	// Description:
	//	This function accepts a bits sequence (word) and read the argument defined pin's value 
	//
	wError err = WERROR_SUCCESS;
	if (value == NULL)
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;
		
	else
		*value = ((1 << pos) & word) > 0 ? 1 : 0;
		
	return(err);
}

static wError _setPin (uint16_t *word, uint8_t pos, bool value)  {
	//
	// Description:
	//	This function accepts a bits sequence (word) and overwrite the argument defined pin with the given value 
	//
	wError err = WERROR_SUCCESS;

	if (word == NULL)
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;
		
	else {
		if (value == 1)
			// Bit setting
			*word |= (1 << pos);
		else
			// Bit resetting
			*word &= ~(1 << pos);
	}
		
	return(err);
}

static wError _swapPin (uint16_t *word, uint8_t pos) {
	//
	// Description:
	//	This function accepts a bits sequence (word) and swap the value of the argument defined pin 
	//
	wError err = WERROR_SUCCESS;

	if (word == NULL)
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;
		
	else {
		bool op;
		if ((err = _getPin (*word, pos, &op)) == WERROR_SUCCESS)
			err = _setPin (word, pos, op ? 0 : 1);
	}
	
	return(err);
}


//------------------------------------------------------------------------------------------------------------------------------
//                                      P U B L I C   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------
wError testDataCompiler_init () {
	//
	// Description:
	//	This function initializes the SRAM devices and registers all available sub-modules
	//
	wError err = WERROR_SUCCESS;

	// Internal db initialization
	for (uint8_t t=0; t<TDC_MAXSUBMODS; t++) {
		(db[t]).check    = NULL;
		(db[t]).generate = NULL;
	}

	//
	// SRAM initialization procedure
	//
#ifdef MOCK
	if ((fh = fopen(UTFILE, "w+")) == NULL) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot open the virtual-memory \"%s\" file\n", UTFILE);
	}
#else
	// TODO: real procedure
#endif

	// SRAM content ereasing...
	if (WERROR_ISERROR(err) == false)
		err = testDataCompiler_clean();

	// Sub-modules registration
	if (WERROR_ISERROR(err) == false) {
		#include <testDataCompiler_subModsRegCalls.dc>
	}

	return(err);
}

wError testDataCompiler_generate (const cJSON *message) {
	//
	// Description:
	//	This function accepts a test-data definition as argument, and sends it to the sub-module able to manage it.
	//	The sub-module will write the raw test-data in the SRAM memory
	//
	// Returned value:
	//	WERROR_SUCCESS              // All definitions have been converted in raw data
	//	WERROR_WARNING_EMPTYLIST    // No test-data definition found
	//	WERROR_ERROR_ITEMNOTFOUND   // No manager module for the specified test-data definition
	//	WERROR_ERROR_INTFAILURE     // The manager returned a critical error
	//
	wError  err = WERROR_SUCCESS;
	cJSON   *jConfiguration = NULL, *jTest_outputData = NULL;
	
	// Checking for configuration session
	if (
		confDB_flag == false                                                      &&
		(jConfiguration = cJSON_GetObjectItem(message, "configuration")) != NULL  &&
		cJSON_IsObject(jConfiguration)                                            &&
		WERROR_ISERROR(_setParams(jConfiguration)) == false
	)
		// Module configured
		confDB_flag = true;
		
		
	// Checking for test_outputData
	if (
		(jTest_outputData = cJSON_GetObjectItem(message, "test_outputData")) != NULL  &&
		cJSON_IsArray(jTest_outputData)
	) {
		cJSON   *item = NULL;
		uint8_t x;
		bool    found;
		
		int arraySize = cJSON_GetArraySize(jTest_outputData);
		
		if (arraySize == 0)
			// WARNING!
			err = WERROR_WARNING_EMPTYLIST;
		
		else {
			for (uint8_t i = 0; i < arraySize; i++) {
				item  = cJSON_GetArrayItem(jTest_outputData, i);
				found = false;
				x = 0;
				
				// Looking for the data-type manager
				while (found == false && x < db_index) {
					if (db[x].check(item) == WERROR_SUCCESS)
						found = true;
					else
						x++;
				}
		
				if (found) {
					err = db[x].generate(item);
					if (WERROR_ISERROR(err)) {
						// ERROR!
						err = WERROR_ERROR_INTFAILURE;
						break;
					}
				} else {
					// ERROR!
					err = WERROR_ERROR_ITEMNOTFOUND;
					break;
				}
			}
		}
	}
	return(err);
}

wError testDataCompiler_register (tdc_check f, tdc_generate g) {
	//
	// Description:
	//	It registers the sub.module's methods defined as arguments
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_ERROR_ILLEGALARG
	//
	wError err = WERROR_SUCCESS;

	if (f != NULL && g != NULL) {
		(db[db_index]).check = f;
		(db[db_index]).generate = g;
		db_index++;
	} else
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;
		
	return(err);
}


wError testDataCompiler_clean() {
	//
	// Description:
	//	It cleans the whole SRAM content
	//
	wError err = WERROR_SUCCESS;

	// All test params will be disabled
	confDB_flag = false;
	
#ifdef MOCK
	uint8_t zeroPool[1024];
	memset(zeroPool, 0, 1024);

	// File rewind
	if (fseek(fh, 0, SEEK_SET) < 0) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot reset the virtual-memory pointer\n");
		
	} else {
		for (uint16_t t = 0; t < trunc(TDC_SRAMSIZE/sizeof(zeroPool)); t++) {
			if (fwrite(zeroPool, 1, sizeof(zeroPool), fh) < sizeof(zeroPool)) {
				// ERROR!
				err = WERROR_ERRUTEST_IOERROR;
				ERRORBANNER(err);
				fprintf(stderr, "ERROR! I cannot write in the virtual-memory\n");
				break;
			}
			printf("\r> %d", t);
		}
		printf("\n");

		fflush(fh);
	} 
#else
	// TODO: SRAM content cleaning...
#endif
	return(err);
}

//------------------------------------------------------------------------------------------------------------------------------
//                                      P R O T E C T E D   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------

wError testDataCompiler_write (uint16_t data, uint32_t addr, tdcLogicOperator_t wrMode) {
	//
	// Description:
	//	It writes the received configuration parameters on the argument defined struct
	//
	//	[!] TDC_SWAP: When you select swap mode, the argument "data" will be analized and for every bit set to 1, the
	//	    respective bit in the original configuration will be swapped
	//
	// Arguments:
	//	data    Bit configuration to store in the SRAM
	//	addr    The item position in the memory (NOT its address in bytes)
	//	wrMode  How to merge the new data {TDC_ANDOP | TDC_OROP | TDC_SWAP}
	//
	wError   err = WERROR_SUCCESS;
	uint16_t tData = 0;

	// Chunk position to bytes offset
	addr = addr * sizeof(uint16_t);
	
#ifdef MOCK
	// Moving to the right position
	if (fseek(fh, addr, SEEK_SET) < 0) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot reset the virtual-memory pointer\n");
	
	// Old data reading...
	} else if (fread((void*)&tData, 1, sizeof(tData), fh) < sizeof(tData)) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot read the virtual-memory\n");
		
	// Returning to the right position
	} else if (fseek(fh, addr, SEEK_SET) < 0) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot reset the virtual-memory pointer\n");

	} else {

		if (wrMode == TDC_SWAP) {
			bool value;

			// Data swapping
			for (uint8_t t = 0; t < (8*sizeof(data)); t++) {
				if ((err = _getPin (data, t, &value)) != WERROR_SUCCESS) {
					// ERROR!
					break;
				
				} else if (value == 1)
					_swapPin(&tData, t);
			}

		} else  if (wrMode == TDC_OROP)
			tData |= data;

		else if (wrMode == TDC_ANDOP)
			tData &= data;

		else
			// Overwriting...
			tData = data;
	
		if (fwrite((void*)&tData, 1, sizeof(tData), fh) < sizeof(tData)) {
			// ERROR!
			err = WERROR_ERRUTEST_IOERROR;
			ERRORBANNER(err);
			fprintf(stderr, "ERROR! I cannot write in the virtual-memory\n");
		}
	}
	
	fflush(fh);
#else
#endif
	return(err);
}

wError testDataCompiler_getParams (configDB_t *cdata) {
	//
	// Description:
	//	It writes the received configuration parameters on the argument defined struct
	//
	// Returned value:
	//	WERROR_SUCCESS             // The config values have been correctly read
	//	WERROR_WARNING_RESNOTAVAIL // The module has not yet been configured
	//	WERROR_ERROR_ILLEGALARG    // NULL argument is not allowed
	//
	wError err = WERROR_SUCCESS;
	
	if (cdata == NULL) 
		// ERROR!
		err = WERROR_ERROR_ILLEGALARG;
	
	else if (confDB_flag == false)
		// WARNING
		err = WERROR_WARNING_RESNOTAVAIL;
		
	else
		// SUCCESS
		*cdata = confDB;
		
	return(err);
}

wError testDataCompiler_read (uint16_t *data, uint32_t addr) {
	//
	// Description:
	//	It writes the received configuration parameters on the argument defined struct
	//
	// Returned value:
	//	WERROR_SUCCESS
	// ---mocking mode---
	//	WERROR_ERRUTEST_IOERROR
	//	WERROR_ERRUTEST_IOERROR
	//
	wError err = WERROR_SUCCESS;
	addr = addr * sizeof(uint16_t);

#ifdef MOCK
	if (fseek(fh, addr, SEEK_SET) < 0) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot reset the virtual-memory pointer\n");
	
	} else if (fread((void*)data, 1, sizeof(data), fh) < sizeof(data)) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;
		ERRORBANNER(err);
		fprintf(stderr, "ERROR! I cannot write in the virtual-memory\n");
	}

#else
	// TODO: data reading from the SRAM
#endif
	return(err);
}
