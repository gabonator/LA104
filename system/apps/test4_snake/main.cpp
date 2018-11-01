#include <library.h>
#include "Snake.h"

__attribute__((__section__(".entry"))) int main(void)
{ 
  CWndSnake snake;

  snake.Create(nullptr, CWnd::WsVisible);
  snake.OnMessage(nullptr, ToWord('L', 'E'), 0);
  snake.SetFocus();
  snake.OnPaint();

  BIOS::KEY::EKey key = BIOS::KEY::None;
  while (key != BIOS::KEY::Escape)
  {
/*
    if (!snake.IsRunning())
    {
      BIOS::DBG::Print("Restart\n");
      snake.WindowMessage(CWnd::WmPaint, 0);
    }
*/    
    key = BIOS::KEY::GetKey();
    if (key != BIOS::KEY::None)
      snake.WindowMessage(CWnd::WmKey, key);

    snake.WindowMessage(CWnd::WmTick, 0);
  }
  return 0;
}
