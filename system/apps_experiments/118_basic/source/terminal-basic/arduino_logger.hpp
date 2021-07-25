/*
 * ArduinoExt is a set of utility libraries for Arduino
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
 *     <https://bitbucket.org/%7Bf50d6fee-8627-4ce4-848d-829168eedae5%7D/>
 *
 * This program is free software: is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * ArduinoExt library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with Posixcpp library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef ARDUINO_LOGGER_HPP
#define ARDUINO_LOGGER_HPP

#include <stdint.h>

#include "arduinoext.hpp"
#include "Stream.h"
#include "Arduino.h"

#define ARDUINO_LOG 0

#if ARDUINO_LOG
#define LOG_INIT(T) Logger::init(T)
#define LOG(args...) Logger::log(args)
#define LOG_TRACE LOG(__PRETTY_FUNCTION__)
#else
#define LOG_INIT(T)
#define LOG(...)
#define LOG_TRACE
#endif

Package(Logger) {
	public:

	enum class format_t
	{
		bin = 2, dec = 10, oct = 8, hex = 16, endl
	};

	static void init(Stream&);

	template<typename... Args>
	    static void log(const Args&... args)
	{
		_log(args...);
		_instance._stream->println();
	}

	private:

	friend Logger &operator<<(Logger &logger, format_t formaat)
	{
		logger._format = formaat;
		return logger;
	}
	
	friend Logger &operator<<(Logger &logger, const Printable &p) {
		logger._stream->print(p);
		return logger.delimiter();
	}

	friend Logger &operator<<(Logger &logger, uint32_t val) {
		logger._stream->print(val, int(_format));
		return logger.delimiter();
	}
	
	friend Logger &operator<<(Logger &logger, int32_t val) {
		logger._stream->print(val, int(_format));
		return logger.delimiter();
	}
	
	friend Logger &operator<<(Logger &logger, uint16_t val) {
		logger._stream->print(val, int(_format));
		return logger.delimiter();
	}
	
	friend Logger &operator<<(Logger &logger, int16_t val) {
		logger._stream->print(val, int(_format));
		return logger.delimiter();
	}
	
	friend Logger &operator<<(Logger &logger, uint8_t val) {
		logger._stream->print(val, int(_format));
		return logger.delimiter();
	}
	
	friend Logger &operator<<(Logger &logger, int8_t val) {
		logger._stream->print(val, int(_format));
		return logger.delimiter();
	}

	friend Logger &operator<<(Logger &logger, const char* first) {
		logger._stream->print(first);
		return logger.delimiter();
	}

	friend Logger &operator<<(Logger &logger, char* first) {
		logger._stream->print(first);
		return logger.delimiter();
	}

	template<typename T>
	friend Logger &operator<<(Logger &logger, T *first)
	{
		logger._stream->print(intptr_t(first), 16);
		return logger.delimiter();
	}

	template<typename T>
	friend Logger &operator<<(Logger &logger, T first)
	{
		logger._stream->print(first);
		return logger.delimiter();
	}

	template<typename T>
	static void _log(T first)
	{
		_instance << first;
	}

	template<typename T, typename... Args>
	static void _log(T first, const Args&... args)
	{
		_log(first);
		_log(args...);
	}
	
	Logger &delimiter()
	{
		_instance._stream->print(" ");
		return *this;
	}

	static Logger _instance;

	static format_t _format;

	Stream *_stream;
};

#endif
