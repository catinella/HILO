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
// Filename: ringBuffer.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This module implemets a special ringBuffer that is resposible for its continuose self reliffilng or emptying.
//	This module is used just in non-interactive mode.
//
//	T(0) - start
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->|   |   |   |   |   |   |   |   |   |   |   |   +---+   
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	T(1) - forced fillup event
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->| X | X | X | X | X | X | X | X | X | X | X | X +---+   
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	T(n) - 50% of items have been pulled out...
//		     0                      50%                     MAX
//		     |                       |                       |
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->|   |   |   |   |   |   | X | X | X | X | X | X +---+
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	T(n+2) - refilling...
//		     0                      50%                     MAX
//		     |                       |                       |
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->| Y | Y | Y | Y | Y | Y | X | X | X | X | X | X +---+
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	T(n) - 50% of items have been pulled out...
//		     0                      50%                     MAX
//		     |                       |                       |
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->| Y | Y | Y | Y | Y | Y |   |   |   |   |   |   +---+
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//
//	[!] Storage's size
//	==================
//		Because the ringBuffer_refillCingB() and ringBuffer_emptyingCB() must use DMA transfer mode, the area to be
//		refill or to emptied must be statically defined. For this reason the area used by DMA is the 50% of the storage's
//		area, and the whole area size must be expressed by an even number.
//
//
//
// License:  LGPL ver 3.0
//
// 	This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU 	Lesser General
//	Public License as published by the Free Software Foundation; either version 3.0 of the License,	or (at your option)
//	any later version. 
//
//	For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 	You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//		Free Software Foundation, Inc.,
//		59 Temple Place, Suite 330,
//		Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint16_t bitConf_t;

typedef struct _ringBuffer ringBuffer_t;

typedef void (*rbCB_t) (ringBuffer_t *obj);

typedef enum {
	RINGBUFFER_INBUFF,
	RINGBUFFER_OUTBUFF
} ringBufferDir_t;

struct _ringBuffer {
	ringBufferDir_t dir;
	bitConf_t       *storage;
	unsigned int    storage_noi;
	unsigned int    index;
	unsigned int    usedSize;
	rbCB_t          myCB;
};


//------------------------------------------------------------------------------------------------------------------------------
//                                    P U B L I C   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------

bool ringBuffer_init (
	ringBuffer_t    *obj, 
	bitConf_t       *storage_a, 
	unsigned int    storageNoi_a, 
	ringBufferDir_t dir_a, 
	rbCB_t          cb_a
);

bool ringBuffer_pushPull (ringBuffer_t *obj, bitConf_t *data);
