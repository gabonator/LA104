#include <library.h>

using namespace BIOS;

int counter = 0;

int main(void)
{
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        BIOS::DBG::Print("Test %d, ", counter++);
        BIOS::SYS::DelayMs(200);
    }
    
    return 0;
}