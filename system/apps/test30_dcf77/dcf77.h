#include "../../os_host/source/framework/Wnd.h"

class CWndDcf77 : public CWnd
{
	int m_nLastChange;
	bool m_bPrevSignal;
	int m_nImpulseLen;

	char m_cBit;
	bool m_bError;
	bool m_bSync;
	bool m_bWasError;

	int m_nBitIndex;
	bool m_bRedraw;

	ui32 m_DcfBits[2];

	ui32 m_DcfValid[2];
	int m_lDcfAcquired;

public:
	virtual void Create(CWnd *pParent, ui16 dwFlags)
	{
		CWnd::Create("CWndDcf77", dwFlags | CWnd::WsNoActivate | CWnd::WsListener, CRect(0, 16, 400, 240), pParent);
	}

	void Clear()
	{
		m_DcfBits[0] = 0;
		m_DcfBits[1] = 0;
		m_lDcfAcquired = 0;
	}

	int GetBit(int nBitIndex, ui32 *pData = NULL)
	{	
		if ( pData == NULL ) 
			pData = m_DcfBits;

		int nIndex = nBitIndex/32;
		int nMask = (1<<(nBitIndex & 31));
		             
		if ( pData[nIndex] & nMask )
			return 1;
		return 0;
	}

	int GetBits(int nIndex, int nLen, ui32 *pData = NULL)
	{
		if ( pData == NULL ) 
			pData = m_DcfBits;

		int nMul = 1;
		int nResult = 0;
		for ( int i=nIndex; i<nIndex+nLen; i++, nMul<<=1)
			if ( GetBit(i, pData) )
				nResult |= nMul;
		return nResult;
	}

	int GetParity(int nIndex, int nLen, ui32 *pData = NULL)
	{
		if ( pData == NULL ) 
			pData = m_DcfBits;

		int nResult = 0;
		for ( int i=nIndex; i<nIndex+nLen; i++)
			if ( GetBit(i, pData) )
				nResult ^= 1;
		return nResult;
	}

	int BcdToInt(int nBcd )
	{
		return (nBcd & 15) + (nBcd/16) * 10;
	}

	const char* GetWeekday( int nDay )
	{
		const char* arrDays[] = {"?", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
		if ( nDay >= 0 && nDay < (int)COUNT(arrDays) )
			return arrDays[nDay];
		return "?";
	}

	const char* GetMonth( int nMonth )
	{
		const char* arrMonths[] = {"?", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		if ( nMonth >= 0 && nMonth < (int)COUNT(arrMonths) )
			return arrMonths[nMonth];
		return "?";
	}

	bool IsDcfValid()
	{
		if ( GetBit(15) != 0 || GetBit(20) != 1 )
			return false;
		if ( GetBit(28) != GetParity(21, 7) )
			return false;
		if ( GetBit(35) != GetParity(29, 6) )
			return false;
		if ( GetBit(58) != GetParity(36, 57-36+1) )
			return false;
		return true;
	}

	virtual void OnTimer() override
	{
		if ( HasOverlay() )
			return;

//		int nTick = BIOS::SYS::GetTick();

		if ( m_bRedraw )
		{
			static int nMinutes, nHours, nDay, nWDay, nMonth, nYear;
			struct TDcfItem {
				const char* strName;
				int nStart;
				int nLength;
				int* pTarget;
				enum ETYPE {
					EAlwaysZero,
					EAlwaysOne,
					EBits,
					EBoolean,
					ENumber,
					EYear,
					EParity,
					EParity2
				} eType;
			} DcfData[] = {
				// id, startbit index, length, valid value, type
				{"reserved",		1,	14,		NULL, 			TDcfItem::EBits},
				{"antenna",			15,	1,		NULL, 			TDcfItem::EAlwaysZero},
				{"summer ann.",		16,	1,		NULL, 			TDcfItem::EBoolean}, // summer time announcement, set 1 hour before change
				{"summer time",		17, 1,		NULL, 			TDcfItem::EBoolean},
				{"winter time",		18, 1,		NULL, 			TDcfItem::EBoolean},
				{"leap sec ann.",	19, 1,		NULL, 			TDcfItem::EBoolean},
				{"startbit",		20, 1,		NULL, 			TDcfItem::EAlwaysOne},
				{"minutes",			21, 7,		&nMinutes, 		TDcfItem::ENumber},
				{"parity",			28, 1,		NULL, 			TDcfItem::EParity},
				{"hours",			29, 6,		&nHours,		TDcfItem::ENumber},
				{"parity",			35, 1,		NULL, 			TDcfItem::EParity},
				{"day",				36, 6,		&nDay, 			TDcfItem::ENumber},
				{"week day",		42, 3,		&nWDay, 		TDcfItem::ENumber},
				{"month",			45, 5,		&nMonth, 		TDcfItem::ENumber},
				{"year",			50, 8,		&nYear, 		TDcfItem::EYear},
				{"parity",			58, 1,		NULL, 			TDcfItem::EParity2}
			};
			 
			int nIndex = 0;
			for ( int i=0; i<(int)COUNT(DcfData); i++ )
			{
				TDcfItem& DcfItem = DcfData[i];
				int x = (nIndex&1) ? 170 : 8;
				int y = 30 + (nIndex/2)*16;
				if ( DcfItem.eType == TDcfItem::EBits )
					nIndex += 2;
				else
					nIndex++;

				bool bCurrent = (m_nBitIndex-1) >= DcfItem.nStart && (m_nBitIndex-1) < DcfItem.nStart + DcfItem.nLength;
				ui16 clrBack = bCurrent ? RGB565(0000b0) : RGB565(000000);	
				
				BIOS::LCD::Print( x, y, RGB565(ffffff), clrBack, DcfItem.strName); 

				x += 100;
				int nCurBit = GetBit(DcfItem.nStart);
				switch ( DcfItem.eType )
				{
					case TDcfItem::EAlwaysZero:
						BIOS::LCD::Print( x+8, y, !nCurBit ? RGB565(ffffff) : RGB565(ff0000), clrBack, itoa(nCurBit) ); 
						break;
					case TDcfItem::EAlwaysOne:
						BIOS::LCD::Print( x+8, y, nCurBit ? RGB565(ffffff) : RGB565(ff0000), clrBack, itoa(nCurBit) ); 
						break;
					case TDcfItem::EBoolean:
						BIOS::LCD::Print( x+8, y, RGB565(ffffff), clrBack, itoa(nCurBit) ); 
						break;
					case TDcfItem::EParity:
						{
							int nCheck = GetParity(DcfData[i-1].nStart, DcfData[i-1].nLength);
							BIOS::LCD::Print( x+8, y, nCheck == nCurBit ? RGB565(ffffff) : RGB565(ff0000), clrBack, itoa(nCurBit) ); 
						}
						break;
					case TDcfItem::EParity2:
						{
							int nCheck = GetParity(36, 57-36+1);
							BIOS::LCD::Print( x+8, y, nCheck == nCurBit ? RGB565(ffffff) : RGB565(ff0000), clrBack, itoa(nCurBit) ); 
						}
						break;
					case TDcfItem::EBits:
						for (int j=0; j<DcfItem.nLength; j++)
						{
							nCurBit = GetBit(DcfItem.nStart+j);
							ui16 clrB2 = (DcfItem.nStart+j == m_nBitIndex-1) ? RGB565(b00000) : clrBack;
							BIOS::LCD::Print( x+j*8+8, y, RGB565(ffffff), clrB2, itoa(nCurBit) ); 								
						}
						break;
					case TDcfItem::ENumber:
						{
							int nValue = GetBits(DcfItem.nStart, DcfItem.nLength);
							BIOS::LCD::Printf( x, y, RGB565(ffffff), clrBack, "%2x ", nValue);
							if ( DcfItem.pTarget )
								*DcfItem.pTarget = BcdToInt(nValue);
						}
						break;
					case TDcfItem::EYear:
						{
							int nValue = GetBits(DcfItem.nStart, DcfItem.nLength);
							BIOS::LCD::Printf( x+8, y, RGB565(ffffff), clrBack, "%x ", nValue | 0x2000);
							if ( DcfItem.pTarget )
								*DcfItem.pTarget = 2000 + BcdToInt(nValue);
						}
						break;
				};
			}

			BIOS::LCD::Printf(10, 240-32-8, RGB565(505050), 0, "BitIndex=%d ", m_nBitIndex );
			int nSeconds = m_nBitIndex > 0 ? m_nBitIndex : 0;

			if ( m_lDcfAcquired == 0 )
			{						
				BIOS::LCD::Printf(10, 240-48-8, RGB565(b0b0b0), 0, "%02d:%02d:%02d %s %d. %s %d      ", 
					nHours, nMinutes, nSeconds, GetWeekday(nWDay), nDay, GetMonth(nMonth), nYear );
			} 

//			int nPassedTick = BIOS::SYS::GetTick() - nTick;
//			BIOS::LCD::Printf(320, 0x10, RGB565(505050), 0, "upd %d ms  ", nPassedTick );
		}

		if ( /*m_bRedraw &&*/ m_lDcfAcquired != 0 )
		{
			int nWDay = BcdToInt(GetBits(42, 3, m_DcfValid));
			int nDay = BcdToInt(GetBits(36, 6, m_DcfValid));
			int nMonth = BcdToInt(GetBits(45, 5, m_DcfValid));
			int nYear = BcdToInt(GetBits(50, 8, m_DcfValid));
			int nHours = BcdToInt(GetBits(29, 6, m_DcfValid));
			int nMinutes = BcdToInt(GetBits(21, 7, m_DcfValid));
			int nSeconds = ( BIOS::SYS::GetTick() - m_lDcfAcquired ) / 1000;

			if ( nSeconds >= 60 )
			{
				nMinutes += nSeconds / 60;
				nSeconds %= 60;
				if ( nMinutes >= 60 )
				{
					nHours += nMinutes / 60;
					nMinutes %= 60;
					if ( nHours >= 24 )
					{
						int nNewDays = nHours / 24;
						nDay += nNewDays;
						nWDay = ((nWDay-1+nNewDays) % 7)+1;
						nHours %= 24;
						// test nDay ?
					}	
				}
			}

			BIOS::LCD::Printf(10, 240-48-8, RGB565(ffffff), 0, "%02d:%02d:%02d %s %d. %s %d      ", 
				nHours, nMinutes, nSeconds, GetWeekday(nWDay), nDay, GetMonth(nMonth), nYear );
		}

		m_bRedraw = false;
		if ( m_bError )
		{
			dbg( 'E', RGB565(ff0000) );
			m_bError = false;
		}
		if ( m_bSync )
		{
			dbg( 'S', RGB565(00ff00) );
			m_bSync = false;
		}
		if ( m_cBit )
		{
			dbg( m_cBit, RGB565(ffffff) );
			m_cBit = 0;
		}
	}

	void dbg(char ch, ui16 clr)
	{
/*
		static int i=0;
		char str[3];
		str[0] = ch;
		str[1] = ' ';
		str[2] = 0;
		if ( i >= 400-8 )
			str[1] = 0;
		BIOS::LCD::Print(i, 240-16, clr, RGB565(000000), str);
		BIOS::SERIAL::Send(str);
		i += 8;
		if ( i >= 400-8 )
			i = 8;
*/
	}

	void PushBit(char c)
	{
		if ( c == 'E' )	
		{
			m_bWasError = true;
			m_bError = true;
			m_nBitIndex = -1;
			return;
		}
		if ( c == 'M' )	
		{
			//BIOS::DBG::Print("sync(%d,%d)", m_nBitIndex, m_bWasError);
			if ( m_nBitIndex == -1 )
 			{
				// first sync
			} else if ( m_nBitIndex == 59 && !m_bWasError )
			{
				if ( IsDcfValid() )
				{
					BIOS::LCD::Printf(10, 240-16-8, RGB565(00ff00), 0, "DCF frame valid!   "); 

					m_DcfValid[0] = m_DcfBits[0];
					m_DcfValid[1] = m_DcfBits[1];
					m_lDcfAcquired = BIOS::SYS::GetTick();
				} else
					BIOS::LCD::Printf(10, 240-16-8, RGB565(ff0000), 0, "DCF frame invalid! "); 
			} else
				BIOS::LCD::Printf(10, 240-16-8, RGB565(ff0000), 0, "DCF decoding error!"); 

			m_bSync = true;
			m_nBitIndex = 0;
			m_bWasError = false;
			return;
		}
		m_cBit = c;
		m_bRedraw = true; // after the bit was received, we have 700ms to draw
		if ( m_nBitIndex >= 0 && m_nBitIndex < 60 )
		{
			int nIndex = m_nBitIndex/32;
			int nMask = (1<<(m_nBitIndex & 31));
			m_nBitIndex++;
			             
			m_DcfBits[nIndex] &= ~nMask;
			if ( c == '1' )
				m_DcfBits[nIndex] |= nMask;
		}
	}

	void OnWave(bool bSignal)
	{
		BIOS::LCD::Bar(80+32, 240-32-8, 80+32+14, 240-32-8+14, bSignal ? RGB565(ffff00) : RGB565(404040));

		if ( bSignal != m_bPrevSignal )
		{
			int nImpulseLen;
			int nTime = BIOS::SYS::GetTick();
			nImpulseLen = nTime - m_nLastChange;
			m_nLastChange = nTime;
			m_bPrevSignal = bSignal;

			if ( bSignal ) 	// low to high
			{
				// duration of pause
				// mark is > 1700 
				if ( nImpulseLen > 1700 && nImpulseLen < 2000 )
					PushBit('M');
				else
				if ( nImpulseLen < 600 || nImpulseLen >= 2000 )
					PushBit('E');
			}
			else						// high to low
			{
				// duration of impulse
				if ( nImpulseLen >= 50 && nImpulseLen < 190 )
					PushBit('0');
				else if ( nImpulseLen < 300 )
					PushBit('1');
				else
					PushBit('E');
			}					
		}
	}

	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
	 	if ( code == ToWord('t', 'i') ) // tick, needs 80 fps
		{
			OnWave(BIOS::GPIO::DigitalRead(BIOS::GPIO::P1));
			return;
		}

		if (code == ToWord('L', 'D') )
		{
			KillTimer();
			return;
		}

		if (code == ToWord('L', 'E') )
		{
			BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
			BIOS::LCD::Clear(RGB565(000000));
			GUI::Background(CRect(0, 0, 320, 14), RGB565(4040b0), RGB565(404040));
			BIOS::LCD::Printf(8, 0, RGB565(ffffff), RGBTRANS, "DCF77 frame decoder"); 

			Clear();
			m_bRedraw = true;
			m_nBitIndex = -1;
			m_bWasError = true; // buffer not valid
			m_bError = false;
			m_bSync = false;
			m_cBit = 0;
			m_bPrevSignal = false;
			m_nLastChange = 0;
			SetTimer(200);
			return;
		}
	}

	bool HasOverlay()
	{
		return false;
	}

	char* itoa(int n)
	{
		static char buf[16];
		sprintf(buf, "%d", n);
		return buf;
	}
};
