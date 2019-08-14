class CWndSigGraph : public CWndGraph
{
	const ui16* m_pSignal;
	ui16 m_nCount;

public:
	CWndSigGraph() : CWndGraph()
	{
		m_pSignal = NULL;
	}

	void Setup(const ui16* pSignal, ui16 nCount )
	{
		m_pSignal = pSignal;
		m_nCount = nCount;
		Invalidate();
	}

	virtual void OnPaint()
	{
		BIOS::LCD::Bar( m_rcClient, RGB565(000000) );	

		CWndGraph::OnPaint();
		if (!m_pSignal)
			return;

		int nWidth = m_rcClient.Width();
		int nHeight = m_rcClient.Height();

		nHeight -= BlkY*2;
		int nBaseY = m_rcClient.bottom-BlkY;
		ui16 oy = 0;
		for (ui16 i=0; i<nWidth; i++)
		{
			ui16 y = m_pSignal[i*m_nCount/nWidth];
			_ASSERT( y < 0x1000 );
			y = y*nHeight/0x1000;
			if (i==0)
				oy = y;
			
			BIOS::LCD::Line( 
				m_rcClient.left + i-1, nBaseY - oy, 
				m_rcClient.left + i, nBaseY - y, 
				RGB565(ffffff) );
			oy = y;
		}
	}
};
