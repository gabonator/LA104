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

#include "bytearray.hpp"

#include "Print.h"
#include "ascii.hpp"
#include <stdlib.h>

ByteArray::ByteArray() :
_data(nullptr), _size(0), _ownsData(false)
{
}

ByteArray::ByteArray(const uint8_t *data, size_t size) :
_data(const_cast<uint8_t*>(data)), _size(size), _ownsData(false)
{
}

void
ByteArray::createData()
{
	if (_ownsData && _data != nullptr)
		free(_data);
	
	if (_size == 0) {
		_data = nullptr;
		return;
	}
	
	_data = reinterpret_cast<uint8_t*>(malloc(_size));
}

size_t
ByteArray::printTo(Print& p) const
{
	size_t res = 0;
	for (size_t i = 0; i < size();) {
		size_t ii = i;

		// Leading zeros of the absolute address
		size_t addr = i + uintptr_t(data());
		uint8_t digits;
		
		// Leading zeros of the relative address
		addr = i;
		for (digits = 0; addr > 15; addr >>= 4, ++digits);
		while (++digits < sizeof(intptr_t)*2)
			p.print('0');
		
		p.print(i, HEX), p.print(':');
		size_t j;
		for (j = 0; j < 8; ++j, ++ii) {
			if (ii >= size())
				break;
			uint8_t c = data()[ii];
			res += p.print(char(ASCII::SPACE));
			if (c < 0x10)
				res += p.print('0');
			res += p.print(c, HEX);
		}
		j*=3;
		for (; j < 8*3; ++j)
			res += p.print(char(ASCII::SPACE));
		res += 2; p.print("  ");
		for (j = 0; j < 8; ++j, ++i) {
			if (i >= size())
				break;
			const signed char c = ((const signed char*)data())[i];
			if (c < ' ')
				res += p.print('.');
			else
				res += p.print((char)c);
		}
		res += p.println();
	}
	return res;
}
