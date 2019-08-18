#pragma once
#include <Framework.h>
#include <Oscilloscope/Controls/GraphOsc.h>

#include "ItemDisp.h"

class CWndMenuDisplay : public CWnd
{
public:
	// Menu items
	CProviderEnum	m_proAxes;
	CProviderEnum	m_proDraw;
	CProviderEnum	m_proAverage;
	CProviderEnum	m_proPersist;
	CProviderEnum	m_proGrid;
	CProviderEnum	m_proAxis;

	CMPItem m_itmAxes;
	CMPItem m_itmDraw;
	CMPItem m_itmAverage;
	CMPItem m_itmPersist;
	CMPItem m_itmGrid;
	CMPItem m_itmAxis;

	virtual void		Create(CWnd *pParent, int dwFlags);
	virtual void		OnMessage(CWnd* pSender, int code, uintptr_t data) override;
};
