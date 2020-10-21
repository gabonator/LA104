#pragma once
//#include <math.h>
#include <stdint.h>
//typedef bool boolean;
#define ARDUINO 100

namespace Arduino 
{
  constexpr int LOW = 0;
  constexpr int HIGH = 0;

  void delay(int d)
  {
    BIOS::SYS::DelayMs(d);
  }

  class CWire
  {
  public:
    void begin()
    {
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
    }

    void end()
    {
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
    }

    void beginTransmission(uint8_t address)
    {
      BIOS::GPIO::I2C::BeginTransmission(address);
    }

    void write(uint8_t data)
    {
      BIOS::GPIO::I2C::Write(data);
    }

    int endTransmission()
    {
      BIOS::GPIO::I2C::EndTransmission();
      return 0;
    }  

    int requestFrom(uint8_t address, int count)
    {
      BIOS::GPIO::I2C::RequestFrom(address, count);
      return count;
    }

    uint8_t read()
    {  
      return BIOS::GPIO::I2C::Read();
    }
  } Wire;
}

using namespace Arduino;


//
//    FILE: PCF8574.H
//  AUTHOR: Rob Tillaart
//    DATE: 02-febr-2013
// VERSION: 0.1.9
// PURPOSE: I2C PCF8574 library for Arduino
//     URL: http://forum.arduino.cc/index.php?topic=184800
//
// HISTORY:
// see PCF8574.cpp file
//

#ifndef _PCF8574_H
#define _PCF8574_H

//#if defined(ARDUINO) && ARDUINO >= 100
//#include "Arduino.h"
//#else
//#include "WProgram.h"
//#endif

#define PCF8574_LIB_VERSION "0.1.9"

#define PCF8574_OK          0x00
#define PCF8574_PIN_ERROR   0x81
#define PCF8574_I2C_ERROR   0x82


class PCF8574
{
public:
    explicit PCF8574(const uint8_t deviceAddress);

    void begin(uint8_t val=0xFF);

    uint8_t read8();
    uint8_t read(uint8_t pin);
    uint8_t value() const { return _dataIn; };

    void write8(const uint8_t value);
    void write(const uint8_t pin, const uint8_t value);
    uint8_t valueOut() const { return _dataOut; }

    //added 0.1.07/08 Septillion
    inline uint8_t readButton8(){ return PCF8574::readButton8(_buttonMask);}
    uint8_t readButton8(const uint8_t mask=0xFF);
    uint8_t readButton(const uint8_t pin);
    void setButtonMask(uint8_t mask);

    // rotate, shift, toggle expect all lines are output
    void toggle(const uint8_t pin);
    void toggleMask(const uint8_t mask);    // invertAll() = toggleMask(0xFF)
    void shiftRight(const uint8_t n=1);
    void shiftLeft(const uint8_t n=1);
    void rotateRight(const uint8_t n=1);
    void rotateLeft(const uint8_t n=1);

    int lastError();

private:
    uint8_t _address;
    uint8_t _dataIn;
    uint8_t _dataOut;
    uint8_t _buttonMask;
    int _error;
};

#endif
//
// END OF FILE
//

//
//    FILE: PCF8574.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 02-febr-2013
// VERSION: 0.1.9
// PURPOSE: I2C PCF8574 library for Arduino
//     URL: http://forum.arduino.cc/index.php?topic=184800
//
// HISTORY:
// 0.1.9  fix warning about return in readButton8()
// 0.1.08 2016-05-20 Merged work of Septillion 
//        Fix/refactor ButtonRead8() - see https://github.com/RobTillaart/Arduino/issues/38
//        missing begin() => mask parameter
// 0.1.07 2016-05-02 (manually merged) Septillion
//        added dataOut so a write() doesn't read first,
//        possibly corrupting a input pin;
//        fixed shift comment, should read 1..7;
//        added begin() to be sure it's in a known state,
//        states could be different if uC is reset and the PCF8574 isn't;
//        added buttonRead() and buttonRead8()
//        which only effect the output while reading
// 0.1.06 (intermediate) added defined errors + refactor rotate
// 0.1.05 2016-04-30 refactor, +toggleMask, +rotLeft, +rotRight
// 0.1.04 2015-05-09 removed ambiguity in read8()
// 0.1.03 2015-03-02 address int -> uint8_t
// 0.1.02 replaced ints with uint8_t to reduce footprint;
//        added default value for shiftLeft() and shiftRight()
//        renamed status() to lastError();
// 0.1.01 added value(); returns last read 8 bit value (cached);
//        value() does not always reflect the latest state of the pins!
// 0.1.00 initial version
//

#include "PCF8574.h"

//#include <Wire.h>

PCF8574::PCF8574(const uint8_t deviceAddress)
{
    _address = deviceAddress;
    _dataIn = 0;
    _dataOut = 0xFF;
    _buttonMask = 0xFF;
    _error = PCF8574_OK;
}

void PCF8574::begin(uint8_t val)
{
//  Wire.begin();
  PCF8574::write8(val);
}

// removed Wire.beginTransmission(addr);
// with  @100KHz -> 265 micros()
// without @100KHz -> 132 micros()
// without @400KHz -> 52 micros()
// TODO @800KHz -> ??
uint8_t PCF8574::read8()
{
    if (Wire.requestFrom(_address, (uint8_t)1) != 1)
    {
        _error = PCF8574_I2C_ERROR;
        return _dataIn; // last value
    }
#if (ARDUINO <  100)
    _dataIn = Wire.receive();
#else
    _dataIn = Wire.read();
#endif

    Wire.endTransmission();
    return _dataIn;
}

void PCF8574::write8(const uint8_t value)
{
    _dataOut = value;
    Wire.beginTransmission(_address);
    Wire.write(_dataOut);
    _error = Wire.endTransmission();
}

uint8_t PCF8574::read(const uint8_t pin)
{
    if (pin > 7)
    {
        _error = PCF8574_PIN_ERROR;
        return 0;
    }
    PCF8574::read8();
    return (_dataIn & (1 << pin)) > 0;
}

void PCF8574::write(const uint8_t pin, const uint8_t value)
{
    if (pin > 7)
    {
        _error = PCF8574_PIN_ERROR;
        return;
    }
    if (value == LOW)
    {
        _dataOut &= ~(1 << pin);
    }
    else
    {
        _dataOut |= (1 << pin);
    }
    write8(_dataOut);
}

void PCF8574::toggle(const uint8_t pin)
{
    if (pin > 7)
    {
        _error = PCF8574_PIN_ERROR;
        return;
    }
    toggleMask(1 << pin);
}

void PCF8574::toggleMask(const uint8_t mask)
{
    _dataOut ^= mask;
    PCF8574::write8(_dataOut);
}

void PCF8574::shiftRight(const uint8_t n)
{
    if (n == 0 || n > 7) return;
    _dataOut >>= n;
    PCF8574::write8(_dataOut);
}

void PCF8574::shiftLeft(const uint8_t n)
{
    if (n == 0 || n > 7) return;
    _dataOut <<= n;
    PCF8574::write8(_dataOut);
}

int PCF8574::lastError()
{
    int e = _error;
    _error = PCF8574_OK;
    return e;
}

void PCF8574::rotateRight(const uint8_t n)
{
    uint8_t r = n & 7;
    _dataOut = (_dataOut >> r) | (_dataOut << (8-r));
    PCF8574::write8(_dataOut);
}

void PCF8574::rotateLeft(const uint8_t n)
{
    rotateRight(8- (n & 7));
}

//added 0.1.07/08 Septillion
uint8_t PCF8574::readButton8(const uint8_t mask)
{
    uint8_t temp = _dataOut;
    PCF8574::write8(mask | _dataOut); 
    PCF8574::read8();
    PCF8574::write8(temp);
    return _dataIn;
}

//added 0.1.07 Septillion
uint8_t PCF8574::readButton(const uint8_t pin)
{
    if (pin > 7)
    {
        _error = PCF8574_PIN_ERROR;
        return 0;
    }
    uint8_t temp = _dataOut;
    PCF8574::write(pin, HIGH);
    uint8_t rtn = PCF8574::read(pin);
    PCF8574::write8(temp);
    return rtn;
}

//added 0.1.08 Septillion
void PCF8574::setButtonMask(uint8_t mask){
  _buttonMask = mask;
}

//
// END OF FILE
//
