#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library

void setup(void) 
{
  Serial.begin(9600);
  BIOS::LCD::Clear(RGB565(202020));
}

int lineByAddr(uint8_t* rom)
{
  switch (rom[2])
  {
    case 0xe8: return 0;
    case 0x32: return 1;
    case 0xdf: return 2;
    case 0x1c: return 3;
    case 0x8b: return 4;
  }
  return -1;
}

void loop(void) {
  static OneWire ds(BIOS::GPIO::P1);  // on pin 10 (a 4.7K resistor is necessary)
  static bool first = true;
  byte i;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;

  if ( first || !ds.search(addr)) 
  {
    first = false;
    ds.reset_search();

    ds.reset();
    ds.skip();
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    delay(2000);

    for (int y=0; y<5; y++)
      BIOS::LCD::Print(4, 28+y*48, RGB565(202020), RGB565(202020), "*");
    return;
  }

  char strRom[40];
  char* strRom_ = strRom;

  char strScratch[40];
  char* strScratch_ = strScratch;

  strcpy(strRom_, "Rom = ");
  strRom_ += strlen(strRom_);
  for( i = 0; i < 8; i++)
    strRom_ += sprintf(strRom_, "%02x ", addr[i]);

  if (OneWire::crc8(addr, 7) != addr[7]) {
      BIOS::DBG::Print(strRom);
      BIOS::DBG::Print("!!CRC Error!!\n");
      return;
  }
 
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
      BIOS::DBG::Print("Device is not a DS18x20 family device.\n");
      return;
  } 

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  strcpy(strScratch_, "Data = ");
  strScratch_ += strlen(strScratch_);
  for( i = 0; i < 9; i++)
    strScratch_ += sprintf(strScratch_, "%02x ", data[i]);

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
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;

  char strTemp[40];
  sprintf(strTemp, "%s temp = %f\xf8" "C\n", chip, celsius);

  int y = lineByAddr(addr);
  if (y==-1)
    BIOS::DBG::Print("%s\n%s\n", strRom, strTemp);

  BIOS::LCD::Print(4, 28+y*48, RGB565(ffff00), RGB565(202020), "*");
  BIOS::LCD::Print(4+16, 0+y*48, RGB565(b0b0b0), RGB565(202020), strRom);
  BIOS::LCD::Print(4+16, 14+y*48, RGB565(b0b0b0), RGB565(202020), strScratch);
  BIOS::LCD::Print(4+16, 28+y*48, RGB565(ffffff), RGB565(202020), strTemp);
}
