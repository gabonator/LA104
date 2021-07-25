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

#ifdef ARDUINO_ARCH_ESP8266

#include "HAL.h"
#include "FS.h"

#if HAL_NVRAM
#define NVRAMSIZE 32768
static File f;
#endif

#if HAL_EXTMEM

#define HAL_ESP8266_EXTEM_SPIFFS 1
#define HAL_ESP8266_EXTEM_SD 2

#define HAL_ESP8266_EXTEM HAL_ESP8266_EXTEM_SPIFFS

#if HAL_ESP8266 == HAL_ESP8266_EXTEM_SD
#include <SD.h>
#endif

static File extmem_files[HAL_EXTMEM_NUM_FILES];

#endif // HAL_EXTMEM

__BEGIN_DECLS

void
HAL_initialize_concrete()
{
	if (!SPIFFS.begin())
		exit(1);
#if HAL_NVRAM
	f = SPIFFS.open("/nvram.bin", "r+");
	if (!f) {
		f = SPIFFS.open("/nvram.bin", "w");
		if (!f)
			exit(4);
		f.close();
	}
#endif // HAL_NVRAM
#if HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SD
	if (!SD.begin(SS))
		exit(3);
#endif // HAL_ESP8266
}

__END_DECLS

void
HAL_finalize()
{
}

#if HAL_NVRAM

HAL_nvram_address_t
HAL_nvram_getsize()
{
	return NVRAMSIZE;
}

uint8_t
HAL_nvram_read(HAL_nvram_address_t addr)
{
	f = SPIFFS.open("/nvram.bin", "r");
	if (!f)
		exit(2);
	if (!f.seek(uint32_t(addr)))
		exit(3);
	uint8_t r = f.read();
	f.close();
	return r;
}

void
HAL_nvram_write(HAL_nvram_address_t addr, uint8_t b)
{
	f = SPIFFS.open("/nvram.bin", "r+");
	if (!f)
		exit(5);

	if (f.size() > uint32_t(addr)) {
		if (!f.seek(uint32_t(addr)))
			exit(6);
	} else {
		if (!f.seek(f.size()))
			exit(7);
		while (f.size() < uint32_t(addr)) {
			f.write(0xFF);
		}
	}
	f.write(b);
	f.close();
}

#endif // HAL_NVRAM

#if HAL_EXTMEM

HAL_extmem_file_t
HAL_extmem_openfile(const char fname[13])
{
	size_t i = 0;
	for (; i < HAL_EXTMEM_NUM_FILES; ++i) {
		if (!extmem_files[i])
			break;
	}

	if (i == HAL_EXTMEM_NUM_FILES) {
		Serial.println("Exceed number of opened fils");
		return 0;
	}

#if HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SPIFFS

	char fname_[14];
	fname_[0] = '/';
	strncpy(fname_ + 1, fname, 12);
	fname_[13] = '\0';

	extmem_files[i] = SPIFFS.open(fname_, "r+");
	if (!extmem_files[i]) {
		extmem_files[i] = SPIFFS.open(fname_, "w");
		if (!extmem_files[i]) {
			Serial.println("SPIFFS.open");
			return 0;
		}
	}
#elif HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SD

	extmem_files[i] = SD.open(fname, "r+");
	if (!extmem_files[i]) {
		extmem_files[i] = SD.open(fname, "w+");
		if (!extmem_files[i])
			return 0;
	}
#endif // HAL_ESP8266_EXTEM

	return i + 1;
}

void
HAL_extmem_closefile(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| (!extmem_files[file - 1]))
		return;

	extmem_files[file - 1].close();
}

uint32_t
_seek(HAL_extmem_file_t file, uint32_t pos, SeekMode whence)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| (!extmem_files[file - 1]))
		return 0;

	extmem_files[file - 1].seek(pos, whence);
	return pos;
}

void
HAL_extmem_setfileposition(
   HAL_extmem_file_t file,
   HAL_extmem_fileposition_t pos)
{
	_seek(file, pos, SeekSet);
}

HAL_extmem_fileposition_t
HAL_extmem_getfilesize(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| (!extmem_files[file - 1]))
		return 0;

	return extmem_files[file - 1].size();
}

uint16_t
HAL_extmem_getnumfiles()
{
	uint16_t result = 0;
#if HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SPIFFS
	Dir d = SPIFFS.openDir("");
	if (!d.rewind())
		return 0;

	while (d.next())
		++result;
#elif HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SD
	File d = SD.open("/");
	if (!d.isDirectory())
		return 0;

	while (true) {
		File f = d.openNextFile();
		if (!f)
			break;
		++result;
		f.close();
	}
	d.close();
#endif // HAL_ESP8266_EXTEM
	return result;
}

void
HAL_extmem_getfilename(uint16_t num, char fname[13])
{
	fname[0] = '\0';
	
#if HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SPIFFS
	Dir d = SPIFFS.openDir("");
	if (!d.rewind())
		return;
	
	while (d.next()) {
		if (num == 0) {
			strncpy(fname, d.fileName().c_str() + 1, 12);
			fname[12] = '\0';
			break;
		}
		num--;
	}
#elif HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SD
	File d = SD.open("/");
	if (!d.isDirectory())
		return;
	
	while (true) {
		File f = d.openNextFile();
		if (!f)
			break;
		if (num == 0) {
			strncpy(fname, f.name(), 12);
			fname[12] = '\0';
			f.close();
			break;
		}
		num--;
		f.close();
	}
	d.close();
#endif // HAL_ESP8266_EXTEM
}

void
HAL_extmem_deletefile(const char fname[13])
{
#if HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SPIFFS

	char fname_[14];
	fname_[0] = '/';
	strncpy(fname_ + 1, fname, 12);
	fname_[13] = '\0';

	if (!SPIFFS.remove(fname_))
		Serial.println("ERROR: SPIFFS.remove");
#elif HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SPIFFS
	if (!SD.remove(fname))
		Serial.println("ERROR: SD.remove");
#endif
}

uint8_t
HAL_extmem_readfromfile(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| !extmem_files[file - 1]) {
		Serial.println(" err");
		return 0;
	}

	const auto res = extmem_files[file - 1].read();
	return res;
}

void
HAL_extmem_writetofile(HAL_extmem_file_t file, uint8_t b)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| !extmem_files[file - 1]) {
		Serial.println(" err");
		return;
	}

	const auto res = extmem_files[file - 1].write(b);
}

HAL_extmem_fileposition_t
HAL_extmem_getfileposition(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| !extmem_files[file - 1]) {
		Serial.println(" err");
		return 0;
	}

	const auto res = extmem_files[file - 1].position();
	return res;
}

BOOLEAN
HAL_extmem_fileExists(const char fname[13])
{
#if HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SPIFFS

	char fname_[14];
	fname_[0] = '/';
	strncpy(fname_ + 1, fname, 12);
	fname_[13] = '\0';

	return SPIFFS.exists(fname_);
#elif HAL_ESP8266_EXTEM == HAL_ESP8266_EXTEM_SD
	return SD.exists(fname);
#endif
}

#endif // HAL_EXTMEM

__BEGIN_DECLS

void
HAL_update_concrete()
{
}

__END_DECLS

#endif // ARDUINO_ARCH_ESP8266
