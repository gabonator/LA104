#ifndef __MENUTRIGGER_H__
#define __MENUTRIGGER_H__

class CItemTrigger : public CWndMenuItem
{
	CProviderNum	m_proLevel;
	int						m_nTimer;
public:
	virtual void Create(CWnd *pParent) 
	{
		CWndMenuItem::Create( "Trig", RGB565(404040), 2, pParent);

		m_proLevel.Create( &Settings.Trig.nLevel, 0, 255 );
		SetTimer(500);
		m_nTimer = 0;
	}

	virtual void OnPaint()
	{
		ui16 clr = RGB565(000000);
		ui16 clrSource = RGB565(ff00ff);

		switch ( Settings.Trig.Source )
		{
			case CSettings::Trigger::_CH1: 
				clrSource = Settings.CH1.u16Color; 
				break;
			case CSettings::Trigger::_CH2: 
				clrSource = Settings.CH2.u16Color; 
				break;
			case CSettings::Trigger::_CH3: 
				clrSource = Settings.CH3.u16Color; 
				break;
			case CSettings::Trigger::_CH4: 
				clrSource = Settings.CH4.u16Color; 
				break;
			case CSettings::Trigger::_Math: 
				_ASSERT(0);
		}


		CWndMenuItem::OnPaint();
		int x = m_rcClient.left + 12 + MarginLeft;
		int y = m_rcClient.top;
		BIOS::LCD::Print( x, y, clr, RGBTRANS, m_pszId );
		x += 52;

		const char *pSym1 = NULL;
		const char *pSym2 = NULL;
		switch (Settings.Trig.Type)
		{
			case CSettings::Trigger::_EdgeLH:
				{
					pSym1 = CShapes::trig_pos_;
					pSym2 = CShapes::trig_pos;
				} break;
			case CSettings::Trigger::_EdgeHL:
				{
					pSym1 = CShapes::trig_neg_;
					pSym2 = CShapes::trig_neg;
				} break;
			case CSettings::Trigger::_LevelLow: pSym2 = CShapes::trig_level_l; break;
			case CSettings::Trigger::_LevelHigh: pSym2 = CShapes::trig_level_g; break;
			case CSettings::Trigger::_LowerDTLow: pSym2 = CShapes::trig_pulse_ll; break;
			case CSettings::Trigger::_GreaterDTLow: pSym2 = CShapes::trig_pulse_gl; break;
			case CSettings::Trigger::_LowerDTHigh: pSym2 = CShapes::trig_pulse_lh; break;
			case CSettings::Trigger::_GreaterDTHigh: pSym2 = CShapes::trig_pulse_gh; break;
		}

		if (pSym1)
			BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, pSym1 );
		x += BIOS::LCD::Draw( x, y, clrSource, RGBTRANS, pSym2 );
		
		x = m_rcClient.left + 12 + MarginLeft;
		y += 16;
		if ( HasFocus() )
		{
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::sel_left);
			x += BIOS::LCD::Draw(x, y+1, RGBTRANS, clr, CShapes::updown);
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::sel_right);

			x = m_rcClient.left + 12 + MarginLeft;
		} else
		{
			BIOS::LCD::Print( x, y, clr, RGBTRANS, CSettings::Trigger::ppszTextSync[Settings.Trig.Sync]);
		}

		x += 52;

		if ( !BIOS::ADC::Enabled() )
		{
			x += BIOS::LCD::Draw( x, y, RGB565(ff0000), RGBTRANS, CShapes::trig_stop);
		} else
		{
			switch ( Settings.Trig.State )
			{
			case CSettings::Trigger::_Run:
				x += BIOS::LCD::Draw( x, y, RGB565(00b000), RGBTRANS, CShapes::trig_run);
				break;
			case CSettings::Trigger::_Stop:
				x += BIOS::LCD::Draw( x, y, RGB565(ff0000), RGBTRANS, CShapes::trig_stop);
				break;
			case CSettings::Trigger::_Wait:
				x += BIOS::LCD::Print( x, y, RGB565(ff0000), RGBTRANS, "W");
				break;
			case CSettings::Trigger::_Unsync:
				_ASSERT(0);
			}
		}
	}

	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Left && m_proLevel-1 == CValueProvider::Yes )
		{
			m_proLevel--;
			SendMessage(m_pParent, ToWord('r', 'u'), 0);
		}
		if ( nKey == BIOS::KEY::Right && m_proLevel+1 == CValueProvider::Yes )
		{
			m_proLevel++;
			SendMessage(m_pParent, ToWord('r', 'u'), 0);
		}
		if ( nKey == BIOS::KEY::Enter )
		{
			SendMessage(m_pParent, ToWord('m', 'r'), 0);
		}

		CWnd::OnKey( nKey );
	}

	virtual void OnTimer() override
	{
		if ( IsVisible() && 
			BIOS::ADC::Enabled() && Settings.Trig.State == CSettings::Trigger::_Wait )
		{
			int x = m_rcClient.left + 12 + MarginLeft;
			int y = m_rcClient.top;
			x += 52;
			y += 16;
			if ( m_nTimer++ & 1 )
				BIOS::LCD::Print( x, y, RGB565(b00000), RGBTRANS, "W");
			else
				BIOS::LCD::Print( x, y, RGB565(00b000), RGBTRANS, "W");
		}
	}
};

#endif