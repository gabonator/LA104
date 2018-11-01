#include "stm32f10x_lib.h"
#include "bios.h"

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

int main(void)
{ 
  int x, y;
  int ax[3] = {10, 310, 140};
  int ay[3] = {20, 40, 220};

  __Bios(PWRCTRL, INIT);        
  __Bios(KEYnDEV, INIT);        
  __Bios(NIVCPTR, 0x8000);      
  __Bios(BUZZDEV, INIT);        
  __Bios(BUZZDEV, 50);
  __Bios(FLSHDEV, INIT);        
  __Bios(USBDEV, INIT);         
  __Bios(IN_PORT, INIT);        
  __Bios(IN_PORT, 8000);

  for (x=0; x<320; x++)
    for (y=0; y<240; y++)
    {
      u32 dx = (x-160);
      u32 dy = (y-120);
      dx *= dx;
      dy *= dy;
      u32 dl = dx+dy;

      Set_Posi(x, y);
      Set_Pixel((uc16)(dl>>3) & 0x001f);

    }

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
