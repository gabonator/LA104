#include "Arduino.h"
#include <math.h>
#include <library.h>
#include "bitbangi2c.h"


namespace Arduino 
{
  bool transmissionOk = false;
  bool i2cinit = false;
  int reading = 0;
  CWire Wire;

  void delay(int d)
  {
    BIOS::SYS::DelayMs(d);
  }
#ifndef HARDI2C
    void CWire::begin()
    {
    }

    bool CWire::beginTransmission(uint8_t address)
    {
      transmissionOk = wire::beginTransmission(address);
      return transmissionOk ? 0 : 1;
    }

    void CWire::write(uint8_t data)
    {
      wire::write(data);
    }

    bool CWire::endTransmission()
    {
      return (wire::endTransmission() && transmissionOk) ? 0 : 1;
    }  

    bool CWire::requestFrom(uint8_t address, int count)
    {
      return wire::requestFrom(address, count);
    }

    uint8_t CWire::read()
    {  
      return wire::read();
    }


#else
    void CWire::begin()
    {
      if (i2cinit)
        return;
      i2cinit = true;

      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
    }

    void CWire::beginTransmission(uint8_t address)
    {
      transmissonOk = BIOS::GPIO::I2C::BeginTransmission(address);
    }

    void CWire::write(uint8_t data)
    {
      BIOS::GPIO::I2C::Write(data);
    }

    bool CWire::endTransmission()
    {
//      if (!transmissonOk)
//        return 1;
      return BIOS::GPIO::I2C::EndTransmission() ? 0 : 1;
    }  

    void CWire::requestFrom(uint8_t address, int count)
    {
      reading = count;
      BIOS::GPIO::I2C::RequestFrom(address, count);
    }

    uint8_t CWire::read()
    {  
      uint8_t aux = BIOS::GPIO::I2C::Read();
      if (--reading == 0)
        BIOS::GPIO::I2C::EndTransmission();
      return aux;
    }
#endif
  int atoi(char* p)
  {
    int k = 0;
    while (*p) {
        if (*p < '0' || *p > '9')
          break;
        k = k * 10 + (*p) - '0';
        p++;
     }
     return k;
  }
};

using namespace Arduino;
