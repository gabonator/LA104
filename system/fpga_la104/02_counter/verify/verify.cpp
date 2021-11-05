#include "../../common/testbench.h"

bool verify()
{
    CFpgaTestBench test;

    uint8_t arrSendData[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    CArray<uint8_t> arrSend(arrSendData, COUNT(arrSendData), COUNT(arrSendData));

    uint8_t arrVerifyData[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    CArray<uint8_t> arrVerify(arrVerifyData, COUNT(arrVerifyData), COUNT(arrVerifyData));

    uint8_t arrReturnData[COUNT(arrSendData)];
    CArray<uint8_t> arrReturn(arrReturnData, COUNT(arrReturnData));

    // first transfer is corrupted on real device for unknown reason (ff at beginning)
    test.SpiTransfer(arrSend, arrReturn);

    for (int i=1; i<COUNT(arrSendData); i++)
    {
        arrSend.SetSize(i);
        arrReturn.SetSize(i);
        arrVerify.SetSize(i);

        test.SpiTransfer(arrSend, arrReturn);

        if (!(arrReturn == arrVerify))
        {
          BIOS::DBG::Print("Pass %d failed: ", i);
          for (int j=0; j<i; j++)
             BIOS::DBG::Print("%02x, ", arrReturn[j]);
          BIOS::DBG::Print("\n");
          return false;
        }
    }

    return true;
}