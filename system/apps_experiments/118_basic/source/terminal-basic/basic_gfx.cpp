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

#include "basic_gfx.hpp"

#if USE_GFX

namespace BASIC
{

static const uint8_t gfxTokens[] PROGMEM = {
#if GFX_EXP_COLOR
	'B','O','X','C', ASCII_NUL,
#endif
	'B','O','X', ASCII_NUL,
#if GFX_EXP_COLOR
	'C','I','R','C','L','E','C', ASCII_NUL,
#endif
	'C','I','R','C','L','E', ASCII_NUL,
	'C','O','L','O','R', ASCII_NUL,
	'C','U','R','S','O','R', ASCII_NUL,
#if GFX_EXP_COLOR
	'E','L','L','I','P','S','E','C',ASCII_NUL,
#endif
	'E','L','L','I','P','S','E',ASCII_NUL,
#if GFX_EXP_COLOR
	'L','I','N','E','C', ASCII_NUL,
#endif
	'L','I','N','E','T','O', ASCII_NUL,
	'L','I','N','E', ASCII_NUL,
#if GFX_EXP_COLOR
	'P','O','I','N','T','C', ASCII_NUL,
#endif
	'P','O','I','N','T', ASCII_NUL,
	'S','C','R','E','E','N', ASCII_NUL,
	ASCII_ETX
};

const FunctionBlock::command GFXModule::comms[] PROGMEM = {
#if GFX_EXP_COLOR
	  GFXModule::command_boxc,
#endif
	  GFXModule::command_box
#if GFX_EXP_COLOR
	, GFXModule::command_circlec
#endif
	, GFXModule::command_circle
	, GFXModule::command_color
	, GFXModule::command_cursor
#if GFX_EXP_COLOR
	, GFXModule::command_ellipsec
#endif
	, GFXModule::command_ellipse
#if GFX_EXP_COLOR
	, GFXModule::command_linec
#endif
	, GFXModule::command_lineto
	, GFXModule::command_line
#if GFX_EXP_COLOR
	, GFXModule::command_pointc
#endif
	, GFXModule::command_point
	, GFXModule::command_screen
#if FAST_MODULE_CALL
	, nullptr
#endif
};

GFXModule::GFXModule()
{
	commands = comms;
	commandTokens = gfxTokens;
}

} // namespace BASIC

#endif // USE_GFX
