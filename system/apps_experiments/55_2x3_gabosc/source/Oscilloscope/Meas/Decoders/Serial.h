class CBitDecoder {
public:
	virtual bool operator << (int nBit) = 0;
};

class CSerialDecoder : public CBitDecoder
{
public:
	typedef bool (CSerialDecoder::* TProcessFunc)(int, int);

private:
	int nBegin, nEnd;
	int nLast, nThresh, nTrigMin, nTrigMax;
	int nBit;

	int nDecodeIndex;

public:
	int nMinPeriod;
	CMeasStatistics* m_pThis;
	int nTotalBits, nTotalTime;
	int nDecoded;
	unsigned char strDecode[32];
	int nIdle;
	CBitDecoder* m_pCurrentDecoder;

public:
	CMeasStatistics* This()
	{
		return m_pThis;
	}

	bool Prepare()
	{
		if ( !This()->_GetRange( nBegin, nEnd, This()->m_curRange ) )
			return false;
		if ( This()->m_nRawMax - This()->m_nRawMin < 16 )
			return false;

		m_pCurrentDecoder = NULL;

		nThresh = ( This()->m_nRawMax - This()->m_nRawMin ) / 4;
		nTrigMin = This()->m_nRawMin + nThresh;
		nTrigMax = This()->m_nRawMax - nThresh;
		nLast = -1;
		nMinPeriod = -1;

		nTotalBits = 0;
		nTotalTime = 0;

		nDecodeIndex = -1;
		nDecoded = 0;
		strDecode[nDecoded] = 0;
		return true;
	}

	void Average()
	{
		int nHigh = 0, nLow = 0;
		for ( int i = nBegin; i < nEnd; i++ )
		{
			int nSample = Sampler::GetAt(i);
			nSample = This()->_GetSample( nSample );

			if ( nSample > nTrigMax )
				nHigh++;
			if ( nSample < nTrigMin )
				nLow++;
		}
		nIdle = nHigh > nLow ? 1 : 0;
	}

	bool Do(TProcessFunc ProcessEdge)
	{
		int nNewState = -1, nTrigState = -1;
		for ( int i = nBegin; i < nEnd; i++ )
		{
			int nSample = Sampler::GetAt(i);
			nSample = This()->_GetSample( nSample );

			if ( nSample > nTrigMax )
				nNewState = 1;
			if ( nSample < nTrigMin )
				nNewState = 0;

			if ( nTrigState != -1 && nNewState != nTrigState )
			{
				if ( nLast != -1 )
					if ( !(this->*ProcessEdge)( i-nLast, nNewState ) )
						return false;
				nLast = i;
			}
			nTrigState = nNewState;
		}
		//if ( !(this->*ProcessEdge)( nEnd-nLast, 1-nNewState ) )
		//	return false;
		return true;
	}

	bool FindSmallest(int nCount, int nRising)
	{
		if ( nCount <= 2 )
			return true; // noise!?
		if ( nMinPeriod == -1 )
			nMinPeriod = nCount;
		if ( nCount < nMinPeriod )
			nMinPeriod = nCount;
		return true;
	}

	bool Sum(int nCount, int nRising)
	{
		if ( nCount <= 2 )
			return true;

		int nBits = 0;
		if ( nCount >= nMinPeriod*9 )
			return false;
		else if ( nCount >= nMinPeriod*8 )
			nBits = 8;
		else if ( nCount >= nMinPeriod*7 )
			nBits = 7;
		else if ( nCount >= nMinPeriod*6 )
			nBits = 6;
		else if ( nCount >= nMinPeriod*5 )
			nBits = 5;
		else if ( nCount >= nMinPeriod*4 )
			nBits = 4;
		else if ( nCount >= nMinPeriod*3 )
			nBits = 3;
		else if ( nCount >= nMinPeriod*2 )
			nBits = 2;
		else if ( nCount >= nMinPeriod*1 )
			nBits = 1;
		else
			return false;
		//int nMin = nMinPeriod;
		nTotalBits += nBits;
		nTotalTime += nCount;
		//BIOS::DBG::Print( "rel=%f %d/%d => %d\n", nCount/(float)nMinPeriod, nCount, nMinPeriod, nBits);
		return true;	
	}

	bool Decode(int nCount, int nRising)
	{
		if ( nCount <= 2 )
			return true;

		_ASSERT( m_pCurrentDecoder );

		int nMin = nMinPeriod;
		for ( int i = nMin; i <= nCount; i += nMin )
			if ( !m_pCurrentDecoder->operator<< ( nIdle ? 1-nRising : nRising) )
				return false;
		return true;
	}

	void SetDecoder( CBitDecoder* pDecoder )
	{
		m_pCurrentDecoder = pDecoder;
	}

	virtual bool operator << (int nBit)
	{
		static int nPrev = 0;
		static int nByte = 0;

		//				BIOS::DBG::Print("%d", nBit);
		// waiting for start bit
		if ( nDecodeIndex == -1 )
		{
			if ( nPrev == 1 && nBit == 0 )
			{
				nDecodeIndex = 0;
				//						BIOS::DBG::Print("<");
			}
			nPrev = nBit;
			return true;
		}

		nByte >>= 1;
		if ( nBit )
			nByte |= 128;

		if ( ++nDecodeIndex >= 8 )
		{
			//					BIOS::DBG::Print(">");
			if ( nDecoded <= (int)sizeof(strDecode) )
			{
				strDecode[nDecoded++] = nByte;
			} else
				return false;

			//					BIOS::DBG::Print(" -> decod=%02x %d '%c'\n", nByte, nByte, nByte);
			nDecodeIndex = -1;
			nByte = 0;
		}
		return true;
	}

	float GetBaud()
	{
		float fBaudPeriod = (nTotalTime / (float) nTotalBits) * Settings.Runtime.m_fTimeRes / CWndGraph::BlkX;
		float fBaud = 1.0f / fBaudPeriod;
		return fBaud;
	}

	void Visualize()
	{
		bool bMidi = _IsMidi();
		bool bHex = bMidi;
		// display decoded characters
		if ( nDecoded <= 0 )
			return;

		int x = 20;
		int y = 30 + ((int)This()->m_curSrc)*16;
		if ( bMidi )
			x += BIOS::LCD::Print(x, y, RGB565(ffffff), 0x0101, "MIDI: " );
		else if ( nIdle == 1 )
			x += BIOS::LCD::Print(x, y, RGB565(ffffff), 0x0101, "UART-TTL: " );
		else
			x += BIOS::LCD::Print(x, y, RGB565(ffffff), 0x0101, "UART-RS232: " );

		if ( bMidi )
		{
			_ShowMidi(x, y);
		} else
			if ( bHex )
			{
				_ShowBinary(x, y);
			} else
			{
				_ShowAscii(x, y);
			}
	}

	void _ShowBinary(int x, int y)
	{
		for ( int i = 0; i < nDecoded; i++ )
		{
			unsigned char ch = strDecode[i];
			x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "%02x ", ch );
		}
	}	

	void _ShowAscii(int x, int y)
	{
		for ( int i = 0; i < nDecoded; i++ )
		{
			unsigned char ch = strDecode[i];
			if ( ch >= ' ' && ch <= 127 )	// readable char
				x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "%c", ch );
			else
				x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "[%02x]", ch );
		}
	}

	bool _IsMidi()
	{
		float fBaud = GetBaud();
		return (fBaud > 30000.0f) && (fBaud < 32000.0f);
	}

	void _ShowMidi(int x, int y)
	{
		for ( int i = 0; i < nDecoded; i++ )
		{
			int ch = strDecode[i];
			int atr1 = i < nDecoded - 1 ? strDecode[i+1] : -1;
			int atr2 = i < nDecoded - 2 ? strDecode[i+2] : -1;

			if ( (ch & 0xf0) >= 0x80 )
			{
				if ( (ch & 0x0f) > 0 )
					x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "%d_", 
					ch & 0x0f );
			}
			if ( (ch & 0xf0) == 0x90 )
			{
				if ( atr2 == 0 )
					x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "NoteOff(%s)", 
					CUtils::MidiNote(atr1), atr2);
				else
					x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "NoteOn(%s,%d)", 
					CUtils::MidiNote(atr1), atr2);
				i += 2;
			}
			if ( (ch & 0xf0) == 0x80 )
			{
				x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "NoteOff(%s)", 
					ch & 0x0f, CUtils::MidiNote(atr1));
				i += 2;
			}
			if ( (ch & 0xf0) == 0xE0 )
			{
				int nValue = (atr1|(atr2<<7)) - 8192;
				x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "Pitch(%d)", 
					nValue);
				i += 2;
			}
			if ( (ch & 0xf0) == 0xB0 )
			{
				x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "Ctl(%d,%d)", 
					atr1, atr2);
				i += 2;
			}
			if ( (ch & 0xf0) == 0xC0 )
			{
				int nInstr = 0;
				if ( atr1 >= 0 )
					nInstr += atr1;
				if ( atr2 >= 0 )
					nInstr += atr1*128;

				x += 8*BIOS::LCD::Printf(x, y, RGB565(ff0000), 0x0101, "Patch(%d)", 
					nInstr);
				i += 2;
			}
			x += 8;
		}
	}

};

