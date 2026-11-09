#include <library.h>
#include "platform.h"

static volatile uint32_t* g_pKeyMask;

namespace PF
{
    void Begin(int x, int y, int w, int h)
    {
        BIOS::LCD::BufferBegin(CRect(x, y, x + w, y + h));
    }
    void Write(const uint16_t* p, int n)
    {
        BIOS::LCD::BufferWrite(p, n);
    }
    void End()
    {
        BIOS::LCD::BufferEnd();
    }
    void Bar(int x1, int y1, int x2, int y2, uint16_t clr)
    {
        BIOS::LCD::Bar(x1, y1, x2, y2, clr);
    }
    int Print(int x, int y, uint16_t fg, uint16_t bg, const char* text)
    {
        return BIOS::LCD::Print(x, y, fg, bg, text);
    }
    uint32_t Keys()
    {
        return g_pKeyMask ? *g_pKeyMask : 0;
    }
    uint32_t Tick()
    {
        return BIOS::SYS::GetTick();
    }
    void Beep(int ms)
    {
        BIOS::SYS::Beep(ms);
    }
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    g_pKeyMask = (volatile uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::KeyMask);
    BIOS::LCD::Clear(RGB565(000000));
    GameMain();
    BIOS::LCD::Clear(RGB565(000000));
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
