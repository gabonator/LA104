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
 * @brief Configuration for the Arduino core AVR8 HAL implementation
 */

#ifndef HAL_AVR8_H
#define HAL_AVR8_H

#include "HAL.h"

#define HAL_ARDUINO_AVR8_TERMINAL_NONE 0
/* Use SerialL[N] objects for terminal */
#define HAL_ARDUINO_AVR8_TERMINAL_SERIALLIGHT 1

#define HAL_ARDUINO_AVR8_TERMINAL HAL_ARDUINO_AVR8_TERMINAL_NONE

#if HAL_ARDUINO_AVR8_TERMINAL == HAL_ARDUINO_AVR8_TERMINAL_SERIALLIGHT
#define HAL_ARDUINO_AVR8_TERMINAL_SERIAL_0_BR 115200
#if HAL_TERMINAL_NUM > 0
#define HAL_ARDUINO_AVR8_TERMINAL_SERIAL_1_BR 115200
#endif /* HAL_TERMINAL_NUM */
#if HAL_TERMINAL_NUM > 1
#define HAL_ARDUINO_AVR8_TERMINAL_SERIAL_2_BR 115200
#endif /* HAL_TERMINAL_NUM */
#if HAL_TERMINAL_NUM > 2
#define HAL_ARDUINO_AVR8_TERMINAL_SERIAL_3_BR 115200
#endif /* HAL_TERMINAL_NUM */
#endif /* HAL_ARDUINO_AVR8_TERMINAL */

// Use ATMega128/2560 external memory interface
#define HAL_ARDUINO_AVR8_EXTMEM 1

#endif /* HAL_ARDUINO_H */
