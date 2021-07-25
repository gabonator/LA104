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

#include "basic_extmemfs.hpp"

#if CONF_USE_EXTMEMFS

#include "basic_program.hpp"
#include <assert.h>
#ifdef ARDUINO_ARCH_ESP32
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

namespace BASIC
{

static const uint8_t sdfsCommands[] PROGMEM = {
	'D', 'C', 'H', 'A', 'I', 'N', ASCII_NUL,
	'D', 'I', 'R', 'E', 'C', 'T', 'O', 'R', 'Y', ASCII_NUL,
	'D', 'L', 'O', 'A', 'D', ASCII_NUL,
	'D', 'S', 'A', 'V', 'E', ASCII_NUL,
#if USE_FILEOP
	'F', 'C', 'L', 'O', 'S', 'E', ASCII_NUL,
	'F', 'D', 'E', 'L', 'E', 'T', 'E',  ASCII_NUL,
	'F', 'S', 'E', 'E', 'K', ASCII_NUL,
	'F', 'W', 'R', 'I', 'T', 'E', ASCII_NUL,
#endif
	'H', 'E', 'A', 'D', 'E', 'R', ASCII_NUL,
	'S', 'C', 'R', 'A', 'T', 'C', 'H', ASCII_NUL,
	ASCII_ETX
};

#if USE_FILEOP
static const uint8_t sdfsFunctions[] PROGMEM = {
	'F', 'O', 'P', 'E', 'N', ASCII_NUL,
	'F', 'R', 'E', 'A', 'D', ASCII_NUL,
	'F', 'S', 'I', 'Z', 'E', ASCII_NUL,
	ASCII_ETX
};

#endif // USE_FILEOP

const FunctionBlock::function  ExtmemFSModule::_commands[] PROGMEM = {
	ExtmemFSModule::dchain,
	ExtmemFSModule::directory,
	ExtmemFSModule::dload,
	ExtmemFSModule::dsave,
#if USE_FILEOP
	ExtmemFSModule::com_fclose,
	ExtmemFSModule::com_fdelete,
	ExtmemFSModule::com_fseek,
	ExtmemFSModule::com_fwrite,
#endif
	ExtmemFSModule::header,
	ExtmemFSModule::scratch
#if FAST_MODULE_CALL
	, nullptr
#endif
};

#if USE_FILEOP
const FunctionBlock::function ExtmemFSModule::_functions[] PROGMEM = {
	ExtmemFSModule::func_fopen,
	ExtmemFSModule::func_fread,
	ExtmemFSModule::func_fsize
};
#endif // USE_FILEOP

ExtmemFSModule::ExtmemFSModule()
{
	commands = _commands;
	commandTokens = sdfsCommands;
#if USE_FILEOP
	functions = _functions;
	functionTokens = sdfsFunctions;
#endif
}

void
ExtmemFSModule::loadAutorun(Interpreter& i)
{
	static const char ar[] PROGMEM = "/AUTORUN.BAS";
	char ss[13];
	strcpy_P(ss, ar);
	
	if (!HAL_extmem_fileExists(ss))
		return;
	HAL_extmem_file_t f = HAL_extmem_openfile(ss);
	if (f == 0)
		return;
	
	FileStream fs(f);
	if (!_loadText(fs, i))
		return;
	
	i.run();
}

#if USE_FILEOP

bool
ExtmemFSModule::com_fclose(Interpreter& i)
{
	INT iv;
	if (getIntegerFromStack(i, iv)) {
		HAL_extmem_closefile(HAL_extmem_file_t(iv));
		return true;
	}
	return false;
}

bool
ExtmemFSModule::com_fseek(Interpreter& i)
{
	INT iv;
	if (getIntegerFromStack(i, iv)) {
		INT bv;
		if (getIntegerFromStack(i, bv)) {
			HAL_extmem_setfileposition(HAL_extmem_file_t(iv), bv);
			return true;
		}
	}
	return false;
}

bool
ExtmemFSModule::com_fwrite(Interpreter& i)
{
	INT iv;
	if (getIntegerFromStack(i, iv)) {
		INT bv;
		if (getIntegerFromStack(i, bv)) {
			HAL_extmem_writetofile(HAL_extmem_file_t(iv), bv);
			return true;
		}
	}
	return false;
}

bool
ExtmemFSModule::com_fdelete(Interpreter& i)
{
	const char* s;
	if (i.popString(s)) {
		HAL_extmem_deletefile(s);
		return true;
	}
	return false;
}

bool
ExtmemFSModule::func_fopen(Interpreter& i)
{
	const char* s;
	if (i.popString(s)) {
		HAL_extmem_file_t f = HAL_extmem_openfile(s);
		if (i.pushValue(Integer(f)))
			return true;
		HAL_extmem_closefile(f);
	}
	return false;
}

bool
ExtmemFSModule::func_fread(Interpreter& i)
{
	INT iv;
	if (getIntegerFromStack(i, iv)) {
		if (i.pushValue(
		    INT(HAL_extmem_readfromfile(HAL_extmem_file_t(iv)))))
			return true;
	}
	return false;
}

bool
ExtmemFSModule::func_fsize(Interpreter& i)
{
	INT iv;
	if (getIntegerFromStack(i, iv)) {
		if (i.pushValue(INT(HAL_extmem_getfilesize(iv))))
			return true;
	}
	return false;
}

#endif // USE_FILEOP

bool
ExtmemFSModule::directory(Interpreter &i)
{
	static const char str[] PROGMEM = "SD CARD CONTENTS";
	
	uint16_t startFile = 0;
	uint16_t endFile = 65535;
	INT iv;
	if (getIntegerFromStack(i, iv)) {
		startFile = iv;
		if (getIntegerFromStack(i, iv)) {
			endFile = startFile;
			startFile = iv;
		}
	}
	
	char buf[17];
	strcpy_P(buf, (PGM_P)str);
	i.print(buf);
	i.newline();
	
	const uint16_t numFiles = HAL_extmem_getnumfiles();
	char fname[13];
	for (uint16_t index=0; index<numFiles; ++index) {
		if (index < startFile)
			continue;
		if (index > endFile)
			break;
		i.print(Integer(index));
		i.print('\t');
		HAL_extmem_getfilename(index, fname);
		i.print(fname);
		uint8_t len = min(uint8_t(13-strlen(fname)),
		    uint8_t(13));
		while (len-- > 0)
			i.print(' ');
		HAL_extmem_file_t f = HAL_extmem_openfile(fname);
		i.print(Integer(HAL_extmem_getfilesize(f)));
		HAL_extmem_closefile(f);
		i.newline();
	}
	return true;
}

bool
ExtmemFSModule::scratch(Interpreter &i)
{
	if (!i.confirm())
		return true;

	char ss[16];
	if (getFileName(i, ss)) {
		HAL_extmem_deletefile(ss);
		return true;
	} else
		return false;
}

bool
ExtmemFSModule::dchain(Interpreter &i)
{
	char ss[16];
	if (!getFileName(i, ss))
		return false;
	
	HAL_extmem_file_t f = HAL_extmem_openfile(ss);
	if (f == 0)
		return false;
	
	FileStream fs(f);
	
	i._program.clearProg();
	i._program.moveData(0);
	i._program.jump(0);
	i.stop();
	return _loadText(fs, i);
}

bool
ExtmemFSModule::dsave(Interpreter &i)
{
	FileStream fs;
	
	{ // Stack section 1
		char ss[16];
		if (getFileName(i, ss))
			HAL_extmem_deletefile(ss);
		HAL_extmem_file_t f = HAL_extmem_openfile(ss);
		if (f == 0)
			return false;
		fs.setFile(f);
	} // Stack section 1
	
	i._program.reset();
	{ // Stack section 2
	Lexer lex;
	for (Program::Line *s = i._program.getNextLine(); s != nullptr;
	    s = i._program.getNextLine()) {
		fs.print(READ_VALUE(s->number));
		lex.init(s->text, true);
		Token tPrev = Token::NOTOKENS;
		while (lex.getNext()) {
			Token t = lex.getToken();
			if (t != Token::COMMA &&
			    t != Token::RPAREN &&
			    tPrev != Token::LPAREN)
			    fs.write(' ');
			tPrev = t;
			if (t < Token::INTEGER_IDENT) {
				uint8_t buf[16];
				const bool res = Lexer::getTokenString(t,
				    reinterpret_cast<uint8_t*>(buf));
				if (res)
					fs.print((const char*)buf);
				else {
					fs.close();
					return false;
				}
				if (t == Token::KW_REM) {
					fs.write(' ');
					fs.print((const char*)s->text +
					    lex.getPointer());
					break;
				}
			} else if (t < Token::C_INTEGER) {
				fs.print(lex.id());
			} else if (t < Token::C_STRING) {
				lex.getValue().printTo(fs);
			} else if (t == Token::C_STRING) {
				fs.write('"');
				fs.print(lex.id());
				fs.write('"');
			}
#if FAST_MODULE_CALL
			else if (t == Token::COMMAND) {
				uint8_t buf[16];
				FunctionBlock::command com =
				    reinterpret_cast<FunctionBlock::command>(
				    readValue<uintptr_t>((const uint8_t*)lex.id()));
				i.parser().getCommandName(com, buf);
				fs.print((const char*)buf);
			}
#endif
		}
		fs.print('\n');
	}
	} // Stack section 2
	fs.close();
	return true;
}

bool
ExtmemFSModule::_loadText(FileStream &f, Interpreter &i)
{
	while (true) {
		char buf[PROGSTRINGSIZE] = {0, };
		// f.setTimeout(10);
		size_t res = 0;
		int c;
		while (f.available() > 0) {
			c = f.read();
			if (c == '\r') {
			    continue;
			} else if (c == '\n') {
			    buf[res] = '\0';
			    break;
			} else if (res < (PROGSTRINGSIZE-1))
				buf[res++] = c;
		}
		if (res > 0) {
                	if (buf[res-1] == '\r')
                        	buf[res-1] = 0;
			Lexer lex;
			lex.init((uint8_t*)buf, false);
			if (!lex.getNext() || lex.getToken() !=
			    Token::C_INTEGER)
				return false;
			if (!i._program.addLine(i.parser(), Integer(lex.getValue()),
			    (uint8_t*)buf+lex.getPointer()))
				return false;
		} else
			break;
	}
	
	f.close();
	i._program.reset();
	
	return true;
}

bool
ExtmemFSModule::dload(Interpreter &i)
{
	char ss[16];
	if (!getFileName(i, ss))
		return false;
	
	if (!HAL_extmem_fileExists(ss))
		return false;
	HAL_extmem_file_t f = HAL_extmem_openfile(ss);
	if (f == 0)
		return false;
	
	FileStream fs(f);
	
	i._program.newProg();
	
	return _loadText(fs, i);
}

bool
ExtmemFSModule::header(Interpreter &i)
{
	if (!i.confirm())
		return true;

	char fname[13];
	while (HAL_extmem_getnumfiles() > 0) {
		HAL_extmem_getfilename(0, fname);
		HAL_extmem_deletefile(fname);
	}
	return true;
}

bool
ExtmemFSModule::getFileName(Interpreter &i, char ss[])
{
	static const char strBAS[] PROGMEM = ".BAS";
	
	const char *s;
	if (!i.popString(s))
		return false;

	const uint8_t len = strlen(s);
	strcpy(ss, s);
	strcpy_P(ss + len, (PGM_P)strBAS);

	return HAL_extmem_fileExists(ss);
}

} // namespace BASIC

#endif // USESD
