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

#ifndef BASIC_PARSER_VALUE_HPP
#define BASIC_PARSER_VALUE_HPP

#include "basic_parser.hpp"
#include "basic_value.h"

#include <Printable.h>

namespace BASIC
{

class PACKED Parser::Value
{
public:
	/**
	 * @brief types of the value
	 */
	enum Type : uint8_t
	{
		INTEGER = BASIC_VALUE_TYPE_INTEGER,
#if USE_LONGINT
		LONG_INTEGER = BASIC_VALUE_TYPE_LONG_INTEGER,
#endif
#if USE_REALS
		REAL = BASIC_VALUE_TYPE_REAL,
#if USE_LONG_REALS
		LONG_REAL = BASIC_VALUE_TYPE_LONG_REAL,
#endif
#endif // USE_REALS
		LOGICAL = BASIC_VALUE_TYPE_LOGICAL,
		STRING = BASIC_VALUE_TYPE_STRING
	};

	/**
	 * @brief Default constructor
	 * 
	 * Initializes a value with 0 INTEGER, which s always available
	 */
	Value()
	{
		basic_value_setFromInteger(&m_value, 0);
	}
	
#if USE_LONGINT
	/**
	 * @brief Constructor ftrom LongInteger number
	 * @param v
	 */
	Value(LongInteger v)
	{
		basic_value_setFromLongInteger(&m_value, v);
	}
#endif
	Value(Integer);
#if USE_REALS
	Value(Real);
#if USE_LONG_REALS
	Value(LongReal);
#endif
#endif // USE_REALS
	Value(bool);

#if USE_REALS
	explicit operator Real() const;
#if USE_LONG_REALS
	explicit operator LongReal() const;
#endif
#endif // USE_REALS
	explicit operator bool() const;
	explicit operator Integer() const;
#if USE_LONGINT
	explicit operator LongInteger() const;
#endif
	
	bool operator<(const Value&) const;
	bool operator==(const Value&) const;
	bool operator>(const Value&) const;
	friend bool operator >=(const Value&, const Value&);
	friend bool operator <=(const Value&, const Value&);

	Value &operator+=(const Value&);
	Value &operator-=(const Value&);
	Value &operator*=(const Value&);
	Value &operator/=(const Value&);
	Value &divEquals(const Value&);
	Value &modEquals(const Value&);
	Value &operator^=(const Value&);
	Value &operator|=(const Value&);
	Value &operator&=(const Value&);
	void switchSign();
	
	static size_t size(Type);
	
	// Return Variable/Array/Function type based on it's name
	static Type typeFromName(const char*);
	
	Type type() const
	{
		return Type(m_value.type);
	}
	
	void setType(Type newVal)
	{
		m_value.type = basic_value_type_t(newVal);
	}
        
        basic_value_t m_value;
	
	// Printable interface
	size_t printTo(Print& p) const;
};

template <>
inline void writeValue<Parser::Value>(Parser::Value v, uint8_t* str)
{
	memcpy(str, &v, sizeof(Parser::Value));
}

template <>
inline typename Parser::Value readValue<Parser::Value>(const uint8_t* str)
{
	Parser::Value result;
	memcpy(&result, str, sizeof(Parser::Value));
	return result;
}

} // namespace BASIC

#endif // BASIC_PARSER_VALUE_HPP
