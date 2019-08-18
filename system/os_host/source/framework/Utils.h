#pragma once

#include "Types.h"
#include <math.h>
#include <stdint.h>

namespace CUtils
{
	int atoi(char *str);
	ui32 htoi(char *str);
	char* itoa(si16 i);
	char* itoa2(ui8 n);
//	static char* ftoa(float f);
	char tohex(ui8 n);
	char* clrhex(ui16 c);
/*
	static char* MidiNote(int n);
	static char* FormatVoltage( float fV, int nChars=8 );
	static char* FormatFrequency( float fF, int nChars=8 );
	static char* FormatTime( float fT, int nChars=8 );
	static char* FormatFloat5( float f );
*/
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
	ui16 InterpolateColor( ui16 clrA, ui16 clrB, int nLevel );
	int Sqrt(int a);
}
