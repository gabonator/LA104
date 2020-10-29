#include "MainWnd.h"

CMainWnd* CMainWnd::m_pInstance = nullptr;

void CMainWnd::Create()
{
	m_pInstance = this;
	CWnd::Create("CMainWnd", WsVisible | WsListener, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), NULL );
	mWndToolBar.Create(this);
	mWndModuleSel.Create(this, CWnd::WsVisible);

	constexpr int nMenuItem = 0;
	SendMessage( &mWndToolBar, ToWord('g', 'i'), nMenuItem);
	mWndModuleSel.SetFocus();
}
