#include "Arduino.h"
#include <math.h>
#include <stdint.h>

typedef bool boolean;
#define F(a) a

namespace Arduino 
{
  void delay(int d)
  {
    BIOS::SYS::DelayMs(d);
  }

  void delayMicroseconds(int us)
  {
    us = us*12;
    while (us--)
    {
      __asm__("");
    }
  }

  uint32_t millis()
  {
    return BIOS::SYS::GetTick();
  }

  void attachInterrupt(int, callback f, int) {}
  void detachInterrupt(int) {}

  void pinMode(int n, int d) 
  { 
    if (d==INPUT)
      BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)n, true); 
    BIOS::GPIO::PinMode((BIOS::GPIO::EPin)n, d==INPUT ? BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp) : BIOS::GPIO::Output); 
  }

  int digitalRead(int n) 
  {
    return BIOS::GPIO::DigitalRead((BIOS::GPIO::EPin)n);
  }

  void digitalWrite(int n, int v) 
  {
    BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)n, v);
  }

  CSerial Serial;

  void CSerial::println(const char* msg) { BIOS::DBG::Print("%s\n", msg);}
  void CSerial::println(int n) { BIOS::DBG::Print("%d\n", n);}
  void CSerial::println(int n, int) { BIOS::DBG::Print("%x\n", n);}

  void CSerial::begin(int baudrate)
  {
//    BIOS::GPIO::UART::Setup(9600, BIOS::GPIO::UART::length8);
//    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
//    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
  }

  bool CSerial::available()
  {
    return false; //BIOS::GPIO::UART::Available();
  }

  uint8_t CSerial::read()
  {
    return 0;
//    while (!BIOS::GPIO::UART::Available());
//    return BIOS::GPIO::UART::Read();
  }

  void CSerial::write(uint8_t* buffer, int len)
  {
    while (len--)
      BIOS::DBG::Print("%c", *buffer++);
//      BIOS::GPIO::UART::Write(*buffer++);
  }

  void CSerial::write(uint8_t c)
  {
      BIOS::DBG::Print("%c", c);
  }

  void CSerial::print(const char* msg)
  {
    BIOS::DBG::Print(msg);
  }

  void CSerial::print(int i)
  {
    BIOS::DBG::Print("%d", i);
  }

  void CSerial::print(int i, int)
  {
    BIOS::DBG::Print("%x", i);
  }

  void noInterrupts()
  {
    // TODO:
  }

  void interrupts()
  {
    // TODO:
  }

}