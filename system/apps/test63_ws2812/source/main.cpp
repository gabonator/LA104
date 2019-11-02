#include <stdint.h>
#include <library.h>
#include "SPI.h"
#include "../../os_host/source/framework/Console.h"
using namespace BIOS;

#include "WS2812B.h"

class CApp
{
  WS2812B mLeds;
	int mPhase{0};
	
public:
	CApp() : mLeds(24)
	{		
	}
  void Init()
  {
	  mLeds.begin();
	  mLeds.setBrightness(16);
  }

  void Do()
  {
	  switch (mPhase++)
	  {
		  case 0:
			  CONSOLE::Print("Green\n");
			  colorWipe(mLeds.Color(0, 255, 0), 20); // Green
			  break;
		  case 1:
			  CONSOLE::Print("Red\n");
			  colorWipe(mLeds.Color(255, 0, 0), 20); // Red
			  break;
		  case 2:
			  CONSOLE::Print("Blue\n");
			  colorWipe(mLeds.Color(0, 0, 255), 20); // Blue
			  break;
		  case 3:
			  mPhase = 0;
	  }
  }
	
	void colorWipe(uint32_t c, uint8_t wait)
	{
	  for(uint16_t i=0; i<mLeds.numPixels(); i++)
	  {
		  mLeds.setPixelColor(i, c);
		  mLeds.show();
		  BIOS::SYS::DelayMs(wait);
	  }
	}
};

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
	CApp app;
	
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    BIOS::LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "WS2812 addressable led test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    BIOS::LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "spi dma protocol emulation");

    app.Init();
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
        app.Do();
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

