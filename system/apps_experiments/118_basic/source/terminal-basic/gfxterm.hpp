/*
 * ArduinoExt is a set of utility libraries for Arduino
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

#ifndef GFXTERM_HPP
#define GFXTERM_HPP

#include "ascii.hpp"
#include "arduinoext.hpp"

Package(GFXTERM)
{
	EXT_PACKAGE(GFXTERM)
	    
public:
	
	enum class Command : uint8_t
	{
		NONE = 0x00,
		// Set terminal video mode
		// param m - uint8_t mode number
		MODE,
		// Set text cursor visible
		// param bool (uint8_t) visible
		CURSOR,
		// Set colors (ink and paper)
		// param uint8_t ink, uint8_t paper
		COLOR,
		// Set active font
		// param uint8_t font index
		FONT,
		// Draw circle
		// param int16_t cx, cy, r
		CIRCLE,
		// Draw circle
		// param int16_t cx, cy, r, uint8_t ink, paper
		CIRCLEC,
		// Draw box (rectangle)
		// param int16_t x,y, w,h
		BOX,
		// Draw color box (rectangle)
		// param int16_t x,y, w,h, uint8_t ink, paper
		BOXC,
		// Draw ellipse
		// param int16_t x,y, w,h
		ELLIPSE,
		// Draw color ellipse
		// param int16_t x,y, w,h, uint8_t ink, paper
		ELLIPSEC,
		// Draw line
		// param int16_t x1,y1, x2,y2
		LINE,
		// Draw color line
		// param int16_t x1,y1, x2,y2, uint8_t ink
		LINEC,
		// Draw line from current position
		// param int16_t x,y
		LINETO,
		// Draw color line from current position
		// param int16_t x,y, uint8_t ink
		LINETOC,
		// Draw point
		// param int16_t x,y
		POINT,
		// Set ink color and draw point
		// param int16_t x,y, uint8_t ink
		POINTC,
		
		NUM_COMMANDS
	};
};

#endif /* GFXTERM_HPP */
