#include <library.h>
#include "manager.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}


CWndUserManager manager;

__attribute__((__section__(".entry"))) 
int _main(void)
{    
  manager.Create(nullptr, CWnd::WsVisible);
  manager.SetFocus();
  manager.OnMessage(nullptr, ToWord('L', 'E'), 0);
  manager.WindowMessage( CWnd::WmPaint );

  BIOS::OS::SetArgument((char*)"");    
  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    if (key != BIOS::KEY::None)
      manager.WindowMessage(CWnd::WmKey, key);

    manager.WindowMessage(CWnd::WmTick);

    if (BIOS::OS::GetArgument()[0])
      break;
  }
//  BIOS::DBG::Print("executing='%s'\n", BIOS::OS::GetArgument());
//  BIOS::SYS::DelayMs(2000);
//  manager.Destroy(); // CRASH!
//  BIOS::DBG::Print("exiting\n");
  return 0;
}
