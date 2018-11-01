#pragma once
#include <stdint.h>

#define F(a) a

void delay(int ms);
uint32_t millis();

class CSerial
{
public:
  void begin(int baudrate);
  bool available();
  uint8_t read();
  void write(uint8_t* buffer, int len);
  void print(const char* msg);
    
    
    void println(const char* msg);
    void println(int);
    void println(int, int);
};

enum {HEX};

extern CSerial Serial;
