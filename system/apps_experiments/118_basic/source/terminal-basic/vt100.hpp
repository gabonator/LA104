/*
 * ArduinoExt is a set of utility libraries for Arduino
 * 
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

#ifndef VT100_HPP
#define VT100_HPP

#include <Print.h>
#include <stdint.h>

#include "arduinoext.hpp"

/**
 * @package VT100
 * @brief Package, implementing some vt100/ANSI terminal functions
 */
Package(VT100)
{
	EXT_PACKAGE(VT100);
public:
	// Terminal text attributes to use when printing
	enum TextAttr : uint8_t
	{
		NO_ATTR = 0x0,
		BRIGHT = 0x1,
		UNDERSCORE = 0x2,
		BLINK = 0x4,
		REVERSE = 0x8,
		C_WHITE = 0x00,
		C_BLACK = 0x10,
		C_RED = 0x20,
		C_GREEN = 0x30,
		C_YELLOW = 0x40,
		C_BLUE = 0x50,
		C_MAGENTA = 0x60,
		C_CYAN = 0x70,
		CB_BLACK = 0x80,
		CB_RED = 0x90,
		CB_GREEN = 0xA0,
		CB_YELLOW = 0xB0,
		CB_BLUE = 0xC0,
		CB_MAGENTA = 0xD0,
		CB_CYAN = 0xE0,
		CB_WHITE = 0xF0,
	};

	enum Color : uint8_t
	{
		COLOR_BLACK = 0,
		COLOR_RED,
		COLOR_GREEN,
		COLOR_YELLOW,
		COLOR_BLUE,
		COLOR_MAGENTA,
		COLOR_CYAN,
		COLOR_WHITE,
		NUM_COLORS
	};

	/**
	 * @brief Print print wrapper with vt100 capabilities
	 */
	class Print : public ::Print
	{
		EXT_NOTCOPYABLE(Print)
	public:

		virtual ~Print() = default;
		
		virtual void clear() = 0;

	protected:
		/**
		 * @brief escape codes parsing automata state set
		 */
		enum State_t : uint8_t
		{
			IDLE,      // Initial state without escape sequances
			ESCAPE,    // escape-char was read
			LBRACKET,  // left bracket char was read
			FIRST_NUM, // reading first number code
			SECOND_NUM // reading second number code
		};
		
		Print()	;
		
		virtual void writeChar(uint8_t) = 0;
		virtual uint8_t getCursorX() = 0;
		virtual void setCursorX(uint8_t) = 0;
		virtual void setCursor(uint8_t, uint8_t) = 0;
		virtual void addAttribute(TextAttr) = 0;
		virtual void resetAttributes() = 0;
		
	private:
		void writeIdle(uint8_t);
		void writeESC(uint8_t);
		void writeLbracket(uint8_t);
		void writeFirstNum(uint8_t);
		void writeSecondNum(uint8_t);

		State_t _state;
		uint16_t _value, _value2;
		// Print zone width
		uint8_t _pZoneWidth;
	// Print interface
	public:
		size_t write(uint8_t) override;
	};
};

inline VT100::TextAttr& operator |= (VT100::TextAttr& left, VT100::TextAttr right)
{
	reinterpret_cast<uint8_t&>(left) |= uint8_t(right);
	return left;
}

inline VT100::TextAttr& operator &= (VT100::TextAttr& left, VT100::TextAttr right)
{
	reinterpret_cast<uint8_t&>(left) &= uint8_t(right);
	return left;
}

#endif // VT100_HPP

