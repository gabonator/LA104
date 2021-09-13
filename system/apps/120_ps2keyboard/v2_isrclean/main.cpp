#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "keyboard.h"

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
  |     ||     |  /
  |   o || o   | /
  |  CLK       |/
  +------------+
*/


#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
  APP::Init("PS2 keyboard test");
  APP::Status("P1: Clock, P2: Data");

  BIOS::KEY::EKey key;

  CKeyboard keyboard;
  keyboard.Attach();
  bool cursor = false;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
  {
    char c = keyboard.Get();
    bool blink = false;
    EVERY(800)
    {
      blink = true;
    }
    if (blink && cursor || c)
    {
      CONSOLE::Print(" \x08");
      cursor = false;
    } else if (blink)      
    {
      CONSOLE::Print("_\x08");
      cursor = true;
    }

    if (c)
      CONSOLE::Print("%c", c);
    if (c==8)
      CONSOLE::Print(" \x08");
  }
  keyboard.Detach();

  return 0;
}
