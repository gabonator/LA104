#include "SmartResponseXE.h"

int palette[] = {RGB565(404040), RGB565(ff0000), RGB565(008000), RGB565(00ff00), RGB565(ffffff)};
/*
char SRXEGetKey()
{
  return 0;
}*/
int ofsx = 30;
int ofsy = 30;

void SRXEWriteString(int x, int y, const char* text, int font, int clrf, int clrb)
{
    //x = x*5/2;
  //x = x*5/2;
  //y = y*2;
    x = x * 2 / 3 + ofsx;
    y += ofsy;
    if (x + strlen(text)*8 >= BIOS::LCD::Width)
        x = BIOS::LCD::Width - strlen(text)*8;
    if (clrb==-1)
        BIOS::LCD::Print(x, y, palette[clrf], /*palette[clrb])*/ RGB565(ffffff), text);
    else
        BIOS::LCD::Print(x, y, palette[clrf], /*palette[clrb])*/ RGB565(000000), text);
}

void delay(int l)
{
  BIOS::SYS::DelayMs(l);
}

void SRXESleep()
{
  _ASSERT(0);
}

void SRXEHorizontalLine(int x, int y, int w, int c, int b)
{
  x = x*2+ofsx;
    y = y + ofsy;
  w = w*2;
  //y = y*2;
  //w = w*5/2;
  BIOS::LCD::Bar(x, y, x+w, y+1, palette[c]);
}

void SRXEVerticalLine(int x, int y, int h, int c)
{
  x = x*2 + ofsx;
    y = y + ofsy;
   // x = x*5/2;
  //x = x*5/2;
  //y = y*2;
  //h = h*5/2;
  BIOS::LCD::Bar(x, y, x+2, y+h, palette[c]);
}
/*
void itoa(int n, char* buf, int base)
{
  sprintf(buf, "%d", n);
}
*/
