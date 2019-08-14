#pragma once
#include <Framework.h>

#include <Oscilloscope/Controls/GraphOsc.h>

#include "ItemMeas.h"
#include "ListMeas.h"

class CWndMenuMeas : public CWnd
{
public:
	// Menu items
	CItemMeas			m_itmMeas[6];
	
	CWndListMeas		m_wndListMeas;

	virtual void		Create(CWnd *pParent, ui16 dwFlags);
	virtual void		OnMessage(CWnd* pSender, ui16 code, ui32 data);

private:
	void				_UpdateAll();
};
