/*
 * ArduinoExt is a set of utility libraries for Arduino
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
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

#ifndef TYPES_HPP
#define TYPES_HPP

#include <inttypes.h>

template <unsigned char size>
class type_factory
{
public:
	using unsigned_type = uint8_t;
};

template <>
class type_factory<2>
{
public:
	using unsigned_type = uint16_t;
};

template <>
class type_factory<4>
{
public:
	using unsigned_type = uint32_t;
};

template <>
class type_factory<8>
{
public:
	using unsigned_type = uint64_t;
};

template <typename T>
class typespec
{
public:
	typedef void longer;
	typedef void shorter;
	
	static constexpr bool isinteger = false;
	static constexpr bool isreal = false;
};

template <>
class typespec<float>
{
public:
	typedef double longer;
	typedef float shorter;
	
	static constexpr bool isinteger = false;
	static constexpr bool isreal = true;
};

template <>
class typespec<double>
{
public:
	typedef long double longer;
	typedef float shorter;
	
	static constexpr bool isinteger = false;
	static constexpr bool isreal = true;
};

template <>
class typespec<int>
{
public:
	typedef long longer;
	typedef short shorter;
	
	static constexpr bool isinteger = true;
	static constexpr bool isreal = false;
};

template <>
class typespec<short>
{
public:
	typedef int longer;
	typedef signed char shorter;
	
	static constexpr bool isinteger = true;
	static constexpr bool isreal = false;
};

#endif
