#pragma once
#include "Design.h"

class CWndMessageBox : public CWnd
{
	ui16 m_clrFrame;
	PCSTR m_strMessage;

public:
	void Show(CWnd* pParent, PCSTR strCaption, PCSTR strMessage, ui16 nColor)
	{
		m_clrFrame = nColor;
		m_strMessage = strMessage;

		if ( IsWindow() )
		{
			KillTimer();
			StopModal();	// will destroy this window
		}

		CWnd::Create(strCaption, CWnd::WsVisible, CRect(40, 100, BIOS::LCD::Width-40, 160), pParent);
		StartModal();
		BIOS::SYS::Beep(100);
		SetTimer(3000);
	}

	virtual void OnPaint()
	{
		CRect rcClient = m_rcClient;
		CDesign::Window(m_rcClient, m_clrFrame);

		BIOS::LCD::Print( rcClient.CenterX()-((ui8)strlen(m_pszId)<<2), rcClient.top+2, 
			RGB565(000000), RGBTRANS, m_pszId);

		rcClient.Deflate(2+12+4, 2+14+2, 2+12+4, 2+2+0);
		int nExtentW = strlen(m_strMessage)*8;
		int nExtentH = 14;

		BIOS::LCD::Print( rcClient.Center().x - (nExtentW >> 1), rcClient.Center().y - (nExtentH >> 1 ),
			RGB565(000000), RGBTRANS, m_strMessage);		
	}

	void Hide()
	{
		KillTimer();
		StopModal();
	}

	virtual void OnKey(ui16 nKey)
	{
		Hide();
	}

	virtual void OnTimer()
	{		
		Hide();
	}
};

