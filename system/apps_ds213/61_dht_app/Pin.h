#pragma once
#include <library.h>
#include "Sampler.h"
#include <math.h>
#include "../../os_host/source/framework/Utils.h"
#include "../../os_host/source/framework/Serialize.h"

class CPin
{
  BIOS::ADC::EInput m_pin;
  volatile uint32_t data;
  volatile uint8_t ch1;
public:
  void Create( BIOS::ADC::EInput input )
  {
    m_pin = input;
  }

  //ADC Cant output
  void High()
  {
    BIOS::DAC::SetMode(BIOS::DAC::EMode::LogicHigh, 0, 0);
    //BIOS::GPIO::PinMode(m_pin, BIOS::GPIO::Output);
    //BIOS::GPIO::DigitalWrite(m_pin, true);
  }

  void Low()
  {
    BIOS::DAC::SetMode(BIOS::DAC::EMode::LogicLow, 0, 0);
    //BIOS::GPIO::PinMode(m_pin, BIOS::GPIO::Output);
    //BIOS::GPIO::DigitalWrite(m_pin, false); 
  }

  void Float()
  {
    //Low();
    BIOS::DAC::SetMode(BIOS::DAC::EMode::LogicHigh, 0, 0);
  }

  bool Get()
  {
    data = BIOS::ADC::Get();
    ch1 = (uint8_t)((data) & 0xff);
    //BIOS::DBG::Print("#%d#\n", ch1);
    BIOS::ADC::Restart(0);
    return (ch1 > 15) ? true : false;
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
