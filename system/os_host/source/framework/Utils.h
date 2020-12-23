#pragma once

#include "Types.h"
#include <stdint.h>

namespace CUtils
{
	int atoi(char *str);
	uint32_t htoi(char *str);
	char* itoa(int16_t i);
	char* itoa2(uint8_t n);
	char tohex(uint8_t n);
	char* clrhex(uint16_t c);

	template <class T>
	inline void Clamp(T& nVariable, T nMin, T nMax)
	{
		if ( nVariable < nMin )
		{
			nVariable = nMin;
			return;
		}
		if ( nVariable > nMax )
		{
			nVariable = nMax;
			return;
		}
	}

	unsigned int Random();
	ui16 InterpolateColor( uint16_t clrA, uint16_t clrB, int nLevel );
	int Sqrt(int a);
}
