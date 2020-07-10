#include "STM32F10x.h"
#include "Snake.h"
#include "la104bios.h"

vu32 vu32Tick = 0;

extern "C" {
void TIM3_IRQHandler(void)
{ 
  // TODO: not working, not called
  vu32Tick++;
}
}

void BIOS::SYS::Beep( int ms )
{
}

void Assert(const char *msg, int n)
{
}

void BIOS::LCD::Pattern(int x1, int y1, int x2, int y2, const ui16 *pat, int l)
{
  if (x2 > 320)
    return;

  __Bios(BLOCK_X, (x2-1) | (x1 << 16));
  __Bios(BLOCK_Y, (y2-1) | (y1 << 16));

  const ui16* patb = pat;
  const ui16* pate = patb + l;

  for (int y=y1; y<y2; y++)
    for (int x=x1; x<x2; x++)
    {
      __Bios(WrPIXEL, *pat);
      if (++pat == pate) 
        pat = patb;
    }
}

ui32 BIOS::SYS::GetTick()
{
  return vu32Tick;
}

int main(void)
{ 
  CWndSnake snake;
  snake.Create(nullptr, CWnd::WsVisible);
  snake.OnMessage(nullptr, ToWord('L', 'E'), 0);
  snake.SetFocus();
  while (1)
  {
    vu32Tick += 1000;

    ui16 KeyHold = __Bios(KEYnDEV, BITMAP);

    if (KeyHold & K1_HOLD)
      snake.WindowMessage(CWnd::WmKey, BIOS::KEY::KeyLeft);
    if (KeyHold & K2_HOLD)
      snake.WindowMessage(CWnd::WmKey, BIOS::KEY::KeyRight);

    snake.WindowMessage(CWnd::WmTick, 0);
    snake.WindowMessage(CWnd::WmPaint, 0);
  }
  return 0;
}
