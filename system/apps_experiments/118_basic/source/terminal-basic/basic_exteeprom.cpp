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

#include "basic.hpp"

#if USE_EXTEEPROM

#include "basic_exteeprom.hpp"
#include "basic_interpreter.hpp"
#include "basic_program.hpp"

#include "i2ceeprom.hpp"

namespace BASIC
{

static AT28C256C i2c_eeprom(0x50);

static const uint8_t extEEPROMCommandTokens[] PROGMEM = {
	'E', 'C', 'H', 'A', 'I', 'N' + 0x80,
	'E', 'L', 'O', 'A', 'D' + 0x80,
	'E', 'S', 'A', 'V', 'E' + 0x80,
	0
};

const FunctionBlock::function ExtEEPROM::_commands[] PROGMEM = {
	ExtEEPROM::com_echain,
	ExtEEPROM::com_eload,
	ExtEEPROM::com_esave
};

struct PACKED ZoneHeader
{
	uint16_t number;
	uint16_t crc;
	uint16_t textEnd, varsEnd, arraysEnd, sp;
};

static constexpr const uint16_t zoneSize = SINGLE_PROGSIZE+sizeof(ZoneHeader);

class ZoneInfo
{
	uint16_t crc_16;
	uint16_t text_end;
	uint16_t vars_end;
	uint16_t arrays_end;
};

ExtEEPROM::ExtEEPROM()
{
	commands = _commands;
	commandTokens = extEEPROMCommandTokens;
}

void
ExtEEPROM::_init()
{
}

bool
ExtEEPROM::com_echain(Interpreter &i)
{
	INT zoneNumber;
	if (!getIntegerFromStack(i, zoneNumber))
		return false;

	const uint16_t zoneAddr = zoneNumber*zoneSize;
	if (zoneAddr+zoneSize > AT28C256C::size)
		return false;
	// Read program from EEPROM
	ZoneHeader h;
	if (!i2c_eeprom.read(zoneAddr, h))
		return false;
	i._program.clearProg();
	i._program.moveData(h.textEnd);
	for (uint16_t p = 0; p < h.textEnd; ++p) {
		delay(5);
		if (!i2c_eeprom.readByte(zoneAddr+p+sizeof(ZoneHeader),
		    reinterpret_cast<uint8_t&>(i._program._text[p])))
			return false;
	}
	i.newline();
	i._program.jump(0);
	
	return true;
}

bool
ExtEEPROM::com_eload(Interpreter &i)
{
	INT zoneNumber;
	if (!getIntegerFromStack(i, zoneNumber))
            return false;

	const uint16_t zoneAddr = zoneNumber*zoneSize;
	if (zoneAddr+zoneSize > AT28C256C::size)
		return false;
	// Read program from EEPROM
	i.newProgram();
	ZoneHeader h;
	if (!i2c_eeprom.read(zoneAddr, h))
		return false;
	for (uint16_t p = 0; p<h.arraysEnd; ++p) {
		delay(5);
		if (!i2c_eeprom.readByte(zoneAddr+sizeof(ZoneHeader)+p,
		    reinterpret_cast<uint8_t&>(i._program._text[p])))
			return false;
		else
			i.print('.');
	}
	i.newline();
	i._program.setTextEnd(h.textEnd);
	i._program.setVarsEnd(h.varsEnd);
	i._program.setArraysEnd(h.arraysEnd);
	i._program.setSP(h.sp);
	
	return true;
}

bool
ExtEEPROM::com_esave(Interpreter &i)
{
	INT zoneNumber;
	if (!getIntegerFromStack(i, zoneNumber))
            return false;

	const uint16_t zoneAddr = zoneNumber*zoneSize;
	if (zoneAddr+zoneSize > AT28C256C::size)
		return false;
	// Write program to EEPROM
	ZoneHeader h;
	h.textEnd = i._program.textEnd();
	h.varsEnd = i._program.varsEnd();
	h.arraysEnd = i._program.arraysEnd();
	h.sp = i._program.sp();
	if (!i2c_eeprom.write(zoneAddr, h))
		return false;
	for (uint16_t p = 0; p<h.arraysEnd; ++p) {
		delay(5);
		if (!i2c_eeprom.writeByte(zoneAddr+sizeof(ZoneHeader)+p,
		    i._program._text[p]))
			return false;
		else
			i.print('.');
	}
	i.newline();
	return true;
}

}

#endif // USE_EXTEEPROM
