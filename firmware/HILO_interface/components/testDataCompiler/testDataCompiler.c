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

wError testDataCompiler_generate (cJSON *message) {
	//
	// Description:
	//	This function accepts a test-data definition as argument, and sends it to the sub-module able to manage it.
	//	The sub-module will write the raw test-data in the SRAM memory
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_ERROR_ITEMNOTFOUND
	//	WERROR_ERROR_INTFAILURE
	//
	wError  err = WERROR_SUCCESS;
	uint8_t x = 0;
	bool    found = false;

	while (found == false && x < TDC_MAXSUBMODS) {
		if (db[x].check(message) == WERROR_SUCCESS)
			found = true;
		else
			x++;
	}
	if (found) {
		err = db[x].generate(message);
		if (WERROR_ISERROR(err))
			// ERROR!
			err = WERROR_ERROR_INTFAILURE;
			
	} else
		// ERROR!
		err = WERROR_ERROR_ITEMNOTFOUND;
		
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

wError testDataCompiler_setParams (cJSON *configMessage) {
	//
	// Description:
	//	It sets the configuration data used by the sub-modules to calculate the testing-data based on the given definition
	//
	wError err = WERROR_SUCCESS;

	return(err);
}


wError testDataCompiler_clean() {
	//
	// Description:
	//	It cleans the whole SRAM content
	//
	wError err = WERROR_SUCCESS;

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
	wError err = WERROR_SUCCESS;
#ifdef MOCK
#else
#endif
	return(err);
}

wError testDataCompiler_getParams (cJSON *configMessage) {
	//
	// Description:
	//	It writes the received configuration parameters on the argument defined struct
	//
	wError err = WERROR_SUCCESS;

	return(err);
}
