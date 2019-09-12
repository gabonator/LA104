#ifndef __GENERATOR_MENUMAINEDIT_H__
#define __GENERATOR_MENUMAINEDIT_H__

#include <Generator/Core/CoreGenerator.h>
#include <Settings.h>
#include <Framework.h>

class CWndMenuGeneratorEdit : public CWnd
{
public:
	CWndMenuItem	m_itmCopy;
	CWndMenuItem	m_itmNormalize;
	CWndMenuItem	m_itmInvert;
	CWndMenuItem	m_itmEdit;
	CWndMenuItem	m_itmSave;
	CWndMenuItem	m_itmLoad;

	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data);
};

#endif
