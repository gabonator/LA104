#include <library.h>

class Ball 
{
    int x;
    int y;
    int vx;
    int vy;
    uint16_t gradient[25];
    CRect backRect;

public:
    Ball()
    {
        x = (random()%(BIOS::LCD::Width-40))+20;
        y = (random()%(BIOS::LCD::Height-40))+20;
        vx = (random() & 3)+1;
        vy = (random() & 3)+1;
        
        for (int i=0; i<COUNT(gradient); i++)
        {
            switch (x%3)
            {
                case 0: 
                    gradient[i] = RGB565RGB(50-i*2, 120-i*5, 255);
                    break;
                case 1: 
                    gradient[i] = RGB565RGB(50-i*2, 255, 120-i*5);
                    break;
                case 2: 
                    gradient[i] = RGB565RGB(255, 50-i*2, 120-i*5);
                    break;
            }
        }
    }
    
    void Draw()
    {
        static constexpr int curve[11] = {10, 10, 10, 9, 9, 8, 8, 7, 6, 4, 0};
        
        backRect = CRect(x-10, y-10, x+11, y+11);
        const int r = 10;
        for (int _y=-10; _y<=10; _y++)
        {
            int px = curve[abs(_y)];
            int ofsx = 10-px;
            BIOS::LCD::BufferBegin(CRect(x-10+ofsx, y+_y, x+21, y+_y+1));
            BIOS::LCD::BufferWrite(gradient+(_y+10)/2, px*2);
            BIOS::LCD::BufferEnd();
        }
    }
    
    void Hide()
    {
        BIOS::LCD::Bar(backRect, RGB565(404040));
    }
    
    void Move()
    {
        x += vx;
        y += vy;
        if (x + vx + 10 >= BIOS::LCD::Width-1 || x + vx <= 10)
        {
            vx = -vx;
            BIOS::SYS::Beep(5);
        }
        if (y + vy + 10 > BIOS::LCD::Height-16 || y + vy <= 10+16)
        {
            vy = -vy;
            BIOS::SYS::Beep(5);
        }
    }
};

Ball ballsData[16];
CArray<Ball> balls(ballsData, COUNT(ballsData));

int main(void)
{
    balls.SetSize(balls.GetMaxSize());
    while (!BIOS::KEY::GetKey())
    {
        for (int i=0; i<balls.GetSize(); i++)
            balls[i].Draw();
            
        BIOS::SYS::DelayMs(10);
        
        for (int i=0; i<balls.GetSize(); i++)
            balls[i].Move();
            
        for (int i=0; i<balls.GetSize(); i++)
            balls[i].Hide();
    }
    return 72;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
}
