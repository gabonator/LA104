#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "scancode.h"

//http://www.lucadavidian.com/2017/11/15/interfacing-ps2-keyboard-to-a-microcontroller/
//https://electronics.stackexchange.com/questions/346202/external-interrupt-delayed-to-in-stm31f103c8

using namespace BIOS;

void Init()
{
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp));
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp));
}

int WaitLow()
{
  int counter = 0;
  while (BIOS::GPIO::DigitalRead(BIOS::GPIO::P1) == 0) 
  {
    if (counter++ > 32000)
      break;
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
  }
  return counter;
}

int WaitHigh()
{
  int counter = 0;
  while (BIOS::GPIO::DigitalRead(BIOS::GPIO::P1) == 1) 
  {
    if (counter++ > 32000)
      break;
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
  }
  return counter;
}

  int buffer[128];
  CArray<int> dur(buffer, COUNT(buffer));
  uint8_t bitsbuf[128];
  CArray<uint8_t> bits(bitsbuf, COUNT(bitsbuf));

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
  APP::Init("PS2 keyboard test");
  APP::Status("P1: Clock, P2: Data");

  Init();
  KEY::EKey key;

  bool release = false; 
  bool shift = false; 

  while ((key = KEY::GetKey()) != KEY::EKey::Escape)
  {
    if (BIOS::GPIO::DigitalRead(BIOS::GPIO::P1) == 0)
    {
      for (int i=0; i<11; i++)
      {
        bits.Add(BIOS::GPIO::DigitalRead(BIOS::GPIO::P2));
        WaitLow();

        int high = WaitHigh(); // 30
        if (high > 40)
          break;
        dur.Add(high);

        int low = WaitLow(); // 30
        if (low > 40)
          break;
        dur.Add(low);
      }

      if (bits.GetSize() == 10) // missed start bit
        bits.InsertAt(0, 0);

      if (bits.GetSize() == 11)
      {
        int code = 0;
        for (int i=0; i<8; i++)
          code |= bits[i+1] ? (1<<i) : 0;

        if (code == 0x12 || code == 0x59)
        {
          shift = !release;
          release = true; // dont show code
        }

        if (code == 0xf0) // release
        {
          release = true;
        } else
        {
          if (!release)
          {
            for (int i=0; i<COUNT(scan_codes); i+=3)
              if (scan_codes[i] == code)
              {
                CONSOLE::Print("%c", shift ? scan_codes[i+2] : scan_codes[i+1]);
                release = true; // dont show code
                break;
              }
          }
//          CONSOLE::Color(release ? RGB565(ffff00) : RGB565(ffffff));
          if (!release)
            CONSOLE::Print("%02x, ", code);
          release = false;
        }
      } else
      {
        for (int i=0; i<bits.GetSize(); i++)
          CONSOLE::Print("%d", bits[i]);
        CONSOLE::Print("\n");
      }
      dur.RemoveAll();
      bits.RemoveAll();
    }
  }

  return 0;
}


void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

