#ifndef __ITEMGENAMPLITUDE_H__
#define __ITEMGENAMPLITUDE_H__

class CItemAmplitude : public CWndMenuItem
{
	int m_nVolt;
public:
	virtual void Create(const char* pszId, ui16 clr, ui8 rows, CWnd *pParent) 
	{
		m_nVolt = 10;
		CWndMenuItem::Create(pszId, clr, rows, pParent);
	}
	void UpdateAdc()
	{
		//float fVolt = Settings.Gen.nScale / 65536/ 10.0f;
	}
	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Left && m_nVolt > 1 )
		{
			m_nVolt--;
			Invalidate();
			UpdateAdc();
			SendMessage(m_pParent, ToWord('i', 'u'), 0);
		}
		if ( nKey == BIOS::KEY::Right && m_nVolt < 30 ) // 1.5x
		{
			m_nVolt++;
			Invalidate();
			UpdateAdc();
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
	float GetAmplitude()
	{
		return m_nVolt*0.1f;
	}
	void SetAmplitude(float fValue)
	{
		m_nVolt = (int)(fValue * 10.0f + 0.01f);
	}
};

#endif
