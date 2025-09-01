//------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
// File:   wError.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//
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
//                                                                                                               cols=128 tab=6
//------------------------------------------------------------------------------------------------------------------------------
#ifndef __WERROR__
#define __WERROR__

#include <stdint.h>

typedef uint8_t wError;

//
// Border settings
//
#define WERROR_LASTINFO     15
#define WERROR_LASTWARNING  127
#define WERROR_LASTERROR    255

//
// Macros
//
#define WERROR_ISERROR(x)   (x == 0              || x >  WERROR_LASTWARNING)
#define WERROR_ISWARNING(x) (x > WERROR_LASTINFO && x <= WERROR_LASTWARNING)
#define WERROR_ISSUCCESS(x) (x > 0               && x <= WERROR_LASTINFO)


//
// Generic error codes
//
#define WERROR_ERROR_GENERIC         0
#define WERROR_SUCCESS               1
//#define WERROR_WARNING_ALREADYOPEN   16
#define WERROR_ERROR_INITFAILED      128
#define WERROR_ERROR_DATAOVERFLOW    130
#define WERROR_ERROR_SPIBUSERROR     132


#endif
