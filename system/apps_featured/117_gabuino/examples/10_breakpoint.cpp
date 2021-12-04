#include <library.h>

int x = BIOS::LCD::Width/2, y = BIOS::LCD::Height/2;
int dx = 1, dy = 1;
char text[20] = "Hello";
volatile struct {uint8_t r, g, b; } color = {255, 0, 0};

int main(void)
{
    CRect inner(8, 14+8, BIOS::LCD::Width-8, BIOS::LCD::Height-14-8);
    BIOS::LCD::Bar(inner, RGB565(404040));
    BIOS::LCD::Rectangle(inner, RGB565(b0b0b0));
    while (!BIOS::KEY::GetKey())
    {
        x += dx;
        y += dy;
        
        CRect draw(x, y, x+strlen(text)*8, y+14);
        BIOS::LCD::Print(x, y, RGB565RGB(color.r, color.g, color.b), 
            RGB565(404040), text);

        if (draw.right+dx >= inner.right)       
        {
            BIOS::SYS::Beep(10);
            dx = -abs(dx);
        }
        if (draw.left+dx <= inner.left)
        {
            BIOS::SYS::Beep(10);
            dx = +abs(dx);
        }
        if (draw.bottom+dy >= inner.bottom)
        {
            BIOS::SYS::Beep(10);
            dy = -abs(dy);
        }
        if (draw.top+dy <= inner.top)  
        {
            BIOS::SYS::Beep(10);
            dy = +abs(dy);
        }
            
        BIOS::SYS::DelayMs(5);
        BIOS::LCD::Bar(draw, RGB565(404040));
    }
    return 0;
}