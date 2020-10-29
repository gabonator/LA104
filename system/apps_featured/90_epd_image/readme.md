based on:
- https://github.com/soonuse/epd-library-arduino/tree/master/2.9inch_e-paper/epd2in9

display:
  - manufacturer: buydisplay.com
  - product: ER-EPM029-1R, 2.9 inch E-Paper display module Red/White/Black Color
  - SSD1675 datasheet: https://cdn-learn.adafruit.com/assets/assets/000/092/748/original/SSD1675_0.pdf?1593792604

connections:
  - VDD - 3V
  - VSS - GND
  - MOSI (SDA) - P1
  - SCK (SCL) - P2
  - /CS - P3
  - /RST - P4
  - BUSY - CH4
  - D/C - (not used, 3 spi 9 bit mode, BS1=VDD, DC=VSS)
