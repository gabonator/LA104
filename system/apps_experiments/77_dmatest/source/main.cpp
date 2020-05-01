#include <library.h>
#include "assert.h"

#include "streamer/streamer.h"

extern int streamerBufferMaxCounter;

using namespace BIOS;

void setup() 
{
  BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P4, BIOS::GPIO::EMode::Input);
  streamerBufferMaxCounter = 200;
  streamerBegin();
}

void finish()
{
  streamerEnd();
}

void loop()
{
  static long lastRequest = 0;
  static int seq = 0;
  
    static int p = 0;
    static int x=0;
    int n = streamerBuffer.size();
    static int lx = 0;
    for (int i=0; i<n; i++, p++)
    {
      int v = streamerBuffer.pull();
      if (v > 0 && v<20) v = 20;
      BIOS::LCD::Bar(lx/20, 52, (lx)/20+2, 68, RGB565(0000b0));
      BIOS::LCD::Bar(x/20, 52, x/20+2, 68, RGB565(ffffff));
      lx = x;
      BIOS::LCD::Bar(x/20, 50, (x+v)/20, 52, (p & 1) ? RGB565(ff0000) : RGB565(0000b0));
      BIOS::LCD::Bar(x/20, 68, (x+v)/20, 70, (p & 1) ? RGB565(0000b0) : RGB565(00ff00));
      x += v;
      if (x/20 >= BIOS::LCD::Width)
        x -= BIOS::LCD::Width * 20;
    }
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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "DMA on pin P4 test");

    setup();

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));

    streamerBegin();
    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      loop();
    }
    finish();
    
    return 0;
}
