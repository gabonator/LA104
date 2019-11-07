#include <library.h>

using namespace BIOS;

void Loop();
void Load();
void Unload();

__attribute__((__section__(".persistent")))
static OS::TInterruptHandler pOldSysTick;

void SysTick()
{
  Loop();
  pOldSysTick();
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
  LCD::Clear(RGB565(404040));

  OS::TInterruptHandler pCurrentSysTick = OS::GetInterruptVector(OS::ISysTick);

  bool bLoaded = pCurrentSysTick == SysTick;

  if (bLoaded)
  {
    Unload();
    DBG::Print("$Unloading USB remote control.\n");
    OS::SetInterruptVector(OS::ISysTick, pOldSysTick);
  } else
  {
    pOldSysTick = pCurrentSysTick;
    OS::SetInterruptVector(OS::ISysTick, SysTick);
    Load();
    DBG::Print("$USB Remote control loaded.\n"); 
  }
  SYS::DelayMs(1000);

/*
  Load();
  KEY::EKey key;
  while ((key = KEY::GetKey()) != KEY::Escape)
  {
    Loop();
  }
  SYS::DelayMs(1000);
  Unload();
*/
  // TSR - Terminate and stay resident
  return 0;
}
