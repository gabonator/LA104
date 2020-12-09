#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "Adafruit_MPR121.cpp"

using namespace BIOS;

Adafruit_MPR121 cap = Adafruit_MPR121();

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    uint32_t* gpioStatus = nullptr;
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);

    APP::Init("MPR121 12 input touch module");
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
        initOk = cap.begin(0x5a);
        if (!initOk)
        {
          CONSOLE::Print("Could not find a valid MPR121 sensor, check wiring!\n");
          SYS::DelayMs(5000);
        } else
        {
          CONSOLE::Print("Ok!\n");
        }
        continue;
      }

      currtouched = cap.touched();
/*      
      for (uint8_t i=0; i<12; i++) {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
          CONSOLE::Print("%d touched            \n", i);
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
          CONSOLE::Print("%d released           \n", i);
        }
      }
      lasttouched = currtouched;
 */

      CONSOLE::Print("Touched: "); 
      for (int i=0; i<12; i++)
        CONSOLE::Print("%d", (currtouched>>i)&1);
//      CONSOLE::Print("Touched value = %x   ", currtouched); 
      CONSOLE::Print("\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8");

      EVERY(10000)
      {
        CONSOLE::Print("\nFilt: "); 
        for (uint8_t i=0; i<12; i++)
          CONSOLE::Print("%3x ", cap.filteredData(i));
        CONSOLE::Print("\nBase: "); 
        for (uint8_t i=0; i<12; i++)
          CONSOLE::Print("%3x ", cap.baselineData(i));
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

