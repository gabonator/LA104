/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
 *     <https://bitbucket.org/%7Bf50d6fee-8627-4ce4-848d-829168eedae5%7D/>
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

#ifdef ARDUINO_ARCH_AVR

#include <avr/io.h>
#include <avr/eeprom.h>

#include "HAL_avr8.h"

#if HAL_ARDUINO_AVR8_TERMINAL == HAL_ARDUINO_AVR8_TERMINAL_SERIALLIGHT
#include "seriallight.hpp"
#endif

__BEGIN_DECLS
void
HAL_initialize_concrete()
{
#if HAL_ARDUINO_AVR8_TERMINAL == HAL_ARDUINO_AVR8_TERMINAL_SERIALLIGHT
	SerialL.begin(HAL_ARDUINO_AVR8_TERMINAL_SERIAL_0_BR);
#if defined(HAVE_HWSERIAL1) && (HAL_TERMINAL_NUM > 0)
	SerialL1.begin(HAL_ARDUINO_AVR8_TERMINAL_SERIAL_1_BR);
#endif
#if defined(HAVE_HWSERIAL2) && (HAL_TERMINAL_NUM > 1)
	SerialL2.begin(HAL_ARDUINO_AVR8_TERMINAL_SERIAL_2_BR);
#endif
#if defined(HAVE_HWSERIAL3) && (HAL_TERMINAL_NUM > 2)
	SerialL3.begin(HAL_ARDUINO_AVR8_TERMINAL_SERIAL_3_BR);
#endif
#endif // HAL_ARDUINO_AVR8_TERMINAL
#if USE_EXTMEM
	XMCRA |= 1ul<<7; // Switch ext mem iface on
	XMCRB = 0;
#endif // USE_EXTMEM
}
__END_DECLS

void
HAL_finalize()
{
}

#if HAL_NVRAM

HAL_nvram_address_t HAL_nvram_getsize()
{
	return (HAL_nvram_address_t)(E2END+1);
}

uint8_t HAL_nvram_read(HAL_nvram_address_t addr)
{
	return eeprom_read_byte((uint8_t*)addr);
}

void HAL_nvram_write(HAL_nvram_address_t addr, uint8_t byte)
{
	return eeprom_update_byte((uint8_t*)addr, byte);
}

#endif // HAL_NVRAM

#if HAL_ARDUINO_AVR8_TERMINAL == HAL_ARDUINO_AVR8_TERMINAL_SERIALLIGHT

void
HAL_terminal_write(HAL_terminal_t t, uint8_t b)
{
	if (t == 0)
		SerialL.write(b);
#if defined(HAVE_HWSERIAL1) && (HAL_TERMINAL_NUM > 0)
	else if (t == 1)
		SerialL1.write(b);
#endif
}

uint8_t
HAL_terminal_read(HAL_terminal_t t)
{
	if (t == 0)
		return SerialL.read();
#if defined(HAVE_HWSERIAL1) && (HAL_TERMINAL_NUM > 0)
	else if (t == 1)
		return SerialL1.read();
#endif
	return 0;
}

BOOLEAN
HAL_terminal_isdataready(HAL_terminal_t t)
{
	if (t == 0)
		return SerialL.available();
#if defined(HAVE_HWSERIAL1) && (HAL_TERMINAL_NUM > 0)
	else if (t == 1)
		return SerialL1.available();
#endif
	return FALSE;
}
#endif // HAL_ARDUINO_AVR8_TERMINAL

__BEGIN_DECLS
void
HAL_update_concrete()
{
}
__END_DECLS

#endif // ARDUINO_ARCH_AVR
