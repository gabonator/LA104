#ifndef __MENUITEMOPERAND_H__
#define __MENUITEMOPERAND_H__

class CItemOperand : public CWndMenuItem
{
public:
	CSettings::MathOperand* m_pOper;

public:
	virtual void Create(CSettings::MathOperand* pOper, CWnd *pParent) 
	{
		_ASSERT( pOper );
		m_pOper = pOper;
		CWndMenuItem::Create( NULL, RGB565(606060), 2, pParent);
	}

	virtual void OnPaint()
	{
		bool bEnabled = true;
		ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);

		CWndMenuItem::OnPaint();

		int x = m_rcClient.left + 10 + MarginLeft;
		int y = m_rcClient.top;
		int _x = x;
		x += BIOS::LCD::Print( x, y, clr, RGBTRANS, m_pOper->strName );
		BIOS::LCD::Print( x, y, clr, RGBTRANS, CSettings::MathOperand::ppszTextType[ (int)m_pOper->Type ] );
		x = _x;
		y += 16;
		if ( m_pOper->Type == CSettings::MathOperand::_Fx )
			BIOS::LCD::Printf( x, y, clr, RGBTRANS, "Fx(%d)", m_pOper->nConstant);
		else
		if ( m_pOper->Type != CSettings::MathOperand::_Constant )
			BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%d%%", m_pOper->nScale);
		else
			BIOS::LCD::Printf( x, y, clr, RGBTRANS, "=%d", m_pOper->nConstant);
	}

	virtual void OnKey(int nKey) override
	{
		_ASSERT( m_pOper );
		if ( nKey == BIOS::KEY::Enter )
		{
			SendMessage(m_pParent, ToWord('m', 'c'), (ui32)(NATIVEPTR)m_pOper);
			return;
		}
		if ( m_pOper->Type == CSettings::MathOperand::_Constant ||
			 m_pOper->Type == CSettings::MathOperand::_Fx ) 
		{
			if ( nKey == BIOS::KEY::Left && m_pOper->nConstant > -255 )
			{
				m_pOper->nConstant--;
				Invalidate();
			}
			if ( nKey == BIOS::KEY::Right && m_pOper->nConstant < 255 )
			{
				m_pOper->nConstant++;
				Invalidate();
			}
		}

		CWnd::OnKey( nKey );
	}

};

#endif