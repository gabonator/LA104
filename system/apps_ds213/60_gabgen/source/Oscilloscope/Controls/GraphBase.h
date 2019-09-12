#pragma once
#include <Framework.h>

class CWndGraph : public CWnd
{
public:
	enum {
		BlkX = 30,
		BlkY = 25,
		DivsX = 10,
		DivsY = 8,
		MaxX = DivsX*BlkX,
		CenterX = MaxX/2,
		MaxY = DivsY*BlkY,
		CenterY = MaxY/2,
		CenterTop = MaxY/4,
		CenterBottom = 3*CenterTop
	};

//public:
//	static ui16	m_nMaxX;

public:
	virtual void Create(CWnd *pParent, int dwFlags) 
	{
		CWnd::Create("CWndGraph", dwFlags, CRect(12, 22, 12+DivsX*BlkX, 22+DivsY*BlkY), pParent);
//		CWndGraph::m_nMaxX = -1;
	}

	virtual void OnPaint()
	{
		CRect rc = m_rcClient;
		rc.Inflate( 1, 1, 1, 1 );
		BIOS::LCD::Rectangle( rc, RGB565(b0b0b0) );

		const ui16 grid[] = {RGB565(808080), 0, 0, 0, 0};

		for (int x=1; x<10; x++)
			BIOS::LCD::Pattern(m_rcClient.left+BlkX*x, m_rcClient.top, m_rcClient.left+BlkX*x+1, m_rcClient.bottom, grid, COUNT(grid));
		for (int y=1; y<8; y++)
			BIOS::LCD::Pattern(m_rcClient.left, m_rcClient.top+BlkY*y, m_rcClient.right, m_rcClient.top+BlkY*y+1, grid, COUNT(grid));
		//BIOS::Printf( m_rcClient.left+4, m_rcClient.top+4, RGB565(ffffff), RGBTRANS, 
		//	"Total CWnd instances %d", CWnd::m_nInstances);
		BIOS::LCD::Print( m_rcClient.left+4, m_rcClient.top+4, RGB565(404040), RGBTRANS, 
			"Valky.eu");

		BIOS::LCD::Print( m_rcClient.left+4, m_rcClient.bottom-16, RGB565(ffffff), RGBTRANS, 
			"Built on " __DATE__ " " __TIME__);
	}
};
