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
// Filename: ringBuffer.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module implemets a special ringBuffer that is resposible for its conrinuose self reliffilng
//
//		
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
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <ringBuffer.h>

static ringBuffer_refillCingB *_refillCingB = NULL;
static ringBuffer_emptyingCB  *_emptyingCB  = NULL;
static uint8_t                initStatus    = 0;

void ringBuffer_set_refillCingB (ringBuffer_refillCingB *refillCingB_a) {
	//
	// Description:
	//	It allows you to set the callback used by the module to refill the ringBuffer. This function MUST be called before
	//	to use the rest of the normal functions
	//
	_refillCingB = refillCingB_a;
	initStatus |= 1;
	return;
}

void ringBuffer_set_emptyingCB (ringBuffer_emptyingCB *emptyingCB_a) {
	//
	// Description:
	//	It allows you to set the callback used by the module to empty the ringBuffer. This function MUST be called before
	//	to use the rest of the normal functions
	//
	_emptyingCB = emptyingCB_a;
	initStatus |= 2;
	return;
}

bool ringBuffer_init (ringBuffer_t *obj, const bitConf_t *storage_a) {
	//
	// Description:
	//	It inirtializes the argument dfined object. All objects of this type, must be initialized before to be used
	//
	bool out = false;
	if (initStatus == 3 && storage_a != NULL) {
		out = true;
		obj->usedSize = 0;
		obj->storage = storage_a;
		obj->index = 0;
	}
	return(out);
}

bool ringBuffer_push (ringBuffer_t *obj, bitConf_t data) {
	//
	// Description:
	//	It push a new data into the ring buffer
	//
	bool out = false;

	return(out);
}

bool ringBuffer_pull (ringBuffer_t *obj, *bitConf_t data) {
	//
	// Description:
	//	It pull a data from the ring buffer
	//
	bool out = false;

	return(out);
}

bool ringBuffer_forcedEmptying (ringBuffer_t *obj) {
	//
	// Description:
	//	It forces the argument defined ringBuffer to be empty. This function is used ad the end of the test process,
	//	usually.
	//
	bool out = false;

	return(out);
}

bool ringBuffer_forcedFilling (ringBuffer_t *obj) {
	//
	// Description:
	//	It forces the argument defined ringBuffer to be filled. This function is used ad the beginning of the test
	//	process, usually.
	//
	bool out = false;

	return(out);
}

