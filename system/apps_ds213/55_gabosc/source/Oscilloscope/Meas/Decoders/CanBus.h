
class CCanbusDecoder : public CBitDecoder 
{
private:
	bool m_bStartBit;
	int m_nLastBit;
	int m_nCount;
	int m_nIndex;

public:
	int m_nFrameArb;
	int m_nFrameDlc;
	int m_arrFrameData[8];
	int m_nFrameCrc;
	bool m_bFinished;

public:
	CCanbusDecoder()
	{
		m_bStartBit = false;
		m_nFrameArb = 0;
		m_nFrameDlc = 0;
		m_nFrameCrc = 0;
		m_bFinished = false;
		memset( m_arrFrameData, 0, sizeof(m_arrFrameData) );
	}

	virtual bool operator << (int nBit)
	{
		if ( !m_bStartBit )
		{
			if ( nBit == 0 )
			{
				m_bStartBit = true;
				m_nIndex = 1;
				m_nLastBit = 0;
				m_nCount = 1;
			}
			return true;
		}

		if ( m_nIndex > 22+8*m_nFrameDlc+15 )
			return true; // idle bits continue

		if ( m_nCount == 5 )
		{
			// bit stuffing
			if ( nBit != 1-m_nLastBit )
			{
				_ASSERT( 0 );
				return false;
			}
			m_nLastBit = nBit;
			m_nCount = 1;
			return true;
		}

		if ( nBit == m_nLastBit )
		{ 
			m_nCount++;
		} else
		{
			m_nCount = 1;
			m_nLastBit = nBit;
		}
		
		m_nIndex++;
		if ( m_nIndex >= 1 && m_nIndex <= 1+11 )
		{
			m_nFrameArb <<= 1;
			m_nFrameArb |= nBit;
		}

		if ( m_nIndex >= 13 && m_nIndex < 13+3 )
		{
			// RTR, IDE, RB0
		}

		if ( m_nIndex >= 16 && m_nIndex < 16+4 )
		{
			m_nFrameDlc <<= 1;
			m_nFrameDlc |= nBit;
			_ASSERT( m_nFrameDlc <= 8 );
		}

		if ( m_nIndex >= 20 && m_nIndex < 20+8*m_nFrameDlc )
		{
			int nByte = (m_nIndex - 20)/8;
			_ASSERT( nByte < 8 );
			m_arrFrameData[nByte] <<= 1;
			m_arrFrameData[nByte] |= nBit;
		}

		if ( m_nIndex >= 22+8*m_nFrameDlc+1 && m_nIndex <= 22+8*m_nFrameDlc+15 )
		{
			m_bFinished = true; // data bytes were received successfully

			m_nFrameCrc <<= 1;
			m_nFrameCrc |= nBit;
		}
				
		return true;
	}

	void Visualize()
	{
		int x = 20;
		int y = 30;
		x += 8*BIOS::LCD::Print(x, y, RGB565(ff0000), 0x0101, "CanBus: ");
		x += 8*BIOS::LCD::Printf(x, y, RGB565(ffffff), 0x0101, "ID=%03x (", m_nFrameArb );
		for (int i=0; i<m_nFrameDlc; i++)
			x += 8*BIOS::LCD::Printf(x, y, RGB565(b0b0b0), 0x0101, (i>0) ? " %02x" : "%02x", m_arrFrameData[i]);
		x += 8*BIOS::LCD::Printf(x, y, RGB565(ffffff), 0x0101, ")" );
		if ( !m_bFinished )
			x += 8*BIOS::LCD::Printf(x, y, RGB565(ffffff), 0x0101, " ?" );
	}

	void Test()
	{
		// http://www.valky.eu/data/software/!can.html
		// id=0x320, dlc=8,	0x12, 0x00, 0x7f, 0xff, 0xff, 0xc1, 0xff, 0x00
		const char* ptest = "11001100100000100010000010100100000100000111110111110111110111110111110100000111110111110000010000011011001110110011111111111";
		for (const char* p=ptest; *p; p++)
			*this << *p - '0';
		Visualize();
	}
};
