#include <library.h>
#include <math.h>

#ifdef __APPLE__
#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))
#endif

// based on PCGPE encyclopedia
// http://qzx.com/pc-gpe/tut9.txt

constexpr int maxpolys = 6;
typedef int Poly_t[maxpolys][4][3];
Poly_t box =
{
    {{-10,10,-10},{10,10,-10},{10,-10,-10},{-10,-10,-10}}, // rear
    {{-10,10,10},{10,10,10},{10,-10,10},{-10,-10,10}}, // front
    
    {{10,10,-10},{10,-10,-10},{10,-10,10},{10,10,10}}, // left
    {{-10,10,-10},{-10,-10,-10},{-10,-10,10},{-10,10,10}}, // right

    {{-10,10,-10},{-10,10,10},{10,10,10},{10,10,-10}}, // top
    {{-10,-10,-10},{-10,-10,10},{10,-10,10},{10,-10,-10}}}; // bottom

uint16_t colors[] = {RGB565(ff0000), RGB565(00ff00),
    RGB565(0000ff), RGB565(ffff00),
    RGB565(ff00ff), RGB565(00ffff)};

int orientations[] = {-1, 1, 1, -1, -1, 1};

constexpr int fixedBits = 10;
constexpr int fixedBase = 1<<fixedBits;
typedef int32_t fixed;
struct Point { fixed x, y, z; };
Point Lines[maxpolys][4];
Point Translated[maxpolys][4];
Point Normals[maxpolys];

fixed Xoff, Yoff, Zoff;
fixed lookup[256][2];

uint16_t buffer[BIOS::LCD::Height][maxpolys][2];
int bufferMin = -1;
int bufferMax = 0;

uint16_t gradient[BIOS::LCD::Height];
bool lighting = true;
int speed = 10;
bool zooming = true;

void HLine(int x1, int x2, int y, int index)
{
    buffer[y][index][0] = x1;
    buffer[y][index][1] = x2;
}

void DrawPoly(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int index)
{
  int x;
  int mny,mxy;
  int mnx,mxx,yc;
  int mul1,div1,mul2,div2,mul3,div3,mul4,div4;

  mny=y1; mxy=y1;
  if (y2<mny) mny=y2;
  if (y2>mxy) mxy=y2;
  if (y3<mny) mny=y3;
  if (y3>mxy) mxy=y3;
  if (y4<mny) mny=y4;
  if (y4>mxy) mxy=y4;

  if (mny<0) mny=0;
  if (mxy>BIOS::LCD::Height-1) mxy=BIOS::LCD::Height-1;
  if (mny>BIOS::LCD::Height-1) return;
  if (mxy<0) return;

  mul1=x1-x4; div1=y1-y4;
  mul2=x2-x1; div2=y2-y1;
  mul3=x3-x2; div3=y3-y2;
  mul4=x4-x3; div4=y4-y3;

  for (yc=mny; yc<=mxy; yc++)
  {
      mnx=BIOS::LCD::Width;
      mxx=-1;
      if ((y4>=yc) || (y1>=yc))
        if ((y4<=yc) || (y1<=yc))
          if (y4!=y1)
          {
              x=(yc-y4)*mul1 / div1+x4;
              if (x<mnx)
                mnx=x;
              if (x>mxx)
                mxx=x;
          }
      if ((y1>=yc) || (y2>=yc))
        if ((y1<=yc) || (y2<=yc))
          if (y1!=y2)
          {
              x=(yc-y1)*mul2 / div2+x1;
              if (x<mnx)
                mnx=x;
              if (x>mxx)
                mxx=x;
          }
      if ((y2>=yc) || (y3>=yc))
        if ((y2<=yc) || (y3<=yc))
          if (y2!=y3)
          {
              x=(yc-y2)*mul3 / div3+x2;
              if (x<mnx)
                mnx=x;
              if (x>mxx)
                mxx=x;
          }
      if ((y3>=yc) || (y4>=yc))
        if ((y3<=yc) || (y4<=yc))
          if (y3!=y4)
          {
              x=(yc-y3)*mul4 / div4+x3;
              if (x<mnx)
                mnx=x;
              if (x>mxx)
                mxx=x;
          }
      if (mnx<0)
        mnx=0;
      if (mxx>BIOS::LCD::Width)
        mxx=BIOS::LCD::Width;
      if (mnx<=mxx)
      {
          if (yc < bufferMin)
              bufferMin = yc;
          if (yc > bufferMax) bufferMax = yc;
        HLine(mnx,mxx,yc,index);
      }
  }
}

void SetUpPoints()
{
    for (int i=0; i<256; i++)
    {
        constexpr float deg2rad = 3.141592653f / 128.0f;
        lookup[i][0] = sin(i * deg2rad) * fixedBase;
        lookup[i][1] = cos(i * deg2rad) * fixedBase;
    }
}

void RotatePoints(int x, int y, int z)
{
  Point temp;
  for (int loop1=0; loop1<maxpolys; loop1++)
    for (int loop2=0; loop2<4; loop2++)
    {
      temp.x=Lines[loop1][loop2].x;
      temp.y=(lookup[x][1]*Lines[loop1][loop2].y - lookup[x][0]*Lines[loop1][loop2].z) >> fixedBits;
      temp.z=(lookup[x][0]*Lines[loop1][loop2].y + lookup[x][1]*Lines[loop1][loop2].z) >> fixedBits;
      Translated[loop1][loop2]=temp;

      if (y>0)
      {
        temp.x=(lookup[y][1]*Translated[loop1][loop2].x - lookup[y][0]*Translated[loop1][loop2].y) >> fixedBits;
        temp.y=(lookup[y][0]*Translated[loop1][loop2].x + lookup[y][1]*Translated[loop1][loop2].y) >> fixedBits;
        temp.z=Translated[loop1][loop2].z;
        Translated[loop1][loop2]=temp;
      }

      if (z>0)
      {
        temp.x=(lookup[z][1]*Translated[loop1][loop2].x + lookup[z][0]*Translated[loop1][loop2].z) >> fixedBits;
        temp.y=Translated[loop1][loop2].y;
        temp.z=(-lookup[z][0]*Translated[loop1][loop2].x + lookup[z][1]*Translated[loop1][loop2].z) >> fixedBits;
        Translated[loop1][loop2]=temp;
      }
    }
}

int orientation(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y)
{
    int val = (p2y - p1y) * (p3x - p2x) - (p2x - p1x) * (p3y - p2y);
    if (val == 0)
        return 0;
    return (val > 0)? 1: -1;
}

Point CrossProduct(Point& a, Point& b)
{
    Point Product;

    //Cross product formula
    Product.x = (a.y * b.z - a.z * b.y) >> fixedBits;
    Product.y = (a.z * b.x - a.x * b.z) >> fixedBits;
    Product.z = (a.x * b.y - a.y * b.x) >> fixedBits;

    return Product;
}

Point Normal(Point& a, Point& b, Point& c)
{
    Point v1{b.x-a.x, b.y-a.y, b.z-a.z}; // b-a
    Point v2{c.x-a.x, c.y-a.y, c.z-a.z}; // c-a
    /*
    v1.x >>= 4;
    v1.y >>= 4;
    v1.z >>= 4;
    v2.x >>= 4;
    v2.y >>= 4;
    v2.z >>= 4;
     */
    return CrossProduct(v1, v2);
}

void DrawPoints()
{
    bufferMin = BIOS::LCD::Height;
    bufferMax = 0;
    
    if (lighting)
    {
        for(int loop1=0; loop1<maxpolys; loop1++)
            Normals[loop1] = Point{0, 0, 0};
    }
    
  for(int loop1=0; loop1<maxpolys; loop1++)
  {                                    
    if ((Translated[loop1][0].z+Zoff<0) && (Translated[loop1][1].z+Zoff<0) &&
       (Translated[loop1][2].z+Zoff<0) && (Translated[loop1][3].z+Zoff<0))
    {
      int temp = (Translated[loop1][0].z+Zoff) >> 8;
      int nx = Translated[loop1][0].x / temp + Xoff;
      int ny = Translated[loop1][0].y / temp + Yoff;
      temp = (Translated[loop1][1].z+Zoff) >> 8;
      int nx2 = Translated[loop1][1].x / temp + Xoff;
      int ny2 = Translated[loop1][1].y / temp + Yoff;
      temp = (Translated[loop1][2].z+Zoff) >> 8;
      int nx3 = Translated[loop1][2].x / temp + Xoff;
      int ny3 = Translated[loop1][2].y / temp + Yoff;
      temp = (Translated[loop1][3].z+Zoff) >> 8;
      int nx4 = Translated[loop1][3].x / temp + Xoff;
      int ny4 = Translated[loop1][3].y / temp + Yoff;
        
      if (orientation(nx, ny, nx2, ny2, nx3, ny3) == orientations[loop1])
      {
        if (lighting)
        {
            Normals[loop1] = Normal(Translated[loop1][0], Translated[loop1][1], Translated[loop1][2]);
        }
        DrawPoly(nx,ny,nx2,ny2,nx3,ny3,nx4,ny4,loop1);
      }
    }
  }
}

int degx = 0, degy = 0, degz = 0;
int zDir = 1;
int lastBufferMin = 14, lastBufferMax = BIOS::LCD::Height-14;

void Init()
{
    memset(buffer, 0, sizeof(buffer));
    Zoff = -(60 << fixedBits);
    Yoff = BIOS::LCD::Height/2;
    Xoff = BIOS::LCD::Width/2;
    Poly_t& poly = box;

    for (int loop1=0; loop1<maxpolys; loop1++)
        for (int loop2=0; loop2<4; loop2++)
        {
            Lines[loop1][loop2].x = poly[loop1][loop2][0] << fixedBits;
            Lines[loop1][loop2].y = poly[loop1][loop2][1] << fixedBits;
            Lines[loop1][loop2].z = poly[loop1][loop2][2] << fixedBits;
        }
    
    for (int i=100; i<BIOS::LCD::Height; i++)
    {
        int y = 90 + (i-100)*4/5;
        gradient[i] = RGB565RGB(0, y, i/3);
    }
    for (int i=0; i<100; i++)
    {
        gradient[i] = RGB565RGB(0, 255-i*3/2, 255-i);
    }
}

unsigned long julery_isqrt(unsigned long val) {
    unsigned long temp, g=0, b = 0x8000, bshft = 15;
    do {
        if (val >= (temp = (((g << 1) + b)<<bshft--))) {
           g += b;
           val -= temp;
        }
    } while (b >>= 1);
    return g;
}

void CalculateLight(uint16_t* colors)
{
    for (int i=0; i<maxpolys; i++)
    {
        Point& normalized = Normals[i];
        if (normalized.x == 0 && normalized.y == 0 && normalized.z == 0)
            continue;
        
        normalized.x >>= 8;
        normalized.y >>= 8;
        normalized.z >>= 8;
        
        int len = julery_isqrt(normalized.x*normalized.x+ normalized.y*normalized.y + normalized.z*normalized.z);
        int light = abs((normalized.z << 8) / len);
        
        light += 32; // ambient
        int spc = max(0, light - 256)*2; // specular
        light = min(255, light); // directional
        
        int color = 0;
        switch (i)
        {
            case 0: color = RGB565RGB(light, spc, spc); break;
            case 1: color = RGB565RGB(spc, light, spc); break;
            case 2: color = RGB565RGB(spc, spc, light); break;
            case 3: color = RGB565RGB(light, light, spc); break;
            case 4: color = RGB565RGB(light, spc, light); break;
            case 5: color = RGB565RGB(spc, light, light); break;
        }
        colors[i] = color;
    }
}

void Render()
{
    bufferMin = max(14, bufferMin);
    bufferMax = min(BIOS::LCD::Height-14, bufferMax);

    uint16_t lightColors[maxpolys];
    uint16_t* _colors = colors;
    if (lighting)
    {
        _colors = lightColors;
        CalculateLight(_colors);
    }

    for (int i=lastBufferMin; i<bufferMin; i++)
        BIOS::LCD::Bar(0, i, BIOS::LCD::Width, i+1, gradient[i]);
    
    for (int i=bufferMax; i<lastBufferMax; i++)
        BIOS::LCD::Bar(0, i, BIOS::LCD::Width, i+1, gradient[i]);

    for (int i=bufferMin; i<bufferMax; i++)
    {
        int _min = BIOS::LCD::Width;
        int _max = 0;
        for (int j=0; j<maxpolys; j++)
        {
            if (buffer[i][j][1] == 0)
                continue;
            _min = min(_min, buffer[i][j][0]);
            _max = max(_max, buffer[i][j][1]);
        }
        if (_max == 0)
        {
            BIOS::LCD::Bar(0, i, BIOS::LCD::Width, i+1, gradient[i]);
            continue;
        }
        if (_min > 0)
            BIOS::LCD::Bar(0, i, _min, i+1, gradient[i]);
        if (_max < BIOS::LCD::Width)
            BIOS::LCD::Bar(_max, i, BIOS::LCD::Width, i+1, gradient[i]);
        for (int j=0; j<maxpolys; j++)
        {
            if (buffer[i][j][1] == 0)
                continue;
            BIOS::LCD::Bar(buffer[i][j][0], i, buffer[i][j][1], i+1, _colors[j]);
            buffer[i][j][1] = 0;
        }
    }
    lastBufferMin = bufferMin;
    lastBufferMax = bufferMax;
}

void MoveAround()
{
    if (zooming)
    {
        Zoff += zDir*200;
        if ((Zoff >> fixedBits) > -20)
            zDir = -1;
        if ((Zoff >> fixedBits) < -160)
            zDir = +1;
    }

    degx += 7*20/speed;
    degy += 13*20/speed;
    degz += 31*20/speed;

    RotatePoints ((degx>>4) & 0xff, (degy>>4) & 0xff, (degz>>4) & 0xff);
    DrawPoints();
    Render();
}
