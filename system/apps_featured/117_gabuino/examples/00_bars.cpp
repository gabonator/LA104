#include <library.h>

using namespace BIOS;

int main(void)
{
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        int x1 = rand() % BIOS::LCD::Width;
        int y1 = (rand() % (BIOS::LCD::Height-28)) + 14;
        int x2 = rand() % BIOS::LCD::Width;
        int y2 = (rand() % (BIOS::LCD::Height-28)) + 14;
        int c = rand() & 0xffff;
        if (x2 > x1 && y2 > y1)
            BIOS::LCD::Bar(x1, y1, x2, y2, c);
    }
    
    return 0;
}
