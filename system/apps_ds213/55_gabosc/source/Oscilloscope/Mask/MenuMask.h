#pragma once
#include <Framework.h>
#include <Oscilloscope/Controls/GraphOsc.h>
#include "../Disp/ItemDisp.h"

class CMIButton : public CWndMenuItem
{
public:
	virtual void Create(const char* pszId, ui16 clr, int nRows, CWnd *pParent) 
	{
		CWndMenuItem::Create( pszId, clr, nRows, pParent);
	}

	virtual void OnPaint()
	{
		CWndMenuItem::OnPaint();
	}

	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Enter || nKey == BIOS::KEY::Left || nKey == BIOS::KEY::Right )
			SendMessage( GetParent(), CWnd::WmKey, nKey );
		CWnd::OnKey( nKey );
	}
};

class CWndMenuMask : public CWnd
{
public:
	enum EAction 
	{
		ActionNone = 0,
		ActionBeep = 1,
		ActionBeepStop = 2,
		ActionStop = 3,
		ActionMax = ActionStop
	};
	enum EDisplay 
	{
		DisplayNo = 0,
		DisplayYes = 1,
		DisplayMax = DisplayYes
	};

	static const char* const m_ppszTextAction[];
	static const char* const m_ppszDispAction[];

public:
	// Menu items
	CMIButton	m_btnSource;
	CMIButton	m_btnReset;
	CMIButton	m_btnCollect;
	CMIButton	m_btnStop;
	//CMIButton	m_btnAction;
	CMIButton	m_btnExpand;
	CMIButton	m_btnBlur;
	CMPItem		m_itmAction;
	CProviderEnum	m_proAction;
	CMPItem		m_itmDisplay;
	CProviderEnum	m_proDisplay;

	EAction		m_Action;
	EDisplay	m_Display;

	virtual void		Create(CWnd *pParent, int dwFlags);
	virtual void		OnMessage(CWnd* pSender, int code, uintptr_t data) override;
};
