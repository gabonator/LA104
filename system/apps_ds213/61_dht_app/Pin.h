#pragma once
#include <library.h>

class CPin
{
  BIOS::ADC::EInput m_pin;

public:
  void Create( BIOS::ADC::EInput input )
  {
    //Setup ADC
    BIOS::ADC::Enable( true );
    BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH1, BIOS::ADC::ECouple::DC, BIOS::ADC::EResolution::_1V , 105*4);
    BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH2, BIOS::ADC::ECouple::DC, BIOS::ADC::EResolution::_1V , 122*4);
    BIOS::ADC::ConfigureTimebase(50e-6f);
  //	BIOS::ADC::ConfigureBuffer( arrLen[ (NATIVEENUM)Settings.Time.Resolution ] );
    BIOS::ADC::Restart(0);
    BIOS::ADC::ConfigureTrigger(0, 0, BIOS::ADC::ETriggerType::None, BIOS::ADC::EInput::CH1 );
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

  /*bool Get()
  {
    data = BIOS::ADC::Get();
    ch1 = (uint8_t)((data) & 0xff);
    //BIOS::DBG::Print("#%d#\n", ch1);
    BIOS::ADC::Restart(0);
    return (ch1 > 15) ? true : false;
  }*/

  void operator =(bool b)
  {
    if (b)
      Float();
    else
      High();
  }

  /*operator const bool ()
  {
  	return Get();
  }*/
};
