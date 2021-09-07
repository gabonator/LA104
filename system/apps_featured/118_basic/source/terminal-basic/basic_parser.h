/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * Copyright (C) 2017-2019 Andrey V. Skvortsov <starling13@mail.ru>
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

#ifndef BASIC_PARSER_H
#define BASIC_PARSER_H

#include "basic.h"
#include "tools.h"

__BEGIN_DECLS

/**
 * Parser mode: syntax check or execute commands of the interpreter
 * context
 */
typedef enum
{
	BASIC_PARSER_SCAN = 0,
	BASIC_PARSER_EXECUTE = 1
} basic_parser_mode_t;

struct basic_parser_context {
    	// current mode
	uint8_t mode;
	// stop parsing string flag
	BOOLEAN stopParse;
};

/**
 * 
 * @param self context
 * @param str string to parse
 * @param success flag of successfull execution
 * @return true if there are more operators in string
 */
BOOLEAN basic_parser_parse(basic_parser_context_t*, const char*, BOOLEAN*);

BOOLEAN basic_parser_fidenifier(basic_parser_context_t*, const char*);

__END_DECLS

#endif /* BASIC_PARSER_H */

