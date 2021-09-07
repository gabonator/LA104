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

#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdlib.h>
#include <inttypes.h>

#include "arduinoext.hpp"
#include "basic.hpp"
#include "basic_parser.h"
#include "basic_internalfuncs.hpp"

namespace BASIC
{

class Lexer;
class Interpreter;

/**
 * @brief Syntactic analyzer class
 */
class Parser
{
public:

	/**
	 * @brief Syntax errors
	 */
	enum ErrorCodes : uint8_t
	{
		NO_ERROR = 0,                 // Not an error
		OPERATOR_EXPECTED = 1,        // Operator expected
		IDENTIFIER_EXPECTED = 2,
		EXPRESSION_EXPECTED = 3,
		INTEGER_CONSTANT_EXPECTED = 4,
		THEN_OR_GOTO_EXPECTED = 5,
		INVALID_DATA_EXPR = 6,
		INVALID_READ_EXPR = 7,
		VARIABLES_LIST_EXPECTED = 8,
		STRING_OVERFLOW = 9,
		MISSING_RPAREN = 10,
#if CONF_USE_ON_GOTO
		INVALID_ONGOTO_INDEX = 11
#endif
	};

	class PACKED Value;
	/**
	 * @brief constructor
	 * @param lexer Lexical analyzer object refertence
	 * @param interpreter Interpreter context object reference
	 */
	Parser(Lexer&, Interpreter&);
	/**
	 * @brief Parse a text string
	 * @param str string to parse
	 * @param ok successfull parsing flag
	 * @param tok flag of the tokenized program text
	 * @return end of parsed string
	 */
	bool parse(const uint8_t*, bool&, bool);

	void stop();

	/**
	 * @brief get last static error code
	 * @return error code
	 */
	ErrorCodes getError() const
	{
		return _error;
	}

	void init();
	
	FunctionBlock::command getCommand(const char*);
	
	void getCommandName(FunctionBlock::command, uint8_t*);

	void addModule(FunctionBlock*);
#if CONF_ERROR_STRINGS
	static PGM_P const errorStrings[] PROGMEM;
#endif
private:

	/**
	 * Parser mode: syntax check or execute commands of the interpreter
	 * context
	 */
	enum Mode : uint8_t
	{
		SCAN = BASIC_PARSER_SCAN,
		EXECUTE = BASIC_PARSER_EXECUTE
	};
	
	void setMode(Mode);
	Mode getMode() const;
	
	void setStopParse(bool);
	bool getSTopParse() const;
	
	bool testExpression(Value&);
	
	bool fOperators(bool&);
	bool fOperator();
#if CONF_USE_ON_GOTO
	bool fOnStatement(uint8_t);
#endif
#if USE_DATA
	bool fDataStatement();
	bool fReadStatement();
#endif // USE_DATA
#if USE_DEFFN
	bool fDefStatement();
	bool fFnexec(Value&);
#endif
	bool fImplicitAssignment(char*);
#if USE_PEEK_POKE
	bool fPoke();
#endif
	bool fPrintList();
	bool fPrintItem();
	bool fExpression(Value&);
	bool fLogicalAdd(Value&);
	bool fLogicalFinal(Value&);
	bool fSimpleExpression(Value&);
	bool fTerm(Value&);
	bool fFactor(Value&);
	bool fFinal(Value&);
	bool fIfStatement();
	bool fCommand();
	void fCommandArguments(FunctionBlock::command);
	bool fGotoStatement();
	bool fForConds();
	bool fIdentifier(char*);
	bool fVarList();
	bool fArrayList();
	bool fArray(uint8_t&);
	bool fDimensions(uint8_t&);
	bool fIdentifierExpr(char*, Value&);
#if USE_MATRIX
	bool fMatrixOperation();
	bool fMatrixPrint();
	bool fMatrixExpression(const char*);
#endif
	// last static semantic error
	ErrorCodes _error;
	// lexical analyser object reference
	Lexer &_lexer;
	// interpreter context object reference
	Interpreter &_interpreter;

	// first module in chain
	InternalFunctions _internal;
	
	basic_parser_context_t m_context;
};

} // namespace BASIC

#endif // PARSER_HPP
