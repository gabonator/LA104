class CWndTimeReferences : public CWnd
{
public:
	virtual void Create(CWnd *pParent, int dwFlags) 
	{
		CWnd::Create("CWndTimeReferences", dwFlags | CWnd::WsNoActivate, 
			CRect(12, 16, 12+CWndGraph::DivsX*CWndGraph::BlkX, 21), pParent);
	}

	virtual void OnPaint()
	{
		//BIOS::LCD::Bar( m_rcClient, RGB565(000000) );
		GUI::Background(m_rcClient, RGB565(101010), RGB565(404040));
		si16 x = (Settings.Trig.nTime-Settings.Time.Shift) /*>> 2*/;
		if ( x >= 0 && x < m_rcClient.Width() )
			BIOS::LCD::Draw( m_rcClient.left + x-4, m_rcClient.top, 
				RGB565(606060), RGBTRANS, CShapes::trig_time );
	}
};
