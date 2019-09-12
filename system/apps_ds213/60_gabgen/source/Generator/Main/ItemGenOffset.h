#ifndef __ITEMGENOFFSET_H__
#define __ITEMGENOFFSET_H__

class CItemOffset : public CWndMenuItem
{
	int m_nVolt;

public:
	virtual void Create(const char* pszId, ui16 clr, ui8 rows, CWnd *pParent) 
	{
		m_nVolt = 10;
		CWndMenuItem::Create(pszId, clr, rows, pParent);
	}
	
	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Left && m_nVolt > 0 )
		{
			m_nVolt--;
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), 0);
		}
		if ( nKey == BIOS::KEY::Right && m_nVolt < 20 )
		{
			m_nVolt++;
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), 0);
		}
		CWndMenuItem::OnKey( nKey );
	}
	virtual void OnPaint()
	{
		CWndMenuItem::OnPaint();

		//BIOS::LCD::Printf( m_rcClient.left + 12 + 16, m_rcClient.top + 16, RGB565(000000), RGBTRANS, "%1f V", 
		//	m_nVolt/10.0f+0.01f ); // rounding error!?
		BIOS::LCD::Printf( m_rcClient.left + 12 + 16, m_rcClient.top + 16, RGB565(000000), RGBTRANS, "%d%%", 
			m_nVolt*5 );
	}
	float GetOffset()
	{
		return m_nVolt*0.1f;
	}
	void SetOffset(float fValue)
	{
		m_nVolt = (int)(fValue * 10.0f + 0.01f);
	}

};

#endif
