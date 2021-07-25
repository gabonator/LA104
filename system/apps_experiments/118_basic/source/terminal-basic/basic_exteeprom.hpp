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
 * @file basic_exteeprom.hpp
 * @brief I2C/SPI external eeprom functions and commands
 */

#ifndef BASIC_EXTEEPROM_HPP
#define BASIC_EXTEEPROM_HPP

#include "basic_functionblock.hpp"

namespace BASIC
{

class ExtEEPROM : public FunctionBlock
{
public:
	explicit ExtEEPROM();
private:
	static bool com_echain(Interpreter&);
	static bool com_eload(Interpreter&);
	static bool com_esave(Interpreter&);
	
	static const FunctionBlock::command _commands[] PROGMEM;
// FunctionBlock interface
protected:
	void _init() override;
};

} // namespace BASIC

#endif // BASIC_EXTEEPROM_HPP
