/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * Copyright (C) 2017-2019 Andrey V. Skvortsov <starling13@mail.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tools.h"
#include "basic.h"

#include <stdlib.h>

#include "avr/pgmspace.h"

uint8_t*
scanTable(const uint8_t *token, const uint8_t table[], uint8_t *index)
{
	uint8_t tokPos = 0, // Position in current token
		tabPos = 0; // Index in the token table
	while (TRUE) {
		// Read next table byte
		uint8_t c = pgm_read_byte(table);
		// token byte to compare with table
		uint8_t ct = token[tokPos];
		/* If token table is over */
		if (c == ASCII_ETX)
			return NULL;
		else if (c > ct && ct > ' ')
			return NULL;
		/* Current symbol matches table element */
		else if (ct == c) {
			++tokPos, ++table;
			if (pgm_read_byte(table) == ASCII_NUL) {
				*index = tabPos;
				return (uint8_t*)token+tokPos;
			}
			continue;
		} else if (ct <= ' ' && c == ASCII_NUL) {
			*index = tabPos;
			return (uint8_t*)token+tokPos;
		} else {
			while (pgm_read_byte(table++) != ASCII_NUL);
			++tabPos, tokPos=0;
			continue;
		}
		/* Not current token, take next table row */
		if (ct == ASCII_NUL) {
			while (pgm_read_byte(table++) != ASCII_NUL);
			++tabPos, tokPos=0;
		}
	}
	return NULL;
}

void getToken(const uint8_t* table, uint8_t index, uint8_t* buf)
{
	uint8_t byte = 0xFF;
	while (byte != ASCII_ETX) {
		if (byte == ASCII_NUL)
			--index;
		if (index == 0) {
			while ((byte = pgm_read_byte(table++)) != ASCII_NUL)
				*(buf++) = byte;
			break;
		}
		byte = pgm_read_byte(table++);
	}
	*buf = '\0';
}
