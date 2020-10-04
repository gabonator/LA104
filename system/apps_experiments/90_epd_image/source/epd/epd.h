/**
 *  @filename   :   epd2in9b.h
 *  @brief      :   Header file for Dual-color e-paper library epd2in9b.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 31 2017
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

#ifndef EPD2IN9B_H
#define EPD2IN9B_H

#include "epdif.h"

// Display resolution
//#define EPD_WIDTH       104
//#define EPD_HEIGHT      212

#define EPD_WIDTH       128
#define EPD_HEIGHT      296

// commands
#define DRIVER_OUTPUT                               0x01
#define SOURCE_DRIVING_VOLTAGE                      0x04
#define DEEP_SLEEP                                  0x10
#define DATA_ENTRY_SEQUENCE                         0x11
#define SW_RESET                                    0x12
#define DISPLAY_UPDATE_SEQUENCE_RUN                 0x20
#define DISPLAY_UPDATE_SEQUENCE_CFG                 0x22
#define DATA_START_TRANSMISSION_1                   0x24
#define DATA_START_TRANSMISSION_2                   0x26
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define DUMMY_LINE_PERIOD                           0x3A
#define GATE_LINE_WIDTH                             0x3B
#define BORDER_WAVEFORM                             0x3C
#define RAM_X_START_END                             0x44
#define RAM_Y_START_END                             0x45
#define RAM_X_COUNTER                               0x4E
#define RAM_Y_COUNTER                               0x4F
#define ANALOG_BLOCK                                0x74
#define DIGITAL_BLOCK                               0x7E

#define COLOR_BW  0x01
#define COLOR_RED 0x02

class Epd : public EpdIf {
public:
  unsigned int width;
  unsigned int height;
               
  Epd();
  bool Init(void);
  void SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int x, int y, int w, int l);
  void SetPartialWindowBlack(const unsigned char* buffer_black, int x, int y, int w, int l);
  void SetPartialWindowRed(const unsigned char* buffer_red, int x, int y, int w, int l);
  void DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red);
  void DisplayFrame(void);
  void ClearFrame(void);
  void Sleep(void);

public:
  void SetXYWindow(unsigned char xs, unsigned char xe, unsigned int ys, unsigned int ye);
  void SetXYCounter(unsigned char x, unsigned char y);
  void SetPartialWindowAux(const unsigned char* buffer, int x, int y, int w, int l, int color);
};

#endif /* EPD2IN9B_H */

/* END OF FILE */
