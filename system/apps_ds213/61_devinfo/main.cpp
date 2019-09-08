#include <library.h>
#include "info.h"
#include "ds203.h"
#include "ds213.h"
#include "la104.h"

	void Window(const CRect& rcRect, uint16_t clr)
	{
		CRect rcClient = rcRect;
		BIOS::LCD::RoundRect(rcClient, RGB565(000000));
		rcClient.Deflate(2, 2, 2, 2);
		BIOS::LCD::RoundRect(rcClient, clr);
		rcClient.Deflate(2, 14, 2, 2);
		BIOS::LCD::RoundRect(rcClient, RGB565(404040));
	}

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    constexpr int Width = 220;
    constexpr int Height = 120;
    constexpr int Line = 14;
    CRect rcMain(BIOS::LCD::Width/2 - Width/2, BIOS::LCD::Height/2 - Height/2, BIOS::LCD::Width/2 + Width/2, BIOS::LCD::Height/2 + Height/2);
    Window(rcMain, RGB565(0080d0));

    uint8_t placement[512];

    CDeviceInfo* devInfo = nullptr;
    char* id = BIOS::SYS::GetDeviceType();
    if (strcmp(id, "LA104") == 0)
      devInfo = new(placement) CLA104();
    if (strcmp(id, "DS203") == 0)
      devInfo = new(placement) CDS203();
    if (strcmp(id, "DS213") == 0)
      devInfo = new(placement) CDS213();

    if (!devInfo)
    {
      BIOS::DBG::Print("Unsupported device!\n");
      BIOS::SYS::DelayMs(1000);
      return 1;
    }

    CPoint cpDraw(rcMain.left + 8, rcMain.top + 16 + 2);

    BIOS::LCD::Printf(rcMain.left +  8, rcMain.top + 2, RGB565(002040), RGBTRANS, "%s", id);
    BIOS::LCD::Printf(rcMain.left + 56, rcMain.top + 2, RGB565(ffffff), RGB565(2fafff), " Hardware ");
    BIOS::LCD::Printf(rcMain.left +140, rcMain.top + 2, RGB565(002040), RGBTRANS, "Firmware");

    CPoint cpLine = cpDraw; 
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "DFU version:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s", devInfo->DfuVersion());
    cpDraw.y += Line;
    cpLine = cpDraw;
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "Hardware version:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s", devInfo->HardwareVersion());
    cpDraw.y += Line;
    cpLine = cpDraw;
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "System version:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s", devInfo->SystemVersion());
    cpDraw.y += Line;
    cpLine = cpDraw;
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "FPGA version:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s", devInfo->FpgaVersion());
    cpDraw.y += Line;
    cpLine = cpDraw;
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "Serial number:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s", devInfo->SerialNumber());
    cpDraw.y += Line;
    cpLine = cpDraw;
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "Display:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s (%dx%d)", devInfo->DisplayType(), BIOS::LCD::Width, BIOS::LCD::Height);
    cpDraw.y += Line;
    cpLine = cpDraw;
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(b0b0b0), RGBTRANS, "Disk:");
    cpLine.x += BIOS::LCD::Printf(cpLine.x, cpLine.y, RGB565(ffffff), RGBTRANS, "%s (%dkB)", devInfo->DiskType(), BIOS::FAT::SectorSize*BIOS::FAT::SectorCount/1024);

    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
    }
    return 0;
}
