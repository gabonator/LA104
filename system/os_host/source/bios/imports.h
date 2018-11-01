#pragma once

#include <stdint.h>
#include <stdbool.h>

extern "C" 
{
  void Set_Posi(uint16_t x, uint16_t y);
  void Set_Pixel(uint16_t Color);
  void ExtFlash_CS_LOW(void);
  void ExtFlash_CS_HIGH(void);
  void Set_Block(int x1, int y1, int x2, int y2);
  void Beep(bool on);
  char GetLastChar();
  void Set_Area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
}