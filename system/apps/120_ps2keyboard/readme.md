# PS2 keyboard test app

Single include ps2 keyboard decoder with scancode translator

```C
#include <library.h>
#include "keyboard.h"

int main(void)
{
  APP::Init("PS2 keyboard test");
  APP::Status("P1: Clock, P2: Data");

  BIOS::KEY::EKey key;

  CKeyboard keyboard;
  keyboard.Attach();
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
  {
    char c = keyboard.Get();
    if (c)
      CONSOLE::Print("%c", c);
  }
  keyboard.Detach();

  return 0;
}
```

Versions:
  - v0_bitbang - bitbanged PS2 protocol decoder
  - v1_isr - interrupt driven decoder with StdPeripheral dependency
  - v2_isrclean - independent from StdPeripheral or CMSIS libraries
  - root - compressed version for simpler integration into other projects

Attach PS2 keyboard to LA104 and you should see decoded characters on the screen, keyboard can be directly powered by internal 3V power supply.
  - P1 - CLK
  - P2 - DATA
  - GND - GND
  - +3V - VCC

## PS/2 socket pinout

```

      CLK            N/C
      GND, DAT, N/C, VCC

        +--------------+
       /  C           /|
      /  .        .  / |
     /  .  .  .  .  /  |
    /  G  D     V  /   |
   /              /    |
  +--------------+     |
  |    DAT       |     |
  |     o  o     |     /
  | GND      VCC |    /
  |  o        o  |   /
  |      ||      |  /
  |   o  ||  o   | /
  |  CLK         |/
  +--------------+
```