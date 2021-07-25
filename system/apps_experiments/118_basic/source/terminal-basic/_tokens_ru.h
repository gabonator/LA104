/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * Copyright (C) 2017-2020 Andrey V. Skvortsov <starling13@mail.ru>
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

#include "basic.h"

/**
 * @brief lexical tokens
 */
typedef enum basic_token
{
	BASIC_TOKEN_NOTOKEN = 0,  // 0
#if USE_DUMP
	BASIC_TOKEN_KW_ARRAYS,    // 2
#endif
#if USE_SAVE_LOAD
	BASIC_TOKEN_COM_CHAIN,     // 3
#endif
#if USE_TEXTATTRIBUTES
	BASIC_TOKEN_COM_CLS,       // 4
#endif
#if USESTOPCONT
	BASIC_TOKEN_COM_CONT,      // 6
#endif
#if USE_MATRIX
	BASIC_TOKEN_KW_CON,        // 7
#endif
#if USE_DELAY
	BASIC_TOKEN_COM_DELAY,     // 10
#endif
#if USE_MATRIX
	BASIC_TOKEN_KW_DET,        // 11
#endif
#if USE_DIV_KW
	BASIC_TOKEN_KW_DIV,        // 13
#endif
#if USE_DUMP
	BASIC_TOKEN_COM_DUMP,      // 15
#endif
	BASIC_TOKEN_KW_FALSE,      // 17
	BASIC_TOKEN_KW_GOSUB,      // 20
#if CONF_SEPARATE_GO_TO
	BASIC_TOKEN_KW_GO,         // 22
#endif
#if USE_MATRIX
	BASIC_TOKEN_KW_IDN,        // 23
	BASIC_TOKEN_KW_INV,        // 26
#endif
#if USE_SAVE_LOAD
	BASIC_TOKEN_COM_LOAD,      // 29
#endif
#if USE_TEXTATTRIBUTES
	BASIC_TOKEN_COM_LOCATE,    // 30
#endif
#if USE_INTEGER_DIV
	BASIC_TOKEN_KW_MOD,        // 33
#endif
//	KW_OPTION,     // 38
#if USE_PEEK_POKE
	BASIC_TOKEN_KW_POKE,
#endif
#if USE_RANDOM
	BASIC_TOKEN_KW_RANDOMIZE,  // 41
#endif
#if USE_SAVE_LOAD
	BASIC_TOKEN_COM_SAVE,      // 47
#endif
#if CONF_USE_SPC_PRINT_COM
	BASIC_TOKEN_KW_SPC,        // 48
#endif
#if USE_TEXTATTRIBUTES
	BASIC_TOKEN_KW_TAB,        // 51
#endif
#if USE_MATRIX
	BASIC_TOKEN_KW_TRN,        // 54
#endif
	BASIC_TOKEN_KW_TRUE,       // 55
#if USE_DUMP
	BASIC_TOKEN_KW_VARS,       // 56
#endif
	BASIC_TOKEN_OP_XOR,        // 57
#if USE_MATRIX
	BASIC_TOKEN_KW_ZER,        // 58
#endif
	BASIC_TOKEN_KW_INPUT,
#if USE_DATA
	BASIC_TOKEN_KW_READ,
#endif
	BASIC_TOKEN_KW_RETURN,
	BASIC_TOKEN_KW_PRINT,
#if USE_DATA
	BASIC_TOKEN_KW_DATA,
#endif
	BASIC_TOKEN_KW_FOR,
	BASIC_TOKEN_KW_TO,
	BASIC_TOKEN_KW_IF,
	BASIC_TOKEN_OP_OR,
	BASIC_TOKEN_OP_AND,
	BASIC_TOKEN_KW_REM,
	BASIC_TOKEN_KW_END,
	BASIC_TOKEN_COM_LIST,
#if USE_MATRIX
	BASIC_TOKEN_KW_MAT,        // 32
#endif
	BASIC_TOKEN_KW_GOTO,
	BASIC_TOKEN_OP_NOT,
	BASIC_TOKEN_COM_NEW,       // 34
#if USE_DEFFN
	BASIC_TOKEN_KW_DEF,
#endif
#if CONF_USE_ON_GOTO
	BASIC_TOKEN_KW_ON,
#endif
	BASIC_TOKEN_COM_RUN,
	BASIC_TOKEN_KW_LET,
	BASIC_TOKEN_KW_DIM,
#if USE_DATA
	BASIC_TOKEN_KW_RESTORE,
#endif
#if USESTOPCONT
	BASIC_TOKEN_KW_STOP,
#endif
	BASIC_TOKEN_KW_THEN,
#if USE_DEFFN
	BASIC_TOKEN_KW_FN,
#endif
	BASIC_TOKEN_KW_NEXT,
	BASIC_TOKEN_KW_STEP,
	    
	// *
	BASIC_TOKEN_STAR,          // 59
	// /
	BASIC_TOKEN_SLASH,         // 50
#if USE_REALS && USE_INTEGER_DIV
	BASIC_TOKEN_BACK_SLASH,    // 61
#endif
	// +
	BASIC_TOKEN_PLUS,          // 62
	// -
	BASIC_TOKEN_MINUS,         // 63
	// =
	BASIC_TOKEN_EQUALS,        // 64
	// :
	BASIC_TOKEN_COLON,         // 65
	// ;
	BASIC_TOKEN_SEMI,          // 66
	// <
	BASIC_TOKEN_LT,            // 67
	// >
	BASIC_TOKEN_GT,            // 68
	// <=
	BASIC_TOKEN_LTE,           // 69
	// >=
	BASIC_TOKEN_GTE,           // 70
	// <>
	BASIC_TOKEN_NE,            // 71
#if CONF_USE_ALTERNATIVE_NE
	//  ><
	BASIC_TOKEN_NEA,           // 72
#endif
	// ,
	BASIC_TOKEN_COMMA,         // 73
	// ^
	BASIC_TOKEN_POW,           // 74
	// (
	BASIC_TOKEN_LPAREN,        // 75
	// )
	BASIC_TOKEN_RPAREN,        // 76

	BASIC_TOKEN_INTEGER_IDENT, // 77
	BASIC_TOKEN_REAL_IDENT,    // 78
#if USE_LONG_REALS
	BASIC_TOKEN_LONG_REAL_IDENT,// 85
#endif
#if USE_LONGINT        // 79
	BASIC_TOKEN_LONGINT_IDENT, // 80
#endif
	BASIC_TOKEN_STRING_IDENT,  // 81
	BASIC_TOKEN_BOOL_IDENT,    // 82

	BASIC_TOKEN_C_INTEGER,     // 83
	BASIC_TOKEN_C_LONG_INTEGER,// 83
	BASIC_TOKEN_C_REAL,        // 84
#if USE_LONG_REALS
	BASIC_TOKEN_C_LONG_REAL,   // 85
#endif
	BASIC_TOKEN_C_BOOLEAN,     // 85
	BASIC_TOKEN_C_STRING,      // 86
#if FAST_MODULE_CALL
	BASIC_TOKEN_COMMAND,       // 88
#endif
	BASIC_TOKEN_NUM_TOKENS     // 87
} basic_token_t;
