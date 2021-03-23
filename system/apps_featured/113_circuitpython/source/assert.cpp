#include <library.h>
#include "assert.h"

extern "C" void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed");
    while (1);

}
