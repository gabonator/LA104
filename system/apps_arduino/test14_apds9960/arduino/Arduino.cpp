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

  uint32_t millis()
  {
    return BIOS::SYS::GetTick();
  }

  void attachInterrupt(int, callback f, int) {}
  void detachInterrupt(int) {}
  void pinMode(int, int) {}

    void CWire::begin()
    {
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
    }

    void CWire::beginTransmission(uint8_t address)
    {
      BIOS::GPIO::I2C::BeginTransmission(address);
    }

    void CWire::write(uint8_t data)
    {
      BIOS::GPIO::I2C::Write(data);
    }

    int CWire::endTransmission()
    {
      BIOS::GPIO::I2C::EndTransmission();
      return 0;
    }  

    int requestCount = 0;
    void CWire::requestFrom(uint8_t address, int count)
    {
      BIOS::GPIO::I2C::RequestFrom(address, count);
      requestCount = count;
    }

    uint8_t CWire::read()
    {  
      uint8_t t = BIOS::GPIO::I2C::Read();

      if (--requestCount == 0)
        BIOS::GPIO::I2C::EndTransmission();

      return t;
    }

    bool CWire::available()
    {  
      return requestCount > 0;
    }

  CSerial Serial;
  CWire Wire;


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

  void CSerial::print(const char* msg)
  {
    BIOS::DBG::Print(msg);
  }

  void CSerial::print(int i)
  {
    BIOS::DBG::Print("%d", i);
  }
}