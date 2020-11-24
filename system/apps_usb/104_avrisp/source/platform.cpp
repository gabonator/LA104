#include <library.h>
#include "platform.h"
#include "ringbuf.h"

RingBufCPP<uint8_t, 512> serialRead;
RingBufCPP<uint8_t, 512> serialWrite;

Serial SERIAL;

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

  int Serial::available()
  {
    return serialRead.size();
  }
  char Serial::read()
  {
    return serialRead.pull();
  }
  void Serial::print(char c)
  {    
    serialWrite.push(c);
  }
  void Serial::print(char *c)
  {
    while (*c)
      serialWrite.push(*c++);
  }
  void Serial::print(const char *c)
  {
    while (*c)
      serialWrite.push(*c++);
  }

extern Serial SERIAL;

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
