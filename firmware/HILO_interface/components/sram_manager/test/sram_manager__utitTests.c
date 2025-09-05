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
/*
 wError sramManager_write          (ssRecord rec);
wError sramManager_read           (ssRecord *rec);
wError sramManager_resetNOIfiled  (ssRecord *oldNOI);
wError sramManager_updateNOIfiled ();
*/

// The following offset allow you to check for the correct numeriuc (little-endian/big-endian) interpretation
#define FOODATAOFFSET 0x0f1e2d3c4b5a6978
#define FOODATASTEP   3

TEST(sram_manage, sramManager_write_1) {
	//
	// Description:
	//	The target of this test is to write 128 progressive numbers into the first virtual chip and verify the file content
	//
	wError   err  = WERROR_SUCCESS;
	ssRecord data = FOODATAOFFSET;
	for (uint8_t t = 0; t < 128; t++) {
 		err = sramManager_write(data);
		data += FOODATASTEP;
		if (WERROR_ISERROR(err)) break;
	}
	ASSERT_EQ (err, WERROR_SUCCESS);

	return;
}


int main() {
	sramManager_write_1();
	return(0);
}
