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
 * @file basic_arduinoio.hpp
 * @brief Arduino io container
 */

#ifndef BASIC_ARDUINOIO_HPP
#define BASIC_ARDUINOIO_HPP

#include "basic_functionblock.hpp"
#include "basic_interpreter.hpp"

namespace BASIC
{

/**
 * @brief Module, containing Arduino digital and abalog I/O functions
 */
class ArduinoIO : public FunctionBlock
{
public:
	explicit ArduinoIO();
private:
#if USE_REALS
	static bool func_aread(Interpreter&);
#endif // USE_REALS
	static bool func_aread_int(Interpreter&);
	static bool func_dread(Interpreter&);
	static bool comm_awrite(Interpreter&);
	static bool comm_dwrite(Interpreter&);
#if CONF_MODULE_ARDUINOIO_TONE
	static bool comm_tone(Interpreter&);
	static bool comm_notone(Interpreter&);
#if CONF_BEEP
	static bool comm_beep(Interpreter&);
#endif // CONF_BEEP
#endif // CONF_MODULE_ARDUINOIO_TONE
	
#if USE_REALS
	static Real aread_r(Real);
#endif // USE_REALS

	static INT aread_i(INT);
	
	static const FunctionBlock::function _funcs[] PROGMEM;
	static const FunctionBlock::command _commands[] PROGMEM;
};

} // namespace BASIC

#endif // BASIC_ARDUINOIO_HPP
