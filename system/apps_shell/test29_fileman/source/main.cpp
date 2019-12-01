#include <library.h>
#include "Manager.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}

CWndUserManager manager;
#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
#ifndef EMSCRIPTEN
int _main(void)
{    
  manager.Create(nullptr, CWnd::WsVisible);
  manager.SetFocus();
  manager.OnMessage(nullptr, ToWord('L', 'E'), 0);
  manager.WindowMessage( CWnd::WmPaint );

//  BIOS::OS::SetArgument((char*)"");    
  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    if (key != BIOS::KEY::None)
      manager.WindowMessage(CWnd::WmKey, key);

    manager.WindowMessage(CWnd::WmTick);

//    if (BIOS::OS::GetArgument()[0])
    if (BIOS::OS::HasArgument()) // was internally set
    {
      BIOS::OS::SetArgument(BIOS::OS::GetArgument()); // hack, keep flag set
      break;
    }
  }
//  BIOS::DBG::Print("executing='%s'\n", BIOS::OS::GetArgument());
//  BIOS::SYS::DelayMs(2000);
//  manager.Destroy(); // CRASH!
//  BIOS::DBG::Print("exiting\n");
  return 0;
}
#else
void mainInit()
{
  BIOS::FAT::Init();
  manager.Create(nullptr, CWnd::WsVisible);
  manager.SetFocus();
  manager.OnMessage(nullptr, ToWord('L', 'E'), 0);
  manager.WindowMessage( CWnd::WmPaint );
}

bool mainLoop()
{
//  BIOS::OS::SetArgument((char*)"");    
  BIOS::KEY::EKey key;
  key = BIOS::KEY::GetKey();
  if (key == BIOS::KEY::Escape)
    return false;

  if (key != BIOS::KEY::None)
    manager.WindowMessage(CWnd::WmKey, key);

  manager.WindowMessage(CWnd::WmTick);

//    if (BIOS::OS::GetArgument()[0])
  if (BIOS::OS::HasArgument()) // was internally set
  {
    BIOS::OS::SetArgument(BIOS::OS::GetArgument()); // hack, keep flag set
    return false;
  }
  return true;
}

void mainFinish()
{
}
#endif