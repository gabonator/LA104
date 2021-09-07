#include <library.h>

uint16_t gradient[25];
uint16_t backBuffer[21*21];
CRect backRect;
int ball[11] = {10, 10, 10, 9, 9, 8, 8, 7, 6, 4, 0};

void InitBall()
{
    for (int i=0; i<COUNT(gradient); i++)
        gradient[i] = RGB565RGB(50-i*2, 120-i*5, 255);
}

void DrawBall(int bx, int by)
{
    backRect = CRect(bx-10, by-10, bx+11, by+11);
    BIOS::LCD::BufferBegin(backRect);
    BIOS::LCD::BufferRead(backBuffer, COUNT(backBuffer));
    BIOS::LCD::BufferEnd();

    const int r = 10;
    for (int y =-10; y<=10; y++)
    {
        int px = ball[abs(y)];
        int ofsx = 10-px;
        BIOS::LCD::BufferBegin(CRect(bx-10+ofsx, by+y, bx+21, by+y+1));
        BIOS::LCD::BufferWrite(gradient+(y+10)/2, px*2);
        BIOS::LCD::BufferEnd();
    }
}

void HideBall()
{
    BIOS::LCD::BufferBegin(backRect);
    BIOS::LCD::BufferWrite(backBuffer, COUNT(backBuffer));
    BIOS::LCD::BufferEnd();
}

int ballX = 100;
int ballY = 100;
int moveX = 2;
int moveY = 2;

int main(void)
{
    InitBall();
    while (!BIOS::KEY::GetKey())
    {
        DrawBall(ballX, ballY);
        BIOS::SYS::DelayMs(10);
        ballX += moveX;
        ballY += moveY;
        if (ballX + 10 > BIOS::LCD::Width-1 || ballX <= 10)
            moveX = -moveX;
        if (ballY + 10 > BIOS::LCD::Height-16 || ballY <= 10+16)
            moveY = -moveY;
        HideBall();
    }
    return 72;
}
