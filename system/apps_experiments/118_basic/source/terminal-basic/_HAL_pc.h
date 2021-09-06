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
 * @file _HAL_pc.h
 * @brief private header of HAL PC implementations (POSIX/Win32)
 */

#ifndef _HAL_PC_H
#define _HAL_PC_H

#include "HAL.h"

#if HAL_NVRAM
extern int nvram_file;
#endif

#if HAL_EXTMEM
extern int extmem_files[HAL_EXTMEM_NUM_FILES];

extern char ext_root[256];
#endif /* HAL_EXTMEM */

#endif /* _HAL_PC_H */
