#include <library.h>
#include "../../os_host/source/framework/Console.h"
//#include "Adafruit_BMP085.h"
#include "Adafruit_BMP085.cpp"

using namespace BIOS;

Adafruit_BMP085 bmp;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    uint32_t* gpioStatus = nullptr;
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
    CONSOLE::window.left = 8;
    CONSOLE::cursor = CONSOLE::window.TopLeft();

    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "BMP085 / BMP180 test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "P1: SCL, P2: SDA");

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      bool initOk = bmp.begin();

      int err = *gpioStatus;
      if (err)
      {
        CONSOLE::Color(RGB565(ffff00));
        CONSOLE::Print("<error %d>", err);
        CONSOLE::Color(RGB565(ffffff));        
        *gpioStatus = 0;
      }

      if (!initOk) 
      {
        CONSOLE::Print("Could not find a valid BMP085 sensor, check wiring!\n");
        SYS::DelayMs(5000);
        continue;
      }

      CONSOLE::Print("Pressure = %f kPa   ", bmp.readPressure() / 1000.0f);  
      CONSOLE::Print("\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8");

      EVERY(10000)
      {
        CONSOLE::Print("\n");
        CONSOLE::Print("Temperature = %f \xf8""C\n", bmp.readTemperature());
        CONSOLE::Print("Altitude = %f meter\n", bmp.readAltitude());

        CONSOLE::Print("Pressure at sealevel = %d Pa\n", bmp.readSealevelPressure());
        // you can get a more precise measurement of altitude
        // if you know the current sea level pressure which will
        // vary with weather and such. If it is 1015 millibars
        // that is equal to 101500 Pascals.
        CONSOLE::Print("Real altitude = %f meters\n", bmp.readAltitude(101500));
        CONSOLE::Print("\n");
      }
      SYS::DelayMs(50);
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

