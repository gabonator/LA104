#ifndef __BITMAP_H__
#define __BITMAP_H__

class CBitmap {
	int naccum;
	const ui8* m_p;
	int base_x;
	int base_y;
	int x;
	int y;
	
public:
	int m_width;
	int m_height;
	ui16 m_arrPalette[16];

public:
	void Load( const ui8* pData )
	{
		naccum = -1;
		if ( pData[0] != 'G' /*|| pData[1] != 'B'*/ )
			return;

		m_p = pData+2;
		m_width = getvlc8();
		m_height = getvlc8();
		if ( pData[1] == 'b' )
			return;
		if ( pData[1] != 'B' )
			return;
		for (int i=0; i<16; i++)
		{
			int nHigh = *m_p++;
			int nLow = *m_p++;
			m_arrPalette[i] = (nHigh<<8) | nLow;
		}
	}

	void Blit( int _x, int _y )
	{
		naccum = -1;
		base_x = _x;
		base_y = _y;
		x = 0;
		y = 0;

		const ui8* pPixels = m_p;

		int nPixels = m_width*m_height;
		bool bSame = true;
		while ( nPixels > 0 )
		{
			int nCount;
			if ( bSame )
			{
				nCount = getvlc4()+1;
				int nColor = getfix4();
				for (int i=0; i<nCount; i++)
					Pixel( m_arrPalette[nColor] );
				bSame = false;
			} else
			{	// unique
				nCount = getfix4()+1;
				for (int i=0; i<nCount; i++)
				{
					int nColor = getfix4();
					Pixel( m_arrPalette[nColor] );
				}
				bSame = true;
			}
			nPixels -= nCount;
			_ASSERT( nPixels >= 0 );
		}
		m_p = pPixels;
	}

private:
	void Pixel(ui16 clr)
	{
		if ( clr != RGB565(ff00ff) )
			BIOS::LCD::PutPixel(base_x+x, base_y+y, clr);
		if ( y & 1 )
		{
			if ( --x < 0 )
			{
				x++;
				y++;
			}
		} else
		{
			if ( ++x >= m_width )
			{
				x--;
				y++;
			}
		}
	}

	int getvlc8()
	{
		ui8 b;
		int nBit = 0, i = 0;
		do 
		{
			b = *m_p++;
			i |= (b & 127) << nBit;
			nBit += 7;
		} while (b & 128);
		return i;
	}

	int getfix4()
	{
		if ( naccum == -1 )
		{
			naccum = *m_p++;
			return naccum>>4;
		}
		int nX = naccum&0x0f;
		naccum = -1;
		return nX;
	}

	int getvlc4()
	{
		ui8 b;
		int nBit = 0, i = 0;
		do 
		{
			b = getfix4();
			i |= (b & 7) << nBit;
			nBit += 3;
		} while (b & 8);
		return i;
	}
};

#endif