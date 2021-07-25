/*
 * ArduinoExt is a set of utility libraries for Arduino
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019,2020 Terminal-BASIC team
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

#ifndef SERIALLIGHT_HPP
#define SERIALLIGHT_HPP

#include <Arduino.h>
#include <Stream.h>

#include "HardwareSerial.h"

#ifdef ARDUINO_ARCH_SAM

typedef HardwareSerial SerialLight;

#define SerialL Serial
#ifdef HAVE_HWSERIAL1
#define SerialL1 Serial1
#endif
#ifdef HAVE_HWSERIAL2
#define SerialL2 Serial2
#endif
#ifdef HAVE_HWSERIAL3
#define SerialL3 Serial3
#endif

#elif ARDUINO_ARCH_AVR

#define SERIAL_8N1 0x06
class SerialLight : public Stream
{
public:
	SerialLight(
		volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
		volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
		volatile uint8_t *ucsrc, volatile uint8_t *udr);

	void
	begin(unsigned long baud)
	{
		begin(baud, SERIAL_8N1);
	}
	void begin(unsigned long, uint8_t);

	int available() override;
	void flush() override;
	int peek() override;
	int read() override;
	size_t write(uint8_t) override;
private:
	volatile uint8_t * const _ubrrh;
	volatile uint8_t * const _ubrrl;
	volatile uint8_t * const _ucsra;
	volatile uint8_t * const _ucsrb;
	volatile uint8_t * const _ucsrc;
	volatile uint8_t * const _udr;
	bool _hasByte;
	uint8_t _byte;
};

extern SerialLight SerialL;
#ifdef HAVE_HWSERIAL1
extern SerialLight SerialL1;
#endif
#ifdef HAVE_HWSERIAL2
extern SerialLight SerialL2;
#endif
#ifdef HAVE_HWSERIAL3
extern SerialLight SerialL3;
#endif

#endif

#endif // SERIALLIGHT_HPP
