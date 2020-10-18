#include <library.h>
#include "../../os_host/source/framework/Console.h"
//#include "Adafruit_BMP085.h"

uint32_t* gpioStatus = nullptr;

bool gpioError()
{
  if (*gpioStatus)
  {
    *gpioStatus = 0;
    return true;
  }
  return false;
}

void check()
{
      int err = *gpioStatus;
      if (err)
      {
        CONSOLE::Color(RGB565(ffff00));
        CONSOLE::Print("<error %d>", err);
        CONSOLE::Color(RGB565(ffffff));        
        *gpioStatus = 0;
      }
}

#include "DFRobot_SHT20.cpp"

using namespace BIOS;

DFRobot_SHT20 sht20;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);

    CONSOLE::window.left = 8;
    CONSOLE::cursor = CONSOLE::window.TopLeft();

    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "SHT20 test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "P1: SCL, P2: SDA");

    sht20.initSHT20();                                  // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20();                                 // Check SHT20 Sensor

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      int err = *gpioStatus;
      if (err)
      {
        CONSOLE::Color(RGB565(ffff00));
        CONSOLE::Print("<error %d>", err);
        CONSOLE::Color(RGB565(ffffff));        
        *gpioStatus = 0;
      }

      CONSOLE::Print("Temp: %f \xf8""C, Humidity: %f %%\n", sht20.readTemperature(), sht20.readHumidity());
      BIOS::SYS::DelayMs(1000);
    }

    Wire.end();
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

