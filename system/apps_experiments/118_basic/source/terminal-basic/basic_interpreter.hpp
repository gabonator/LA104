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

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "basic.hpp"
#include "basic_lexer.hpp"
#include "basic_parser.hpp"
#include "basic_program.hpp"
#include "helper.hpp"
#include "vt100.hpp"

namespace BASIC
{

#if CONF_USE_EXTMEMFS
class ExtmemFSModule;
#endif

/**
 * @brief variable memory frame
 */
struct PACKED VariableFrame
{
	/**
	 * @brief size of the initialized frame
	 * @return size in bytes
	 */
	uint8_t size() const;
	
	static uint8_t size(Parser::Value::Type);

	/**
	 * @brief getValue from Variable frame
	 * @param T value type
	 * @return value
	 */
	template <typename T>
	T get() const
	{
		union
		{
			const char *b;
			const T *i;
		} U;
		U.b = bytes;
		return *U.i;
	}

	// Variable name
	char name[VARSIZE];
	// Variable type
	Parser::Value::Type type;
	// Frame body
	char bytes[];
};
#if USE_DEFFN
#define TYPE_DEFFN 0x80
#endif

/**
 * Array memory frame
 */
struct PACKED ArrayFrame
{
	/**
	 * @brief get frame size in bytes
	 * @return size
	 */
	uint16_t size() const;

	uint16_t dataSize() const;

	uint16_t numElements() const;

	/**
	 * @brief get array raw data pointer
	 * @return pointer
	 */
	uint8_t *data()
	{
		return reinterpret_cast<uint8_t*> (this+1) +
		    sizeof(uint16_t) * numDimensions;
	}

	/**
	 * @brief Overloaded version
	 */
	const uint8_t *data() const
	{
		return reinterpret_cast<const uint8_t*> (this+1) +
		    sizeof(uint16_t) * numDimensions;
	}

	/**
	 * @brief get array value by raw index
	 * @param index shift in array data
	 * @return value
	 */
	template <typename T>
	T get(uint16_t index) const
	{
		const union
		{
			const uint8_t *b;
			const T *i;
		} U = { .b = this->data() };
		return U.i[index];
	}

	bool get(uint16_t, Parser::Value&) const;
	bool set(uint16_t, const Parser::Value&);

	template <typename T>
	void set(uint16_t index, T val)
	{
		union
		{
			uint8_t *b;
			T *i;
		} U = { .b = this->data() };
		U.i[index] = val;
	}

	// Array data
	char name[VARSIZE];
	// Array type
	Parser::Value::Type type;
	// Number of dimensions
	uint8_t numDimensions;
	// Actual dimensions values
	uint16_t dimension[];
};

#if USE_DEFFN
/**
 * Array memory frame
 */
struct PACKED FunctionFrame
{
	uint16_t lineNumber;
	uint8_t linePosition;
};
#endif // USE_DEFFN

/**
 * @brief Interpreter context object
 */
class Interpreter
{
public:

	/**
	 * Dynamic (runtime error codes)
	 */
	enum ErrorCodes : uint8_t
	{
		NO_ERROR = 0,			// Ok
		OUTTA_MEMORY = 1,		// Out of memory
		REDIMED_ARRAY = 2,		// Attempt to define existing array
		STACK_FRAME_ALLOCATION = 3,	// Unable to allocate stack frame
		STRING_FRAME_SEARCH = 4,	// Missing string frame
		INVALID_NEXT = 5,		// 
		RETURN_WO_GOSUB = 6,
		NO_SUCH_LINE = 7,
		INVALID_VALUE_TYPE = 8,
		NO_SUCH_ARRAY = 9,
		INTEGER_EXPRESSION_EXPECTED = 10, // Integer expression expected
#if SAVE_LOAD_CHECKSUM
		BAD_CHECKSUM = 11,		// Bad program checksum
#endif
		INVALID_TAB_VALUE = 12,
		INVALID_ELEMENT_INDEX = 13,
#if USE_MATRIX
		SQUARE_MATRIX_EXPECTED = 14,
#endif
		DIMENSIONS_MISMATCH = 15,
		COMMAND_FAILED = 16,
#if USE_DEFFN
		VAR_DUPLICATE = 17,
		FUNCTION_DUPLICATE = 18,
		NO_SUCH_FUNCION = 19,
#endif
		INTERNAL_ERROR = 255
	};

	/**
	 * Type of the occured error
	 */
	enum ErrorType : uint8_t
	{
		STATIC_ERROR, // syntax
		DYNAMIC_ERROR // runtime
	};

	/**
	 * Interpreter FSM state
	 */
	enum State : uint8_t
	{
		SHELL = 0,		// Wait for user input of line or command
		PROGRAM_INPUT,	// Inputting of the program lines
#if BASIC_MULTITERMINAL
		COLLECT_INPUT,	//
		GET_VAR_VALUE,
#endif // BASIC_MULTITERMINAL
		EXECUTE,	// Runniong the program
		VAR_INPUT,	// Input of the variable value
#if USE_DELAY
		DELAY,          // Wait for timeout
#endif
		CONFIRM_INPUT	// Input of the confirmation
	};
#if USE_DUMP
	// Memory dump modes
	enum DumpMode : uint8_t
	{
		MEMORY, VARS, ARRAYS
	};
#endif //USE_DUMP
	
	/**
	 * @brief constructor
	 * @param stream Boundary output object
	 * @param print Boundary input object
	 * @param program Program size
	 */
	explicit Interpreter(Stream&, Print&, Pointer);
	/**
	 * [re]initialize interpreter object
	 */
	void init();
	// Interpreter cycle: request a string or execute one operator
	void step();
	// Execute entered command (command or inputed program line)
	void exec();
#if USE_DATA
	// Restore data pointer
	void restore();
#endif
#if USE_TEXTATTRIBUTES
	// Clear screen
	void cls();
#endif
#if USESTOPCONT
	void cont();
#endif
	// Output program memory
	void list(uint16_t = 1, uint16_t = 0);
#if USE_DUMP
	// Dump program memory
	void dump(DumpMode);
#endif
	// Add module on tail of the modules list
	void addModule(FunctionBlock*);

#if USE_INKEY
	uint8_t lastKey();
#endif
	
#if USE_DELAY
	void delay(uint16_t);
#endif
	
#if USE_TEXTATTRIBUTES
	void locate(Integer, Integer);
#endif
	
	// New print line
	void newline();
	
	void print(char);
	// Execute command by function pointer
	void execCommand(FunctionBlock::command);
	
#if USE_PEEK_POKE
	void poke(Pointer, Integer);
#endif

#if USE_MATRIX
	/**
	 * @bief Matrix expression oprations
	 */
	enum MatrixOperation_t : uint8_t
	{
		MO_NOP,
		MO_SCALE,
		MO_SUM,
		MO_SUB,
		MO_MUL,
		MO_TRANSPOSE,
		MO_INVERT
	};
	
	void zeroMatrix(const char*);
	void onesMatrix(const char*);
	void identMatrix(const char*);
	void printMatrix(const char*);
	void matrixDet(const char*);
	/**
	 * @brief Assign matrix a result of matrix operation
	 * @param name Name of the matrix to assign to
	 * @param first First and possible the only matrix expression operand
	 * @param second Second optional matrix exprerssion operand
	 * @param op Operation type
	 */
	void assignMatrix(const char*, const char*, const char* = nullptr,
	    MatrixOperation_t = MO_NOP);
#if USE_DATA
	void matrixRead(const char*);
#endif
#endif // USE_MATRIX

#if USE_DATA
	bool read(Parser::Value&);
#endif
	
	void print(Integer, VT100::TextAttr = VT100::NO_ATTR);
#if USE_TEXTATTRIBUTES
	/**
	 * @brief control print space or tab
	 * @param v Value of the spaces
	 * @param flag true - TAB, false - SPC
	 */
	void printTab(const Parser::Value&, bool);
#endif
	void print(long, VT100::TextAttr = VT100::NO_ATTR);
	void print(ProgMemStrings, VT100::TextAttr = VT100::NO_ATTR);
	void writePgm(ProgMemStrings);
	void writePgm(PGM_P);
	void print(Token);
	void print(const char *, VT100::TextAttr = VT100::NO_ATTR);
	// print value
	void print(const Parser::Value&, VT100::TextAttr = VT100::NO_ATTR);

	void printEsc(const char*);
	void printEsc(ProgMemStrings);
        
	// run program
	void run();
	// goto new line
	void gotoLine(const Parser::Value&);
	// CLear program memory
	void newProgram();
	/**
	 * save current line on stack
	 */
	void pushReturnAddress();
	// return from subprogram
	void returnFromSub();
	// save for loop
	Program::StackFrame *pushForLoop(const char*, uint8_t, const Parser::Value&,
	    const Parser::Value&);
	bool pushValue(const Parser::Value&);
	
	void pushInputObject(const char*);
	
	bool popValue(Parser::Value&);
	
	bool popString(const char*&);
	
	void randomize();
#if USE_DEFFN
	void execFn(const char*);
	void setFnVars();
	void returnFromFn();
#endif
	/**
	 * @brief iterate over loop
	 * @param varName loop variable name
	 * @return loop end flag
	 */
	bool next(const char*);
	bool testFor(Program::StackFrame&);

#if USE_SAVE_LOAD
	// Internal EEPROM commands
	struct EEpromHeader_t
	{
		Pointer len;
		Pointer magic_FFFFminuslen;
		uint16_t crc16;
	};
	void save();
	void load();
	void chain();
#endif // USE_SAVE_LOAD
	/**
	 * @breif Input variables
	 */
	void input();

	void end();
	/**
	 * @brief set value to initialized object
	 * @param f frame to set to
	 * @param v value to set
	 */
	void set(VariableFrame&, const Parser::Value&);
	/**
	 * @brief set a new value and possibly create new variable
	 * @param name variable name
	 * @param v value to assign
	 */
	VariableFrame *setVariable(const char*, const Parser::Value&);
	/**
	 * @brief setarray element a given value with indexes on the stack
	 * @param name array name
	 * @param value value to set
	 */
	void setArrayElement(const char*, const Parser::Value&);
	/**
	 * @brief create array
	 * @param name array name
	 */
	void newArray(const char*);
	/**
	 * @brief get variable frame pointer (or create new one)
	 * @param name variable name
	 * @return frame pointer
	 */
	const VariableFrame *getVariable(const char*);
	/**
	 * @brief Fill value object with the value of a variable
	 * @param val value object
	 * @param var name of the variable
	 */
	void valueFromVar(Parser::Value&, const char*);
	/**
	 * @brief Fill value object with the value of an array element
	 * @param val value object
	 * @param var name of the array
	 */
	bool valueFromArray(Parser::Value&, const char*);
	/**
	 * @brief push string constant on the stack
	 */
	void pushString(const char*);
	/**
	 * @brief push the next array dimesion on the stack
	 * @param dim dimension value
	 */
	void pushDimension(uint16_t);
	/**
	 * @brief push the number of array dimesions on the stack
	 * @param num number of dimensions
	 */
	void pushDimensions(uint8_t);
        
	bool pushResult();

#if USE_STRINGOPS
	void strConcat();
	bool strCmp();
#endif
	/**
	 * @brief request user confirmation
	 * @return 
	 */
	bool confirm();

	void stop()
	{
		_parser.stop();
	}
	
#if USE_DEFFN
	/**
	 * @brief Create new function frame
	 * @param fname
	 */
	void newFunction(const char*, uint8_t pos);
#endif // USE_DEFFN

	Program _program;
	
	Parser& parser() { return _parser; }
	
#if CONF_USE_EXTMEMFS
	void setSDFSModule(BASIC::ExtmemFSModule* newVal) { m_sdfs = newVal; }
#endif
	
private:
	
	class AttrKeeper;
#if USE_MATRIX
	/**
	 * @Fill matrix elements with the value
	 * @param array name for the matrix
	 * @param value object to fill the matrix with
	 */
	void fillMatrix(const char*, const Parser::Value&);
	/**
	 * 
	 * @param frame
	 * @param rows
	 * @param columns
	 */
	void setMatrixSize(ArrayFrame&, uint16_t, uint16_t);
	/**
	 * @brief Get 2 dimensional array from stack
	 * 
	 * Returns nullptr if there is no such array or if that array is not
	 * 2 dimensional. Raises corresponding error.
	 * 
	 * @param array name
	 * @return frame pointer
	 */
	ArrayFrame *get2DArray(const char*);
	/**
	 * @brief Get 2 dimensional square array from stack
	 * 
	 * Returns nullptr if there is no such array or if that array is not
	 * 2 dimensional or if it's not square. Raises corresponding error.
	 * 
	 * @param array name
	 * @return frame pointer
	 */
	ArrayFrame *getSquareArray(const char*);
#endif // USE_MATRIX
	
	// Get next input object from stack
	bool nextInput();
	// Place input values to objects
	void doInput();

	void print(Lexer&);

	void raiseError(ErrorType, ErrorCodes = NO_ERROR, bool = true);
	/**
	 * @brief read and buffer one symbol
	 * @return input finished flag
	 */
	bool readInput();
	/**
	 * @brief Add new array frame
	 * @param name name of the array (also defines type of the elements)
	 * @param dim number of dimensions
	 * @param num overall elements number
	 * @return 
	 */
	ArrayFrame *addArray(const char*, uint8_t, uint16_t);

	bool arrayElementIndex(ArrayFrame*, uint16_t&);
#if USE_SAVE_LOAD
	/**
	 * @brief Check program text
	 * @param len Length of the program
	 * @return Flag of success
	 */
	bool checkText(Pointer&);
	/**
	 * @brief load program memory from eeprom
	 * @param len Length of program
	 * @param showPogress show loading progress
	 */
	void loadText(Pointer, bool=true);
#if SAVE_LOAD_CHECKSUM
	uint16_t eepromProgramChecksum(uint16_t);
#endif
#endif // USE_SAVE_LOAD
	// Interpreter FSM state
	State			 _state;
	State			 _lastState;
	// Input oject
	Stream			&_input;
	// Output object
	Print			&_output;
	// Lexical analyzer object
	Lexer			 _lexer;
	// Syntactical analyzer object
	Parser			 _parser;
	//Input buffer
	uint8_t			 _inputBuffer[PROGSTRINGSIZE];
	// Position in the input buffer
	uint8_t			 _inputPosition;
	// Input variable name string;
	char			 _inputVarName[VARSIZE];
	// Global RESULT() variable
	Parser::Value		 _result;
#if LOOP_INDENT
	// Loop indention spaces
	uint8_t			 _loopIndent;
#endif
#if BASIC_MULTITERMINAL
	static uint8_t		 _termnoGen;
	uint8_t			 _termno;
#endif
#if USE_DELAY
	// Milliseconds left to timeout end
	uint32_t		_delayTimeout;
#endif
#if USE_DATA
	// Data statement parser continue flag
	bool			_dataParserContinue;
#endif
#if CONF_USE_EXTMEMFS
	BASIC::ExtmemFSModule*	m_sdfs;
#endif
#if CONF_ERROR_STRINGS
	static PGM_P const errorStrings[] PROGMEM;
#endif
};

} // namespace BASIC

#endif // INTERPRETER_HPP
