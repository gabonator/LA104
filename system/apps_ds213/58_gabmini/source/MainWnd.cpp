#include "MainWnd.h"

CMainWnd* CMainWnd::m_pInstance = nullptr;
CSettings m_Settings; // TODO:!

long m_lLastAcquired = 0;
long m_lLastRequested = 0;

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

        // just for testing
        BIOS::DAC::SetMode(BIOS::DAC::EMode::Square, nullptr, 0);
        BIOS::DAC::SetFrequency(1000);

#if defined(DS203)
	// Start in scan mode
	CCoreOscilloscope::ConfigureTrigger();
	CCoreOscilloscope::ConfigureAdc();
	BIOS::ADC::Restart(0);
#elif defined(DS213)
        BIOS::ADC::Enable( true );
	CCoreOscilloscope::ConfigureAdc();
	CCoreOscilloscope::ConfigureTrigger();
#else
#error Unknown target
#endif
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
		BIOS::ADC::EState state = BIOS::ADC::GetState();

		bool ready = state != BIOS::ADC::EState::Offline && state != BIOS::ADC::EState::Busy;
#ifdef DS203
		bool triggered = state == BIOS::ADC::EState::Full;
#else
		bool triggered = state == BIOS::ADC::EState::Triggered;
#endif

		if ( (Settings.Trig.Sync != CSettings::Trigger::_None) && triggered )
		{
			m_lLastAcquired = BIOS::SYS::GetTick();
			Sampler::Copy();
			BIOS::ADC::Restart(0);
         		m_lLastRequested = BIOS::SYS::GetTick();

			CWnd::WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
		}

	if ( ready && Settings.Trig.Sync == CSettings::Trigger::_Auto )
	{
                // we dont need to wait for the screen to be fully reloaded - on slow sampling modes
		int screenDuration = Settings.Runtime.m_nScreenDuration*3/2+20;
		if ( BIOS::SYS::GetTick() - m_lLastAcquired > 500 && BIOS::SYS::GetTick() - m_lLastRequested > screenDuration)
		{
// If we enter this point, we will lost buffer-full synchronization - because we immediatelly 
// request new transfer after the wptr reaches end of screen. So the "FULL" flag will be never
// set
// only if we will wait for full buffer, but that would reduce redraw time
#ifdef DS203
// + invalid first + scroll offset
			bool bScreenReady = BIOS::ADC::GetPointer() > (300 + Settings.Time.InvalidFirst);
//			bool canProcess = true;
#else
			bool bScreenReady = true;
//			int nScreenDuration = Settings.Runtime.m_nScreenDuration*2;
//			bool bScreenReady = BIOS::SYS::GetTick() - m_lLastRequested > nScreenDuration;
//                        bool canProcess = bScreenReady;
#endif
//			if (bScreenReady)
			{
         			Sampler::Copy();

         			// redraw the screen even when the sampler is not full
         			WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
         		
         			// force restart if the write pointer is behind current window
         			// restart only when the write ptr is behind screen - slow signals
         			if ( bScreenReady )
         			{
         				BIOS::ADC::Restart(0);
         				m_lLastRequested = BIOS::SYS::GetTick();
         			} 
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
