#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"
#include "assert.h"
#include "epd/epd.h"
#include "bitmap/bitmap.h"
using namespace BIOS;

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

Epd epd;
uint8_t mFileSystemBuffer[BIOS::FAT::SectorSize];
uint16_t pixelBuffer[256];

CRect CenterOnScreen(const CRect& rcImage_)
{
    CRect rcScreen(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
    CRect rcImage(rcImage_);
    rcImage.Offset((rcScreen.Width() - rcImage.Width()) / 2, (rcScreen.Height() - rcImage.Height()) / 2);
    return rcImage;
}


void DrawBitmap(char* name, CRect(*place)(const CRect&))
{
    BIOS::FAT::SetSharedBuffer(mFileSystemBuffer);
    CRect rcImage = GetImageSize(name);

    rcImage = place(rcImage);
    DrawImage(name, rcImage.left, rcImage.top);

    BIOS::FAT::SetSharedBuffer(nullptr);
}


int processPixelColor(uint16_t clr, int p)
{
  int r = Get565R(clr);
  int g = Get565G(clr);
  int b = Get565B(clr);

  if (r > g+40 && r > b+40)
  {
    return 2; // red
  }
  else if (r + g + b < 50*3)
  {
    return 0; // black
  }
  else if (r + g + b < 200*3)
  {
    //return p&1; // gray
    return 0;
  }

  return 1; // white
}

void BeginPass(int x, int y, int w, int h, int index)
{
  epd.SetXYWindow(x>>3, ((x+h)>>3)-1, y, (y+w)-1);
  epd.DelayMs(2);
  epd.SetXYCounter(x>>3, y);
  epd.DelayMs(2);
  epd.SendCommand(DATA_START_TRANSMISSION_1 + index*2);
}

void FinishPass()
{
  epd.DelayMs(2);
}

CRect LoadAndProcess(char* name)
{
  CRect rcEpd(0, 0, EPD_HEIGHT, EPD_WIDTH);
  rcEpd = CenterOnScreen(rcEpd);
  BIOS::LCD::Bar(rcEpd, RGB565(d0d0d0));

  static CPoint cpAnchor;
  cpAnchor = CPoint(rcEpd.left, rcEpd.top);

  CRect rcFrame(rcEpd);
  rcFrame.Inflate(1, 1, 1, 1);
  BIOS::LCD::Rectangle(rcFrame, RGB565(000000));
  rcFrame.Inflate(1, 1, 1, 1);
  BIOS::LCD::Rectangle(rcFrame, RGB565(000000));

  static CRect rcFrame_;
  rcFrame_ = rcEpd;

  char *showName = strstr(name, "/");
  showName = showName ? showName+1 : name;

  BIOS::LCD::Printf(rcFrame_.left+4, rcFrame_.top+4+16*0, RGB565(ff0000), RGBTRANS, "SSD1675");
  BIOS::LCD::Printf(rcFrame_.left+4, rcFrame_.top+4+16*1, RGB565(ff0000), RGBTRANS, "on LA104");
  BIOS::LCD::Printf(rcFrame_.left+4, rcFrame_.top+4+16*2, RGB565(000000), RGBTRANS, "%s", showName);
  BIOS::LCD::Printf(rcFrame_.right-70, rcFrame_.bottom-16*2, RGB565(000000), RGBTRANS, "    2020");
  BIOS::LCD::Printf(rcFrame_.right-70, rcFrame_.bottom-16*1, RGB565(ff0000), RGBTRANS, "valky.eu");

  DrawBitmap(name, [](const CRect& rcImage) {
    BIOS::LCD::Printf(rcFrame_.left+4, rcFrame_.top+4+16*3, RGB565(000000), RGBTRANS, "%dx%d", rcImage.Width(), rcImage.Height());
    return CenterOnScreen(rcImage);
  });

  static const uint16_t newColor[3] = {RGB565(202020), RGB565(ffffff), RGB565(ff0000)};
  int height = rcEpd.Height();

  // Process image 
  for (int x=rcEpd.left; x<rcEpd.right; x++)
  {
    BIOS::LCD::BufferBegin(CRect(x, rcEpd.top, x+1, rcEpd.bottom));
    BIOS::LCD::BufferRead(pixelBuffer, COUNT(pixelBuffer));
    BIOS::LCD::BufferEnd();

    for (int i=0; i<height; i++) 
      pixelBuffer[i] = newColor[processPixelColor(pixelBuffer[i], x+i)];

    BIOS::LCD::BufferBegin(CRect(x, rcEpd.top, x+1, rcEpd.bottom));
    BIOS::LCD::BufferWrite(pixelBuffer, COUNT(pixelBuffer));
    BIOS::LCD::BufferEnd();
  }

  return rcEpd;
}

void Transfer(const CRect& rcEpd)
{
  static const uint16_t newColor[3] = {RGB565(202020), RGB565(ffffff), RGB565(ff0000)};
  int height = rcEpd.Height();

  // Transfer 2 sub images
  _ASSERT(epd.Ready());
  for (int pass=0; pass<2; pass++)
  {
    BeginPass(0, 0, rcEpd.Width(), rcEpd.Height(), pass);

    for (int x=rcEpd.left; x<rcEpd.right; x++)
    {
      BIOS::LCD::BufferBegin(CRect(x, rcEpd.top, x+1, rcEpd.bottom));
      BIOS::LCD::BufferRead(pixelBuffer, COUNT(pixelBuffer));
      BIOS::LCD::BufferEnd();

      uint32_t buf = 0;
      for (int y=0; y<height; y++)
      {
        buf <<= 1;

        if (pixelBuffer[y] == newColor[pass+1])
          buf |= 1;

        if ((y&7) == 7)
          epd.SendData(buf & 0xff);
      }
    }
    FinishPass();
  }

  // display
  epd.DisplayFrame();
}

bool Wait()
{
  CONSOLE::Print("      ");
  long begin = BIOS::SYS::GetTick();
  int phase = 0;
  while (!epd.Ready())
  { 
    long duration = (BIOS::SYS::GetTick() - begin)/1000;
    BIOS::SYS::DelayMs(500);
    CONSOLE::Print("\x08\x08\x08\x08\x08\x08");
    switch (phase++ % 3)
    {
      case 0: CONSOLE::Print("%2d .  ", duration); break;
      case 1: CONSOLE::Print("%2d .. ", duration); break;
      case 2: CONSOLE::Print("%2d ...", duration); break;
    }
    if (KEY::GetKey() != KEY::EKey::None)
    {
      CONSOLE::Print("\x08\x08\x08\x08\x08\x08");
      CONSOLE::Print(" Cancel!\n");
      return false;
    }
  }
  CONSOLE::Print("\x08\x08\x08\x08\x08\x08");
  CONSOLE::Print(" Done!\n");
  return true;
}

CFileDialog mFile;

void epdinit()
{
  CONSOLE::cursor = CPoint(0, 16);

  if (!epd.Init()) {
    CONSOLE::Print("e-Paper init failed\n");
    return;
  }
                               
  CONSOLE::Print("e-Paper Ready, ");
  CONSOLE::Print("Clearing screen ");

  epd.ClearFrame();
  epd.DisplayFrame();
}

bool epdtest() 
{
  CFileFilterSuffix filter(".BMP");
  if (mFile.ModalShow(nullptr, "Load bitmap", &filter))
  {
    CRect rcClient(0, 16, LCD::Width, LCD::Height-16);
    LCD::Bar(rcClient, RGB565(0000b0));
    CONSOLE::cursor = CPoint(0, 16);

    CRect rcEpd = LoadAndProcess(mFile.GetFilename());

    if (!epd.Ready())
    {
      CONSOLE::Print("Busy ");  

      if (!Wait())
        return true;
    }

    CONSOLE::Print("Transferring ");
    Transfer(rcEpd);
    if (!Wait())
      return true;

    return true;
  }

  return false;
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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "eInk display test application");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Printf(8, rc2.top, RGB565(808080), RGBTRANS, "SSD1675A %dx%d", EPD_WIDTH, EPD_HEIGHT);

    epdinit();
    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      if (!epdtest())
        break;
    }
    return 0;
}
