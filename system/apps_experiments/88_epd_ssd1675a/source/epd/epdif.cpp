/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <library.h>
#include "epdif.h"
#include "../spi/spi.h"

CSpi spi;

EpdIf::EpdIf() {
};

EpdIf::~EpdIf() {
};

void EpdIf::DelayMs(unsigned int delaytime) {
  BIOS::SYS::DelayMs(delaytime);
}

int EpdIf::IfInit(void) {
  spi.begin();
  return 0;
}

void EpdIf::SendCommand(unsigned char command)
{
  spi.select();
  spi.transfer9(command);
  spi.deselect();
}

void EpdIf::SendData(unsigned char data)
{
  spi.select();
  spi.transfer9(data | 0x100);
  spi.deselect();
}

void EpdIf::WaitUntilIdle(void)
{
  spi.wait();
}

void EpdIf::Reset(void)
{
  spi.reset();
}
