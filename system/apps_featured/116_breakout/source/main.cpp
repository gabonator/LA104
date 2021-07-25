#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include <math.h>
//https://www.spriters-resource.com/arcade/arkanoidiirevengeofdoh/sheet/60830/

uint32_t pattern[] = {0x00000011, 0x23444333, 0x33333452, 0x11110000, 0x06776001, 0x34433222, 0x22222342, 0x11100000, 0x67777602, 0x44322111, 0x11122235, 0x21100000, 0x66776603, 0x43221011, 0x11111234, 0x21100000, 0x06666125, 0x32110601, 0x11111123, 0x52100000, 0x00000234, 0x21111011, 0x11111113, 0x45210000, 0x11122353, 0x21111111, 0x11111112, 0x34522111, 0x55544532, 0x11111000, 0x00000111, 0x23344555, 0x33333452, 0x11110000, 0x00000011, 0x23444333, 0x22222342, 0x11100000, 0x06776001, 0x34433222, 0x11122235, 0x21100000, 0x67777602, 0x44322111, 0x11111234, 0x21100000, 0x66776603, 0x43221011, 0x11111123, 0x52100000, 0x06666125, 0x32110601, 0x11111113, 0x45210000, 0x00000234, 0x21111011, 0x11111112, 0x34522111, 0x11122353, 0x21111111, 0x00000111, 0x23344555, 0x55544532, 0x11111000};

int palette[] = {RGB565(0031a6), RGB565(000095), RGB565(000085), RGB565(000084), RGB565(000073), RGB565(000074), RGB565(0000b6), RGB565(0021c7)};

uint32_t powerup[] = {0x01111111, 0x11111110, 0x01111111, 0x11111110, 0x01111111, 0x11111110, 0x01111111, 0x11111110, 0x01111111, 0x11111110, 0x01111222, 0x22211110, 0x01111220, 0x11111110, 0x01111220, 0x11111110, 0x13333223, 0x33333311, 0x13333333, 0x33333311, 0x13333333, 0x33333311, 0x13333333, 0x33333311, 0x13333333, 0x33333311, 0x13333300, 0x00003311, 0x13333222, 0x22233311, 0x13333220, 0x33333311, 0x34444220, 0x44444411, 0x31444444, 0x44444411, 0x31444444, 0x44444411, 0x31444444, 0x44444411, 0x31444444, 0x44444411, 0x31444444, 0x44444411, 0x31444400, 0x00004411, 0x31444220, 0x44444411, 0x14444220, 0x44444441, 0x14444224, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444222, 0x22244441, 0x14444220, 0x44444441, 0x14444220, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444444, 0x44444441, 0x14444400, 0x00004441, 0x11444222, 0x22244411, 0x11444220, 0x44444411, 0x11444224, 0x44444411, 0x11444444, 0x44444411, 0x11444444, 0x44444411, 0x11444444, 0x44444411, 0x11444444, 0x44444411, 0x11444444, 0x44444411, 0x01111100, 0x00001110, 0x01111220, 0x11111110, 0x01111220, 0x11111110, 0x01111221, 0x11111110, 0x01111111, 0x11111110, 0x01111111, 0x11111110, 0x01111111, 0x11111110, 0x01111111, 0x11111110};

int poweruppal[] = {RGB565(000000), RGB565(f70000), RGB565(f7f700), RGB565(f7f7f7), RGB565(a50000)};

uint16_t powerupbuf[16*7];
uint16_t ballbuf[11*11];

class CGameBreakout
{
    CRect mScreen;
    
    CPoint mBall;
    CPoint mBallDir;
    int mBallRadius{5};
    int mBallRadiusSq{5*5};
    CPoint mPaddle;
    int mPaddleWidth;
        
    static const int mapCols{13};
    static const int mapRows{12};
    int map[mapCols*mapRows];

public:
    void setup(CRect screen)
    {
        mScreen = screen;
        mBall = mScreen.Center();
        mBall.y = mScreen.bottom-20;
        mBall.x *= 16;
        mBall.y *= 16;
        mBallDir = CPoint(16, -16);
        mPaddle.x = mBall.x/16;
        mPaddle.y = screen.bottom - 8;
        mPaddleWidth = 20;
        
        int index = 0;
        
        for (int y=0; y<mapRows; y++)
            for (int x=0; x<mapCols; x++)
                map[index++] = (x+y)%6;

        for (int x=0; x<mapCols; x++)
        {
            map[x] = -1;
            map[mapCols+x] = -1;
            if ((x&1) == 1)
                map[(mapRows-1)*mapCols + x] = 6;
        }
        for (int y=0; y<mapRows; y++)
            map[y*mapCols + mapCols/2] = -1;

        drawBackground(mScreen);
        BIOS::LCD::Rectangle(mScreen, RGB565(ffffff));
        drawMap();
    }
    
    void clearRect(const CRect rcWindow, const CRect& rcSection)
    {
        for (int y=rcSection.top - rcWindow.top; y<rcSection.bottom - rcWindow.top; y++)
            for (int x=rcSection.left - rcWindow.left; x<rcSection.right - rcWindow.left; x++)
            {
                int pix = pattern[(y&15)*4+(x&31)/8] >> (28-4*(x&7));
                pix &= 0xf;
                int color = palette[pix];
                BIOS::LCD::PutPixel(rcWindow.left+x, rcWindow.top+y, color);
            }
    }
    
    void drawBackground(const CRect& rc)
    {
        for (int y=0; y<rc.Height(); y++)
            for (int x=0; x<rc.Width(); x++)
            {
                int pix = pattern[(y&15)*4+(x&31)/8] >> (28-4*(x&7));
                pix &= 0xf;
                int color = palette[pix];
                BIOS::LCD::PutPixel(rc.left+x, rc.top+y, color);
            }
    }
    
    void drawPowerup(int _x, int _y, int p)
    {
        CRect rcCurrent(_x, _y, _x+16, _y+7);
        static CRect rcLast;
        
        if (rcLast.IsValid())
        {
            BIOS::LCD::BufferBegin(rcLast);
            BIOS::LCD::BufferWrite(powerupbuf, 16*7);
            BIOS::LCD::BufferEnd();
        }
        
        BIOS::LCD::BufferBegin(rcCurrent);
        BIOS::LCD::BufferRead(powerupbuf, 16*7);
        BIOS::LCD::BufferEnd();
        
        rcLast = rcCurrent;
        for (int y=0; y<7; y++)
            for (int x=0; x<16; x++)
            {
                int pix = powerup[p*2+y*16+x/8] >> (28-4*(x&7));
                pix &= 0xf;
                int color = poweruppal[pix];
                if (color != RGBTRANS)
                    BIOS::LCD::PutPixel(_x+x, _y+y, color);
            }

    }
    
    void loop()
    {
        mPaddle.x = mBall.x / 16;
        if (mPaddle.x - mPaddleWidth - 2 < mScreen.left)
            mPaddle.x = mScreen.left + mPaddleWidth + 2;
        if (mPaddle.x + mPaddleWidth + 2 > mScreen.right)
            mPaddle.x = mScreen.right - mPaddleWidth - 2;

        drawBall();
        drawPaddle();
        moveBall();
        static int p=0;
        static int y=100;
        drawPowerup(100, y, (p++/4) & 7);
        y++;
        if (y>200)
            y = 20;
        
        int dirx[] = {-1, 0, +1, 0,  -1, -1, +1, +1};
        int diry[] = {0, -1, 0, +1,  -1, +1, -1, +1};
        int dirs[] = {5, 5, 5, 5,    3, 3, 3, 3};
        for (int i=0; i<COUNT(dirx); i++)
        {
            int p = GetBlockByPos(mBall.x/16 + dirx[i]*dirs[i],
                                  mBall.y/16 + diry[i]*dirs[i]);
            
            if (p != -1 && map[p] != -1)
            {
                removeBlock(p);
                if (dirx[i] != 0)
                    mBallDir.x = dirx[i]*-16;
                if (diry[i] != 0)
                    mBallDir.y = diry[i]*-16;
            }
        }
    }
    
    void removeBlock(int p)
    {
        if (map[p] == 6)
            return;
        
        map[p] = -1;
        CRect rc = GetBlockRect(p);
        clearRect(mScreen, rc);
    }
    
    void drawMap()
    {
        int colors[] = {RGB565(ffaaaa), RGB565(aaffaa), RGB565(aaaaff),
            RGB565(ffffaa), RGB565(ffaaff), RGB565(aaffff),
            RGB565(ffffff)
        };
        
        int p = 0;
        
        for (int y=0; y<mapRows; y++)
            for (int x=0; x<mapCols; x++, p++)
            {
                CRect block = GetBlockRect(p);
                int c = map[y*mapCols+x];
                if (c == -1)
                    continue;
                BIOS::LCD::Bar(block, colors[c]);
                CRect shadow1(block.left, block.bottom-2, block.right, block.bottom);
                BIOS::LCD::Bar(shadow1, colors[c] - RGB565(202020));

                CRect shadow2(block.right-2, block.top, block.right, block.bottom);
                BIOS::LCD::Bar(shadow2, colors[c] - RGB565(202020));
            }
                
    }
    
    int GetBlockByPos(int ptx, int pty)
    {
        CRect inner(mScreen.left+2, mScreen.top+2, mScreen.right-1, mScreen.top + mScreen.Height()*6/10);
        
        if (!inner.IsInside(ptx, pty))
            return -1;

        int bx = (ptx - inner.left)*mapCols/inner.Width();
        int by = (pty - inner.top)*mapRows/inner.Height();
        
        return by*mapCols + bx;
    }
    
    CRect GetBlockRect(int p)
    {
        CRect inner(mScreen.left+2, mScreen.top+2, mScreen.right-1, mScreen.top + mScreen.Height()*6/10);
        
        int x = p % mapCols;
        int y = p / mapCols;
        
        return CRect(
            inner.left + inner.Width()*x/mapCols,
            inner.top + inner.Height()*y/mapRows,
            inner.left + inner.Width()*(x+1)/mapCols-1,
            inner.top + inner.Height()*(y+1)/mapRows-1
        );
    }
    
    void drawBall()
    {
        static CRect rcLast;
        
        if (rcLast.IsValid())
        {
            BIOS::LCD::BufferBegin(rcLast);
            BIOS::LCD::BufferWrite(ballbuf, 11*11);
            BIOS::LCD::BufferEnd();
        }

        rcLast = CRect(mBall.x/16 - mBallRadius, mBall.y/16 - mBallRadius,
                       mBall.x/16 + mBallRadius, mBall.y/16 + mBallRadius);

        BIOS::LCD::BufferBegin(rcLast);
        BIOS::LCD::BufferRead(ballbuf, 11*11);
        BIOS::LCD::BufferEnd();

//        if (rcLast.IsValid())
//            clearRect(mScreen, rcLast);
        
                
        for (int y=-mBallRadius; y<mBallRadius; y++)
            for (int x=-mBallRadius; x<mBallRadius; x++)
                if (x*x+y*y < mBallRadiusSq-10)
                    BIOS::LCD::PutPixel(mBall.x/16 + x, mBall.y/16 + y, RGB565(d0d0d0));
                else if (x*x+y*y < mBallRadiusSq)
                    BIOS::LCD::PutPixel(mBall.x/16 + x, mBall.y/16 + y, RGB565(808080));

    }
    
    void drawPaddle()
    {
        static CRect rcLast;
        CRect rcCurrent = CRect(mPaddle.x - mPaddleWidth, mPaddle.y, mPaddle.x + mPaddleWidth, mPaddle.y + 5);
        
        if (rcCurrent == rcLast)
            return;
        
        if (rcLast.IsValid())
        {
            CRect rcTemp = rcLast;
            rcTemp.left = min(rcLast.left, rcCurrent.left+1);
            rcTemp.right = max(rcLast.left, rcCurrent.left+1);
            clearRect(mScreen, rcTemp);
            rcTemp.left = min(rcLast.right, rcCurrent.right-1);
            rcTemp.right = max(rcLast.right, rcCurrent.right-1);
            clearRect(mScreen, rcTemp);
        }
        
        int colors[] = {RGB565(ffff00), RGB565(eeee00), RGB565(dddd00), RGB565(cccc00), RGB565(bbbb00)};
        int ofs[] = {1, 0, 0, 0, 1};
        for (int i=0; i<5; i++)
            BIOS::LCD::Bar(mPaddle.x - mPaddleWidth + ofs[i], mPaddle.y+i,
                           mPaddle.x + mPaddleWidth - ofs[i], mPaddle.y+i+1, colors[i]);
        
        rcLast = rcCurrent;
    }
    
    void moveBall()
    {
        mBall.x += mBallDir.x;
        mBall.y += mBallDir.y;
        if (mBall.x/16 + mBallRadius >= mScreen.right-2 || mBall.x/16 - mBallRadius < mScreen.left+2)
            mBallDir.x = -mBallDir.x;
        if (mBall.y/16 + mBallRadius >= mPaddle.y || mBall.y/16 - mBallRadius < mScreen.top+2)
            mBallDir.y = -mBallDir.y;
    }
};

CGameBreakout game;

bool setup()
{
    GUI::Background(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14), RGB565(0000b0), RGB565(4040d0));
    game.setup(CRect(10, 14+20, BIOS::LCD::Width-10, BIOS::LCD::Height-14-20));
    return true;
}

void loop(BIOS::KEY::EKey key)
{
    game.loop();
    BIOS::SYS::DelayMs(5);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::colorBack = RGB565(202020);

    APP::Init("DEMO: Breakout game core");
    APP::Status("");

    if (setup())
    {
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
        {
          loop(key);
        }
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
