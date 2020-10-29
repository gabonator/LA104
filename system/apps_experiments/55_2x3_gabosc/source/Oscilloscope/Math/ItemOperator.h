#ifndef __MENUITEMOPERATOR_H__
#define __MENUITEMOPERATOR_H__

class CItemOperator : public CWndMenuItem
{
public:
	CSettings::MathOperator* m_pOper;

public:
	virtual void Create(CSettings::MathOperator* pOper, CWnd *pParent) 
	{
		_ASSERT( pOper );
		m_pOper = pOper;
		CWndMenuItem::Create( NULL, pOper->uiColor, 2, pParent);
		SetColorPtr( &pOper->uiColor );
	}

	virtual void OnPaint() override
	{
		bool bEnabled = m_pOper->Type != CSettings::MathOperator::_Off;
		ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);

		CWndMenuItem::OnPaint();

		int x = m_rcClient.left + 10 + MarginLeft;
		int y = m_rcClient.top;
		BIOS::LCD::Print( x, y, clr, RGBTRANS, "Function" );
	    y += 16;
		BIOS::LCD::Print( x, y, clr, RGBTRANS, CSettings::MathOperator::ppszTextType[ (int)m_pOper->Type ] );
	}

	virtual void OnKey(int nKey) override
	{
		_ASSERT( m_pOper );
		if ( nKey == BIOS::KEY::Enter )
		{
			SendMessage(m_pParent, ToWord('m', 'c'), (ui32)(NATIVEPTR)m_pOper);
			return;
		}
		if ( nKey == BIOS::KEY::Left && m_pOper->Type > 0 )
		{
			_ASSERT(sizeof(m_pOper->Type) == sizeof(NATIVEENUM));
			DecEnum(m_pOper->Type);
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), (ui32)(NATIVEPTR)m_pOper);
		}
		if ( nKey == BIOS::KEY::Right && m_pOper->Type < CSettings::MathOperator::_TypeMax )
		{
			_ASSERT(sizeof(m_pOper->Type) == sizeof(NATIVEENUM));
			IncEnum(m_pOper->Type);
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), (ui32)(NATIVEPTR)m_pOper);
		}

		CWnd::OnKey( nKey );
	}

};

#endif
