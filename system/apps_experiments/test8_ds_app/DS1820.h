#pragma once
#include "Pin.h"
#include "Delay.h"

/*
	Usage:
	// connect DS1820/DS18B20 to UART-TX pin, no pullup necessary when the cable is short

	m_Thermometer.Create( BIOS::GPIO::PortA, 9 );

	m_Thermometer.Init();
	m_Thermometer.ReadRom();

	ui8 nFamilyCode = m_Thermometer.GetScratchpad()[0];
	float fTemp;
	m_Thermometer.Init();
	m_Thermometer.Convert(nFamilyCode);
	m_Thermometer.Temperature(fTemp, nFamilyCode);
*/

class CDS1820 : public CPin, CDelay
{
	ui8 m_arrScratchpad[9];

public:
	enum {
		DS18S20_FAMILY_CODE = 0x10,
		DS18B20_FAMILY_CODE = 0x28,
		DS1822_FAMILY_CODE = 0x22
	};

public:
	bool Init()
	{
		High();
		DelayUs(50);	
		Low();
		DelayUs(500);
		High();
		DelayUs(100);
		if ( Get() )
			return false;
		DelayUs(400);
		return true;
	}

	bool ReadRom()
	{
		if ( !Init() )
			return false;

		Out(0x33);
		for (int n = 0; n < 8; n++)
			m_arrScratchpad[n] = In();
		return true;
	}

	bool SetConfig(ui8 nTh, ui8 nTl, ui8 nConf )
	{
		if ( !Init() )
			return false;

		Out(0xcc);
		Out(0x4e);	// write scratchpad
		Out(nTh);	// Th/user
		Out(nTl);	// Tl/user
		Out(nConf);	// config -> 12 bit resolution 750ms / conv
		return true;
	}

	bool Convert(int nFamilyCode)
	{
		if ( !Init() )
			return false;

		Out(0xcc);	// skip ROM
		Out(0x44);	// perform temperature conversion

		if ( nFamilyCode == DS18B20_FAMILY_CODE )
			Power();

		// conversion takes max. 750ms
		DelayMs(750+20);

		if ( !Init() )
			return false;	
	
		Out(0xcc);
		Out(0xbe);	// read result
	
		for (int n = 0; n < 9; n++)
			m_arrScratchpad[n] = In();

		return true;
	}

	bool Temperature(float& fTemp, int nFamilyCode)
	{
		if ( nFamilyCode == DS18S20_FAMILY_CODE )
		{
			si16 stemp = (m_arrScratchpad[1] << 8) | m_arrScratchpad[0];
			int temp = stemp;
			temp >>= 1; // cut LSB
			temp <<= 4;
			if ( m_arrScratchpad[/*DS_REG_CntPerDeg*/ 7] != 0x10 )
				return false;
			temp -= 4;
			temp += m_arrScratchpad[/*DS_REG_CntPerDeg*/ 7];
			temp -= m_arrScratchpad[/*DS_REG_CntRemain*/ 6];
			fTemp = temp / 16.0f;
			return true;
		}
		if ( nFamilyCode == DS18B20_FAMILY_CODE )
		{
			si16 temp = (m_arrScratchpad[1] << 8) | m_arrScratchpad[0];
			fTemp = temp / 16.0f;
			return true;
		}

		return false;
	}

	bool Crc(int nLen)
	{
		int shift_reg=0, data_bit, sr_lsb, fb_bit;
   	
   	for (int i=0; i< nLen; i++) /* for each byte */
   	{
   	   for(int j=0; j<8; j++)   /* for each bit */
   	   {
   	      data_bit = (m_arrScratchpad[i]>>j)&0x01;
   	      sr_lsb = shift_reg & 0x01;
   	      fb_bit = (data_bit ^ sr_lsb) & 0x01;
   	      shift_reg = shift_reg >> 1;
   	      if (fb_bit)
   	      {
   	         shift_reg = shift_reg ^ 0x8c;
   	      }
   	   }
		}
		return ( shift_reg == 0 ) ? true : false;
	}

	ui8* GetScratchpad()
	{
		return m_arrScratchpad;
	}

//private:
	// override High method to HighZ + pullup
	void High()
	{
		CPin::Float();
	}
	void Power()
	{
		CPin::High();
	}
	ui8 In(void)
	{
		ui8 i_byte = 0;
		for (int n=0; n<8; n++)
		{
			Low();	
			DelayUs(5);
			High();
			DelayUs(5);

      i_byte >>= 1;
      if ( Get() )
        i_byte |= 0x80;	// least sig bit first

			DelayUs(50);
		}
		return i_byte;
	}
	
	void Out(ui8 d)
	{
		for(int n=0; n<8; n++)
		{
			Low();
			if (d & 1)
			{
			DelayUs(5);
				High();
				DelayUs(55);
			} else {        
				DelayUs(55);
				High();
			DelayUs(5);

			}
      d >>= 1;
		}
	}

};
