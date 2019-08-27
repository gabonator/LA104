#include "MenuDisp.h"

#include <MainWnd.h>

/*virtual*/ void CWndMenuDisplay::Create(CWnd *pParent, int dwFlags) 
{
	CWnd::Create("CWndMenuDisp", dwFlags, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);

	m_proAxes.Create( (const char**)CSettings::Display::ppszTextAxes,
		(NATIVEENUM*)&Settings.Disp.Axes, CSettings::Display::_AxesMax );
	m_proDraw.Create( (const char**)CSettings::Display::ppszTextDraw,
		(NATIVEENUM*)&Settings.Disp.Draw, CSettings::Display::_DrawMax );
	m_proAverage.Create( (const char**)CSettings::Display::ppszTextAverage,
		(NATIVEENUM*)&Settings.Disp.Average, CSettings::Display::_AverageMax );
	m_proPersist.Create( (const char**)CSettings::Display::ppszTextPersist,
		(NATIVEENUM*)&Settings.Disp.Persist, CSettings::Display::_PersistMax );
	m_proGrid.Create( (const char**)CSettings::Display::ppszTextGrid,
		(NATIVEENUM*)&Settings.Disp.Grid, CSettings::Display::_GridMax );
	m_proAxis.Create( (const char**)CSettings::Display::ppszTextAxis,
		(NATIVEENUM*)&Settings.Disp.Axis, CSettings::Display::_AxisMax );

	m_itmAxes.Create("Mode", RGB565(808080), &m_proAxes, this);
	m_itmDraw.Create("Display", RGB565(808080), &m_proDraw, this);
	m_itmAverage.Create("Average", RGB565(808080), &m_proAverage, this);
	m_itmPersist.Create("Persist", RGB565(808080), &m_proPersist, this);
	m_itmGrid.Create("Grid", RGB565(808080), &m_proGrid, this);
	m_itmAxis.Create("Axis", RGB565(808080), &m_proAxis, this);
}

/*virtual*/ void CWndMenuDisplay::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndGraph.ShowWindow(false);
		MainWnd.m_wndInfoBar.ShowWindow(false);
		MainWnd.m_wndLReferences.ShowWindow(false);
		MainWnd.m_wndTReferences.ShowWindow(false);
	}

	if (code == ToWord('L', 'E') )
	{
		MainWnd.m_wndGraph.ShowWindow(true);
		MainWnd.m_wndInfoBar.ShowWindow(true);
		MainWnd.m_wndLReferences.ShowWindow(true);
		MainWnd.m_wndTReferences.ShowWindow(true);
	}

	if ( code == ToWord('l', 'e') && data == (ui32)&m_proAverage )
	{
		MainWnd.m_wndGraph.ClearAverage();
	}
	if ( code == ToWord('l', 'e') && data == (ui32)&m_proPersist )
	{	
		MainWnd.m_wndGraph.ClearPersist();
	}
	if ( code == ToWord('u', 'p') && pSender == &m_itmAxes )
	{	
		// force redraw background (invalidate is not enough)
		MainWnd.m_wndGraph.ShowWindow(false);
		MainWnd.m_wndGraph.ShowWindow(true);
	}

	/* // produces nice effect when not cleared :)
	if ( code == ToWord('u', 'p') && pSender == &m_itmAverage )
	{
		MainWnd.m_wndGraph.ClearAverage();
	}*/
}


