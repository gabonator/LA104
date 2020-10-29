#pragma once
#include "../../os_host/source/framework/Wnd.h"
#include "../../os_host/source/framework/Utils.h"
#include "../../os_host/source/gui/Gui.h"
#include "../../os_host/source/gui/Controls.h"
#include "Bitmap.h"

class CWndMenuBlockIcon : public CWnd
{
public:
	ui16 m_clr;
	const void* m_pImage;

public:
	virtual void Create( const char* pszId, ui16 clr, CRect& rcRect, CWnd *pParent, const void* pImage ) 
	{
		m_pImage = pImage;
		m_clr = clr;
		CWnd::Create( pszId, WsVisible, rcRect, pParent );
	}

	void RemapPalette(ui16* p, ui16 clrFront, ui16 clrBack)
	{
		for (int i=0; i<16; i++)
		{
			ui16& clr = p[i];
			if ( clr == RGB565(000000) )
			{
				// transparent
				clr = RGB565(ff00ff);
				continue;
			}
			int nF = Get565G( clr ); // foreground
			int nS = Get565B( clr ); // shadow
			clr = CUtils::InterpolateColor(clrBack, clrFront, nF);
			clr = CUtils::InterpolateColor(clr, 0, nS);
		}
	}

	virtual void OnPaint() override
	{
		if ( HasFocus() )
		{
			MenuBlockEnabled( m_rcClient, m_clr );
			if ( m_pImage )
			{
				CBitmap bmp;
				bmp.Load( (const ui8*)m_pImage );
				RemapPalette( bmp.m_arrPalette, RGB565(8080e0), RGB565(ffffff) );
				bmp.Blit( m_rcClient.right - bmp.m_width, m_rcClient.bottom - bmp.m_height );
			}
			BIOS::LCD::Print( m_rcClient.left+12, m_rcClient.top, RGB565(000000), RGBTRANS, m_pszId );
		} else {
			MenuBlockDisabled( m_rcClient, m_clr );
			if ( m_pImage )
			{
				CBitmap bmp;
				bmp.Load( (const ui8*)m_pImage );
				RemapPalette( bmp.m_arrPalette, RGB565(e0e0e0), RGB565(b0b0b0) );
				bmp.Blit( m_rcClient.right - bmp.m_width, m_rcClient.bottom - bmp.m_height );
			}
			BIOS::LCD::Print( m_rcClient.left+12, m_rcClient.top, RGB565(000000), RGBTRANS, m_pszId );
		}
	}
	virtual void OnKey(int nKey) override
	{
		// Parent will manage the movement
		GetParent()->OnKey( nKey );
	}

	// Design
        constexpr static int gray = RGB565(b0b0b0);

	static void MenuBlockDisabled(const CRect& rcRect, ui16 clr)
	{
		// shadow
		BIOS::LCD::RoundRect( rcRect.right-4, rcRect.top+4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));
		BIOS::LCD::RoundRect( rcRect.left+4, rcRect.bottom-4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));

		// inside
		BIOS::LCD::RoundRect( rcRect.left, rcRect.top, rcRect.left+12, rcRect.bottom, clr);
		BIOS::LCD::Bar( rcRect.left+8, rcRect.top, rcRect.right-4, rcRect.bottom, gray);
		BIOS::LCD::RoundRect( rcRect.right-8, rcRect.top, rcRect.right, rcRect.bottom, gray);
	}

	static void MenuBlockEnabled(const CRect& rcRect, ui16 clr)
	{
		// shadow
		BIOS::LCD::RoundRect( rcRect.right-4, rcRect.top+4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));
		BIOS::LCD::RoundRect( rcRect.left+4, rcRect.bottom-4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));

		// inside
		BIOS::LCD::RoundRect( rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, clr);
	}

};
