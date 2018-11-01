#include "Arduino.h"
//#include <library.h>
#include "../host/source/bios/Bios.h"

void CSerial::println(const char* msg) {}
void CSerial::println(int) {}
void CSerial::println(int, int) {}

void delay(int ms)
{
  BIOS::SYS::DelayMs(ms);
}

uint32_t millis()
{
  return BIOS::SYS::GetTick();
}

  void CSerial::begin(int baudrate)
  {
    BIOS::GPIO::UART::Setup(9600, BIOS::GPIO::UART::length8);
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
  }

  bool CSerial::available()
  {
    return BIOS::GPIO::UART::Available();
  }

  uint8_t CSerial::read()
  {
    while (!BIOS::GPIO::UART::Available());
    return BIOS::GPIO::UART::Read();
  }

  void CSerial::write(uint8_t* buffer, int len)
  {
    while (len--)
      BIOS::GPIO::UART::Write(*buffer++);
  }

  void CSerial::print(const char* msg)
  {
    BIOS::DBG::Print(msg);
  }

CSerial Serial;
