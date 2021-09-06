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

/**
 * @file basic_config.h
 * @brief Configuration parameters, common among versions
 */

#ifndef BASIC_CONFIG_H
#define BASIC_CONFIG_H

/*
 * 1. Real arithmetics
 * 
 * Support of operations with real numbers.
 * When enabled, all variables and arrays, which names are not ending with "$ ! %"
 * are treated as reals. Mathematical functions support depend on this option
 * This type is 4-byte binary floating point (C/C++ float)
 */
#define USE_REALS            1

#if USE_REALS
    /**
     * 1.1 Long real numbers
     * 
     * Support of long real numbers.  Arrays, variables and functions, ending
     * with "!" character are long real.
     * This type is 8-byte binary floating point (C/C++ double). This means
     * it can't be used on systems w/o double type support, such as 8-bit AVR
     * arduino boards (UNO/NANO/PRO mini/Mega...)
     * 
     * All real number constants (1e6, 0.5, 3., .04 ... ) treated as long reals
     */
#define USE_LONG_REALS       0
#endif

/*
 * Support of 4-byte integer datatype
 * 
 * Default integer constants are 2-byte signed [-32768 .. 32768), but this 
 * option enables 4-byte signed type.
 * Functions, variables and arrays of long integer type ends with double "%!"
 * symbols
 */
#define USE_LONGINT          0

/**
 * DUMP command support
 * This command can be used to check BASIC memory image, variables and arrays list
 */
#define USE_DUMP             0

/*
 * RANDOMIZE command and RND() function support
 */
#define USE_RANDOM           1

/*
 * Support of Darthmouth BASIC-style matrix operations
 */
#define USE_MATRIX           0

/**
 * Support of DATA/READ statements
 */
#define USE_DATA             1

/*
 * Support of DEF FN construct
 */
#define USE_DEFFN            0

/*
 * DELAY command, suspends execution for N ms
 */
#define USE_DELAY           1

/*
 * Allow ON ... GOTO ... statements
 */
#define CONF_USE_ON_GOTO    1

/*
 * Allow GO TO OPERATOR in addition to GOTO
 */
#define CONF_SEPARATE_GO_TO 1

/*
 * Fast command call using C-function address
 */
#define FAST_MODULE_CALL    0

/*
 * Support of integer division and modulo operation
 */
#define USE_INTEGER_DIV     1
#if USE_INTEGER_DIV
	/*
	 * Use DIV keyword for integer division in addition to \ operation
	 */
	#define USE_DIV_KW  0
#endif // USE_INTEGER_DIV

/*
 * Use >< as not-equals operator (with default <>)
 */
#define CONF_USE_ALTERNATIVE_NE 0

/*
 * SAVE, LOAD and CHAIN commands support
 */
#define USE_SAVE_LOAD        1

/*
 * Use ANSI text attributes
 */
#define USE_TEXTATTRIBUTES   1
#if USE_TEXTATTRIBUTES
	/*
	 * Support of SPC(N) print command
	 */
	#define CONF_USE_SPC_PRINT_COM  1
#endif // USE_TEXTATTRIBUTES

/*
 * STOP and CONTINUE commands support
 */
#define USESTOPCONT       1

/**
 * PEEK FUNCTION, POKE command support
 */
#define USE_PEEK_POKE 0

/**
 * Language constants for later usage
 */
#define LANG_EN 0 /* English */
#define LANG_RU 1 /* Russian */
#define LANG_FR 3 /* French */

/**
 * Messages and errors localization
 */
#define CONF_LANG LANG_EN

/**
 * Lexer localization
 */
#define CONF_LEXER_LANG LANG_EN

/* Size of the string identifiers */
#define STRING_SIZE 72

/**
 * Enabling packed structures reduces BASIC memory usage, but results to
 * unaligned memory acces. It should be desabled on ESP8266, Motorola 68k, etc.
 */
#define USE_PACKED_STRUCT 0

/*
 * High-level code optimisation mode
 */
#define OPT_SPEED     1 /* Extensive use of switch/case constructs */
#define OPT_SIZE      2 /* Use cascade of if/else if instead of switch/case */
#define OPT           OPT_SIZE /* Selected mode */

#endif /* BASIC_CONFIG_H */
