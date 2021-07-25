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

#include "avr/pgmspace.h"
#include "basic_config.h"
#include "basic.h"

#if CONF_LEXER_LANG == LANG_RU

const uint8_t _basic_lexer_symbolsShift PROGMEM = (uint8_t)('Ä')-'A';

const uint8_t _basic_lexer_tokenTable[] PROGMEM = {
	ASCII_NUL,
#if USE_DUMP
	'A', 'R', 'R', 'A', 'Y', 'S',ASCII_NUL, // 2
#endif
#if USE_SAVE_LOAD
	'C', 'H', 'A', 'I', 'N', ASCII_NUL,      // 4
#endif
#if USE_TEXTATTRIBUTES
	'C', 'L', 'S', ASCII_NUL,                // 5
#endif
#if USESTOPCONT
	'C', 'O', 'N', 'T', ASCII_NUL,           // 6
#endif
#if USE_MATRIX
	'C', 'O', 'N', ASCII_NUL,                // 7
#endif
#if USE_DELAY
	'D', 'E', 'L', 'A', 'Y', ASCII_NUL,      // 10
#endif
#if USE_MATRIX
	'D', 'E', 'T', ASCII_NUL,                // 11
#endif
#if USE_DIV_KW
	'D', 'I', 'V', ASCII_NUL,                // 13
#endif
#if USE_DUMP
	'D', 'U', 'M', 'P', ASCII_NUL,           // 15
#endif
	'F', 'A', 'L', 'S', 'E', ASCII_NUL,
	'G', 'O', 'S', 'U', 'B', ASCII_NUL,      // 20
#if CONF_SEPARATE_GO_TO
	'G', 'O', ASCII_NUL,                     // 12
#endif
#if USE_MATRIX
	'I', 'D', 'N', ASCII_NUL,
	'I', 'N', 'V', ASCII_NUL,
#endif
#if USE_SAVE_LOAD
	'L', 'O', 'A', 'D', ASCII_NUL,           // 20
#endif
#if USE_TEXTATTRIBUTES
	'L', 'O', 'C', 'A', 'T', 'E', ASCII_NUL, // 21
#endif
#if USE_INTEGER_DIV
	'M', 'O', 'D', ASCII_NUL,                // 24
#endif
#if USE_PEEK_POKE
	'P', 'O', 'K', 'E', ASCII_NUL,
#endif
#if USE_RANDOM
	'R', 'A', 'N', 'D', 'O', 'M', 'I', 'Z', 'E', ASCII_NUL, //26
#endif
#if USE_SAVE_LOAD
	'S', 'A', 'V', 'E', ASCII_NUL,
#endif
#if CONF_USE_SPC_PRINT_COM
	'S', 'P', 'C', ASCII_NUL,
#endif
#if USE_TEXTATTRIBUTES
	'T', 'A', 'B', ASCII_NUL,
#endif
#if USE_MATRIX
	'T', 'R', 'N', ASCII_NUL,
#endif
	'T', 'R', 'U', 'E', ASCII_NUL,
#if USE_DUMP
	'V', 'A', 'R', 'S', ASCII_NUL,
#endif
	'X', 'O', 'R', ASCII_NUL,
#if USE_MATRIX
	'Z', 'E', 'R',ASCII_NUL,
#endif
	'Ç', 'Ç', 'é', 'Ñ', ASCII_NUL,
#if USE_DATA
	'Ç', 'á', 'ü', 'í', 'ú', ASCII_NUL,
#endif
	'Ç', 'é', 'á', 'Ç', 'ê', 'Ä', 'í', ASCII_NUL,
	'Ç', 'õ', 'Ç', 'é', 'Ñ', ASCII_NUL,
#if USE_DATA
	'Ñ', 'Ä', 'ç', 'ç', 'õ', 'Ö', ASCII_NUL,
#endif
	'Ñ', 'ã', 'ü', ASCII_NUL,
	'Ñ', 'é', ASCII_NUL,
	'Ö', 'ë', 'ã', 'à', ASCII_NUL,
	'à', 'ã', 'à', ASCII_NUL,
        'à', ASCII_NUL,
	'ä', 'é', 'å', ASCII_NUL,
	'ä', 'é', 'ç', 'Ö', 'ñ', ASCII_NUL,
	'ã', 'à', 'ë', 'í', 'Ä', 'í', 'ú',  ASCII_NUL,
#if USE_MATRIX
	'å', 'Ä', 'í', ASCII_NUL,                // 23
#endif
	'ç', 'Ä', ASCII_NUL,
	'ç', 'Ö', ASCII_NUL,
	'ç', 'é', 'Ç', 'Ä', 'ü', ASCII_NUL,      // 21
#if USE_DEFFN
	'é', 'è', 'ê', ASCII_NUL,
#endif
#if CONF_USE_ON_GOTO
	'è', 'ê', 'à', ASCII_NUL,
#endif
	'è', 'ì', 'ë', 'ä', ASCII_NUL,
	'è', 'ì', 'ë', 'í', 'ú', ASCII_NUL,
	'ê', 'Ä', 'á', 'å', 'Ö', 'ê', ASCII_NUL,
#if USE_DATA
	'ë', 'ç', 'é', 'Ç', 'Ä', ASCII_NUL,
#endif
#if USESTOPCONT
	'ë', 'í', 'é', 'è', ASCII_NUL,
#endif
	'í', 'é', ASCII_NUL,
#if USE_DEFFN
	'î', 'ì', 'ç', 'ä', ASCII_NUL,
#endif
	'ñ', 'à', 'ä', 'ã', ASCII_NUL,
	'ò', 'Ä', 'É', ASCII_NUL,
	ASCII_ETX
};

#endif  // CONF_LEXER_LANG
