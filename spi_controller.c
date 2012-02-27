/*
 * USB to SPI converter for Analog Devices DAC
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

#include <stdio.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "usart.h"
#include "hardware.h"

#include <util/delay.h>

//////////////////////
// Global Variables //
//////////////////////

FILE usart0_stream = FDEV_SETUP_STREAM(usart0_putchar, usart0_getchar, _FDEV_SETUP_RW);

///////////////////////
// Private Functions //
///////////////////////

void initialize_controller(void);

///////////////
// Functions //
///////////////

void initialize_controller() 
{
	// Enable interrupts
	
	sei();
	
	// Initialize the USART
	
	usart_init();
	
	// Redirect stdio
	
	stdout = stdin = stderr = &usart0_stream;
}

int main(void) 
{
	// Initialize the controller
	
	initialize_controller();
	
	for(;;) 
	{
		puts_P(PSTR("Hello world!\n"));
		
		_delay_ms(1000);
	}
}