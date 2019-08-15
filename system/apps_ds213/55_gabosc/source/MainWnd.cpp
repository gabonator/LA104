#include "MainWnd.h"

CMainWnd* CMainWnd::m_pInstance = nullptr;
CSettings m_Settings; // TODO:!

void CMainWnd::Create()
{
	
	m_pInstance = this;
	CWnd::Create("CMainWnd", WsVisible | WsListener, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), NULL );
	mWndToolBar.Create(this);
	m_wndGraph.Create( this, WsHidden | WsNoActivate );

	m_wndMenuInput.Create( this, WsVisible );
	m_wndMenuCursor.Create( this, WsHidden );
	m_wndMenuMeas.Create( this, WsHidden );
	m_wndMenuMath.Create( this, WsHidden );
	m_wndMenuDisplay.Create( this, WsHidden );
	m_wndMenuMask.Create( this, WsHidden );


	constexpr int nMenuItem = 1;
	SendMessage( &mWndToolBar, ToWord('g', 'i'), nMenuItem);
	m_wndMenuInput.SetFocus();
}

/*virtual*/ void CMainWnd::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	if ( pSender == &mWndToolBar )
	{
		if ( code == ToWord('L', 'D') && data )	// Layout disable
		{
			CWnd* pLayout = (CWnd*)data;
			SendMessage( pLayout, code, 0 );
			pLayout->ShowWindow( false );
		}
		if ( code == ToWord('L', 'E') && data )	// Layout enable
		{
			CWnd* pLayout = (CWnd*)data;
			SendMessage( pLayout, code, 0 );
			pLayout->ShowWindow( true );
		}
		if ( code == ToWord('L', 'R') )	// Layout reset
		{
			Invalidate();
		}
		return;
	}
}

/*virtual*/ void CMainWnd::WindowMessage(int nMsg, int nParam)
{
	if ( nMsg == WmTick )
	{
		static long l = 0;
		long n = BIOS::SYS::GetTick();
		if (n-l > 100)
		{
			CWnd::WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
			l = n;
		}
/*
		if ( (Settings.Trig.Sync != CSettings::Trigger::_None) && BIOS::ADC::Enabled() && BIOS::ADC::Ready() )
		{
			// ADC::Ready means that the write pointer is at the end of buffer, we can restart sampler
			BIOS::ADC::Copy( BIOS::ADC::GetCount() );
			BIOS::ADC::Restart();
			Resample();
		
			// trig stuff
			m_lLastAcquired = BIOS::SYS::GetTick();
			if ( BIOS::ADC::Enabled() && Settings.Trig.Sync == CSettings::Trigger::_Single )
			{
				BIOS::ADC::Enable( false );
				Settings.Trig.State = CSettings::Trigger::_Stop;
				if ( m_wndMenuInput.m_itmTrig.IsVisible() )
					m_wndMenuInput.m_itmTrig.Invalidate();
					}
		
			// broadcast message for windows that process waveform data
			WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
		}*/
	}
	CWnd::WindowMessage(nMsg, nParam);
}
