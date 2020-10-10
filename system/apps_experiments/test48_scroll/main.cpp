#include <library.h>
using namespace BIOS;
    CRect window(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14);
    constexpr int lines = 20;
    uint16_t buffer[BIOS::LCD::Width*lines];

    void Scroll(const CRect& view, int offset)
    {
        for (int y=view.top; y<view.bottom; y+=lines)
        {
            int maxlines = min(lines, view.bottom-offset-y);
            CRect rcOrigin(window.left, y+offset, window.right, y+offset+maxlines);
            CRect rcTarget(window.left, y, window.right, y+maxlines);
            BIOS::LCD::BufferBegin(rcOrigin);
            BIOS::LCD::BufferRead(buffer, rcOrigin.Width()*maxlines);
            BIOS::LCD::BufferEnd();

            BIOS::LCD::BufferBegin(rcTarget);
            BIOS::LCD::BufferWrite(buffer, rcOrigin.Width()*maxlines);
            BIOS::LCD::BufferEnd();
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
    
    int scrollBy = 5;
    CRect rcTarget(window.left, window.bottom-scrollBy, window.right, window.bottom);

    int nScrolls = 0;
    int nBegin = BIOS::SYS::GetTick();
    BIOS::KEY::EKey key;
    int c = 0;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      Scroll(window, scrollBy);
      for (int x=0; x<BIOS::LCD::Width; x++)
        buffer[x] = c++;

      BIOS::LCD::BufferBegin(rcTarget);
      for (int i=0; i<scrollBy; i++)
        BIOS::LCD::BufferWrite(buffer, BIOS::LCD::Width);

      nScrolls++;

      EVERY(5000)
      {
        int nNow = BIOS::SYS::GetTick();
        float nScrollsPerSecond = nScrolls*1000.0f / (nNow-nBegin);
        BIOS::LCD::Printf(40, window.bottom-20, RGB565(ffffff), RGB565(404040), "Scrolls per second = %f", nScrollsPerSecond);

        nScrolls = 0;
        nBegin = BIOS::SYS::GetTick();
      }
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

