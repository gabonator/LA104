#pragma once

#include <math.h>
#include <stdint.h>
#include "../../host/source/bios/Bios.h"
typedef bool boolean;
#define F(a) a

namespace Arduino 
{
  typedef void (*callback)(void);

  uint32_t millis();
  void delay(int d);

  enum {RISING, FALLING};
  void attachInterrupt(int, callback f, int);
  void detachInterrupt(int);

  enum {INPUT, OUTPUT};
  void pinMode(int, int);

  class CWire
  {
  public:
    void begin();
    void beginTransmission(uint8_t address);
    void write(uint8_t data);
    int endTransmission();
    void requestFrom(uint8_t address, int count);
    uint8_t read();
    bool available();
  };
  
  enum {HEX};

  class CSerial
  {
  public:
    void begin(int baudrate);
    bool available();
    uint8_t read();
    void write(uint8_t* buffer, int len);
    void print(const char* msg);
    void print(int);
      
    void println(const char* msg);
    void println(int);
    void println(int, int);
    void println() {print("\n");}
  };

  extern CSerial Serial;
  extern CWire Wire;
}

using namespace Arduino;