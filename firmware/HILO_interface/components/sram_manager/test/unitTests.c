/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
//
// File:   sram_manager__utitTests.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	Unit tests for the sram_manager module
//
//
------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <debugTools.h>
#include <wError.h>
#include <sram_manager.h>
#include <mockedFunctions.h>

/*
wError sramManager_write          (ssRecord rec);
wError sramManager_read           (ssRecord *rec);
wError sramManager_resetNOIfiled  (ssRecord *oldNOI);
wError sramManager_updateNOIfiled ();
*/

// The following offset allow you to check for the correct numeriuc (little-endian/big-endian) interpretation
//#define FOODATA_OFFSET 0x0f1e2d3c4b5a6978
#define FOODATA_OFFSET 0xff
#define FOODATA_STEP   3
#define FOODATA_T1NOI  128

#ifndef VERBOSE_UNITTEST
#define VERBOSE_UNITTEST 1
#endif

void fooDataGenerating (ssRecord *ds, uint32_t noi) {
	//
	// Description:
	//	It generates foo data for the tests
	//
	
	for (uint32_t t = 0; t < noi; t++)
		ds[t] = (ssRecord)(FOODATA_OFFSET + FOODATA_STEP * t);
	
	printf("[*] %d values have been generated\n", noi);
	
	return;
}


wError dataWriting (const ssRecord *ds, uint32_t noi) {
	//
	// Description:
	//	It writes the argumen defined data set into the virtual SRAM
	//
	wError err = WERROR_SUCCESS;
	
	for (uint32_t t = 0; t < noi; t++) {
 		if ((err = sramManager_write(ds[t])) && WERROR_ISERROR(err))
			break;
		else {
#if VERBOSE_UNITTEST == 1
			printf("\rWriting: %d/%d", (t+1), noi);
#elif VERBOSE_UNITTEST == 2
			printf("%d: %ld\n", t, ds[t]);
#endif
		}
	}
#if VERBOSE_UNITTEST > 0
	printf("\n");
#endif

	return(err);
}


wError dataReading (ssRecord *ds, uint32_t noi) {
	//
	// Description:
	//	It reads data from the virtual SRAM
	//
	wError err = WERROR_SUCCESS;
	
	for (uint32_t t = 0; t < noi; t++) {
 		if ((err = sramManager_read((ds+t))) && WERROR_ISERROR(err))
			break;
		else {
#if VERBOSE_UNITTEST == 1
			printf("\rReading: %d/%d", (t+1), noi);
#endif
		}
	}
	if (WERROR_ISERROR(err) == false) {
#if VERBOSE_UNITTEST == 1
		printf("\n");
#endif
	}
	return(err);
}

//------------------------------------------------------------------------------------------------------------------------------
//                                               U N I T  T E S T S
//------------------------------------------------------------------------------------------------------------------------------

TEST(sram_manage, sramManager_write__T1) {
	//
	// Description:
	//	The target of this test is to write 128 progressive numbers into the first virtual chip and verify the file content
	//
	wError   err  = WERROR_SUCCESS;
	ssRecord srcData[FOODATA_T1NOI];
	ssRecord retData[FOODATA_T1NOI];

	memset(srcData, 0, (FOODATA_T1NOI * sizeof(ssRecord)));
	memset(retData, 0, (FOODATA_T1NOI * sizeof(ssRecord)));

	// Foo-data generating...
	fooDataGenerating(srcData, FOODATA_T1NOI);

	// Data writing...
	err = dataWriting(srcData, FOODATA_T1NOI);
	ASSERT_EQ (err, WERROR_SUCCESS);

	// Data reading...
	if (WERROR_ISERROR(err) == false) {
		err = dataReading(retData, FOODATA_T1NOI);
		ASSERT_EQ (err, WERROR_SUCCESS);
	}

	// Checking for the data content
	if (WERROR_ISERROR(err) == false) {
		for (uint32_t t = 0; t < FOODATA_T1NOI; t++) {
			if (retData[t] != srcData[t]) {
				// Error!
				err = WERROR_ERRUTEST_CORRUPTDATA;
				printf("%d: [%ld != %ld] differ!\n", t, retData[t], srcData[t]);
				break;

			} else {
				// SUCCESS
#if VERBOSE_UNITTEST == 1
				printf("\r%d: [%ld] ok", t, retData[t]);
#endif
			}
				
		}
#if VERBOSE_UNITTEST == 1
		if (WERROR_ISERROR(err) == false)
			printf("\n");
#endif
		ASSERT_EQ (err, WERROR_SUCCESS);
	}
	return;
}

int main() {
	sramManager_write__T1();

	testEnd();

	return(0);
}
