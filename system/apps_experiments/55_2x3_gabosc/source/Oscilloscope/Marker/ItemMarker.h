#ifndef __MENUITEMMARKER_H__
#define __MENUITEMMARKER_H__

class CItemMarker : public CWndMenuItem
{
	CSettings::Marker* m_pMarker;
	PSTR	m_strId;

public:
	virtual void Create(PSTR strId, CSettings::Marker* pMarker, CWnd *pParent) 
	{
		_ASSERT( pMarker );
		m_pMarker = pMarker;
		m_strId = strId;
		CWndMenuItem::Create( NULL, pMarker->u16Color, 1, pParent);
	}

	virtual void OnPaint() override
	{
		ui16 clr = ( m_pMarker->Mode != 
			CSettings::Marker::_Off ) ? RGB565(000000) : RGB565(808080);
		ui16 clr2 = ( m_pMarker->Mode != 
			CSettings::Marker::_Off ) ? RGB565(404040) : RGB565(909090);

		CWndMenuItem::OnPaint();
		int x = m_rcClient.left + 12 + MarginLeft;
		int y = m_rcClient.top;

		if ( HasFocus() )
			x -= 2;
		x -= 9;
		if ( m_strId[0] == 'X' && m_strId[1] == '1' )
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::curx1);
		else if ( m_strId[0] == 'Y' && m_strId[1] == '1' )
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::cury1);
		else if ( m_strId[0] == 'X' && m_strId[1] == '2' )
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::curx2);
		else if ( m_strId[0] == 'Y' && m_strId[1] == '2' )
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::cury2);
		else
			x += BIOS::LCD::Print( x, y, clr, RGBTRANS, m_strId );

		if ( HasFocus() )
		{
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::sel_left);
			if ( m_pMarker->Mode == CSettings::Marker::_Auto )
				x += BIOS::LCD::Print(x, y+1, RGBTRANS, clr, "A");
			else
				x += BIOS::LCD::Draw(x, y+1, RGBTRANS, clr, CShapes::leftright);
			x += BIOS::LCD::Draw(x, y+1, clr, RGBTRANS, CShapes::sel_right);
			x -= 2;
		} else
			x += 4;

		if ( m_pMarker->Display == CSettings::Marker::_Raw )
			x += BIOS::LCD::Print( x, y, clr, RGBTRANS, CUtils::itoa(m_pMarker->nValue) );
		else
		{
			if ( m_pMarker->Type == CSettings::Marker::_Time )
			{ 
				float fTimeRes = Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
				float fValue = fTimeRes * ( m_pMarker->nValue - ( Settings.Trig.nTime - Settings.Time.Shift) );
				
				x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%f", fValue*1000 );
				if (!HasFocus())
				{
					x += 2;
					BIOS::LCD::Print( x, y, clr2, RGBTRANS, "ms");
				}
			}
			if ( m_pMarker->Type == CSettings::Marker::_Voltage )
			{
				CSettings::Calibrator::FastCalc fastCalc;
				if (m_pMarker->Source == CSettings::Marker::_CH1)
					Settings.CH1Calib.Prepare( &Settings.CH1, fastCalc );
				else
					Settings.CH2Calib.Prepare( &Settings.CH2, fastCalc );

				float fValue = Settings.CH1Calib.Voltage( fastCalc, (float)m_pMarker->nValue );
				x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%f", fValue );
				if (!HasFocus())
				{
					x += 2;
					BIOS::LCD::Print( x, y, clr2, RGBTRANS, "V");
				}
			}
		}
	}

	virtual void OnKey(int nKey) override
	{
		_ASSERT( m_pMarker );
		if ( nKey == BIOS::KEY::Left )
		{
			if ( m_pMarker->Type == CSettings::Marker::_Time && m_pMarker->nValue > 0 )
				m_pMarker->nValue--;
			if ( m_pMarker->Type == CSettings::Marker::_Voltage && m_pMarker->nValue > 0 )
				m_pMarker->nValue--;

			SendMessage(m_pParent, ToWord('u', 'p'), (ui32)(NATIVEPTR)this);
			Invalidate();
		}
		if ( nKey == BIOS::KEY::Right )
		{
			if ( m_pMarker->Type == CSettings::Marker::_Time && m_pMarker->nValue < 1024 )
				m_pMarker->nValue++;
			if ( m_pMarker->Type == CSettings::Marker::_Voltage && m_pMarker->nValue < 256 )
				m_pMarker->nValue++;

			SendMessage(m_pParent, ToWord('u', 'p'), (ui32)(NATIVEPTR)this);
			Invalidate();
		}
		if ( nKey == BIOS::KEY::Enter )
		{
			SendMessage(m_pParent, ToWord('m', 'c'), (ui32)(NATIVEPTR)m_pMarker);
			return;
		}
		CWnd::OnKey( nKey );
	}
};

#endif
