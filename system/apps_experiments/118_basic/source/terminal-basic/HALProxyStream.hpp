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

#ifndef HALPROXYSTREAM_HPP
#define HALPROXYSTREAM_HPP

#include "HAL.h"
#include <Stream.h>

namespace BASIC
{

class HALProxyStream : public Stream
{
public:
	
	HALProxyStream(uint8_t);
	
	void begin(int) {}
	
private:
	
	const uint8_t m_term;
	
	bool m_hasByte;
	
	uint8_t m_byte;
	
// Stream interface
public:
	
	int available() override;
	
	size_t write(uint8_t) override;
	
	void flush() override;
	
	int peek() override;
	
	int read() override;
};

} // namespace BASIC

#endif // HALPROXYSTREAM_HPP

