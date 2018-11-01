#include "Bios.h"

void BIOS::SYS::DelayUs(int intervalUs)
{
  assert(0);
}

void BIOS::SYS::DelayMs(int intervalMs)
{
  //assert(0);
}

uint32_t BIOS::SYS::GetTick()
{
  return SDL_GetTicks();
}

int BIOS::SYS::Execute(uint32_t addr)
{
  assert(0);
  return 0;
}

void BIOS::SYS::Beep(int intervalMs)
{
}
