#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "assert.h"
#include "spi.h"

using namespace BIOS;
//https://gist.github.com/nrdobie/8193350

/*
  P1 - CLK
  P2 - CS
  P3 - DIN
*/

CSpi spi;

void maxTransfer(int address, int value)
{
  spi.select();
  spi.transfer(address);
  spi.transfer(value);
  spi.deselect();
}

void setup()
{
  spi.begin();

}

const uint32_t img1[] = 
  {0b00111100,
   0b01000010,
   0b10100101,
   0b10000001,
   0b10100101,
   0b10011001,
   0b01000010,
   0b00111100 };

const uint32_t img2[] = 
  {0b00000000,
   0b01101100,
   0b10010010,
   0b10000010,
   0b10000010,
   0b01000100,
   0b00101000,
   0b00010000 };

void loop()
{
//  maxTransfer(0x0F, 0x01); // all on
//  maxTransfer(0x0F, 0x00); // all off  
  maxTransfer(0x09, 0x00); // no decode (can decode for 7seg displays)  
  maxTransfer(0x0A, 0x00); // Use lowest intensity 1/16
  maxTransfer(0x0B, 0x07); // Scan 8 rows  
  maxTransfer(0x0C, 0x01); // Turn on chip
  
  const uint32_t* img = ((BIOS::SYS::GetTick() >> 10) & 1) ? img1 : img2;

  for (uint8_t x=0; x<8; x++)
  {
    int c = 0;
    for (uint8_t y=0; y<8; y++)
      if ((img[y] << x) & 128)
        c |= 1<<y;
                 
    maxTransfer(x+1, c);
  }
  BIOS::SYS::DelayMs(100);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "MAX7219 8x8 dot display test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "P1: CLK, P2: CS, P3: DIN");

    setup();
    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      loop();
    }
    
//    GPIO::PinMode(GPIO::P1, GPIO::Output);
//    GPIO::PinMode(GPIO::P2, GPIO::Input);
    return 0;
}
