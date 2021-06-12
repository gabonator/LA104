#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include <math.h>

uint32_t pattern[] = {0x00000011, 0x23444333, 0x33333452, 0x11110000, 0x06776001, 0x34433222, 0x22222342, 0x11100000, 0x67777602, 0x44322111, 0x11122235, 0x21100000, 0x66776603, 0x43221011, 0x11111234, 0x21100000, 0x06666125, 0x32110601, 0x11111123, 0x52100000, 0x00000234, 0x21111011, 0x11111113, 0x45210000, 0x11122353, 0x21111111, 0x11111112, 0x34522111, 0x55544532, 0x11111000, 0x00000111, 0x23344555, 0x33333452, 0x11110000, 0x00000011, 0x23444333, 0x22222342, 0x11100000, 0x06776001, 0x34433222, 0x11122235, 0x21100000, 0x67777602, 0x44322111, 0x11111234, 0x21100000, 0x66776603, 0x43221011, 0x11111123, 0x52100000, 0x06666125, 0x32110601, 0x11111113, 0x45210000, 0x00000234, 0x21111011, 0x11111112, 0x34522111, 0x11122353, 0x21111111, 0x00000111, 0x23344555, 0x55544532, 0x11111000};
int palette[] = {RGB565(0031a6), RGB565(000095), RGB565(000085), RGB565(000084), RGB565(000073), RGB565(000074), RGB565(0000b6), RGB565(0021c7)};


class CGameBreakout
{
    CRect mScreen;
    
    CPoint mBall;
    CPoint mBallDir;
    int mBallRadius{5};
    int mBallRadiusSq{5*5};
    CPoint mPaddle;
    int mPaddleWidth;
    
    int map[10*6];
    
public:
    void setup(CRect screen)
    {
        mScreen = screen;
        mBall = mScreen.Center();
        mBall.x *= 16;
        mBall.y *= 16;
        mBallDir = CPoint(16, 16);
        mPaddle.x = mBall.x/16;
        mPaddle.y = screen.bottom - 8;
        mPaddleWidth = 20;
        
        int cols = 10;
        int rows = 6;
        int index = 0;
        
        for (int y=0; y<rows; y++)
            for (int x=0; x<cols; x++)
                map[index++] = (x+y)%6;
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
    
    void loop()
    {
        drawBackground(mScreen);
        //BIOS::LCD::Bar(mScreen, RGB565(000000));
        BIOS::LCD::Rectangle(mScreen, RGB565(ffffff));
        mPaddle.x = mBall.x / 16;
        if (mPaddle.x - mPaddleWidth - 2 < mScreen.left)
            mPaddle.x = mScreen.left + mPaddleWidth + 2;
        if (mPaddle.x + mPaddleWidth + 2 > mScreen.right)
            mPaddle.x = mScreen.right - mPaddleWidth - 2;

        drawMap();
        drawBall();
        drawPaddle();
        moveBall();
        
        int p = GetBlockByPos(mBall.x/16, mBall.y/16 - mBallRadius);
        if (p != -1 && map[p] != -1)
        {
            map[p] = -1;
            mBallDir.y = 16;
        }
        p = GetBlockByPos(mBall.x/16, mBall.y/16 + mBallRadius);
        if (p != -1 && map[p] != -1)
        {
            map[p] = -1;
            mBallDir.y = -16;
        }
        p = GetBlockByPos(mBall.x/16 - mBallRadius, mBall.y/16);
        if (p != -1 && map[p] != -1)
        {
            map[p] = -1;
            mBallDir.x = 16;
        }
        p = GetBlockByPos(mBall.x/16 + mBallRadius, mBall.y/16);
        if (p != -1 && map[p] != -1)
        {
            map[p] = -1;
            mBallDir.x = -16;
        }
    }
    
    void drawMap()
    {
        int colors[] = {RGB565(ffb0b0), RGB565(bbffbb), RGB565(bbbbff),
            RGB565(ffffbb), RGB565(ffbbff), RGB565(bbffff)};

        CRect inner(mScreen.left+2, mScreen.top+2, mScreen.right-1, mScreen.top + mScreen.Height()*4/10);
        
        int cols = 10;
        int rows = 6;
        
        for (int y=0; y<rows; y++)
            for (int x=0; x<cols; x++)
            {
                CRect block(
                    inner.left + inner.Width()*x/cols,
                    inner.top + inner.Height()*y/rows,
                    inner.left + inner.Width()*(x+1)/cols-1,
                    inner.top + inner.Height()*(y+1)/rows-1
                );
                
                int c = map[y*cols+x];
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
        CRect inner(mScreen.left+2, mScreen.top+2, mScreen.right-1, mScreen.top + mScreen.Height()*4/10);
        
        if (!inner.IsInside(ptx, pty))
            return -1;

        int cols = 10;
        int rows = 6;

        int bx = (ptx - inner.left)*cols/inner.Width();
        int by = (pty - inner.top)*rows/inner.Height();
        
        return by*cols + bx;
    }
    
    void drawBall()
    {
        for (int y=-mBallRadius; y<mBallRadius; y++)
            for (int x=-mBallRadius; x<mBallRadius; x++)
                if (x*x+y*y < mBallRadiusSq-10)
                    BIOS::LCD::PutPixel(mBall.x/16 + x, mBall.y/16 + y, RGB565(d0d0d0));
                else if (x*x+y*y < mBallRadiusSq)
                    BIOS::LCD::PutPixel(mBall.x/16 + x, mBall.y/16 + y, RGB565(808080));

    }
    
    void drawPaddle()
    {
        int colors[] = {RGB565(ffff00), RGB565(eeee00), RGB565(dddd00), RGB565(cccc00), RGB565(bbbb00)};
        int ofs[] = {1, 0, 0, 0, 1};
        for (int i=0; i<5; i++)
            BIOS::LCD::Bar(mPaddle.x - mPaddleWidth + ofs[i], mPaddle.y+i,
                           mPaddle.x + mPaddleWidth - ofs[i], mPaddle.y+i+1, colors[i]);
    }
    
    void moveBall()
    {
        mBall.x += mBallDir.x;
        mBall.y += mBallDir.y;
        if ((mBall.x + mBallRadius)/16 >= mScreen.right || (mBall.x - mBallRadius)/16 < mScreen.left)
            mBallDir.x = -mBallDir.x;
        if ((mBall.y + mBallRadius)/16 >= mPaddle.y || (mBall.y - mBallRadius)/16 < mScreen.top)
            mBallDir.y = -mBallDir.y;
    }
};

CGameBreakout game;

bool setup()
{
    GUI::Background(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14), RGB565(0000b0), RGB565(4040d0));
    game.setup(CRect(20, 14+20, BIOS::LCD::Width-20, BIOS::LCD::Height-14-20));
    return true;
}

void loop(BIOS::KEY::EKey key)
{
    game.loop();
    
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
