#pragma once
#include <Framework.h>
#include <Oscilloscope/Controls/GraphOsc.h>

#include "ItemOperand.h"
#include "ItemOperator.h"
#include "ListOperand.h"
#include "ListOperator.h"


class CWndMenuMath : public CWnd
{
  enum {
    A = 0, 
    B = 1,
    C = 2
  };

public:
	// Menu items
	CItemOperand		m_itmOperand[3];
	CItemOperator		m_itmOperator;
	
	CWndListOperand		m_wndListOperand;
	CWndListOperator	m_wndListOperator;

	virtual void		Create(CWnd *pParent, int dwFlags);
	virtual void		OnMessage(CWnd* pSender, int code, uintptr_t data) override;
};
