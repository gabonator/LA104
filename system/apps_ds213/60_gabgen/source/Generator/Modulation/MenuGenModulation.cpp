#include "MenuGenModulation.h"
#include <Source/Gui/MainWnd.h>

/*virtual*/ void CWndMenuGeneratorMod::Create(CWnd *pParent, ui16 dwFlags) 
{
	CWnd::Create("CWndMenuGeneratorMod", dwFlags, CRect(280-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);
	m_itm[0].Create("~Mode\nFM", RGB565(b00040), 2, this );
	m_itm[1].Create("~Depth\n500 Hz", RGB565(b00040), 2, this );
	m_itm[2].Create("~Source\nInternal", RGB565(b00040), 2, this );
}

/*virtual*/ void CWndMenuGeneratorMod::OnMessage(CWnd* pSender, ui16 code, ui32 data)
{
}

