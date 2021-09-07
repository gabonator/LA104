/*
 * ArduinoExt is a set of utility libraries for Arduino
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
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

#ifndef BYTEARRAY_HPP
#define BYTEARRAY_HPP

#include <inttypes.h>

#include "Printable.h"

class ByteArray : public Printable
{
public:
	ByteArray();
	ByteArray(const uint8_t*, size_t);
	ByteArray(const char *c, size_t s) :
	  ByteArray(reinterpret_cast<const uint8_t*>(c), s) {}
	
	void createData();

	size_t printTo(Print& p) const override;

	const uint8_t *data() const
	{
		return (_data);
	}
	
	uint8_t *data()
	{
		return (_data);
	}

	size_t size() const
	{
		return (_size);
	}
private:
	uint8_t *_data;
	size_t _size;
	bool _ownsData;
};

#endif // BYTEARRAY_HPP
