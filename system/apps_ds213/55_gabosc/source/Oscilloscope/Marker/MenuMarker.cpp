#include "MenuMarker.h"

#include <MainWnd.h>

CWndMenuCursor::CWndMenuCursor()
{
}

/*virtual*/ void CWndMenuCursor::Create(CWnd *pParent, int dwFlags) 
{
	CWnd::Create("CWndMenuCursor", dwFlags | CWnd::WsListener, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);
	m_itmX1.Create( (char*)"X1", &Settings.MarkT1, this );
	m_itmY1.Create( (char*)"Y1", &Settings.MarkY1, this );
	m_itmX2.Create( (char*)"X2", &Settings.MarkT2, this );
	m_itmY2.Create( (char*)"Y2", &Settings.MarkY2, this );
	m_itmDeltaY.Create( (char*)"dY", &Settings.MarkY1, &Settings.MarkY2, this);
	m_itmDeltaX.Create( (char*)"dX", &Settings.MarkT1, &Settings.MarkT2, this);
}

/*virtual*/ void CWndMenuCursor::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndGraph.ShowWindow(false);
		MainWnd.m_wndLReferences.ShowWindow(false);
		MainWnd.m_wndTReferences.ShowWindow(false);
		MainWnd.m_wndInfoBar.ShowWindow(false);
		return;
	}

	if (code == ToWord('L', 'E') )
	{
		MainWnd.m_wndGraph.ShowWindow(true);
		MainWnd.m_wndLReferences.ShowWindow(true);
		MainWnd.m_wndTReferences.ShowWindow(true);
		MainWnd.m_wndInfoBar.ShowWindow(true);
		return;
	}

	// update, called when the user changes the cursor position
	if (code == ToWord('u', 'p') )
	{
		if ( data == (NATIVEPTR)&m_itmX1 ||
			 data == (NATIVEPTR)&m_itmX2 )
		{
			m_itmDeltaX.Invalidate();
		}
		if ( data == (NATIVEPTR)&m_itmY1 ||
			 data == (NATIVEPTR)&m_itmY2 )
		{
			m_itmDeltaY.Invalidate();
		}
		if ( !BIOS::ADC::Enabled() )
		{
			// we should redraw the graph, but not immediatelly for keeping
			// keyboard response quick
			MainWnd.m_wndGraph.Invalidate();
		}
		return;
	}

	// new waveform acquired, update the Y values 	
	if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
	{
		if ( Settings.MarkY1.Mode == CSettings::Marker::_Auto )
			m_itmY1.Invalidate();
		if ( Settings.MarkY2.Mode == CSettings::Marker::_Auto )
			m_itmY2.Invalidate();
		if ( Settings.MarkY1.Mode == CSettings::Marker::_Auto ||
			 Settings.MarkY1.Mode == CSettings::Marker::_Auto )
		{
			m_itmDeltaY.Invalidate();
		}
		return;
	}

	// Show listbox - cursor details
	if (code == ToWord('m', 'c') )	
	{
		m_wndListCursor.Create( (CSettings::Marker*)(NATIVEPTR)data, this );
		m_wndListCursor.StartModal( &m_wndListCursor.m_itmMode );
		return;
	}
	if ( code == ToWord('o', 'k') && pSender == &m_wndListCursor )
	{
		// done
		m_wndListCursor.StopModal();
		return;
	}
	if ( code == ToWord('e', 'x') && pSender == &m_wndListCursor )
	{
		// done
		m_wndListCursor.StopModal();
		return;
	}
	
	// Clicked on Find
	if ( code == ToWord('l', 'e') && data == (NATIVEPTR)&m_wndListCursor.m_proFind )
	{
		Find( m_wndListCursor.m_pMarker, m_wndListCursor.m_modeFind );
		return;
	}
}

void CWndMenuCursor::Find(CSettings::Marker* pMarker, CSettings::Marker::EFind mode)
{
	int nSampleBegin, nSampleEnd;
	int nMin = 0, nMax = 0, nMinI = 0, nMaxI = 0;
	MainWnd.m_wndGraph.GetCurrentRange( nSampleBegin, nSampleEnd );

	for ( int i = nSampleBegin; i < nSampleEnd; i++ )
	{
		int nAdc = Sampler::GetAt(i);
		if ( pMarker->Source == CSettings::Marker::_CH1 )
			nAdc = (ui8)((nAdc) & 0xff);
		else
			nAdc = (ui8)((nAdc>>8) & 0xff);

		if ( i == nSampleBegin )
		{
			nMax = nAdc;
			nMin = nAdc;
			nMaxI = i;
			nMinI = i;
		}		
		if ( nAdc > nMax )
		{
			nMax = nAdc;
			nMaxI = i;
		}
		if ( nAdc < nMin )
		{
			nMin = nAdc;
			nMinI = i;
		}
	}

	/// what if there are multiple extrema points?
	CSettings::Marker* pMarkerTime = NULL; 
	if ( pMarker == &Settings.MarkY1 )
		pMarkerTime = &Settings.MarkT1;
	if ( pMarker == &Settings.MarkY2 )
		pMarkerTime = &Settings.MarkT2;
	
	if ( mode == CSettings::Marker::_MaxFind )
	{
		pMarker->nValue = nMax;
		if ( pMarkerTime )
			pMarkerTime->nValue = nMaxI;
	}
	if ( mode == CSettings::Marker::_MinFind )
	{
		pMarker->nValue = nMin;
		if ( pMarkerTime )
			pMarkerTime->nValue = nMinI;
	}
	if ( mode == CSettings::Marker::_AvgFind )
	{
		if ( pMarker->Mode == CSettings::Marker::_Auto )
			 pMarker->Mode = CSettings::Marker::_On;
		pMarker->nValue = (nMin + nMax)/2;
		
	}
}

