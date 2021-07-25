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

#ifndef I2CEEPROM_HPP
#define I2CEEPROM_HPP

#include <Wire.h>

template<uint32_t SIZE, uint8_t pageSize>
class I2CEEPROM
{
	static_assert((SIZE > pageSize) && (SIZE % pageSize == 0),
	    "EEPROM size should be multiply of the page size");
public:

	I2CEEPROM(uint8_t address) :
	_address(address)
	{
	}

	bool
	writePage(uint16_t pageNum, const uint8_t *pageBuf)
	{
		const uint16_t pageAddr = pageNum*numPages;
		Wire.beginTransmission(_address);
		Wire.write(pageAddr >> 8); // MSB
		Wire.write(pageAddr & uint16_t(0xFF)); // LSB
		for (uint8_t c = 0; c < pageSize; ++c)
			Wire.write(pageBuf[c]);
		const uint8_t res = Wire.endTransmission();
		Serial.println(res, DEC);
		Wire.flush();
		return res == 0;
	}

	bool
	writeByte(uint16_t address, uint8_t byte)
	{
		Wire.beginTransmission(_address);
		Wire.write(address >> 8); // MSB
		Wire.write(address & 0xFF); // LSB
		Wire.write(byte);
		const uint8_t res = Wire.endTransmission();
		Wire.flush();
		return res == 0;
	}

	bool
	readByte(uint16_t address, uint8_t &byte)
	{
		Wire.beginTransmission(_address);
		Wire.write(address >> 8); // MSB
		Wire.write(address & 0xFF); // LSB
		const uint8_t res = Wire.endTransmission();
		if (res != 0)
			return false;
		Wire.requestFrom(_address, uint8_t(1));
		uint8_t repeat = 3;
		while (!Wire.available()) {
			delay(1);
			if (repeat-- == 0)
				break;
		}
		if (Wire.available()) {
			byte = Wire.read();
			return true;
		} else
			return false;
	}

	bool
	readPage(uint16_t pageNum, uint8_t *pageBuf)
	{
		const uint16_t pageAddr = pageNum*numPages;
		Wire.beginTransmission(_address);
		Wire.write(pageAddr >> 8); // MSB
		Wire.write(pageAddr & uint16_t(0xFF)); // LSB
		const uint8_t res = Wire.endTransmission();
		if (res != 0)
			return false;
		Wire.flush();
		Wire.requestFrom(_address, pageSize);
		for (uint8_t c = 0; c < pageSize; ++c) {
			uint8_t repeat = 3;
			while (!Wire.available()) {
				delay(1);
				if (repeat-- == 0)
					break;
			}
			if (Wire.available())
				pageBuf[c] = Wire.read();
			else
				return false;
		}
		return true;
	}
	
	template<typename T>
	bool
	read(uint16_t address, T &object)
	{
		uint8_t *buf = reinterpret_cast<uint8_t*>(&object);
		
		for (uint16_t i=0, position=0; i<sizeof(object); ++i, ++position) {
			if (position == 0) {
				Wire.beginTransmission(_address);
				Wire.write(address >> 8); // MSB
				Wire.write(address & uint16_t(0xFF)); // LSB
				const uint8_t res = Wire.endTransmission();
				if (res != 0)
					return false;
				Wire.flush();
				Wire.requestFrom(_address, sizeof(object));
			}
			if (position == pageSize) {
				position = 0;
				const uint8_t res = Wire.endTransmission();
				Wire.flush();
				if (res != 0)
					return false;
			}
			uint8_t repeat = 3;
			while (!Wire.available()) {
				delay(1);
				if (repeat-- == 0)
					break;
			}
			if (Wire.available())
				buf[i] = Wire.read();
			else
				return false;
		}
		return true;
	}
	
	template<typename T>
	bool
	write(uint16_t address, const T &object)
	{
		const uint8_t *buf = reinterpret_cast<const uint8_t*>(&object);
		for (uint16_t i=0, position=0; i<sizeof(object); ++i, ++position) {
			if (position == 0) {
				Wire.beginTransmission(_address);
				Wire.write(address >> 8); // MSB
				Wire.write(address & uint16_t(0xFF)); // LSB
			}
			if (position == pageSize) {
				position = 0;
				const uint8_t res = Wire.endTransmission();
				Wire.flush();
				if (res != 0)
					return false;
				if (i>=sizeof(object))
					return true;
			}
			Wire.write(buf[i]);
		}
		const uint8_t res = Wire.endTransmission();
		Wire.flush();
		return res == 0;
	}

	static constexpr uint16_t numPages = SIZE / pageSize;
	static constexpr uint32_t size = SIZE;

private:
	uint8_t _address;
};

typedef I2CEEPROM<16384ul, 64> AT28C128C;
typedef I2CEEPROM<32768ul, 64> AT28C256C;
typedef I2CEEPROM<65536ul, 128> AT28C512C;

#endif // I2CEEPROM_HPP
