#include "Framework.h"
#include "Oscilloscope/Oscilloscope.h"
#include "ToolBar.h"

#define MainWnd (*CMainWnd::m_pInstance)

class CMainWnd : public CWnd
{
public:
	static CMainWnd	*m_pInstance;

	CWndToolBar mWndToolBar;

	CWndOscGraph		m_wndGraph;

	CWndMenuInput		m_wndMenuInput;
	CWndMenuCursor		m_wndMenuCursor;
	CWndMenuDisplay		m_wndMenuDisplay;

	CWndMessageBox		m_wndMessage;	
	CWndZoomBar			m_wndZoomBar;
	CWndOscInfo			m_wndInfoBar;
	CWndLevelReferences	m_wndLReferences;
	CWndTimeReferences	m_wndTReferences;

	bool				mRunning{true};
	
	void Create();
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
	virtual void WindowMessage(int nMsg, int nParam = 0) override;
	virtual void OnPaint() override;
	bool IsRunning();
};
