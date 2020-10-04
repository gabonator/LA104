#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "assert.h"
#include "imagedata.h"
#include "epd/epd.h"
#include "epdpaint/epdpaint.h"

unsigned char image[1024];

void epdtest() 
{
  enum {
    COLORED = 0,
    UNCOLORED = 1,
  };

  Epd epd;

  if (epd.Init() != 0) {
    CONSOLE::Print("e-Paper init failed\n");
    return;
  }

  CONSOLE::Print("e-Paper init success\n");

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();
  epd.DisplayFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  Paint paint(image, 128, 18);    //width should be the multiple of 8

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "e-Paper Demo", &Font12, COLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), 12, 32, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();
  paint.Clear(COLORED);
  paint.DrawStringAt(2, 2, "Hello world", &Font16, UNCOLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), 8, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), 64, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 8, 200, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 64, 200, paint.GetWidth(), paint.GetHeight());

  // This displays the data from the SRAM in e-Paper module
  epd.DisplayFrame();

  // This displays an image
//  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED); // wrong resolution

  // Deep sleep
//  epd.Sleep();
}


using namespace BIOS;

void setup()
{
}

void loop()
{
  epdtest();
  BIOS::SYS::DelayMs(15000);
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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "SSD1675A - eInk display");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "...");

    setup();
    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      loop();
    }
    return 0;
}
