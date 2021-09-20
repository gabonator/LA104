#include <library.h>
#include "platform.h"

using namespace BIOS;
uint32_t* gpioStatus = nullptr;

void check(const char* m)
{
  if (!gpioStatus)
  {
//    uint32_t *gpioI2cSpeed = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioI2cSpeed);
//    if (gpioI2cSpeed)
//      *gpioI2cSpeed = 400000;

    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
  }

  if (*gpioStatus != 0)
  {
    BIOS::DBG::Print(" i2c error:%d %s", *gpioStatus, m);
    BIOS::SYS::DelayMs(1000);
    *gpioStatus = 0;
  } else {
    BIOS::DBG::Print("%s", m);
  }
}

//void check(const char*) {}
void checkx(int) {}

void TwoWire::begin() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode(GPIO::EMode::Input | GPIO::EMode::PullUp));
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode(GPIO::EMode::Input | GPIO::EMode::PullUp));
  BIOS::SYS::DelayMs(100);

  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::I2c);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::I2c);
//  check("[[begin]]");
  BIOS::SYS::DelayMs(100);
}

void TwoWire::end() 
{
//  check("end");
//  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
//  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Input);
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode(GPIO::EMode::Input | GPIO::EMode::PullUp));
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode(GPIO::EMode::Input | GPIO::EMode::PullUp));
}

bool TwoWire::beginTransmission(int addr)  
{
  _ASSERT(0);
  check("B{");
  bool wireOk = GPIO::I2C::BeginTransmission(addr);
  checkx(wireOk);
  check("}r");
  return wireOk;
}

int TwoWire::read() 
{
  _ASSERT(0);
  check("R{");
  int a = GPIO::I2C::Read();
  check("}r");
  return a;
}

void TwoWire::write(int d)
{
  _ASSERT(0);
  check("W{");
  GPIO::I2C::Write(d);
  check("}w");
}

bool TwoWire::endTransmission(bool stop) 
{
  _ASSERT(0);
  check("E{");
  GPIO::I2C::EndTransmission(stop);    
  check("}e");
  return true;
}

void TwoWire::requestFrom(int addr, int count)
{
  _ASSERT(0);
  check("X{");
  GPIO::I2C::RequestFrom(addr, count);    
  check("}x");
}

void delay(int d)
{
  BIOS::SYS::DelayMs(d);
}

TwoWire Wire;
Adafruit_I2CDevice commoni2c;

long millis()
{
  return BIOS::SYS::GetTick();
}

bool Adafruit_I2CDevice::read(uint8_t* buf, int len) 
{
//  BIOS::DBG::Print("<Radr=%02x> ", address);
//  check("Read{");
  if (!GPIO::I2C::RequestFrom(address, len))
    return false;
//  check("a,");
  for (int i=0; i<len; i++)
  {
    buf[i] = GPIO::I2C::Read();
//    BIOS::DBG::Print("wr=%02x", buf[i]);
//    check(",");
  }
  if (!GPIO::I2C::EndTransmission(true))
    return false;
//  check("}");
  return true;
}

bool Adafruit_I2CDevice::write(uint8_t* buf, int len) 
{
//  BIOS::DBG::Print("<Wadr=%02x> ", address);
//  check("Write{");
  if (!GPIO::I2C::BeginTransmission(address))
  {
    BIOS::SYS::DelayMs(50);
//    BIOS::DBG::Print("BEGIN ERROR (adr=%02x)", address);
    return false;
  }
//  check("a,");
  for (int i=0; i<len; i++)
  {
    GPIO::I2C::Write(buf[i]);
//    BIOS::DBG::Print("wr=%02x", buf[i]);
//    check(",");
  }
  if (!GPIO::I2C::EndTransmission(true))
  {
//    BIOS::DBG::Print("END ERROR");
    return false;
  }
//  check("WRITE OK}");
  return true;
}
