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

#if USE_GFX
#if USETVOUT

#include "TVoutEx.h"

namespace BASIC
{

void GFXModule::_init() {}

bool
GFXModule::command_box(Interpreter &i)
{
	INT x,y,w,h;
	
	if (getIntegerFromStack(i, h)) {
		if (getIntegerFromStack(i, w)) {
			if (getIntegerFromStack(i, y)) {
				if (getIntegerFromStack(i, x)) {
					TVoutEx::instance()->drawRect(x, y,
					    w, h);
					return true;
				}
			}
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
			TVoutEx::instance()->setCursorVisible(bool(v));
			return true;
		}
	}
	return false;
}

bool
GFXModule::command_circle(Interpreter &i)
{
	INT x,y,r;
	
	if (getIntegerFromStack(i, r)) {
		if (getIntegerFromStack(i, y)) {
			if (getIntegerFromStack(i, x)) {
				TVoutEx::instance()->drawCircle(x,y,r);
				return true;
			}
		}
	}
	return false;
}

bool
GFXModule::command_ellipse(Interpreter& i)
{
	INT x,y,w,h;
	
	if (getIntegerFromStack(i, h)) {
		if (getIntegerFromStack(i, w)) {
			if (getIntegerFromStack(i, y)) {
				if (getIntegerFromStack(i, x)) {
					TVoutEx::instance()->drawEllipse(x, y,
					    w, h);
					return true;
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_color(Interpreter &i)
{
	INT c, b;
	
	if (getIntegerFromStack(i, b)) {
		if (getIntegerFromStack(i, c)) {
			TVoutEx::instance()->setColor(Color_t(c), Color_t(b));
			return true;
		}
	}
	return false;
}

bool
GFXModule::command_line(Interpreter &i)
{
	INT x1,y1,x2,y2;
	
	if (getIntegerFromStack(i, y2)) {
		if (getIntegerFromStack(i, x2)) {
			if (getIntegerFromStack(i, y1)) {
				if (getIntegerFromStack(i, x1)) {
					TVoutEx::instance()->drawLine(x1, y1,
					    x2, y2);
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
	INT x1,y1;
	
	if (getIntegerFromStack(i, y1)) {
		if (getIntegerFromStack(i, x1)) {
			TVoutEx::instance()->drawLineTo(x1, y1);
			return true;
		}
	}
	
	return false;
}

bool
GFXModule::command_point(Interpreter &i)
{
	INT x,y;
	
	if (getIntegerFromStack(i, y)) {
		if (getIntegerFromStack(i, x)) {
			TVoutEx::instance()->setPixel(x, y);
			return true;
		}
	}
	return false;
}

#if GFX_EXP_COLOR
bool
GFXModule::command_boxc(Interpreter &i)
{
	INT x,y,w,h,c;
	
	if (getIntegerFromStack(i, c)) {
		if (getIntegerFromStack(i, h)) {
			if (getIntegerFromStack(i, w)) {
				if (getIntegerFromStack(i, y)) {
					if (getIntegerFromStack(i, x)) {
						TVoutEx::instance()->drawRect(x, y,
						    w, h, Color_t(c));
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_circlec(Interpreter &i)
{
	INT x,y,r,z;
	
	if (getIntegerFromStack(i, z)) {
		if (getIntegerFromStack(i, r)) {
			if (getIntegerFromStack(i, y)) {
				if (getIntegerFromStack(i, x)) {
					TVoutEx::instance()->drawCircle(x,y,r,Color_t(z));
					return true;
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_ellipsec(Interpreter& i)
{
	INT x,y,w,h,c;
	
	if (getIntegerFromStack(i, c)) {
		if (getIntegerFromStack(i, h)) {
			if (getIntegerFromStack(i, w)) {
				if (getIntegerFromStack(i, y)) {
					if (getIntegerFromStack(i, x)) {
						TVoutEx::instance()->drawEllipse(
						    x, y, w, h, Color_t(c));
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool
GFXModule::command_linec(Interpreter &i)
{
	INT x1,y1,x2,y2,z;
	
	if (getIntegerFromStack(i, z)) {
		if (getIntegerFromStack(i, y2)) {
			if (getIntegerFromStack(i, x2)) {
				if (getIntegerFromStack(i, y1)) {
					if (getIntegerFromStack(i, x1)) {
						TVoutEx::instance()->drawLine(x1, y1,
						    x2, y2, Color_t(z));
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
	INT x,y,z;
	
	if (getIntegerFromStack(i, z)) {
		if (getIntegerFromStack(i, y)) {
			if (getIntegerFromStack(i, x)) {
				TVoutEx::instance()->setPixel(x, y, Color_t(z));
				return true;
			}
		}
	}
	return false;
}
#endif // GFX_EXP_COLOR

bool
GFXModule::command_screen(Interpreter &i)
{
	INT x;
	
	if (getIntegerFromStack(i, x)) {
		if (x == 1) {
			TVoutEx::instance()->clearScreen();
			TVoutEx::instance()->setCursorVisible(false);
		} else if (x == 0) {
			TVoutEx::instance()->clearScreen();
			TVoutEx::instance()->setCursorVisible(true);
		}
		return true;
	}
	return false;
}

} // namespace BASIC

#endif // USETVOUT
#endif // USE_GFX
