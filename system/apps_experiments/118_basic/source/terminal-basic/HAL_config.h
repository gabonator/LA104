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

#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

/*
 * Number of available terminals
 */
#define HAL_TERMINAL_NUM 1

/*
 * Enable NVRAM read/write functions
 * 
 * NVRAM provides a single storage space, preserved between software runs
 */
#define HAL_NVRAM 1

/*
 * Enable external memory interface
 * 
 * External memory is a simple filesystem model
 */
#define HAL_EXTMEM 0
#if HAL_EXTMEM
/*
 * Maximal number of simulteniously opened files
 */
#define HAL_EXTMEM_NUM_FILES 2
#endif /* HAL_EXTMEM */

/*
 * Enable GFX interface functions
 */
#define HAL_GFX 0
#if HAL_GFX
/* Default implementation of the explicit color setting commands */
#define HAL_GFX_EXPCOLOR_SIMPL 1
#endif /* HAL_GFX */

#define HAL_GPIO 1

#define HAL_BUZZER 0

#endif /* HAL_CONFIG_H */
