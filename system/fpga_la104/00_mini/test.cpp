#include <verilated.h>
#include "Vapp.h"
#include "../common/testbench2.h"

bool main_verify(CTestBench& test)
{
    test.spiBegin(false);
    test.cs(1);
    test.tick();
    test.tick();
    test.tick();
    test.cs(0);
    test.tick();
    test.tick();
    test.tick();
    
    char temp[32];
    int read;
    
    test.groupBegin("write 0xf0");
    read = test.spiTransfer(0xf0);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);
    
    test.groupBegin("write 0xf1");
    read = test.spiTransfer(0xf1);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);
    
    test.groupBegin("write 0xf2");
    read = test.spiTransfer(0xf2);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0x73");
    read = test.spiTransfer(0x73);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0x48");
    read = test.spiTransfer(0x48);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0xa9");
    read = test.spiTransfer(0xa9);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0xf5");
    read = test.spiTransfer(0x48);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);
    
    test.eval();
    test.cs(1);
    test.eval();
    test.printLabel("release");
    test.tick();
    test.tick();
    test.tick();
    test.spiEnd();
    return true;
}
