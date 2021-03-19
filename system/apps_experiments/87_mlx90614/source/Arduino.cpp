#include <library.h>
#include "Arduino.h"

using namespace BIOS;

void check(const char*) {}
void TwoWire::begin() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Output);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Output);
  GPIO::DigitalWrite(GPIO::EPin::P1, 0);
  GPIO::DigitalWrite(GPIO::EPin::P2, 0);
  BIOS::SYS::DelayMs(200);
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::I2c);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::I2c);
}

void TwoWire::end() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Output);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Output);
  GPIO::DigitalWrite(GPIO::EPin::P1, 0);
  GPIO::DigitalWrite(GPIO::EPin::P2, 0);
  BIOS::SYS::DelayMs(200);
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Input);
}

bool TwoWire::beginTransmission(int addr)  
{
  check("B{");
  bool b = GPIO::I2C::BeginTransmission(addr);
  check("}");
  return b;
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

void TwoWire::endTransmission(bool stop) 
{
  check("E{");
  GPIO::I2C::EndTransmission(stop);    
  check("}");
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

TwoWire Wire;