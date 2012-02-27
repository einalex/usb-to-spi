/*
 * Code for a FIFO that stores bytes with a user-selectable buffer size.
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

#include <stdlib.h>
#include <stdint.h>

#include "fifo.h"

FIFO * fifo_initialize(uint8_t fifo_size) {
	FIFO * fifo;
	
	fifo = malloc(sizeof(FIFO));	
	fifo->buffer_length = fifo_size;
	
	fifo->buffer = malloc(fifo_size*sizeof(uint8_t));
	
	fifo->data_stored = 0;
	fifo->write_position = 0;
	
	return fifo;
}

uint8_t fifo_write(FIFO * fifo, uint8_t data) {
	// Check to see if the FIFO is already full.
	
	if (fifo->data_stored == fifo->buffer_length) {
		return FIFO_FULL;
	}
	
	// Go ahead and write in the character, then.
	
	fifo->buffer[fifo->write_position] = data;
	
	// Increment the data counter.
	
	fifo->data_stored++;
	
	// Increment the write pointer.
	
	fifo->write_position++;
	
	if (fifo->write_position == fifo->buffer_length) {
		fifo->write_position = 0x00;
	}
	
	return FIFO_NO_ERROR;
}

uint8_t fifo_read(FIFO * fifo) {
	uint8_t read_position;
	
	// Is the FIFO empty?
	
	if (fifo->data_stored == 0x00) {
		return 0x00;
	}
	
	// Figure out where the next character to be read is.
	
	if (fifo->data_stored <= fifo->write_position) {
		read_position = fifo->write_position - fifo->data_stored;
	}
	else {
		read_position = fifo->buffer_length - (fifo->data_stored - fifo->write_position);
	}
	
	// Decrement the data counter.
	
	fifo->data_stored--;
	
	return fifo->buffer[read_position];
}

uint8_t fifo_space_remaining(FIFO * fifo) {
	return fifo->buffer_length - fifo->data_stored;
}

uint8_t fifo_data_stored(FIFO * fifo) {
	return fifo->data_stored;
}

void fifo_free(FIFO * fifo) {
	free(fifo->buffer);
	free(fifo);
}

void fifo_clear(FIFO * fifo) {
	fifo->data_stored = 0x00;
	fifo->write_position = 0x00;
}