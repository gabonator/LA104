#ifndef __MENUITEMMARKERDELTA_H__
#define __MENUITEMMARKERDELTA_H__

class CItemMarkerDelta : public CWndMenuItem
{
	CSettings::Marker* m_pMarker1;
	CSettings::Marker* m_pMarker2;
	PSTR	m_strId;

public:
	virtual void Create(PSTR strId, CSettings::Marker* pMarker1, CSettings::Marker* pMarker2, CWnd *pParent) 
	{
		_ASSERT( pMarker1 );
		_ASSERT( pMarker2 );
		m_pMarker1 = pMarker1;
		m_pMarker2 = pMarker2;
		m_strId = strId;
	
		if ( m_pMarker1->Type == CSettings::Marker::_Voltage )
			CWndMenuItem::Create( NULL, RGB565(8080b0), 2, pParent);
		else
			CWndMenuItem::Create( NULL, RGB565(8080b0), 3, pParent);
	}

	virtual void OnPaint()
	{
		bool bDisabled = ( m_pMarker1->Mode == CSettings::Marker::_Off ) ||
			( m_pMarker2->Mode == CSettings::Marker::_Off );

		ui16 clr = bDisabled ? RGB565(808080) : RGB565(000000);

		CWndMenuItem::OnPaint();
		int x = m_rcClient.left + 12 + MarginLeft;
		int y = m_rcClient.top;
		int _x = x;
		x += 8;
		x += BIOS::LCD::Print( x, y, clr, RGBTRANS, m_strId );
		if ( bDisabled )
			return;

		x = _x;
		y += 16;
		//x += BIOS::LCD::Print( x, y, clr, RGBTRANS, CUtils::itoa(nResult) );

		if ( m_pMarker1->Display == CSettings::Marker::_Raw )
		{
			int nResult = m_pMarker1->nValue - m_pMarker2->nValue;
			x += BIOS::LCD::Print( x, y, clr, RGBTRANS, CUtils::itoa(nResult) );
		}
		else
		{
			if ( m_pMarker1->Type == CSettings::Marker::_Time )
			{ 
				float fTimeRes = Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
				float fValue = fTimeRes * ( m_pMarker2->nValue - m_pMarker1->nValue );
				x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%f", fValue*1000 ) * 8;
				x += 2;
				BIOS::LCD::Print( x, y, RGB565(404040), RGBTRANS, "ms");
				x = _x;
				y += 16;
				if ( fValue == 0 )
					BIOS::LCD::Print( x, y, clr, RGBTRANS, "Error");
				else
				{
					if ( fValue < 0 )
						fValue = -1.0f / fValue;
					else
						fValue = 1.0f / fValue;
					fValue /= 1000.0f; // kHz
					fValue += 0.0001f; // sprintf rounding 
					x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%f", fValue ) * 8;
					x += 2;
					BIOS::LCD::Print( x, y, RGB565(404040), RGBTRANS, "kHz");
				}
			}
			if ( m_pMarker1->Type == CSettings::Marker::_Voltage )
			{
				if ( m_pMarker1->Source != m_pMarker2->Source )
				{
					BIOS::LCD::Print( x, y, RGB565(808080), RGBTRANS, "Error");
				} else
				{
					CSettings::Calibrator::FastCalc fastCalc;
					if (m_pMarker1->Source == CSettings::Marker::_CH1)
						Settings.CH1Calib.Prepare( &Settings.CH1, fastCalc );
					else
						Settings.CH2Calib.Prepare( &Settings.CH2, fastCalc );

					float fValue1 = Settings.CH1Calib.Voltage( fastCalc, (float)m_pMarker1->nValue );
					float fValue2 = Settings.CH1Calib.Voltage( fastCalc, (float)m_pMarker2->nValue );
					float fResult = fValue2 - fValue1;
					x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%f", fResult ) * 8;
					x += 2;
					BIOS::LCD::Print( x, y, RGB565(404040), RGBTRANS, "V");
				}
			}
		}

		

	}
};

#endif