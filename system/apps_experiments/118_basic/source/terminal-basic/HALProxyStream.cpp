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

#include "HALProxyStream.hpp"

namespace BASIC
{

HALProxyStream::HALProxyStream(uint8_t term) :
    m_term(term), m_hasByte(false)
{
}

int
HALProxyStream::available()
{
	return HAL_terminal_isdataready(m_term) ? 1 : 0;
}

size_t
HALProxyStream::write(uint8_t byte)
{
	HAL_terminal_write(m_term, byte);
	return 1;
}

void
HALProxyStream::flush()
{
}

int
HALProxyStream::peek()
{
	if (m_hasByte) {
		m_hasByte = false;
	} else if (HAL_terminal_isdataready(m_term)) {
		m_byte = HAL_terminal_read(m_term);
		m_hasByte = true;
	} else
		return -1;
	return m_byte;
}

int
HALProxyStream::read()
{
	if (m_hasByte)
		return  m_byte;
	else if (HAL_terminal_isdataready(m_term))
		return HAL_terminal_read(m_term);
	else
		return -1;
		
}

} // namespace BASIC
