#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" 
{
#endif

  void Set_Posi(uint_fast16_t x, uint_fast16_t y);
  void Set_Pixel(uint_fast16_t Color);
  uint16_t Get_Pixel();
  void ExtFlash_CS_LOW(void);
  void ExtFlash_CS_HIGH(void);
  void Set_Block(int x1, int y1, int x2, int y2);
  void xBeep(bool on);
  char GetLastChar();
  char PeekLastChar();
  void Set_Area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void EnableUsb(bool enable);
  void HardwareInit();
  uint32_t GetKeys();

  uint32_t FPGA32(uint8_t Cmd, uint16_t Cnt, uint32_t Data);
  uint16_t FPGA16(uint8_t Cmd, uint16_t Cnt, uint16_t Data);
#ifdef __cplusplus
}
#endif