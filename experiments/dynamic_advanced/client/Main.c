#include "library.h"
#include <stdint.h>

int main(void)
{ 
  int x, y;
  int ax[3] = {10, 310, 140};
  int ay[3] = {20, 40, 220};

  for (x=0; x<320; x++)
    for (y=0; y<240; y++)
    {
      uint32_t dx = (x-160);
      uint32_t dy = (y-120);
      dx *= dx;
      dy *= dy;
      uint32_t dl = dx+dy;
      PutPixel(x, y, ((dl>>3) & 0x001f)<<11);
    }

  int curx = ax[0];
  int cury = ay[0];
  while (1) 
  {
    int c = random();
    int r = c%3;
    curx = (curx+ax[r]) / 2;
    cury = (cury+ay[r]) / 2;

    PutPixel(curx, cury, c << 1);
  }

  return 0;
}
