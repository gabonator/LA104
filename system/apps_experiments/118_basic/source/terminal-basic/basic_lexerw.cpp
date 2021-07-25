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

#include "basic_lexer.hpp"
#include "helper.hpp"

#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

/*
 * OP_AND = "AND"       // 1
 * KW_ARRAYS = "ARRAYS" // 1
 * KW_BASE = "BASE"     // 2
 * COM_CHAIN = "CHAIN"  // 3
 * COM_CLS = "CLS"      // 4
 * KW_CON = "CON"
 * KW_DATA = "DATA"     // 5
 * KW_DEF = "DEF"       // 6
 * KW_DET = "DET"
 * COM_DELAY = "DELAY"  // 7
 * KW_DIM = "DIM"       // 8
 * KW_DO = "DO"
 * COM_DUMP = "DUMP"    // 9
 * KW_END = "END"       // 10
 * KW_FALSE = "FALSE"   // 11
 * KW_FOR = "FOR"       // 12
 * KW_GOSUB = "GOSUB"   // 13
 * KW_GOTO = "GOTO"     // 14
 * KW_GO = "GO"         // 15
 * KW_IDN = "IDN"
 * KW_IF = "IF"         // 16
 * KW_INPUT = "INPUT"   // 17
 * KW_INV = "INV"
 * KW_LET = "LET"       // 18
 * COM_LIST = "LIST"    // 19
 * COM_LOAD = "LOAD"    // 20
 * KW_MAT = "MAT"
 * COM_NEW = "NEW"      // 21
 * KW_NEXT = "NEXT"     // 22
 * OP_NOT = "NOT"
 * KW_ON = "ON"         // 23
 * KW_OPTION = "OPTION" // 24
 * OP_OR = "OR"
 * KW_PRINT = "PRINT"   // 25
 * KW_RANDOMIZE = "RANDOMIZE"
 * KW_READ = "READ"
 * KW_REM = "REM"
 * KW_RETURN = "RETURN"
 * COM_RUN = "RUN"
 * COM_SAVE = "SAVE"
 * KW_STEP = "STEP"
 * KW_STOP = "STOP"
 * KW_TAB = "TAB"
 * KW_THEN = "THEN"
 * KW_TO = "TO"
 * KW_TRN = "TRN"
 * KW_TRUE = "TRUE"
 * KW_VARS = "VARS"
 * KW_ZER = "ZER"
 *
 * STAR = '*'
 * SLASH = '/'
 * PLUS = '+'
 * COLON = ':'
 * SEMI = ';'
 * LT = '<'
 * LTE = "<="
 * GT = '>'
 * GTE = ">="
 * EQUALS = '='
 * NE = "<>"
 * NEA = "><"
 * MINUS = '-'
 * POW = '^'
 * IDENT = [A-Z][A-Z0-9]*
 * C_INTEGER = [0-9]+
 * C_REAL = [0-9]+[.][0-9]*
 */

namespace BASIC
{

#if ARDUINO_LOG

Logger&
operator<<(Logger &logger, Token tok)
{
	char buf[12];
	strcpy_P(buf, (PGM_P) pgm_read_word(&(Lexer::tokenStrings[uint8_t(tok)])));

	logger.log(buf);
	return (logger);
}
#endif

#define SYM (uint8_t(_string[_pointer]))

bool
Lexer::getTokenString(Token t, uint8_t *buf)
{
	return basic_lexer_tokenString(basic_token_t(t), buf);
}

void
Lexer::init(const uint8_t *string, bool tok)
{
	basic_lexer_init(&m_context, string, tok);
}

bool
Lexer::getNext()
{
	return basic_lexer_getNext(&m_context);
}

uint8_t
Lexer::tokenize(uint8_t* dst, uint8_t dstlen, const uint8_t* src)
{
	return basic_lexer_tokenize(&m_context, dst, dstlen, src);
}

} // namespace BASIC
