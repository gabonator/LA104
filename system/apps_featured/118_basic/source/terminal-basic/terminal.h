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

/**
 * @file sgen_terminal.h
 * @brief Simple text terminal interface
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <inttypes.h>

#include <sys/cdefs.h>

__BEGIN_DECLS

typedef uint16_t len_t;

extern uint8_t terminal_rows;
extern uint8_t terminal_cols;

/**
 * Cursor movement direction
 */
typedef enum
{
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
} direction_t;

/**
 * [Re]initialize terminal component
 */
void terminal_init();
/**
 * @brief Move cursor one position
 * @param direction
 */
void terminal_moveCursor(direction_t);
/**
 * @brief get current cursor position
 * @param line out
 * @param column out
 */
void terminal_cursorPosition(uint8_t*, uint8_t*);

char terminal_getc();

len_t terminal_putc(char);

/**
 * Terminal update callback (cursor blinking, etc...)
 */
void terminal_update();

__END_DECLS

#endif /* TERMINAL_H */
