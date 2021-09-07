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

#include "basic_program.hpp"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "basic_interpreter.hpp"
#include "basic_parser.hpp"

namespace BASIC
{

Program::Program(Pointer progsize) :
#if USE_EXTMEM
_text(reinterpret_cast<char*> (EXTMEM_ADDRESS)),
#endif
programSize(progsize)
{
	assert(_text != nullptr);
	assert(progsize <= SINGLE_PROGSIZE);
}

Program::Line*
Program::getNextLine()
{
	if (_jumpFlag) {
		_current.index = _jump;
		_jumpFlag = false;
		return current(_current);
	}
	
	return getNextLine(_current);
}

Program::Line*
Program::getNextLine(Position &pos)
{
	Program::Line *result = current(pos);
	if (result != nullptr) {
		pos.index += result->size;
		pos.position = 0;
	}
	return result;
}

Program::Line*
Program::current(const Position &pos) const
{
	if (pos.index < _textEnd)
		return lineByIndex(pos.index);
	else
		return nullptr;
}

Program::Line*
Program::first() const
{
	return lineByIndex(0);
}

Program::Line*
Program::last() const
{
	return lineByIndex(_textEnd);
}

void
Program::jump(Pointer newVal)
{
	_jump = newVal;
	_jumpFlag = true;
}

Program::Line*
Program::lineByIndex(Pointer address) const
{
	return const_cast<Line*> (reinterpret_cast<const Line*> (
	    _text + address));
}

Program::Line*
Program::lineByNumber(
    uint16_t number,
    Pointer address)
{
	Program::Line *result = nullptr;

	if (address <= _textEnd) {
		_current.index = address;
		for (Line *cur = getNextLine(); cur != nullptr;
		    cur = getNextLine()) {
			if (READ_VALUE(cur->number) == number) {
				result = cur;
				break;
			}
		}
	}
	return result;
}

uint8_t
Program::StackFrame::size(Type t)
{
	static const constexpr uint8_t minSize = sizeof(StackFrame) - sizeof(Body);
#if OPT == OPT_SPEED
	switch (t) {
	case SUBPROGRAM_RETURN:
		return minSize + sizeof (GosubReturn);
	case FOR_NEXT:
		return minSize + sizeof (ForBody);
	case STRING:
		return minSize + STRING_SIZE;
	case ARRAY_DIMENSION:
		return minSize + sizeof (uint16_t);
	case ARRAY_DIMENSIONS:
		return minSize + sizeof (uint8_t);
	case VALUE:
		return minSize + sizeof (Parser::Value);
	case INPUT_OBJECT:
		return minSize + sizeof (VariableBody);
	default:
		return 0;
	}
#else
	if (t == SUBPROGRAM_RETURN)
		return (minSize + sizeof (GosubReturn));
	else if (t == FOR_NEXT)
		return (minSize + sizeof (ForBody));
	else if (t == STRING)
		return (minSize + STRING_SIZE);
	else if (t == ARRAY_DIMENSION)
		return (minSize + sizeof (uint16_t));
	else if (t == ARRAY_DIMENSIONS)
		return (minSize + sizeof (uint8_t));
	else if (t == VALUE)
		return (minSize + sizeof (Parser::Value));
	else if (t == INPUT_OBJECT)
		return (minSize + sizeof (VariableBody));
	else
		return 0;
#endif
}

void
Program::clearProg()
{
	_jumpFlag = false;
	_reset();
}

void
Program::moveData(Pointer dest)
{
	const int32_t diff = _textEnd-dest;
	memmove(_text+dest, _text+_textEnd, _arraysEnd-_textEnd);
	_variablesEnd -= diff;
	_arraysEnd -= diff;
	_textEnd = dest;
}

void
Program::newProg()
{
	clearProg();
	_textEnd = _variablesEnd = _arraysEnd = _jump = 0;
#if CLEAR_PROGRAM_MEMORY
	memset(_text, 0xFF, programSize);
#endif
}

VariableFrame*
Program::variableByName(const char *name)
{
	auto index = _textEnd;

	VariableFrame* f;
	while ((f = variableByIndex(index)) != nullptr) {
#if CONF_USE_ALIGN
		if (_text[index] == 0) {
			++index;
			continue;
		}
#endif
#if USE_DEFFN
		if (!(f->type & TYPE_DEFFN)) {
#endif
			const int8_t res = strncmp(name, f->name, VARSIZE);
			if (res == 0)
				return f;
			else if (res < 0)
				break;
#if USE_DEFFN
		}
#endif
		index += f->size();
	}
	return nullptr;
}

#if USE_DEFFN
VariableFrame*
Program::functionByName(const char *name)
{
	auto index = _textEnd;

	VariableFrame* f;
	while ((f = variableByIndex(index)) != nullptr) {
		if (f->type & TYPE_DEFFN) {
			const int8_t res = strncmp(name, f->name, VARSIZE);
			if (res == 0)
				return f;
			else if (res < 0)
				break;
		}
		index += f->size();
	}
	return nullptr;
}
#endif // DEF_FN

Pointer
Program::objectIndex(const void *obj) const
{
	return reinterpret_cast<const char*>(obj) - _text;
}

Program::StackFrame*
Program::push(StackFrame::Type t)
{
	const uint8_t s = StackFrame::size(t);
	if ((_sp - s) < _arraysEnd)
		return nullptr;

	_sp -= s;
	StackFrame *f = stackFrameByIndex(_sp);
	if (f != nullptr)
		f->_type = t;
	return f;
}

void
Program::pop()
{
	const StackFrame *f = stackFrameByIndex(_sp);
	if (f != nullptr)
		_sp += StackFrame::size(f->_type);
}

void
Program::reverseLast(StackFrame::Type type)
{
	StackFrame *f = this->currentStackFrame();
	if (f != nullptr && f->_type == type) {
		char buf[sizeof (StackFrame)];
		StackFrame *fr = reinterpret_cast<StackFrame*> (buf);
		memcpy(fr, f, StackFrame::size(f->_type));
		this->pop();
		reverseLast(type);
		pushBottom(fr);
	}
}

void
Program::pushBottom(StackFrame *f)
{
	auto newFrame = this->currentStackFrame();
	if ((newFrame == nullptr) || (newFrame->_type != f->_type)) {
		newFrame = this->push(f->_type);
		memcpy(newFrame, f, StackFrame::size(f->_type));
	} else {
		char buf[sizeof (StackFrame)];
		auto fr = reinterpret_cast<StackFrame*> (buf);
		memcpy(fr, newFrame, StackFrame::size(f->_type));
		this->pop();
		pushBottom(f);
		f = this->push(newFrame->_type);
		memcpy(f, fr, StackFrame::size(newFrame->_type));
	}
}

Program::StackFrame*
Program::stackFrameByIndex(Pointer address)
{
	if ((address > 0) && (address < programSize))
		return reinterpret_cast<StackFrame*> (_text + address);
	else
		return nullptr;
}

Program::StackFrame*
Program::currentStackFrame()
{
	if (_sp < programSize)
		return stackFrameByIndex(_sp);
	else
		return nullptr;
}

ArrayFrame*
Program::arrayByName(const char *name)
{
	auto index = _variablesEnd;

	ArrayFrame* f;
	while ((f = arrayByIndex(index)) != nullptr) {
#if CONF_USE_ALIGN
		if (_text[index] == 0) {
			++index;
			continue;
		}
#endif
		const int8_t res = strcmp(name, f->name);
		if (res == 0) {
			return f;
		} else if (res < 0)
			break;
		index += f->size();
	}
	return nullptr;
}

VariableFrame*
Program::variableByIndex(Pointer index)
{
	if (index >= _textEnd && index < _variablesEnd)
		return reinterpret_cast<VariableFrame*> (_text + index);
	else
		return nullptr;
}

ArrayFrame*
Program::arrayByIndex(Pointer index)
{
	if (index >= _variablesEnd && index < _arraysEnd)
		return reinterpret_cast<ArrayFrame*> (_text + index);
	else
		return nullptr;
}

bool
Program::addLine(
    Parser& parser,
    uint16_t num,
    const uint8_t *line)
{
	uint8_t size;
	uint8_t tempBuffer[2*PROGSTRINGSIZE];

	Lexer lexer;
	// 1-st pass tokenization: lexer do what it can by itself
	size = lexer.tokenize(tempBuffer, 2*PROGSTRINGSIZE, line);
        
	// 2-nd pass tokenization: command calls translated into command
	//  implementation asddress
#if FAST_MODULE_CALL
	lexer.init(tempBuffer, true);
	while (lexer.getNext()) {
		const auto token = lexer.getToken();
		if (token == Token::KW_REM)
			break;
		else if (token >= Token::INTEGER_IDENT &&
		    token <= Token::BOOL_IDENT) {
			auto c = parser.getCommand(lexer.id());
			if (c != nullptr) {
				const int8_t tokLen = strlen(lexer.id());
				const int8_t dist = tokLen-2-sizeof(uintptr_t);
				const uint8_t pos = lexer.getPointer() - tokLen;
				
				size -= dist;
				tempBuffer[pos] = ASCII_DLE;
				tempBuffer[pos+1] = BASIC_TOKEN_COMMAND;
				memmove(tempBuffer+pos+2+sizeof(uintptr_t),
				    tempBuffer+lexer.getPointer(), size-2);
				writeValue(uintptr_t(c), &tempBuffer[pos+2]);
				lexer.setPointer(lexer.getPointer()-dist);
			}
		}
	}
#endif // FAST_MODULE_CALL

	return addLine(num, tempBuffer, size);
}

bool
Program::addLine(
    uint16_t num,
    const uint8_t *text,
    uint8_t len)
{
	reset();

	if (_textEnd == 0) // First string insertion
		return insert(num, text, len);

	const uint8_t strLen = sizeof(Line) + len;
	// Iterate over lines
	Line *cur;
	for (cur = current(_current); _current.index < _textEnd;
	    cur = current(_current)) {
		const auto curnumber = READ_VALUE(cur->number);
		if (num < curnumber) {
			// Current line has number greater then new one,
			// point of insertion
			break;
		} else if (num == curnumber) {
			// Current line has number equals to new one,
			// replace string
			const uint8_t newSize = strLen;
			const uint8_t curSize = cur->size;
			const int8_t dist = newSize - curSize;
			const Pointer bytes2copy = _arraysEnd -
			    (_current.index + curSize);
			if ((_arraysEnd + dist) >= _sp)
				return false;
			memmove(_text + _current.index + newSize,
			    _text + _current.index + curSize, bytes2copy);
			WRITE_VALUE(cur->number, num);
			cur->size = strLen;
			memcpy(cur->text, text, len);
			_textEnd += dist, _variablesEnd += dist,
			    _arraysEnd += dist;
#if CONF_USE_ALIGN
			return alignVars(_textEnd);
#else
			return true;
#endif
		}
		_current.index += cur->size;
	}
	return insert(num, text, len);
}

void
Program::removeLine(uint16_t num)
{
	const Line *line = this->lineByNumber(num, 0);
	if (line != nullptr) {
		const Pointer index = objectIndex(line);
		assert(index < _textEnd);
		const Pointer next = index+line->size;
		const Pointer len = _arraysEnd-next;
		_textEnd -= line->size;
		_variablesEnd -= line->size;
		_arraysEnd -= line->size;
		memmove(_text+index, _text+next, len);
#if CONF_USE_ALIGN
		alignVars(_textEnd);
#endif
	}
}

bool
Program::insert(
    uint16_t num,
    const uint8_t *text,
    uint8_t len)
{
	const uint8_t strLen = sizeof(Line) + len;

	if (_arraysEnd + strLen >= _sp)
		return false;

	memmove(_text + _current.index + strLen, _text + _current.index,
	    _arraysEnd - _current.index);

	Line *cur = lineByIndex(_current.index);
	WRITE_VALUE(cur->number, num);
	cur->size = strLen;
	memcpy(cur->text, text, len);
	_textEnd += strLen, _variablesEnd += strLen, _arraysEnd += strLen;
#if CONF_USE_ALIGN
	return alignVars(_textEnd);
#else
	return true;
#endif
}

void
Program::reset(Pointer size)
{
	_reset();
	if (size > 0)
		_textEnd = _variablesEnd = _arraysEnd = size;
}

Pointer
Program::size() const
{
	return _textEnd;
}

void
Program::_reset()
{
	_current.index = _current.position = 0;
#if USE_DATA
	_dataCurrent.index = _dataCurrent.position = 0;
#endif
	_sp = programSize;
}

#if CONF_USE_ALIGN
bool
Program::alignVars(Pointer index)
{
	Pointer lastIndex = index;
	VariableFrame *f;
	while ((f = variableByIndex(index)) != nullptr) {
		if (_text[index] == 0) {
			++index;
			continue;
		}
#if USE_DEFFN
		if (f->type & TYPE_DEFFN) {
			index += f->size();
			continue;
		}
#endif
		const Parser::Value::Type t = f->type;
		Pointer i = lastIndex + sizeof(VariableFrame);
		int8_t a = alignPointer(i, t);
		int8_t dist = a - int8_t(index - lastIndex);
		
		if (_arraysEnd+dist >= _sp) {
			return false;
		}
		const auto src = _text + index;
		const auto dst = src + dist;
		if (_arraysEnd > index)
			memmove(dst, src, _arraysEnd - index);
		for (i=lastIndex; i<lastIndex+a; ++i) {
			_text[i] = 0;
		}
		_variablesEnd += dist;
		_arraysEnd += dist;
		
		alignVars(_variablesEnd);
		
		lastIndex += a + VariableFrame::size(t);
		index = lastIndex;
	}
	
	return true;
}

bool
Program::alignArrays(Pointer index)
{
	Pointer lastIndex = index;
	ArrayFrame *f;
	while ((f = arrayByIndex(index)) != nullptr) {
		if (_text[index] == 0) {
			++index;
			continue;
		}
		
		const Parser::Value::Type t = f->type;
		const uint16_t fsize = f->size();
		Pointer i = lastIndex + fsize - f->dataSize();
		int8_t a = alignPointer(i, t);
		int8_t dist = a - int8_t(index - lastIndex);
		
		if (_arraysEnd+dist >= _sp) {
			return false;
		}
		const auto src = _text + index;
		const auto dst = src + dist;
		if (_arraysEnd > index)
			memmove(dst, src, _arraysEnd - index);
		for (i=lastIndex; i<lastIndex+a; ++i) {
			_text[i] = 0;
		}
		_arraysEnd += dist;
		
		lastIndex += a + fsize;
		index = lastIndex;
	}
	
	return true;
}

int8_t
Program::alignPointer(
    Pointer i,
    Parser::Value::Type t)
{
	int8_t a = 0;
	
	if (t == Parser::Value::Type::INTEGER) {
		a = i % sizeof (Integer);
	}
#if USE_LONGINT
	else if (t == Parser::Value::Type::LONG_INTEGER) {
		a = i % sizeof (LongInteger);
		if (a > 0)
			a = sizeof (LongInteger) - a;
	}
#endif
#if USE_REALS
	else if (t == Parser::Value::Type::REAL) {
		a = i % sizeof (Real);
		if (a > 0)
			a = sizeof (Real) - a;
	}
#if USE_LONG_REALS
	else if (t == Parser::Value::Type::LONG_REAL) {
		a = i % sizeof (LongReal);
		if (a > 0)
			a = sizeof (LongReal) - a;
	}
#endif
#endif // USE_REALS
	return a;
}

#endif // CONF_USE_ALIGN

} // namespace BASIC
