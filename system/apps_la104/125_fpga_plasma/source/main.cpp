// Plasma animation app for LA104
// Flashes FPGA bitstream from filesystem, then runs the animation
//
// Place plasma.fpg alongside the .elf on device storage
// Controls: Up/Down = speed A, Left/Right = speed B, F1 = speed C, F3 = palette, F2 = exit

#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"
#include "fpga.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

// FPGA SPI commands
enum {
    FPGA_SET_COLOR   = 0x05,
    FPGA_SET_WIDTH   = 0x06,
    FPGA_SET_HEIGHT  = 0x07,
    FPGA_SET_CLKDIV  = 0x08,
    FPGA_SET_SPEED_A = 0x09,
    FPGA_SET_SPEED_B = 0x0A,
    FPGA_SET_SPEED_C = 0x0B,
    FPGA_START       = 0x10,
    FPGA_RELEASE     = 0x11,
};

CFpga fpga;

__attribute__((__section__(".entry")))
int _main(void)
{
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    APP::Init("Plasma Animation");
    APP::Status("Ud:A Lr:B F1:C F3:pal F2:exit");

    // Flash bitstream
    CONSOLE::Print("Flashing plasma.fpg...\n");
    CBufferedReader fw;
    if (!fw.Open((char*)"plasma.fpg"))
    {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("Cannot open plasma.fpg!\n");
        while (BIOS::KEY::GetKey() != BIOS::KEY::EKey::Escape);
        BIOS::FAT::SetSharedBuffer(nullptr);
        return 1;
    }

    if (!fpga.Flash(fw))
    {
        fw.Close();
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("FPGA configuration failed!\n");
        while (BIOS::KEY::GetKey() != BIOS::KEY::EKey::Escape);
        BIOS::FAT::SetSharedBuffer(nullptr);
        return 1;
    }
    fw.Close();

    CONSOLE::Color(RGB565(00ff00));
    CONSOLE::Print("FPGA configured!\n");
    CONSOLE::Color(RGB565(ffffff));

    // Re-init SPI3 for register access
    fpga.SpiInit();

    uint8_t ver = fpga.ReadVersion();
    CONSOLE::Print("Plasma v%d\n", ver);
    BIOS::SYS::DelayMs(500);

    // Set write window: (0,16)-(320,224) = 320x208
    CRect rc(0, 16, BIOS::LCD::Width, BIOS::LCD::Height - 16);
    BIOS::LCD::BufferBegin(rc);

    fpga.BusRelease();

    // Palette offsets: rotate hue in 45-degree steps
    static const uint8_t palettes[] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0};
    int pal = 0;

    // Speed registers (signed 8-bit, added to phase each frame)
    int8_t speed_a = 127;
    int8_t speed_b = 15;
    int8_t speed_c = -20;

    // Configure FPGA and start
    fpga.WriteReg(FPGA_SET_WIDTH, 320);
    fpga.WriteReg(FPGA_SET_HEIGHT, 208);
    fpga.WriteReg(FPGA_SET_COLOR, palettes[pal]);
    fpga.WriteReg(FPGA_SET_SPEED_A, (uint8_t)speed_a);
    fpga.WriteReg(FPGA_SET_SPEED_B, (uint8_t)speed_b);
    fpga.WriteReg(FPGA_SET_SPEED_C, (uint8_t)speed_c);
    fpga.WriteReg(FPGA_START, 0);

    // Up/Down = speed A, Left/Right = speed B, F1 = speed C, F3 = palette, F2 = exit
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (key == BIOS::KEY::None)
            continue;
        if (key == BIOS::KEY::Up)
        {
            speed_a += 10;
            fpga.WriteReg(FPGA_SET_SPEED_A, (uint8_t)speed_a);
            continue;
        }
        if (key == BIOS::KEY::Down)
        {
            speed_a -= 10;
            fpga.WriteReg(FPGA_SET_SPEED_A, (uint8_t)speed_a);
            continue;
        }
        if (key == BIOS::KEY::Right)
        {
            speed_b += 10;
            fpga.WriteReg(FPGA_SET_SPEED_B, (uint8_t)speed_b);
            continue;
        }
        if (key == BIOS::KEY::Left)
        {
            speed_b -= 10;
            fpga.WriteReg(FPGA_SET_SPEED_B, (uint8_t)speed_b);
            continue;
        }
        if (key == BIOS::KEY::F1)
        {
            speed_c += 10;
            fpga.WriteReg(FPGA_SET_SPEED_C, (uint8_t)speed_c);
            continue;
        }
        if (key == BIOS::KEY::F3)
        {
            pal = (pal + 1) % (int)(sizeof(palettes));
            fpga.WriteReg(FPGA_SET_COLOR, palettes[pal]);
            continue;
        }
        if (key == BIOS::KEY::F2)
            break;
    }

    // Stop and reclaim
    fpga.WriteReg(FPGA_RELEASE, 0);
    fpga.BusReclaim();

    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
