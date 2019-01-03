#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "screenshot.h"

using namespace BIOS;

void _HandleAssertion(const char* file, int line, const char* cond)
{
    DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

// Prevent moving into BSS section
__attribute__((__section__(".persistent")))
static OS::TInterruptHandler pOldSysTick;

bool bLastPressed = false;
int nLastImage = 0;

void Screenshot()
{
  char strFilename[16];
  sprintf(strFilename, "scr%03d.bmp", nLastImage++);
  if (!SaveScreenshot16(strFilename))
    BIOS::DBG::Print("$Failed to create '%s'.", strFilename);
}

void SysTick()
{
  typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
  Tfunc __Bios = (Tfunc)(0x08000100|1); 
  constexpr uint32_t KEYnDEV = 6;
  constexpr uint32_t BITMAP = 0xFFFC0000;

  bool bCurrentPressed = (~__Bios(KEYnDEV, BITMAP) & 0xf) == 0b0100;
  if (!bLastPressed && bCurrentPressed)
  {
    Screenshot();
  }

  bLastPressed = bCurrentPressed;
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
    DBG::Print("$Unloading screenshotter. Reverting timer to %08x\n", pOldSysTick);
    OS::SetInterruptVector(OS::ISysTick, pOldSysTick);
  } else
  {
    pOldSysTick = pCurrentSysTick;
    OS::SetInterruptVector(OS::ISysTick, SysTick);
    DBG::Print("$Screenshotter loaded, press F3 to activate.\n"); 
  }

  SYS::DelayMs(1000);
  // TSR - Terminate and stay resident
  return 0;
}
