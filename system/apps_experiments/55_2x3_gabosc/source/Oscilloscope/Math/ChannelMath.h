#pragma once
#include <Settings.h>
#include "FirFilter.h"

class CMathChannel
{
	CSettings::Calibrator::FastCalc* m_pFC1;
	CSettings::Calibrator::FastCalc* m_pFC2;
	int m_nTemp;
	CFirFilter	m_Fir;

public:
	void			MathSetup( CSettings::Calibrator::FastCalc* pFC1, CSettings::Calibrator::FastCalc* pFC2 );
	int				MathCalc(ui32 nSample);

private:
	int				MathGet(CSettings::MathOperand& op, int nSample);
	int				MathFx( int nParam, int nScale );
};
