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
// File:   wError.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//
//
//		//
//		// Success codes
//		//
//		WERROR_SUCCESS                Everything has beed completed correctly
//
//		//
//		// Warning codes
//		//
//		WERROR_WARNING_ALREADYOPEN    Op. completed correctly, but the resource was already open
//
//		//
//		// Error codes
//		//
//		WERROR_ERROR_INITFAILED       Some internal component's initialization failed
//		WERROR_ERROR_DATAOVERFLOW     Data overflow occurred
//		WERROR_ERROR_SPIBUSERROR      Some erro occurred on SPI bus using
//		WERROR_ERROR_ILLEGALOP        Operation was not permitted
//		WERROR_ERROR_ILLEGALARG       Argument malformed or not permitted
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
------------------------------------------------------------------------------------------------------------------------------*/
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



// Success codes
#define WERROR_SUCCESS               1


//
// Warning codes
//
#define WERROR_WARNING_RESBUSY       16
#define WERROR_WARNING_EMPTYLIST     18
#define WERROR_WARNING_TYPEMISSMATCH 20


//
// Error codes
//
#define WERROR_ERROR_GENERIC         0

#define WERROR_ERROR_INITFAILED      128
#define WERROR_ERROR_DATAOVERFLOW    130
#define WERROR_ERROR_SPIBUSERROR     132
#define WERROR_ERROR_ILLEGALOP       134
#define WERROR_ERROR_ILLEGALARG      136
#define WERROR_ERROR_CONFFAILED      138
#define WERROR_ERROR_INTFAILURE      140
#define WERROR_ERROR_REGPROCFAILED   142
#define WERROR_ERROR_ITEMNOTFOUND    144
#define WERROR_ERROR_INTERNALS       146
#define WERROR_ERROR_ILLEGALSYNTAX   148


//
// Error codes for tests
//
#define WERROR_ERRUTEST_IOERROR      201
#define WERROR_ERRUTEST_ILLEGALARG   203
#define WERROR_ERRUTEST_NOTYETIMPL   205
#define WERROR_ERRUTEST_TESTFAILED   207
#define WERROR_ERRUTEST_CORRUPTDATA  209

#endif
     
