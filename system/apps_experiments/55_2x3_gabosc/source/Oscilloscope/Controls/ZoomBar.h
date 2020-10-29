class CWndZoomBar : public CWnd
{
public:
	virtual void Create(CWnd* pParent, ui16 nFlags, CWnd* pGraph) 
	{
		CRect rcClient( pGraph->m_rcClient );
		rcClient.top = rcClient.bottom + 4;
		rcClient.bottom = rcClient.top + 7;
		CWnd::Create("CWndZoomBar", nFlags | WsNoActivate, rcClient, pParent);
	}

	virtual void OnPaint()
	{
		ui16 clr = RGB565(808080);
		CRect rcBorder = m_rcClient;
		rcBorder.Inflate(1, 0, 1, 0);
		BIOS::LCD::Bar( rcBorder, clr );
		//rcBorder.Deflate(1, 1, 1, 1);
		//BIOS::LCD::Bar( rcBorder, RGB565(000000));
		
		CRect rc = rcBorder;
		ui16 nBegin = Settings.Time.Shift;
		ui16 nView = CWndGraph::DivsX*CWndGraph::BlkX;

		ui32 nXBegin = (nBegin*m_rcClient.Width()) >> 12;
		ui32 nXEnd = ((nBegin+nView)*m_rcClient.Width()) >> 12;

		rc.top++;
		rc.bottom--;
		rc.left = m_rcClient.left + (si16)nXBegin;
		rc.right = m_rcClient.left + (si16)nXEnd;

		if ( rc.right > rcBorder.right )
			rc.right = rcBorder.right;
		BIOS::LCD::Bar( rc, RGB565(000000) );
	}

};
