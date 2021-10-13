```

LA104:

 GND
  P1 - RST
  P2 - MOSI
  P3 - MISO 
  P4 - SCK
 VCC


ICSP:

  RST   SCK   MISO
  P1     P4   P3
   .     .     . 


   .     .     .
  GND    P2   VCC
        MOSI
  



 ATTiny45
 PB5  1   8  VCC
 PB3  2   7  PB2
 PB4  3   6  PB1
 GND  4   5  PB0


  ADC: PB3, PB4
  PWM: PB0, PB1
 
 RST  1   8  VCC
      2   7  CLK
      3   6  MISO
 GND  4   5  MOSI

```