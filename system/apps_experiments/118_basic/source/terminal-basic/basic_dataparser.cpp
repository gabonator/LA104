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

#include "basic_dataparser.hpp"

#if USE_DATA

namespace BASIC
{

DataParser::DataParser(Interpreter &interpreter) :
_interpreter(interpreter)
{	
}

bool
DataParser::searchData(const uint8_t *str, Parser::Value &value)
{
	_lexer.init(str, true);
	while (_lexer.getNext()) {
		if ((_lexer.getToken() == Token::KW_DATA) &&
		    _lexer.getNext())
			return readValue(value);
	}
	return false;
}

bool
DataParser::read(const uint8_t *str, Parser::Value &value)
{
	_lexer.init(str, true);
	if (_lexer.getNext() && (_lexer.getToken() == Token::COMMA)
	    && _lexer.getNext())
		return readValue(value);
	return false;
}

bool
DataParser::readValue(Parser::Value &value)
{
	bool minus = _lexer.getToken() == Token::MINUS;
	if (minus || _lexer.getToken() == Token::PLUS) {
		if (!_lexer.getNext())
			return false;
	}
	if ((_lexer.getToken() >= Token::C_INTEGER)
	 && (_lexer.getToken() <= Token::C_STRING)) {
		value = _lexer.getValue();
		if (minus)
			value.switchSign();
		if (_lexer.getToken() == Token::C_STRING)
			_interpreter.pushString(_lexer.id());
		return true;
	}
	return false;
}

} // namespace BASIC

#endif // USE_DATA
