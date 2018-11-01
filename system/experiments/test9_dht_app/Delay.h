#pragma once
#include <library.h>

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
