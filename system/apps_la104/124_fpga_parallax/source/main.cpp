// Standalone parallax animation app for LA104
// Flashes FPGA bitstream from filesystem, then runs the animation
// No Gabuino dependency — runs directly from BIOS shell
//
// Place parallax.fpg alongside the .elf on device storage
// Controls: F1 = speed, F3 = palette, F2/Escape = exit

#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"
#include "fpga.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

// FPGA SPI commands
enum {
    FPGA_CMD_SET_COLOR  = 0x05,
    FPGA_CMD_SET_WIDTH  = 0x06,
    FPGA_CMD_SET_HEIGHT = 0x07,
    FPGA_CMD_SET_CLKDIV = 0x08,
    FPGA_CMD_START      = 0x10,
    FPGA_CMD_RELEASE    = 0x11,
};

CFpga fpga;

__attribute__((__section__(".entry")))
int _main(void)
{
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    APP::Init("Parallax Animation");
    APP::Status("F1:speed F3:palette F2:exit");

    // Flash bitstream
    CONSOLE::Print("Flashing parallax.fpg...\n");
    CBufferedReader fw;
    if (!fw.Open((char*)"parallax.fpg")) {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("Open failed!\n");
        while (BIOS::KEY::GetKey() != BIOS::KEY::EKey::Escape);
        BIOS::FAT::SetSharedBuffer(nullptr);
        return 1;
    }

    if (!fpga.Flash(fw))
    {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("FPGA configuration failed!\n");
        while (BIOS::KEY::GetKey() != BIOS::KEY::EKey::Escape);
        BIOS::FAT::SetSharedBuffer(nullptr);
        return 1;
    }

    CONSOLE::Color(RGB565(00ff00));
    CONSOLE::Print("FPGA configured!\n");
    CONSOLE::Color(RGB565(ffffff));

    // Re-init SPI3 for register access
    fpga.SpiInit();

    uint8_t ver = fpga.ReadVersion();
    CONSOLE::Print("FPGA version: %d\n", ver);
    CONSOLE::Print("Starting animation...\n");
    BIOS::SYS::DelayMs(500);

    // Set LCD write window: skip top/bottom rows for status bars
    CRect rc(0, 16, BIOS::LCD::Width, BIOS::LCD::Height - 14);
    BIOS::LCD::BufferBegin(rc);

    fpga.BusRelease();

    // Animation parameters
    static const uint8_t palettes[] = {0x00, 0x15, 0x2A, 0x3F, 0x30, 0x0C, 0x03, 0x21};
    int pal = 0;
    int clkdiv = 3;

    fpga.WriteReg(FPGA_CMD_SET_WIDTH, 320);
    fpga.WriteReg(FPGA_CMD_SET_HEIGHT, BIOS::LCD::Height - 16 - 14);
    fpga.WriteReg(FPGA_CMD_SET_COLOR, palettes[pal]);
    fpga.WriteReg(FPGA_CMD_SET_CLKDIV, clkdiv);
    fpga.WriteReg(FPGA_CMD_START, 0);

    // Main loop: F1=speed, F3=palette, F2/Escape=exit
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (key == BIOS::KEY::None)
            continue;
        if (key == BIOS::KEY::F1)
        {
            clkdiv = (clkdiv + 1) % 4;
            fpga.WriteReg(FPGA_CMD_SET_CLKDIV, clkdiv);
            continue;
        }
        if (key == BIOS::KEY::F3)
        {
            pal = (pal + 1) % (int)(sizeof(palettes));
            fpga.WriteReg(FPGA_CMD_SET_COLOR, palettes[pal]);
            continue;
        }
        if (key == BIOS::KEY::F2)
            break;
    }

    fpga.WriteReg(FPGA_CMD_RELEASE, 0);
    fpga.BusReclaim();

    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
