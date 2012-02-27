/*
 * usart.c contains code that deals with initializing the USART and
 * sending/receiving characters.
 */
 
/******************************************************************************
 * Copyright © 2012, Mike Roddewig (mike@dietfig.org).
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published 
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"
#include "fifo.h"
#include "hardware.h"

#include <util/setbaud.h>

//////////////////////
// Global Variables //
//////////////////////

FIFO * receive_fifo0;			// FIFO for the receive buffer.

volatile uint8_t usart0_error_flags;	// Register for setting error flags.

///////////////
// Functions //
///////////////

void usart_init(void) {
	char c;
	
	#if USE_2X
		UCSR0A |= _BV(U2X0);
	#else
		UCSR0A &= ~(_BV(U2X0));
	#endif
	
	UBRR0 = UBRR_VALUE; // Computed by setbaud.h
  
	UCSR0B = (_BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0)); // TX and RX enable, RX complete interrupt enable.
	UCSR0C = (_BV(UCSZ01) | _BV(UCSZ00)); // Asynchronous, no parity, 8 data bits, 1 stop bit.
	
	// Initialize our FIFO.
	
	receive_fifo0 = fifo_initialize(BUFFER_SIZE);
	
	// Flush out the receive buffer.
	
	while ((UCSR0A & _BV(RXC0)) != 0x00) {
		c = UDR0;
	}
}

// Sends a character over the USART. Busy waits until the TX buffer is 
// empty. In reality this should be implemented using an interrupt.

int usart0_putchar(char c, FILE * stream) {

	if (c == '\n') {
		// Add a carriage return to make everyone happy.
		usart0_putchar('\r', stream);
	}
  
	while (((UCSR0A & _BV(UDRE0)) == 0x00)) {
		// Wait until the transmit buffer is empty and the 
		// terminal on the other end is ready.
	}
  
	UDR0 = c;

	return 0x00;
}

int usart0_getchar(FILE *stream) {
	if (fifo_data_stored(receive_fifo0) != 0x00) {
		return (int) fifo_read(receive_fifo0);
	}
	else {
		return (int) 0x00;
	}
}

// Flush the receive buffer.
void usart0_flush(void) {
	fifo_clear(receive_fifo0);
}

// ISR to handle adding a character to the receive buffer.

ISR(USART_RX_vect) {
	char c;
	
	// A character is in the receive buffer.
	
	// First, check for error flags.
	
	if ((UCSR0A & _BV(FE0)) | (UCSR0A & _BV(DOR0)) | (UCSR0A & _BV(UPE0))) {
		// Some error occurred.
		
		if ((UCSR0A & _BV(FE0))) {
			usart0_error_flags |= USART_FRAME_ERROR;
		}
		
		if (UCSR0A & _BV(DOR0)) {
			usart0_error_flags |= USART_OVERRUN_ERROR;
		}
		
		if (UCSR0A & _BV(UPE0)) {
			usart0_error_flags |= USART_PARITY_ERROR;
		}
		
		// Write a null character in the buffer. We'll read off the bad 
		// character and discard it.
		
		fifo_write(receive_fifo0, 0x00);
		
		c = UDR0;
	}
	else {
		fifo_write(receive_fifo0, UDR0);
	}
}

