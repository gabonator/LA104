#include "Bios.h"

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


void BIOS::SYS::DelayUs(int intervalUs)
{
  // TODO: volatile?
  intervalUs = 63 * intervalUs / 8;
  while(intervalUs--)
    __asm__("");
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
//  Beep(true);
//  gBeepCounter = intervalMs;
}


/*
class CDelay
{
public:
	static void DelayUs(ui32 us)
	{
		us = us*12;
		while (us--)
		{
			__asm__("");
		}
	}

	static void DelayMs(ui32 ms)
	{
		while (ms--)
		{
			ui32 us = 12000;
			while (us--)
			{
				__asm__("");
			}
		}
	}
};
*/