class CWndOscInfo : public CWnd
{
public:
	virtual void Create(CWnd* pParent, ui16 nFlags, CWnd* pGraph) 
	{
		CRect rcClient( pGraph->m_rcClient );
		rcClient.top = rcClient.bottom + 2;
		rcClient.bottom = rcClient.top + 7;
		CWnd::Create("CWndOscInfo", nFlags | WsNoActivate, rcClient, pParent);
	}

	virtual void OnPaint()
	{
		int x = m_rcClient.left, y = m_rcClient.top;
		int _x = x;
        x += BIOS::LCD::Print( x, y, Settings.CH1.u16Color, RGB565(000000), "CH1: " );
        x += BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(000000), 
            CSettings::AnalogChannel::ppszTextResolutionByProbe[Settings.CH1.Probe][Settings.CH1.Resolution]);  

        x = _x + 100;
        x += BIOS::LCD::Print( x, y, Settings.CH2.u16Color, RGB565(000000), "CH2: " );
        x += BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(000000), 
            CSettings::AnalogChannel::ppszTextResolutionByProbe[Settings.CH2.Probe][Settings.CH2.Resolution]);

		x = _x + 200;
		x += BIOS::LCD::Print( x, y, RGB565(b0b0b0), RGB565(000000), "T: " );
		x += BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(000000), 
			CSettings::TimeBase::ppszTextResolution[ Settings.Time.Resolution ]);
	}

};
