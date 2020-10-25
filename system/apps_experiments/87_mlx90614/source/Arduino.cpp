#include <library.h>
#include "Arduino.h"

using namespace BIOS;

void TwoWire::begin() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::I2c);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::I2c);
}

void TwoWire::beginTransmission(int addr)  
{
  GPIO::I2C::BeginTransmission(addr);
}

int TwoWire::read() 
{
  return GPIO::I2C::Read();
}

void TwoWire::write(int d)
{
  GPIO::I2C::Write(d);
}

void TwoWire::endTransmission(bool stop) 
{
  GPIO::I2C::EndTransmission(stop);    
}

void TwoWire::requestFrom(int addr, int count)
{
  GPIO::I2C::RequestFrom(addr, count);    
}

void delay(int d)
{
  BIOS::SYS::DelayMs(d);
}

TwoWire Wire;