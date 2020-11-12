#pragma once
#include <stdint.h>
#include <string.h>

typedef uint8_t byte;

class TwoWire
{
public:
  void begin();
  bool beginTransmission(int addr);
  int read();
  void write(int d);
  void endTransmission(bool stop = false);
  void requestFrom(int addr, int count);
};

void delay(int d);

extern TwoWire Wire;