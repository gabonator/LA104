#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <library.h>
#include "Utils.h"
#include "Wnd.h"

//			BIOS::LCD::LcdWidth = 400,
//			BIOS::LCD::LcdHeight = 240,

/*
  400 x 224  
  sprite 16x16
	map 25x14
*/

//class CSnakeResources

static const ui32 sprites[] = {
  // wall
	0x2ccccccc, 0xccccccc2,
	0xcccccccc, 0xcccccccc,
	0xccc44888, 0x44888ccc,
	0xcc448884, 0x888844cc,
	0xcc488844, 0x888448cc,
	0xcc888448, 0x884488cc,
	0xcc884488, 0x844888cc,
	0xcc844888, 0x448884cc,
	0xcc448884, 0x488844cc,
	0xcc488844, 0x888448cc,
	0xcc888448, 0x884488cc,
	0xcc884488, 0x844888cc,
	0xcc844888, 0x448884cc,
	0xccc48884, 0x48884ccc,
	0xcccccccc, 0xcccccccc,
	0x2ccccccc, 0xccccccc2,

  // ground
	0x22222222, 0x22222222,
	0x22232222, 0x22222232,
	0x22232222, 0x22222232,
	0x22222222, 0x22222222,
	0x22222222, 0x22222222,
	0x22222322, 0x22222222,
	0x22222322, 0x22232222,
	0x22222222, 0x22232222,
	0x22222223, 0x22222222,
	0x22322223, 0x22222222,
	0x22322222, 0x22222222,
	0x22222222, 0x22222222,
	0x22222222, 0x22223222,
	0x32222222, 0x22223222,
	0x32222222, 0x22222222,
	0x22222222, 0x22222222,

  // diamod
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x0000000b, 0x00000000,
	0x000000bb, 0xb0000000,
	0x00000bbf, 0xbb000000,
	0x0000bbfb, 0xbbb00000,
	0x000bbfbb, 0xbbbb0000,
	0x0bbbfbbb, 0xbbbbb000,
	0x00bbbbbb, 0xbbbb9000,
	0x000bbbbb, 0xbbb90000,
	0x0000bbbb, 0xbb900000,
	0x00000bbb, 0xb9000000,
	0x000000bb, 0x90000000,
	0x00000009, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,

  // body
	0x22229999, 0x99992222,
	0x22999999, 0x99999922,
	0x29999999, 0x99999992,
	0x29999999, 0x99999992,
	0x99999111, 0x11199999,
	0x99991111, 0x11119999,
	0x99991111, 0x11119999,
	0x99991111, 0x11119999,
	0x99991111, 0x11119999,
	0x99991111, 0x11119999,
	0x99991111, 0x11119999,
	0x99999111, 0x11199919,
	0x29999999, 0x99999912,
	0x29999999, 0x99999192,
	0x22999999, 0x91111922,
	0x22229999, 0x99992222,

  // tail
	0x00000000, 0x00000000,
	0x00000000, 0x00090000,
	0x00000000, 0x00999000,
	0x00000000, 0x09999000,
	0x00000000, 0x99119900,
	0x00000009, 0x91111990,
	0x00000099, 0x11111199,
	0x00000991, 0x11111199,
	0x00000991, 0x11111990,
	0x00000099, 0x11111990,
	0x00000009, 0x91119900,
	0x00000000, 0x99119900,
	0x00000000, 0x09999000,
	0x00000000, 0x00999000,
	0x00000000, 0x00090000,
	0x00000000, 0x00000000,

	  // head
	0x22999999, 0x99999922,
	0x29999999, 0x99999992,
	0x99911111, 0x11111999,
	0x99111111, 0x11111199,
	0x99118888, 0x8ff11199,
	0x99118888, 0xffff1199,
	0x99118888, 0x11111199,
	0x99118881, 0x99999999,
	0x99118888, 0x11111111,
	0x99118888, 0x88881119,
	0x99118888, 0x88881199,
	0x99111888, 0x88811199,
	0x99111111, 0x11111199,
	0x99911111, 0x11111999,
	0x29999999, 0x99999992,
	0x22999999, 0x99999922,

/*
  // blank
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
*/
};


static const ui16 palette[16] = 
	{ RGB565(000000), RGB565(0000b0), RGB565(00b000), RGB565(00b0b0), RGB565(b00000), RGB565(b000b0), RGB565(b0b000), RGB565(b0b0b0),
	  RGB565(404040), RGB565(0000ff), RGB565(00ff00), RGB565(00ffff), RGB565(ff0000), RGB565(ff00ff), RGB565(ffff00), RGB565(ffffff)};

static const ui8 levels[] = {
 	"#########################"
	"#                   #####"
	"#                   #####"
	"#                   #####"
	"#     ##########    #####"
	"#     #        #    #####"
	"#     #        #    #####"
	"#     #        #    #####"
	"#     #        #    #####"
	"#     #        #    #####"
	"#                   #####"
	"#                   #####"
	"#                   #####"
	"#########################"};


/*
	// blank
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
	"                         ",
*/

class CWndSnake : public CWnd
{
public:
	enum {
		eWall = 0,
		eGround = 1,
		eDiamond = 2,
		eBody = 3,
		eTail = 4,
		eHead = 5,

		eDirRight = 0,
		eDirDown = 1,
		eDirLeft = 2,
		eDirUp = 3
	};

	class CMapPos
	{
	public:
		CMapPos()
		{
		};
		CMapPos(ui8 _nX, ui8 _nY, ui8 _nD) :
			nX( _nX ), nY( _nY ), nDir( _nD )
		{
		}

		ui8 nX;
		ui8 nY;
		ui8 nDir;
	};

public:
	bool m_bReset;
	int m_nX, m_nY;
	int m_nDir;
	int m_nTailDir;
	int m_nLength;
	int m_nScore;
	ui8 m_arrLevel[14*25/2];

	CMapPos m_arrBodyPos_[30];
	CArray<CMapPos> m_arrBodyPos;

        bool IsRunning()
        {
          return m_bReset;
        }	

	void SetLevel(int nLevel)
	{
		const ui8* pLevelSrc = &levels[nLevel*14*25];

		for (int y=0; y<14; y++)
			for (int x=0; x<25; x++)
			{
				switch ( *pLevelSrc++ )
				{
					case ' ': SetBlock(x, y, eGround); break;
					case '#': SetBlock(x, y, eWall); break;
					default:
						_ASSERT(0);
				}
			}
	}

	CMapPos SetDiamond()
	{
		int x, y;
		do 
		{
			x = CUtils::Random() % 25;
			y = CUtils::Random() % 14;
		} while ( GetBlock(x, y) != eGround );
		SetBlock(x, y, eDiamond);

		return CMapPos( x, y, eDirRight );
	}

	void SetRandomPos()
	{
		m_nDir = eDirRight;
		do 
		{
			m_nX = CUtils::Random() % 25;
			m_nY = CUtils::Random() % 14;
		} while ( GetBlock(m_nX, m_nY) != eGround );
		SetBlock(m_nX, m_nY, eHead);
	}

	void DrawMap()
	{
		for (int y=0; y<14; y++)
			for (int x=0; x<25; x++)
				DrawBlock(x, y);
	}

	void DrawBlock(int x, int y)
	{
		ui16 pSprite[256];
		ui8 blk = GetBlock(x, y);

		if ( blk == eHead )
			PrepareSprite( blk, pSprite, m_nDir );
		else
		if ( blk == eTail )
			PrepareSprite( blk, pSprite, m_nTailDir );
		else
			PrepareSprite( blk, pSprite, eDirRight );

		BIOS::LCD::Pattern( x<<4, (y+1)<<4, (x+1)<<4, (y+2)<<4, pSprite, 16*16 );
	}

	ui8 GetBlock(int x, int y)
	{
		ui8 blk = m_arrLevel[x+(y/2)*25];
		return ( y & 1 ) ? (blk >> 4) : blk & 0x0f;
	}

	void SetBlock(int x, int y, ui8 d)
	{
		ui8& blk = m_arrLevel[x+(y/2)*25];
		if ( y & 1 )
			blk = (blk & 0x0f) | (d<<4);
		else
			blk = (blk & 0xf0) | (d);
	}

	void PrepareSprite(int nType, ui16* pSprite, int nTransform)
	{
		for (int _x=0; _x<16; _x++)
			for (int _y=0; _y<16; _y++)
			{
				int x=0, y=0;
				switch (nTransform)
				{
					case eDirRight: x=15-_y; y=_x; break;
					case eDirDown: x=15-_x; y=_y; break;
					case eDirLeft: x=_y; y=15-_x; break;
					case eDirUp: x=_x; y=_y; break;
				}

				ui16& pixel = pSprite[x|(y<<4)];
				int color = sprites[(nType<<5)|(_y<<1)|(_x>>3)];
				color >>= (7-(_x&7)) << 2;
				color &= 0x0f;
				if ( color == 0 )
					color = 2; // transparent

				pixel = palette[color];
			}
	}

	/*virtual*/ void OnKey(ui16 nKey)
	{
		if ( nKey == BIOS::KEY::Left )
		{
			if ( ++m_nDir > eDirUp )
				m_nDir = eDirRight;
		} else
		if ( nKey == BIOS::KEY::Right )
		{
			if ( m_nDir > 0 )
				m_nDir--;
			else
				m_nDir = eDirUp;
		} else
		{
			CWnd::OnKey( nKey );
			return;
		}
		DrawBlock( m_nX, m_nY );
	}


	CWndSnake()
	{
		m_bReset = false;
		m_arrBodyPos.Init( m_arrBodyPos_, COUNT( m_arrBodyPos_ ) );
	}
	
	virtual void Create(CWnd *pParent, ui16 dwFlags)
	{
		CWnd::Create("CWndSnake", dwFlags, CRect(0, 16, 400, 240), pParent);
	}

	virtual void OnPaint()
	{
		if ( m_bReset )
		{
			SetLevel(0);
			SetRandomPos();
			SetDiamond();
			SetDiamond();
			SetDiamond();
			SetDiamond();

			m_nLength = 3;
			m_nScore = 0;
			m_arrBodyPos.RemoveAll();
			UpdateTimer();
			m_bReset = false;
		}
		DrawMap();
	}

	void UpdateTimer()
	{
		KillTimer();
		int nInterval = 300-m_nScore*10;
		if ( nInterval < 100 )
			nInterval = 100;
		SetTimer( nInterval );
	}

	virtual void OnTimer()
	{
		static si8 dx[4] = {1,0,-1,0};
		static si8 dy[4] = {0,1,0,-1};

		if ( GetFocus() != this )
			return;

		SetBlock( m_nX, m_nY, eBody );
		DrawBlock( m_nX, m_nY );

		if ( m_arrBodyPos.GetSize() == 0 )
			m_arrBodyPos.Add( CMapPos( m_nX, m_nY, m_nDir ) );

		m_nX += dx[m_nDir];
		m_nY += dy[m_nDir];

		ui8 blk = GetBlock( m_nX, m_nY );
		m_arrBodyPos.Add( CMapPos( m_nX, m_nY, m_nDir ) );

		if ( blk == eDiamond )
		{
			if ( m_nLength < (int)COUNT( m_arrBodyPos_ )-1 )
				m_nLength++;

			CMapPos posDiamond = SetDiamond();
			DrawBlock( posDiamond.nX, posDiamond.nY );
			BIOS::SYS::Beep(5);
			UpdateTimer();
		} else
		if ( blk == eTail )
		{
		} else
		if ( blk == eGround )
		{
		} else
		{
			//MainWnd.m_wndMessage.Show(&MainWnd, "Snake", "Game Over!", RGB565(FFFF00));
//BIOS::DBG::Print("Snake dies!");
			BIOS::SYS::Beep(200);
			KillTimer();
			m_bReset = true;
OnPaint(); // force restart
			return;
		}

		SetBlock( m_nX, m_nY, eHead );
		DrawBlock( m_nX, m_nY );
		if ( m_arrBodyPos.GetSize() > m_nLength )
		{
			CMapPos& posTail = m_arrBodyPos[0];
			SetBlock( posTail.nX, posTail.nY, eGround );
			DrawBlock( posTail.nX, posTail.nY );
			m_arrBodyPos.RemoveAt( 0 );
		}
		if ( m_arrBodyPos.GetSize() > m_nLength-1 )
		{
			CMapPos& posTail = m_arrBodyPos[0];
			m_nTailDir = m_arrBodyPos[1].nDir;
			SetBlock( posTail.nX, posTail.nY, eTail );
			DrawBlock( posTail.nX, posTail.nY );	
		}
	}

	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data)
	{
		// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
		if (code == ToWord('L', 'D') )
		{
			KillTimer();
			return;
		}

		if (code == ToWord('L', 'E') )
		{
			m_bReset = true;
			return;
		}
	}
};

#endif