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

#ifndef BASIC_FUNCTIONBLOCK_HPP
#define BASIC_FUNCTIONBLOCK_HPP

#include <stdlib.h>
#include "basic.hpp"

namespace BASIC
{

class Interpreter;

class FunctionBlock
{
public:

	typedef bool (*function)(Interpreter&);
	
	typedef bool (*command)(Interpreter&);
	
	function getFunction(const char*) const;
	
	command getCommand(const char*) const;
	
	void getCommandName(command, uint8_t*) const;
	
	void init();
	
	FunctionBlock *next() { return _next; }
	
	void setNext(FunctionBlock *newVal);
	
protected:
#if USE_REALS
	typedef Real (*_funcReal)(Real);
#if USE_LONG_REALS
	typedef LongReal (*_funcLongReal)(LongReal);
#endif
#endif // USE_REALS
	typedef INT (*_funcInteger)(INT);

	explicit FunctionBlock(FunctionBlock* =NULL);
	
	virtual void _init() {}
	/**
	 * @brief return pointer to the function of this block
	 *   or NULL if no one
	 * @param fName function name
	 * @return function pointer
	 */
	function _getFunction(const char*) const;
	/**
	 * @brief return pointer to the command, provided by this block
	 *   or NULL if no one
	 * @param fName command name
	 * @return command pointer
	 */
	command _getCommand(const char*) const;
#if USE_REALS
	/**
	 * @brief general function wrapper with 1 Real argument
	 * @param interpreter Interpreter object
	 * @param func underlying function pointer
	 * @return ok status
	 */
	static bool general_func(Interpreter&, _funcReal);
#if USE_LONG_REALS
	static bool general_func(Interpreter&, _funcLongReal);
#endif
#endif // USE_REALS
	/**
	 * @brief general function wrapper with 1 Real argument
	 * @param interpreter Interpreter object
	 * @param func underlying function pointer
	 * @return ok status
	 */
	static bool general_func(Interpreter&, _funcInteger);
	
	static bool getIntegerFromStack(Interpreter&, INT&);
	
	const uint8_t *commandTokens;
	const FunctionBlock::command *commands;
	const uint8_t *functionTokens;
	const FunctionBlock::function *functions;
private:
	FunctionBlock *_next;
};

} // namespace BASIC

#endif // BASIC_FUNCTIONBLOCK_HPP
