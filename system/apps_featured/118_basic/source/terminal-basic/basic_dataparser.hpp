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
 * @file	basic_dataparser.hpp
 */

#ifndef BASIC_DATAPARSER_HPP
#define BASIC_DATAPARSER_HPP

#include "basic_interpreter.hpp"

namespace BASIC
{

class DataParser
{
public:
	DataParser(Interpreter&);
	bool searchData(const uint8_t*, Parser::Value&);
	bool read(const uint8_t*, Parser::Value&);
	const Lexer &lexer() const { return _lexer; }
private:
	bool readValue(Parser::Value&);
	Lexer	     _lexer;
	Interpreter &_interpreter;
};

} // namespace BASIC

#endif // BASIC_DATAPARSER_HPP
