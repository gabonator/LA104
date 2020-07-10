#include "STM32F10x.h"
#include "bios.h"

class CPRNG
{
private:
  unsigned int nSeed;
  unsigned int nX;

public:
  CPRNG() : nSeed(5323), nX(0)
  {
  }

  unsigned int get()
  {
    nSeed = (8253729 * nSeed + 2396403);
    nSeed += nX++;
    return nSeed & 32767;
  }
};

void HwInit()
{
  __Bios(PWRCTRL, INIT);        
  __Bios(KEYnDEV, INIT);        
  __Bios(NIVCPTR, 0x8000);      
  __Bios(BUZZDEV, INIT);        
  __Bios(BUZZDEV, 50);
  __Bios(FLSHDEV, INIT);        
  __Bios(USBDEV, INIT);         
  __Bios(IN_PORT, INIT);        
  __Bios(IN_PORT, 8000);
}

int main(void)
{ 
  CPRNG prng;
  HwInit();

  for (int x=0; x<320; x++)
    for (int y=0; y<240; y++)
    {
      u32 dx = (x-160);
      u32 dy = (y-120);
      dx *= dx;
      dy *= dy;
      u32 dl = dx+dy;

      Set_Posi(x, y);
      Set_Pixel((uc16)((dl>>3) & 0x001f)<<5);
    }

  int ax[3] = {10, 310, 140};
  int ay[3] = {20, 40, 220};

  int curx = ax[0];
  int cury = ay[0];
  while (1) 
  {
    int c = prng.get();
    int r = c%3;
    curx = (curx+ax[r]) / 2;
    cury = (cury+ay[r]) / 2;

    Set_Posi(curx, cury);
    Set_Pixel(c<<1);
  }

  return 0;
}
