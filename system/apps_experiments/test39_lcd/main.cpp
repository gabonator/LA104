#include <math.h>
#include <library.h>
#include "lcd.h"

__attribute__((__section__(".entry"))) int main(void)
{ 
  SetScrollArea(0, 0);
  while (BIOS::KEY::GetKey() == BIOS::KEY::None)
  {
    int i=(int)(sin(BIOS::SYS::GetTick()*0.005f)*159+160);
    SetScrollPosition(i);
  }
  SetScrollPosition(0);
  return 0;
}
