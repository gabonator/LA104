#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "spi.h"

/*

  P1 - SPI MOSI - DIN
  P2 - RST
  P3 - SPI SCK - CLK
  P4 - DC

*/

// https://www.avdweb.nl/arduino/hardware-interfacing/nokia-5110-lcd
class CNokia5110
{
  CSpi mSpi;

public:
  void Cmd(unsigned char c)
  {
    mSpi.dataCommand(false);
    mSpi.transfer(c);
  }
   
  void Data(unsigned char d)
  {
    mSpi.dataCommand(true);
    mSpi.transfer(d);
  }

  void dotPixel(unsigned char x,unsigned char y)
  {
    Cmd(0x40|(y&0x07));
    Cmd(0x80|(x&0x7f));
  }

  void allClear(void)
  {
    int pixel;
    dotPixel(0,0);                  
    for (pixel=504;pixel>0;pixel--) 
      Data(0);
  }

  void Deinit(void)
  {
    mSpi.end();
  }

  void Init(void)
  {
    mSpi.begin();
    mSpi.reset(false);
    BIOS::SYS::DelayMs(5);
    mSpi.reset(true);

    Cmd(0x21);    
    Cmd(0xb2);    
    Cmd(0x13);  
    Cmd(0x20);    
    Cmd(0x09);    
    allClear();   
    Cmd(0x08);    
    Cmd(0x0C);  
    dotPixel(0,0);    
  }

  void setPointer(unsigned char row,unsigned char col){
    if((row>6) || (row<1) || (col<1) || (col>14))
      return;
    dotPixel(((col-1)*6),(row-1));
  }

  void logoDisp(unsigned char logo[])
  {
    dotPixel(0,0);
    for(int i=0;i<504;i++)
      Data(logo[i]);  
  }
};

CNokia5110 display;
uint8_t buf[504];

__attribute__((__section__(".entry")))
int main(void)
{
    APP::Init("Nokia 5110 display test"); //pcd8544
    CONSOLE::Print("P1 - DIN\n");
    CONSOLE::Print("P2 - RST\n");
    CONSOLE::Print("P3 - CLK\n");
    CONSOLE::Print("P4 - DC\n");
    CONSOLE::Print("GND - CE\n");

    display.Init();
    int grow = 0;
    while (!BIOS::KEY::GetKey())
    {
      for (int i=0; i<504; i++)
        buf[i] = rand()&255;
      for (int i=0; i<grow; i++)
        buf[i] = 255;
      if (grow<504)
        grow++;
      else
      {
        static int reinit = 0;
        if (reinit++ > 5)
        {
          display.Init();
          reinit = 0;
        }
        grow = 0;
      }
      display.logoDisp(buf);
    }
    display.Deinit();
    return 0;

}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
