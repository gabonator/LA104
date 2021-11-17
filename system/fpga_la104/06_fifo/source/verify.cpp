#include "../../common/testbench.h"


bool getToken(const char*& cmd, char* signal, int& value, bool& done)
{
  while (*cmd == ' ')
    cmd++;
  if (*cmd==';')
  {
    cmd++;
    done = true;
    return true;
  }
  if (cmd[0] == 0)
    return false;

  int i;
  while (*cmd != '=')
    *signal++ = *cmd++;
  *signal = 0;
  cmd++;
  if (*cmd == '0')
    value = 0;
  else if (*cmd == '1')
    value = 1;
  else 
    _ASSERT(0);
  cmd++;
  if (*cmd == 0)
    return false;
  else if (*cmd == ',')
    done = false;
  else if (*cmd == ';')
    done = true;
  else
    _ASSERT(0);
  cmd++;
  return true;
}

void print(CFpgaTestBench& test)
{
  if (test.CLK() == 0)
    BIOS::DBG::Print("  [clk=%d]", test.CLK());
  else
    BIOS::DBG::Print("  [ss=%d, miso=%d, mosi=%d, sck=%d, clk=%d]  ",
      test.SS(), test.MISO(), test.MOSI(), test.SCK(), test.CLK());
}

void eval(CFpgaTestBench& test, const char* cmd, bool quiet = false)
{
  char signal[8];
  int value;
  bool done;

  BIOS::DBG::Print("%s\n", cmd);

  while (getToken(cmd, signal, value, done))
  {
    if (strcmp(signal, "sck") == 0) 
      test.SCK() = value;
    else if (strcmp(signal, "miso") == 0) 
      test.MISO() = value;
    else if (strcmp(signal, "mosi") == 0) 
      test.MOSI() = value;
    else if (strcmp(signal, "ss") == 0) 
      test.SS() = value;
    else
      _ASSERT(0);

    if (done)
    {
      if (!quiet) print(test);
      test.eval();
      test.CLK() = 1;

      if (!quiet) print(test);
      test.eval();
      if (!quiet) BIOS::DBG::Print("\n");
      test.CLK() = 0;

      if (!quiet) print(test);
      test.eval();
      test.CLK() = 1;

      if (!quiet) print(test);
      test.eval();
      if (!quiet) BIOS::DBG::Print("\n");
      test.CLK() = 0;

      if (!quiet) print(test);
      test.eval();
      test.CLK() = 1;

      if (!quiet) print(test);
      test.eval();
      if (!quiet) BIOS::DBG::Print("\n");
      test.CLK() = 0;

      test.eval();
    }
  }
  BIOS::DBG::Print(" <%d>\n", test.MISO());
}

bool verify0()
{
    enum { Write = 1, Read = 2, Invalid = 0x00 };

    CFpgaTestBench test;
    eval(test, "sck=0, mosi=0, miso=0, ss=1;", true);
    eval(test, "ss=0;", true); // active
    // first byte
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    BIOS::DBG::Print("============\n");
    // second
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    eval(test, "mosi=0,sck=1;sck=0;", true);
    eval(test, "mosi=1,sck=1;sck=0;", true);
    BIOS::DBG::Print("============\n");

    return true;
}
 
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

  Array(uint8_t a, uint8_t b) : CArray(mData, 2, 2)
  {
    mData[0] = a;
    mData[1] = b;
  }

  Array(uint8_t a, uint8_t b, uint8_t c, uint8_t d) : CArray(mData, 4, 4)
  {
    mData[0] = a;
    mData[1] = b;
    mData[2] = c;
    mData[3] = d;
  }

  Array(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) : CArray(mData, 5, 5)
  {
    mData[0] = a;
    mData[1] = b;
    mData[2] = c;
    mData[3] = d;
    mData[4] = e;
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
    //if (!(arrReturn == arrCheck))
    {
        BIOS::DBG::Print("(ret:");
        for (int i=0; i<arrReturn.GetSize(); i++)
            BIOS::DBG::Print(i > 0 ? ", %02x" : "%02x", arrReturn[i]);
        BIOS::DBG::Print(")\n");
//        return false;
    }
    return arrReturn == arrCheck;
}

bool verify()
{
    CFpgaTestBench test;
/*
    Array dummyOut(0xee);
    Array dummyIn;
    test.SpiTransfer(dummyOut, dummyIn);
*/
    // push byte 0x33
    if (!Check(test, 
          Array(0x01, 0x33), 
          Array(0x33, 0xbb)))
        return false;

    // push byte 0x34
    if (!Check(test, 
          Array(0x01, 0x34), 
          Array(0x33, 0xbb)))
        return false;

    // push byte 0x35
    if (!Check(test, 
          Array(0x01, 0x35), 
          Array(0x33, 0xbb)))
        return false;


    // pop bytes 0x33, 0x34, 0x35
    if (!Check(test, 
          Array(0x02, 0x00, 0x00, 0x00), 
          Array(0x33, 0x33, 0x34, 0x35)))
        return false;

    // identify
    if (!Check(test, 
          Array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00), 
          Array(0x33, 0x47, 0x61, 0x62, 0x6f, 0x6e, 0x74, 0x61)))
        return false;

    return true;
}