#pragma once
#include <stdint.h>

typedef bool boolean;

namespace Arduino 
{
  void delay(int d);

  class CWire
  {
  public:
    void begin();
    bool beginTransmission(uint8_t address);
    void write(uint8_t data);
    bool endTransmission();
    bool requestFrom(uint8_t address, int count);
    uint8_t read();
  };

  template <typename T> void bitSet(T& v, int b)
  {
    v |= 1<<b;
  }
  template <typename T> void bitClear(T& v, int b)
  {
    v &= ~(1<<b);
  }
  template <typename T> void bitWrite(T& v, int b, int nv)
  {
    nv ? bitSet(v, b) : bitClear(v, b);
  }
  template <typename T> bool bitRead(const T& v, int b)
  {
    return v & (1<<b);
  }
  extern CWire Wire;
  int atoi(char* p);
}

using namespace Arduino;
