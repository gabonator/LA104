#include <library.h>
#include "dcf77.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}


CWndDcf77 dcf77;

__attribute__((__section__(".entry"))) 
int _main(void)
{    
  dcf77.Create(nullptr, CWnd::WsVisible | CWnd::WsTick);
  dcf77.SetFocus();
  dcf77.OnMessage(nullptr, ToWord('L', 'E'), 0);
  dcf77.WindowMessage( CWnd::WmPaint );

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    if (key != BIOS::KEY::None)
      dcf77.WindowMessage(CWnd::WmKey, key);

    dcf77.WindowMessage(CWnd::WmTick);
  }
  return 0;
}
