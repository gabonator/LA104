#include <stm32f10x_gpio.h>
#include <library.h>
#include "gpio.h"

using namespace BIOS;

// Prevent moving into BSS section
__attribute__((__section__(".persistent")))
static OS::TInterruptHandler pOldSysTick;
__attribute__((__section__(".persistent")))
int nCounter;

void UartWrite(const char* text)
{
  while (*text)
    BIOS::GPIO::UART::Write(*text++);
}

void Init()
{
  nCounter = 0;
  BIOS::GPIO::UART::Setup(9600, BIOS::GPIO::UART::length8);
  // P1
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
  // P2
  SetGpioState<GPIOB_BASE, 11, GPIO_Speed_50MHz | GPIO_Mode_Out_PP>();
  // P3
  SetGpioState<GPIOB_BASE, 8, GPIO_Speed_50MHz | GPIO_Mode_Out_PP>();
  // P4
  SetGpioState<GPIOB_BASE, 9, GPIO_Speed_50MHz | GPIO_Mode_Out_PP>();
}

void SysTickHandler()
{
  // called every 1ms
  nCounter++;

  if (nCounter & 1)
    SetGpioLevel<GPIOB_BASE, 11, true>();
  else
    SetGpioLevel<GPIOB_BASE, 11, false>();

  if (nCounter & 2)
    SetGpioLevel<GPIOB_BASE, 8, true>();
  else
    SetGpioLevel<GPIOB_BASE, 8, false>();

  if (nCounter & 4)
    SetGpioLevel<GPIOB_BASE, 9, true>();
  else
    SetGpioLevel<GPIOB_BASE, 9, false>();

  if ((nCounter & 1023) == 0)
    UartWrite("Hello!\n"); // every 1024ms

  pOldSysTick();
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{   
  LCD::Clear(RGB565(404040));
  OS::TInterruptHandler pCurrentSysTick = OS::GetInterruptVector(OS::ISysTick);

  bool bLoaded = pCurrentSysTick == SysTickHandler;

  if (bLoaded)
  {
    DBG::Print("$Unloading demo generator. Reverting timer to %08x\n", pOldSysTick);
    OS::SetInterruptVector(OS::ISysTick, pOldSysTick);
  } else
  {
    Init();
    pOldSysTick = pCurrentSysTick;
    OS::SetInterruptVector(OS::ISysTick, SysTickHandler);
    DBG::Print("Demo generator loaded.\n"); 
  }

  SYS::DelayMs(1000);
  // TSR - Terminate and stay resident
  return 0;
}
