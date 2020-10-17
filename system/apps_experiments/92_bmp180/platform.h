#include <library.h>
#include <math.h>

typedef bool boolean;
#define delay(ms) BIOS::SYS::DelayMs(ms)

class CWire 
{
public:
  void begin() {
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
  }
  void end() { 
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
  }
  void requestFrom(int a, int c) { BIOS::GPIO::I2C::RequestFrom(a, c); }
  void beginTransmission(int a) { BIOS::GPIO::I2C::BeginTransmission(a); }
  void endTransmission() { BIOS::GPIO::I2C::EndTransmission(); }
  int receive() { return BIOS::GPIO::I2C::Read(); }
  void send(int v) { BIOS::GPIO::I2C::Write(v); }
};
      
CWire Wire;
