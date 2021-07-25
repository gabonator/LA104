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

#ifdef ARDUINO_ARCH_ESP32

#include "HAL_esp32.h"

#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD
#include <SD.h>
#endif

#if HAL_NVRAM || HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS
#include "SPIFFS.h"
#endif

#if HAL_NVRAM
static File f;
#endif

#if HAL_EXTMEM

static File extmem_files[HAL_EXTMEM_NUM_FILES];

#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS
static FS& gfs = SPIFFS;
#elif HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD
static FS& gfs = SD;
#endif

#endif // HAL_EXTMEM

#if HAL_BUZZER
#define BUZZER_CHANNEL 0
#endif

__BEGIN_DECLS

void
HAL_initialize_concrete()
{
#if HAL_NVRAM || (HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS)
	if (!SPIFFS.begin(true)) {
		Serial.println("ERROR: SPIFFS.begin");
		exit(1);
	}
#endif

	/*Serial.println("Format? [y/n]");
	while (true) {
		while (Serial.available() < 1) delay(100);
		auto r = Serial.read();
		if (r == 'y') {
			if (!SPIFFS.format()) {
				Serial.println("ERROR: SPIFFS.format");
				exit(2);
			}
			break;
		} else if (r == 'n')
			break;
	}*/

#if HAL_NVRAM

	if (SPIFFS.exists("/nvram.bin"))
		f = SPIFFS.open("/nvram.bin", "r+");
	else
		f = SPIFFS.open("/nvram.bin", "w");
	if (!f) {
		Serial.println("ERROR: SPIFFS.open");
		exit(4);
	}
	f.close();

#endif // HAL_NVRAM

#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD
	if (!SD.begin(SS)) {
		Serial.println("ERROR: SD.begin");
		exit(3);
	}
#endif // HAL_ESP32_EXTMEM
}

__END_DECLS

void
HAL_finalize()
{
#if HAL_NVRAM || (HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS)
	SPIFFS.end();
#endif
#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD
	SD.end();
#endif // HAL_ESP32_EXTMEM
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
	if (!f) {
		Serial.println("ERROR: SPIFFS.open");
		exit(2);
	}
	if (!f.seek(uint32_t(addr))) {
		Serial.println("ERROR: SPIFFS.open");
		exit(3);
	}
	uint8_t r = f.read();
	f.close();
	return r;
}

void
HAL_nvram_write(HAL_nvram_address_t addr, uint8_t b)
{
	f = SPIFFS.open("/nvram.bin", "r+");
	if (!f) {
		Serial.println("ERROR: SPIFFS.open");
		exit(5);
	}

	if (f.size() > uint32_t(addr)) {
		if (!f.seek(uint32_t(addr))) {
			Serial.println("ERROR: f.seek(addr)");
			exit(6);
		}
	} else {
		if (!f.seek(f.size())) {
			Serial.println("ERROR: f.seek(f.size())");
		}
		while (f.size() < uint32_t(addr)) {
			f.write(0xFF);
			f.flush();
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
		Serial.println("ERROR: Maximum opened files reached");
		return 0;
	}

	char fname_[14];
	fname_[0] = '/';
	strncpy(fname_ + 1, fname, 12);
	fname_[13] = '\0';

#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS

	if (SPIFFS.exists(fname_))
		extmem_files[i] = SPIFFS.open(fname_, "r+");
	else
		extmem_files[i] = SPIFFS.open(fname_, "w");
	if (!extmem_files[i]) {
		Serial.println("ERROR: SPIFFS.open");
		return 0;
	}
#elif HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD

	extmem_files[i] = SD.open(fname_, "r+");
	if (!extmem_files[i]) {
		extmem_files[i] = SD.open(fname_, "w");
		if (!extmem_files[i]) {
			Serial.println("ERROR: SD.open");
			return 0;
		}
	}
#endif // HAL_ESP32_EXTMEM

	return i + 1;
}

void
HAL_extmem_closefile(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| (!extmem_files[file - 1])) {
		Serial.println("ERROR: HAL_extmem_closefile");
		return;
	}

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
	File d = gfs.open("/");
	if (!d || !d.isDirectory())
		return 0;
#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS

	d.rewindDirectory();
	File f = d.openNextFile();
	while (f) {
		++result;
		f = d.openNextFile();
	}
#elif HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD

	while (true) {
		File f = d.openNextFile();
		if (!f)
			break;
		++result;
		f.close();
	}
	d.close();
#endif // HAL_ESP32_EXTMEM

	return result;
}

void
HAL_extmem_getfilename(uint16_t num, char fname[13])
{
	fname[0] = '\0';
	File d = gfs.open("/");
	if (!d || !d.isDirectory())
		return;

#if HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SPIFFS

	d.rewindDirectory();
	File f = d.openNextFile();
	while (f) {
		if (num == 0) {
			strncpy(fname, f.name() + 1, 12);
			break;
		}
		f = d.openNextFile();
		num--;
	}

#elif HAL_ESP32_EXTMEM == HAL_ESP32_EXTEM_SD

	while (true) {
		File f = d.openNextFile();
		if (!f)
			break;
		if (num == 0) {
			strncpy(fname, f.name() + 1, 12);
			f.close();
			break;
		}
		num--;
		f.close();
	}

#endif // HAL_ESP32_EXTMEM
	d.close();
	fname[12] = '\0';
}

void
HAL_extmem_deletefile(const char fname[13])
{
	char fname_[14];
	fname_[0] = '/';
	strncpy(fname_ + 1, fname, 12);
	fname_[13] = '\0';

	if (!gfs.remove(fname_))
		Serial.println("ERROR: FS.remove");
}

uint8_t
HAL_extmem_readfromfile(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| !extmem_files[file - 1])
		return 0;

	return extmem_files[file - 1].read();
}

void
HAL_extmem_writetofile(HAL_extmem_file_t file, uint8_t b)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| !extmem_files[file - 1])
		return;

	extmem_files[file - 1].write(b);
}

HAL_extmem_fileposition_t
HAL_extmem_getfileposition(HAL_extmem_file_t file)
{
	if ((file == 0)
	|| (file > HAL_EXTMEM_NUM_FILES)
	|| !extmem_files[file - 1])
		return 0;

	return extmem_files[file - 1].position();
}

BOOLEAN
HAL_extmem_fileExists(const char fname[13])
{
	char fname_[14];
	fname_[0] = '/';
	strncpy(fname_ + 1, fname, 12);
	fname_[13] = '\0';

	return gfs.exists(fname_);
}

#endif // HAL_EXTMEM

#if HAL_BUZZER

#if HAL_BUZZER_ESP32 == HAL_BUZZER_ESP32_PWM

void
HAL_buzzer_tone(uint8_t channel, uint16_t freq, uint16_t dur)
{
	ledcAttachPin(channel, BUZZER_CHANNEL);
	ledcWriteTone(BUZZER_CHANNEL, freq);
	if (dur > 0) {
		delay(dur);
		HAL_buzzer_notone(channel);
  }
}

void
HAL_buzzer_notone(uint8_t channel)
{
	ledcWrite(BUZZER_CHANNEL, 0);
	ledcDetachPin(channel);
	pinMode(channel, OUTPUT);
	digitalWrite(channel, HIGH);
}

#endif // HAL_BUZZER_ESP32

#endif // HAL_BUZZER

__BEGIN_DECLS

void
HAL_update_concrete()
{
}

__END_DECLS

void analogWrite(uint8_t, uint8_t) {}

#endif // ARDUINO_ARCH_ESP32
