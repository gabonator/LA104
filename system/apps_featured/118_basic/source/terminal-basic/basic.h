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

#ifndef BASIC_H
#define BASIC_H

#include "sys/cdefs.h"
#include <stdint.h>

#include "basic_config.h"

#if USE_REALS
#include <math.h>
#endif

__BEGIN_DECLS

typedef uintptr_t pointer_t;

/**
 * @brief ASCII characters
 */
typedef enum ascii_codes
{
	ASCII_NUL = 0x00, /* Null */
	SOH = 0x01, /* Start of heading */
	STX = 0x02, /* Start of text */
	ASCII_ETX = 0x03, /* End of text */
	EOT = 0x04, /* End of transmission */
	ENQ = 0x05, /* Enquire */
	ACK = 0x06, /* Acknowledgement */
	BEL = 0x07, /* Bell */
	BS = 0x08,  /* Backspace */
	HT = 0x09,  /* Horizontal tab */
	LF = 0x0A,  /* Line feed */
	VT = 0x0B,  /* Vertical tab */
	FF = 0x0C,  /* Form feed */
	CR = 0x0D,  /* Carriage return */
	SO = 0x0E,  /* Shift out */
	SI = 0x0F,  /* Shift in */
	ASCII_DLE = 0x10, /* Data link escape */
	DC1 = 0x11, /* Device control 1 */
	DC2 = 0x12, /* Device control 2 */
	DC3 = 0x13, /* Device control 3 */
	DC4 = 0x14, /* Device control 4 */
	NAK = 0x15, /* Negative acknowledgement */
	SYN = 0x16, /* Synchronous idle */
	ETB = 0x17, /* End of transmission block */
	CAN = 0x18, /* Cancel */
	ESC = 0x1B, /* Escape */
	FS = 0x1C,  /* File separator */
	GS = 0x1D,  /* Group separator */
	RS = 0x1E,  /* Record separator */
	US = 0x1F,  /* Unit separator */
	SPACE = ' ',
	QMARK = '?',
	DOT = '.',
	COLON = ':',
	SEMICOLON = ';',
	QUMARK = '\"',
	LPAREN = '(',
	RPAREN = ')',
	COMMA = ',',
	GT = '>',
	LT = '<',
	TILDE = 0x7E,
	DEL = 0x7F
} ascii_codes_t;

// integer type
typedef int16_t integer_t;
#define MAX_INTEGER (integer_t)(INT16_MAX)
#if USE_LONGINT
// long integer type
typedef int32_t long_integer_t;
#define MAX_LONG_INTEGER (long_integer_t)(INT32_MAX)
typedef long_integer_t INT;
#define MAXINT MAX_LONG_INTEGER
#else
typedef integer_t INT;
#define MAXINT MAX_INTEGER
#endif // USE_LONGINT
// floating point type
#if USE_REALS
typedef float real_t;
#if USE_LONG_REALS
typedef double long_real_t;
#endif
#endif // USE_REALS

/**
 * @brief Scan token table
 * @param token
 * @param table
 * @param index
 * @return find flag
 */
uint8_t *scanTable(const uint8_t*, const uint8_t[], uint8_t*);

/**
 * 
 * @param table [in]
 * @param index [in]
 * @param tokenString [out]
 */
void getToken(const uint8_t*, uint8_t, uint8_t*);

#if CONF_LEXER_LANG == LANG_EN
#include "_tokens_en.h"
#elif CONF_LEXER_LANG == LANG_RU
#include "_tokens_ru.h"
#elif CONF_LEXER_LANG == LANG_FR
#include "_tokens_fr.h"
#else
#error You should define CONF_LEXER_LANG to one of the supported languages
#endif // CONF_LANG

struct basic_lexer_context;
typedef struct basic_lexer_context basic_lexer_context_t;

struct basic_parser_context;
typedef struct basic_parser_context basic_parser_context_t;

__END_DECLS

#endif /* BASIC_H */
