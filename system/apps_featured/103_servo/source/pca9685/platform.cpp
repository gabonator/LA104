#include <library.h>
#include "platform.h"

using namespace BIOS;
uint32_t* gpioStatus = nullptr;

void check(const char* m)
{
  if (!gpioStatus)
  {
    uint32_t *gpioI2cSpeed = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioI2cSpeed);
    if (gpioI2cSpeed)
      *gpioI2cSpeed = 400000;

    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
  }

  if (*gpioStatus != 0)
  {
    BIOS::DBG::Print(" i2c error:%d %s", *gpioStatus, m);
    BIOS::SYS::DelayMs(1000);
    *gpioStatus = 0;
  } else {
//    BIOS::DBG::Print("%s", m);
  }
}

//void check(const char*) {}
void checkx(int) {}

void TwoWire::begin() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::I2c);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::I2c);
}

void TwoWire::end() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Input);
}

bool TwoWire::beginTransmission(int addr)  
{
  check("B{");
  bool wireOk = GPIO::I2C::BeginTransmission(addr);
  checkx(wireOk);
  check("}r");
  return wireOk;
}

int TwoWire::read() 
{
  check("R{");
  int a = GPIO::I2C::Read();
  check("}r");
  return a;
}

void TwoWire::write(int d)
{
  check("W{");
  GPIO::I2C::Write(d);
  check("}w");
}

bool TwoWire::endTransmission(bool stop) 
{
  check("E{");
  GPIO::I2C::EndTransmission(stop);    
  check("}e");
  return true;
}

void TwoWire::requestFrom(int addr, int count)
{
  check("X{");
  GPIO::I2C::RequestFrom(addr, count);    
  check("}x");
}

void delay(int d)
{
  BIOS::SYS::DelayMs(d);
}

TwoWire Wire;
