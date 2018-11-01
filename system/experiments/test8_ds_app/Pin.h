#pragma once
#include <library.h>

class CPin
{
  BIOS::GPIO::EPin m_pin;

public:
  void Create( BIOS::GPIO::EPin pin )
  {
    m_pin = pin;
  }

  void High()
  {
    BIOS::GPIO::PinMode(m_pin, BIOS::GPIO::Output);
    BIOS::GPIO::DigitalWrite(m_pin, true); 
  }

  void Low()
  {
    BIOS::GPIO::PinMode(m_pin, BIOS::GPIO::Output);
    BIOS::GPIO::DigitalWrite(m_pin, false); 
  }

  void Float()
  {
    BIOS::GPIO::PinMode(m_pin, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp));
    BIOS::GPIO::DigitalWrite(m_pin, true); 
  }

  bool Get()
  {
    return BIOS::GPIO::DigitalRead(m_pin);
  }

  void operator =(bool b)
  {
    if (b)
      Float();
    else
      High();
  }

  operator const bool ()
  {
  	return Get();
  }
};
