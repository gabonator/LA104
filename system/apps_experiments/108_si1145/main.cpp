#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "Adafruit_SI1145.cpp"

using namespace BIOS;

Adafruit_SI1145 uv = Adafruit_SI1145();

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    uint32_t* gpioStatus = nullptr;
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);

    APP::Init("Si1145 uv/ir/visible light sensor");
    APP::Status("P1: SCL, P2: SDA");

    bool initOk = false;
    int lasttouched = 0;
    int currtouched = 0;  
    auto _BV = [](int i){ return 1<<i; };

    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
      int err = *gpioStatus;
      if (err && err != 2)
      {
        CONSOLE::Color(RGB565(ffff00));
        CONSOLE::Print("<error %d>", err);
        CONSOLE::Color(RGB565(ffffff));        
        *gpioStatus = 0;
      }

      if (!initOk) 
      {
        CONSOLE::Print("Connecting... ");
        initOk = uv.begin();
        if (!initOk)
        {
          CONSOLE::Print("Could not find a valid Si1145 sensor, check wiring!\n");
          SYS::DelayMs(5000);
        } else
        {
          CONSOLE::Print("Ok!\n");
        }
        continue;
      }


      CONSOLE::Print("Vis: %d, IR: %d, UV: %.1f", uv.readVisible(), uv.readIR(), uv.readUV() / 100.f);
      CONSOLE::Print("\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8");

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

