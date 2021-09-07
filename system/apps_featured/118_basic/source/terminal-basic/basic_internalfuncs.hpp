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

#ifndef BASIC_INTERNALFUNCS_HPP
#define BASIC_INTERNALFUNCS_HPP

#include "basic_functionblock.hpp"
#include "helper.hpp"

namespace BASIC
{

class InternalFunctions : public FunctionBlock
{
public:
	InternalFunctions(FunctionBlock* = nullptr);
private:
	static bool func_abs(Interpreter&);
#if USE_ASC
	static bool func_asc(Interpreter&);
#endif
#if USE_CHR
	static bool func_chr(Interpreter&);
#endif
#if USE_HEX
	static bool func_hex(Interpreter&);
#endif
#if USE_INKEY
	static bool func_inkey(Interpreter&);
#endif
#if USE_REALS
	static bool func_int(Interpreter&);
#endif
#if USE_LEFT
	static bool func_left(Interpreter&);
#endif
#if USE_RIGHT
	static bool func_right(Interpreter&);
#endif
#if USE_LEN
	static bool func_len(Interpreter&);
#endif
#if USE_MID || USE_SEG
	static bool func_mid(Interpreter&);
#endif
#if USE_PEEK_POKE
	static bool func_peek(Interpreter&);
#endif
	static bool func_result(Interpreter&);
#if USE_RANDOM
	static bool func_rnd(Interpreter&);
#endif
	static bool func_sgn(Interpreter&);
        static bool func_str(Interpreter&);
	static bool func_tim(Interpreter&);
#if USE_REALS
#define ___TYP Real
#else
#define ___TYP INT
#endif // USE_REALS
	static ___TYP sgn(___TYP);
#undef ___TYP
	
	static const FunctionBlock::function funcs[] PROGMEM;
};

} // namespace BASIC

#endif // BASIC_INTERNALFUNCS_HPP
