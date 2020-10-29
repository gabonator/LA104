#include "Statistics.h"
#include <MainWnd.h>
#include <math.h>
#include <Settings.h>

#include "Decoders/Serial.h"
#include "Decoders/CanBus.h"

float CMeasStatistics::_GetSamplef( BIOS::ADC::TSample::SampleType& nSample )
{
	float fSample = 0;
	if ( m_curSrc == CSettings::Measure::_CH1 )
	{
		nSample = (ui8)((nSample) & 0xff);
		fSample = Settings.CH1Calib.Voltage( fastCalc1, (float)nSample );
	}
	else if ( m_curSrc == CSettings::Measure::_CH2 )
	{
		nSample = (ui8)((nSample>>8) & 0xff);
		fSample = Settings.CH2Calib.Voltage( fastCalc2, (float)nSample );
	}
	else if ( m_curSrc == CSettings::Measure::_Math )
	{
		//nSample = ;
		int s = MainWnd.m_wndGraph.MathCalc( nSample );
		nSample = (s < 0) ? 0 : s;
		fSample = (s - Settings.Math.Position) / 32.0f * (Settings.CH1Calib.GetMultiplier(Settings.Math.Resolution) / 1);	// matches the vertical grid
	} else
	_ASSERT( 0 );
	return fSample;
}

int CMeasStatistics::_GetSample( BIOS::ADC::TSample::SampleType nSample )
{
	if ( m_curSrc == CSettings::Measure::_CH1 )
	{
		nSample = (ui8)((nSample) & 0xff);
	}
	else if ( m_curSrc == CSettings::Measure::_CH2 )
	{
		nSample = (ui8)((nSample>>8) & 0xff);
	}
	else if ( m_curSrc == CSettings::Measure::_Math )
	{
		nSample = MainWnd.m_wndGraph.MathCalc( nSample );
	}
	return nSample;
}

bool CMeasStatistics::_GetEffectiveValuesForPower(float &fVoltage, float &fCurrent)
{
	fVoltage = sqrt(m_fSum2/m_nCount);

	int nBegin = 0, nEnd = 0;
	float fSum2 = 0;
	if ( _GetRange( nBegin, nEnd, m_curRange ) )
	{
		CSettings::Measure::ESource tmp = m_curSrc;
		m_curSrc = CSettings::Measure::_Math;

		Settings.CH1Calib.Prepare( &Settings.CH1, fastCalc1 );
		Settings.CH2Calib.Prepare( &Settings.CH2, fastCalc2 );
		MainWnd.m_wndGraph.MathSetup( &fastCalc1, &fastCalc2 );

		for ( int i = nBegin; i < nEnd; i++ )
		{
			BIOS::ADC::TSample::SampleType nSample = Sampler::GetAt( i );
			float fSample, fSample2;
			fSample = _GetSamplef( nSample );
			fSample2 = fSample * fSample;

			fSum2 += fSample2;
		}
		m_curSrc = tmp;
		fCurrent = sqrt(fSum2/m_nCount);
		return (true);
	}
	return (false);
}

int CMeasStatistics::_FindEdge( CSettings::Measure::ESource src, CSettings::Measure::ERange range, si8 nRising, int nOffset, int &nTrigState )
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, range ) )
		return 0;
	
	if (nOffset > nBegin)
		nBegin = nOffset;

	int nThresh = 2;
	int nTrigMin = Settings.Trig.nLevel - nThresh;
	int nTrigMax = Settings.Trig.nLevel + nThresh;

	int nNewState = -1;

	m_curSrc = src;
	for ( int i = nBegin; i < nEnd; i++ )
	{
		int nSample_ = Sampler::GetAt(i);
		int nSample = _GetSample( nSample_ );

		if ( nSample > nTrigMax )
			nNewState = 1;
		if ( nSample < nTrigMin )
			nNewState = 0;

		if ( nTrigState != -1 && nNewState != nTrigState )
		{
			// trigger changing it's output
	 		nTrigState = nNewState;
			if ( ( nNewState == 1 ) && (nRising >= 0) )
			{
				// rising edge
				return i;
			}
			if ( ( nNewState == 0 ) && (nRising <= 0) )
			{
				// falling edge
				return i;
			}
		}
 		nTrigState = nNewState;
	}

	return 0;
}

bool CMeasStatistics::Process( CSettings::Measure::ESource src, CSettings::Measure::ERange range )
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, range ) )
		return false;

	if ( src == CSettings::Measure::_CH1 )
		Settings.CH1Calib.Prepare( &Settings.CH1, fastCalc1 );
	if ( src == CSettings::Measure::_CH2 )
		Settings.CH2Calib.Prepare( &Settings.CH2, fastCalc2 );
	if ( src == CSettings::Measure::_Math )
	{
		Settings.CH1Calib.Prepare( &Settings.CH1, fastCalc1 );
		Settings.CH2Calib.Prepare( &Settings.CH2, fastCalc2 );
		MainWnd.m_wndGraph.MathSetup( &fastCalc1, &fastCalc2 );
	}

	m_curSrc = src;
	m_curRange = range;
	m_nRawMin = 0;
	m_nRawMax = 0;
	m_fMin = 0;
	m_fMax = 0;
	m_fSum = 0;
	m_fSumR = 0;
	m_fSum2 = 0;
	m_nCount = 0;

	for ( int i = nBegin; i < nEnd; i++ )
	{
		BIOS::ADC::TSample::SampleType nSample = Sampler::GetAt( i );
		float fSample, fSample2;
		fSample = _GetSamplef( nSample );
		fSample2 = fSample * fSample;

		if ( i == nBegin )
		{
			m_fMin = m_fMax = fSample;
			m_nRawMin = m_nRawMax = nSample;
		}

		if ( fSample > m_fMax )
			m_fMax = fSample;
		if ( fSample < m_fMin )
			m_fMin = fSample;
		if ( (int)nSample > m_nRawMax )
			m_nRawMax = nSample;
		if ( (int)nSample < m_nRawMin )
			m_nRawMin = nSample;
			
		m_fSum += fSample;
		m_fSum2 += fSample2;
		m_fSumR += abs(fSample);
			
		m_nCount++;
	}

	return m_nCount > 0 ? true : false;
}

float CMeasStatistics::GetPeriod() 
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, m_curRange ) )
		return 0;
	if ( m_nRawMax - m_nRawMin < 16 )
		return 0;
		
	int nThresh = ( m_nRawMax - m_nRawMin ) / 4;
	int nTrigMin = m_nRawMin + nThresh;
	int nTrigMax = m_nRawMax - nThresh;
	int nTrigState = -1, nNewState = -1;
	int nLastRise = -1, nLastFall = -1;
	int nTotalRise = 0, nTotalFall = 0;
	int nSumRise = 0, nSumFall = 0;

	for ( int i = nBegin; i < nEnd; i++ )
	{
		int nSample_ = Sampler::GetAt(i);
		int nSample = _GetSample( nSample_ );

		if ( nSample > nTrigMax )
			nNewState = 1;
		if ( nSample < nTrigMin )
			nNewState = 0;

		if ( nTrigState != -1 && nNewState != nTrigState )
		{
			// trigger changing it's output
			if ( nNewState == 1 ) // rising edge
			{
				if ( nLastRise != -1 )
				{
					nSumRise += i - nLastRise; // period length
					nTotalRise++;
				}
				nLastRise = i;
			} else
			{	// falling edge
				if ( nLastFall != -1 )
				{
					nSumFall += i - nLastFall; // period length
					nTotalFall++;
				}
				nLastFall = i;
			}
		}
 		nTrigState = nNewState;
	}
			
	if ( nTotalRise + nTotalFall == 0 )
		return 0;

	float fAvgPeriod = (nSumRise + nSumFall) / (float)(nTotalRise + nTotalFall);
	// period in samples -> time in seconds
	float fTimeRes = Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
	float fValue = fTimeRes * fAvgPeriod;
	if ( fValue == 0 )
		return 0;
	return fValue;
}

float CMeasStatistics::GetChannelsDelta(bool rising) 
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, m_curRange ) )
		return 0;
	if ( m_nRawMax - m_nRawMin < 16 )
		return 0;
		
	Settings.CH1Calib.Prepare( &Settings.CH1, fastCalc1 );
	Settings.CH2Calib.Prepare( &Settings.CH2, fastCalc2 );
	MainWnd.m_wndGraph.MathSetup( &fastCalc1, &fastCalc2 );

	int nTrigStateChannel = -1, nTrigStateMath = -1;
	int nTotal = 0;
	int nSum = 0;

	CSettings::Measure::ESource src = m_curSrc;
	for ( int i = nBegin; i < nEnd; i++ )
	{
		int edge = _FindEdge(src, m_curRange, rising ? 1 : -1, i, nTrigStateChannel);

		if (edge > 0)
		{
			int edge2 = _FindEdge(CSettings::Measure::_Math, m_curRange, rising ? 1 : -1, edge, nTrigStateMath);
			if (edge2 > edge)
			{
				nSum += edge2 - edge;
				nTotal++;
			}
			i = edge;
		}
		else
			break;
	}
	m_curSrc = src;

	float fAvgDelta = nSum  / (float)nTotal;
	// period in samples -> time in seconds
	float fTimeRes = Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
	float fValue = fTimeRes * fAvgDelta;
	if ( fValue == 0 )
		return 0;
	return fValue;
}

float CMeasStatistics::GetPwm() 
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, m_curRange ) )
		return 0;
	if ( m_nRawMax - m_nRawMin < 6 )
		return 0;
		
	int nThresh = ( m_nRawMax + m_nRawMin ) / 2;
	int nLow = 0, nHigh = 0;

	for ( int i = nBegin; i < nEnd; i++ )
	{
		int nSample_ = Sampler::GetAt(i);
		int nSample = _GetSample( nSample_ );

		if ( nSample > nThresh )
			nHigh++;
		else
			nLow++;
	}
			
	if ( nLow + nHigh == 0 )
		return 0;

	return (float)nHigh/(nLow+nHigh);
}

float CMeasStatistics::GetFreq() 
{ 
	float f = GetPeriod(); 
	if ( f <= 0 )
		return 0;
	return 1.0f / f;
}

float CMeasStatistics::GetSigma() // variance
{
	// subject to rounding error, but faster than two-pass formula
	if ( m_nCount == 0 )
		return 0;
	float fAux = (m_fSum2 - m_fSum*m_fSum/2) / m_nCount;
	if ( abs(fAux) > 200 )
		return 0;
	return fAux;
}

float CMeasStatistics::GetBaud()
{                
	CSerialDecoder Iterate;
	//BIOS::DBG::Print( "Begin------\n");
	Iterate.m_pThis = this;
	if ( !Iterate.Prepare() )
		return 0.0f;
	Iterate.Average();
	Iterate.Do(&CSerialDecoder::FindSmallest);
	Iterate.Do(&CSerialDecoder::Sum);

	float fBaud = Iterate.GetBaud();

	bool bCan100k = (fBaud > 100000.0f-5000.0f) && (fBaud < 100000.0f+5000.0f);
	bool bCan500k = (fBaud > 500000.0f-5000.0f) && (fBaud < 500000.0f+5000.0f);

	/*if (1)
	{
		CCanbusDecoder Decoder;
		Decoder.Test();
	} else*/
	if ( bCan100k || bCan500k )
	{
		CCanbusDecoder Decoder;
		Iterate.SetDecoder( &Decoder );
		
		Decoder << 1;	// idle before start bit
		Iterate.Do(&CSerialDecoder::Decode);
		Decoder.Visualize();
	} else
	{
		Iterate.SetDecoder( &Iterate );
		Iterate << 1;	// idle before start bit
		Iterate.Do(&CSerialDecoder::Decode);
		for ( int i = 0; i < 8; i++ )
			Iterate << 0;	// finish byte when only single byte was sent
		Iterate.Visualize();
	}

	return fBaud;
}

float CMeasStatistics::GetMin() { return m_fMin; }
float CMeasStatistics::GetMax() { return m_fMax; }
float CMeasStatistics::GetAvg() { return m_fSum/m_nCount; }
float CMeasStatistics::GetRms() { return sqrt(m_fSum2/m_nCount); }
float CMeasStatistics::GetRectAvg() { return m_fSumR/m_nCount; }
float CMeasStatistics::GetVpp() { return m_fMax - m_fMin; }

float CMeasStatistics::GetTime(bool bHighLevel)
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, m_curRange ) )
		return 0;
	if ( m_nRawMax - m_nRawMin < 16 )
		return 0;
		
	int nTrigStateChannel = -1, nTrigStateMath = -1;
	int nTotal = 0;
	int nSum = 0;

	CSettings::Measure::ESource src = m_curSrc;
	for ( int i = nBegin; i < nEnd; i++ )
	{
		int edge = _FindEdge(src, m_curRange, bHighLevel ? 1 : -1, i, nTrigStateChannel);

		if (edge > 0)
		{
			int edge2 = _FindEdge(src, m_curRange, bHighLevel ? -1 : 1, edge, nTrigStateMath);
			if (edge2 > edge)
			{
				nSum += edge2 - edge;
				nTotal++;

				i = edge2;
			}
			else
				i = edge;
		}
		else
			break;
	}
	m_curSrc = src;

	float fAvgDelta = nSum  / (float)nTotal;
	// period in samples -> time in seconds
	float fTimeRes = Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
	float fValue = fTimeRes * fAvgDelta;
	if ( fValue == 0 )
		return 0;
	return fValue;
}

float CMeasStatistics::GetEdgeTime(bool bRising)
{
	int nBegin = 0, nEnd = 0;
	if ( !_GetRange( nBegin, nEnd, m_curRange ) )
		return 0;
	if ( m_nRawMax - m_nRawMin < 16 )
		return 0;
		
	int nTrig10 = (int)(0.1 * (m_nRawMax - m_nRawMin)) + m_nRawMin;
	int nTrig90 = (int)(0.9 * (m_nRawMax - m_nRawMin)) + m_nRawMin;
	int nEdge10 = -1, nEdge90 = -1;

	int nTotal = 0;
	int nSum = 0;

	for ( int i = nBegin; i < nEnd; i++ )
	{
		int nSample_ = Sampler::GetAt(i);
		int nSample = _GetSample( nSample_ );

		if ( (nTrig90 - 10 < nSample) && ( nSample < nTrig90 + 10) )
		{
			nEdge90 = i;
			if ( (nEdge10 != -1) && (nEdge90 != -1) && bRising )
			{
				nSum += nEdge90 - nEdge10;
				nEdge10 = -1;
				nTotal++;
			}
		}
		if ( (nTrig10 - 10 < nSample) && ( nSample < nTrig10 + 10) )
		{
			nEdge10 = i;
			if ( (nEdge10 != -1) && (nEdge90 != -1) && (!bRising) )
			{
				nSum += nEdge10 - nEdge90;
				nEdge90 = -1;
				nTotal++;
			}
		}
	}

	float fAvgDelta = nSum  / (float)nTotal;
	// period in samples -> time in seconds
	float fTimeRes = Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
	float fValue = fTimeRes * fAvgDelta;
	if ( fValue == 0 )
		return 0;
	return fValue;
}

float CMeasStatistics::GetFormFactor() { return GetRms() / GetRectAvg(); }
float CMeasStatistics::GetDispersion() { float f = GetSigma(); return f*f; }

float CMeasStatistics::GetActivePower()
{
	float effectiveVoltage = 0, effectiveCurrent = 0;

	if (_GetEffectiveValuesForPower(effectiveVoltage, effectiveCurrent))
	{
		float fPeriod = GetPeriod();
		if (fPeriod > 0)
		{
			float cosfi = (float)cos(2 * 3.141592 * GetChannelsDelta(true) / fPeriod);

			return (effectiveVoltage * effectiveCurrent * cosfi);
		}
	}
	return 0;
}

float CMeasStatistics::GetReactivePower()
{
	float effectiveVoltage = 0, effectiveCurrent = 0;

	if (_GetEffectiveValuesForPower(effectiveVoltage, effectiveCurrent))
	{
		float fPeriod = GetPeriod();
		if (fPeriod > 0)
		{
			float sinfi = (float)sin(2 * 3.141592 * GetChannelsDelta(true) / fPeriod);
			return (effectiveVoltage * effectiveCurrent * sinfi);
		}
	}
	return 0;
}

float CMeasStatistics::GetApparentPower()
{
	float effectiveVoltage = 0, effectiveCurrent = 0;

	if (_GetEffectiveValuesForPower(effectiveVoltage, effectiveCurrent))
	{
		return (effectiveVoltage * effectiveCurrent);
	}
	return 0;
}

bool CMeasStatistics::_GetRange( int& nBegin, int& nEnd, CSettings::Measure::ERange range )
{
	switch ( range )
	{
		case CSettings::Measure::_View:
			MainWnd.m_wndGraph.GetCurrentRange( nBegin, nEnd );
			break;
		case CSettings::Measure::_Selection:
			if ( Settings.MarkT1.Mode != CSettings::Marker::_On || 
					Settings.MarkT2.Mode != CSettings::Marker::_On ||
					Settings.MarkT2.nValue <= Settings.MarkT1.nValue )
			{
				return false;
			}
			nBegin = Settings.MarkT1.nValue;
			nEnd = Settings.MarkT2.nValue;
			break;
		case CSettings::Measure::_All:
			nBegin = 0;
			nEnd = BIOS::ADC::NumSamples;
			break;
	}
	return true;
}
