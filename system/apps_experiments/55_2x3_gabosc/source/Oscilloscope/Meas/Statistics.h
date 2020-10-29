#pragma once
#include <Framework.h>
#include <Settings.h>

class CMeasStatistics
{
	friend class CSerialDecoder;

	float m_fMin, m_fMax, m_fSum, m_fSumR, m_fSum2;
	int m_nCount;
	int m_nRawMin, m_nRawMax;
	CSettings::Measure::ESource m_curSrc;
	CSettings::Measure::ERange m_curRange;
	CSettings::Calibrator::FastCalc fastCalc1;
	CSettings::Calibrator::FastCalc fastCalc2;

public:
	bool Process( CSettings::Measure::ESource src, CSettings::Measure::ERange range );
	float GetPeriod();
	float GetFreq(); 
	float GetChannelsDelta(bool rising);
	float GetSigma(); // variance
	float GetBaud();
	float GetMin();
	float GetMax();
	float GetAvg();
	float GetRms();
	float GetRectAvg();
	float GetVpp();
	float GetTime(bool bHighLevel);
	float GetEdgeTime(bool bRising);
	float GetFormFactor();
	float GetDispersion();
	float GetPwm();
	float GetActivePower();
	float GetReactivePower();
	float GetApparentPower();

private:
	bool _GetRange( int& nBegin, int& nEnd, CSettings::Measure::ERange range );
	float _GetSamplef( BIOS::ADC::TSample::SampleType& nSample );
	int _GetSample( BIOS::ADC::TSample::SampleType nSample );
	bool _GetEffectiveValuesForPower(float &fVoltage, float &fCurrent);
	// nRising - 1(rising), 0(both), -1(falling)
	int _FindEdge( CSettings::Measure::ESource src, CSettings::Measure::ERange range, si8 nRising, int nOffset, int &nTrigState ); 
};
