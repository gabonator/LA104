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

#ifndef BASIC_GFX_HPP
#define BASIC_GFX_HPP

#include "basic_functionblock.hpp"
#include "basic_interpreter.hpp"

namespace BASIC
{

class GFXModule : public FunctionBlock
{
public:
	explicit GFXModule();

#if GFX_EXP_COLOR
	static bool command_boxc(Interpreter&);
#endif
	static bool command_box(Interpreter&);
#if GFX_EXP_COLOR
	static bool command_circlec(Interpreter&);
#endif
	static bool command_circle(Interpreter&);
	static bool command_color(Interpreter&);
	static bool command_cursor(Interpreter&);
#if GFX_EXP_COLOR
	static bool command_ellipsec(Interpreter&);
#endif
	static bool command_ellipse(Interpreter&);
#if GFX_EXP_COLOR
	static bool command_linec(Interpreter&);
#endif
	static bool command_lineto(Interpreter&);
	static bool command_line(Interpreter&);
#if GFX_EXP_COLOR
	static bool command_pointc(Interpreter&);
#endif
	static bool command_point(Interpreter&);
	static bool command_screen(Interpreter&);
	
protected:
	
	void _init() override;

private:
	static const FunctionBlock::command comms[] PROGMEM;
};

} // namespace BASIC

#endif // BASIC_GFX_HPP
