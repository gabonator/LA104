/**
 *  @filename   :   epd2in9b.cpp
 *  @brief      :   Implements for Dual-color e-paper library
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

#include <stdlib.h>
#include "epd.h"

const unsigned char init_data[] = {
  0x22,  0x11, 0x10, 0x00, 0x10, 0x00, 0x00, 0x11, 0x88, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6A, 0x9B,
  0x9B, 0x9B, 0x9B, 0x00, 0x00, 0x6A, 0x9B, 0x9B, 0x9B, 0x9B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x18, 0x04, 0x16, 0x01, 0x0A, 0x0A, 0x0A, 0x0A, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x08, 0x3C, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

Epd::Epd() {
  width = EPD_WIDTH;
  height = EPD_HEIGHT;
};

bool Epd::Init(void) {
  /* this calls the peripheral hardware interface, see epdif */
  if (!IfInit()) {
    return false;
  }
  /* EPD hardware init start */
  Reset();
  SendCommand(SW_RESET);

  if (!Ready())
    return false;

  SendCommand(ANALOG_BLOCK);
  SendData(0x54);
  SendCommand(DIGITAL_BLOCK);
  SendData(0x3b);
  SendCommand(DRIVER_OUTPUT);           // Set MUX as 296
  SendData(0x27);
  SendData(0x01);
  SendData(0x00);
  SendCommand(DUMMY_LINE_PERIOD);       // Set 100Hz
  SendData(0x35);                       // Set 130Hz
  SendCommand(GATE_LINE_WIDTH);         // Set 100Hz
  SendData(0x04);                       // Set 130Hz
  SendCommand(DATA_ENTRY_SEQUENCE);     // data enter mode
  SendData(0x03);
  SendCommand(SOURCE_DRIVING_VOLTAGE);  // set VSH,VSL value
  SendData(0x41);                       // 2D9  15v
  SendData(0xa8);                       // 2D9   5v
  SendData(0x32);                       // 2D9  -15v
  SendCommand(WRITE_VCOM_REGISTER);     // vcom
  SendData(0x68);                       // -2.6V
  SendCommand(BORDER_WAVEFORM);         // board
  SendData(0x33);                       //GS1-->GS1
  SendCommand(WRITE_LUT_REGISTER);      // board
  for(int i=0;i<70;i++){                // write LUT register with 29bytes instead of 30bytes 2D13
    SendData(init_data[i]);
  }
  /* EPD hardware init end */
  return true;
}

/**
 *  @brief: basic function for sending commands
 */
/*
void Epd::SendCommand(unsigned char command) {
  DigitalWrite(dc_pin, LOW);
  SpiTransfer(command);
}
*/
/**
 *  @brief: basic function for sending data
 */
/*
void Epd::SendData(unsigned char data) {
  DigitalWrite(dc_pin, HIGH);
  SpiTransfer(data);
}
*/
void Epd::SetXYWindow(unsigned char xs, unsigned char xe, unsigned int ys, unsigned int ye){
  SendCommand(RAM_X_START_END);    // set RAM x address start/end, in page 36
  SendData(xs);    // RAM x address start at 00h;
  SendData(xe);    // RAM x address end at 0fh(12+1)*8->104
  SendCommand(RAM_Y_START_END);   // set RAM y address start/end, in page 37
  SendData(ys);    // RAM y address start at 0;
  SendData(ys>>8);
  SendData(ye);    // RAM y address end at
  SendData(ye>>8);   // RAM y address end at
}

void Epd::SetXYCounter(unsigned char x, unsigned char y){
  SendCommand(RAM_X_COUNTER);    // set RAM x address count
  SendData(x);
  SendCommand(RAM_Y_COUNTER);   // set RAM y address count
  SendData(y);
  SendData(y>>8);
}

void Epd::SetPartialWindowAux(const unsigned char* buffer, int x, int y, int w, int l, int color){
  SetXYWindow(x>>3, ((x+w)>>3)-1, y, (y+l)-1);
  DelayMs(2);
  SetXYCounter(x>>3, y);
  DelayMs(2);
  switch(color){
    case COLOR_RED:
      SendCommand(DATA_START_TRANSMISSION_2); // Write RAM (red)
      break;
    case COLOR_BW:
      SendCommand(DATA_START_TRANSMISSION_1); // Write RAM (B/W)
    default:
      break;
  }

  if (buffer != NULL) {
    for(int i = 0; i < w  / 8 * l; i++) {
      SendData(buffer[i]);
    }
  } else {
    for(int i = 0; i < w  / 8 * l; i++) {
      switch(color){
        case COLOR_RED:
          SendData(0x00);
          break;
        case COLOR_BW:
        default:
          SendData(0xFF);
          break;
      }
    }
  }
  DelayMs(2);
}

/**
 *  @brief: transmit partial data to the SRAM
 */
void Epd::SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int x, int y, int w, int l) {
  SetPartialWindowAux(buffer_black, 0, 0, width, height, COLOR_BW);
  SetPartialWindowAux(buffer_red, 0, 0, width, height, COLOR_RED);
}

/**
 *  @brief: transmit partial data to the black part of SRAM
 */
void Epd::SetPartialWindowBlack(const unsigned char* buffer_black, int x, int y, int w, int l) {
  SetPartialWindowAux(buffer_black, x, y, w, l, COLOR_BW);
}

/**
 *  @brief: transmit partial data to the red part of SRAM
 */
void Epd::SetPartialWindowRed(const unsigned char* buffer_red, int x, int y, int w, int l) {
  SetPartialWindowAux(buffer_red, x, y, w, l, COLOR_RED);
}

/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
  SetPartialWindow(frame_buffer_black, frame_buffer_red, 0, 0, width, height);
  DisplayFrame();
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
  SetXYWindow(0, (width>>3)-1, 0, height-1);
  SetXYCounter(0, 0);
  SendCommand(DATA_START_TRANSMISSION_1); // Write RAM (B/W)
  for (int i = 0; i < width * height / 8; i++) {
    SendData(0xFF);
  }

  SendCommand(DATA_START_TRANSMISSION_2); // Write RAM (red)
  for (int i = 0; i < width * height / 8; i++) {
    SendData(0x00);
  }
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
  SendCommand(DISPLAY_UPDATE_SEQUENCE_CFG);
  SendData(0xC7);    //Load LUT from MCU(0x32), Display update
  SendCommand(DISPLAY_UPDATE_SEQUENCE_RUN);
  //WaitUntilIdle();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power.
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a
 *         check code, the command would be executed if check code = 0xA5.
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
  SendCommand(DEEP_SLEEP);
  SendData(0x01);
  return;
}

/* END OF FILE */
