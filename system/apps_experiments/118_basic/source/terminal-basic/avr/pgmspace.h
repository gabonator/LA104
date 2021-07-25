/*
 * Libarduinoemulator is a simple library to emulate ArduinoIDE API on a Linux PC
 * Copyright (C) 2016, 2017 Andrey V. Skvortsov <starling13@mail.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#ifndef PGMSPACE_H
#define PGMSPACE_H

#if (defined ARDUINO_ARCH_AVR) || (defined ARDUINO_ARCH_SAM) || (defined ARDUINO_ARCH_ESP8266)
#include_next <avr/pgmspace.h>
#else

#include <stdint.h>
#include <string.h>

#define PGM_P const char*
#define PROGMEM
#define pgm_read_byte(a) (*((const uint8_t*)a))
#define pgm_read_word(a) (*a)
#define pgm_read_ptr(a) (*a)
#define strcpy_P(a,b) strcpy(a,b)
#define strcmp_P(a,b) strcmp(a,b)
#define memcpy_P(a,b,c) memcpy(a,b,c)

#endif /* ARDUINO_ARCH_AVR */

#endif /* PGMSPACE_H */
