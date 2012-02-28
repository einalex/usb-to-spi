/*
 * SPI code
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

#include <avr/io.h>

#include "hardware.h"
#include "spi.h"

///////////////
// Functions //
///////////////

void spi_init(void)
{
	// Configure SPI port for master operation
	
	SPI_DDR |= (_BV(MOSI_PIN) | _BV(SCK_PIN) | _BV(SS_PIN));	// Set MOSI, SCK, and SS to output
	SPI_DDR &= ~(_BV(MISO_PIN));					// Set MISO to input
	
	SPCR |= (_BV(SPE) | _BV(MSTR) | _BV(CPOL) | _BV(SPR0));	// Enable SPI, master mode, sample on falling edge, 
								// setup on leading, SPI clk freq 1 MHz, MSB first.
	
	// Set SS high
	
	SPI_PORT |= _BV(SS_PIN);
}

void spi_transmit(uint8_t * buffer, uint8_t length)
{
	uint8_t i;
	
	// Set SS low
	
	SPI_PORT &= ~(_BV(SS_PIN));
	
	for (i = length; i > 0; i--)
	{
		SPDR = buffer[i-1];
		
		// Wait for the transmission to complete

		while ((SPSR & _BV(SPIF)) == 0x00);
	}
	
	// Set SS high
	
	SPI_PORT |= _BV(SS_PIN);
}
