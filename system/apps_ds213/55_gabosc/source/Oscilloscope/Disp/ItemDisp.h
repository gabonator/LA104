#pragma once
#include <Framework.h>

class CMPItem : public CWndMenuItem
{
protected:
	CValueProvider* m_pProvider;

public:
	void Create(const char* pszId, ui16 clr, CValueProvider* pProvider, CWnd* pParent)
	{
		m_pProvider = pProvider;
		CWndMenuItem::Create( pszId, clr, 2, pParent);
	}

	virtual void OnPaint() override
	{
		ui16 clr = RGB565(000000);

		CWndMenuItem::OnPaint();
		int x = m_rcClient.left + 12 + MarginLeft;
		int y = m_rcClient.top;
		BIOS::LCD::Print( x, y, clr, RGBTRANS, m_pszId );
		y += 16;
		if ( HasFocus() )
		{
			x += BIOS::LCD::Draw(x, y, RGB565(000000), RGBTRANS, CShapes::sel_left);
			CRect rcRect(x, y, x + m_pProvider->GetWidth(), y + 14);
			BIOS::LCD::Bar( rcRect, RGB565(0000000) );
			m_pProvider->OnPaint( rcRect, HasFocus() );
			x = rcRect.right;
			x += BIOS::LCD::Draw(x, y, RGB565(000000), RGBTRANS, CShapes::sel_right);
		} else
		{
			CRect rcRect(x, y, x + m_pProvider->GetWidth(), y + 14);
			m_pProvider->OnPaint( rcRect, HasFocus() );
		}
	}

	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Left && (*m_pProvider)-1 == CValueProvider::Yes )
		{
			(*m_pProvider)--;
			Invalidate();
			SendMessage(m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey == BIOS::KEY::Right && (*m_pProvider)+1 == CValueProvider::Yes )
		{
			(*m_pProvider)++;
			Invalidate();
			SendMessage(m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey == BIOS::KEY::Enter  )
		{
			// is that provider enumerator ?
			if ( m_pProvider->Get() !=	CValueProvider::Invalid )
			{
				SendMessage(m_pParent, ToWord('l', 'e'), (ui32)(NATIVEPTR)(m_pProvider));
			}
		}
		CWndMenuItem::OnKey( nKey );
	}
};
