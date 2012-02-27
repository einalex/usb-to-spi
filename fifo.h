/*
 * Header file for fifo.c
 */

/******************************************************************************
 * Copyright © 2010, Mike Roddewig.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

/////////////
// A FIFO! //
/////////////

typedef struct {
	uint8_t * buffer;
	uint8_t data_stored;
	uint8_t write_position;
	uint8_t buffer_length;
} FIFO;

///////////////
// Functions //
///////////////

// Initialize a FIFO with the given size.
FIFO * fifo_initialize(uint8_t);

// Write a byte to the FIFO. Returns one of the two return codes given below.
uint8_t fifo_write(FIFO *, uint8_t);

// Read a byte from the FIFO. Returns null if the FIFO is empty (so check
// beforehand!).
uint8_t fifo_read(FIFO *);

// Returns how many more bytes can be stored in the FIFO.
uint8_t fifo_space_remaining(FIFO *);

// Returns how many bytes are stored in the FIFO.
uint8_t fifo_data_stored(FIFO *);

// Frees the memory used by a FIFO.
void fifo_free(FIFO *);

// Clears the FIFO.
void fifo_clear(FIFO *);

//////////////////
// Return Codes //
//////////////////

#define FIFO_NO_ERROR 0x00
#define FIFO_FULL 0x01

#endif