#pragma once
#include "../framework/Types.h"
#include "Gui.h"

class CDesignPalette
{
public:
#ifdef LA104
    constexpr static int gray = RGB565(505050);
#endif
#ifdef DS203
	constexpr static int gray = RGB565(b0b0b0);
#endif
#ifdef DS213
	constexpr static int gray = RGB565(b0b0b0);
#endif
};

// TODO: Remove and merge with Gui!
class CDesignNice
{
public:
	enum {
		MenuItemIndent = 4
	};

public:
	static void MenuItemDisabled(const CRect& rcRect, ui16 clr)
	{
		BIOS::LCD::RoundRect( rcRect.left, rcRect.top, rcRect.left+12, rcRect.bottom, clr);
        BIOS::LCD::Bar( rcRect.left+8, rcRect.top, rcRect.right, rcRect.bottom, CDesignPalette::gray);
	}

	static void MenuItemEnabled(const CRect& rcRect, ui16 clr)
	{
		BIOS::LCD::RoundRect( rcRect.left-2, rcRect.top, rcRect.right, rcRect.bottom, clr);
		BIOS::LCD::RoundRect( rcRect.left+8-2, rcRect.top+1, rcRect.right-1, rcRect.bottom-1, RGB565(ffffff));
	}

	static void MenuItemClearDisabled(const CRect& rcRect)
	{
        GUI::Background(rcRect, RGB565(101010), RGB565(404040));
//		BIOS::LCD::Bar(rcRect.left, rcRect.top, rcRect.left + MenuItemIndent + 4, rcRect.bottom, RGB565(000000));
	}

	static void MenuItemClearEnabled(const CRect& rcRect)
	{
        GUI::Background(rcRect, RGB565(101010), RGB565(404040));
		//BIOS::LCD::Bar(rcRect.left, rcRect.top, rcRect.left + MenuItemIndent + 4, rcRect.bottom, RGB565(000000));
		//BIOS::LCD::Bar(rcRect.right-4, rcRect.top, rcRect.right, rcRect.bottom, RGB565(000000));
	}

	static void MenuBlockDisabled(const CRect& rcRect, ui16 clr)
	{
		// shadow
		BIOS::LCD::RoundRect( rcRect.right-4, rcRect.top+4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));
		BIOS::LCD::RoundRect( rcRect.left+4, rcRect.bottom-4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));

		// inside
		BIOS::LCD::RoundRect( rcRect.left, rcRect.top, rcRect.left+12, rcRect.bottom, clr);
		BIOS::LCD::Bar( rcRect.left+8, rcRect.top, rcRect.right-4, rcRect.bottom, CDesignPalette::gray);
		BIOS::LCD::RoundRect( rcRect.right-8, rcRect.top, rcRect.right, rcRect.bottom, CDesignPalette::gray);
	}

	static void MenuBlockEnabled(const CRect& rcRect, ui16 clr)
	{
		// shadow
		BIOS::LCD::RoundRect( rcRect.right-4, rcRect.top+4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));
		BIOS::LCD::RoundRect( rcRect.left+4, rcRect.bottom-4, rcRect.right+4, rcRect.bottom+4, RGB565(000070));

		// inside
		BIOS::LCD::RoundRect( rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, clr);
	}

	static void GradientTop(const CRect& rcRect)
	{
		ui16 pattern[16];
		_ASSERT( rcRect.Height() == COUNT(pattern) );

		ui16 clradd = RGB565(00000b);
		ui16 clr = RGB565(00204f);
		for (int i=0; i<16; i++)
		{
			clr += clradd;
			pattern[15-i] = clr;
		}
		BIOS::LCD::BufferBegin( rcRect );
		for (int y=0; y<rcRect.Width(); y++)
		  BIOS::LCD::BufferWrite(pattern, 16);
		BIOS::LCD::BufferEnd();
//		BIOS::LCD::Pattern(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, pattern, 16);
		
/*
		// TODO: Optimize!!! STACK OVERFLOW!!!!!!!!!
		uint16_t row[BIOS::LCD::Width];
		BIOS::LCD::BufferBegin( rcRect );
		for (int i=0; i<16; i++)
		{
			clr += clradd;
			for (int x=0; x<rcRect.Width(); x++)
				row[x] = clr;
			BIOS::LCD::BufferWrite(row, rcRect.Width());

		}
		BIOS::LCD::BufferEnd();
*/
	}

	static void Window(const CRect& rcRect, ui16 clr)
	{
		CRect rcClient = rcRect;
		BIOS::LCD::RoundRect(rcClient, RGB565(000000));
		rcClient.Deflate(2, 2, 2, 2);
		BIOS::LCD::RoundRect(rcClient, clr);
		rcClient.Deflate(2, 14, 2, 2);
		BIOS::LCD::RoundRect(rcClient, RGB565(b0b0b0));
	}

	static void WindowSelector(const CRect& rcRect, ui16 clr)
	{
		CRect rcClient = rcRect;
		BIOS::LCD::RoundRect(rcClient, RGB565(000000));
		rcClient.Deflate(2, 2, 2, 2);
		BIOS::LCD::RoundRect(rcClient, clr);
		rcClient.Deflate(12, 14, 12, 2);
		BIOS::LCD::RoundRect(rcClient, RGB565(b0b0b0));
	}

	static void ListItemDisabled(const CRect& rcRect)
	{
		BIOS::LCD::Bar(rcRect, RGB565(b0b0b0));
	}

	static void ListItemEnabled(const CRect& rcRect)
	{
		BIOS::LCD::BufferBegin(rcRect);
		ui8 c = 0xd0;
		for (int y=0; y<rcRect.Height(); y++)
		{
			ui16 clr = RGB565RGB(c, c, c);
			for (int x=rcRect.Width(); x--; )
				BIOS::LCD::BufferWrite(clr);
			c -= 0x0d;
		}
		BIOS::LCD::BufferEnd();
	}

	static void Shadow(const CRect& rcRect, ui32 clr)
	{
		BIOS::LCD::Shadow( rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, clr);
	}

};

class CDesignFast
{
public:
	static void MenuItemDisabled(const CRect& rcRect, ui16 clr)
	{
		BIOS::LCD::Bar( rcRect.left, rcRect.top, rcRect.left+8, rcRect.bottom, clr );
		BIOS::LCD::Bar( rcRect.left+8, rcRect.top, rcRect.right, rcRect.bottom, RGB565(b0b0b0) );
	}

	static void MenuItemEnabled(const CRect& rcRect, ui16 clr)
	{
		BIOS::LCD::Bar( rcRect.left-2, rcRect.top, rcRect.left+8-1, rcRect.bottom, clr );
		BIOS::LCD::Bar( rcRect.left+8-2, rcRect.top, rcRect.right, rcRect.bottom, RGB565(ffffff) );
	}

	static void GradientTop(const CRect& rcRect)
	{
		BIOS::LCD::Bar( rcRect, RGB565(0000B0) );
	}

	static void Window(const CRect& rcRect, ui16 clr)
	{
		BIOS::LCD::Bar( rcRect.left, rcRect.top, rcRect.right, rcRect.top+16, clr );
		BIOS::LCD::Bar( rcRect.left, rcRect.top+16, rcRect.right, rcRect.bottom, CDesignPalette::gray );
	}

	static void WindowSelector(const CRect& rcRect, ui16 clr)
	{
		BIOS::LCD::Bar( rcRect.left, rcRect.top, rcRect.right, rcRect.top+16, clr );
		BIOS::LCD::Bar( rcRect.left, rcRect.top+16, rcRect.right, rcRect.bottom, CDesignPalette::gray );
	}

	static void ListItemDisabled(const CRect& rcRect)
	{
		BIOS::LCD::Bar(rcRect, CDesignPalette::gray);
	}

	static void ListItemEnabled(const CRect& rcRect)
	{
		BIOS::LCD::Bar(rcRect, RGB565(404040));
	}

	static void Shadow(const CRect& rcRect, ui32 clr)
	{
		BIOS::LCD::Shadow( rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 0xffffff80);
	}
};

// CDesignNice - nice round shaped rectangles, but slow
// CDesignFast - fast, only solid rectangles

class CDesign : public CDesignNice
//class CDesign : public CDesignFast
{
};

