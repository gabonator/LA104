#include <library.h>

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

__attribute__((__section__(".entry"))) int main(void);

int main(void)
{ 
  CPRNG prng;

  for (int x=0; x<320; x++)
    for (int y=0; y<240; y++)
    {
      uint32_t dx = (x-160);
      uint32_t dy = (y-120);
      dx *= dx;
      dy *= dy;
      uint32_t dl = dx+dy;

      BIOS::LCD::PutPixel(x, y, (uint16_t)((dl>>3) & 0x001f)<<11);
    }

  int ax[3] = {10, 310, 140};
  int ay[3] = {20, 40, 220};

  int curx = ax[0];
  int cury = ay[0];

  while (BIOS::KEY::GetKey() == BIOS::KEY::None) 
  {
    int c = prng.get();
    int r = c%3;
    curx = (curx+ax[r]) / 2;
    cury = (cury+ay[r]) / 2;

    BIOS::LCD::PutPixel(curx, cury, c<<1);
  }

  return 0;
}
