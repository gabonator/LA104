#ifndef __MENUWINDOW_H__
#define __MENUWINDOW_H__

class CItemWindow : public CWndMenuItem
{
	CProviderNum	m_proTime;

public:
	virtual void Create(CWnd *pParent) 
	{
		CWndMenuItem::Create( "Window", RGB565(ffffff), 1, pParent);
		m_proTime.Create( &Settings.Time.Shift, Settings.Time.InvalidFirst, 4096 - CWndGraph::BlkX * CWndGraph::DivsX );
	}

	virtual void OnKey(int nKey) override
	{
		int nMin = Settings.Time.InvalidFirst;
		m_proTime.SetMin(nMin);

		if ( nKey == BIOS::KEY::Left && m_proTime-1 == CValueProvider::Yes )
		{
			for (ui8 t=0; t<32 && m_proTime-1 == CValueProvider::Yes; t++)
				m_proTime--;
			SendMessage(m_pParent, ToWord('w', 'u'), 0);
		}
		if ( nKey == BIOS::KEY::Right && m_proTime+1 == CValueProvider::Yes )
		{
			for (ui8 t=0; t<32 && m_proTime+1 == CValueProvider::Yes; t++)
				m_proTime++;
			SendMessage(m_pParent, ToWord('w', 'u'), 0);
		}

		CWnd::OnKey( nKey );
	}

};

#endif