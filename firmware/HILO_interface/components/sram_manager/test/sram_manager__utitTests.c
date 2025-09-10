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
#include <debugTools.h>
#include <wError.h>
#include <sram_manager.h>
#include <sram_manager__mockedFunctions.h>

/*
 wError sramManager_write          (ssRecord rec);
wError sramManager_read           (ssRecord *rec);
wError sramManager_resetNOIfiled  (ssRecord *oldNOI);
wError sramManager_updateNOIfiled ();
*/

// The following offset allow you to check for the correct numeriuc (little-endian/big-endian) interpretation
#define FOODATA_OFFSET 0x0f1e2d3c4b5a6978
#define FOODATA_STEP   3
#define FOODATA_T1NOI  128

TEST(sram_manage, sramManager_write__T1) {
	//
	// Description:
	//	The target of this test is to write 128 progressive numbers into the first virtual chip and verify the file content
	//
	wError   err  = WERROR_SUCCESS;
	ssRecord data = FOODATA_OFFSET;

	for (uint8_t t = 0; t < FOODATA_T1NOI; t++) {
 		err = sramManager_write(data);
		if (WERROR_ISERROR(err))
			break;
		else {
			data += FOODATA_STEP;
			printf("\r> %d/%d", (t+1), FOODATA_T1NOI);
		}
	}
	printf("\n");

	ASSERT_EQ (err, WERROR_SUCCESS);

	return;
}

int main() {
	sramManager_write__T1();

	testEnd();

	return(0);
}
