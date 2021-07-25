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

#ifndef BASIC_SDFS_HPP
#define BASIC_SDFS_HPP

#include "basic_functionblock.hpp"
#include "basic_interpreter.hpp"

#if CONF_USE_EXTMEMFS

namespace BASIC
{

class FileStream : public Stream
{
public:
	
	FileStream() : m_file(0) {}
	
	explicit FileStream(HAL_extmem_file_t f) :
	m_file(f) {}
	
	void setFile(HAL_extmem_file_t f)
	{
		m_file = f;
	}
	
	void close()
	{
		HAL_extmem_closefile(m_file);
	}
	
private:
	
	HAL_extmem_file_t m_file;
	
// Print  interface
public:
	size_t write(uint8_t b) override
	{
		HAL_extmem_writetofile(m_file, b);
		return 1;
	}
	
// Stream interface
public:
	int available() override
	{
		return HAL_extmem_getfilesize(m_file)-
		    HAL_extmem_getfileposition(m_file);
	}
	
	void flush() override
	{
	}
	
	int peek() override
	{
		auto pos = HAL_extmem_getfileposition(m_file);
		int res = HAL_extmem_readfromfile(m_file);
		HAL_extmem_setfileposition(m_file, pos);
		return res;
	}
	
	int read() override
	{
		return HAL_extmem_readfromfile(m_file);
	}
};

/**
 * @brief Module with commands to store the programs on the exxternal memory
 *     file system
 */
class ExtmemFSModule : public FunctionBlock
{
	// Function block interface
public:
	ExtmemFSModule();
	
	void loadAutorun(Interpreter&);
	
private:
	static bool dchain(Interpreter&);
	static bool dsave(Interpreter&);
	static bool directory(Interpreter&);
	static bool scratch(Interpreter&);
	static bool dload(Interpreter&);
	static bool header(Interpreter&);
#if USE_FILEOP
	static bool com_fclose(Interpreter&);
	static bool com_fdelete(Interpreter&);
	static bool com_fseek(Interpreter&);
	static bool com_fwrite(Interpreter&);
	
	static bool func_fopen(Interpreter&);
	static bool func_fsize(Interpreter&);
	static bool func_fread(Interpreter&);
#endif // USE_FILEOP
	static bool getFileName(Interpreter&, char[]);
	static bool _loadText(FileStream&, Interpreter&);
	
	static const FunctionBlock::function _commands[] PROGMEM;
#if USE_FILEOP
	static const FunctionBlock::function _functions[] PROGMEM;
#endif
};

} // namespace BASIC

#endif // USESD

#endif // BASIC_SDFS_HPP
