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
//		In order to obtain a software module opens to extensions and close to changes, the module's architecture is based
//		on the Fabric DP. It means you can develop new sub-modules without to change one line of this software. The new
//		sub-modules will allows you to manage new test-data definitions.
//
//		Usually, every test-client software output-generator tools (eg. keypad, clock...) has its own manager that is
//		implemented by a testDataCompiler sub-module
//
//		UML scheme:
//		===========
//		                       +----------------------------+
//		                       |      testDataCompiler      |
//		                       +----------------------------+
//		                       |                            |
//		                       |                            |
//                                 | testDataCompiler_init()    |
//		                       | testDataCompiler_generate()|
//		                       | testDataCompiler_register()|
//		                       +----------------------------+
//		                          ^           ^         ^
//		                          |           |         |
//		         +----------------+           +---+     +------------------+
//		         |                                |                        |
//		+--------------------------+      +----------------------+      +-------------------+
//		|      fixedTimePeriod     |      |      squareWave      |      |       number      |
//		+--------------------------+      +----------------------+      +-------------------+
//		|                          |      |                      |      |                   |
//		|fixedTimePeriod_init()    |      | squareWave_init()    |      | number_init()     |    ....
//		|fixedTimePeriod_check()   |      | squareWave_check()   |      | number_check()    |
//		|fixedTimePeriod_generate()+--+   | squareWave_generate()|  +---+ number_generate() |
//		+--------------------------+  |   +------+---------------+  |   +-------------------+
//		                              |          |                  |
//		                              +-----+    |     +------------+
//		                                    |    |     |
//		                                 +-------------------------++
//		                                 |           SRAM           |
//		                                 +-------------------------++
//		
//		How to write sub-modules:
//		=========================
//		In order to allow the fabric to manage your custom module in a correct way. The module must have the following 
//		functions:
//			1) <module-name>_init()
//			2) <module-name>_check()
//			3) <module-name>_generate
//
//		The first one initialize itself and it is used by the Fabric to register the other two methods in its own db
//		The second is used by the Fabric to understand if the JSON message is compatible with that sub-module.
//		It this function will return a success code, then the last one will be used by the Fabric to generate the test-data
//		in the SRAM.
//
//		All sub-module file-names must respect the following syntax: testData_<module-name>.[ch]
//
//
//		Configurable symbols:
//		=====================
//		TDC_MAXSUBMODS   Maximum number of sub-modules
//		TDC_SRAMSIZE     Max SRAM size
//		TDC_NUMOFPINS    Number of HILO's digital pins
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

#include <stdint.h>
#include <wError.h>
#include <cjson/cJSON.h>

#define TDC_MAXSUBMODS 8
#define TDC_SRAMSIZE   8388608
#define TDC_NUMOFPINS  16

#ifdef MOCK
#define UTFILE "/tmp/testDataCompiler.foo"
#endif

// Test's configuration data
typedef struct {
	float freq;
} configDB_t;

// Sub-module's object methods
typedef wError (*tdc_check)   (const cJSON *message);
typedef wError (*tdc_generate)(const cJSON *message);

typedef enum {
	TDC_ANDOP,
	TDC_OROP,
	TDC_SWAP,
	TDC_OWROP
} tdcLogicOperator_t;

//
// Public functions
//
wError testDataCompiler_init      ();
wError testDataCompiler_generate  (const cJSON *message);
wError testDataCompiler_register  (tdc_check f, tdc_generate g);
wError testDataCompiler_clean     ();

//
// Protected functions
//	[!] The following functions should be used just by the sub-modules
//
wError testDataCompiler_write     (uint16_t data, uint32_t addr, tdcLogicOperator_t wrMode);
wError testDataCompiler_read      (uint16_t *data, uint32_t addr);
wError testDataCompiler_getParams (configDB_t *cdata);

#endif
