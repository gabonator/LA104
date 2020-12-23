#pragma once
#include <library.h>

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
