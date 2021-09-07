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

#ifndef HAL_H
#define HAL_H

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

#include "sys/cdefs.h"
#include "tools.h"
#include "HAL_config.h"

__BEGIN_DECLS

/**
 * @brief Initialize hardware interface
 */
void HAL_initialize();

/**
 * @brief finalize hardware interface
 */
void HAL_finalize();

/**
 * @brief Regular update platform state
 */
void HAL_update();

/*** Non volatile RAM interface ***/

#if HAL_NVRAM

/**
 * @address
 */
typedef uint32_t HAL_nvram_address_t;

/**
 * @brief Get NVRAM size
 * @return 
 */
HAL_nvram_address_t HAL_nvram_getsize();

/**
 * @brief write byte to NVRAM
 * @param address [in]
 * @param dataByte [in]
 */
void HAL_nvram_write(HAL_nvram_address_t, uint8_t);

/**
 * @brief read byte from NVRAM
 * @param address
 * @return byte
 */
uint8_t HAL_nvram_read(HAL_nvram_address_t);

/**
 * 
 * @param address
 * @param buf
 * @param length
 */
void HAL_nvram_read_buf(HAL_nvram_address_t, void*, uint32_t);

/**
 * 
 * @param address
 * @param buf
 * @param length
 */
void HAL_nvram_write_buf(HAL_nvram_address_t, const void*, uint32_t);

#endif /* HAL_NVRAM */

/*** Terminal interface ***/

typedef uint8_t HAL_terminal_t;

/**
 * @param termno
 * @param byte
 */
void HAL_terminal_write(HAL_terminal_t, uint8_t);

/**
 * @param termno
 * @return byte
 */
uint8_t HAL_terminal_read(HAL_terminal_t);

/**
 * @parram termno
 * @return number of bytes, ready to read
 */
BOOLEAN HAL_terminal_isdataready(HAL_terminal_t);

/*** External memory interface ***/

#if HAL_EXTMEM

/**
 * File handle
 */
typedef uint8_t HAL_extmem_file_t;

/**
 * Position in file/file size
 */
typedef uint32_t HAL_extmem_fileposition_t;

/**
 * @brief Open file in external memory
 * @param path
 * @return file handle > 0 or 0 on error
 */
HAL_extmem_file_t HAL_extmem_openfile(const char[13]);

/**
 * @brief Delete file from external memory
 * @param path
 */
void HAL_extmem_deletefile(const char[13]);

/**
 * @brief close opened file
 * @param handle
 */
void HAL_extmem_closefile(HAL_extmem_file_t);

/**
 * @brief Read byte from file
 * @param handle
 * @return byte
 */
uint8_t HAL_extmem_readfromfile(HAL_extmem_file_t);

/**
 * @brief Write byte to file
 * @param handle
 * @param byte
 */
void HAL_extmem_writetofile(HAL_extmem_file_t, uint8_t);

/**
 * @brief 
 * @param handle
 * @return position
 */
HAL_extmem_fileposition_t HAL_extmem_getfileposition(HAL_extmem_file_t);

/**
 * @brief Set position in file
 * @param handle
 * @param position
 */
void HAL_extmem_setfileposition(HAL_extmem_file_t, HAL_extmem_fileposition_t);

/**
 * @brief handle
 * @param handle
 * @return file size
 */
HAL_extmem_fileposition_t HAL_extmem_getfilesize(HAL_extmem_file_t);

/**
 * @brief
 * @return 
 */
uint32_t HAL_extmem_getfreespace();

/**
 * @brief Get number of 
 * @return number of files
 */
uint16_t HAL_extmem_getnumfiles();

/**
 * @brief Get file name in a directory
 * @param num [in] entry number 
 * @param name [out]
 */
void HAL_extmem_getfilename(uint16_t, char[13]);

/**
 * @brief
 * @param fname 
 * @return 
 */
BOOLEAN HAL_extmem_fileExists(const char[13]);

#endif /* HAL_EXTMEM */

/*** Time interface ***/

/**
 * @brief Delay for aa interval
 * @param ms milliseconds [in]
 */
void HAL_time_sleep_ms(uint32_t);

/**
 * @brief get current time from system start
 * @return time in milliseconds
 */
uint32_t HAL_time_gettime_ms();

/*** Random number interface ***/
void HAL_random_seed(uint32_t);

uint32_t HAL_random_generate(uint32_t);

#if HAL_GFX

typedef enum {
	HAL_GFX_NOTACOLOR = 0,
	HAL_GFX_COLOR_WHITE,
	HAL_GFX_COLOR_BLACK,
	HAL_GFX_COLOR_RED,
	HAL_GFX_COLOR_GREEN,
	HAL_GFX_COLOR_BLUE,
	HAL_GFX_COLOR_CYAN,
	HAL_GFX_COLOR_MAGENTA,
	HAL_GFX_COLOR_YELLOW,
	HAL_GFX_COLOR_GRAY,
		
	HAL_GFX_NUMCOLORS
} HAL_gfx_color_t;

/**
 * @param fgcolor
 */
void HAL_gfx_setColor(HAL_gfx_color_t);


/**
 * @param bgcolor
 */
void HAL_gfx_setBgColor(HAL_gfx_color_t);

/**
 * @param fgColor
 * @param bgColor
 */
void HAL_gfx_setColors(HAL_gfx_color_t, HAL_gfx_color_t);

/**
 * @param x
 * @param y
 */
void HAL_gfx_point(uint16_t, uint16_t);

/**
 * @param x
 * @param y
 * @param color
 */
void HAL_gfx_pointc(uint16_t, uint16_t, HAL_gfx_color_t);

/**
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void HAL_gfx_line(uint16_t, uint16_t, uint16_t, uint16_t);

/**
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void HAL_gfx_linec(uint16_t, uint16_t, uint16_t, uint16_t, HAL_gfx_color_t);

/**
 * @param x
 * @param y
 * @param w
 * @param h
 */
void HAL_gfx_rect(uint16_t, uint16_t, uint16_t, uint16_t);

/**
 * @param x
 * @param y
 * @param w
 * @param h
 * @param color
 */
void HAL_gfx_rectc(uint16_t, uint16_t, uint16_t, uint16_t, HAL_gfx_color_t);

/**
 * @param x
 * @param y
 */
void HAL_gfx_lineto(uint16_t, uint16_t);

/**
 * @param x
 * @param y
 * @param color
 */
void HAL_gfx_linetoc(uint16_t, uint16_t, HAL_gfx_color_t);

/**
 * @param x
 * @param y
 * @param r
 */
void HAL_gfx_circle(uint16_t, uint16_t, uint16_t);

/**
 * @param x
 * @param y
 * @param r
 * @param color
 */
void HAL_gfx_circlec(uint16_t, uint16_t, uint16_t, HAL_gfx_color_t);

#endif /* HAL_GFX */

#if HAL_GPIO
/**
 * @brief write pin value
 */
void HAL_gpio_writePin(uint8_t, BOOLEAN);
/**
 * @brief read pin value
 */
BOOLEAN HAL_gpio_readPin(uint8_t);

#endif /* HAL_GPIO */

#if HAL_BUZZER
/**
 * @brief
 * 
 * @param ch channel
 * @param freq Frequency (HZ)
 * @param dur duration (ms) 0 - endless
 */
void HAL_buzzer_tone(uint8_t, uint16_t, uint16_t);

/**
 * 
 */
void HAL_buzzer_notone(uint8_t);

#endif /* HAL_BUZZER */

__END_DECLS

#endif /* HAL_H */
