#include <library.h>

typedef uint8_t byte;
typedef bool boolean;

enum {LOW = 0, HIGH = 1};
enum {INPUT = BIOS::GPIO::EMode::Input | BIOS::GPIO::EMode::PullUp, OUTPUT = BIOS::GPIO::EMode::Output};

class TwoWire
{
public:
  void begin();
  void end();
  bool beginTransmission(int addr);
  int read();
  void write(int d);
  bool endTransmission(bool stop = false);
  void requestFrom(int addr, int count);
};

void delay(int d);
void digitalWrite(int, int);
void pinMode(int, int);
long millis();
extern TwoWire Wire;