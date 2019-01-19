#include "Bios.h"

char gArgument[128] = "";

void BIOS::SYS::DelayUs(int intervalUs)
{
  assert(0);
}

void BIOS::SYS::DelayMs(int intervalMs)
{
    SDL_Delay(intervalMs);
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

void BIOS::OS::SetArgument(char* argument)
{
    _ASSERT(strlen(argument) < COUNT(gArgument)-1);
    strcpy(gArgument, argument);
}

char* BIOS::OS::GetArgument()
{
    return gArgument;
}


