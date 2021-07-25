#include <library.h>

__attribute__((__section__(".entry")))
int main(void)
{
    BIOS::DBG::Print("Hello world!\n");
    return 724;
}
