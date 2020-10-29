#include "WndButton.h"

class CWndConfirm : public CWnd
{
	ui16 m_clrFrame;
	PCSTR m_strMessage;
	PCSTR m_strButton1;
	PCSTR m_strButton2;
	CWndButton m_wndButton1;
	CWndButton m_wndButton2;

public:
	void Show(CWnd* pParent, PCSTR strCaption, PCSTR strMessage, ui16 nColor, PCSTR strButton1, PCSTR strButton2)
	{
		m_clrFrame = nColor;
		m_strMessage = strMessage;
		m_strButton1 = strButton1;
		m_strButton2 = strButton2;

		if ( IsWindow() )
			StopModal();	// will destroy this window
		
		int nLines = 1;
		for ( int i = 0; strMessage[i]; i++ )
			if ( strMessage[i] == '\n' )
				nLines++;

		CWnd::Create(strCaption, CWnd::WsVisible | CWnd::WsModal, CRect(30, 90, 350, 140 + nLines*16), pParent);
		m_wndButton1.Create( strButton1, CWnd::WsVisible, 
			CRect( m_rcClient.left + 20, m_rcClient.bottom - 20, m_rcClient.left + 20 + 40, m_rcClient.bottom - 4 ),
			this);
		m_wndButton2.Create( strButton2, CWnd::WsVisible, 
			CRect( m_rcClient.right - 20 - 40, m_rcClient.bottom - 20, m_rcClient.right - 20, m_rcClient.bottom - 4 ),
			this);
		StartModal( &m_wndButton1 );
	}

	void Hide()
	{
		if ( IsWindow() )
			StopModal();
	}

	virtual void OnPaint() override
	{
		CRect rcClient = m_rcClient;
		CDesign::Window(m_rcClient, m_clrFrame);

		BIOS::LCD::Print( rcClient.CenterX()-((ui8)strlen(m_pszId)<<2), rcClient.top+2, 
			RGB565(000000), RGBTRANS, m_pszId);

		rcClient.Deflate(2+12+4, 2+14+2, 2+12+4, 2+2+0);

		BIOS::LCD::Print( rcClient.left + 4, rcClient.top + 4,
			RGB565(000000), RGBTRANS, m_strMessage);		
	}
	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::KeyEnter )
		{
			_ASSERT( GetFocus() );
			SendMessage( GetParent(), ToWord('e', 'd'), (ui32)GetFocus()->m_pszId);
		}
	}
};
