#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"
#include "assert.h"
#include "epd/epd.h"

#include "bitmap/bitmap.h"

using namespace BIOS;

Epd epd;
uint8_t mFileSystemBuffer[BIOS::FAT::SectorSize];
uint16_t pixelBuffer[256];

void CenterOnScreen(CRect& rcImage)
{
    CRect rcScreen(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
    rcImage.Offset((rcScreen.Width() - rcImage.Width()) / 2, (rcScreen.Height() - rcImage.Height()) / 2);
}

void DrawBitmap(char* name)
{
    BIOS::FAT::SetSharedBuffer(mFileSystemBuffer);
    CRect rcImage = GetImageSize(name);
    CenterOnScreen(rcImage);

    DrawImage(name, rcImage.left, rcImage.top);

    BIOS::FAT::SetSharedBuffer(nullptr);
}

int processPixelColor(uint16_t clr)
{
  int r = Get565R(clr);
  int g = Get565R(clr);
  int b = Get565B(clr);

  if (r + g + b < 180*3)
  {
    // black
    return 0; 
  } else
  if (r + r > g + b)
  {
    // red
    // 249, 183, 183 -> 249*2
    return 2;
  }
  // white
  return 1;
}

void BeginPass(int x, int y, int w, int h, int index)
{
  epd.SetXYWindow(x>>3, ((x+h)>>3)-1, y, (y+w)-1);
  epd.DelayMs(2);
  epd.SetXYCounter(x>>3, y);
  epd.DelayMs(2);
  epd.SendCommand(DATA_START_TRANSMISSION_1 + index*2);
}

void Transfer(char* name)
{
  CRect rcEpd(0, 0, EPD_HEIGHT, EPD_WIDTH);
  CenterOnScreen(rcEpd);
  BIOS::LCD::Bar(rcEpd, RGB565(d0d0d0));
  CRect rcFrame(rcEpd);
  rcFrame.Inflate(1, 1, 1, 1);
  BIOS::LCD::Rectangle(rcFrame, RGB565(404040));

  DrawBitmap(name);
  uint16_t newColor[3] = {RGB565(202020), RGB565(ffffff), RGB565(ff0000)};
  int height = rcEpd.Height();

  for (int x=rcEpd.left; x<rcEpd.right; x++)
  {
    BIOS::LCD::BufferBegin(CRect(x, rcEpd.top, x+1, rcEpd.bottom));
    for (int y=0; y<height; y++)
      pixelBuffer[y] = BIOS::LCD::GetPixel(x, y + rcEpd.top);
    //BIOS::LCD::BufferRead(pixelBuffer, COUNT(pixelBuffer));
    BIOS::LCD::BufferEnd();

    for (int i=0; i<height; i++) 
      pixelBuffer[i] = newColor[processPixelColor(pixelBuffer[i])];

    BIOS::LCD::BufferBegin(CRect(x, rcEpd.top, x+1, rcEpd.bottom));
    for (int y=0; y<height; y++)
      BIOS::LCD::PutPixel(x, y+rcEpd.top, pixelBuffer[y]);
//    BIOS::LCD::BufferWrite(pixelBuffer, COUNT(pixelBuffer));
    BIOS::LCD::BufferEnd();
  }

  for (int pass=0; pass<2; pass++)
  {
    BeginPass(0, 0, rcEpd.Width(), rcEpd.Height(), pass);

    for (int x=rcEpd.left; x<rcEpd.right; x++)
    {
      BIOS::LCD::BufferBegin(CRect(x, rcEpd.top, x+1, rcEpd.bottom));
      for (int y=0; y<height; y++)
        pixelBuffer[y] = BIOS::LCD::GetPixel(x, y + rcEpd.top);
      BIOS::LCD::BufferEnd();

      uint32_t buf = 0;
      for (int y=0; y<height; y++)
      {
        buf <<= 1;
        if (pass == 0) // bw
        {
          if (pixelBuffer[height-1-y] == newColor[1])
            buf |= 1;
        } else // red
        {
          if (pixelBuffer[height-1-y] == newColor[2])
            buf |= 1;
        }

        if ((y&7) == 7)
          epd.SendData(buf & 0xff);
      }
    }
    epd.DelayMs(2);
  }
  epd.DisplayFrame();
}

void epdtest() 
{
  enum {
    COLORED = 0,
    UNCOLORED = 1,
  };


  if (epd.Init() != 0) {
    CONSOLE::Print("e-Paper init failed\n");
    return;
  }

  CONSOLE::Print("e-Paper init success\n");

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();
  epd.DisplayFrame();

  Transfer((char*)"macka.bmp");
}

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
