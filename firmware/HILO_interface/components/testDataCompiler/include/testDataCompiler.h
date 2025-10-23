/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   testDataCompiler.h
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module accepts a particular JSON message as input and write the rendered data into the sram.
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
//
------------------------------------------------------------------------------------------------------------------------------*/

#ifndef __TESTDATACOMPILER__
#define __TESTDATACOMPILER__
#include <wError.h>

typedef (testDataCompiler_check*)(JSON);

wError testDataCompiler_init     ();
wError testDataCompiler_generate (JSON message);
wError testDataCompiler_register (testDataCompiler_check* f);

#endif
