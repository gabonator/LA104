#pragma once
#include <library.h>
#include <math.h>

#define USE_I2C
typedef bool boolean;
typedef uint8_t byte;
#define PROGMEM
#define __FlashStringHelper char
#define delay(ms) BIOS::SYS::DelayMs(ms)
#define F(string) string
#define pgm_read_byte(addr) (*(addr))
void Serial_print(char *);
void Serial_println(char *);
void Serial_print(const char *);
void Serial_println(const char *);
void Serial_print(int);
void Serial_print(int, int);
void Serial_println(int, int);
void Serial_println();
long millis();

void pinMode(int, int);
int digitalRead(int);
void digitalWrite(int, int);

enum {
  SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0, HEX, DEC, SS, LOW, HIGH, OUTPUT, INPUT
};

struct SPISettings 
{
  SPISettings(int a, int b, int c);
};

class CWire 
{
public:
  void begin();
  void end();
  void requestFrom(int a, int c);
  void beginTransmission(int a);
  void endTransmission();
  int read();
  void write(int v);
//  int available();
  bool error();
};
      
extern CWire Wire;
