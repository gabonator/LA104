class CWndZoomBar : public CWnd
{
public:
	virtual void Create(CWnd* pParent, uint16_t nFlags, CWnd* pGraph) 
	{
		CRect rcClient( pGraph->m_rcClient );
		rcClient.top = rcClient.bottom + 4;
		rcClient.bottom = rcClient.top + 7;
		CWnd::Create("CWndZoomBar", nFlags | WsNoActivate, rcClient, pParent);
	}

	virtual void OnPaint()
	{
		uint16_t clr = RGB565(808080);
		CRect rcBorder = m_rcClient;
		rcBorder.Inflate(1, 0, 1, 0);
		BIOS::LCD::Bar( rcBorder, clr );
		//rcBorder.Deflate(1, 1, 1, 1);
		//BIOS::LCD::Bar( rcBorder, RGB565(000000));
		
		CRect rc = rcBorder;
		uint16_t nBegin = Settings.Time.Shift;
		uint16_t nView = CWndGraph::DivsX*CWndGraph::BlkX;

		ui32 nXBegin = (nBegin*m_rcClient.Width()) >> 12;
		ui32 nXEnd = ((nBegin+nView)*m_rcClient.Width()) >> 12;

		rc.top++;
		rc.bottom--;
		rc.left = m_rcClient.left + (int16_t)nXBegin;
		rc.right = m_rcClient.left + (int16_t)nXEnd;

		if ( rc.right > rcBorder.right )
			rc.right = rcBorder.right;
		BIOS::LCD::Bar( rc, RGB565(000000) );
	}

};
