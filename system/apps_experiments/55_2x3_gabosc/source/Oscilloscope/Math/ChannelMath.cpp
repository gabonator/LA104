#include "ChannelMath.h"
#include <Oscilloscope/Controls/GraphOsc.h>
#include <math.h>
#include <Framework.h>
#include <MainWnd.h>


void CMathChannel::MathSetup(CSettings::Calibrator::FastCalc* pFC1, CSettings::Calibrator::FastCalc* pFC2)
{
	if ( Settings.Math.Type == CSettings::MathOperator::_FirAplusC || 
		 Settings.Math.Type == CSettings::MathOperator::_FirAdivBplusC )
		m_Fir.Reset();

	if ( Settings.Math.Type == CSettings::MathOperator::_Off )
	{
		// type set to CSettings::MathOperator::_Off would cause assert in MathCalc
		Settings.Math.Type = CSettings::MathOperator::_AplusBplusC;
		MainWnd.m_wndMessage.Show( &MainWnd, "Information", "Math operator set to A+B+C", RGB565(ffff00) );	
	}

	m_pFC1 = pFC1;
	m_pFC2 = pFC2;
	m_nTemp = 0;
}

int CMathChannel::MathCalc(ui32 nSample)
{
	int res = 0;
	switch ( Settings.Math.Type )
	{
	case CSettings::MathOperator::_A:
		res = MathGet(Settings.MathA, nSample);
		break;
	case CSettings::MathOperator::_B:
		res = MathGet(Settings.MathB, nSample);
		break;
	case CSettings::MathOperator::_C:
		res = MathGet(Settings.MathC, nSample);
		break;
	case CSettings::MathOperator::_AplusBplusC:
		res =  
			MathGet(Settings.MathA, nSample) +
			MathGet(Settings.MathB, nSample) +
			MathGet(Settings.MathC, nSample);
		break;
	case CSettings::MathOperator::_AminusBplusC:
		res = 
			MathGet(Settings.MathA, nSample) -
			MathGet(Settings.MathB, nSample) +
			MathGet(Settings.MathC, nSample);
		break;
	case CSettings::MathOperator::_BminusAplusC:
		res = 
			MathGet(Settings.MathB, nSample) -
			MathGet(Settings.MathA, nSample) +
			MathGet(Settings.MathC, nSample);
		break;
	case CSettings::MathOperator::_AgreaterBplusC:
		res = (MathGet(Settings.MathA, nSample) > 
			MathGet(Settings.MathB, nSample) ? 32 : 0) +
			MathGet(Settings.MathC, nSample);
		break;
	case CSettings::MathOperator::_AlessBplusC:
		res = (MathGet(Settings.MathA, nSample) <
			MathGet(Settings.MathB, nSample) ? 32 : 0) +
			MathGet(Settings.MathC, nSample);
		break;
	case CSettings::MathOperator::_minAB:
		{
			int fA = MathGet(Settings.MathA, nSample);
			int fB = MathGet(Settings.MathB, nSample);
			res = min(fA, fB);
		} break;
	case CSettings::MathOperator::_maxAB:
		{
			int fA = MathGet(Settings.MathA, nSample);
			int fB = MathGet(Settings.MathB, nSample);
			res = max(fA, fB);
		} break;
	case CSettings::MathOperator::_FirAplusC:
		{
			int nA = MathGet(Settings.MathA, nSample);
			int nC = MathGet(Settings.MathC, nSample);
			int nFiltered = 0;

			m_Fir << nA;
			m_Fir >> nFiltered;
			
			res = nFiltered + nC;
		} break;
	case CSettings::MathOperator::_FirAdivBplusC:
		{
			int nA = MathGet(Settings.MathA, nSample);
			int nB = MathGet(Settings.MathB, nSample);
			int nC = MathGet(Settings.MathC, nSample);
			int nFiltered = 0;

			m_Fir << nA;
			m_Fir >> nFiltered;
			
			if ( nB == 0 )
				nB = 1;

			res = (nFiltered / nB) + nC;
		} break;
	case CSettings::MathOperator::_AmulBmulC:
		res = 
			MathGet(Settings.MathA, nSample) * 
			MathGet(Settings.MathB, nSample) * 
			MathGet(Settings.MathC, nSample);
		break;
	default:
		_ASSERT(0);
		return 0;
	}
	return res + Settings.Math.Position;
}

int CMathChannel::MathGet(CSettings::MathOperand& op, int nSample)
{
	int nAux;
	switch ( op.Type )
	{
	case CSettings::MathOperand::_CH1Corrected:
		nAux = ( nSample & 0xff ) - Settings.CH1Calib.GetZero(*m_pFC1);
		nAux = Settings.CH1Calib.Correct( *m_pFC1, nAux );
		if ( op.nScale != 100 )
			nAux = nAux * op.nScale / 100;
		nAux = (int)(nAux * m_pFC1->fMultiplier / Settings.CH1Calib.GetMultiplier(Settings.Math.Resolution));

		return nAux;	
	case CSettings::MathOperand::_Ch1Raw:
		nAux = ( nSample & 0xff ) - Settings.CH1Calib.GetZero(*m_pFC1);
		if ( op.nScale != 100 )
			nAux = nAux * op.nScale / 100;
		nAux = (int)(nAux * m_pFC1->fMultiplier / Settings.CH1Calib.GetMultiplier(Settings.Math.Resolution));

		return nAux;	
	case CSettings::MathOperand::_Ch2Corrected:
		nAux = ( (nSample >> 8) & 0xff ) - Settings.CH2Calib.GetZero(*m_pFC2);
		nAux = Settings.CH2Calib.Correct( *m_pFC2, nAux );
		if ( op.nScale != 100 )
			nAux = nAux * op.nScale / 100;
		nAux = (int)(nAux * m_pFC2->fMultiplier / Settings.CH2Calib.GetMultiplier(Settings.Math.Resolution));

		return nAux;	
	case CSettings::MathOperand::_Ch2Raw:
		nAux = ( (nSample >> 8) & 0xff ) - Settings.CH2Calib.GetZero(*m_pFC2);
		if ( op.nScale != 100 )
			nAux = nAux * op.nScale / 100;
		nAux = (int)(nAux * m_pFC2->fMultiplier / Settings.CH2Calib.GetMultiplier(Settings.Math.Resolution));

		return nAux;
	case CSettings::MathOperand::_Constant:
		return op.nConstant;
	case CSettings::MathOperand::_Fx:
		return MathFx(op.nConstant, op.nScale);
	}
	return 0;
}

int	CMathChannel::MathFx( int nParam, int nScale )
{
	int nTotal = CWndGraph::BlkX*CWndGraph::DivsX;
	int nValue = (int)(128+127*nScale/100.0f*sin( 2*3.141592653f * nParam * (m_nTemp / (float)nTotal)));
#ifdef _WIN32
	_ASSERT( m_nTemp < nTotal );
#endif
	m_nTemp++;
	return nValue;
}
