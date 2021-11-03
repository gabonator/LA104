#include "../../common/testbench.h"

bool verify()
{
    CFpgaTestBench test;

    int fib_a = 0;
    int fib_b = 1;
    int count = 1000;
    bool ok = true;

    test.SpiBegin();

    // drop first byte (0xff)
    test.SpiCs(true);
    test.SpiTransfer(0);
    test.SpiCs(false);

    test.SpiCs(true);
    for (int i=0; i<count; i++)
    {
      int ret = test.SpiTransfer(0);
      if (ret != fib_a)
      {
        ok = false;
        BIOS::DBG::Print("Failed on %d. step: (%02x != %02x)\n", i, ret, fib_a);
        break;
      }
      int temp = fib_a + fib_b;
      fib_a = (uint8_t)fib_b;
      fib_b = temp;
    }
    test.SpiCs(false);
    test.SpiEnd();

    return ok;
}