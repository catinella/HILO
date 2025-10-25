/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:  testData_squareWave.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module convert the test-data definition in a stream of bits.
//		In this definition type, user can set a digital wave as test-data
//
//		The key to select this module is "type = squareWave"
//
//		JSON message = {
//			type      = squareWave,
//			pin       = <0-15>,
//			period    = <n>,         # milli seconds
//			dutyCycle = <0 - 100>    # percent
//		}
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

#include <testDataCompiler.h>

wError squareWave_init() {
	//
	// Description:
	//	This function registers the _check() and _generate() methods in the parent class testDataCompiler
	//
	wError err = WERROR_SUCCESS;


	return(err);
}

wError squareWave_check (JSON message) {
	//
	// Description:
	//	This function accepts a JSON message as arguments and returns a success value only if the message can be handled
	//	by this module
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_WARNING_MISSMATCHTYPE
	//
	wError err = WERROR_SUCCESS;


	return(err);
}

wError squareWave_generate() {
	//
	// Description:
	//	This generates the test data stream
	//
	wError err = WERROR_SUCCESS;


	return(err);
}
