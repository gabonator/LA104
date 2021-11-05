#include "../../common/testbench.h"

bool Wait(CFpgaTestBench& test, bool b)
{
    while (test.GetMiso() == b)
    {
        if (test.Timeout())
            return false;

        BIOS::SYS::DelayMs(2);
    }
    return true;
}

bool verify()
{
    CFpgaTestBench test;
    // FPGA should generate square signal with 1s toggle period

    Wait(test, 0);
    Wait(test, 1);
    int32_t t0 = BIOS::SYS::GetTick();
    Wait(test, 0);
    int32_t t1 = BIOS::SYS::GetTick();
    Wait(test, 1);
    int32_t t2 = BIOS::SYS::GetTick();

    int32_t durationLow = t1-t0;
    int32_t durationHigh = t2-t1;

    // will return 932/932ms on ARM
    // returns 1048/1048 on verilator
    if (abs(durationLow-1000) > 100 || abs(durationHigh-1000) > 100)
    {
        BIOS::DBG::Print("Test failed: durationLow=%d, durationHigh=%d\n", 
            durationLow, durationHigh);
        return false;
    }
    return true;
}
