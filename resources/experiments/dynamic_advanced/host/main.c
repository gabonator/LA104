#include "STM32F10x.h"
#include <stdint.h>
#include "bios.h"

void PutPixel(uint16_t x, uint16_t y, uint16_t c);
uint32_t random();

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
  #include "code.h"
}

uint32_t random()
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

int main(void)
{ 
  experiment();
  return 0;
}
