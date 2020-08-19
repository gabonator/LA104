#include "ToolBar.h"
#include "MainWnd.h"

/*static*/ const CWndToolBar::CBarItem* CWndToolBar::GetMenuItems()
{
	static const CBarItem arrMenuItems[] = {

		{ CBarItem::IMain,	(PSTR)"Oscilloscope", nullptr},
		{ CBarItem::ISub,	(PSTR)"Input", nullptr},
		{ CBarItem::ISub,	(PSTR)"Cursor", nullptr},
		{ CBarItem::ISub,	(PSTR)"Meas", nullptr},
		{ CBarItem::ISub,	(PSTR)"Math", nullptr},
		{ CBarItem::ISub,	(PSTR)"Disp", nullptr},
		{ CBarItem::ISub,	(PSTR)"Mask", nullptr},

		{ CBarItem::IMain,	(PSTR)"Spectrum", nullptr},
		{ CBarItem::ISub,	(PSTR)"FFT", nullptr},
		{ CBarItem::ISub,	(PSTR)"Marker", nullptr},

		{ CBarItem::IMain,	(PSTR)"Generator", nullptr},
		{ CBarItem::ISub,	(PSTR)"Wave", nullptr},
		{ CBarItem::ISub,	(PSTR)"Edit", nullptr},
//		{ CBarItem::ISub,	(PSTR)"Modulation", &MainWnd.m_wndMenuGeneratorMod},
		
		{ CBarItem::IMain,	(PSTR)"Settings", nullptr},
		{ CBarItem::ISub,	(PSTR)"Main", nullptr},
		{ CBarItem::ISub,	(PSTR)"Keys", nullptr},
		{ CBarItem::ISub,	(PSTR)"Calib", nullptr},
		
		{ CBarItem::IMain,	(PSTR)"About", nullptr},
		{ CBarItem::ISub,	(PSTR)"Device", nullptr},
		{ CBarItem::ISub,	(PSTR)"Firmware", nullptr},
		{ CBarItem::ISub,	(PSTR)"Modules", nullptr},
		{ CBarItem::ISub,	(PSTR)"Status", nullptr},


		{ CBarItem::IMain,	(PSTR)"User app", nullptr},
		{ CBarItem::ISub,	(PSTR)"File manager", nullptr},

		{ CBarItem::IEnd,		NULL, NULL }
	};

	return arrMenuItems;
}

/*virtual*/ void CWndToolBar::OnPaint()
{
	const CWndToolBar::CBarItem* pItems = GetMenuItems();
	int nFocus = m_nFocus;

	// find nearest sub menu
	int nMenu = nFocus;
	while ( pItems[nMenu].m_eType == CBarItem::ISub && nMenu > 0 )
		nMenu--;

	_ASSERT( pItems[nMenu].m_eType == CBarItem::IMain );

	int x = m_rcClient.left;
	int nExtentx = (strlen(pItems[nMenu].m_pName) << 3);
	
	if ( nFocus == nMenu && HasFocus() )
	{
		x += BIOS::LCD::Draw(x, 0, RGB565(ffffff), RGB565(000000), CShapes::corner_left);
		BIOS::LCD::Bar( x, m_rcClient.top, x + nExtentx, m_rcClient.bottom, RGB565(FFFFFF));	
		x += BIOS::LCD::Print(x, 0, RGB565(000000), RGBTRANS, pItems[nMenu].m_pName);
	} else {
		x += BIOS::LCD::Draw(x, 0, RGB565(0020ff), RGB565(000000), CShapes::corner_left);
		BIOS::LCD::Bar( x, m_rcClient.top, x + nExtentx, m_rcClient.bottom, RGB565(0020ff));
		x += BIOS::LCD::Print(x, 0, RGB565(ffffff), RGBTRANS, pItems[nMenu].m_pName);
	}

	CRect rcSub( m_rcClient );
	rcSub.left = x;

	// gradient background
	CDesign::GradientTop( rcSub );

	if ( nFocus == nMenu && HasFocus() )
		x += BIOS::LCD::Draw(x, 0, RGB565(ffffff), RGBTRANS, CShapes::corner_right);
	else
		x += BIOS::LCD::Draw(x, 0, RGB565(0020ff), RGBTRANS, CShapes::corner_right);
	x += 10;

	int nIgnoreFirst = 1; // 1 -> first sub menu
	
	// calculate how many items we need to hide from left to reach the selected one
	int nRequired = 0;
	int nAvailable = BIOS::LCD::Width - 16 - x; // 16px reserved for arrows
	for ( int i = nFocus; i > 0 && pItems[i].m_eType != CBarItem::IMain; i-- )
	{
		nRequired += strlen(pItems[i].m_pName)*8 + 16;
		if ( nRequired > nAvailable )
			nIgnoreFirst++;
	}

	if ( nIgnoreFirst > 1 )
		x += BIOS::LCD::Print( x, m_rcClient.top, RGB565(b0b0b0), RGBTRANS, "\x11");

	for ( int i = nMenu+nIgnoreFirst; pItems[i].m_eType == CBarItem::ISub; i++ )
	{
		ui8 bSelected = (i==nFocus);
		u16 clr = bSelected ? clrSelected : clrNormal;
		u16 bgr = bSelected ? 0 : RGBTRANS;

		if ( HasFocus() && bSelected )
		{
			clr = clrSelectedFocus;
			bgr = bgrSelectedFocus;
		}

		if ( x + 16 + strlen(pItems[i].m_pName)*8 >= BIOS::LCD::Width )
		{
			x += BIOS::LCD::Print( x, m_rcClient.top, RGB565(b0b0b0), RGBTRANS, "\x10");
			break;
		}
		
		if ( bSelected )
			x += BIOS::LCD::Draw( x, m_rcClient.top, bgr, RGBTRANS, CShapes::corner_left);
		else
			x += 8;
		
		if ( bSelected )	                                           
			BIOS::LCD::Bar( x, m_rcClient.top, x + (strlen(pItems[i].m_pName)<<3), m_rcClient.bottom, bgr);

		x += BIOS::LCD::Print( x, m_rcClient.top, clr, bgr, pItems[i].m_pName);

		if ( bSelected )
			x += BIOS::LCD::Draw( x, m_rcClient.top, bgr, RGBTRANS, CShapes::corner_right);
		else
			x += 8;
	}
}

/*virtual*/ void CWndToolBar::OnKey(int nKey)
{
	const CWndToolBar::CBarItem* pItems = GetMenuItems();
	ui8 oldFocus = m_nFocus;
	if ( nKey == BIOS::KEY::Left)
	{
		if ( pItems[m_nFocus].m_eType == CBarItem::IMain ) 
		{
			m_nFocus++;
			while((pItems[m_nFocus].m_eType != CBarItem::IMain)&&(pItems[m_nFocus].m_eType != CBarItem::IEnd))
			{	// Find the last item
				m_nFocus++;
			}
			m_nFocus--;
		}
		else 
		{
			m_nFocus--;
		}
		CWndToolBar::ChangeFocus(oldFocus);
	}

	if ( nKey == BIOS::KEY::Right)
	{
		if ((pItems[m_nFocus+1].m_eType == CBarItem::IMain)||((pItems[m_nFocus+1].m_eType == CBarItem::IEnd)))
		{
			m_nFocus--;
			while(pItems[m_nFocus].m_eType != CBarItem::IMain)
			{	// Find the first item
				m_nFocus--;
			}
		}
		else
		{
			m_nFocus++;
		}
		CWndToolBar::ChangeFocus(oldFocus);
	}

	if( nKey == BIOS::KEY::Enter )
	{
		CWnd::OnKey( BIOS::KEY::Down );
	}

	if( nKey == BIOS::KEY::Escape )
	{	// Focus on first item
		if (pItems[m_nFocus].m_eType != CBarItem::IMain)
		{
			m_nFocus--;
			while(pItems[m_nFocus].m_eType != CBarItem::IMain)
			{	// Find the first item
				m_nFocus--;
			}
			CWndToolBar::ChangeFocus(oldFocus);
		}
	}
	CWnd::OnKey( nKey );
}

/*virtual*/ void CWndToolBar::ChangeFocus(ui8 oldFocus)
{
	const CWndToolBar::CBarItem* pItems = GetMenuItems();
	SendMessage( GetParent(), ToWord('L', 'D'), (NATIVEPTR)pItems[oldFocus].m_pWndMenu );
	SendMessage( GetParent(), ToWord('L', 'E'), (NATIVEPTR)pItems[m_nFocus].m_pWndMenu );
	SendMessage( GetParent(), ToWord('L', 'R'), 0 );
//TODO:GABO:	Settings.Runtime.m_nMenuItem = m_nFocus;
Invalidate();
}

/*virtual*/ void CWndToolBar::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	if ( code == ToWord('g', 'i') )
	{
		const CBarItem *pItems = GetMenuItems();
		m_nFocus = (ui8)data;
		CWnd* pFocus = GetFocus();
		SendMessage( GetParent(), ToWord('L', 'E'), (NATIVEPTR)pItems[m_nFocus].m_pWndMenu );
		if ( GetFocus() == pFocus )
		{
			// else the focus was stolen by the current wnd
			SetFocus();
		}
	}

	if ( code == ToWord('g', 'o') )
	{
		const char* strId = (const char*)data;
		int nItem = Find( strId );
		
		code = ToWord('g', '2');
		data = nItem;
	}

	if ( code == ToWord('g', '2') )
	{
		const CBarItem *pItems = GetMenuItems();

		int nItem = data;
		if ( nItem == -1 )
			return;

		SendMessage( GetParent(), ToWord('L', 'D'), (NATIVEPTR)pItems[m_nFocus].m_pWndMenu );

		if ( pItems[nItem].m_eType == CBarItem::IMain ) 
			m_nFocus = nItem+1;
		else
			m_nFocus = nItem;

		SendMessage( GetParent(), ToWord('L', 'E'), (NATIVEPTR)pItems[m_nFocus].m_pWndMenu );
		SetFocus();
		SendMessage( GetParent(), ToWord('L', 'R'), 0 );
//TODO:GABO:		Settings.Runtime.m_nMenuItem = m_nFocus;
		Invalidate();
	}
}

CWnd* CWndToolBar::GetCurrentLayout()
{
	const CWndToolBar::CBarItem* pItems = GetMenuItems();
	return (CWnd*)pItems[m_nFocus].m_pWndMenu;
}

int CWndToolBar::Find(const char* strId)
{	
	const CBarItem *pItems = GetMenuItems();

	for (int i=0; pItems[i].m_eType != CBarItem::IEnd; i++ )
	{
		if ( /*pItems[i].m_eType == CBarItem::IMain &&*/ 
				strcmp(pItems[i].m_pName, strId) == 0 )
		{
			return i;
		}
	}
	_ASSERT( 0 );
	return -1;
}
