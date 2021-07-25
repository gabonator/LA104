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

#include "HAL.h"

#if HAL_NVRAM

void
HAL_nvram_write_buf(HAL_nvram_address_t address, const void* buf, uint32_t size)
{
	const uint8_t* bp = (const uint8_t*)buf;
	HAL_nvram_address_t a;
	for (a=address; a<address+size; ++a)
		HAL_nvram_write(a, *(bp++));
}

void
HAL_nvram_read_buf(HAL_nvram_address_t address, void* buf, uint32_t size)
{
	uint8_t* bp = (uint8_t*)buf;
	HAL_nvram_address_t a;
	for (a=address; a<address+size; ++a)
		*(bp++) = HAL_nvram_read(a);
}

#endif /* HAL_NVRAM */

#if HAL_GFX

#if HAL_GFX_EXPCOLOR_SIMPL
void
HAL_gfx_pointc(uint16_t x, uint16_t y, HAL_gfx_color_t color)
{
	HAL_gfx_setColor(color);
	HAL_gfx_point(x, y);
}

void
HAL_gfx_linec(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, HAL_gfx_color_t color)
{
	HAL_gfx_setColor(color);
	HAL_gfx_line(x1, y1, x2, y2);
}

void
HAL_gfx_rectc(uint16_t x, uint16_t y, uint16_t w, uint16_t h, HAL_gfx_color_t color)
{
    	HAL_gfx_setColor(color);
	HAL_gfx_rect(x, y, w, h);
}

void
HAL_gfx_circlec(uint16_t x, uint16_t y, uint16_t r, HAL_gfx_color_t color)
{
	HAL_gfx_setColor(color);
	HAL_gfx_circle(x, y, r);
}
#endif /* HAL_GFX_EXPCOLOR_SIMPL */

void
HAL_gfx_setColors(HAL_gfx_color_t fgColor, HAL_gfx_color_t bgColor)
{
    HAL_gfx_setColor(fgColor);
    HAL_gfx_setBgColor(bgColor);
}
   
#endif /* HAL_GFX */
