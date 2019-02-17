#include <stdint.h>
#include "source/usb/Disk.h"
#include "lib/STM32_USB-FS-Device_Driver/inc/usb_core.h"
#include "lib/STM32_USB-FS-Device_Driver/inc/usb_init.h"
#include "lib/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "source/bios/imports.h"
#include "system_stm32f10x.h"
#include <stdbool.h>

//#define _DEBUG_SP

volatile uint32_t gCounter = 0;
volatile uint32_t Dly_mS = 0;
volatile uint32_t gBeepCounter = 0;

#ifdef _DEBUG_SP
uint32_t GetStackPointer();
#endif

volatile char lastChar = 0;
char GetLastChar()
{
  char t = lastChar;
  lastChar = 0;
  return t;
}

char PeekLastChar()
{
  return lastChar;
}

void SysTickHandler(void)
{
#ifdef _DEBUG_SP
  static uint32_t dcounter = 0;
  if (dcounter++ == 500)
  {
    dcounter = 0;
    dbgPrint("$<%08x>", GetStackPointer());
  } 
#endif

  gCounter++;
  if (Dly_mS)
    Dly_mS--;

  if (gBeepCounter)
  {
    if (!--gBeepCounter)
    {
      xBeep(false);
    }
  }

  // keyboard
  uint16_t keysReg = GetKeys();

  // yYxX 1111  1111 DCBA 
  uint8_t encX = (keysReg >> 12) & 3;
  uint8_t encY = (keysReg >> 14) & 3;
  uint8_t keys = keysReg & 0x0f;

  static uint8_t encXold = -1, encYold = -1, keysOld = -1;
  if (encXold == -1)
  {
    encXold = encX;
    encYold = encY;
    keysOld = keys;
  }
  static int8_t encTable[16] = {
//     [0b0001] = 1, // 11 -> 10
     [0b0111] = 1, // 10 -> 00
     [0b1100] = 1, // 00 -> 11
     [0b0011] = -1, // 10 -> 11
//     [0b1101] = -1, // 00 -> 10
     [0b0100] = -1, // 11 -> 00
  };

  if (encX != encXold)
  {
    int8_t diff = encTable[(encXold << 2) | encX];
    if (diff>0)
      lastChar = '+'; 
    else if (diff<0)
      lastChar = '-'; 
    encXold = encX;
  }

  if (encY != encYold)
  {
    int8_t diff = encTable[(encYold << 2) | encY];
    if (diff>0)
      lastChar = '>'; 
    else if (diff<0)
      lastChar = '<'; 
    encYold = encY;
  }

  if (keysOld != keys)
  {
    if (keys & 0b0001)
      lastChar = '1';
    if (keys & 0b0010)
      lastChar = '2';
    if (keys & 0b0100)
      lastChar = '3';
    if (keys & 0b1000)
      lastChar = '4';
    keysOld = keys;
  }
}

extern void UartPushByte(uint8_t data);

void USART3_IRQHandler(void)
{
  if ((USART3->SR & USART_FLAG_RXNE) != (u16)RESET)
  {
    UartPushByte(USART_ReceiveData(USART3));
  }
}

extern void USB_Istr(void);

void USB_LP_CAN_RX0_IRQHandler(void)
{
    USB_Istr();
}
