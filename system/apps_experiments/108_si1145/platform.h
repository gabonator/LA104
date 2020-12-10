#include <library.h>
#include <math.h>

typedef uint8_t byte;
typedef bool boolean;
#define delay(ms) BIOS::SYS::DelayMs(ms)

class TwoWire 
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
  void endTransmission(bool stop=true) { BIOS::GPIO::I2C::EndTransmission(stop); }
  int read() { return BIOS::GPIO::I2C::Read(); }
  void write(int v) { BIOS::GPIO::I2C::Write(v); }
  int available() { return 10; }
};
      
TwoWire Wire;
