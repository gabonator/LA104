#include "../../common/testbench.h"

 
class Array : public CArray<uint8_t>
{
  uint8_t mData[8];
public:
  Array() : CArray(mData, 8)
  {
  }

  Array(uint8_t a) : CArray(mData, 1, 1)
  {
    mData[0] = a;
  }

  Array(uint8_t a, uint8_t b, uint8_t c, uint8_t d) : CArray(mData, 4, 4)
  {
    mData[0] = a;
    mData[1] = b;
    mData[2] = c;
    mData[3] = d;
  }

  Array(uint8_t a, uint8_t b, uint8_t c, uint8_t d, 
      uint8_t e, uint8_t f, uint8_t h, uint8_t g) : CArray(mData, 8, 8)
  {
    mData[0] = a;
    mData[1] = b;
    mData[2] = c;
    mData[3] = d;
    mData[4] = e;
    mData[5] = f;
    mData[6] = g;
    mData[7] = h;
  }
};

bool Check(CFpgaTestBench& test, const Array& arrSend, const Array& arrCheck)
{
    Array arrReturn;
    test.SpiTransfer(arrSend, arrReturn);

    if (!(arrReturn == arrCheck))
    {
        BIOS::DBG::Print("(ret:");
        for (int i=0; i<arrReturn.GetSize(); i++)
            BIOS::DBG::Print(i > 0 ? ", %02x" : "%02x", arrReturn[i]);
        BIOS::DBG::Print(")\n");
        return false;
    }
    return true;
}

bool verify()
{
    enum { Write = 1, Read = 2, Invalid = 0x00 };

    CFpgaTestBench test;

    Array dummyOut(0xff, 0xff, 0xff, 0xff);
    Array dummyIn;
    test.SpiTransfer(dummyOut, dummyIn);

    if (!Check(test, Array(0x12, 0x34, 0x56, 0x78), Array(Invalid, 0x77, 0xff, 0xff)))
        return false;
    // Write bytes: 0x34, 0x56, 0x78
    if (!Check(test, Array(Write, 0x34, 0x56, 0x78), Array(Invalid, 0x77, 0x00, 0x00)))
        return false;
    // Read bytes: 0x34, 0x56, 0x78                         
    if (!Check(test, Array(Read, 0xaa, 0xbb, 0xcc), Array(Invalid, 0x34, 0x56, 0x78)))
        return false;

    // Write long sequence
    if (!Check(test, Array(Write, 5, 6, 3, 2, 9, 4, 7), Array(Invalid, 0x77, 0, 0, 0, 0, 0, 0)))
        return false;
    if (!Check(test, Array(Read, 0, 0, 0, 0, 0, 0, 0), Array(Invalid, 5, 6, 3, 2, 9, 4, 7)))
        return false;

    // Overwrite first 3 bytes
    if (!Check(test, Array(Write, 13, 14, 15), Array(Invalid, 0x77, 0, 0)))
        return false;
    if (!Check(test, Array(Read, 0, 0, 0, 0, 0, 0, 0), Array(Invalid, 13, 14, 15, 2, 9, 4, 7)))
        return false;

    return true;
}