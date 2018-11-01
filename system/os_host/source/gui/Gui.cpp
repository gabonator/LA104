#include "Gui.h"

namespace GUI
{
  void Background(CRect& rc, uint32_t clr1, uint32_t clr2)
  {
    static int8_t randpat[8][8];
    static bool randpatinit = true;
    if (randpatinit)
    {
        randpatinit = false;
        for (int y=0; y<8; y++)
            for (int x=0; x<8; x++)
                randpat[y][x] = (x+y)%8-3;
    }

    uint16_t pat[8];
    
    int r1 = Get565R(clr1);
    int g1 = Get565G(clr1);
    int b1 = Get565B(clr1);
    int r2 = Get565R(clr2);
    int g2 = Get565G(clr2);
    int b2 = Get565B(clr2);
    bool interpolate = clr1 != clr2;
    CRect rect(rc);
    rect.bottom = rect.top + 1;
    int w = rect.Width();

    for (int y=rc.top; y<rc.bottom; y++)
    {
        int r = r1;
        int g = g1;
        int b = b1;

        if (interpolate)
        {
          r += (r2-r1)*y/239;
          g += (g2-g1)*y/239;
          b += (b2-b1)*y/239;
        }
        
        for (int x=0; x<8; x++)
        {
            int _r = r + randpat[x%8][y%8];
            int _g = g + randpat[(x+3)%8][y%8];
            int _b = b + randpat[x%8][(y+5)%8];
            _r = max(0, min(_r, 255));
            _g = max(0, min(_g, 255));
            _b = max(0, min(_b, 255));
            pat[x] = RGB565RGB(_r, _g, _b);
        }
        BIOS::LCD::BufferBegin(rect, 0);
        rect.top++;
        rect.bottom++;

        for (int x=0; x<w; x++)
        {
          BIOS::LCD::BufferPush(pat[x&7]);
        }
    }
  }

  void Window(const CRect& rcRect, uint16_t clr)
  {
    CRect rcClient = rcRect;
    BIOS::LCD::RoundRect(rcClient, RGB565(000000));
    rcClient.Deflate(2, 2, 2, 2);
    BIOS::LCD::RoundRect(rcClient, clr);
    rcClient.Deflate(2, 14, 2, 2);
    BIOS::LCD::RoundRect(rcClient, RGB565(b0b0b0));
  }
}
