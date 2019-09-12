#ifndef __GENERATOR_MENUMAINMODULATION_H__
#define __GENERATOR_MENUMAINMODULATION_H__

#include <Generator/Core/CoreGenerator.h>
#include <Settings.h>
#include <Framework.h>

class CWndMenuGeneratorMod : public CWnd
{
public:
	CWndMenuItem	m_itm[5];

	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data);
};

#endif