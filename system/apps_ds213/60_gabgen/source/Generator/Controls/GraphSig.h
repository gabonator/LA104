#include <Oscilloscope/Controls/GraphBase.h>

//#define _GENERATOR_MULTIVIEW

class CWndSigGraph : public CWnd
{
public:
	const ui16* m_pSignal;
	ui16 m_nCount;
	CRect m_rcRange;
	int	m_nEditX;
	int m_nStep;
	CWnd* m_pExit;

	enum {
		BlkX = 25,
		BlkY = 25,
		DivsX = 10,
		DivsY = 8
	};

public:
	void Create(CWnd *pParent, ui16 dwFlags) 
	{
		CWnd::Create("CWndGraph", dwFlags, CRect(12, 22, 12+DivsX*BlkX, 22+DivsY*BlkY), pParent);
		m_nEditX = 0;
		m_pSignal = NULL;
		m_nStep = 8;
		m_pExit = NULL;
	}

	void ExitHandler(CWnd* pExit)
	{
		m_pExit = pExit;
	}

	void Setup(const ui16* pSignal, ui16 nCount )
	{
		m_pSignal = pSignal;
		m_nCount = nCount;
		m_nEditX = 0;
		Invalidate();
	}

	virtual void OnPaint()
	{
		if ( !m_rcRange.IsValid() )
		{
			CRect rc = m_rcClient;
			rc.Inflate( 1, 1, 1, 1 );
			BIOS::LCD::Rectangle( rc, RGB565(b0b0b0) );
			BIOS::LCD::Bar( m_rcClient, RGB565(000000) );	
		
			const ui16 grid[] = {RGB565(808080), 0, 0, 0, 0};

			for (int x=1; x<10; x++)
				BIOS::LCD::Pattern(m_rcClient.left+BlkX*x, m_rcClient.top, m_rcClient.left+BlkX*x+1, m_rcClient.bottom-4, grid, COUNT(grid));
			for (int y=1; y<8; y++)
				BIOS::LCD::Pattern(m_rcClient.left, m_rcClient.top+BlkY*y, m_rcClient.right, m_rcClient.top+BlkY*y+1, grid, COUNT(grid));
		}

		if (!m_pSignal)
			return;

		int nWidth = m_rcClient.Width();
		int nHeight = m_rcClient.Height();

		nHeight -= BlkY*2;
		int nBaseY = m_rcClient.bottom-BlkY;
		ui16 oy = 0;
		int nSelected = HasFocus() ? m_nEditX*nWidth/m_nCount + nWidth/m_nCount/2 : -20;
		int nSelectedY = 0;

		if ( !m_rcRange.IsValid() )
		{
			m_rcRange.left = 0;
			m_rcRange.right = nWidth;
		} else
		{	
			m_rcRange.left = max(m_rcRange.left, m_rcClient.left);
			m_rcRange.right = min(m_rcRange.right, m_rcClient.right);
			m_rcRange.Deflate(1, 0, 1, 0);
			BIOS::LCD::Bar( m_rcRange, RGB565(000000) );	
			m_rcRange.Inflate(1, 0, 1, 0);
			m_rcRange.Offset( -m_rcClient.left, -m_rcClient.top );
		}

		for ( ui16 i = m_rcRange.left; i < m_rcRange.right; i++ )
		{
			int nSubIndex = i*m_nCount/nWidth;
			ui16 y = m_pSignal[nSubIndex];

			_ASSERT( y < 0x1000 );
			y = y*nHeight/0x1000;
			if (i==m_rcRange.left)
				oy = y;
			ui16 clr = RGB565(ffffff);
			BIOS::LCD::Line( 
				m_rcClient.left + i-1, nBaseY - oy, 
				m_rcClient.left + i, nBaseY - y, 
				clr );

			if ( i == nSelected )
				nSelectedY = y;

			if ( i == nSelected+3 )
				BIOS::LCD::Draw( m_rcClient.left + i-6, nBaseY - nSelectedY - 4, 
					(m_nStep==8) ? RGB565(ff0000) : RGB565(b00000), RGBTRANS, CShapes::markerX);

			oy = y;
		}
		m_rcRange.Invalidate();
	}

	void _Inc(ui8& nByte, int nDiff)
	{
		int nNew = nByte + nDiff;
		CUtils::Clamp<int>(nNew, 0, 255);
		nByte = nNew;
	}

	virtual void OnKey(int nKey) override
	{
		_ASSERT( Settings.Gen.Wave == CSettings::Generator::_Volatile );
		if ( nKey == BIOS::KEY::Right )
		{
			if ( m_nEditX < CCoreGenerator::GetVolatileLen() - 1 )
			{
				m_rcRange = m_rcClient;
				m_rcRange.left += (m_nEditX)*m_rcClient.Width()/m_nCount-2;
				m_rcRange.right = m_rcRange.left + 2*m_rcClient.Width()/m_nCount+4;
				m_nEditX++;
				Invalidate();
			}
			return;
		}
		if ( nKey == BIOS::KEY::Left )
		{
			if ( m_nEditX > 0 )
			{
				m_nEditX--;
				m_rcRange = m_rcClient;
				m_rcRange.left += (m_nEditX)*m_rcClient.Width()/m_nCount-2;
				m_rcRange.right = m_rcRange.left + 2*m_rcClient.Width()/m_nCount+4;
				Invalidate();
			}
			return;
		}
		if ( nKey == BIOS::KEY::Up )
		{
			_ASSERT( m_nEditX >= 0 && m_nEditX < CCoreGenerator::GetVolatileLen() );

			_Inc( CCoreGenerator::GetVolatile()[m_nEditX], m_nStep);
			if ( m_nStep == 8 && m_nEditX+1 < CCoreGenerator::GetVolatileLen() )
				_Inc( CCoreGenerator::GetVolatile()[m_nEditX+1], m_nStep/2);
			if ( m_nStep == 8 && m_nEditX-1 > 0  )
				_Inc( CCoreGenerator::GetVolatile()[m_nEditX-1], m_nStep/2);

			CCoreGenerator::Update();
			m_rcRange = m_rcClient;
			m_rcRange.left += m_nEditX*m_rcClient.Width()/m_nCount-1;
			m_rcRange.right = m_rcRange.left + 10;
			if ( m_nStep == 8 )
			{
				m_rcRange.left -= 2*m_rcClient.Width()/m_nCount-1;
				m_rcRange.right += 2*m_rcClient.Width()/m_nCount-1;
			}
			Invalidate();
			return;
		}
		if ( nKey == BIOS::KEY::Down )
		{
			_ASSERT( m_nEditX >= 0 && m_nEditX < CCoreGenerator::GetVolatileLen() );

			_Inc( CCoreGenerator::GetVolatile()[m_nEditX], -m_nStep);
			if ( m_nStep == 8 && m_nEditX+1 < CCoreGenerator::GetVolatileLen() )
				_Inc( CCoreGenerator::GetVolatile()[m_nEditX+1], -m_nStep/2);
			if ( m_nStep == 8 && m_nEditX-1 > 0  )
				_Inc( CCoreGenerator::GetVolatile()[m_nEditX-1], -m_nStep/2);

			CCoreGenerator::Update();
			m_rcRange = m_rcClient;
			m_rcRange.left += m_nEditX*m_rcClient.Width()/m_nCount-1;
			m_rcRange.right = m_rcRange.left + 10;
			if ( m_nStep == 8 )
			{
				m_rcRange.left -= 2*m_rcClient.Width()/m_nCount-1;
				m_rcRange.right += 2*m_rcClient.Width()/m_nCount-1;
			}
			Invalidate();
			return;
		}

		if ( nKey == BIOS::KEY::Enter )
		{
			m_nStep = ( m_nStep == 8 ) ? 1 : 8;
			Invalidate();
		}

		if ( nKey == BIOS::KEY::Escape )
		{
			_ASSERT( m_pExit );
			SendMessage( m_pExit, ToWord('e', 'x'), 0 );
		}
	}
};

