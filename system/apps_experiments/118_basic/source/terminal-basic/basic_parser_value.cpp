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

#include "basic_parser_value.hpp"

#include "math.hpp"
#include "basic_lexer.hpp"
#include "ascii.hpp"

namespace BASIC
{

#if USE_LONGINT

#endif // USE_LONGINT

Parser::Value::Value(Integer v)
{
	basic_value_setFromInteger(&m_value, v);
}

#if USE_REALS
Parser::Value::Value(Real v)
{
	basic_value_setFromReal(&m_value, v);
}
#if USE_LONG_REALS
Parser::Value::Value(LongReal v)
{
	basic_value_setFromLongReal(&m_value, v);
}
#endif
#endif // USE_REALS

Parser::Value::Value(bool v)
{
	basic_value_setFromLogical(&m_value, v);
}

#if USE_REALS
Parser::Value::operator Real() const
{
	return basic_value_toReal(&m_value);
}
#if USE_LONG_REALS
Parser::Value::operator LongReal() const
{
	return basic_value_toLongReal(&m_value);
}
#endif
#endif // USE_REALS

Parser::Value::operator bool() const
{
	return basic_value_toLogical(&m_value);
}

#if USE_LONGINT

Parser::Value::operator LongInteger() const
{
	return basic_value_toLongInteger(&m_value);
}
#endif // USE_LONGINT

Parser::Value::operator Integer() const
{
	return basic_value_toInteger(&m_value);
}

bool
Parser::Value::operator<(const Value &rhs) const
{
	return !((*this > rhs) || (*this == rhs));
}

bool
Parser::Value::operator==(const Value &rhs) const
{
	return basic_value_equals(&m_value, &rhs.m_value);
}

bool
Parser::Value::operator>(const Value &rhs) const
{
	return basic_value_greater(&m_value, &rhs.m_value);
}

bool
operator>=(const Parser::Value &l, const Parser::Value &r)
{
	return l.operator>(r) || l.operator==(r);
}

bool
operator<=(const Parser::Value &l, const Parser::Value &r)
{
	return l.operator<(r) || l.operator==(r);
}

Parser::Value&
Parser::Value::operator+=(const Value &rhs)
{
	basic_value_pluseq(&m_value, &rhs.m_value);
	return *this;
}

Parser::Value&
Parser::Value::operator-=(const Value &rhs)
{
	basic_value_minuseq(&m_value, &rhs.m_value);
	return *this;
}

Parser::Value&
Parser::Value::operator*=(const Value &rhs)
{
	basic_value_multeq(&m_value, &rhs.m_value);
	return *this;
}

// '/' operation always return REAL if real numbers support used
Parser::Value&
Parser::Value::operator/=(const Value &rhs)
{
	basic_value_diveq(&m_value, &rhs.m_value);
	return *this;
}

Parser::Value&
Parser::Value::divEquals(const Value &rhs)
{
#if USE_LONGINT
	if (type() == Value::INTEGER || type() == Value::LOGICAL)
		*this = Integer(Integer(*this) / Integer(rhs));
	else
#endif
		*this = INT(INT(*this) / INT(rhs));
	
	return *this;
}

Parser::Value&
Parser::Value::modEquals(const Value &rhs)
{
#if USE_LONGINT
	if (type() == Value::INTEGER || type() == Value::LOGICAL)
		*this = Integer(Integer(*this) % Integer(rhs));
	else
#endif
		*this = INT(INT(*this) % INT(rhs));
	
	return *this;
}

Parser::Value&
Parser::Value::operator^=(const Value &rhs)
{
	basic_value_poweq(&m_value, &rhs.m_value);
	return *this;
}

void
Parser::Value::switchSign()
{
	basic_value_switchSign(&m_value);
}

size_t
Parser::Value::size(Type t)
{
	switch (t) {
	case INTEGER:
		return sizeof(Integer);
#if USE_LONGINT
	case LONG_INTEGER:
		return sizeof(LongInteger);	
#endif
#if USE_REALS
	case REAL:
		return sizeof(Real);
#if USE_LONG_REALS
	case LONG_REAL:
		return sizeof(LongReal);
#endif
#endif // USE_REALS
	case LOGICAL:
		return sizeof(bool);
	default:
		return 0;
	}
}

Parser::Value&
Parser::Value::operator|=(const Value &v)
{
	basic_value_oreq(&m_value, &v.m_value);
	
	return *this;
}

Parser::Value&
Parser::Value::operator&=(const Value &v)
{
	basic_value_andeq(&m_value, &v.m_value);
	
	return *this;
}

size_t
Parser::Value::printTo(Print& p) const
{
	switch (type()) {
	case LOGICAL:
	{
		char buf[6]; // Size, sufficient to store both 'TRUE' and 'FALSE
		bool res;
		if (m_value.body.logical)
			res = Lexer::getTokenString(Token::KW_TRUE, (uint8_t*)buf);
		else
			res = Lexer::getTokenString(Token::KW_FALSE, (uint8_t*)buf);
		if (res)
			return p.print(buf);
		else
			return 0;
	}
		break;
#if USE_REALS
	case REAL:
	{
		char buf[14];
#ifdef __AVR_ARCH__
		int8_t decWhole = 1;
		Real n = math<Real>::abs(m_value.body.real);
                
		if (n >= Real(1)) {
			while (n >= Real(10)) {
				n /= Real(10);
				++decWhole;
			}
		} else {
			while (n < Real(1)) {
				n *= Real(10);
				--decWhole;
				if (decWhole <= -3)
					break;
			}
		}
		if (decWhole >= -3 && decWhole <= 7)
			::dtostrf(m_value.body.real, 13, 7 - decWhole, buf);
		else
			::dtostre(m_value.body.real, buf, 6, DTOSTR_ALWAYS_SIGN);
#else
		::sprintf(buf, "%- .6G", m_value.body.real);
#endif // ARDUINO
		if (buf[1] == '0' && buf[2] == '.')
			memmove(buf+1, buf+2, 14-2);
		return p.print(buf);
	}
#if USE_LONG_REALS
	case LONG_REAL:
	{
		char buf[16];
		::sprintf(buf, "%- .10G", m_value.body.long_real);
		if (buf[1] == '0' && buf[2] == '.')
			memmove(buf+1, buf+2, 16-2);
		return p.print(buf);
	}
#endif
#endif // USE_REALS
#if USE_LONGINT
	case Parser::Value::LONG_INTEGER:
		if (m_value.body.long_integer >= LongInteger(0))
			p.print(char(ASCII::SPACE));
		return p.print(m_value.body.long_integer) + 1;
#endif // USE_LONGINT
	case Parser::Value::INTEGER:
		if (m_value.body.integer >= Integer(0))
			p.print(char(ASCII::SPACE));
		return p.print(m_value.body.integer) + 1;
	default:
		return p.print(char(ASCII::QMARK));
	}
}

Parser::Value::Type
Parser::Value::typeFromName(const char *fname)
{
	Parser::Value::Type t;
#if USE_LONGINT
	if (endsWith(fname, "%!")) {
		t = Parser::Value::LONG_INTEGER;
	} else
#endif // USE_LONGINT
		if (endsWith(fname, '%')) {
		t = Parser::Value::INTEGER;
	} else if (endsWith(fname, '@')) {
		t = Parser::Value::LOGICAL;
	} else if (endsWith(fname, '$')) {
		t = Parser::Value::STRING;
	}
#if USE_REALS
#if USE_LONG_REALS
         else if (endsWith(fname, '!')) {
		t = Parser::Value::LONG_REAL;
	}
#endif // USE_LONG_REALS
         else {
		t = Parser::Value::REAL;
#else
	 else {
		t = Parser::Value::INTEGER;
#endif // USE_REALS
	}
	return t;
}

} // namespace BASIC
