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
//	Externally defined symbols:
//	===========================
//		FOODATA_T1NOI
//		VERBOSE_UNITTEST
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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
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

#define CURSOR_ON      printf("\e[?25h");
#define CURSOR_OFF     printf("\e[?25l");

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
			CURSOR_OFF
			printf("\rWriting: %d/%d", (t+1), noi);
			CURSOR_ON
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
			CURSOR_OFF
			printf("\rReading: %d/%d", (t+1), noi);
			CURSOR_ON
#elif VERBOSE_UNITTEST == 2
			printf("%d) %ld\n", (t+1), ds[t]);
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
	ssRecord *srcData = NULL;
	ssRecord *retData = NULL;
	uint32_t memSize = FOODATA_T1NOI * sizeof(ssRecord);
	
	srcData = (ssRecord*)malloc(memSize);
	retData = (ssRecord*)malloc(memSize);
	
	memset(srcData, 0, memSize);
	memset(retData, 0, memSize);

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
				CURSOR_OFF
				printf("\r%d: [%ld] ok", t, retData[t]);
				CURSOR_ON
#endif
			}
				
		}
#if VERBOSE_UNITTEST == 1
		if (WERROR_ISERROR(err) == false)
			printf("\n");
#endif
		ASSERT_EQ (err, WERROR_SUCCESS);
	}

	free(srcData);
	free(retData);
	
	return;
}

int main() {
	sramManager_write__T1();

	testEnd();

	return(0);
}
