#include <library.h>
//#include "../../os_host/source/bios/Bios.h"
//#include "../../os_host/source/framework/Wnd.h"
//#include "../../os_host/source/gui/Gui.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

#include "CharMap.h"

#ifndef EMSCRIPTEN
__attribute__((__section__(".entry"))) int main(void)
{ 
  CWndCharMap app;

  app.Create(nullptr, CWnd::WsVisible);
  app.OnMessage(nullptr, ToWord('L', 'E'), 0);
  app.SetFocus();
  app.OnPaint();

  BIOS::KEY::EKey key = BIOS::KEY::None;
  while (key != BIOS::KEY::Escape)
  {
    key = BIOS::KEY::GetKey();
    if (key != BIOS::KEY::None)
      app.WindowMessage(CWnd::WmKey, key);

    app.WindowMessage(CWnd::WmTick, 0);
  }
  return 0;
}

#else
CWndCharMap app;

void mainInit()
{
  app.Create(nullptr, CWnd::WsVisible);
  app.OnMessage(nullptr, ToWord('L', 'E'), 0);
  app.SetFocus();
  app.OnPaint();
}

bool mainLoop()
{
  BIOS::KEY::EKey key = BIOS::KEY::GetKey();

  if (key == BIOS::KEY::Escape)
    return false;

  if (key != BIOS::KEY::None)
    app.WindowMessage(CWnd::WmKey, key);

  app.WindowMessage(CWnd::WmTick, 0);
  return true;
}

void mainFinish()
{
}
#endif