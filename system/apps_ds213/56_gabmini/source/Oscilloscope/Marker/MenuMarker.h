#pragma once
#include <Framework.h>
#include <Oscilloscope/Controls/GraphOsc.h>

#include "ItemMarker.h"
#include "ItemDelta.h"
#include "ItemMarker.h"
#include "ListMarker.h"

class CWndMenuCursor : public CWnd
{
public:
	// Menu items
	CItemMarker			m_itmX1;
	CItemMarker			m_itmX2;
	CItemMarker			m_itmY1;
	CItemMarker			m_itmY2;
	CItemMarkerDelta	m_itmDeltaX;
	CItemMarkerDelta	m_itmDeltaY;
	
	CWndListMarker		m_wndListCursor;

	CWndMenuCursor();

	virtual void		Create(CWnd *pParent, int dwFlags);
	virtual void		OnMessage(CWnd* pSender, int code, uintptr_t data) override;

private:
	void				Find(CSettings::Marker* pMarker, CSettings::Marker::EFind mode);
};
