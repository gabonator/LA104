#include "Bios.h"
#include "imports.h"

extern "C" volatile uint32_t gCounter;
extern "C" volatile uint32_t gBeepCounter;
extern "C" volatile uint32_t Dly_mS;
extern "C" void Beep(bool);
extern "C" 
void Delay_mS(uint32_t mS)
{
    Dly_mS = mS;
    while(Dly_mS);
}

void BIOS::SYS::Init()
{
  HardwareInit();
}

void BIOS::SYS::DelayMs(int intervalMs)
{
  Delay_mS(intervalMs);
}

uint32_t BIOS::SYS::GetTick()
{
  return gCounter;
}

int BIOS::SYS::Execute(uint32_t addr)
{
//  u32 *vector_table = (u32 *) dwGotoAddr;
//  __MSR_MSP(vector_table[0]);
  addr |= 1; // thumb
  return ((int (*)(void)) addr)();
}

void BIOS::SYS::Beep(int intervalMs)
{
  if (intervalMs == 0)
  {
    xBeep(false);
    return;
  }

  xBeep(true);
  gBeepCounter = intervalMs;
}

bool BIOS::SYS::LoadFpga(char* path)
{
  return false;
}
