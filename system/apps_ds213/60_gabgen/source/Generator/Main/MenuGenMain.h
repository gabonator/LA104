#ifndef __GENERATOR_H__
#define __GENERATOR_H__

//#include <Source/HwLayer/bios.h>
#include "Framework.h"
#include <Generator/Core/CoreGenerator.h>
#include <Settings.h>

#include "ItemGenWaveform.h"
#include "ItemGenFrequency.h"
#include "ItemGenAmplitude.h"
#include "ItemGenOffset.h"
#include "ItemGenDuty.h"

class CWndMenuGenerator : public CCoreGenerator, public CWnd
{
public:
	CItemGenWaveform	m_itmWave;
	CItemFrequency		m_itmFreq;
	CItemAmplitude		m_itmAmpl;
	CItemOffset			m_itmOffset;
	CItemDuty			m_itmDuty;

	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
	virtual void OnTimer() override;
};

#endif
