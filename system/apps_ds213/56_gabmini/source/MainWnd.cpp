#include "MainWnd.h"

CMainWnd* CMainWnd::m_pInstance = nullptr;
CSettings m_Settings; // TODO:!

long m_lLastAcquired = 0;

void CMainWnd::Create()
{	
	m_pInstance = this;
	CWnd::Create("CMainWnd", WsVisible | WsListener, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), NULL );
	mWndToolBar.Create(this);
	m_wndGraph.Create( this, WsHidden | WsNoActivate );

	m_wndMenuInput.Create( this, WsVisible );
	m_wndMenuCursor.Create( this, WsHidden );
	m_wndMenuDisplay.Create( this, WsHidden );

	m_wndLReferences.Create( this, WsHidden );
	m_wndTReferences.Create( this, WsHidden );
	
	m_wndZoomBar.Create(this, WsHidden, &m_wndGraph);

	constexpr int nMenuItem = 1;
	SendMessage( &mWndToolBar, ToWord('g', 'i'), nMenuItem);
	m_wndMenuInput.SetFocus();

	// Start in scan mode
	BIOS::ADC::Enable(true);
	BIOS::ADC::Restart();
	Settings.Trig.Sync = CSettings::Trigger::_Scan;
	CCoreOscilloscope::ConfigureTrigger();
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
		if ( code == ToWord('L', 'E') && !data )
		{
			// Entered root menu, exit app
			mRunning = false;
		}
		if ( code == ToWord('L', 'R') )	// Layout reset
		{
			Invalidate();
		}
		return;
	}
}

/*virtual*/ void CMainWnd::OnPaint()
{
	//BIOS::LCD::Clear(RGB565(000000));
	GUI::Background(CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), RGB565(101010), RGB565(404040));
}

bool CMainWnd::IsRunning()
{
	return mRunning;
}

/*virtual*/ void CMainWnd::WindowMessage(int nMsg, int nParam)
{
	if ( nMsg == WmTick )
	{
		if ( (Settings.Trig.Sync != CSettings::Trigger::_None) && BIOS::ADC::Enabled() && BIOS::ADC::Ready() )
		{
			Sampler::Copy();
			BIOS::ADC::Restart();
			CWnd::WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
		}

	if ( BIOS::ADC::Enabled() && Settings.Trig.Sync == CSettings::Trigger::_Auto )
	{
		if ( m_lLastAcquired != -1 && BIOS::SYS::GetTick() - m_lLastAcquired > 150 )
		{
			bool bScreenReady = BIOS::ADC::GetPointer() > (300 + Settings.Time.InvalidFirst);
			Sampler::Copy();

			// redraw the screen even when the sampler is not full
			WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
		
			// force restart if the write pointer is behind current window
			if ( bScreenReady )
			{
				BIOS::ADC::Restart();
			} 
		}
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
