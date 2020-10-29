#include "Framework.h"
#include "MessageBox.h"
#include "MainMenu.h"

#define MainWnd (*CMainWnd::m_pInstance)

class CMainWnd : public CWnd
{
public:
	static CMainWnd	*m_pInstance;

	CWndMessageBox mWndMessage;
	CWndModuleSelector mWndModuleSel;
	CWndToolBar mWndToolBar;

	void Create();
};