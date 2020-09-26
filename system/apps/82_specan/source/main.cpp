#include <library.h>
#include "srxe_specan.h"

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CRect rcClient(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
    GUI::Background(rcClient, RGB565(404040), RGB565(101010));
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "Spectrum analyser");

    setup();

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
//        long l0 = BIOS::SYS::GetTick();
        for (int i=0; i<20; i++)
            loop();
//        long l1 = BIOS::SYS::GetTick();
//        BIOS::LCD::Printf(0, BIOS::LCD::Height-16, RGB565(ffffff), RGB565(000000), "took %d ms...", l1-l0);
        handlekey(key);
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

