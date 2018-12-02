#include <math.h>
#include <stdint.h>
typedef bool boolean;

namespace Arduino 
{
  constexpr int LOW = 0;
  constexpr int HIGH = 0;

  void delay(int d)
  {
    BIOS::SYS::DelayMs(d);
  }

  class CWire
  {
  public:
    void begin()
    {
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
    }

    void beginTransmission(uint8_t address)
    {
      BIOS::GPIO::I2C::BeginTransmission(address);
    }

    void write(uint8_t data)
    {
      BIOS::GPIO::I2C::Write(data);
    }

    int endTransmission()
    {
      BIOS::GPIO::I2C::EndTransmission();
      return 0;
    }  

    int requestFrom(uint8_t address, int count)
    {
      BIOS::GPIO::I2C::RequestFrom(address, count);
      return count;
    }

    uint8_t read()
    {  
      return BIOS::GPIO::I2C::Read();
    }
  } Wire;
}

