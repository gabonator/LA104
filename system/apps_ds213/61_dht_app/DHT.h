#pragma once
#include "Pin.h"
#include "Delay.h"

#define DHT11 11  /**< DHT TYPE 11 */
#define DHT12 12  /**< DHY TYPE 12 */
#define DHT22 22  /**< DHT TYPE 22 */
#define DHT21 21  /**< DHT TYPE 21 */
#define AM2301 21 /**< AM2301 */

const uint8_t _type = DHT11;

const int Samples =  BIOS::ADC::NumSamples;
static BIOS::ADC::TSample::SampleType memory[Samples];

class CDHT : public CPin, CDelay
{
	uint8_t m_buffer[5];
	uint32_t ch1Value;
	uint16_t nSample1;
	uint16_t nSample2;
	bool init = false;
	uint8_t adcValue;

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
		CopySamples();
		// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
		for(int i = 0; i < BIOS::ADC::NumSamples;)
		{
			if(!init){
				nSample1 = 0;
				//Detect Init
				do{
					ch1Value = memory[i];
					adcValue = (uint8_t)((ch1Value) & 0xff);
					nSample1++;
					i++;
					if(i >= BIOS::ADC::NumSamples){ return false; }
				}while(Check(adcValue) == false);
				//BIOS::DBG::Print("Sample1: %d\n", nSample1);
				nSample1 = 0;
				do{
					ch1Value = memory[i];
					adcValue = (uint8_t)((ch1Value) & 0xff);
					nSample1++;
					i++;
					if(i >= BIOS::ADC::NumSamples){ return false; }
				}while(Check(adcValue) == true);
				//BIOS::DBG::Print("Sample2: %d\n", nSample1);
				init = true;
			}
			i++;
			ch1Value = memory[i];
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
		float f = 0;
		switch (_type) {
		case DHT11:
		case DHT12:
			f = m_buffer[0] + m_buffer[1] * 0.1;
			break;
		case DHT22:
		case DHT21:
			f = ((uint32_t)m_buffer[0]) << 8 | m_buffer[1];
			f *= 0.1;
			break;
		}
		return f;
	}

	float GetTemperature()
	{
		float f = 0;
		switch (_type) {
		case DHT11:
			f = m_buffer[2];
			if (m_buffer[3] & 0x80) {
				f = -1 - f;
			}
			f += (m_buffer[3] & 0x0f) * 0.1;
			break;
		case DHT12:
			f = m_buffer[2];
			f += (m_buffer[3] & 0x0f) * 0.1;
			if (m_buffer[2] & 0x80) {
				f *= -1;
			}
			break;
		case DHT22:
		case DHT21:
			f = ((uint32_t)(m_buffer[2] & 0x7F)) << 8 | m_buffer[3];
			f *= 0.1;
			if (m_buffer[2] & 0x80) {
				f *= -1;
			}
			break;
		}
		return f;
	}


	bool CheckCrc()
	{
		return m_buffer[4] == (ui8)(m_buffer[0] + m_buffer[1] + m_buffer[2] + m_buffer[3]);
	}

	void CopySamples()
	{
		for (int i=0; i<Samples; i++)
		{
			memory[i] = BIOS::ADC::Get();
		}
	}
};
