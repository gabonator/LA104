#pragma once
#include <Framework.h>
#include <Oscilloscope/Controls/GraphOsc.h>

#include "ListAnalog.h"
#include "ListDigital.h"
#include "ListTrigger.h"
#include "ItemAnalog.h"
#include "ItemDigital.h"
#include "ItemTime.h"
#include "ItemTrigger.h"
#include "ItemWindow.h"

class CWndMenuInput : public CWnd
{
public:
	// Menu items
	CItemAnalog		m_itmCH1;
	CItemAnalog		m_itmCH2;
	CItemDigital	m_itmCH3;
	CItemDigital	m_itmCH4;
	CItemTime		m_itmTime;
	CItemWindow		m_itmWindow;
	CItemTrigger	m_itmTrig;

	// Lists
	CWndListAInput	m_wndListAInput;
	CWndListDInput	m_wndListDInput;
	CComboSelector	m_wndComboSelector;
	CWndListTrigger	m_wndListTrigger;

	CWndMenuInput();

	virtual void Create(CWnd *pParent, int dwFlags);
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
};
