#include <library.h>
#include "platform.h"

using namespace BIOS;

/*
void check(const char* m)
{
  static uint32_t* gpioStatus = nullptr;
  if (!gpioStatus)
  {
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
  }

  if (*gpioStatus != 0)
  {
    CONSOLE::Color(RGB565(ff0000));
    CONSOLE::Print(" i2c error:%d %s", *gpioStatus, m);
    CONSOLE::Color(RGB565(ffffff));
    if (*gpioStatus == 3)
    {
//      GPIO::I2C::EndTransmission(true);    
//      CONSOLE::Print(" stop? = %d ", *gpioStatus);
    }
    *gpioStatus = 0;
  } else
    CONSOLE::Print("%s", m);
}
*/

void check(const char*) {}
void checkx(int) {}

void TwoWire::begin() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::I2c);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::I2c);
}

void TwoWire::end() 
{
  GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
  GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Input);
}

bool TwoWire::beginTransmission(int addr)  
{
  check("B{");
  bool wireOk = GPIO::I2C::BeginTransmission(addr);
  checkx(wireOk);
  check("}");
  return wireOk;
}

int TwoWire::read() 
{
  check("R{");
  int a = GPIO::I2C::Read();
  check("}");
  return a;
}

void TwoWire::write(int d)
{
  check("W{");
  GPIO::I2C::Write(d);
  check("}");
}

bool TwoWire::endTransmission(bool stop) 
{
  check("E{");
  GPIO::I2C::EndTransmission(stop);    
  check("}");
  return true;
}

void TwoWire::requestFrom(int addr, int count)
{
  check("X{");
  GPIO::I2C::RequestFrom(addr, count);    
  check("}");
}

void delay(int d)
{
  BIOS::SYS::DelayMs(d);
}

long millis()
{
  return BIOS::SYS::GetTick();
}

void pinMode(int pin, int mode)
{
  BIOS::GPIO::PinMode((BIOS::GPIO::EPin)pin, (BIOS::GPIO::EMode)mode);
}

void digitalWrite(int pin, int level)
{
  BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)pin, level);
}

TwoWire Wire;


Adafruit_GFX::Adafruit_GFX(int16_t w, int16_t h) : WIDTH(w), HEIGHT(h) {
  _width = WIDTH;
  _height = HEIGHT;
  rotation = 0;
}
  uint8_t Adafruit_GFX::getRotation(void) const { return rotation; }

void Adafruit_GFX::setRotation(uint8_t x) {
  rotation = (x & 3);
  switch (rotation) {
  case 0:
  case 2:
    _width = WIDTH;
    _height = HEIGHT;
    break;
  case 1:
  case 3:
    _width = HEIGHT;
    _height = WIDTH;
    break;
  }
}

  int16_t Adafruit_GFX::width(void) const { return _width; };
  int16_t Adafruit_GFX::height(void) const { return _height; }


void Adafruit_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                            uint16_t color) {
  for (int16_t _y = y; _y < y + h; _y++) {
    for (int16_t _x = x; _x < x + w; _x++) {
      drawPixel(_x, _y, color);
    }
  }
}

void Adafruit_GFX::drawPixel(int16_t x, int16_t y, uint16_t color) {}

void Adafruit_GFX::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                              int16_t w, int16_t h, uint16_t color) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      if (byte & 0x80)
        drawPixel(x + i, y, color);
    }
  }
}
