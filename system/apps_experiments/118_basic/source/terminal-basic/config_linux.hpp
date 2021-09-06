/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019-2021 Terminal-BASIC team
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

#ifndef CONFIG_LINUX_HPP
#define CONFIG_LINUX_HPP

#include "Arduino.h"
#include "basic_config.hpp"
#include "basic.h"

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

namespace BASIC
{

// Number of bytes for program text, variables and stack
const pointer_t PROGRAMSIZE = 128000;

#if BASIC_MULTITERMINAL
const pointer_t SINGLE_PROGSIZE = PROGRAMSIZE / NUM_TERMINALS;
#else
const pointer_t SINGLE_PROGSIZE = PROGRAMSIZE;
#endif

} // namespace BASIC

// BEGIN PRIVATE

#define USEUTFT	           0
#define USETVOUT           0
#define USEPS2USARTKB      0

// END PRIVATE

#endif // CONFIG_LINUX_HPP
