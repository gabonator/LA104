#include <library.h>
#include "OneWire.h"

class CDS1820
{
public:
  typedef uint8_t TAddress[8];
  typedef uint8_t TScratchpad[9];

private:
    OneWire ds{BIOS::GPIO::EPin::P1};
//  bool first{true};

public:
  void StartConversion()
  {
    ds.reset();
    ds.skip();
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    // wait 2 seconds
  }

  void SearchReset()
    {
        ds.reset_search();
    }
  bool SearchNext(TAddress& addr)
    {
        return ds.search(addr);
    }
    bool ReadScratchpad(TAddress& addr, TScratchpad& data)
    {
        if (OneWire::crc8(addr, 7) != addr[7])
            return false;
        
        ds.reset();
        ds.select(addr);
        ds.write(0xBE);         // Read Scratchpad
        
        for (int i = 0; i < 9; i++)
            data[i] = ds.read();
        
        return true;

    }

  bool GetTemperature(TAddress& addr, TScratchpad& data, int& temperature)
  {
    uint8_t type_s = 0;
    const char* chip = "Unknown";

    // the first ROM byte indicates which chip
    switch (addr[0]) {
      case 0x10:
        chip = "DS18S20";
        type_s = 1;
        break;
      case 0x28:
        chip = "DS1BS20";
        type_s = 0;
        break;
      case 0x22:
        chip = "DS1822";
        type_s = 0;
        break;
      default:
        _ASSERT(!"Device is not a DS18x20 family device.\n");
        return false;
    }
    (void)chip;
    // TODO: add crc check

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } else {
      uint8_t cfg = (data[4] & 0x60);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      //// default is 12 bit resolution, 750 ms conversion time
    }
      temperature = raw; // calculate (raw / 16.0) to get degrees
    return true;
  }
};
