/*
 * ArduinoExt is a set of utility libraries for Arduino
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019 Terminal-BASIC team
 *     <https://bitbucket.org/%7Bf50d6fee-8627-4ce4-848d-829168eedae5%7D/>
 *
 * This program is free software: is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * ArduinoExt library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with Posixcpp library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "vt100.hpp"

#include "ascii.hpp"

#include <ctype.h>

VT100::Print::Print() :
_state(IDLE), _pZoneWidth(10)
{
}

void
VT100::Print::writeIdle(uint8_t c)
{
	switch (c) {
	case uint8_t(ASCII::ESC) :
		// ESC symbol - begin ANSI sequance
		_state = ESCAPE;
		break;
	case uint8_t('\t') : {
		// TAB symbol, deal with print zones
		const uint8_t pZone = this->getCursorX() / this->_pZoneWidth;
		this->setCursorX((pZone+1)*this->_pZoneWidth);
		break;
	}
	default:
		// Simply write symbol
		this->writeChar(c);
	}
}

void
VT100::Print::writeESC(uint8_t c)
{
	_value = _value2 = 0;
	switch (c) {
	case uint8_t('[') :
		_state = LBRACKET;
		break;
	default:
		_state = IDLE;
	}
}

void
VT100::Print::writeLbracket(uint8_t c)
{
	switch (c) {
	case 'H':
		this->setCursor(0, 0);
		break;
	default:
		if (isdigit(c)) {
			_value = c-'0';
			_state = FIRST_NUM;
			return;
		}
		break;
	}
	_state = IDLE;
}

void
VT100::Print::writeFirstNum(uint8_t c)
{
	switch (c) {
	case 'J': // \x1B2J clear screen
		if (_value == 2)
			this->clear();
		break;
	case 'm':
		switch (_value) {
			case 0 :
				resetAttributes(); break;
			case 1 :
				addAttribute(BRIGHT); break;
			case 30 :
				addAttribute(C_BLACK); break;
			case 31 :
				addAttribute(C_RED); break;
			case 32 :
				addAttribute(C_GREEN); break;
			case 33 :
				addAttribute(C_YELLOW); break;
			case 34 :
				addAttribute(C_BLUE); break;
			case 35 :
				addAttribute(C_MAGENTA); break;
			case 36 :
				addAttribute(C_CYAN); break;
			case 37 :
				addAttribute(C_WHITE); break;
		}
		break;
	case 'C': {
		const auto x = this->getCursorX() + 1;
		this->setCursorX(_value + x);
	} break;
	case 'D': {
		const auto x = this->getCursorX();
		if (_value > x)
			_value = x;
		this->setCursorX(x-_value);
	} break;
	case ';':
		_state = SECOND_NUM;
		_value2 = 0;
		return;
	default:
		if (isdigit(c)) {
			_value *= 10;
			_value += c-'0';
			return;
		}
	}
	_state = IDLE;
}

void
VT100::Print::writeSecondNum(uint8_t c)
{
	switch (c) {
	case 'f':
	case 'H':
		this->setCursor(_value2-1, _value-1);
		_state = IDLE;
		break;
	default:
		if (isdigit(c)) {
			_value2 *= 10;
			_value2 += c-'0';
			return;
		}	
	}
}

size_t
VT100::Print::write(uint8_t c)
{
	switch (_state) {
	case IDLE:
		writeIdle(c);
		break;
	case ESCAPE:
		writeESC(c);
		break;
	case LBRACKET:
		writeLbracket(c);
		break;
	case FIRST_NUM:
		writeFirstNum(c);
		break;
	case SECOND_NUM:
		writeSecondNum(c);
		break;
	}
	return 1;
}
