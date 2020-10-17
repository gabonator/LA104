#include "platform.h"

SPISettings::SPISettings(int a, int b, int c) 
{
}

void CWire::begin() {
  BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Output);
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
}
void CWire::end() { 
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
  BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Input);
}
void CWire::requestFrom(int a, int c) { BIOS::GPIO::I2C::RequestFrom(a, c); }
void CWire::beginTransmission(int a) { BIOS::GPIO::I2C::BeginTransmission(a); }
void CWire::endTransmission() { BIOS::GPIO::I2C::EndTransmission(); }
int CWire::read() { return BIOS::GPIO::I2C::Read(); }
void CWire::write(int v) { BIOS::GPIO::I2C::Write(v); }
//int CWire::available() { return BIOS::GPIO::I2C::Available(); }
//bool CWire::error() {}
    
CWire Wire;
