#include "MenuMeas.h"

#include <MainWnd.h>
#include <math.h>
#include "Statistics.h"

/*virtual*/ void CWndMenuMeas::Create(CWnd *pParent, int dwFlags) 
{
	CWnd::Create("CWndMenuMeas", dwFlags | CWnd::WsListener, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);
	m_itmMeas[0].Create( &Settings.Meas[0], this );
	m_itmMeas[1].Create( &Settings.Meas[1], this );
	m_itmMeas[2].Create( &Settings.Meas[2], this );
	m_itmMeas[3].Create( &Settings.Meas[3], this );
	m_itmMeas[4].Create( &Settings.Meas[4], this );
	m_itmMeas[5].Create( &Settings.Meas[5], this );
}

/*virtual*/ void CWndMenuMeas::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndGraph.ShowWindow(false);
		MainWnd.m_wndInfoBar.ShowWindow(false);
		MainWnd.m_wndLReferences.ShowWindow(false);
		MainWnd.m_wndTReferences.ShowWindow(false);
		return;
	}

	if (code == ToWord('L', 'E') )
	{
		MainWnd.m_wndGraph.ShowWindow(true);
		MainWnd.m_wndInfoBar.ShowWindow(true);
		MainWnd.m_wndLReferences.ShowWindow(true);
		MainWnd.m_wndTReferences.ShowWindow(true);
		return;
	}

	// new waveform acquired, update the Y values 	
	if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
	{
		_UpdateAll();
		return;
	}

	// something was changed in listbox
	if (code == ToWord('u', 'p') )
	{
		_UpdateAll();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[0].m_pMeas )
			m_itmMeas[0].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[1].m_pMeas )
			m_itmMeas[1].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[2].m_pMeas )
			m_itmMeas[2].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[3].m_pMeas )
			m_itmMeas[3].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[4].m_pMeas )
			m_itmMeas[4].Invalidate();
		if ( m_wndListMeas.m_pMeas == m_itmMeas[5].m_pMeas )
			m_itmMeas[5].Invalidate();
		return;
	}

	// Show listbox - cursor details
	if (code == ToWord('m', 'c') )	
	{
		m_wndListMeas.Create( (CSettings::Measure*)(NATIVEPTR)data, this );
		m_wndListMeas.StartModal( &m_wndListMeas.m_itmEnabled );
		return;
	}
	if ( code == ToWord('o', 'k') && pSender == &m_wndListMeas )
	{
		// done
		m_wndListMeas.StopModal();
		return;
	}
	if ( code == ToWord('e', 'x') && pSender == &m_wndListMeas )
	{
		// done
		m_wndListMeas.StopModal();
		return;
	}
}

void CWndMenuMeas::_UpdateAll()
{
	CMeasStatistics m_Stat;

	for ( int nFilter = CSettings::Measure::_CH1; nFilter <= CSettings::Measure::_Math; nFilter++ )
	{
		int nLastRange = -1;

		for ( int i = 0; i < (int)COUNT( m_itmMeas ); i++ )
		{
			CSettings::Measure& meas = Settings.Meas[i];
			if ( meas.Enabled == CSettings::Measure::_Off )
				continue;
			if ( meas.Source != nFilter )
				continue;

			if ( nLastRange != meas.Range )
			{
				if ( !m_Stat.Process( (CSettings::Measure::ESource) nFilter, meas.Range ) )
				{
					continue;
				}
				nLastRange = meas.Range;
			}
			
			float fPrev = meas.fValue;
			meas.fValue = -1;
			switch ( meas.Type )
			{
				case CSettings::Measure::_Min:		meas.fValue = m_Stat.GetMin(); break;
				case CSettings::Measure::_Max:		meas.fValue = m_Stat.GetMax(); break;
				case CSettings::Measure::_Avg:		meas.fValue = m_Stat.GetAvg(); break;
				case CSettings::Measure::_RectAvg:	meas.fValue = m_Stat.GetRectAvg(); break;
				case CSettings::Measure::_Rms:		meas.fValue = m_Stat.GetRms(); break;
				case CSettings::Measure::_Vpp:		meas.fValue = m_Stat.GetVpp(); break;
				case CSettings::Measure::_Freq:		meas.fValue = m_Stat.GetFreq() / 1000.0f; break; // khz
				case CSettings::Measure::_Period:	meas.fValue = m_Stat.GetPeriod() * 1000.0f; break; // ms
				case CSettings::Measure::_DeltaTime: 
					meas.fValue = m_Stat.GetChannelsDelta(true) * 1000; break;
				case CSettings::Measure::_Angle:    
					{
						float fPeriod = m_Stat.GetPeriod();
						meas.fValue = 0;
						if (fPeriod != 0)
							meas.fValue = 360 * m_Stat.GetChannelsDelta(true) / fPeriod; 
					} break;
				case CSettings::Measure::_TimeH:    meas.fValue = m_Stat.GetTime(true) * 1000; break;
				case CSettings::Measure::_TimeL:    meas.fValue = m_Stat.GetTime(false) * 1000; break;
				case CSettings::Measure::_TimeRise: meas.fValue = m_Stat.GetEdgeTime(true) * 1000; break;
				case CSettings::Measure::_TimeFall: meas.fValue = m_Stat.GetEdgeTime(false) * 1000; break;
				case CSettings::Measure::_FormFactor:
					meas.fValue = m_Stat.GetFormFactor(); break;
				case CSettings::Measure::_Sigma:	meas.fValue = m_Stat.GetSigma(); break;
				case CSettings::Measure::_Dispersion:
					meas.fValue = m_Stat.GetDispersion(); break;
				case CSettings::Measure::_Baud:		meas.fValue = m_Stat.GetBaud(); break;
				case CSettings::Measure::_Pwm:		meas.fValue = m_Stat.GetPwm(); break;
				case CSettings::Measure::_P:        meas.fValue = m_Stat.GetActivePower(); break;
				case CSettings::Measure::_Pk:       meas.fValue = m_Stat.GetActivePower() / 1000; break;
				case CSettings::Measure::_Q:        meas.fValue = m_Stat.GetReactivePower(); break;
				case CSettings::Measure::_Qk:       meas.fValue = m_Stat.GetReactivePower() / 1000; break;
				case CSettings::Measure::_S:        meas.fValue = m_Stat.GetApparentPower(); break;
				case CSettings::Measure::_Sk:       meas.fValue = m_Stat.GetApparentPower() / 1000; break;
				default:
					_ASSERT( !!!"Unknown measurement type" );
			}
			if ( fPrev != meas.fValue )
				m_itmMeas[i].Invalidate();
		}
	}
}

