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

/**
 * @file config_arduino.hpp
 * @brief Configuration parameters, specific for Arduino builds
 */

#ifndef CONFIG_ARDUINO_HPP
#define CONFIG_ARDUINO_HPP

#include "basic_config.hpp"
#include "basic.h"

/**
 * Parameters
 */

#ifdef ARDUINO_ARCH_SAM
#define HAVE_HWSERIAL1
#define HAVE_HWSERIAL2
#define HAVE_HWSERIAL3
#define HAVE_HWSERIAL4
#endif

#if BASIC_MULTITERMINAL
#define NUM_TERMINALS 1
#ifdef HAVE_HWSERIAL1
#define SERIAL_PORT1 Serial1
#undef NUM_TERMINALS
#define NUM_TERMINALS 2
#endif
#ifdef HAVE_HWSERIAL2
#define SERIAL_PORT2 Serial2
#undef NUM_TERMINALS
#define NUM_TERMINALS 3
#endif
#ifdef HAVE_HWSERIAL3
#define SERIAL_PORT3 Serial3
#undef NUM_TERMINALS
#define NUM_TERMINALS 4
#endif
#endif // BASIC_MULTITERMINAL

// Use external memory
#define USE_EXTMEM                0
#if USE_EXTMEM
#define EXTMEM_ADDRESS 0x8000
#define EXTMEM_SIZE    32768
#endif

namespace BASIC
{
// Number of bytes for program text, variables and stack
#if USE_EXTMEM
const pointer_t PROGRAMSIZE = EXTMEM_SIZE;
#elif defined (__AVR_ATmega1284__) || defined (__AVR_ATmega1284P__)
#if (S_OUTPUT != TVOUT_O)
const pointer_t PROGRAMSIZE = 14336;
#else
const pointer_t PROGRAMSIZE = 8192;
#endif
#elif defined (__AVR_ATmega2560__)
#if (S_OUTPUT != TVOUT_O) && (!USE_EXTMEM) && (!CONF_USE_EXTMEMFS)
const pointer_t PROGRAMSIZE = 6144;
#elif (!USE_EXTMEM) && (CONF_USE_EXTMEMFS)
const pointer_t PROGRAMSIZE = 5900;
#else
const pointer_t PROGRAMSIZE = 512;
#endif
#elif defined (__AVR_ATmega128__) || defined (__AVR_ATmega128A__)
const pointer_t PROGRAMSIZE = 3072;
#elif defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#if CONF_USE_EXTMEMFS
const pointer_t PROGRAMSIZE = 256;
#else
const pointer_t PROGRAMSIZE = 1024;
#endif
#elif defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__)
const pointer_t PROGRAMSIZE = 384;
#elif defined ARDUINO_ARCH_SAM
const pointer_t PROGRAMSIZE = 65535;
#elif defined ARDUINO_ARCH_ESP8266
const pointer_t PROGRAMSIZE = 32768;
#elif defined ARDUINO_ARCH_ESP32
const pointer_t PROGRAMSIZE = 65535;
#else
const pointer_t PROGRAMSIZE = 1024;
#endif // USE_EXTMEM

#if BASIC_MULTITERMINAL
const pointer_t SINGLE_PROGSIZE = PROGRAMSIZE / (NUM_TERMINALS+1);
#else
const pointer_t SINGLE_PROGSIZE = PROGRAMSIZE;
#endif

// BEGIN PRIVATE

#define USEUTFT            0
#define USETVOUT           0
#define USEPS2USARTKB      0
#define USELIQUIDCRYSTAL   1

// END PRIVATE

} // nameespace BASIC

#endif // CONFIG_ARDUINO_HPP
