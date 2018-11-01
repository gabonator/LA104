#pragma once
#include "Pin.h"
#include "Delay.h"

class CDHT : public CPin, CDelay
{
	ui8 m_buffer[5];

public:
	ui8* GetBuffer()
	{
		return m_buffer;
	}

	bool Init()
	{
		// REQUEST SAMPLE
		Low();
		DelayMs(1);
//		High();
//		DelayUs(40);
		Float();

int d1 = Delay(1000, true);
if (d1==1000)
  return false;

int d2 = Delay(1000, false);
if (d2==1000)
  return false;

int d3 = Delay(1000, true);
if (d3==1000)
  return false;


		return true;
	}

	int Delay(int nUs, bool bLevel)
	{
		int i;
		nUs *= 2;
		for ( i = 0; i < nUs; i += 3 )
		{
			if ( Get() != bLevel )
				break;
			DelayUs(1); // single loop takes probably 1.5us, multiply 3/2 x
		}
		if ( i >= nUs-3 )
			i = nUs;

		return i / 2;
	}

	bool Read()
	{
		// BUFFER TO RECEIVE
		ui8 cnt = 7;
		ui8 idx = 0;

		// EMPTY BUFFER
		for (int i=0; i<5; i++) 
			m_buffer[i] = 0;

		// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
		for (int i=0; i<40; i++)
		{
		   int a = Delay( 200, false );
		   int b = Delay( 200, true );

		   if (a<b)
		     m_buffer[idx] |= 1 << cnt;

			if ( cnt == 0 )
			{
				cnt = 7;
				idx++;
			} else
				cnt--;
		}

//BIOS::DBG::Print("%d/%d %d/%d %d/%d %d/%d %d/%d\n", 
//  temp[0], temp[1], temp[2], temp[3], temp[4], 
//  temp[5], temp[6], temp[7], temp[8], temp[9]);
		return true;
	}

	float GetHumidity()
	{
		return (m_buffer[0]*256 + m_buffer[1]) * 0.1f;
	}

	float GetTemperature()
	{
		return (m_buffer[2]*256 + m_buffer[3]) * 0.1f;
	}

	bool CheckCrc()
	{
		return m_buffer[4] == (ui8)(m_buffer[0] + m_buffer[1] + m_buffer[2] + m_buffer[3]);
	}
};
