#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "scancode.h"

void isrinit(void);
void isrhandle(void);
uint32_t isrget();

/*           
PS/2 socket pinout

      CLK            N/C
      GND, DAT, N/C, VCC

        +-----------+
       /  C        /|
      /  .     .  / |
     /  . . . .  /  |
    /  G D   V  /   |
   /           /    |
  + -----------+    |
  |   DAT      |    |
  |    o  o    |    |
  | GND    VCC |    +
  |  o      o  |   /
  |     ##     |  /
  |   o ## o   | /
  |  CLK       |/
  +------------+
*/

// http://www.lucadavidian.com/2017/11/15/interfacing-ps2-keyboard-to-a-microcontroller/
// https://electronics.stackexchange.com/questions/346202/external-interrupt-delayed-to-in-stm31f103c8
// https://stackoverflow.com/questions/12622948/generating-exti0-interrupt-when-extenal-temperature-sensor-is-connected-to-stm32
// https://stm32f4-discovery.net/2014/08/stm32f4-external-interrupts-tutorial/
// https://topic.alibabacloud.com/a/stm32f103-external-interrupt-programming_8_8_31203533.html
// http://mcu.cz/comment-n2177.html

using namespace BIOS;

void Init()
{
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp));
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp));
}

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

  isrinit();

  BIOS::OS::TInterruptHandler isrDebugOld;
  isrDebugOld = BIOS::OS::GetInterruptVector(BIOS::OS::IEXTI15_10_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IEXTI15_10_IRQ, [](){ 
    isrhandle();
  });

  while ((key = KEY::GetKey()) != KEY::EKey::Escape)
  {
    uint32_t codeblock = isrget();
    for (int i=24; i>=0; i-=8)
    {
      int code = (codeblock >> i) & 0xff;

      if (!code)
        continue;

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
              if (scan_codes[i+1] == 8)
                CONSOLE::Print("\x08 \x08");
              else
                CONSOLE::Print("%c", shift ? scan_codes[i+2] : scan_codes[i+1]);
              release = true; // dont show code
              break;
            }
        }
        if (!release)
          CONSOLE::Print("%02x, ", code);
        release = false;
      }
    }
  }

  BIOS::OS::SetInterruptVector(BIOS::OS::IEXTI15_10_IRQ, isrDebugOld);

  return 0;
}

