#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/Serialize.h"
#include "../../../os_host/source/framework/BufferedIo.h"
#include "ps2/keyboard.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

static const char CShapes_sel_left[] =
"\x0e"
"  ..........  "
" ............ "
".............."
"..............";

static const char CShapes_sel_right[] =
"\x0e"
//".............."
".............."
" ............ "
"  ..........  ";


struct char_t
{
    int x;
    int y;
    const char* c;
    int len;
    int scancode;
    
    void Draw(int clrFront, int clrBack)
    {
        int xx = 10+x*8-4;
        xx += BIOS::LCD::Draw(xx, 40+y*20, clrBack, RGBTRANS, CShapes_sel_left);
        for (int i=0; i<len; i++)
            xx += BIOS::LCD::Print(xx, 40+y*20, clrFront, clrBack, c[i]);
        BIOS::LCD::Draw(xx, 40+y*20, clrBack, RGBTRANS, CShapes_sel_right);
    }
};

#pragma pack(push)
#pragma pack(2)
struct BmpHdr
{
    uint16_t wBfType;
    uint32_t dwBfSize;
    uint16_t wBfReserved1;
    uint16_t wBfReserved2;
    uint32_t dwBfOffset;
    
    uint32_t dwBiSize;
    uint32_t dwBiWidth;
    uint32_t dwBiHeight;
    uint16_t wBiPlanes;
    uint16_t wBiBitDepth;
    uint32_t dwBiCompression;
    uint32_t dwBiSizeImage;
    uint32_t dwBiXPels;
    uint32_t dwBiYPels;
    
    uint32_t dwBiClrUsed;
    uint32_t dwBiClrImportant;
};
#pragma pack(pop)


char_t arrCharsData[64];
CArray<char_t> arrChars(arrCharsData, COUNT(arrCharsData));
CKeyboard keyboard;

template <typename T> T binary_search(CArray<T>& arr, int target) {
    int low = 0;
    int high = arr.GetSize() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Avoid overflow

        if (arr[mid].scancode == target) {
            return arr[mid];  // Element found
        } else if (arr[mid].scancode < target) {
            low = mid + 1;  // Search in the right half
        } else {
            high = mid - 1; // Search in the left half
        }
    }

    return T{0};  // Element not found
}

class CApp {
    int mLeds{0};
    int mMode{1};
    bool mNeedRedraw{false};
    
public:
    void UpdateLeds()
    {
//        BIOS::DBG::Print("1");
        if (!keyboard.Write(0xED))
            return;
        BIOS::SYS::DelayMs(10);
//        BIOS::DBG::Print("2");
        if (!keyboard.WaitAck()) // ACK=00
            ;//return;
        BIOS::SYS::DelayMs(10);
//        BIOS::DBG::Print("3");

        if (!keyboard.Write(mLeds))
            return;
        BIOS::SYS::DelayMs(10);
//        BIOS::DBG::Print("4");

        if (!keyboard.WaitAck())
            return;
//        BIOS::DBG::Print("5");

    }
    bool RenderImage(char* path)
    {
        CBufferedReader reader;
        if (!reader.Open(path))
            return false;

        BmpHdr header;
        reader >> CStream(&header, sizeof(header));
        if (header.wBiBitDepth != 4)
        {
            reader.Close();
            return false;
        }
        
        uint16_t palette[16];
        uint8_t rgb[4];
        for (int i=0; i<16; i++)
        {
            reader >> CStream(rgb, 4);
            palette[i] = RGB565RGB(rgb[2], rgb[1], rgb[0]);
        }
        
        reader.Seek(header.dwBfOffset);
        int rowSize = ((header.dwBiWidth * header.wBiBitDepth + 31) / 32) * 4;
        
        int bx = 0, by = 0;
        CRect rcImage(
            (BIOS::LCD::Width - header.dwBiWidth)/2,
            (BIOS::LCD::Height - header.dwBiHeight)/2,
            (BIOS::LCD::Width - header.dwBiWidth)/2+header.dwBiWidth,
            (BIOS::LCD::Height - header.dwBiHeight)/2+header.dwBiHeight);

        /*
        uint16_t clrBack(0);
        if (rcImage.left > 0)
            BIOS::LCD::Bar(CRect(0, 0, rcImage.left, BIOS::LCD::Height), clrBack);
        if (rcImage.right < BIOS::LCD::Width)
            BIOS::LCD::Bar(CRect(rcImage.left, 0, BIOS::LCD::Width, BIOS::LCD::Height), clrBack);
        if (rcImage.top > 0)
            BIOS::LCD::Bar(CRect(0, 0, BIOS::LCD::Width, rcImage.top), clrBack);
        if (rcImage.bottom < BIOS::LCD::Height)
            BIOS::LCD::Bar(CRect(0, rcImage.bottom, BIOS::LCD::Width, BIOS::LCD::Height), clrBack);
*/
        bool first{true};
        CRect rcLine(rcImage.left, rcImage.bottom-1, rcImage.right, rcImage.bottom);
        
        int gapSize = rowSize - header.dwBiWidth/2;
        uint8_t gapData[8];
        CStream gapStream(gapData, gapSize);
        
        for (int y=0; y<(int)header.dwBiHeight; y++)
        {
                int w = header.dwBiWidth/2;
                if (first)
                {
                    first = false;
                    uint8_t color;
                    reader >> color;
                    BIOS::LCD::Bar(CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), palette[color >> 4]);
                    
                    BIOS::LCD::BufferBegin(rcLine);
                    BIOS::LCD::BufferWrite(palette[color >> 4]);
                    BIOS::LCD::BufferWrite(palette[color & 15]);
                    w--;
                } else
                    BIOS::LCD::BufferBegin(rcLine);
                
                while (w--)
                {
                    uint8_t color;
                    reader >> color;
                    //uint16_t pair[2] = {palette[color >> 4], palette[color & 15]};
                    //BIOS::LCD::BufferWrite(palette, 2);
                    BIOS::LCD::BufferWrite(palette[color >> 4]);
                    BIOS::LCD::BufferWrite(palette[color & 15]);
                }
                rcLine.top--;
                rcLine.bottom--;
            reader >> gapStream;
            BIOS::LCD::BufferEnd();
         }
         reader.Close();
        return true;
    }
    bool ShowImage(char c)
    {
        static int lastIndex = -1;
        
        if (BIOS::FAT::OpenDir((char*)"bebetoy") != BIOS::FAT::EResult::EOk)
            return false;

        BIOS::FAT::TFindFile file;
        int index = 0;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
        {
            if (index > lastIndex && file.strName[0] == c)
            {
                char path[32];
                sprintf(path, "bebetoy/%s", file.strName);
                RenderImage(path);
                lastIndex = index;
                return true;
            }
            index++;
        }

        if (BIOS::FAT::OpenDir((char*)"bebetoy") != BIOS::FAT::EResult::EOk)
            return false;

        index = 0;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
        {
            if (file.strName[0] == c)
            {
                char path[32];
                sprintf(path, "bebetoy/%s", file.strName);
                RenderImage(path);
                lastIndex = index;
                return true;
            }
            index++;
        }
        return false;
    }
    int charToScancode(const char* s, int l)
    {
        if (l==1)
        {
            for (int i=0; scan_codes[i]; i+=3)
                if (s[0] == scan_codes[i+2] || s[0] == scan_codes[i+1])
                    return scan_codes[i];
        }
        if (l==2 && s[0] == 'F')
        {
            static const uint8_t Fkeys[] = {0x00, 0x05, 0x06, 0x04, 0x0c, 0x03, 0x0b, 0x83, 0x0a, 0x01, 0x09, 0x78, 0x07};
            return Fkeys[s[1] - '0'];
        }
    
        return -1;
    }
    
    void setup(const CRect& r)
    {
        drawline(0, "  F1 F2 F3 F4 F5 F6 F7 F8 F9 F0 F1 F2");
        drawline(1, "1  2  3  4  5  6  7  8  9  0  -  =  \\");
        drawline(2, " Q  W  E  R  T  Y  U  I  O  P  [  ]");
        drawline(3, "  A  S  D  F  G  H  J  K  L  ;  '");
        drawline(4, "   Z  X  C  V  B  N  M  ,  .  /  ");
        //drawbtn(5, 10, "     space      ");
        arrChars.Add(char_t{.x=10, .y=5, .c="     space      ", .len=16, .scancode=0x29});
        arrChars.Add(char_t{.x=0, .y=0, .c=(const char*)" ",  .len=1, .scancode=0x76});
        arrChars.Sort([](char_t& a, char_t& b){
            return b.scancode - a.scancode;
        });
        redraw();
    }
    void redraw()
    {
        GUI::Background(CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), RGB565(0000b0), RGB565(4040d0));
        for (int i=0;i<arrChars.GetSize(); i++)
            arrChars[i].Draw(RGB565(ffffc0), RGB565(000060));

    }
    void drawline(int y, const char* l)
    {
        for (int i=0; l[i]; i++)
        {
            if (l[i] != ' ')
            {
                char_t curChar{.x=i, .y=y, .c=l+i, .len=0};
                while (l[i] != ' ' && l[i])
                {
                    curChar.len++;
                    i++;
                }
                curChar.scancode = charToScancode(l+i-curChar.len, curChar.len);
                i--;
                arrChars.Add(curChar);
            }
        }
    }
    
    void drawbtn(int y, int i, const char* s)
    {
        BIOS::LCD::Draw(10+i*8-4, 40+y*20, RGB565(000040), RGBTRANS, CShapes_sel_left);
        BIOS::LCD::Print(10+i*8, 40+y*20, RGB565(f0f0e0), RGB565(000040), s);
        BIOS::LCD::Draw(10+(i+strlen(s))*8, 40+y*20, RGB565(000040), RGBTRANS, CShapes_sel_right);
    }
    
    void OnChange(int scancode, bool press)
    {
        if (mNeedRedraw)
        {
            if (press && scancode == 0x29)
            {
                mNeedRedraw = false;
                redraw();
            }
            return;
        }
        if (press)
        {
            if (scancode == 0x58) // caps lock
            {
                BIOS::SYS::Beep(100);
                mLeds ^= 4;
                UpdateLeds();
            }
            if (scancode == 0x77) // num lock
            {
                BIOS::SYS::Beep(100);
                mLeds ^= 2;
                UpdateLeds();
            }
            if (scancode == 0x7e) // scr lock
            {
                BIOS::SYS::Beep(100);
                mLeds ^= 1;
                UpdateLeds();
            }
            if (scancode == 0x05) // F1
            {
                mMode = 1;
            }
            if (scancode == 0x06) // F2
            {
                mMode = 2;
            }
        }

        char_t c = binary_search(arrChars, scancode);
        //findScancode(scancode);
        if (c.scancode == 0)
            return;
        
        if (press)
            c.Draw(RGB565(0000b0), RGB565(ffffff));
        else
            c.Draw(RGB565(ffffc0), RGB565(000060));
        
        if (!press && mMode == 1 && c.len == 1)
        {
            if (ShowImage(c.c[0]))
                mNeedRedraw = true;
        }
    }
    
    void loop()
    {
        static bool press = true;
        int code = keyboard.GetScancode();
        if (!code)
            return;
        if (code == 0xf0)
        {
            press = false;
            return;
        }
        OnChange(code, press);
        press = true;
    }
};

CApp game;

bool setup()
{
    //GUI::Background(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14), RGB565(0000b0), RGB565(4040d0));
    game.setup(CRect(10, 14+20, BIOS::LCD::Width-10, BIOS::LCD::Height-14-20));

    keyboard.Attach();

    return true;
}
void stop()
{
    keyboard.Detach();
}
void loop(BIOS::KEY::EKey key)
{
    game.loop();
    //BIOS::SYS::DelayMs(20);
}

void bresenhamLine(int x0, int y0, int x1, int y1, int c)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        BIOS::LCD::PutPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
/*
void cubicBezierCurve(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int steps) {
    // Scale factor to preserve precision during integer division
    const int scale = 1000;

    // Precompute scaled control points (scaled by 1000)
    int x0s = x0 * scale, y0s = y0 * scale;
    int x1s = x1 * scale, y1s = y1 * scale;
    int x2s = x2 * scale, y2s = y2 * scale;
    int x3s = x3 * scale, y3s = y3 * scale;

    // Precompute constants for the Bézier curve
    int N = steps;
    for (int t = 0; t <= N; ++t) {
        // t_scaled = t / N, but in integer form, t_scaled = t * scale / N
        int t_scaled = (t * scale) / N;
        int one_minus_t_scaled = scale - t_scaled;

        // Compute the Bézier polynomial coefficients
        int coeff0 = one_minus_t_scaled * one_minus_t_scaled * one_minus_t_scaled; // (1 - t)^3
        int coeff1 = 3 * one_minus_t_scaled * one_minus_t_scaled * t_scaled;      // 3(1 - t)^2 t
        int coeff2 = 3 * one_minus_t_scaled * t_scaled * t_scaled;                // 3(1 - t) t^2
        int coeff3 = t_scaled * t_scaled * t_scaled;                              // t^3

        // Compute x and y using integer arithmetic
        int x = (coeff0 * x0s + coeff1 * x1s + coeff2 * x2s + coeff3 * x3s) / (scale * scale);
        int y = (coeff0 * y0s + coeff1 * y1s + coeff2 * y2s + coeff3 * y3s) / (scale * scale);

        // Place the pixel
        BIOS::LCD::PutPixel(x0, y0, 0xffff);
        //putPixel(x, y);
    }
}
 */
void plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2)
{ /* plot a limited quadratic Bezier segment */
  int sx = x2-x1, sy = y2-y1;
  long xx = x0-x1, yy = y0-y1, xy; /* relative values for checks */
  double dx, dy, err, cur = xx*sy-yy*sx; /* curvature */
  assert(xx*sx <= 0 && yy*sy <= 0); /* sign of gradient must not change */
  if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) { /* begin with longer part */
    x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur; /* swap P0 P2 */
  }
  if (cur != 0) { /* no straight line */
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1; /* x step direction */
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1; /* y step direction */
    xy = 2*xx*yy; xx *= xx; yy *= yy; /* differences 2nd degree */
    if (cur*sx*sy < 0) { /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*sy*cur*(x1-x0)+xx-xy; /* differences 1st degree */
    dy = 4.0*sx*cur*(y0-y1)+yy-xy;
    xx += xx; yy += yy; err = dx+dy+xy; /* error 1st step */
    do {
        BIOS::LCD::PutPixel(x0, y0, 0xffff);
      if (x0 == x2 && y0 == y2) return; /* last pixel -> curve finished */
      y1 = 2*err < dx; /* save value for test of y step */
      if (2*err > dy) { x0 += sx; dx -= xy; err += dy += yy; } /* x step */
      if ( y1 ) { y0 += sy; dy -= xy; err += dx += xx; } /* y step */
    } while (dy < 0 && dx > 0); /* gradient negates -> algorithm fails */
  }
  bresenhamLine(x0,y0, x2,y2, 0xffff); /* plot remaining part to end */
}

void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2)
{ /* plot any quadratic Bezier curve */
  int x = x0-x1, y = y0-y1;
  double t = x0-2*x1+x2, r;
  if ((long)x*(x2-x1) > 0) { /* horizontal cut at P4? */
    if ((long)y*(y2-y1) > 0) /* vertical cut at P6 too? */
      if (fabs((y0-2*y1+y2)/t*x) > abs(y)) { /* which first? */
        x0 = x2; x2 = x+x1; y0 = y2; y2 = y+y1; /* swap points */
      } /* now horizontal cut at P4 comes first */
    t = (x0-x1)/t;
    r = (1-t)*((1-t)*y0+2.0*t*y1)+t*t*y2; /* By(t=P4) */
    t = (x0*x2-x1*x1)*t/(x0-x1); /* gradient dP4/dx=0 */
    x = floor(t+0.5); y = floor(r+0.5);
    r = (y1-y0)*(t-x0)/(x1-x0)+y0; /* intersect P3 | P0 P1 */
    plotQuadBezierSeg(x0,y0, x,floor(r+0.5), x,y);
    r = (y1-y2)*(t-x2)/(x1-x2)+y2; /* intersect P4 | P1 P2 */
    x0 = x1 = x; y0 = y; y1 = floor(r+0.5); /* P0 = P4, P1 = P8 */
  }
  if ((long)(y0-y1)*(y2-y1) > 0) { /* vertical cut at P6? */
    t = y0-2*y1+y2; t = (y0-y1)/t;
    r = (1-t)*((1-t)*x0+2.0*t*x1)+t*t*x2; /* Bx(t=P6) */
    t = (y0*y2-y1*y1)*t/(y0-y1); /* gradient dP6/dy=0 */
    x = floor(r+0.5); y = floor(t+0.5);
    r = (x1-x0)*(t-y0)/(y1-y0)+x0; /* intersect P6 | P0 P1 */
    plotQuadBezierSeg(x0,y0, floor(r+0.5),y, x,y);
    r = (x1-x2)*(t-y2)/(y1-y2)+x2; /* intersect P7 | P1 P2 */
    x0 = x; x1 = floor(r+0.5); y0 = y1 = y; /* P0 = P6, P1 = P7 */
  }
  plotQuadBezierSeg(x0,y0, x1,y1, x2,y2); /* remaining part */
}

void renderSvg(const char* svg)
{
    int x, y;
    auto svgArg = [&svg]() -> int {
        char *end;
        int temp;
        temp = (int)strtol(svg, &end, 10);
        svg = end;
        if (*svg == ' ')
            svg++;
        return temp;
    };
    auto svgLine = [](int x, int y, int nx, int ny)
    {
//        BIOS::DBG::Print("line(%d,%d,%d,%d)\n", x, y, nx, ny);
        bresenhamLine(x/4, y/5+40, nx/4, ny/5+40, RGB565(ffffff));
    };
    auto interpolate = [](int a, int b, int t)
    {
        return a + ((b-a)*t>>8);
    };
    auto svgCurve = [&](int x, int y, int dx1, int dy1, int dx2, int dy2, int dx, int dy)
    {
        dx1 = (x+dx1)/4; dy1 = (y+dy1)/5+40;
        dx2 = (x+dx2)/4; dy2 = (y+dy2)/5+40;
        dx = (x+dx)/4; dy = (y+dy)/5+40;
        x = x/4; y = y/5+40;
        int px = x;
        int py = y;
        for (int i=32; i<=256; i+=32)
        {
            int xa = interpolate(x, dx1, i);
            int ya = interpolate(y, dy1, i);
            int xb = interpolate(dx1, dx2, i);
            int yb = interpolate(dy1, dy2, i);
            int xc = interpolate(dx2, dx, i);
            int yc = interpolate(dy2, dy, i);
            
            int xd = interpolate(xa, xb, i);
            int yd = interpolate(ya, yb, i);
            int xe = interpolate(xb, xc, i);
            int ye = interpolate(yb, yc, i);
            
            int xf = interpolate(xd, xe, i);
            int yf = interpolate(yd, ye, i);
            bresenhamLine(px, py, xf, yf, RGB565(ffffff));
            px = xf;
            py = yf;
        }
    };

    while (*svg)
    {
        switch (*svg++)
        {
            case 'M':
            {
                x = svgArg();
                y = svgArg();
                break;
            }
            case 'l':
            {
                int dx = svgArg();
                int dy = svgArg();
                svgLine(x, y, x+dx, y+dy);
                x += dx;
                y += dy;
                break;
            }
            case 'c':
            {
                int dx1 = svgArg();
                int dy1 = svgArg();
                int dx2 = svgArg();
                int dy2 = svgArg();
                int dx = svgArg();
                int dy = svgArg();
                svgCurve(x, y, dx1, dy1, dx2, dy2, dx, dy);
                x += dx;
                y += dy;
                break;
            }
            case 'h':
            {
                int dx = svgArg();
                svgLine(x, y, x+dx, y);
                x += dx;
                break;
            }
            case 'v':
            {
                int dy = svgArg();
                svgLine(x, y, x, y+dy);
                y += dy;
                break;
            }
            case 0:
                return;
            default:
                BIOS::DBG::Print("junk: %s\n", svg);
                assert(0);
        }
    }
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    CONSOLE::colorBack = RGB565(202020);

    APP::Init("PS2 keyboard test v2");
    APP::Status("");

    if (setup())
    {
        if (0)
            renderSvg("M219 93c-76 -30 -139 -99 -139 -161c0 -83 81 -122 197 -122c125 0 210 48 210 131c0 168 -335 103 -335 217c0 28 21 58 68 81M273 555c-96 0 -172 -60 -172 -161c0 -97 67 -159 174 -159c105 0 174 59 174 156c0 112 -86 164 -174 164M344 543h173M318 802l-78 -155");
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
        {
          loop(key);
        }
        stop();
    }
    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
