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
//	This module implemets a special ringBuffer that is resposible for its conrinuose self reliffilng
//
//	                          +------------+           +-------------+           +------------+
//	                refill CB |    INPUT   |    pull() |             | push()    |   OUTPUT   |   empty CB
//	[SPI SRAM 1] ============>| ringBuffer +---------->|   PROCESS   +---------->| ringBuffer |============> [SPI SRAM 2]
//	               (DMA 1)    |            |           |             |           |            |    (DMA 2)
//	                          +------------+           +-------------+           +------------+
//
// License:  LGPL ver 3.0
//
//	This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU Lesser General
//	Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
//	any later version. 
//
//	For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
//	You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//		Free Software Foundation, Inc.,
//		59 Temple Place, Suite 330,
//		Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <ringBuffer.h>

#define RBUFFTSH(X) (X * RINGBUFFER_PERCTHRESHOLD / 100)

bool ringBuffer_init (
	ringBuffer_t    *obj, 
	bitConf_t       *storage_a, 
	unsigned int    storageNoi_a, 
	ringBufferDir_t dir_a, 
	rbCB_t          cb_a
) {
	//
	// Description:
	//	It inirtializes the argument dfined object. All objects of this type, must be initialized before to be used
	//
	// Returned value:
	//	true    The argument defined object has been successfully initialized
	//	false   ERROR!: invalid arguments
	//
	bool out = false;
	if (storage_a != NULL && cb_a != NULL && obj != NULL && storageNoi_a > 0) {
		obj->dir         = dir_a;
		obj->myCB        = cb_a;
		obj->storage     = storage_a;
		obj->storage_noi = storageNoi_a;
		obj->usedSize    = 0;
		obj->index       = 0;
		out = true;
	} else {
		// ERROR!
	}
	return(out);
}

bool ringBuffer_push (ringBuffer_t *obj, bitConf_t data) {
	//
	// Description:
	//	It push a new data into the ring buffer
	//
	bool out = false;
	
	if (obj->dir == RINGBUFFER_OUTBUFF) {
		
		// Checking for ovesize
		if (obj->usedSize < obj->storage_noi) {
			obj->storage[obj->index] = data;
			obj->index++;
			obj->usedSize++;
			
			// Closing the ring
			if (obj->index == obj->storage_noi)
				obj->index = 0;
			
			// Checking for the used size
			if (obj->usedSize >= RBUFFTSH(obj->storage_noi))
				obj->myCB(obj);
				
			out = true;
			
		} else {
			// ERROR! The buffer is full
		}
	} else {
		// ERROR! Illegal operation
	}
	
	return(out);
}

bool ringBuffer_pull (ringBuffer_t *obj, bitConf_t *data) {
	//
	// Description:
	//	It pull a data from the ring buffer
	//
	bool out = false;

	if (obj->dir == RINGBUFFER_INBUFF) {
		
		// Checking for ovesize
		if (obj->usedSize > 0) {
			*data = obj->storage[obj->index];
			obj->usedSize--;
			if (obj->index == 0)
				obj->index = obj->storage_noi - 1;
			else
				obj->index--;
			
			// Checking for the used size
			if (obj->usedSize <= RBUFFTSH(obj->storage_noi))
				obj->myCB(obj);
				
			out = true;
		
		} else {
			// ERROR! The buffer is empty
		}
	} else {
		// ERROR! Illegal operation
	}
		
	return(out);
}

bool ringBuffer_forcedEmptying (ringBuffer_t *obj) {
	//
	// Description:
	//	It forces the argument defined ringBuffer to be empty. This function is used ad the end of the test process,
	//	usually.
	//
	bool out = false;
	if (obj->dir == RINGBUFFER_OUTBUFF) {
		obj->myCB(obj);
		out = true;
	}

	return(out);
}

bool ringBuffer_forcedFilling (ringBuffer_t *obj) {
	//
	// Description:
	//	It forces the argument defined ringBuffer to be filled. This function is used ad the beginning of the test
	//	process, usually.
	//
	bool out = false;
	if (obj->dir == RINGBUFFER_INBUFF) {
		obj->myCB(obj);
		out = true;
	}
	
	return(out);
}

