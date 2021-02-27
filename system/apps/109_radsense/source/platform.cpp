#include <library.h>
#include "platform.h"

using namespace BIOS;

void check(const char*) {}
void checkx(int) {}

void TwoWire::begin() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::I2c);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::I2c);
}

void TwoWire::end() 
{
  BIOS::SYS::DelayMs(100);
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Input);
  BIOS::SYS::DelayMs(100);
}

bool wireOk = false;

bool TwoWire::beginTransmission(int addr)  
{
  check("B{");
  wireOk = GPIO::I2C::BeginTransmission(addr);
  checkx(wireOk);
  check("}");
  return wireOk;
}

int TwoWire::read() 
{
  check("R{");
  int a = GPIO::I2C::Read();
  check("}");
  return a;
}

void TwoWire::write(int d)
{
  check("W{");
  GPIO::I2C::Write(d);
  check("}");
}

bool TwoWire::endTransmission(bool stop) 
{
  check("E{");
  GPIO::I2C::EndTransmission(stop);    
  check("}");
  return wireOk;
}

void TwoWire::requestFrom(int addr, int count)
{
  check("X{");
  GPIO::I2C::RequestFrom(addr, count);    
  check("}");
}

void delay(int d)
{
  BIOS::SYS::DelayMs(d);
}

long millis()
{
  return BIOS::SYS::GetTick();
}

void pinMode(int pin, int mode)
{
  BIOS::GPIO::PinMode((BIOS::GPIO::EPin)pin, (BIOS::GPIO::EMode)mode);
}

void digitalWrite(int pin, int level)
{
  BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)pin, level);
}

TwoWire Wire;