#include <library.h>

typedef uint8_t byte;
typedef bool boolean;

class TwoWire
{
public:
  void begin();
  void end();
  bool beginTransmission(int addr);
  int read();
  void write(int d);
  bool endTransmission(bool stop = true);
  void requestFrom(int addr, int count);
};

void delay(int d);
extern TwoWire Wire;
