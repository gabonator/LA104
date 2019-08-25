#include <stdint.h>
#include "source/usb/Disk.h"
#include "library/STM32_USB-FS-Device_Driver/inc/usb_core.h"
#include "library/STM32_USB-FS-Device_Driver/inc/usb_init.h"
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "source/bios/imports.h"
#include "system_stm32f10x.h"
#include <stdbool.h>

volatile uint32_t gCounter = 0;
volatile uint32_t Dly_mS = 0;
volatile uint32_t gBeepCounter = 0;

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
  uint32_t keyMask = GetKeys();

  static uint32_t keyMaskOld = 0;

  if (keyMaskOld != keyMask)
  {
    if (keyMask & KeyUp)
      lastChar = '>';
    if (keyMask & KeyDown)
      lastChar = '<';
    if (keyMask & KeyRight)
      lastChar = '+';
    if (keyMask & KeyLeft)
      lastChar = '-';
    if (keyMask & KeyF1)
      lastChar = '1';
    if (keyMask & KeyF2)
      lastChar = '2';
    if (keyMask & KeyF3)
      lastChar = '3';
    if (keyMask & KeyF4)
      lastChar = '4';

    keyMaskOld = keyMask;
  }
}

#ifdef DS203
#define KEY_IF_RST 19
//extern void __Set(uint8_t Object, uint32_t Value);
extern void ___Set(int x, int y);

void TIM3_IRQHandler(void)
{
  ___Set(KEY_IF_RST, 0);

  //__Set(KEY_IF_RST, 0);
  SysTickHandler();
}
#endif

extern void UartPushByte(uint8_t data);

void USART3_IRQHandler(void)
{
#ifdef LA104
  if ((USART3->SR & USART_FLAG_RXNE) != (u16)RESET)
  {
    UartPushByte(USART_ReceiveData(USART3));
  }
#endif
}

// TODO: move to platform dependent code
#ifdef DS203

extern void USB_Istr(void);
extern void __USB_Istr(void);
extern void __CTR_HP(void);

extern void ___Int_HP();
extern void ___Int_LP();

void USB_HP_CAN_TX_IRQHandler(void)
{
  ___Int_HP();
//  __CTR_HP();
}

void USB_LP_CAN_RX0_IRQHandler(void)
{
  ___Int_LP();
//  __USB_Istr();
}

#else

void USB_LP_CAN_RX0_IRQHandler(void)
{
    USB_Istr();
}
#endif