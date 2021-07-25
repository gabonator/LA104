/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
 *     <https://bitbucket.org/%7Bf50d6fee-8627-4ce4-848d-829168eedae5%7D/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TVOUT_PRINT_HPP
#define TVOUT_PRINT_HPP

#include "basic.hpp"

#if USETVOUT

#include "TVoutEx.h"
#include "vt100.hpp"

/**
 * @brief TVoutEx print wrapper with vt100 capabilities
 */
class TVoutPrint : public VT100::Print
{
	EXT_NOTCOPYABLE(TVoutPrint)
public:
	/**
	 * @brief default constructor
	 * @param tv TVoutEx object instance to wrap
	 */
	explicit TVoutPrint();

	~TVoutPrint() = default;

private:
	void writeLbracket(uint8_t);
	void writeFirstNum(uint8_t);
	void writeSecondNum(uint8_t);
// VT100::Print interface
public:
	void clear() override;
protected:
	void writeChar(uint8_t) override;
	uint8_t getCursorX() override;
	void setCursorX(uint8_t) override;
	void setCursor(uint8_t, uint8_t) override;
	void addAttribute(VT100::TextAttr) override {};
	void resetAttributes() override {};
};

#endif // USETVOUT

#endif /* TVOUT_PRINT_HPP */
