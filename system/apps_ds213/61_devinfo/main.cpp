#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"

using namespace BIOS;

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedWriter mWriter;

void Window(const CRect& rcRect, uint16_t clr)
{
  CRect rcClient = rcRect;
  BIOS::LCD::RoundRect(rcClient, RGB565(000000));
  rcClient.Deflate(2, 2, 2, 2);
  BIOS::LCD::RoundRect(rcClient, clr);
  rcClient.Deflate(2, 14, 2, 2);
  BIOS::LCD::RoundRect(rcClient, RGB565(404040));
}

void WindowClear(const CRect& rcRect)
{
  CRect rcClient = rcRect;
  rcClient.Deflate(2, 2, 2, 2);
  rcClient.Deflate(2, 14, 2, 2);
  BIOS::LCD::RoundRect(rcClient, RGB565(404040));
}

void RedrawPage(const CRect& rcMain, int page)
{
  CPoint cpDraw(rcMain.left + 8, rcMain.top + 16 + 2);
  CPoint cpHeading(rcMain.left +  8, rcMain.top + 2);
  constexpr int Line = 14;

  auto PrintHeading = [&](bool selected, const char* format, ...)
  {
    char buf[64];
    
    va_list args;    
    va_start( args, format );
    vsprintf( buf, format, args );
    va_end(args);

    if (selected)
      cpHeading.x += BIOS::LCD::Print(cpHeading.x, cpHeading.y, RGB565(ffffff), RGB565(2fafff), buf);
    else
      cpHeading.x += BIOS::LCD::Print(cpHeading.x, cpHeading.y, RGB565(002040), RGB565(0080d0), buf);
  };

  auto PrintLine = [&](const char* key, const char* format, ...)
  {
    char buf[64];
    
    va_list args;    
    va_start( args, format );
    vsprintf( buf, format, args );
    va_end(args);

    if (strlen(key) + strlen(buf)+1 > 27)
      strcpy(&buf[27-strlen(key)-1-3], "...");

    CPoint cpLine = cpDraw;
    cpLine.x += 8 + BIOS::LCD::Print(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, key);
    cpLine.x += BIOS::LCD::Print(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, buf);
    cpDraw.y += Line;
  };

  auto FormatLicenseValid = [](int i)
  {
    switch (i)
    {
      case 0: return "Invalid";
      case 1: return "Ok!";
      default: return "Unknown";
    }
  };

  PrintHeading(false, "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DeviceType));
  PrintHeading(page == 0, " HW ");
  PrintHeading(page == 1, " SW ");
  PrintHeading(page == 2, " LIC ");
//  PrintHeading(page == 3, " HASH ");

  switch (page)
  {
    case 0:
      PrintLine("DFU version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionDfu));
      PrintLine("Hardware version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionHardware));
      PrintLine("System version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionSystem));
      PrintLine("FPGA version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionFpga));
//      PrintLine("Serial number:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::SerialNumber));
      PrintLine("Display:", "%s (%dx%d)", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DisplayType), 
        BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenWidth),
        BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenHeight));
      PrintLine("Disk:", "%s (%dkB)", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskType), 
        BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize)*
        BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorCount)/1024);
    break;
    case 1:
      PrintLine("BIOS version:", "%04x", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BiosVersion));
      PrintLine("Build rev:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildRevision));
      PrintLine("Build date:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildDate));
      PrintLine("Build user:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildUser));
      PrintLine("Build system:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildSystem));
    break;
    case 2:
      PrintLine("Serial number:", "%08x", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::SerialNumber));
      PrintLine("License number:", "%08x", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::LicenseNumber));
      PrintLine("License validity:", "%s", FormatLicenseValid(BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::LicenseValid)));
    break;
/*
    case 3:
      InitHw();
      PrintLine("App1Base:", "%08x", Hw.App1Base);
      PrintLine("App2Base:", "%08x", Hw.App2Base);
      PrintLine("FpgaBase:", "%08x", Hw.FpgaBase);
      PrintLine("LogoBase:", "%08x", Hw.LogoBase);
      PrintLine("MaxLimit:", "%08x", Hw.MaxLimit);
    break;
*/
  }
}

void DumpAll()
{
  _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
  BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
  mWriter.Open((char*)"devinfo.txt");

  auto PrintLine = [&](const char* key, const char* format, ...)
  {
    char buf[64];
    
    va_list args;    
    va_start( args, format );
    vsprintf( buf, format, args );
    va_end(args);

    mWriter << key << " " << buf << "\r\n";
  };

  auto PrintSection = [](const char* section)
  {
    mWriter << section << "\n";
    mWriter << "==============" << "\r\n";
  };

  auto PrintEndSection = []()
  {
    mWriter << "\r\n";
  };

  PrintSection("Hardware");
  PrintLine("DFU version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionDfu));
  PrintLine("Hardware version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionHardware));
  PrintLine("System version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionSystem));
  PrintLine("FPGA version:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::VersionFpga));
  PrintLine("Display:", "%s (%dx%d)", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DisplayType), 
    BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenWidth),
    BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenHeight));
  PrintLine("Disk:", "%s (%dkB)", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskType), 
    BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize)*
    BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorCount)/1024);
  PrintEndSection();

  PrintSection("Software");
  PrintLine("BIOS version:", "%04x", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BiosVersion));
  PrintLine("Build rev:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildRevision));
  PrintLine("Build date:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildDate));
  PrintLine("Build user:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildUser));
  PrintLine("Build system:", "%s", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BuildSystem));
  PrintEndSection();

  PrintSection("License");
  PrintLine("Serial number:", "%08x", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::SerialNumber));
  PrintLine("License number:", "%08x", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::LicenseNumber));
  PrintLine("License valid:", "%d", BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::LicenseValid));
  PrintEndSection();

  mWriter.Close();
  BIOS::FAT::SetSharedBuffer(nullptr);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    constexpr int Width = 228;
    constexpr int Height = 120;
    int screenWidth = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenWidth);
    int screenHeight = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenHeight);

    CRect rcMain(screenWidth/2 - Width/2, screenHeight/2 - Height/2, screenWidth/2 + Width/2, screenHeight/2 + Height/2);
    Window(rcMain, RGB565(0080d0));

    if (BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::BiosVersion) < 0x0102)
    {
      BIOS::DBG::Print("Wrong bios version.\n");
      BIOS::SYS::DelayMs(1000);
      return 0;
    }

    int page = 0;
    RedrawPage(rcMain, page);

    DumpAll();

    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      if (key == KEY::Left && page > 0)
      {
        page--;
        WindowClear(rcMain);
        RedrawPage(rcMain, page);
      }
      if (key == KEY::Right && page < 2)
      {
        page++;
        WindowClear(rcMain);
        RedrawPage(rcMain, page);
      }
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed! in %s [%d]: ", file, line);
    BIOS::DBG::Print("%s\n", cond);
    while (1);
}
