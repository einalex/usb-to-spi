/*
 * SPI includes
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

#ifndef SPI_H
#define SPI_H

//////////////////////
// Function Defines //
//////////////////////

void spi_init(void);

void spi_transmit(uint8_t *, uint8_t);

//////////////////////////
// SPI Port Definitions //
//////////////////////////

#define	SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define MOSI_PIN	3
#define MISO_PIN	4
#define SS_PIN		2
#define SCK_PIN		5

#endif