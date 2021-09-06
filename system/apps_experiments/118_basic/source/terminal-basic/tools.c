/*
 * Terminal-BASIC is a lightweight BASIC-like language interpreter
 * Copyright (C) 2017-2019 Andrey V. Skvortsov <starling13@mail.ru>
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

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "tools.h"

#define FLOAT_DIGITS10 7

BOOLEAN
tools_isAlpha(uint8_t c)
{
	/* cp866 alpha symbols */
	return (c >= 224 && c <= 247) ||
	       (c >= 128 && c <= 175) ||
	       (c >= 65 && c <= 90) ||
	       (c >= 97 && c <= 122);
}

BOOLEAN
tools_isAlphaNum(uint8_t c)
{
	return isdigit(c) || tools_isAlpha(c);
}

void
_ftoa(float f, char *buf)
{
	if (f != f) { // NaN - comparision invalid
		strcpy(buf, "NAN");
		return;
	} else if (f + f == f) { // machine zero
		if (1.f / f < 0.f)
			strcpy(buf, "-0.0");
		else if (f == 0.0f)
			strcpy(buf, "0.0");
		else if (f > 0.0f)
			strcpy(buf, "INF");
		else
			strcpy(buf, "-INF");
		return;
	}

	if (f < 0.f) { // Add "-" for negative ordinar values
		f = -f;
		*(buf++) = '-';
	}

	int32_t n = 0;
	// Move to set 1st significant decimal digit before decimal point
	if (f < 1e6f) {
		do {
			f *= 10.f;
			--n;
		} while (f < (1e6f - 0.5f));
	} else if (f >= 1e7f) {
		do {
			f /= 10.f;
			++n;
		} while (f >= 1e7f + 0.5f);
	}
	n += FLOAT_DIGITS10 - 1;
	uint32_t fi = f + 0.5f;

	uint8_t i;
	for (i = FLOAT_DIGITS10; i > 1; --i) {
		buf[i] = fi % 10 + '0';
		fi /= 10;
	}
	buf[1] = '.';
	buf[0] = fi % 10 + '0';
	buf += FLOAT_DIGITS10 + 1;
	if (n != 0) {
		*(buf++) = 'e';
		if (n < 0) {
			*(buf++) = '-';
			n = -n;
		}
		if (n >= 10)
			*(buf++) = n / 10 + '0';
		*(buf++) = n % 10 + '0';
	}

	*buf = '\0';
}

union U16 {
	uint16_t num;
	char bytes[sizeof(uint16_t)];
};

union U32 {
	uint32_t num;
	char bytes[sizeof(uint32_t)];
};

union U64 {
	uint64_t num;
	char bytes[sizeof(uint64_t)];
};

union R32 {
	float num;
	char bytes[sizeof(float)];
};

union R64 {
	double num;
	char bytes[sizeof(double)];
};

void
readU16(uint16_t *num, const uint8_t *buf)
{
	union U16 s;
	
	s.bytes[1] = *(buf++);
	s.bytes[0] = *buf;
	
	*num = s.num;
}

void
writeU16(uint16_t num, uint8_t *buf)
{
	union U16 s;
	
	s.num = num;
	
	*(buf++) = s.bytes[1];
	*buf = s.bytes[0];
}

void
readU32(uint32_t *num, const uint8_t *buf)
{
	union U32 s;
	
	s.bytes[3] = *(buf++);
	s.bytes[2] = *(buf++);
	s.bytes[1] = *(buf++);
	s.bytes[0] = *buf;
	
	*num = s.num;
}

void
writeU32(uint32_t num, uint8_t *buf)
{
	union U32 s;
	
	s.num = num;
	
	*(buf++) = s.bytes[3];
	*(buf++) = s.bytes[2];
	*(buf++) = s.bytes[1];
	*buf = s.bytes[0];
}

void
readU64(uint64_t *num, const uint8_t *buf)
{
	union U64 s;
	
	s.bytes[7] = *(buf++);
	s.bytes[6] = *(buf++);
	s.bytes[5] = *(buf++);
	s.bytes[4] = *(buf++);
	s.bytes[3] = *(buf++);
	s.bytes[2] = *(buf++);
	s.bytes[1] = *(buf++);
	s.bytes[0] = *buf;
	
	*num = s.num;
}

void
writeU64(uint64_t num, uint8_t *buf)
{
	union U64 s;
	
	s.num = num;
	
	*(buf++) = s.bytes[7];
	*(buf++) = s.bytes[6];
	*(buf++) = s.bytes[5];
	*(buf++) = s.bytes[4];
	*(buf++) = s.bytes[3];
	*(buf++) = s.bytes[2];
	*(buf++) = s.bytes[1];
	*buf = s.bytes[0];
}

void
readR32(float *num, const const uint8_t *buf)
{
	union R32 s;
	
	s.bytes[3] = *(buf++);
	s.bytes[2] = *(buf++);
	s.bytes[1] = *(buf++);
	s.bytes[0] = *buf;
	
	*num = s.num;
}

void
writeR32(float num, uint8_t *buf)
{
	union R32 s;
	
	s.num = num;
	
	*(buf++) = s.bytes[3];
	*(buf++) = s.bytes[2];
	*(buf++) = s.bytes[1];
	*buf = s.bytes[0];
}

void
writeR64(double num, uint8_t *buf)
{
	union R64 s;
	
	s.num = num;
	
	*(buf++) = s.bytes[7];
	*(buf++) = s.bytes[6];
	*(buf++) = s.bytes[5];
	*(buf++) = s.bytes[4];
	*(buf++) = s.bytes[3];
	*(buf++) = s.bytes[2];
	*(buf++) = s.bytes[1];
	*buf = s.bytes[0];
}

void
readR64(double *num, const uint8_t *buf)
{
	union U64 s;
	
	s.bytes[7] = *(buf++);
	s.bytes[6] = *(buf++);
	s.bytes[5] = *(buf++);
	s.bytes[4] = *(buf++);
	s.bytes[3] = *(buf++);
	s.bytes[2] = *(buf++);
	s.bytes[1] = *(buf++);
	s.bytes[0] = *buf;
	
	*num = s.num;
}
