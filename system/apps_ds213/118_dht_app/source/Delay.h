#pragma once
#include <library.h>
#define delayUS_ASM(us) do {\
	asm volatile (	"MOV R0,%[loops]\n\t"\
			"1: \n\t"\
			"SUB R0, #1\n\t"\
			"CMP R0, #0\n\t"\
			"BNE 1b \n\t" : : [loops] "r" (16*us) : "memory"\
		      );\
} while(0)

class CDelay
{
public:

	static void DelayUs(uint32_t us)
	{
		us = us*12;
		while (us--)
		{
			__asm__("");
		}
	}

	static void DelayMs(uint32_t ms)
	{
		while (ms--)
		{
			uint32_t us = 12000;
			while (us--)
			{
				__asm__("");
			}
		}
	}
};
