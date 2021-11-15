#include "../../common/testbench.h"

void applyCrc(uint32_t& crc, int data)
{
    crc = crc ^ data;
    for (int j = 7; j >= 0; j--) 
    {
        uint32_t mask = -(crc & 1);
        crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
} 

bool verify()
{
    CFpgaTestBench test;

    int count = 512;
    uint32_t crc = 0xffffffff;

    test.SpiBegin();

    test.SpiCs(true);
    test.SpiTransfer(0); // skip two bytes
    test.SpiTransfer(0);
    for (int i=0; i<count; i++)
    {
      int ret = test.SpiTransfer(0);
      //BIOS::DBG::Print("%c", ret);
      applyCrc(crc, ret);
    }
    test.SpiCs(false);
    test.SpiEnd();
    return ~crc == 0x7d9a348a;
}
