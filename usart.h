/*
 * Header file for usart.c
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

#ifndef USART_H
#define USART_H

/////////////
// Defines //
/////////////

// Receive buffer size (in characters)
#define BUFFER_SIZE 80

// Error Flags

#define USART_PARITY_ERROR 0x01
#define USART_OVERRUN_ERROR 0x02
#define USART_FRAME_ERROR 0x04

///////////////
// Functions //
///////////////

// Initialize the USARTs.
void usart_init(void);

// Send a character over USART 0.
int usart0_putchar(char c, FILE *stream);

// Pops a character (if available) off the receive FIFO from USART 0.
int usart0_getchar(FILE *stream);

// Flushes the receive buffer.
void usart0_flush(void);

#endif