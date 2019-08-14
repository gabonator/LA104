#include <library.h>
#include "../../os_host/source/framework/Console.h"

//using namespace I2C = BIOS::GPIO::I2C;
using namespace BIOS::GPIO;

#define DS3231_I2C_ADDR 0x68
#define DS3231_TIME_CAL_ADDR        0x00

struct ts
{
    int sec;
    int min;
    int hour;
    int mday;
    int mon;
    int year;
    int wday;
    int year_s;
};

uint8_t dectobcd(uint8_t num)
{
  return ((num/10 * 16) + (num % 10));
}

uint8_t bcdtodec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}

bool DS3231_get(struct ts *t)
{
    uint8_t TimeDate[7];        //second,minute,hour,dow,day,month,year
    uint8_t century = 0;
    uint8_t i, n;
    uint16_t year_full;

    I2C::BeginTransmission(DS3231_I2C_ADDR);
    I2C::Write(DS3231_TIME_CAL_ADDR);
    I2C::EndTransmission();

    I2C::RequestFrom(DS3231_I2C_ADDR, 7);
    
    for (i = 0; i <= 6; i++) {
        n = I2C::Read();
        if (i == 5) {
            TimeDate[5] = bcdtodec(n & 0x1F);
            century = (n & 0x80) >> 7;
        } else
            TimeDate[i] = bcdtodec(n);
    }
    I2C::EndTransmission();

    if (century == 1) {
        year_full = 2000 + TimeDate[6];
    } else {
        year_full = 1900 + TimeDate[6];
    }

    t->sec = TimeDate[0];
    t->min = TimeDate[1];
    t->hour = TimeDate[2];
    t->mday = TimeDate[4];
    t->mon = TimeDate[5];
    t->year = year_full;
    t->wday = TimeDate[3];
    t->year_s = TimeDate[6];
    return true;
}

void DS3231_set(struct ts t)
{
    uint8_t i, century;

    if (t.year >= 2000) {
        century = 0x80;
        t.year_s = t.year - 2000;
    } else {
        century = 0;
        t.year_s = t.year - 1900;
    }

    uint8_t TimeDate[7] = { (uint8_t)t.sec, (uint8_t)t.min, (uint8_t)t.hour, (uint8_t)t.wday, (uint8_t)t.mday, (uint8_t)t.mon, (uint8_t)t.year_s };

    I2C::BeginTransmission(DS3231_I2C_ADDR);
    I2C::Write(DS3231_TIME_CAL_ADDR);
    for (i = 0; i <= 6; i++) {
        TimeDate[i] = dectobcd(TimeDate[i]);
        if (i == 5)
            TimeDate[5] += century;
        I2C::Write(TimeDate[i]);
    }
    I2C::EndTransmission();
}

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  CONSOLE::Print("DS3231 test app\n");

  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);

  BIOS::KEY::EKey key;

  ts t;

  DS3231_get(&t);
  if (t.year != 2019)
  {
    CONSOLE::Print("Setting default time\n");
    t.year = 2019;
    t.mon = 8;
    t.mday = 11;
    t.hour = 22;
    t.min = 32;
    t.sec = 0;
    DS3231_set(t);
  }

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    bool ok = DS3231_get(&t);
    if (ok)
    {
      CONSOLE::Print("Y:%d M:%d D:%d  %02d:%02d:%02d\n", t.year, t.mon, t.mday, t.hour, t.min, t.sec);
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

