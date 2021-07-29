#include "CoreOscilloscope.h"
#include "Framework.h"

/*static*/ void CCoreOscilloscope::ConfigureAdc()
{
	BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH1, BIOS::ADC::ECouple::DC, BIOS::ADC::EResolution::_1V , 105*4);
	BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH2, BIOS::ADC::ECouple::DC, BIOS::ADC::EResolution::_1V , 122*4);
	BIOS::ADC::ConfigureTimebase(50e-6f);
//	BIOS::ADC::ConfigureBuffer( arrLen[ (NATIVEENUM)Settings.Time.Resolution ] );
	BIOS::ADC::Restart(0);
}


/*static*/ void CCoreOscilloscope::ConfigureTrigger()
{
	BIOS::ADC::ConfigureTrigger(0, 0, BIOS::ADC::ETriggerType::None, BIOS::ADC::EInput::CH1 );
}

/*static*//* void CCoreOscilloscope::UpdateConstants()
{
	Settings.Runtime.m_fTimeRes = CSettings::TimeBase::pfValueResolution[ Settings.Time.Resolution ];
	Settings.Runtime.m_nScreenDuration = (int)(Settings.Runtime.m_fTimeRes*1000*10);

	Settings.Runtime.m_fCH1Res  = 
		CSettings::AnalogChannel::pfValueResolution[ Settings.CH1.Resolution ] *
		CSettings::AnalogChannel::pfValueProbe[ Settings.CH1.Probe ];
	Settings.Runtime.m_fCH2Res  = 
		CSettings::AnalogChannel::pfValueResolution[ Settings.CH2.Resolution ] *
		CSettings::AnalogChannel::pfValueProbe[ Settings.CH2.Probe ];
}*/

/*static*/ void CCoreOscilloscope::GetMaskAt(int nX, int** pLow, int** pHigh)
{
	// desperate situation, we are running out of RAM... we will share the ADC buffer (every fourth byte is unused)
	BIOS::ADC::TSample& ts1 = (BIOS::ADC::TSample&)Sampler::GetAt( nX );
	BIOS::ADC::TSample& ts2 = (BIOS::ADC::TSample&)Sampler::GetAt( nX+300 );
	_ASSERT( nX >= 0 && nX < 300 );
	*pLow = &((int*)&ts1)[3];
	*pHigh = &((int*)&ts2)[3];
}

/*static*/ void CCoreOscilloscope::GetMaskStats(int** nPass, int** nFail)
{
	static int g_nPass = 0;
	static int g_nFail = 0;
	*nPass = &g_nPass;
	*nFail = &g_nFail;
}
