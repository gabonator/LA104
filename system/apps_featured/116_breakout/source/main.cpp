#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include <math.h>
//https://www.spriters-resource.com/arcade/arkanoidiirevengeofdoh/sheet/60830/

#ifdef _ARM
uint8_t buffer[1024*3];
int ofs = 0;

void * operator new(size_t size)
{
    if (sizeof(buffer)-ofs < size)
        return nullptr;
    
    uint8_t* p = buffer+ofs;
    ofs += size;
    return p;
}

void operator delete(void*, unsigned int l)
{
}
#endif


uint32_t pattern[] = {0x00000011, 0x23444333, 0x33333452, 0x11110000, 0x06776001, 0x34433222, 0x22222342, 0x11100000, 0x67777602, 0x44322111, 0x11122235, 0x21100000, 0x66776603, 0x43221011, 0x11111234, 0x21100000, 0x06666125, 0x32110601, 0x11111123, 0x52100000, 0x00000234, 0x21111011, 0x11111113, 0x45210000, 0x11122353, 0x21111111, 0x11111112, 0x34522111, 0x55544532, 0x11111000, 0x00000111, 0x23344555, 0x33333452, 0x11110000, 0x00000011, 0x23444333, 0x22222342, 0x11100000, 0x06776001, 0x34433222, 0x11122235, 0x21100000, 0x67777602, 0x44322111, 0x11111234, 0x21100000, 0x66776603, 0x43221011, 0x11111123, 0x52100000, 0x06666125, 0x32110601, 0x11111113, 0x45210000, 0x00000234, 0x21111011, 0x11111112, 0x34522111, 0x11122353, 0x21111111, 0x00000111, 0x23344555, 0x55544532, 0x11111000};

int palette[] = {RGB565(0031a6), RGB565(000095), RGB565(000085), RGB565(000084), RGB565(000073), RGB565(000074), RGB565(0000b6), RGB565(0021c7)};

#include "images.h"

class CGame
{
    CRect mScreenRect;
    
public:
    void setup(const CRect& screenRect)
    {
        mScreenRect = screenRect;
    }
    
    const CRect& GetScreenRect() const
    {
        return mScreenRect;
    }
    
    void clearRect(const CRect rcWindow, const CRect& rcSection) const
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
    
    void drawBackground(const CRect& rc) const
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
};

class CObject
{
public:
    enum Type {
        None,
        Ball,
        Powerup,
        Paddle,
        Enemy
    };
    virtual Type GetType() = 0;
    virtual void Draw() = 0;
    virtual void Hide() = 0;
    virtual void Move() = 0;
    virtual CRect GetRect() const = 0;
};

class CBall : public CObject
{
    const CGame& mGame;
    CPoint mBall;
    CPoint mBallDir;
    int mBallRadius{5};
    int mBallRadiusSq{5*5};

    CRect rcLast;
    uint16_t ballbuf[11*11];

public:
    CBall(const CGame& game, CPoint position, CPoint dir) :
        mGame(game), mBall(position), mBallDir(dir)
    {
    }

    virtual Type GetType()
    {
        return Ball;
    }
        
    virtual void Draw()
    {
        rcLast = CRect(mBall.x/16 - mBallRadius, mBall.y/16 - mBallRadius,
                       mBall.x/16 + mBallRadius, mBall.y/16 + mBallRadius);

        BIOS::LCD::BufferBegin(rcLast);
        BIOS::LCD::BufferRead(ballbuf, 11*11);
        BIOS::LCD::BufferEnd();
        
        for (int y=-mBallRadius; y<mBallRadius; y++)
            for (int x=-mBallRadius; x<mBallRadius; x++)
                if (x*x+y*y < mBallRadiusSq-10)
                    BIOS::LCD::PutPixel(mBall.x/16 + x, mBall.y/16 + y, RGB565(d0d0d0));
                else if (x*x+y*y < mBallRadiusSq)
                    BIOS::LCD::PutPixel(mBall.x/16 + x, mBall.y/16 + y, RGB565(808080));
    }
    
    bool Surface(int i, int& x, int& y)
    {
        if (i < 0 || i >= 8)
            return false;
        
        static const int dirx[] = {-4, 0, +4, 0,  -3, -3, +3, +3};
        static const int diry[] = {0, -4, 0, +4,  -3, +3, -3, +3};
        x = mBall.x/16+dirx[i];
        y = mBall.y/16+diry[i];
        return true;
    }
    
    void SurfaceHit(int i)
    {
        static const int dirx[] = {-4, 0, +4, 0,  -3, -3, +3, +3};
        static const int diry[] = {0, -4, 0, +4,  -3, +3, -3, +3};
        int dx = dirx[i];
        int dy = diry[i];
        if (dx > 0)
            mBallDir.x = -abs(mBallDir.x);
        if (dx < 0)
            mBallDir.x = +abs(mBallDir.x);
        if (dy > 0)
            mBallDir.y = -abs(mBallDir.y);
        if (dy < 0)
            mBallDir.y = +abs(mBallDir.y);
    }
    
    virtual void Hide()
    {
        if (rcLast.IsValid())
        {
            BIOS::LCD::BufferBegin(rcLast);
            BIOS::LCD::BufferWrite(ballbuf, 11*11);
            BIOS::LCD::BufferEnd();
        }
    }
    
    virtual void Move()
    {
        const CRect& screenRect = mGame.GetScreenRect();
        CPoint mPaddle(mBall.x/16, screenRect.bottom-5);
        
        mBall.x += mBallDir.x;
        mBall.y += mBallDir.y;
        if (mBall.x/16 + mBallRadius >= screenRect.right-2 || mBall.x/16 - mBallRadius < screenRect.left+2)
            mBallDir.x = -mBallDir.x;
        if (mBall.y/16 + mBallRadius >= mPaddle.y || mBall.y/16 - mBallRadius < screenRect.top+2)
            mBallDir.y = -mBallDir.y;
    }
    
    virtual CRect GetRect() const
    {
        return rcLast;
    }
    
    int GetX()
    {
        return rcLast.CenterX();
    }
};

class CPaddle : public CObject
{
    const CGame& mGame;
    CRect rcLast;
    CRect rcCurrent;
    CPoint mPaddle;
    int mPaddleWidth;

public:
    CPaddle(const CGame& game, const CPoint& paddle, int paddleWidth) :
        mGame(game), mPaddle(paddle), mPaddleWidth(paddleWidth)
    {
    }

    virtual Type GetType()
    {
        return Paddle;
    }
    
    virtual void Hide()
    {
        if (rcLast.IsValid())
        {
            CRect rcTemp = rcLast;
            rcTemp.left = min(rcLast.left, rcCurrent.left+1);
            rcTemp.right = max(rcLast.left, rcCurrent.left+1);
            mGame.clearRect(mGame.GetScreenRect(), rcTemp);
            rcTemp.left = min(rcLast.right, rcCurrent.right-1);
            rcTemp.right = max(rcLast.right, rcCurrent.right-1);
            mGame.clearRect(mGame.GetScreenRect(), rcTemp);
        }

    }
    
    virtual void Draw()
    {
        int colors[] = {RGB565(ffff00), RGB565(eeee00), RGB565(dddd00), RGB565(cccc00), RGB565(bbbb00)};
        int ofs[] = {1, 0, 0, 0, 1};
        for (int i=0; i<5; i++)
            BIOS::LCD::Bar(mPaddle.x - mPaddleWidth + ofs[i], mPaddle.y+i,
                           mPaddle.x + mPaddleWidth - ofs[i], mPaddle.y+i+1, colors[i]);

        
        rcLast = rcCurrent;
        rcCurrent = CRect(mPaddle.x - mPaddleWidth, mPaddle.y, mPaddle.x + mPaddleWidth, mPaddle.y + 5);
    }
    
    virtual void Move()
    {
    }
    
    virtual CRect GetRect() const
    {
        return rcLast;
    }
    
    void SetX(int x)
    {
        if (x > mGame.GetScreenRect().left + mPaddleWidth && x < mGame.GetScreenRect().right-mPaddleWidth)
        {
            mPaddle.x = x;
            Move();
        }
    }
};

class CPowerup : public CObject
{
    const CGame& mGame;
    CRect rcLast;
    CRect rcCurrent;
    int _x;
    int _y;
    int phase;
    uint16_t powerupbuf[16*7];
    const uint32_t* powerup;
    const uint16_t* poweruppal;
    
public:
    CPowerup(const CGame& game, const CPoint& p) :
        mGame(game), _x(p.x), _y(p.y)
    {
        rcCurrent = CRect(_x, _y, _x+16, _y+7);
        int t = rand()%10;
        const static uint32_t* powerups[] = {powerup0, powerup1, powerup2, powerup3, powerup4, powerup5, powerup6, powerup7, powerup8, powerup9};
        const static uint16_t* poweruppals[] = {powerup0pal, powerup1pal, powerup2pal, powerup3pal, powerup4pal, powerup5pal, powerup6pal, powerup7pal, powerup8pal, powerup9pal};
        
        phase = 0;
        powerup = powerups[t];
        poweruppal = poweruppals[t];
    }
    
    virtual Type GetType()
    {
        return (_y > mGame.GetScreenRect().bottom-10) ? None : Powerup;
    }
    
    virtual void Hide()
    {
        if (rcLast.IsValid())
        {
            BIOS::LCD::BufferBegin(rcLast);
            BIOS::LCD::BufferWrite(powerupbuf, 16*7);
            BIOS::LCD::BufferEnd();
        }
    }
    
    virtual void Draw()
    {
        rcCurrent = CRect(_x, _y, _x+16, _y+7);
        
        BIOS::LCD::BufferBegin(rcCurrent);
        BIOS::LCD::BufferRead(powerupbuf, 16*7);
        BIOS::LCD::BufferEnd();
        
        int p = (phase/4) & 7;
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
    
    virtual CRect GetRect() const
    {
        return rcCurrent;
    }
    
    virtual void Move()
    {
        phase++;
        _y++;
    }

};

class CEnemy : public CObject
{
    const CGame& mGame;
    CRect rcLast;
    CRect rcCurrent;
    int _x;
    int _y;
    int _dx;
    int phase;
    uint16_t powerupbuf[16*16];
    
public:
    CEnemy(const CGame& game, const CPoint& p) :
        mGame(game), _x(p.x*16), _y(p.y*16)
    {
        phase = 0;
        rcCurrent = CRect(_x, _y, _x+16, _y+16);
        _dx = -12;
    }
    
    virtual Type GetType()
    {
        return Enemy;
    }
    
    virtual void Hide()
    {
        if (rcLast.IsValid())
        {
            BIOS::LCD::BufferBegin(rcLast);
            BIOS::LCD::BufferWrite(powerupbuf, 16*16);
            BIOS::LCD::BufferEnd();
        }
    }
    
    virtual void Draw()
    {
        rcCurrent = CRect(_x/16, _y/16, _x/16+16, _y/16+16);
        
        BIOS::LCD::BufferBegin(rcCurrent);
        BIOS::LCD::BufferRead(powerupbuf, 16*16);
        BIOS::LCD::BufferEnd();
        
        int p = (phase/5) % 24;
        rcLast = rcCurrent;
        for (int y=0; y<16; y++)
            for (int x=0; x<16; x++)
            {
                int pix = enemy2[p*2+y*2*24+x/8] >> (28-4*(x&7));
                pix &= 0xf;
                int color = enemy2pal[pix];
                if (color != RGBTRANS)
                    BIOS::LCD::PutPixel(_x/16+x, _y/16+y, color);
            }
    }
    
    virtual CRect GetRect() const
    {
        return rcCurrent;
    }
    
    virtual void Move()
    {
        phase++;
        _x+=_dx;
        if ((_x+_dx)/16+16 > mGame.GetScreenRect().right)
            _dx = -abs(_dx);
        if ((_x+_dx)/16 < mGame.GetScreenRect().left)
            _dx = +abs(_dx);
    }

};


class CObjectContainer
{
    uint8_t data[64];
};

class CGameBreakout
{
    CGame mGame;
    
    static const int mapCols{13};
    static const int mapRows{12};
    int map[mapCols*mapRows];
    
    CObject* mObjectsData[32];
    CArray<CObject*> mObjects;

public:
    void setup(CRect screen)
    {
        mGame.setup(screen);
        //mScreen = screen;

        mObjects.Init(mObjectsData, COUNT(mObjectsData));
        CPoint ballPoint = screen.Center();
        ballPoint.y = screen.bottom-20;
        ballPoint.x *= 16;
        ballPoint.y *= 16;
        mObjects.Add(new CBall(mGame, ballPoint, CPoint(16, -16)));
        //mObjects.Add(new CBall(mGame, ballPoint, CPoint(-10, -22)));

        CPoint paddle(ballPoint.x/16, screen.bottom-8);
        mObjects.Add(new CPaddle(mGame, paddle, 20));

        mObjects.Add(new CEnemy(mGame, CPoint(screen.CenterX(), screen.top+2)));

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

        mGame.drawBackground(screen);
        BIOS::LCD::Rectangle(screen, RGB565(ffffff));
        drawMap();
    }
    
    void loop()
    {
        for (int i=mObjects.GetSize()-1; i >= 0; i--)
            mObjects[i]->Hide();
        CBall* firstBall = nullptr;
        CPaddle* firstPaddle = nullptr;
        
        for (int i=0; i<mObjects.GetSize(); i++)
        {
            CObject* pObj = mObjects[i];
            pObj->Move();
            if (pObj->GetType() == CObject::Ball)
            {
                if (!firstBall)
                    firstBall = (CBall*)pObj;
                
                if (CheckBricks((CBall*)pObj))
                {
                    //if (rand()%10 == 0)
                        //mObjects.Add(new CPowerup(mGame, pObj->GetRect().Center()));

                }
            }
            if (pObj->GetType() == CObject::Paddle && !firstPaddle)
            {
                firstPaddle = (CPaddle*)pObj;
            }
            if (pObj->GetType() == CObject::None)
            {
                delete pObj;
                mObjects.RemoveAt(i--);
                continue;
            }
        }

        firstPaddle->SetX(firstBall->GetX());
        
        for (int i=0; i<mObjects.GetSize(); i++)
            mObjects[i]->Draw();
    }
    
    bool CheckBricks(CBall* pObj)
    {
        bool hit = false;
        for (int i=0, x=0, y=0; pObj->Surface(i, x, y); i++)
        {
            int p = GetBlockByPos(x, y);
            
            if (p != -1 && map[p] != -1)
            {
                removeBlock(p);
                pObj->SurfaceHit(i);
                hit = true;
                break;
            }
        }
        return hit;
    }
    
    void removeBlock(int p)
    {
        if (map[p] == 6)
            return;
        
        map[p] = -1;
        CRect rc = GetBlockRect(p);
        mGame.clearRect(mGame.GetScreenRect(), rc);
        rc.Deflate(2, 0, 2, 0);
        
        CObject* pPowerup = new CPowerup(mGame, rc.TopLeft());
        if (pPowerup)
            mObjects.Add(pPowerup);
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
        const CRect& screen = mGame.GetScreenRect();
        CRect inner(screen.left+2, screen.top+2, screen.right-1, screen.top + screen.Height()*6/10);
        
        if (!inner.IsInside(ptx, pty))
            return -1;

        int bx = (ptx - inner.left)*mapCols/inner.Width();
        int by = (pty - inner.top)*mapRows/inner.Height();
        
        return by*mapCols + bx;
    }
    
    CRect GetBlockRect(int p)
    {
        const CRect& screen = mGame.GetScreenRect();
        CRect inner(screen.left+2, screen.top+2, screen.right-1, screen.top + screen.Height()*6/10);
        
        int x = p % mapCols;
        int y = p / mapCols;
        
        return CRect(
            inner.left + inner.Width()*x/mapCols,
            inner.top + inner.Height()*y/mapRows,
            inner.left + inner.Width()*(x+1)/mapCols-1,
            inner.top + inner.Height()*(y+1)/mapRows-1
        );
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
    //BIOS::SYS::DelayMs(20);
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
