#ifndef __MENUDIGITAL_H__
#define __MENUDIGITAL_H__

class CItemDigital : public CWndMenuItem
{
	CSettings::DigitalChannel* m_pInfo;

public:
	virtual void Create(CSettings::DigitalChannel* pInfo, CWnd *pParent) 
	{
		_ASSERT( pInfo );
		m_pInfo = pInfo;
		CWndMenuItem::Create( NULL, pInfo->u16Color, 1, pParent);
		CWndMenuItem::SetColorPtr( &pInfo->u16Color );
	}

	virtual void OnPaint() override
	{
		ui16 clr = ( m_pInfo->Enabled == 
			CSettings::DigitalChannel::_YES ) ? RGB565(000000) : RGB565(808080);

		CWndMenuItem::OnPaint();
		int x = m_rcClient.left + 12 + MarginLeft;
		int y = m_rcClient.top;
		x += BIOS::LCD::Print( x, y, clr, RGBTRANS, 
			m_pInfo->pszName );
		x += 8;
		
		if ( m_pInfo->Polarity == CSettings::DigitalChannel::_POS )
			x += BIOS::LCD::Draw( x, y, clr, RGBTRANS, CShapes::sig_dig );

		if ( m_pInfo->Polarity == CSettings::DigitalChannel::_NEG )
			x += BIOS::LCD::Draw( x, y, clr, RGBTRANS, CShapes::sig_dign );

		if ( HasFocus() )
		{
			//x -= 8;
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::sel_left);
			x += BIOS::LCD::Draw(x, y+1, RGBTRANS, clr, CShapes::updown);
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::sel_right);
		}
	}

	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Left && m_pInfo->u16Position > -100 )
		{
			m_pInfo->u16Position--;
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), (ui32)(NATIVEPTR)m_pInfo);
		}
		if ( nKey == BIOS::KEY::Right && m_pInfo->u16Position < 100 )
		{
			m_pInfo->u16Position++;
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), (ui32)(NATIVEPTR)m_pInfo);
		}
		if ( nKey == BIOS::KEY::Enter )
		{
			SendMessage(m_pParent, ToWord('m', 'd'), (ui32)(NATIVEPTR)m_pInfo);			
		}
		CWnd::OnKey( nKey );
	}
};

#endif