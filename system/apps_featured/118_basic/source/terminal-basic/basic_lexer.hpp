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

#ifndef LEXER_HPP
#define LEXER_HPP

#include <inttypes.h>
#include <stdlib.h>

#include "arduino_logger.hpp"
#include "basic_parser.hpp"
#include "basic_parser_value.hpp"
#include "helper.hpp"
#include "basic_lexer.h"

namespace BASIC
{

#if ARDUINO_LOG
Logger&
operator<<(Logger &logger, Token tok);
#endif

/**
 * @brief Lexical analyzer class
 */
class Lexer
{
public:
	/**
	 * @brief lexical analyses stage errors
	 */
	enum Error : uint8_t
	{
		NO_ERROR = 0,
		STRING_OVERFLOW = 1
	};
	/**
	 * @brief initialize lexer session
	 * @param str null-terminating string to extract tokens from
	 * @param tok tokenized text flag
	 */
	void init(const uint8_t*, bool);
	/**
	 * @brief continue lexical analyze for next token
	 * @return string end flag
	 */
	bool getNext();
	/**
	 * @brief get last extracted token
	 * @return last token
	 */
	Token getToken() const { return Token(m_context.token); }
	/**
	 * @brief get last lexer error
	 * @return error code
	 */
	Error getError() const { return Error(m_context._error); }
	/**
	 * @brief get current value (numberm boolean...)
	 * @return value, extracted from string
	 */
	const Parser::Value &getValue() const
	{
		return reinterpret_cast<const Parser::Value&>(m_context.value);
	}
	/**
	 * @brief get current string (identifier)
	 * @return identifier string
	 */
	const char *id() const { return m_context._id; }
	/**
	 * @brief get current string position
	 * @return string position index
	 */
	uint8_t getPointer() const { return m_context.string_pointer; }
	
	void setPointer(uint8_t newVal) { m_context.string_pointer = newVal; }
	
	const uint8_t* getString() const { return m_context.string_to_parse; }
	/**
	 * @brief Get null-terminated token string representation
	 * @param token Token code
	 * @param buf String buffer to copy to
	 * @return buffer pointer or nullptr if error
	 */
	static bool getTokenString(Token, uint8_t*);
	
	/**
	 * 
	 * @param dst
	 * @param dstlen
	 * @param src
	 */
	uint8_t tokenize(uint8_t*, uint8_t, const uint8_t*);
private:
	
	// Parse decimal number
	void decimalNumber();
	// Parse Binary number
	void binaryInteger();
#if USE_REALS
	void binaryReal();
	bool numberScale();
#endif
	void ident();
	void stringConst();
	
	basic_lexer_context_t m_context;
};

} // namespace BASIC

#endif
