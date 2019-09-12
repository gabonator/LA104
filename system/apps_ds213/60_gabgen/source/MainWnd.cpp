#include "MainWnd.h"

CMainWnd* CMainWnd::m_pInstance = nullptr;
CSettings m_Settings; // TODO:!

long m_lLastAcquired = 0;

void CMainWnd::Create()
{	
	m_pInstance = this;
	CWnd::Create("CMainWnd", WsVisible | WsListener, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), NULL );
	mWndToolBar.Create(this);
	//m_wndGraph.Create( this, WsHidden | WsNoActivate );

	m_wndSignalGraph.Create( this, WsNoActivate | WsVisible);
	m_wndMenuGenerator.Create( this, WsHidden );
	m_wndMenuGeneratorEdit.Create( this, WsHidden );

	constexpr int nMenuItem = 1;
	SendMessage( &mWndToolBar, ToWord('g', 'i'), nMenuItem);
	m_wndMenuGenerator.SetFocus();
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
	CWnd::WindowMessage(nMsg, nParam);
}
