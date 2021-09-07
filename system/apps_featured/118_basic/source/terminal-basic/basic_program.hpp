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

#ifndef BASIC_INTERPRETER_PROGRAM_HPP
#define BASIC_INTERPRETER_PROGRAM_HPP

#include "arduinoext.hpp"
#include "basic.hpp"
#include "basic_parser_value.hpp"

namespace BASIC
{

class VariableFrame;
class ArrayFrame;
class Interpreter;
class Parser;

/**
 * @brief BASIC program in memory
 */
class Program
{
	friend class Interpreter;
public:
	
	/**
	 * @brief BASIC program string object
	 */
	struct PACKED Line
	{
		// string decimal number (label)
		uint16_t number;
		// size in bytes
		uint8_t size;
		// string body
		uint8_t text[];
	};
	
	/**
	 * @brief BASIC program position marker
	 */
	struct PACKED Position
	{
		// line index in program memory
		Pointer index;
		// position in line text
		uint8_t position;
	};

	/**
	 * @program stack frame object
	 */
	struct PACKED StackFrame
	{
		/**
		 * @brief Stack frame type
		 */
		enum Type : uint8_t
		{
			// Subprogram return address
			SUBPROGRAM_RETURN,
			// FOR-loop state frame
			FOR_NEXT,
			// String frame
			STRING,
			// One array dimension
			ARRAY_DIMENSION,
			// Number of array dimensions
			ARRAY_DIMENSIONS,
			// Value frame
			VALUE,
			// Input object frame
			INPUT_OBJECT
		};

		/**
		 * @brief FOR-loop state frame body
		 */
		struct PACKED ForBody
		{
			// Program counter on loop begin
			Pointer 	calleeIndex;
			// Loop begin position in the program string
			uint8_t		textPosition;
			// Loop variable name
			char		varName[VARSIZE];
			// Loop step value
			Parser::Value	stepValue;
			// Loop final value
			Parser::Value	finalvalue;
		};
		static_assert (sizeof (ForBody) <= UINT8_MAX, "bad size");
		
		/**
		 * @brief Subprogram return address frame body
		 */
		struct PACKED GosubReturn
		{
			// Program counter of the colee string
			Pointer calleeIndex;
			// Position in the program string
			uint8_t	textPosition;
		};
		
		/**
		 * @brief Input object frame body
		 */
		struct PACKED VariableBody
		{
			enum Type : uint8_t
			{
				INPUT_VAR, INPUT_ARR_ELM
			};
			Type	type;
			char	name[VARSIZE];
		};

		static uint8_t size(Type);
		
		uint8_t size() const { return size(this->_type); }

		Type _type;

		union PACKED Body
		{
			GosubReturn	gosubReturn;
			uint8_t		arrayDimensions;
			uint16_t	arrayDimension;
			ForBody		forFrame;
			VariableBody	inputObject;
			char		string[STRING_SIZE];
			Parser::Value	value;
		};
		Body body;
	};

	Program(Pointer = SINGLE_PROGSIZE);
	/**
	 * @brief Clear program text, but not vars and arrays
	 */
	void clearProg();
	/**
	 * @brief Move program state (variables and arrays) to dest
	 * @param dest New Data start
	 */
	void moveData(Pointer);
	/**
	 * @brief Clear program memory
	 */
	void newProg();
	/**
	 * @brief reset program 
	 * @brief newSize 0 if use existing program, text size if clear vars
	 *   and arrays
	 */
	void reset(Pointer = 0);
	/**
	 * @brief get actual size of stored program in bytes
	 * @return program size
	 */
	Pointer size() const;
	/**
	 * @brief get next program line
	 * @return line object or nullptr if beyond last line
	 */
	Line *getNextLine();
	/**
	 * @brief get next program line, based on provided text position
	 * @param pos Text position object
	 * @return line object or nullptr if beyond last line
	 */
	Line *getNextLine(Position&);
	/**
	 * @brief Get current program line to be executed
	 * @return pointer to current program line
	 */
	Line *current(const Position&) const;
	/**
	 * @brief Get first stored program line
	 * @return 
	 */
	Line *first() const;
	
	Line *last() const;
	
	void jump(Pointer);
	/**
	 * @brief program line at given address
	 * @param address
	 * @return string pointer or nullptr if not exists
	 */
	Line *lineByIndex(Pointer) const;
	/**
	 * @brief program line of given number
	 * @param number Program line number to get
	 * @param address start of the search
	 * @return Line pointer or NULL if not found
	 */
	Line *lineByNumber(
	    uint16_t,
	    Pointer = 0);
	/**
	 * @brief get variable frame at a given index
	 * @param index basic memory address
	 * @return pointer
	 */
	VariableFrame *variableByIndex(Pointer);
	VariableFrame *variableByName(const char*);
#if USE_DEFFN
	VariableFrame *functionByName(const char*);
#endif
	
	ArrayFrame *arrayByIndex(Pointer);
	ArrayFrame *arrayByName(const char*);
	
	Pointer objectIndex(const void*) const;

	StackFrame *stackFrameByIndex(Pointer);
	StackFrame *currentStackFrame();
	/**
	 * @brief create new stack frame of given type and get its pointer
	 * @param type
	 */
	StackFrame *push(StackFrame::Type);
	/**
	 * @brief pop top stack frame
	 */
	void pop();
	/**
	 * @brief reverse order of last same type elements
	 */
	void reverseLast(StackFrame::Type);
	/**
	 * @brief Add new Program line
	 * @param number decimal line number
	 * @param text
	 * @return flag of success
	 */
	bool addLine(
	    Parser&,
	    uint16_t,
	    const uint8_t*);
	/**
	 * @brief Remove program line
	 * @param num line number
	 */
	void removeLine(uint16_t);
	/**
	 * @brief Insert line at current position
	 * @param num line number
	 * @param text line text
	 * @param len line length
	 */
	bool insert(
	    uint16_t,
	    const uint8_t*,
	    uint8_t);
	
#if CONF_USE_ALIGN
	/**
	 * @brief Align variables
	 * @param index address of the first variable frame to align from
	 */
	bool alignVars(Pointer);
	
	bool alignArrays(Pointer);
	
	int8_t alignPointer(
	    Pointer,
	    Parser::Value::Type);
#endif
	
#if USE_EXTMEM
	char *_text;
#else
	char _text[SINGLE_PROGSIZE];
#endif
	const Pointer programSize;
private:
	
	void _reset();
	
	void pushBottom(StackFrame*);
	/**
	 * @brief Add tokenized program line
	 * @param num line number
	 * @param text line body w/o number
	 * @param len line body length
	 * @return flag of success
	 */
	bool addLine(uint16_t, const uint8_t*, uint8_t);
	
	// End of program text
	Pointer _textEnd;
	// End of variables area
	Pointer _variablesEnd;
	// End of arrays area
	Pointer _arraysEnd;
	// Stack pointer
	Pointer _sp;
	// Position of main execution thread
	Position _current;
#if USE_DATA
	// Position of data reader
	Position _dataCurrent;
#endif
	// Jump flag
	bool _jumpFlag;
	// Jump pointer
	Pointer _jump;
};

} // namespace BASIC

#endif // BASIC_INTERPRETER_PROGRAM_HPP
