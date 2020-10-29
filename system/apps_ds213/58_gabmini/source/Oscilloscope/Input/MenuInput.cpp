
#include "MenuInput.h"

#include <MainWnd.h>
#include <Oscilloscope/Core/CoreOscilloscope.h>

CWndMenuInput::CWndMenuInput()
{
}

/*virtual*/ void CWndMenuInput::Create(CWnd *pParent, int dwFlags) 
{
	CWnd::Create("CWndMenuInput", dwFlags, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);
	m_itmCH1.Create(&Settings.CH1, this);
	m_itmCH2.Create(&Settings.CH2, this);
	m_itmCH3.Create(&Settings.CH3, this);
	m_itmCH4.Create(&Settings.CH4, this);
	m_itmTime.Create(&Settings.Time, this);
	m_itmWindow.Create(this);
	m_itmTrig.Create(this);
}

/*virtual*/ void CWndMenuInput::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndGraph.ShowWindow(false);
		MainWnd.m_wndZoomBar.ShowWindow(false);
		MainWnd.m_wndLReferences.ShowWindow(false);
		MainWnd.m_wndTReferences.ShowWindow(false);
	}

	if (code == ToWord('L', 'E') )
	{
		MainWnd.m_wndGraph.ShowWindow(true);
		MainWnd.m_wndZoomBar.ShowWindow(true);
		MainWnd.m_wndLReferences.ShowWindow(true);
		MainWnd.m_wndTReferences.ShowWindow(true);
	}


	// Trigger
	if (code == ToWord('m', 'r') )
	{
		m_wndListTrigger.Create( this );
		m_wndListTrigger.StartModal(&m_wndListTrigger.m_itmLevel);
	}
	if ( code == ToWord('o', 'k') && pSender->m_pParent == &m_wndListTrigger )
	{
		// done
		m_wndListTrigger.StopModal();
	}
	if ( code == ToWord('e', 'x') && pSender == &m_wndListTrigger )
	{
		// cancel
		m_wndListTrigger.StopModal();
	}
	if ( code == ToWord('u', 'p') && pSender->m_pParent == &m_wndListTrigger )
	{
		if ( pSender == &m_wndListTrigger.m_itmLevel )
			MainWnd.m_wndLReferences.Invalidate();
//		if ( pSender == &m_wndListTrigger.m_itmTime )
//			MainWnd.m_wndTReferences.Invalidate();
		// first 150 samples in buffer are for storing pre-trigger
		// samples. When no trigger is set, these samples are invalid.
		int nInvalid = Settings.Trig.Sync == CSettings::Trigger::_None ? 150 : 30;
		Settings.Time.InvalidFirst = nInvalid;
		if ( Settings.Time.Shift < nInvalid )
			Settings.Time.Shift = nInvalid;
		MainWnd.m_wndZoomBar.Invalidate();

		if ( pSender == &m_wndListTrigger.m_itmSync )
		{
#ifdef DS203
			BIOS::ADC::Enable( true );
			BIOS::ADC::Restart(0);
#endif
		}

		Settings.Trig.State = (Settings.Trig.Sync == CSettings::Trigger::_Single) ? 
			CSettings::Trigger::_Wait : CSettings::Trigger::_Run;;

		CCoreOscilloscope::ConfigureTrigger();
		Settings.Trig.nLastChange = BIOS::SYS::GetTick();
		// update
		CWnd::GetTopModal().m_pPrevFocus->Invalidate();
	}

	// window
	if ( code == ToWord('w', 'u') )
	{
		MainWnd.m_wndTReferences.Invalidate();
		MainWnd.m_wndZoomBar.Invalidate();
		// if the osccilloscope window is freezed, inform to redraw it
		if ( !BIOS::ADC::Enabled() )
		{
			MainWnd.WindowMessage( CWnd::WmBroadcast, ToWord('d', 'g') );
		}
	}

	// Timebase
	if (code == ToWord('m', 't') )	// more timebase
	{
		m_wndComboSelector.Create( GetFocus()->m_pszId, WsVisible | WsModal, 
			CRect( 30, 100, 370, 140), RGB565(ffffff), (CValueProvider*)(NATIVEPTR)data, this);

		m_wndComboSelector.StartModal();
	}

	// Digital
	if (code == ToWord('m', 'd') )	// more input digital
	{
		m_wndListDInput.Create( (CSettings::DigitalChannel*)(NATIVEPTR)data, this );
		m_wndListDInput.StartModal( &m_wndListDInput.m_itmPosition );
	}
	if ( code == ToWord('o', 'k') && pSender->m_pParent == &m_wndListDInput )
	{
		// done
		m_wndListDInput.StopModal();
	}
	if ( code == ToWord('e', 'x') && pSender == &m_wndListDInput )
	{
		// cancel
		m_wndListDInput.StopModal();
	}

	// Analog
	if (code == ToWord('m', 'a') )	// more input analog
	{
		m_wndListAInput.Create( (CSettings::AnalogChannel*)(NATIVEPTR)data, this );
		m_wndListAInput.StartModal( &m_wndListAInput.m_itmResolution );
	}
	if ( code == ToWord('o', 'k') && pSender->m_pParent == &m_wndListAInput )
	{
		// done
		m_wndListAInput.StopModal();
	}
	if ( code == ToWord('e', 'x') && pSender == &m_wndListAInput )
	{
		m_wndListAInput.StopModal();
	}
	if ( code == ToWord('u', 'p') && pSender->m_pParent == &m_wndListAInput )
	{
		if ( pSender == &m_wndListAInput.m_itmRed ||
			pSender == &m_wndListAInput.m_itmGrn ||
			pSender == &m_wndListAInput.m_itmBlu )
		{
			m_wndListAInput.m_itmColour.Invalidate();
		}
		if ( pSender == &m_wndListAInput.m_itmPosition )
		{
			MainWnd.m_wndLReferences.Invalidate();
		}
		// update
		CWnd::GetTopModal().m_pPrevFocus->Invalidate();
		CCoreOscilloscope::ConfigureAdc();
	}
	if ( code == ToWord('u', 'p') && pSender->m_pParent == &m_wndListDInput )
	{
		if ( pSender == &m_wndListDInput.m_itmRed ||
			pSender == &m_wndListDInput.m_itmGrn ||
			pSender == &m_wndListDInput.m_itmBlu )
		{
			m_wndListDInput.m_itmColour.Invalidate();
		}
		if ( pSender == &m_wndListDInput.m_itmPosition )
		{
			MainWnd.m_wndLReferences.Invalidate();
		}
		// update
		CWnd::GetTopModal().m_pPrevFocus->Invalidate();
	}
	// Selector
	if ( code == ToWord('l', 'e') )	// provider selector combo box
	{
		CValueProvider* pProvider = (CValueProvider*)(NATIVEPTR)data;
		/*
		if ( pProvider->Get() == 0 )
		{
			// TODO: remove? used only for button element testing
			// button ?
			MainWnd.m_wndMessage.Show( this, "Message", ((CProviderBtn*)pProvider)->m_pName, RGB565(ffff00));
			return;
		} else
		{*/
			m_wndComboSelector.Create( GetFocus()->m_pszId, WsVisible | WsModal, 
				CRect( 30, 100, 370, 140), RGB565(ffffff), pProvider, this);
		//}
		m_wndComboSelector.StartModal();
	}
	if ( code == ToWord('o', 'k') && pSender == &m_wndComboSelector )
	{
		// done
		m_wndComboSelector.StopModal();
	}
	if ( code == ToWord('e', 'x') && pSender == &m_wndComboSelector )
	{
		// cancel
		m_wndComboSelector.StopModal();
	}
	if ( code == ToWord('i', 'u') )
	{
		CCoreOscilloscope::ConfigureAdc();
	}
	if ( code == ToWord('r', 'u') )
	{
		Settings.Trig.nLastChange = BIOS::SYS::GetTick();
		MainWnd.m_wndLReferences.Invalidate();
		CCoreOscilloscope::ConfigureTrigger();
	}
}


