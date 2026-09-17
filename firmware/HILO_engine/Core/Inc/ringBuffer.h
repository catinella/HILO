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
//	T(1) - forced fillup
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->| X | X | X | X | X | X | X | X | X | X | X | X +---+   
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	T(n) - emptying...
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->|   |   |   |   |   |   | X | X | X | X | X | X +---+
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	T(n+2) - emptying...
//		     +---+---+---+---+---+---+---+---+---+---+---+---+
//		+--->| Y | Y | Y | Y | Y | Y |   |   | X | X | X | X +---+
//		|    +---+---+---+---+---+---+---+---+---+---+---+---+   |
//		+--------------------------------------------------------+
//
//	The ringBuffer_refillCingB() and ringBuffer_emptyingCB() must use DMA transfer mode
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

typedef uint16_t bitConf_t

#define RINGBUFFER_PERCTHRESHOLD 50

typedef struct {
	unsigned int index;
	bitConf_t    *storage;
	unsigned int usedSize;
} ringBuffer_t;

void (*ringBuffer_refillCingB) (ringBuffer_t *obj);
void (*ringBuffer_emptyingCB)  (ringBuffer_t *obj);

//------------------------------------------------------------------------------------------------------------------------------
//                                    P U B L I C   F U N C T I O N S
//------------------------------------------------------------------------------------------------------------------------------
void ringBuffer_set_refillCingB (ringBuffer_refillCingB *refillCingB_a);
void ringBuffer_set_emptyingCB  (ringBuffer_emptyingCB  *emptyingCB_a);

bool ringBuffer_init            (ringBuffer_t *obj, const bitConf_t *storage_a);
bool ringBuffer_push            (ringBuffer_t *obj, bitConf_t data);
bool ringBuffer_pull            (ringBuffer_t *obj, *bitConf_t data);

bool ringBuffer_forcedEmptying  (ringBuffer_t *obj);
bool ringBuffer_forcedFilling   (ringBuffer_t *obj);

