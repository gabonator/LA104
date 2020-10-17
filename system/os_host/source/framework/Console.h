#pragma once
#define CONSOLE_MODE_B

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

#ifdef CONSOLE_MODE_B
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
#ifdef CONSOLE_MODE_B
        if (cursor.y + 14 > window.bottom)
        {
            cursor.y -= 14;
            Scroll(window, 14);
            BIOS::LCD::Bar(window.left, cursor.y, window.right, cursor.y + 14, colorBack);
        }
#endif
        if (c == 0x08)
        {
          cursor.x -= 8;
          if (cursor.x < window.left)
            cursor.x = window.left;
          return;
        }
        if (c != 0x0d && c != 0x0a)
        {
            cursor.x += BIOS::LCD::Print(cursor.x, cursor.y, colorFront, colorBack, c);
            if (cursor.x+7 < window.right)
                return;
        }

        cursor.x = window.left;
        cursor.y += 14;


#ifdef CONSOLE_MODE_A
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
