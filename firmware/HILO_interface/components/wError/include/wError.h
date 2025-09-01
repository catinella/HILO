//------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   wError.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//
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
