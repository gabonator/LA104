#include <library.h>
#include "platform.h"

void pinMode(int pin, int mode)
{
  BIOS::GPIO::PinMode((BIOS::GPIO::EPin)pin, (BIOS::GPIO::EMode)mode);
}

void digitalWrite(int pin, int level)
{
  BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)pin, level);
}

int digitalRead(int pin)
{
  return BIOS::GPIO::DigitalRead((BIOS::GPIO::EPin)pin);
}

void delay(int n)
{
  BIOS::SYS::DelayMs(n);
}

void delayMicroseconds(int us)
{
    us = us*12;
    while (us--)
    {
        __asm__("");
    }
}
