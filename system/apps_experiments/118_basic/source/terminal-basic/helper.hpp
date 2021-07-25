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

#ifndef HELPER_HPP
#define HELPER_HPP

#include "arduinoext.hpp"
#include <string.h>

#ifdef ARDUINO
#include <Arduino.h>

inline void
positiveLedBlink(size_t num = 1)
{
	for (size_t i = 0; i<num; ++i) {
		digitalWrite(13, HIGH);
		delay(100);
		digitalWrite(13, LOW);
		delay(100);
	}
}

inline void
negativeLedBlink(size_t num = 1)
{
	for (size_t i = 0; i < num; ++i) {
		digitalWrite(13, HIGH);
		delay(500);
		digitalWrite(13, LOW);
		delay(300);
	}
}

#else

#ifndef ARDUINO
#define positiveLedBlink(a...)
#define negativeLedBlink(a...)
#endif

#endif

inline bool
endsWith(const char *str, const char end)
{
	size_t l = strlen(str);
	if (str == NULL || l==0)
		return false;
	if (end == 0)
		return true;
	if (str[l-1] == end)
		return true;
	return false;
}

inline bool
endsWith(const char *str, const char *end)
{
	const size_t l1 = strlen(str);
	const size_t l2 = strlen(end);
	
	if (str == NULL || l1 == 0 || l2 == 0 || l1 < l2)
		return false;
	else if (strcmp(str+l1-l2, end) == 0)
		return true;
	return false;
}

#endif // HELPER_HPP
