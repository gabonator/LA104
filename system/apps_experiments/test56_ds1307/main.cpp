#include <library.h>
#include "../../os_host/source/framework/Console.h"

// Tiny RTC I2C modules - device not working when backup battery is present

//using namespace I2C = BIOS::GPIO::I2C;
using namespace BIOS::GPIO;

#define DS1307_CTRL_ID 0x68 

struct tmElements_t
{
  int Second, Minute, Hour, Wday, Day, Month, Year;
};

uint8_t dec2bcd(uint8_t num)
{
  return ((num/10 * 16) + (num % 10));
}

uint8_t bcd2dec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}

int y2kYearToTm(int n)
{
  return 2000+n;
}

int tmYearToY2k(int n)
{
  return n-2000;
}

bool read(tmElements_t &tm)
{
  uint8_t sec;
  I2C::BeginTransmission(DS1307_CTRL_ID);
  I2C::Write((uint8_t)0x00); 
  if (I2C::EndTransmission() != 0)
  {
//    CONSOLE::Print("Device not found\n");
//    return false;
  }

  // request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
  const int tmNbrFields = 7;
  I2C::RequestFrom(DS1307_CTRL_ID, tmNbrFields);
  sec = I2C::Read();
  tm.Second = bcd2dec(sec & 0x7f);   
  tm.Minute = bcd2dec(I2C::Read() );
  tm.Hour =   bcd2dec(I2C::Read() & 0x3f);  // mask assumes 24hr clock
  tm.Wday = bcd2dec(I2C::Read() );
  tm.Day = bcd2dec(I2C::Read() );
  tm.Month = bcd2dec(I2C::Read() );
  tm.Year = y2kYearToTm((bcd2dec(I2C::Read())));
  I2C::EndTransmission();
  if (sec & 0x80)  
  {
    CONSOLE::Print("Clock halted\n");
    return false; // clock is halted
  }

  return true;
}


bool write(tmElements_t &tm)
{
  // To eliminate any potential race conditions,
  // stop the clock before writing the values,
  // then restart it after.
  I2C::BeginTransmission(DS1307_CTRL_ID);
  I2C::Write((uint8_t)0x00); // reset register pointer  
  I2C::Write((uint8_t)0x80); // Stop the clock. The seconds will be written last
  I2C::Write(dec2bcd(tm.Minute));
  I2C::Write(dec2bcd(tm.Hour));      // sets 24 hour format
  I2C::Write(dec2bcd(tm.Wday));   
  I2C::Write(dec2bcd(tm.Day));
  I2C::Write(dec2bcd(tm.Month));
  I2C::Write(dec2bcd(tmYearToY2k(tm.Year))); 
  if (I2C::EndTransmission() != 0) {
//    return false;
  }

  // Now go back and set the seconds, starting the clock back up as a side effect
  I2C::BeginTransmission(DS1307_CTRL_ID);
  I2C::Write((uint8_t)0x00); // reset register pointer  
  I2C::Write(dec2bcd(tm.Second)); // write the seconds, with the stop bit clear to restart
  if (I2C::EndTransmission() != 0) {
//    return false;
  }
  return true;
}

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  CONSOLE::Print("DS1307 test app\n");

  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);

  BIOS::KEY::EKey key;

  tmElements_t t;

  if (!read(t))
  {
    CONSOLE::Print("Setting default time\n");
    t.Year = 2019;
    t.Month = 8;
    t.Day = 11;
    t.Hour = 21;
    t.Minute = 42;
    t.Second = 0;
    write(t);
  }

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    bool ok = read(t);
    if (ok)
    {
      CONSOLE::Print("Y:%d M:%d D:%d  %02d:%02d:%02d\n", t.Year, t.Month, t.Day, t.Hour, t.Minute, t.Second);
    }

    BIOS::SYS::DelayMs(1000);
  }

  return 0;
}


void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}

