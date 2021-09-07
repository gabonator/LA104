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

#ifndef BASIC_LEXER_H
#define BASIC_LEXER_H

#include "basic.h"
#include "basic_value.h"

__BEGIN_DECLS

/**
 * @brief lexical analyses stage errors
 */
typedef enum basic_lexer_error
{
	BASIC_LEXER_ERROR_NOERROR = 0,
	BASIC_LEXER_ERROR_STRING_OVERFLOW = 1
} basic_lexer_error_t;

struct basic_lexer_context
{
	const uint8_t *string_to_parse;
	/* position in the parsing string */
	uint8_t string_pointer;
	
	basic_token_t token;
	
	/* current identifier string */
	char _id[STRING_SIZE];
	/* Identifier string pointer */
	uint8_t _value_pointer;
	/* scanned numeric/logical value */
	basic_value_t value;
	/* scanner error */
	basic_lexer_error_t _error;
	
	BOOLEAN tokenized;
};

/**
 * @brief Initialize lexer by the string
 * @param self lexer context
 * @param str ASCII or tokenized basic string
 * @param tokenized
 */
void basic_lexer_init(basic_lexer_context_t*, const uint8_t*, BOOLEAN);

/**
 * @brief Get next token from ASCII string
 * @param self lexer context
 * @return true if there are more symbols to scan
 */
BOOLEAN basic_lexer_getnextPlain(basic_lexer_context_t*);

/**
 * @brief Get next token from tokenized string
 * @param self lexer context
 * @return true if there are more symbols to scan
 */
BOOLEAN basic_lexer_getnextTokenized(basic_lexer_context_t*);

BOOLEAN basic_lexer_getNext(basic_lexer_context_t*);

/**
 * @brief Get token text representation to buffer
 * @param t token
 * @param buf char buffer of appropriate length 
 */
BOOLEAN basic_lexer_tokenString(basic_token_t, uint8_t*);

/**
 * @brief tokenize a string
 * @param self Lexer context
 * @param dst destination buffer
 * @param dstsize length of the dst buffer
 * @param src source buffer
 * @return size of the actually tokenized buffer
 */
uint8_t basic_lexer_tokenize(basic_lexer_context_t*, uint8_t*, uint8_t,
    const uint8_t*);

__END_DECLS

#endif /* BASIC_LEXER_H */

