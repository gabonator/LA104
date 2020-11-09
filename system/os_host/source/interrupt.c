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
uint32_t gKeyMask = 0;

char GetLastChar()
{
  char t = lastChar;
  lastChar = 0;
  return t;
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

  // TODO: Could be somewhere else!

  // keyboard
  uint32_t keyMask = GetKeys();

  if (gKeyMask != keyMask)
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

    gKeyMask = keyMask;
  }
}

#if defined(DS203) || defined(DS203HD)
#define KEY_IF_RST 19
//extern void __Set(uint8_t Object, uint32_t Value);
extern void __Set(int x, int y);

void TIM3_IRQHandler(void)
{
  __Set(KEY_IF_RST, 0);

  SysTickHandler();
}
#endif

extern void UartPushByte(uint8_t data);
extern void UartPushError(uint16_t error);

void USART3_IRQHandler(void)
{
#ifdef LA104
  if (USART3->SR & (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
  {
    UartPushError(USART3->SR);
  }
  if (USART3->SR & USART_FLAG_RXNE)
  {
    UartPushByte(USART_ReceiveData(USART3));
  }

#endif
}

// TODO: move to platform dependent code
#if defined(DISABLE_USB) || defined(DS203) || defined(DS203HD)

extern void __USB_Istr(void);
extern void __CTR_HP(void);

void USB_HP_CAN_TX_IRQHandler(void)
{
  __CTR_HP();
}

void USB_LP_CAN_RX0_IRQHandler(void)
{
  __USB_Istr();
}

#else

extern void USB_Istr();

void USB_LP_CAN_RX0_IRQHandler(void)
{
    USB_Istr();
}
#endif