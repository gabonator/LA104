# PS2 keyboard test app

Attach PS2 keyboard to LA104 and you should see decoded characters on the screen, keyboard can be directly powered by internal 3V power supply.
  - P1 - CLK
  - P2 - DATA
  - GND - GND
  - +3V - VCC

## PS/2 socket pinout

```

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
```