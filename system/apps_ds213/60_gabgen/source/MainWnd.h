#include "Framework.h"
#include "Oscilloscope/Oscilloscope.h"
#include "Generator/Generator.h"
#include "ToolBar.h"

#define MainWnd (*CMainWnd::m_pInstance)

class CMainWnd : public CWnd
{
public:
	static CMainWnd	*m_pInstance;

	CWndToolBar mWndToolBar;

	CWndSigGraph		m_wndSignalGraph;
	CWndMenuGenerator	m_wndMenuGenerator;
//	CWndMenuGeneratorMod	m_wndMenuGeneratorMod;
	CWndMenuGeneratorEdit	m_wndMenuGeneratorEdit;

	bool				mRunning{true};
	
	void Create();
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
	virtual void WindowMessage(int nMsg, int nParam = 0) override;
	virtual void OnPaint() override;
	bool IsRunning();
};
