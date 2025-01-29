#include <library.h>
#include "../../../os_host/source/framework/BufferedIo.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

CBufferedReader fw;

void start();
#ifdef LA104
void WriteTim8(int reg, uint16_t val)
{
  *((uint16_t*)(0x40013400 + reg)) = val;
}

void SoundOn()
{
  WriteTim8(0x20, 0x3000);
  WriteTim8(0x00, 0x0081);
  WriteTim8(0x40, 50); // volume
}

void SoundOff()
{
  WriteTim8(0x20, 0x0000);
  WriteTim8(0x00, 0x0080);
}

void Sound(int f)
{
    if (!f)
    {
        SoundOff();
        return;
    }
    WriteTim8(0x24, 0);
    WriteTim8(0x28, 15-1);
    int div = 72e6/15/f;
    WriteTim8(0x2c, div-1);
    SoundOn();
}
#else
void Sound(int)
{
}
#endif
void PlayTone(int f)
{
    Sound(f);
}

void SetPixel(int x, int y, int c)
{
    static const uint16_t colors[] = { RGB565(000000), RGB565(55ffff), RGB565(ff55ff), RGB565(ffffff) };
    BIOS::LCD::PutPixel(x, y+20, colors[c]);
}

uint32_t GetTick()
{
    return BIOS::SYS::GetTick();
}

void Load(uint8_t* buf, int ofs, int size)
{
#ifdef __APPLE__
    printf("Read %04x,%x\n", ofs, size);
    FILE* f = fopen("/Users/gabrielvalky/Documents/git/Projects/CicoJit/gamelib/alleycat/dos/CAT.EXE", "rb");
    fseek(f, 0x300+ofs, SEEK_SET);
    fread(buf, size, 1, f);
    fclose(f);
#else
    fw.Seek(0x300+ofs);
    fw >> CStream(buf, size);
#endif
}

bool Sync()
{
    return BIOS::KEY::GetKey() != BIOS::KEY::None;
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#ifdef __APPLE__
    BIOS::SYS::DelayMs(1000);
#else
    if (!fw.Open((char*)"alleycat.exe"))
    {
        BIOS::DBG::Print("Cannot open alleycat.exe!");
        return false;
    }
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    BIOS::LCD::Bar(CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), RGB565(ff55ff));
    start();
    fw.Close();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}

void _HandleAssertion1(const char* file, int line, const char* cond)
{
    _HandleAssertion(file, line, cond);
}

