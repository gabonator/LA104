#pragma once

#include <math.h>
#include <stdint.h>
#include "../../../os_library/include/library.h"
typedef bool boolean;
typedef uint8_t byte;
#define F(a) a

template<typename T, typename T1, typename T2>
T constrain(T value, T1 vmin, T2 vmax)
{
  return max((T)vmin, min(value, (T)vmax));
}

namespace Arduino 
{
  typedef void (*callback)(void);

  uint32_t millis();
  void delay(int d);
  void delayMicroseconds(int d);

  enum {RISING, FALLING};
  void attachInterrupt(int, callback f, int);
  void detachInterrupt(int);

  enum {INPUT, OUTPUT};
  enum {LOW, HIGH};
  void pinMode(int, int);
  void digitalWrite(int, int);
  int digitalRead(int);

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
    void write(uint8_t c);
    void print(const char* msg);
    void print(int);
    void print(int, int);
      
    void println(const char* msg);
    void println(int);
    void println(int, int);
    void println() {print("\n");}
  };

  void noInterrupts();
  void interrupts();

  extern CSerial Serial;
  extern CWire Wire;
}

using namespace Arduino;
