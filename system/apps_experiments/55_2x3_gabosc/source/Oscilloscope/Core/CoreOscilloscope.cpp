#include "CoreOscilloscope.h"
#include <Framework.h>
#include <Settings.h>

/*static*/ void CCoreOscilloscope::ConfigureAdc()
{
	/*
	const static ui16 arrPsc[] = 
	{ 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 10-1, 10-1, 10-1, 100-1, 100-1, 
	100-1, 1000-1, 1000-1, 1000-1, 1000-1, 1000-1 };
	const static ui16 arrArr[] = 
	{ 1-1, 1-1, 2-1, 3-1, 5-1, 12-1, 24-1, 48-1, 120-1, 240-1, 480-1, 120-1, 240-1, 480-1,
	120-1, 240-1, 480-1, 120-1, 240-1, 480-1, 1200-1, 1000-1};
	const static int arrLen[] = 
	{ 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 2048, 2048, 2048, 2048, 
	1024, 1024, 1024, 512, 512, 512 };
	*/
//	_ASSERT( COUNT(arrPsc) == COUNT(arrArr) && COUNT(arrPsc) == CSettings::TimeBase::_ResolutionMax+1 );

	ui8 nACouple = Settings.CH1.Coupling == CSettings::AnalogChannel::_DC ? 0 : 1;
	ui8 nARange = (ui8)(NATIVEENUM)Settings.CH1.Resolution;
	ui16 nAOffset = Settings.CH1.u16Position;
	ui8 nBCouple = Settings.CH2.Coupling == CSettings::AnalogChannel::_DC ? 0 : 1;
	ui8 nBRange = (ui8)(NATIVEENUM)Settings.CH2.Resolution;
	ui16 nBPosition = Settings.CH2.u16Position;

	float fTimePerDiv = CSettings::TimeBase::pfValueResolution[ (NATIVEENUM)Settings.Time.Resolution ];
	//ui16 nTimePsc = arrPsc[ (NATIVEENUM)Settings.Time.Resolution ];
	//ui16 nTimeArr = arrArr[ (NATIVEENUM)Settings.Time.Resolution ];
	
	BIOS::ADC::Enable(true);
	BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH1, (BIOS::ADC::ECouple)nACouple, (BIOS::ADC::EResolution)nARange, nAOffset);
	BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH2, (BIOS::ADC::ECouple)nBCouple, (BIOS::ADC::EResolution)nBRange, nBPosition);
	BIOS::ADC::ConfigureTimebase(fTimePerDiv);
	BIOS::ADC::ConfigureTimebase(fTimePerDiv);
//	BIOS::ADC::ConfigureBuffer( arrLen[ (NATIVEENUM)Settings.Time.Resolution ] );
	BIOS::ADC::Restart(0);

	UpdateConstants();
}

/*static*/ void CCoreOscilloscope::ConfigureTrigger()
{
	if ( Settings.Trig.Sync == CSettings::Trigger::_None || Settings.Trig.Sync == CSettings::Trigger::_Scan )
	{
		BIOS::ADC::ConfigureTrigger(0, 0, BIOS::ADC::ETriggerType::None, BIOS::ADC::EInput::CH1 );
	} else
	{
		BIOS::ADC::ConfigureTrigger(Settings.Trig.nTime, Settings.Trig.nLevel, 
			(BIOS::ADC::ETriggerType)Settings.Trig.Type, (BIOS::ADC::EInput)Settings.Trig.Source);
	}
}

/*static*/ void CCoreOscilloscope::UpdateConstants()
{
	Settings.Runtime.m_fTimeRes = CSettings::TimeBase::pfValueResolution[ Settings.Time.Resolution ];
	Settings.Runtime.m_fCH1Res  = 
		CSettings::AnalogChannel::pfValueResolution[ Settings.CH1.Resolution ] *
		CSettings::AnalogChannel::pfValueProbe[ Settings.CH1.Probe ];
	Settings.Runtime.m_fCH2Res  = 
		CSettings::AnalogChannel::pfValueResolution[ Settings.CH2.Resolution ] *
		CSettings::AnalogChannel::pfValueProbe[ Settings.CH2.Probe ];
}

/*static*/ void CCoreOscilloscope::GetMaskAt(int nX, ui8** pLow, ui8** pHigh)
{
	// desperate situation, we are running out of RAM... we will share the ADC buffer (every fourth byte is unused)
	BIOS::ADC::TSample& ts1 = (BIOS::ADC::TSample&)Sampler::GetAt( nX );
	BIOS::ADC::TSample& ts2 = (BIOS::ADC::TSample&)Sampler::GetAt( nX+300 );

	_ASSERT( nX >= 0 && nX < 300 );
	*pLow = &((ui8*)&ts1)[3];
	*pHigh = &((ui8*)&ts2)[3];
}

/*static*/ void CCoreOscilloscope::GetMaskStats(int** nPass, int** nFail)
{
	static int g_nPass = 0;
	static int g_nFail = 0;
	*nPass = &g_nPass;
	*nFail = &g_nFail;
}
