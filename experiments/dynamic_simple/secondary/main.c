#include "STM32F10x.h"
#include "bios.h"

void Test();

void WriteMem(uint8_t* buf, int len, uint32_t addr)
{
  uint8_t* pdest = (uint8_t*)addr;
  while (len--)
    *pdest++ = *buf++;
}

void WriteDword(uint32_t addr, uint32_t value)
{
  uint32_t* pdest = (uint32_t*)addr;
  *pdest = value;
}

void ExecEntry(uint32_t addr)
{
  typedef int (*TInitFunc)(void);
  TInitFunc InitFunc = (TInitFunc)addr;
  InitFunc();
}

void experiment()
{
  // .text
  uint8_t data_20005000[0x16] = {
    0x08, 0xb5, 0x00, 0xf0, 0x6d, 0xf8, 0x00, 0xf0, 0x63, 0xf8, 0x00, 0xf0, 0x79, 0xf8, 0x00, 0xf0, 
    0x6f, 0xf8, 0x01, 0x20, 0x08, 0xbd, 
  };
  WriteMem(data_20005000, 0x16, 0x20005000);
  // .plt
  uint8_t data_200050c0[0x50] = {
    0x00, 0xb5, 0xdf, 0xf8, 0x08, 0xe0, 0xfe, 0x44, 0x5e, 0xf8, 0x08, 0xff, 0xec, 0x00, 0x00, 0x00, 
    0x40, 0xf2, 0xe8, 0x0c, 0xc0, 0xf2, 0x00, 0x0c, 0xfc, 0x44, 0xdc, 0xf8, 0x00, 0xf0, 0x00, 0xbf, 
    0x40, 0xf2, 0xdc, 0x0c, 0xc0, 0xf2, 0x00, 0x0c, 0xfc, 0x44, 0xdc, 0xf8, 0x00, 0xf0, 0x00, 0xbf, 
    0x40, 0xf2, 0xd0, 0x0c, 0xc0, 0xf2, 0x00, 0x0c, 0xfc, 0x44, 0xdc, 0xf8, 0x00, 0xf0, 0x00, 0xbf, 
    0x40, 0xf2, 0xc4, 0x0c, 0xc0, 0xf2, 0x00, 0x0c, 0xfc, 0x44, 0xdc, 0xf8, 0x00, 0xf0, 0x00, 0xbf, 
  };
  WriteMem(data_200050c0, 0x50, 0x200050c0);
  // .got
  uint8_t data_200051b8[0x1c] = {
    0x18, 0x50, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x00, 0x20, 
    0xc0, 0x50, 0x00, 0x20, 0xc0, 0x50, 0x00, 0x20, 0xc0, 0x50, 0x00, 0x20, 
  };
  WriteMem(data_200051b8, 0x1c, 0x200051b8);
  // Relocation table
  WriteDword(0x200051c4, (uint32_t)Test);
  WriteDword(0x200051c8, (uint32_t)Test);
  WriteDword(0x200051cc, (uint32_t)Test);
  WriteDword(0x200051d0, (uint32_t)Test);
  ExecEntry(0x20005001); // !!! +1 for thumb function!
}

unsigned int PRNG()
{
    // our initial starting seed is 5323
    static unsigned int nSeed = 5323;
    static unsigned int nX = 0;

    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);
    nSeed += nX++;

    // Take the seed and return a value between 0 and 32767
    return nSeed & 32767;
}

void Clear()
{
  int x, y;

  for (x=0; x<320; x++)
    for (y=0; y<240; y++)
    {
      Set_Posi(x, y);
      Set_Pixel(0x5555);
    }
}

void Test()
{
  int x, y;

  for (x=0; x<320; x++)
    for (y=0; y<240; y++)
    {
      u32 dx = (x-160);
      u32 dy = (y-120);
      dx *= dx;
      dy *= dy;
      u32 dl = dx+dy;

      Set_Posi(x, y);
      Set_Pixel((uc16)((dl>>3) & 0x001f)<<11);
    }
}

int main(void)
{ 
  Clear();
  experiment();

  int ax[3] = {10, 310, 140};
  int ay[3] = {20, 40, 220};

  int curx = ax[0];
  int cury = ay[0];
  while (1) 
  {
    int c = PRNG();
    int r = c%3;
    curx = (curx+ax[r]) / 2;
    cury = (cury+ay[r]) / 2;

    Set_Posi(curx, cury);
    Set_Pixel(c<<1);
  }

  return 0;
}
