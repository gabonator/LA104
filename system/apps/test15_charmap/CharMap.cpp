#include "CharMap.h"

int nCursor = 0;

char* itoa2(char* buf, ui8 n)
{
	for (int i=0; i<8; i++)
		if (n & (128>>i) )
			buf[i] = '1';
		else
			buf[i] = '0';
	buf[8] = 0;
	return buf;
}

static const char CShapes_sel_right[] =
"\x0e"
".............."
".............."
" ............ "
"  ..........  "
"              "
"              "
"              "
"              ";

uint8_t* GetBiosChar(char ch)
{
  typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
  static Tfunc __Bios = (Tfunc)(0x08000100|1); 
  const int FONTPTR=15;

  static uint8_t ret[14];
  uint8_t* p = (uint8_t*)__Bios(FONTPTR, ch);

  for (int y=0; y<14; y++)
  {
    uint8_t row = 0;
    for (int x=0; x<8; x++)
    {
      uint16_t t = p[0+x*2]+p[1+x*2]*256;
      if (t & (1<<(15-y)))
        row |= 1<<(7-x);
    }
    ret[y] = row;
  }
  return ret;
}

void DrawBig(int _x, int _y, int s, int ch)
{
	CRect rcRect(_x-1, _y-1, _x+8*s+1, _y+14*s+1);
        GUI::Background(rcRect, RGB565(4040b0), RGB565(404040));
	BIOS::LCD::Rectangle(rcRect, RGB565(ffffff));

	ui8* pData;
	if (1)
        {
		pData = (ui8*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::CharRom);
		pData += ch*14;
        } else {
	        pData = GetBiosChar(ch);
        }

	if (!pData)
		return;

        uint16_t clr = RGB565(ffffff);

	for (int y=0; y<14; y++)
	{
		int nBits = ~pData[y];
		for (int x=0; x<8; x++)
		{
		        if (nBits & (128>>x))
				BIOS::LCD::Bar( _x + x*s, _y + y*s, _x + (x+1)*s, _y + (y+1)*s, clr ); 
		}
	}

}

void UpdateChar( int nPos, bool bSel )
{
	char str[2] = { (char)nPos, 0 };
	int bx = nPos % 32;
	int by = nPos / 32;
	int x = 26 + bx*9 + ((bx >= 16) ? 2 : 0);
	int y = 100 + by*16;
	str[0] = nPos ? nPos : ' ';

	if (bSel)
	  BIOS::LCD::Print( x, y, RGB565(ffffff), RGB565(0000b0), str );        
        else
        {
          GUI::Background(CRect(x, y, x+8, y+14), RGB565(404040), RGB565(101010));
	  BIOS::LCD::Print( x, y, RGB565(b0b0b0), RGBTRANS, str );
        }

	if ( bSel )
	{
		DrawBig( 20, 28, 4, nPos );
		char buf[32];
		char binbuf[16];

		sprintf(buf, "Decimal     = %d  ", nPos);
                GUI::Background(CRect(80, 34+16*0, 80+22*8,  34+16*0+14), RGB565(404040), RGB565(101010));
		BIOS::LCD::Print(80, 34+16*0, RGB565(ffffff), RGBTRANS, buf);

		sprintf(buf, "Hexadecimal = 0x%02x", nPos);
                GUI::Background(CRect(80, 34+16*1, 80+22*8,  34+16*1+14), RGB565(404040), RGB565(101010));
		BIOS::LCD::Print(80, 34+16*1, RGB565(ffffff), RGBTRANS, buf);

                sprintf(buf, "Binary      = %s", itoa2(binbuf, nPos));
                GUI::Background(CRect(80, 34+16*2, 80+22*8,  34+16*2+14), RGB565(404040), RGB565(101010));
		BIOS::LCD::Print(80, 34+16*2, RGB565(ffffff), RGBTRANS, buf);
	}
}

void CWndCharMap::Create(CWnd *pParent, ui16 dwFlags)
{
	CWnd::Create("CWndCharMap", dwFlags, CRect(0, 14, 400, 240), pParent);
}

void CWndCharMap::OnPaint()
{
        CRect rc1(0, 0, BIOS::LCD::Width, 14);
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
        int x = 0;
        x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " LA104 Character map"); // TODO:
        x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);

        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));

	char str[2] = {0, 0};

	for ( int i = 0; i < 255; i++ )
	{
		int bx = i%32;
		int by = i/32;
		int x = 26 + bx*9 + ((bx >= 16) ? 2 : 0);
		int y = 100 + by*16;
		str[0] = i ? i : ' ';
		BIOS::LCD::Print( x, y, RGB565(b0b0b0), RGBTRANS, str );
	}

	for ( int i=0; i<256; i+=32 )
	{
		int x = 4;
		int y = 100 + (i/32)*16;

		char buf[32];
		sprintf(buf, "%02x", i);
		BIOS::LCD::Print(x, y, RGB565(00ff00), RGBTRANS, buf);
	}

	UpdateChar( nCursor, HasFocus() ? true : false );
}

void CWndCharMap::OnKey(int nKey)
{
	if ( nKey == BIOS::KEY::Left && nCursor > 0 )
	{
		UpdateChar(nCursor, false);
		nCursor--;
		UpdateChar(nCursor, true);
		return;
	}
	if ( nKey == BIOS::KEY::Right && nCursor < 255 )
	{
		UpdateChar(nCursor, false);
		nCursor++;
		UpdateChar(nCursor, true);
		return;
	}
	if ( nKey == BIOS::KEY::Up && nCursor >= 32 )
	{
		UpdateChar(nCursor, false);
		nCursor-=32;
		UpdateChar(nCursor, true);
		return;
	}
	if ( nKey == BIOS::KEY::Down && nCursor <= 255-32 )
	{
		UpdateChar(nCursor, false);
		nCursor+=32;
		UpdateChar(nCursor, true);
		return;
	}
}

void CWndCharMap::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		return;
	}

	if (code == ToWord('L', 'E') )
	{
		return;
	}
}
