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

/**
 * @file HAL_pc.cpp
 * @brief Common part of POSIX/Linux terminal I/O HAL implementation
 */

#ifdef HAL_PC_TERMINAL

#include <sys/stat.h>

#include <unistd.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "_HAL_pc.h"
#include "HAL_pc.h"

#if HAL_NVRAM
int nvram_file = -1;
#endif

#if HAL_EXTMEM
int extmem_files[HAL_EXTMEM_NUM_FILES];
char ext_root[256];
#endif /* HAL_EXTMEM */

void
HAL_update()
{
}

#if HAL_EXTMEM

HAL_extmem_file_t
HAL_extmem_openfile(const char str[13])
{
	char fpath[256];
	strncpy(fpath, ext_root, 255);
	strncat(fpath, str, 255);
	
	int fp = open(fpath, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fp == -1) {
		perror("open");
		return 0;
	}
	
	for (size_t i=0; i<HAL_EXTMEM_NUM_FILES; ++i) {
		if (extmem_files[i] == -1) {
			extmem_files[i] = fp;
			return i+1;
		}
	}
	
	return 0;
}

void
HAL_extmem_deletefile(const char fname[13])
{
	char fpath[256];
	strncpy(fpath, ext_root, 255);
	strncat(fpath, fname, 255);
	
	unlink(fpath);
}

void
HAL_extmem_closefile(HAL_extmem_file_t file)
{
	if ((file == 0)
	 || (file > HAL_EXTMEM_NUM_FILES)
	 || (extmem_files[file-1] == -1))
		return;
	
	if (close(extmem_files[file-1]) != 0) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	extmem_files[file-1] = -1;
}

off_t
_seek(HAL_extmem_file_t file, off_t pos, int whence)
{
	if ((file == 0)
	 || (file > HAL_EXTMEM_NUM_FILES)
	 || (extmem_files[file-1] == -1))
		return 0;
	
	off_t res = lseek(extmem_files[file-1], pos, whence);
	if (res == -1)
		perror("lseek");
	
	return res;
}

HAL_extmem_fileposition_t
HAL_extmem_getfileposition(HAL_extmem_file_t file)
{
	return _seek(file, 0, SEEK_CUR);
}

void
HAL_extmem_setfileposition(HAL_extmem_file_t file,
    HAL_extmem_fileposition_t pos)
{
	_seek(file, pos, SEEK_SET);
}

HAL_extmem_fileposition_t
HAL_extmem_getfilesize(HAL_extmem_file_t file)
{
	off_t current = _seek(file, 0, SEEK_CUR);
	off_t result =  _seek(file, 0, SEEK_END);
	_seek(file, current, SEEK_SET);
	
	return result;
}

uint8_t
HAL_extmem_readfromfile(HAL_extmem_file_t file)
{
	if ((file == 0)
	 || (file > HAL_EXTMEM_NUM_FILES)
	 || (extmem_files[file-1] == -1))
		return 0;
	
	char result = '\0';
	if (read(extmem_files[file-1], &result, 1) != 1)
		fputs("HAL error \"read\": Can't read from file", stderr);
	
	return result;
}

void
HAL_extmem_writetofile(HAL_extmem_file_t file, uint8_t byte)
{
	if ((file == 0)
	 || (file > HAL_EXTMEM_NUM_FILES)
	 || (extmem_files[file-1] == -1))
		return;
	
	if (write(extmem_files[file-1], &byte, 1) != 1)
		fputs("HAL error \"write\": Can't write to file", stderr);
}

BOOLEAN
HAL_extmem_fileExists(const char fname[13])
{
	char fpath[256];
	strncpy(fpath, ext_root, 255);
	strncat(fpath, fname, 255);
	
	if (access(fpath, F_OK) == 0)
		return TRUE;
	return FALSE;
}

#endif /* HAL_EXTMEM */

#endif /* HAL_PC_TERMINAL */
