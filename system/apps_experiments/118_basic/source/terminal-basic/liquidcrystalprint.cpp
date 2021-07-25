/*
 * ArduinoExt is a set of utility libraries for Arduino
 * Copyright (C) 2016, 2017 Andrey V. Skvortsov <starling13@mail.ru>
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

#include <string.h>

#include "liquidcrystalprint.hpp"
#include "ascii.hpp"

LiquidCrystalVt100::LiquidCrystalVt100(LiquidCrystal &lc, uint8_t w, uint8_t h,
    uint8_t *buffer) :
_crystal(lc), _w(w), _h(h), _buffer(buffer)
{
}

void
LiquidCrystalVt100::clear()
{
	_crystal.clear();
	_x=_y=0;
	_crystal.setCursor(0,0);
        memset(_buffer, ' ', _w*_h);
}

uint8_t
LiquidCrystalVt100::getCursorX()
{
	return _x;
}

void
LiquidCrystalVt100::setCursor(uint8_t x, uint8_t y)
{
	if (x >= _w)
		x = _w-1;
	if (y >= _h)
		y = _h-1;
	_x=x; _y=y;
	_crystal.setCursor(x,y);
}

void
LiquidCrystalVt100::setCursorX(uint8_t x)
{
	while (x >= _w) {
		x -= _w;
		_x = 0;
		scroll();
	}
	_x = x;
	_crystal.setCursor(_x, _y);
}

void
LiquidCrystalVt100::writeChar(uint8_t c)
{
	switch (c) {
	case '\n':
		scroll(); break;
	case '\r':
		_x = 0;
		_crystal.setCursor(_x, _y); break;
	case uint8_t(ASCII::BS) :
		if (_x > 0)
			--_x;
		else if (_y > 0) {
			--_y;
			_x = _w-1;
		}
		_crystal.setCursor(_x, _y);
		_crystal.write(' ');
		_crystal.setCursor(_x, _y);
		_buffer[_y*_w+_x] = ' ';
		break;
	default:
		_crystal.write(c);
		_buffer[_y*_w+_x] = c;
		if (_x < (_w-1))
			++_x;
		else {
			scroll();
			_x = 0;
			_crystal.setCursor(_x, _y);
		}	
	}
}

void
LiquidCrystalVt100::scroll()
{
	if (_y < (_h-1)) {
		++_y;
	} else {
		_crystal.noCursor();
		memmove(_buffer, _buffer+_w, _w*(_h-1));
		for (uint8_t y=0; y<(_h-1); ++y) {
			_crystal.setCursor(0,y);
			for (uint8_t x=0; x<_w; ++x)
				_crystal.write(_buffer[(_w*y)+x]);
		}
		_crystal.setCursor(0, _h-1);
		for (uint8_t x=0; x<_w; ++x) {
			_crystal.write(uint8_t(' '));
			_buffer[_w*(_h-1)+x] = ' ';
		}
		_crystal.cursor();
	}
	_crystal.setCursor(_x, _y);
}
