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
#include <string.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "usart.h"
#include "spi.h"
#include "hardware.h"

#include <util/delay.h>

/////////////
// Defines //
/////////////

#define INPUT_BUFFER_SIZE 80
#define BACKSPACE 0x08

//////////////////////
// Global Variables //
//////////////////////

FILE usart0_stream = FDEV_SETUP_STREAM(usart0_putchar, usart0_getchar, _FDEV_SETUP_RW);

const char command_prompt[] PROGMEM = "> ";

///////////////////////
// Private Functions //
///////////////////////

void initialize_controller(void);
char * process_input(void);

///////////////
// Functions //
///////////////

void initialize_controller() 
{
	// Enable interrupts
	
	sei();
	
	// Initialize the hardware
	
	usart_init();
	spi_init();
	
	// Redirect stdio
	
	stdout = stdin = stderr = &usart0_stream;
}

char * process_input(void) {
	static char input[INPUT_BUFFER_SIZE];
	char c;
	uint8_t done;
	uint8_t input_position;
	
	done = 0x00;
	input_position = 0x00;
	
	while (done == 0x00) {
		c = fgetc(stdin);
		
		if (c == EOF) {
			puts_P(PSTR("\nStream error, please try again.\n"));
			printf_P(command_prompt);
			
			input_position = 0x00;
			clearerr(stdin);
		}
		else if ((input_position == INPUT_BUFFER_SIZE - 1) && (c != BACKSPACE)) {
			// Null-terminate the current buffer before printing.
			
			input[input_position] = 0x00;
			
			printf_P(PSTR("\r> %s"), input);
		}
		else if (c == 0x00) {
			// Ignore!
		}
		else {
			// Echo the character back.
			fputc(c, stdout);
			
			if (c == BACKSPACE) {
				// Delete the current character.
				
				input_position--;
				
				// Null-terminate.

				input[input_position] = 0x00;
				printf_P(PSTR("%c  \r> %s"), BACKSPACE, input);
			}
			else if (c == '\r') {
				done = 0x01;
				
				input[input_position] = c;
				input_position++;
			}
			else {
				input[input_position] = c;
				input_position++;
			}
		}
	}
	
	// Null-terminate the end of the string.
	
	input[input_position] = 0x00;
	
	// Send a new line to the terminal.
	
	fputc('\n', stdout);
	
	return input;
}

int main(void) 
{
	uint8_t buffer[3];
	uint8_t ret;
	
	char * input_string;
	
	// Initialize the controller
	
	initialize_controller();
	
	puts_P(PSTR("SPI Controller v1.0"));
	
	for(;;) 
	{
		printf_P(command_prompt);
		input_string = process_input();
		
		ret = sscanf_P(input_string, PSTR("0x%2hhx %2hhx %2hhx"), &buffer[2], &buffer[1], &buffer[0]);
		
		if (ret == 3) 
		{
			spi_transmit(buffer, 3);
			printf_P(PSTR("Transmitted 0x%2hhX%2hhX%2hhX\n"), buffer[2], buffer[1], buffer[0]);
		}
	}
}