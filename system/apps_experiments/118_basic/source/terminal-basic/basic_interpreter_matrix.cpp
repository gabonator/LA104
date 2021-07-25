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

#include "basic_interpreter.hpp"

#if USE_MATRIX

#include "matrix.hpp"
#include "ascii.hpp"

#include <assert.h>

namespace BASIC
{

ArrayFrame*
Interpreter::get2DArray(const char *name)
{
	ArrayFrame *array = _program.arrayByName(name);
	
	if (array == nullptr)
		raiseError(DYNAMIC_ERROR, NO_SUCH_ARRAY);
	else if (array->numDimensions != 2)
		raiseError(DYNAMIC_ERROR, DIMENSIONS_MISMATCH);
	
	return array;
}

ArrayFrame*
Interpreter::getSquareArray(const char *name)
{
	ArrayFrame *array = get2DArray(name);
	
	if (array != nullptr) {
		if (array->numDimensions != 2)
			raiseError(DYNAMIC_ERROR, DIMENSIONS_MISMATCH);
		else if (array->dimension[0] != array->dimension[1])
			raiseError(DYNAMIC_ERROR, SQUARE_MATRIX_EXPECTED);
	}
	
	return array;
}

void
Interpreter::zeroMatrix(const char *name)
{
	fillMatrix(name, Integer(0));
}

void
Interpreter::onesMatrix(const char *name)
{
	fillMatrix(name, Integer(1));
}

void
Interpreter::identMatrix(const char *name)
{
	ArrayFrame *array = getSquareArray(name);
	
	if (array != nullptr) {
		for (uint16_t row = 0; row <= array->dimension[0]; ++row) {
			for (uint16_t column = 0; column <= array->dimension[1];
			    ++column) {
				Parser::Value v;
				if (row == column)
					v = Integer(1);
				else
					v = Integer(0);
				if (!array->set(row*(array->dimension[1]+1)+column,
				    v))
					raiseError(DYNAMIC_ERROR,
					    INTERNAL_ERROR);
			}
		}
	}
}

void	
Interpreter::fillMatrix(const char *name, const Parser::Value &v)
{
	ArrayFrame *array = get2DArray(name);
	
	if (array != nullptr) {
		for (uint16_t index = 0; index<array->numElements(); ++index) {
			if (!array->set(index, v)) {
				raiseError(DYNAMIC_ERROR, INTERNAL_ERROR);
				return;
			}
		}
	}
}

void
Interpreter::printMatrix(const char *name)
{
	ArrayFrame *array = get2DArray(name);
	
	if (array == nullptr)
		return;
	
	for (uint16_t row = 0; row <= array->dimension[0]; ++row) {
		for (uint16_t column = 0; column <= array->dimension[1];
		     ++column) {
			Parser::Value v;
			if (array->get(row * (array->dimension[1] + 1) + column, v))
				this->print(v), _output.print(char(ASCII::SPACE));
			else
				raiseError(DYNAMIC_ERROR, INTERNAL_ERROR);
		}
		this->newline();
	}
}

void
Interpreter::matrixDet(const char *name)
{
	ArrayFrame *array = getSquareArray(name);
	
	if (array != nullptr) {
		const uint8_t eSize = Parser::Value::size(array->type);
		if (eSize == 0)
			return;
		uint16_t bufSize = 0;
		for (uint16_t i=1; i<array->dimension[0]+1; ++i)
			bufSize += i*i*eSize;
		if (_program._arraysEnd+bufSize >= _program._sp) {
			raiseError(DYNAMIC_ERROR, OUTTA_MEMORY);
			return;
		}
		uint8_t *tbuf = reinterpret_cast<uint8_t*>(_program._text+
		    _program._arraysEnd);
		_result.setType(array->type);
		switch (array->type) {
		case Parser::Value::INTEGER: {
			Integer r;
			if (!Matricies<Integer>::determinant(
			    reinterpret_cast<const Integer*>(array->data()),
			    array->dimension[0]+1, r,
			    reinterpret_cast<Integer*>(tbuf)))
				_result = false;
			_result = r;
		}
		break;
#if USE_LONGINT
		case Parser::Value::LONG_INTEGER:
			LongInteger r;
			if (!Matricies<LongInteger>::determinant(
			    reinterpret_cast<const LongInteger*>(array->data()),
			    array->dimension[0]+1, r,
			    reinterpret_cast<LongInteger*>(tbuf)))
				_result = false;
			_result = r;
#endif // USE_LONGINT
#if USE_REALS
		case Parser::Value::REAL: {
			Real r;
			if (!Matricies<Real>::determinant(
			    reinterpret_cast<const Real*>(array->data()),
			    array->dimension[0]+1, r,
			    reinterpret_cast<Real*>(tbuf)))
				_result = false;
			_result = r;
		}
		break;
#if USE_LONG_REALS
		case Parser::Value::LONG_REAL: {
			LongReal r;
			if (!Matricies<LongReal>::determinant(
			    reinterpret_cast<const LongReal*>(array->data()),
			    array->dimension[0]+1, r,
			    reinterpret_cast<LongReal*>(tbuf)))
				_result = false;
			_result = r;
		}
		break;
#endif
#endif // USE_REALS
		default:
			_result = false;
		}
	}
}

#if USE_DATA
void
Interpreter::matrixRead(const char *name)
{
	ArrayFrame *array = get2DArray(name);
	
	if (array != nullptr) {
		for (uint16_t row = 0; row <= array->dimension[0]; ++row) {
			for (uint16_t column = 0; column <= array->dimension[1];
			    ++column) {
				Parser::Value v;
				this->read(v);
				if (!array->set(row*
				    (array->dimension[1]+1)+column, v))
					raiseError(DYNAMIC_ERROR,
					    INTERNAL_ERROR);
			}
		}
	}
}
#endif // USE_DATA

void
Interpreter::setMatrixSize(ArrayFrame &array, uint16_t rows, uint16_t columns)
{
	const uint16_t oldSize = array.size();
	array.dimension[0] = rows, array.dimension[1] = columns;
	const uint16_t newSize = array.size();
	int32_t delta = int32_t(newSize) - int32_t(oldSize);
	const uint16_t aIndex = _program.objectIndex(&array);
	if (_program._arraysEnd + delta >= _program._sp) {
		raiseError(DYNAMIC_ERROR, OUTTA_MEMORY);
		return;
	} else {
		const uint16_t oldIndex = aIndex+oldSize;
		const uint16_t newIndex = aIndex+newSize;
		memmove(_program._text + newIndex, _program._text + oldIndex,
		    _program._arraysEnd-oldIndex);
		_program._arraysEnd += delta;
	}
}

void
Interpreter::assignMatrix(const char *name, const char *first, const char *second,
    MatrixOperation_t op)
{
	ArrayFrame *array = get2DArray(name); // target matrix
	ArrayFrame *arrayFirst = get2DArray(first); // first operand (required)
	
	if (array == nullptr || arrayFirst == nullptr)
		return;
	
	const uint8_t eSize = Parser::Value::size(arrayFirst->type);
	if (eSize == 0)
		return;
	
	// If first right side operand is not the target mat, resize
	// target according to source and copy it's data
	if (array != arrayFirst) {
		setMatrixSize(*array, arrayFirst->dimension[0],
		    arrayFirst->dimension[1]);
		// If matrices are of the same type, simply memcpy
		// In other case assign members through the Value
		// converter
		if (array->type == arrayFirst->type)
			memcpy(array->data(), arrayFirst->data(), array->dataSize());
		else {
			Parser::Value val;
			for (uint16_t index = 0; index<array->numElements();
			    ++index) {
				if (!arrayFirst->get(index, val) ||
				    !array->set(index, val)) {
					raiseError(DYNAMIC_ERROR, INTERNAL_ERROR);
					return;
				}
			}
		}
	}
	switch (op) {
	case MO_NOP: // simple assign, already done
		break;
	case MO_SCALE: { // multiply by scalar
		Parser::Value v;
		if (!popValue(v)) {
			raiseError(DYNAMIC_ERROR, INTERNAL_ERROR);
			return;
		}
		Parser::Value elm;
		for (uint16_t index = 0; index<array->numElements(); ++index) {
			if (!array->get(index, elm) ||
			    !array->set(index, elm*=v)) {
				raiseError(DYNAMIC_ERROR, INVALID_ELEMENT_INDEX);
				return;
			}
		}
	}
		break;
	case MO_TRANSPOSE: { // source mat already have been copied,
			     // performng in-place transpose
		switch (array->type) {
		case Parser::Value::INTEGER:
			Matricies<Integer>::transpose(
			    reinterpret_cast<Integer*>(array->data()),
			    array->dimension[0]+1, array->dimension[1]+1);
			break;
#if USE_LONGINT
		case Parser::Value::LONG_INTEGER:
			Matricies<LongInteger>::transpose(
			    reinterpret_cast<LongInteger*>(array->data()),
			    array->dimension[0]+1, array->dimension[1]+1);
			break;	
#endif // USE_LONGINT
#if USE_REALS
		case Parser::Value::REAL:
			Matricies<Real>::transpose(
			    reinterpret_cast<Real*>(array->data()),
			    array->dimension[0]+1, array->dimension[1]+1);
			break;
#if USE_LONG_REALS
		case Parser::Value::LONG_REAL:
			Matricies<LongReal>::transpose(
			    reinterpret_cast<LongReal*>(array->data()),
			    array->dimension[0]+1, array->dimension[1]+1);
			break;
#endif
#endif // USE_REALS
		default:
			break;
		}
		setMatrixSize(*array, arrayFirst->dimension[1],
		    arrayFirst->dimension[0]);
	}
		break;
	case MO_SUM:
	case MO_SUB: {
		assert(second != nullptr);
		ArrayFrame *arraySecond = getSquareArray(second);
		if (arraySecond == nullptr)
			return;
		
		if (arraySecond->dimension[0] != array->dimension[0] ||
		    arraySecond->dimension[1] != array->dimension[1]) {
			raiseError(DYNAMIC_ERROR, DIMENSIONS_MISMATCH);
			return;
		}
		Parser::Value val, valOld;
		for (uint16_t index = 0; index<array->numElements(); ++index) {
			if (arraySecond->get(index, val) &&
			    array->get(index, valOld)) {
				if (op == MO_SUM)
					valOld += val;
				else // MO_SUB
					valOld -=val;
				if (array->set(index, valOld))
					continue;
			}
			raiseError(DYNAMIC_ERROR, INTERNAL_ERROR);
			return;
		}
	}
		break;
	case MO_MUL: {
		assert(second != nullptr);
		ArrayFrame *arraySecond = get2DArray(second);
		if (arraySecond == nullptr)
			return;
		if (arraySecond->type != arrayFirst->type) {
			raiseError(DYNAMIC_ERROR, NO_SUCH_ARRAY); //@TODO type mismatch
			return;
		}
		if (arraySecond->dimension[0] != arrayFirst->dimension[1]) {
			raiseError(DYNAMIC_ERROR, DIMENSIONS_MISMATCH);
			return;
		}
		const uint16_t r = arrayFirst->dimension[0]+1;
		const uint16_t c = arraySecond->dimension[1]+1;
		
		const uint16_t bufSize = r*c*eSize;
		if (_program._arraysEnd+bufSize >= _program._sp) {
			raiseError(DYNAMIC_ERROR, OUTTA_MEMORY);
			return;
		}
		uint8_t *tbuf = reinterpret_cast<uint8_t*>(_program._text+
		    _program._arraysEnd);
		switch (arrayFirst->type) {
		case Parser::Value::INTEGER:
			Matricies<Integer>::mul(
			    reinterpret_cast<Integer*>(arrayFirst->data()),
			    arrayFirst->dimension[0]+1, arrayFirst->dimension[1]+1,
			    reinterpret_cast<Integer*>(arraySecond->data()),
			    arraySecond->dimension[0]+1, arraySecond->dimension[1]+1,
			    reinterpret_cast<Integer*>(tbuf));
			break;
#if USE_LONGINT
		case Parser::Value::LONG_INTEGER:
			Matricies<LongInteger>::mul(
			    reinterpret_cast<LongInteger*>(arrayFirst->data()),
			    arrayFirst->dimension[0]+1, arrayFirst->dimension[1]+1,
			    reinterpret_cast<LongInteger*>(arraySecond->data()),
			    arraySecond->dimension[0]+1, arraySecond->dimension[1]+1,
			    reinterpret_cast<LongInteger*>(tbuf));
			break;
#endif //  USE_LONGINT
#if USE_REALS
		case Parser::Value::REAL:
			Matricies<Real>::mul(
			    reinterpret_cast<Real*>(arrayFirst->data()),
			    arrayFirst->dimension[0]+1, arrayFirst->dimension[1]+1,
			    reinterpret_cast<Real*>(arraySecond->data()),
			    arraySecond->dimension[0]+1, arraySecond->dimension[1]+1,
			    reinterpret_cast<Real*>(tbuf));
			break;
#if USE_LONG_REALS
		case Parser::Value::LONG_REAL:
			Matricies<LongReal>::mul(
			    reinterpret_cast<LongReal*>(arrayFirst->data()),
			    arrayFirst->dimension[0]+1, arrayFirst->dimension[1]+1,
			    reinterpret_cast<LongReal*>(arraySecond->data()),
			    arraySecond->dimension[0]+1, arraySecond->dimension[1]+1,
			    reinterpret_cast<LongReal*>(tbuf));
			break;
#endif
#endif // USE_REALS
		default:
			return;
		}
		setMatrixSize(*array, r-1, c-1);
		memcpy(array->data(), tbuf, bufSize);
	}
		return;
	case MO_INVERT: {
		if (array->dimension[0] != array->dimension[1]) {
			raiseError(DYNAMIC_ERROR, DIMENSIONS_MISMATCH);
			return;
		}
		const uint16_t r = array->dimension[0]+1;
		const uint16_t bufSize = (r+r+r*r)*eSize;
		if (_program._arraysEnd+bufSize >= _program._sp) {
			raiseError(DYNAMIC_ERROR, OUTTA_MEMORY);
			return;
		}
		uint8_t *tbuf = reinterpret_cast<uint8_t*>(_program._text+
		    _program._arraysEnd);
		bool res = false;
		switch (array->type) {
		case Parser::Value::INTEGER:
			res = Matricies<Integer>::invert(
			    reinterpret_cast<Integer*>(array->data()),
			    r, reinterpret_cast<Integer*>(tbuf));
			break;
#if USE_LONGINT
		case Parser::Value::LONG_INTEGER:
			res = Matricies<LongInteger>::invert(
			    reinterpret_cast<LongInteger*>(array->data()),
			    r, reinterpret_cast<LongInteger*>(tbuf));
			break;
#endif // USE_LONGINT
#if USE_REALS
		case Parser::Value::REAL:
			res = Matricies<Real>::invert(
			    reinterpret_cast<Real*>(array->data()),
			    r, reinterpret_cast<Real*>(tbuf));
			break;
#if USE_LONG_REALS
		case Parser::Value::LONG_REAL:
			res = Matricies<LongReal>::invert(
			    reinterpret_cast<LongReal*>(array->data()),
			    r, reinterpret_cast<LongReal*>(tbuf));
			break;
#endif
#endif // USE_REALS
		default:
			break;
		}
		_result = res;
	}
		return;
	default:
		return;
	}
}

} // namespace BASIC

#endif // USE_MATRIX
