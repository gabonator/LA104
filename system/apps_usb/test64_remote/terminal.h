#include <library.h>

class CTerminal
{
public:
  void begin();
  void end();
  void write(uint8_t b);
  bool available();
  uint8_t read();
};

