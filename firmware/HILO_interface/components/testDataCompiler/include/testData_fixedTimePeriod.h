/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:  testData_fixedTimePeriod.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module convert the test-data definition in a stream of bits.
//		In this definition type, user can set a fixed period where a defined bit must have a constant value.
//		This value can be set as "swap", in this case the new value will be the inverted old one 
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

#ifndef __TD_FIXEDTIMEPERIOD__
#define __TD_FIXEDTIMEPERIOD__

#include <wError.h>
#include <cjson/cJSON.h>

#define TD_FIXEDTIMEPERIOD_KEYWORD "fixedTimePeriod"

wError testData_fixedTimePeriod_init     ();
wError testData_fixedTimePeriod_check    (const cJSON *root);
wError testData_fixedTimePeriod_generate (const cJSON *root);

#endif
