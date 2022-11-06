#include <library.h>
#include "Manager.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}

CWndManager manager;
#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
  manager.Create(nullptr, CWnd::WsVisible);
  manager.SetFocus();
//  manager.OnMessage(nullptr, ToWord('L', 'E'), 0);
  manager.WindowMessage( CWnd::WmPaint );

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    if (key != BIOS::KEY::None)
      manager.WindowMessage(CWnd::WmKey, key);

    manager.WindowMessage(CWnd::WmTick);

    if (BIOS::OS::HasArgument()) // was internally set
    {
      BIOS::OS::SetArgument(BIOS::OS::GetArgument()); // hack, keep flag set
      break;
    }
  }
  return 0;
}
