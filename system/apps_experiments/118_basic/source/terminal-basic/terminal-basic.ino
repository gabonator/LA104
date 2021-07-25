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

#include "basic.hpp"
#include "HALProxyStream.hpp"
#include "basic_interpreter.hpp"

#include "arduino_logger.hpp"
#include "basic_program.hpp"
#ifdef ARDUINO
#include "seriallight.hpp"
#endif

#if CONF_MODULE_ARDUINOIO
#include "basic_arduinoio.hpp"
#endif

#if USEPS2USARTKB
#include "ps2uartstream.hpp"
#endif

#if CONF_USE_EXTMEMFS
#include "basic_extmemfs.hpp"
#endif

#if USEUTFT
#include "utft_stream.hpp"
#endif

#if USEMATH
#include "basic_math.hpp"
#endif

#if USELIQUIDCRYSTAL
#include "liquidcrystalprint.hpp"
#endif

#if USETVOUT
#include "tvoutprint.hpp"
#include "utility/Font6x8.h"
#include "utility/Font8x8.h"
#include "utility/font6x8_cp866.h"
#endif

#if USE_EXTEEPROM
#include "basic_exteeprom.hpp"
#endif
#if USE_WIRE
#include <Wire.h>
#endif

#if USE_GFX
#include "basic_gfx.hpp"
#endif

#if USE_SDL_ISTREAM
#include "sdlstream.hpp"
#endif

#if USE_SDL_OSTREAM
#include "sdlvt100print.hpp"
#include "HAL_sdl.h"
#endif

namespace BASIC
{

#if USE_SAVE_LOAD && (!HAL_NVRAM)
#error "USE_SAVE_LOAD option requires HAL to support HAL_NVRAM"
#endif

#if USE_GFX && !(HAL_GFX)
#error "USE_GFX option requires HAL to support HAL_GFX"
#endif

} // namespace BASIC

static BASIC::HALProxyStream halproxy1(0);

/**
 * Instantiating modules
 */

#if USEUTFT
static UTFT	utft(CTE32HR, 38, 39, 40, 41);
static UTFTTerminal utftPrint(utft);
#define OUTPUT_PRINT utftPrint
#elif USETVOUT
static uint8_t		tvOutBuf[TVoutEx::bufferSize(TVOUT_HORIZ, TVOUT_VERT)];
static TVoutEx		tvOut;
static TVoutPrint	tvoutPrint;
#elif USELIQUIDCRYSTAL
static LiquidCrystal lCrystal(LIQCR_RS, LIQCR_E, LIQCR_D0, LIQCR_D1, LIQCR_D2, LIQCR_D3);
static uint8_t lCrBuf[20*4];
static LiquidCrystalVt100 lsvt100(lCrystal, 20, 4, lCrBuf);
#define OUTPUT_PRINT lsvt100
#endif

#if CONF_USE_EXTMEMFS
BASIC::ExtmemFSModule sdfs;
#endif

#if USEMATH
static BASIC::Math mathBlock;
#endif

#if CONF_MODULE_ARDUINOIO
static BASIC::ArduinoIO arduinoIo;
#endif

#if USE_EXTEEPROM
static BASIC::ExtEEPROM extEeprom;
#endif

#if USEPS2USARTKB
static PS2UARTKeyboardStream ps2usartStream;
#endif

#if USE_GFX
static BASIC::GFXModule gfxModule;
#endif

#if USE_SDL_ISTREAM
static SDLStream sdlStream;
#define INPUT_STREAM sdlStream
#endif

#if USE_SDL_OSTREAM
static SDL2vt100Print sdlPrint(*hal_sdl_renderer);
#define OUTPUT_PRINT sdlPrint
#endif

#if BASIC_MULTITERMINAL
static BASIC::Interpreter basic(SERIAL_PORT_I, SERIAL_PORT_O, BASIC::SINGLE_PROGSIZE);
#ifdef HAVE_HWSERIAL1
static BASIC::Interpreter basic1(SERIAL_PORT1, SERIAL_PORT1, BASIC::SINGLE_PROGSIZE);
#endif
#ifdef HAVE_HWSERIAL2
static BASIC::Interpreter basic2(SERIAL_PORT2, SERIAL_PORT2, BASIC::SINGLE_PROGSIZE);
#endif
#ifdef HAVE_HWSERIAL3
static BASIC::Interpreter basic3(SERIAL_PORT3, SERIAL_PORT3, BASIC::SINGLE_PROGSIZE);
#endif
#else
#if (USEPS2USARTKB && USETVOUT)
static BASIC::Interpreter basic(ps2usartStream, tvoutPrint, BASIC::SINGLE_PROGSIZE);
#elif USEPS2USARTKB
static BASIC::Interpreter basic(ps2usartStream, SERIAL_PORT_O, BASIC::SINGLE_PROGSIZE);
#elif USETVOUT
#if USE_SDL_ISTREAM
static BASIC::Interpreter basic(sdlStream, tvoutPrint, BASIC::SINGLE_PROGSIZE);
#else
static BASIC::Interpreter basic(SERIAL_PORT_I, tvoutPrint, BASIC::SINGLE_PROGSIZE);
#endif // USE_SDL_ISTREAM
#else
static BASIC::Interpreter basic(INPUT_STREAM, OUTPUT_PRINT, BASIC::SINGLE_PROGSIZE);
#endif // USEUTFT
#endif // BASIC_MULTITERMINAL

void
setup()
{
	HAL_initialize();
	
#if USE_WIRE
	Wire.begin();
	Wire.setClock(400000);
#endif // USE_WIRE
#ifdef SERIAL_PORT_I
	SERIAL_PORT_I.begin(SERIAL_I_BR);
#endif
#ifdef SERIAL_PORT_O
#ifdef SERIAL_PORT_I
	if (&SERIAL_PORT_I != &SERIAL_PORT_O)
#endif // SERIAL_PORT_I
		SERIAL_PORT_O.begin(SERIAL_O_BR);
#endif // SERIAL_PORT_O
#if USEPS2USARTKB
	ps2usartStream.begin();
#endif
#if USETVOUT
	tvOut.selectFont(Font6x8_cp866);
	tvOut.begin(PAL, TVOUT_HORIZ, TVOUT_VERT, tvOutBuf);
#elif USEUTFT
	utftPrint.begin();
#elif USELIQUIDCRYSTAL
	lCrystal.begin(LIQCR_HORIZ, LIQCR_VERT);
	lCrystal.cursor();
	lCrystal.blink();
	lsvt100.clear();
#endif
#if USE_SDL_ISTREAM
	sdlStream.init();
#endif	
	
#if BASIC_MULTITERMINAL
#ifdef HAVE_HWSERIAL1
	SERIAL_PORT1.begin(115200);
#endif
#ifdef HAVE_HWSERIAL2
	SERIAL_PORT2.begin(115200);
#endif
#ifdef HAVE_HWSERIAL3
	SERIAL_PORT3.begin(115200);
#endif
#endif

	LOG_INIT(SERIAL_PORT);

	LOG_TRACE;

#if CONF_MODULE_ARDUINOIO
	basic.addModule(&arduinoIo);
#endif
	
#if USE_GFX
	basic.addModule(&gfxModule);
#endif
	
#if USEMATH
	basic.addModule(&mathBlock);
#if BASIC_MULTITERMINAL
#ifdef HAVE_HWSERIAL1
	basic1.addModule(&mathBlock);
#endif
#endif // BASIC_MULTITERMINAL
#endif // USEMATH

#if USE_EXTEEPROM
	basic.addModule(&extEeprom);
#endif
	
#if CONF_USE_EXTMEMFS
	basic.addModule(&sdfs);
#endif
	
	basic.init();
#if BASIC_MULTITERMINAL
#ifdef HAVE_HWSERIAL1
	basic1.init();
#endif
#ifdef HAVE_HWSERIAL2
	basic2.init();
#endif
#ifdef HAVE_HWSERIAL3
	basic3.init();
#endif
#endif // BASIC_MULTITERMINAL
}

void
loop()
{
	LOG_TRACE;
	
	HAL_update();
	
	basic.step();
#if BASIC_MULTITERMINAL
#ifdef HAVE_HWSERIAL1
	basic1.step();
#endif
#ifdef HAVE_HWSERIAL2
	basic2.step();
#endif
#ifdef HAVE_HWSERIAL3
	basic3.step();
#endif
#endif // BASIC_MULTITERMINAL
}
