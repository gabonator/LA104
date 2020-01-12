#include "CoreGenerator.h"
#include <Settings.h>

/*static*/ ui16 CCoreGenerator::m_pRamWave[128*4];
/*static*/ int CCoreGenerator::m_nRamLen = 0;
/*static*/ ui8 CCoreGenerator::m_pVolatile[256];
/*static*/ int CCoreGenerator::m_nVolatileLen = 0;

const ui16 pSineWave[] =
			{0x000,0x027,0x08E,0x130,0x209,0x311,0x441,0x58F,0x6F0,    // 90
			0x85A,0x9C0,0xB19,0xC59,0xD76,0xE68,0xF26,0xFAB,0xFF3,    // 180
			0xFFF,0xFD7,0xF70,0xECE,0xDF5,0xCED,0xBBD,0xA6F,0x90E,    // 270
			0x7A4,0x63E,0x4E5,0x3A5,0x288,0x196,0x0D8,0x053,0x00B};   // 360   

const ui16 pTriangleWave[] =
			{0x000,0x0E3,0x1C6,0x2AA,0x38D,0x471,0x554,0x638,0x71B,    // 90
			0x7FF,0x8E2,0x9C6,0xAA9,0xB8D,0xC70,0xD54,0xE37,0xF1B,    // 180
			0xFFE,0xF1B,0xE37,0xD54,0xC70,0xB8D,0xAA9,0x9C6,0x8E2,    // 270
			0x7FF,0x71B,0x638,0x554,0x471,0x38D,0x2AA,0x1C6,0x0E3};    // 360   

const ui16 pSawWave[] =
			{0x000,0x075,0x0EA,0x15F,0x1D4,0x249,0x2BE,0x333,0x3A8,    // 90
			0x41D,0x492,0x507,0x57C,0x5F1,0x666,0x6DB,0x750,0x7C5,    // 180
			0x83A,0x8AF,0x924,0x999,0xA0E,0xA83,0xAF8,0xB6D,0xBE2,    // 270
			0xC57,0xCCC,0xD41,0xDB6,0xE2B,0xEA0,0xF15,0xF8A,0xFFF};    // 360    

const ui16 pSquareWave[] =
			{0x000,0xfff};    // 90

const ui16 pCardiac128Wave[] = {
	0x3bd, 0x397, 0x391, 0x391, 0x396, 0x393, 0x3a2, 0x3b3, 0x3b1, 0x39f, 0x38f, 0x383, 0x39b, 0x3a3, 0x3a0, 0x3aa, 
	0x3ac, 0x3bc, 0x3d0, 0x3f2, 0x41c, 0x439, 0x425, 0x401, 0x3f1, 0x3c5, 0x392, 0x37a, 0x374, 0x37a, 0x37f, 0x386, 
	0x385, 0x381, 0x36c, 0x385, 0x3a1, 0x39c, 0x394, 0x321, 0x176, 0x1bf, 0x3ad, 0x728, 0xc38, 0xee3, 0xfff, 0x9f6, 
	0x410, 0x181, 0x000, 0x0e0, 0x154, 0x1b8, 0x1e8, 0x219, 0x238, 0x24f, 0x288, 0x2b8, 0x2d6, 0x2ed, 0x319, 0x33a, 
	0x34c, 0x371, 0x38f, 0x3aa, 0x3b7, 0x3c7, 0x3e4, 0x3fc, 0x413, 0x423, 0x437, 0x439, 0x450, 0x470, 0x47f, 0x48a, 
	0x482, 0x48b, 0x49a, 0x499, 0x48e, 0x48a, 0x477, 0x44a, 0x426, 0x3e6, 0x386, 0x324, 0x2c3, 0x280, 0x25f, 0x244, 
	0x234, 0x233, 0x235, 0x251, 0x275, 0x295, 0x2a6, 0x2b0, 0x2c8, 0x2e7, 0x300, 0x316, 0x328, 0x32e, 0x32f, 0x33f, 
	0x350, 0x351, 0x352, 0x351, 0x34a, 0x356, 0x35f, 0x35d, 0x363, 0x367, 0x36b, 0x377, 0x384, 0x37a, 0x378, 0x369 };

/*static*/ const CCoreGenerator::TWaveData CCoreGenerator::Waves[] = 
	{
		{(char*)"DC", 0, NULL, 0},
		{(char*)"sin - HQ", 36, pSineWave, 2 },
		{(char*)"sin - LQ", 36, pSineWave, 0 },
		{(char*)"triangle", 36, pTriangleWave, 2 },
		{(char*)"sawtooth", 36, pSawWave, 2 },
		{(char*)"square", 2, pSquareWave, 0 },
		{(char*)"cardio", 128, pCardiac128Wave, 2 },
		{(char*)"Volatile", 0, NULL, 0 },
		{NULL, 0, NULL, 0}		
	};

/*static*/ const CCoreGenerator::TWaveData* CCoreGenerator::GetWave( int nWave )
{
	return &Waves[nWave];
}

/*static*/ ui16* CCoreGenerator::GetRamDac()
{
	return m_pRamWave;
}

/*static*/ int CCoreGenerator::GetRamLen()
{
	return m_nRamLen;
}

/*static*/ ui8* CCoreGenerator::GetVolatile()
{
	return m_pVolatile;
}

/*static*/ int& CCoreGenerator::GetVolatileLen()
{
	return m_nVolatileLen;
}

ui16 CCoreGenerator::_GetCount(ui8 nWaveIndex)
{
	if ( nWaveIndex == CSettings::Generator::_Volatile )
	{
		_ASSERT( m_nVolatileLen > 0 );
		return m_nVolatileLen; // oversampling?
	}
	// handle DC ?
	ui16 nCount = Waves[nWaveIndex].nCount;
	ui16 nSmooth = Waves[nWaveIndex].nOversampling;

	return nCount << nSmooth;
}

ui16* CCoreGenerator::_GetWave(ui8 nWaveIndex)
{
	const ui16* pWave = Waves[nWaveIndex].pWave;
	ui16 nCount = Waves[nWaveIndex].nCount;
	ui16 nSmooth = Waves[nWaveIndex].nOversampling;
	_ASSERT( _GetCount(nWaveIndex) <= COUNT( m_pRamWave ) );
	int nLen = nCount << nSmooth;

	int nScale = Settings.Gen.nScale;
	int nOffset = Settings.Gen.nOffset >> 4;

	if ( nWaveIndex == CSettings::Generator::_Volatile )
	{
		int nLen = m_nVolatileLen;
		for (int i=0; i<nLen; i++)
		{
			int nSample = m_pVolatile[i];
			nSample -= 128;
			nSample = ( nSample * nScale ) >> (16-4);
			nSample += nOffset;
			if ( nSample < 0 )
				nSample = 0;
			if ( nSample > 0xfff )
				nSample = 0xfff;
			m_pRamWave[i] = nSample;
		}
		m_nRamLen = nLen;
		Settings.Gen.nSamples = nLen;
		return m_pRamWave;
	}

	if ( nSmooth <= 0 )
	{
		for (int i=0; i<nCount; i++)
			m_pRamWave[i] = pWave[i];
	} else
	{
		ui16* pSample = m_pRamWave;
		ui8 nSmoothVals = 1<<nSmooth;
		for (int i=0; i<nCount; i++)
		{
			ui16 nSample0 = pWave[i];
			ui16 nSample1 = pWave[(i+1)%nCount];
			
			for (ui8 j=0; j<nSmoothVals; j++)
			{
				ui32 nInterpolation = nSample0;
				nInterpolation += ( ( (ui32)(nSample1-nSample0) ) * j ) >> nSmooth;
				*pSample++ = (ui16)nInterpolation;
			}
		}
	}

	// scaling & offset
	if ( nScale != 0x10000 || nOffset != 0x800 )
	{
		for (int i=0; i<nLen; i++)
		{
			int nSample = m_pRamWave[i];
			nSample -= 0x800;
			nSample = ( nSample * nScale ) >> 16;
			nSample += nOffset;
			if ( nSample < 0 )
				nSample = 0;
			if ( nSample > 0xfff )
				nSample = 0xfff;
			m_pRamWave[i] = nSample;
		}
	}
	m_nRamLen = nLen;
	Settings.Gen.nSamples = nLen;
	return m_pRamWave;
}

void CCoreGenerator::CopyToVolatile( int nWave )
{
	m_nVolatileLen = Waves[nWave].nCount;
	const ui16* pData = Waves[nWave].pWave;
	for ( int i = 0; i < m_nVolatileLen; i++, pData++ )
		m_pVolatile[i] = *pData >> 4;
}

void CCoreGenerator::Update()
{
	int nWaveIndex = Settings.Gen.Wave;

	switch (Settings.Gen.Wave)
	{
		case CSettings::Generator::_Dc:
			ConfigureDc( Settings.Gen.nOffset >> 4 );
			break;

		case CSettings::Generator::_Square:
			ConfigureSq( Settings.Gen.nPsc, Settings.Gen.nArr, Settings.Gen.nCcr);
			break;

		case CSettings::Generator::_Volatile:
			if ( m_nVolatileLen == 0 )
				CopyToVolatile( CSettings::Generator::_SinLq );

			ConfigureWave( _GetWave(nWaveIndex), _GetCount(nWaveIndex) );
			ConfigureWaveRate( Settings.Gen.nArr );
			break;

		default:
			ConfigureWave( _GetWave(nWaveIndex), _GetCount(nWaveIndex) );
			ConfigureWaveRate( Settings.Gen.nArr );
	}
}

int CCoreGenerator::GetDuty()
{
	return Settings.Gen.nCcr * 100 / Settings.Gen.nArr;
}

void CCoreGenerator::SetDuty(int nPercent)
{
	_ASSERT( (Settings.Gen.nArr) * nPercent / 100 >= 0 );
	Settings.Gen.nCcr = (Settings.Gen.nArr) * nPercent / 100;
}

  #define CPUCLOCK (72 MHz)
  #define MHz *1000000.0f


void CCoreGenerator::ConfigureSq( int psc, int arr, int ccr )
{
  BIOS::DAC::SetMode(BIOS::DAC::EMode::Square, nullptr, 0);
  BIOS::DAC::SetFrequency(CPUCLOCK / (arr + 1) / (psc + 1));
}

void CCoreGenerator::ConfigureWaveRate( int arr )
{
  BIOS::DAC::SetFrequency(CPUCLOCK / 20 / ( arr + 1 ));
}

void CCoreGenerator::ConfigureDc( int dc )
{
  static ui16 sData;
  sData = min(dc, BIOS::DAC::SampleMaxValue);
  ConfigureWave( &sData, 1 );
}

void CCoreGenerator::ConfigureWave( uint16_t* p, int n)
{
  BIOS::DAC::SetMode(BIOS::DAC::EMode::Buffer, p, n);
}

