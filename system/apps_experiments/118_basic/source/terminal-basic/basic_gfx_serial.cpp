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

#include "basic_gfx.hpp"

#if USE_GFX && SERIAL_GFX

#include "gfxterm.hpp"

namespace BASIC
{

void GFXModule::_init() {}

static void
write16(Interpreter &i, int16_t v)
{
    i.print(char(v>>8)); i.print(char(v&0xFF));
}

bool
GFXModule::command_box(Interpreter &i)
{
	INT h;
	if (getIntegerFromStack(i, h)) {
		INT w;
		if (getIntegerFromStack(i, w)) {
			INT y;
			if (getIntegerFromStack(i, y)) {
				INT x;
				if (getIntegerFromStack(i, x)) {
					HAL_gfx_rect(x,y,w,h);
					return true;
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_ellipse(Interpreter &i)
{
	INT h;
	if (getIntegerFromStack(i, h)) {
		INT w;
		if (getIntegerFromStack(i, w)) {
			INT y;
			if (getIntegerFromStack(i, y)) {
				INT x;
				if (getIntegerFromStack(i, x)) {
					i.print(char(ASCII::DLE));
					i.print(char(GFXTERM::Command::ELLIPSE));
					write16(i, x);
					write16(i, y);
					write16(i, w);
					write16(i, h);
					return true;
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_circle(Interpreter &i)
{
	INT r;
	if (getIntegerFromStack(i, r)) {
		INT y;
		if (getIntegerFromStack(i, y)) {
			INT x;
			if (getIntegerFromStack(i, x)) {
				HAL_gfx_circle(x,y,r);
				return true;
			}
		}
	}
	
	return false;
}

bool
GFXModule::command_color(Interpreter &i)
{
	INT b;
	if (getIntegerFromStack(i, b)) {
		INT c;
		if (getIntegerFromStack(i, c)) {
			HAL_gfx_setColors(HAL_gfx_color_t(c), HAL_gfx_color_t(b));
			return true;
		}
	}
	return false;
}

bool
GFXModule::command_cursor(Interpreter &i)
{
	Parser::Value v(false);
	if (i.popValue(v)) {
		if (v.type() == Parser::Value::LOGICAL) {
			i.print(char(ASCII::DLE));
			i.print(char(GFXTERM::Command::CURSOR));
			i.print(bool(v) ? char(1) : char(0));
			return true;
		}
	}
	return false;
}

bool
GFXModule::command_line(Interpreter &i)
{
	INT y2;
	if (getIntegerFromStack(i, y2)) {
		INT x2;
		if (getIntegerFromStack(i, x2)) {
			INT y1;
			if (getIntegerFromStack(i, y1)) {
				INT x1;
				if (getIntegerFromStack(i, x1)) {
					HAL_gfx_line(x1, y1, x2, y2);
					return true;
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_lineto(Interpreter &i)
{
	INT y;
	if (getIntegerFromStack(i, y)) {
		INT x;
		if (getIntegerFromStack(i, x)) {
			i.print(char(ASCII::DLE));
			i.print(char(GFXTERM::Command::LINETO));
			write16(i, x);
			write16(i, y);
			return true;
		}
	}
	
	return false;
}

bool
GFXModule::command_point(Interpreter &i)
{
	INT y;
	if (getIntegerFromStack(i, y)) {
		INT x;
		if (getIntegerFromStack(i, x)) {
			HAL_gfx_point(x, y);
			return true;
		}
	}
	return false;
}

bool
GFXModule::command_screen(Interpreter &i)
{
	INT x;
	if (getIntegerFromStack(i, x)) {
		i.print(char(ASCII::DLE));
		i.print(char(GFXTERM::Command::MODE));
		i.print(char(x));
		return true;
	}
	return false;
}

#if GFX_EXP_COLOR
bool
GFXModule::command_boxc(Interpreter &i)
{
	INT c;
	if (getIntegerFromStack(i, c)) {
		INT h;
		if (getIntegerFromStack(i, h)) {
			INT w;
			if (getIntegerFromStack(i, w)) {
				INT y;
				if (getIntegerFromStack(i, y)) {
					INT x;
					if (getIntegerFromStack(i, x)) {
						HAL_gfx_rectc(x,y,w,h, HAL_gfx_color_t(c));
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_ellipsec(Interpreter &i)
{
	INT c;
	if (getIntegerFromStack(i, c)) {
		INT h;
		if (getIntegerFromStack(i, h)) {
			INT w;
			if (getIntegerFromStack(i, w)) {
				INT y;
				if (getIntegerFromStack(i, y)) {
					INT x;
					if (getIntegerFromStack(i, x)) {
						i.print(char(ASCII::DLE));
						i.print(char(GFXTERM::Command::ELLIPSEC));
						write16(i, x);
						write16(i, y);
						write16(i, w);
						write16(i, h);
						i.print(char(c));
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_pointc(Interpreter &i)
{
	INT c;
	if (getIntegerFromStack(i, c)) {
		INT y;
		if (getIntegerFromStack(i, y)) {
			INT x;
			if (getIntegerFromStack(i, x)) {
				HAL_gfx_pointc(x, y, HAL_gfx_color_t(c));
				return true;
			}
		}
	}
	return false;
}

bool
GFXModule::command_circlec(Interpreter &i)
{
	INT c;
	if (getIntegerFromStack(i, c)) {
		INT r;
		if (getIntegerFromStack(i, r)) {
			INT y;
			if (getIntegerFromStack(i, y)) {
				INT x;
				if (getIntegerFromStack(i, x)) {
					HAL_gfx_circlec(x, y, r, HAL_gfx_color_t(c));
					return true;
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_linec(Interpreter &i)
{
	INT c;
	if (getIntegerFromStack(i, c)) {
		INT y2;
		if (getIntegerFromStack(i, y2)) {
			INT x2;
			if (getIntegerFromStack(i, x2)) {
				INT y1;
				if (getIntegerFromStack(i, y1)) {
					INT x1;
					if (getIntegerFromStack(i, x1)) {
						HAL_gfx_linec(x1, y1, x2, y2, HAL_gfx_color_t(c));
						return true;
					}
				}
			}
		}
	}
	return false;
}
#endif

} // namespace BASIC

#endif // USE_GFX && SERIAL_GFX
