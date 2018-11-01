#include <library.h>
#include "shapes.h"
#include "DS1820.h"

// TODO: import DelayUs/DelayMs

class CVisualMeasure
{
  int pwm[4] = {0};
  int focus = 0;
  int _x, _y;
  CDS1820 m_Thermometer;

public:
  void Create()
  {
    m_Thermometer.Create( BIOS::GPIO::P1 );
  }

	void UpdateDs1820()
	{
char buf[64];

		bool bInit = false, bConv = false, bCrc = false, bTemp = false, bRom = false;
		float fTemp = 0;
		ui8* pSP = m_Thermometer.GetScratchpad();
		ui16 clr;
		ui8 nFamilyCode = 0;
		bInit = m_Thermometer.Init();
		memset( pSP, 0, 9 );


//	        BIOS::LCD::Print(20, 20, RGB565(ffffff), RGB565(b0b0b0), bInit ? "Init ok!" : "Init err");

		if ( bInit )
			bRom = m_Thermometer.ReadRom();
			
//                BIOS::LCD::Print(20, 20+16, RGB565(ffffff), RGB565(b0b0b0), bRom ? "rom ok!" : "rom err");

		if ( bRom )
		{
			if ( m_Thermometer.Crc(8) )
				nFamilyCode = pSP[0];
		}

		clr = (bRom && nFamilyCode) ? RGB565(ffff00) : RGB565(000000);
		BIOS::LCD::Print(_x, _y+0x20, clr, RGB565(b0b0b0), "ROM=");
		for (int i=0; i<8; i++)
{
  sprintf(buf, "%02x", pSP[i]);
			BIOS::LCD::Print(_x+4*8+i*18, _y+0x20, clr, RGB565(b0b0b0), buf);
}

		const char* strFamily = "Unknown";
		switch ( nFamilyCode )
		{
			case CDS1820::DS18S20_FAMILY_CODE: strFamily = "DS18S20"; break;
			case CDS1820::DS18B20_FAMILY_CODE: strFamily = "DS18B20"; break;
			case CDS1820::DS1822_FAMILY_CODE: strFamily = "DS1822"; break;
		}

  sprintf(buf, "Family code = (%x%x) %s  ", nFamilyCode>>4, nFamilyCode&0xf, strFamily);

		BIOS::LCD::Print(_x, _y+0x30, clr, RGB565(b0b0b0), buf);

		memset( pSP, 0, 9 );
		if ( bInit )
			bConv = m_Thermometer.Convert(nFamilyCode);
		if ( bConv )
			bCrc = m_Thermometer.Crc(9);
		if ( bCrc && nFamilyCode )
			bTemp = m_Thermometer.Temperature(fTemp, nFamilyCode);

		const char* strStatus = "Ok!";
		if (!bInit)
			strStatus = "Err:Init";
		else if (!bConv)        
			strStatus = "Err:Conv";
		else if (!bCrc)
			strStatus = "Err:Crc";
		else if (!bTemp)
			strStatus = "Err:Temp";

  sprintf(buf, "OneWire status = %s     ", strStatus);
		BIOS::LCD::Print(_x, _y+0x10, RGB565(ffFF00), RGB565(b0b0b0), buf );
		
		clr = (bConv && bCrc) ? RGB565(ffff00) : RGB565(000000);

		BIOS::LCD::Print(_x, _y+0x40, clr, RGB565(b0b0b0), "SPD=");
		for (int i=0; i<9; i++)
{
  sprintf(buf, "%02x", pSP[i]);

			BIOS::LCD::Print(_x+4*8+i*18, _y+0x40, clr, RGB565(b0b0b0), buf);
}

		clr = bTemp ? RGB565(ffff00) : RGB565(000000);
  sprintf(buf, "Temperature = %2f" "\xf8" "C   ", fTemp);

		BIOS::LCD::Print(_x, _y+0x50, clr, RGB565(b0b0b0), buf);

		if ( nFamilyCode == CDS1820::DS18B20_FAMILY_CODE && pSP[4] != 0x7f ) 
		{
			m_Thermometer.SetConfig( 0, 0, 0x7f );	// set 12 bits resolution
		}
	}

  void OnPaint()
  {
    CRect rc(0, 0, 320, 240);
    GUI::Background(rc, RGB565(404040), RGB565(101010));

    CRect rc1(0, 0, 320, 14);
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    CRect rc2(20, 16+50, 320-20, 240-50);
    GUI::Window(rc2, RGB565(ffffff));
    
    int x = 0;
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " DS1820 test");
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
    x += BIOS::LCD::Print (rc2.right/2 + rc2.left/2- 21*4, rc2.top + 2, RGB565(000000), RGBTRANS, "DS1820 measure report");

    _x = rc2.left + 8;
    _y = rc2.top + 2 + 16 + 8 - 24;
    UpdateDs1820();
  }

  void OnKey(BIOS::KEY::EKey key)
  {
    UpdateDs1820();
    CDelay::DelayMs(500);
  }

};

__attribute__((__section__(".entry"))) int main(void)
{ 
  CVisualMeasure app;
  app.Create();
  app.OnPaint();

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    app.OnKey(key);

  return 0;
}
