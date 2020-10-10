#include <library.h>
using namespace BIOS;

#define MODE_B

namespace CONSOLE
{
    CRect window(0, 16, BIOS::LCD::Width, BIOS::LCD::Height-14);
    CPoint cursor(0, 16);
    uint16_t colorFront(RGB565(ffffff));
    uint16_t colorBack(RGB565(0000b0));

    void Color(uint16_t c)
    {
        colorFront = c;
    }

#ifdef MODE_B
    void Scroll(const CRect& view, int offset)
    {
        uint16_t buffer[BIOS::LCD::Width];
        for (int y=view.top; y<view.bottom-offset; y++)
        {
            CRect rcOrigin(window.left, y+offset, window.right, y+offset+1);
            CRect rcTarget(window.left, y, window.right, y+1);
            BIOS::LCD::BufferBegin(rcOrigin);
            BIOS::LCD::BufferRead(buffer, rcOrigin.Width());
            BIOS::LCD::BufferEnd();

            BIOS::LCD::BufferBegin(rcTarget);
            BIOS::LCD::BufferWrite(buffer, rcOrigin.Width());
            BIOS::LCD::BufferEnd();

        }
    }
#endif

    void Putch(char c)
    {
#ifdef MODE_B
        if (cursor.y >= window.bottom)
        {
            cursor.y -= 14;
            Scroll(window, 14);
            BIOS::LCD::Bar(window.left, cursor.y, window.right, cursor.y + 14, colorBack);
        }
#endif

        if (c != 0x0d && c != 0x0a)
        {
            cursor.x += BIOS::LCD::Print(cursor.x, cursor.y, colorFront, colorBack, c);
            if (cursor.x+8 < window.right)
                return;
        }

        cursor.x = window.left;
        cursor.y += 14;


#ifdef MODE_A
        if (cursor.y >= window.bottom)
            cursor.y = 0;
        BIOS::LCD::Bar(window.left, cursor.y, window.right, cursor.y + 14, colorBack);
#endif
    }
    
    
    void Print(const char * format, ...)
    {
        char buf[128];
        char* bbuf = buf;
        
        va_list args;
        
        va_start( args, format );
        vsprintf( bbuf, format, args );
        va_end(args);
        
        while (*bbuf)
            Putch(*bbuf++);
    }

    
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rcTop(rcClient);
    rcTop.bottom = 14;
    GUI::Background(rcTop, RGB565(4040b0), RGB565(404040));
    BIOS::LCD::Print(0, 0, RGB565(ffffff), RGBTRANS, "\x10 Scrolling console test");


    CRect rcBottom(rcClient);
    rcBottom.top = rcBottom.bottom-14;
    GUI::Background(rcBottom, RGB565(202020), RGB565(202020));
    BIOS::LCD::Print(0, rcBottom.top, RGB565(b0b0b0), RGBTRANS, "\x07 Status line");
    
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
        static int n = 0;
        CONSOLE::Color(n&1 ? RGB565(ffffff) : RGB565(b0b0b0));
        CONSOLE::Print("This is line %d!\n", n++);
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(") [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

