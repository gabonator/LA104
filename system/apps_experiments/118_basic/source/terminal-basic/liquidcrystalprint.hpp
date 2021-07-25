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
	
#ifndef LIQUIDCRYSTALPRINT_HPP
#define LIQUIDCRYSTALPRINT_HPP

#include "vt100.hpp"
#include "LiquidCrystal.h"

class LiquidCrystalVt100 : public VT100::Print
{
	EXT_NOTCOPYABLE(LiquidCrystalVt100)
public:
	LiquidCrystalVt100(LiquidCrystal&, uint8_t, uint8_t, uint8_t*);
	void clear() override;
protected:
	uint8_t getCursorX() override;
	void setCursor(uint8_t, uint8_t) override;
	void setCursorX(uint8_t) override;
	void writeChar(uint8_t) override;
private:
	void scroll();
	LiquidCrystal &_crystal;
	uint8_t _x,_y, _w, _h;
	uint8_t *_buffer;
};

#endif
