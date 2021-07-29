#pragma once
#include "Pin.h"
#include "Delay.h"
#include "Sampler.h"

class CDHT : public CPin, CDelay
{
	ui8 m_buffer[5];
	volatile uint32_t ch1Value;
	uint16_t nSample1;
	uint16_t nSample2;
	bool init = false;
	volatile uint8_t adcValue;

public:
	ui8* GetBuffer()
	{
		return m_buffer;
	}

	bool Check(int value)
	{
		return (value > 15) ? true : false;
	}

	bool Read()
	{
		// BUFFER TO RECEIVE
		uint8_t cnt = 7;
		uint8_t idx = 0;
		// EMPTY BUFFER
		for (int i=0; i<5; i++) 
			m_buffer[i] = 0;

		High();
		DelayMs(800);
		Low();
		DelayMs(15);
		High();
		DelayUs(185);
		//Start Collect samples
		BIOS::ADC::Restart(0);
		DelayMs(50);
		Sampler::Copy();
		// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
		for(int i = 0; i < BIOS::ADC::NumSamples;)
		{
			if(!init){
				nSample1 = 0;
				//Detect Init
				do{
					ch1Value = Sampler::GetAt(i);
					adcValue = (uint8_t)((ch1Value) & 0xff);
					nSample1++;
					i++;
					if(i >= BIOS::ADC::NumSamples){ return false; }
				}while(Check(adcValue) == false);
				//BIOS::DBG::Print("Sample1: %d\n", nSample1);
				nSample1 = 0;
				do{
					ch1Value = Sampler::GetAt(i);
					adcValue = (uint8_t)((ch1Value) & 0xff);
					nSample1++;
					i++;
					if(i >= BIOS::ADC::NumSamples){ return false; }
				}while(Check(adcValue) == true);
				//BIOS::DBG::Print("Sample2: %d\n", nSample1);
				init = true;
			}
			i++;
			ch1Value = Sampler::GetAt(i);
			adcValue = (uint8_t)((ch1Value) & 0xff);
			if(Check(adcValue) == false){
				nSample1++;
			}
			if(Check(adcValue) == true)
			{
				nSample2++;
			}
			if(nSample1 > 0 && nSample2 > 0 && Check(adcValue) == false){
				if (nSample2 > nSample1)
				{
					m_buffer[idx] |= 1 << cnt;
				}
				if (cnt == 0)   // next byte?
				{
					cnt = 7;    // restart at MSB
					idx++;      // next byte!
				}
				else { cnt--; }
				nSample1 = 0;
				nSample2 = 0;
			}
		}
		//BIOS::DBG::Print("%x-%x-%x-%x-%x\n", m_buffer[0], m_buffer[1], m_buffer[2], m_buffer[3], m_buffer[4]);
		return true;
	}

	float GetHumidity()
	{
		return m_buffer[0];
	}

	float GetTemperature()
	{
		return m_buffer[2];
	}

	bool CheckCrc()
	{
		return m_buffer[4] == (ui8)(m_buffer[0] + m_buffer[1] + m_buffer[2] + m_buffer[3]);
	}
};
