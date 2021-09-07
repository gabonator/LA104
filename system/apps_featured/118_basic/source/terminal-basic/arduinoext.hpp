/*
 * ArduinoExt is a set of utility libraries for Arduino
 * Copyright (C) 2016-2020 Andrey V. Skvortsov <starling13@mail.ru>
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

#ifndef EXT_HPP
#define EXT_HPP

#if __cplusplus <  201103L
#error This file requires C++11-able compiler
#endif

// Packed data object
#define EXT_PACKED __attribute__ ((__packed__))

// Class of not copyable objects
#define EXT_NOTCOPYABLE(ClassName)        \
private:                                  \
  ClassName(const ClassName&) = delete;   \
  ClassName(ClassName&&) = delete;        \
  ClassName &operator =(const ClassName&) = delete;

// Not instantiable class
#define EXT_STATIC(ClassName)        \
private:                             \
  ClassName() = delete;              \
  EXT_NOTCOPYABLE(ClassName)

// Class-package (Ada nostalgie)
#define Package(ClassName) class ClassName final
#define EXT_PACKAGE(ClassName) EXT_STATIC(ClassName) \

// Interface class (Wanna be java)
#define Interface(ClassName) class ClassName
#define EXT_INTERFACE (ClassName) \
public: \
  virtual ~ClassName() = default; \
protected: \
  ClassName() = default; \
  EXT_NOTCOPYABLE(ClassName)

#ifdef swap
#undef swap
#endif
template <typename T>
void swap(T &r, T &l)
{
	T buf = r;
	r = l;
	l = buf;
}

#endif //EXT_HPP
