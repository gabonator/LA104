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

#include "basic_arduinoio.hpp"

#if CONF_MODULE_ARDUINOIO

#include "Arduino.h"
#ifdef ARDUINO_ARCH_ESP32
#include "HAL_esp32.h"
#endif
#include <assert.h>

namespace BASIC
{

static const uint8_t arduinoIOFuncs[] PROGMEM = {
	'A', 'R', 'E', 'A', 'D', '%', ASCII_NUL,
#if USE_REALS
	'A', 'R', 'E', 'A', 'D', ASCII_NUL,
#endif
	'D', 'R', 'E', 'A', 'D', ASCII_NUL,
	ASCII_ETX
};

const FunctionBlock::function ArduinoIO::_funcs[] PROGMEM = {
	ArduinoIO::func_aread_int,
#if USE_REALS
	ArduinoIO::func_aread,
#endif
	ArduinoIO::func_dread
};

static const uint8_t arduinoIOCommands[] PROGMEM = {
	'A', 'W', 'R', 'I', 'T', 'E', ASCII_NUL,
#if CONF_BEEP
	'B', 'E', 'E', 'P', ASCII_NUL,
#endif
#if CONF_MODULE_ARDUINOIO_TONE
	'D', 'N', 'O', 'T', 'O', 'N', 'E', ASCII_NUL,
	'D', 'T', 'O', 'N', 'E', ASCII_NUL,
#endif
	'D', 'W', 'R', 'I', 'T', 'E', ASCII_NUL,
	ASCII_ETX
};

const FunctionBlock::command ArduinoIO::_commands[] PROGMEM = {
	ArduinoIO::comm_awrite
#if CONF_BEEP
	, ArduinoIO::comm_beep
#endif
#if CONF_MODULE_ARDUINOIO_TONE
	, ArduinoIO::comm_notone
	, ArduinoIO::comm_tone
#endif
	, ArduinoIO::comm_dwrite
#if FAST_MODULE_CALL
	, nullptr
#endif
};

ArduinoIO::ArduinoIO()
{
	commands = _commands;
	commandTokens = arduinoIOCommands;
	functions = _funcs;
	functionTokens = arduinoIOFuncs;
}

#if USE_REALS

bool
ArduinoIO::func_aread(Interpreter &i)
{
	return general_func(i, aread_r);
}
#endif

bool
ArduinoIO::func_aread_int(Interpreter &i)
{
	return general_func(i, aread_i);
}

bool
ArduinoIO::func_dread(Interpreter &i)
{
	INT v;
	if (getIntegerFromStack(i, v))
		return i.pushValue(bool(HAL_gpio_readPin(v)));
	
	return false;
}

bool
ArduinoIO::comm_awrite(Interpreter &i)
{
	INT v;
	if (getIntegerFromStack(i, v)) {
		INT v2;
		if (getIntegerFromStack(i, v2)) {
			pinMode(v2, OUTPUT);
			analogWrite(v2, v);
			return true;
		}
	}
	return false;
}

bool
ArduinoIO::comm_dwrite(Interpreter &i)
{
	Parser::Value v(false);
	if (i.popValue(v)) {
		if (v.type() == Parser::Value::LOGICAL) {
			INT v2;
			if (getIntegerFromStack(i, v2)) {
				HAL_gpio_writePin(v2, bool(v));
				return true;
			}
		}
	}

	return false;
}

#if CONF_MODULE_ARDUINOIO_TONE

bool
ArduinoIO::comm_tone(Interpreter &i)
{
	INT pin, freq, dur;
	Parser::Value on(false);
	if (getIntegerFromStack(i, dur)) {
		if (getIntegerFromStack(i, freq)) {
			if (getIntegerFromStack(i, pin)) {
				HAL_buzzer_tone(pin, freq, dur);
				return true;
			}
		}
	}
	return false;
}

bool
ArduinoIO::comm_notone(Interpreter &i)
{
	INT pin;
	if (getIntegerFromStack(i, pin)) {
		HAL_buzzer_notone(pin);
		return true;
	}
	return false;
}

#endif

#if USE_REALS
Real
ArduinoIO::aread_r(Real v)
{
	pinMode(v, INPUT);

	return Real(analogRead(v)) / Real(1023) * Real(5.0);
}
#endif

INT
ArduinoIO::aread_i(INT v)
{
	return analogRead(v);
}


#if CONF_BEEP

bool
ArduinoIO::comm_beep(Interpreter &)
{
	HAL_buzzer_tone(BEEP_PIN, BEEP_FREQ, BEEP_DURATION);
	return true;
}
#endif // CONF_BEEP

} // namespace BASIC

#endif // CONF_MODULE_ARDUINOIO
