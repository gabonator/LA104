#include "GraphOsc.h"
#include <MainWnd.h>

CWndOscGraph::CWndOscGraph()
{
	ClearAverage();
	m_bPersistReset = false;
	m_bNeedRedraw = true;
}

void CWndOscGraph::_PrepareColumn( ui16 *column, ui16 n, ui16 clr )
{
	for (ui16 y=0; y<DivsY*BlkY; y++)
		column[y] = clr;
//		memset( column, clr, DivsY*BlkY*2 );
	if ( n == 0 )
		return;
	// Grid display logic
	if(Settings.Disp.Grid != CSettings::Display::_GridNone)
	{
		if ( (n % BlkX) == 0)
		{	// Display vertical line or dots
			ui8 inc = (Settings.Disp.Grid == CSettings::Display::_GridLines) ? 1 : 5;
			for (ui16 y=0; y<DivsY*BlkY; y += inc)
				column[y] = RGB565(808080);
		} else
		if ((Settings.Disp.Grid == CSettings::Display::_GridLines) || ((n%6) == 0 ))
		{	// Display horizontal line or dots
			for (ui16 y=BlkY; y<DivsY*BlkY-1; y += BlkY)
				column[y] = RGB565(808080);
		}
	}
	// Axis display logic
	if(Settings.Disp.Axis != CSettings::Display::_AxisNone)
	{
		if (n == CenterX)
		{	// Display center X axis
			for (ui16 y=0; y<DivsY*BlkY; y += 1)
				column[y] = RGB565(808080);
		}
		else if( (n==1) || (n == (MaxX-1)) || (n == CenterX-1) || (n == CenterX+1))
		{	// Display X axis sub divisions
			for (ui16 y=5; y<DivsY*BlkY; y += 5)
				column[y] = RGB565(808080);
		}
		else if( (n==2) || (n == (MaxX-2)) || (n == CenterX-2) || (n == CenterX+2))
		{	// Display X axis main divisions
			for (ui16 y=BlkY; y<DivsY*BlkY; y += BlkY)
				column[y] = RGB565(808080);
		}
		// Display horizontal main axis
		column[CenterY] = RGB565(808080);
		if (Settings.Disp.Axis == CSettings::Display::_AxisDouble)
		{	// Display horizontal double axis
			column[CenterTop] = RGB565(808080);
			column[CenterBottom] = RGB565(808080);
		}
		if((n%6) == 0 )
		{	// Displau horizontal sub divisions
			column[0] = RGB565(808080);
			column[CenterY-1] = RGB565(808080);
			column[CenterY+1] = RGB565(808080);
			column[MaxY-1] = RGB565(808080);
			if (Settings.Disp.Axis == CSettings::Display::_AxisDouble)
			{
				column[CenterTop+1] = RGB565(808080);
				column[CenterTop-1] = RGB565(808080);
				column[CenterBottom+1] = RGB565(808080);
				column[CenterBottom-1] = RGB565(808080);
			}
		}
		if((n%BlkX) == 0 )
		{	// Display horizontal main divisions
			column[1] = RGB565(808080);
			column[CenterY+2] = RGB565(808080);
			column[CenterY-2] = RGB565(808080);
			column[MaxY-2] = RGB565(808080);
			if (Settings.Disp.Axis == CSettings::Display::_AxisDouble)
			{
				column[CenterTop+2] = RGB565(808080);
				column[CenterTop-2] = RGB565(808080);
				column[CenterBottom+2] = RGB565(808080);
				column[CenterBottom-2] = RGB565(808080);
			}
		}
	}
}

ui16 CWndOscGraph::_Interpolate( ui16 clrA, ui16 clrB )
{
	int ar = Get565R(clrA);
	int ag = Get565G(clrA);
	int ab = Get565B(clrA);
	int br = Get565R(clrB);
	int bg = Get565G(clrB);
	int bb = Get565B(clrB);
	ar = (ar+br)/2;
	ag = (ag+bg)/2;
	ab = (ab+bb)/2;
	return RGB565RGB(ar, ag, ab);
}

/*virtual*/ void CWndOscGraph::Create(CWnd *pParent, int dwFlags) 
{
	CWndGraph::Create( pParent, dwFlags | CWnd::WsListener );
}

/*virtual*/ void CWndOscGraph::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
	if ( pSender == NULL && code == WmBroadcast && data == ToWord('d', 'g') )
	{
		Invalidate();
		CCoreOscilloscope::UpdateConstants();
		return;
	}
	if ( pSender == this && code == ToWord('S', 'H') && data == 1 )
	{
		m_bNeedRedraw = true;
	}
}

/*virtual*/ void CWndOscGraph::OnPaint()
{
	if ( Settings.Disp.Axes == CSettings::Display::_TY )
		OnPaintTY();
	else
		OnPaintXY();
}

void CWndOscGraph::OnPaintXY()
{
	if ( CWnd::GetOverlay().IsValid() )
	{
		return;
	}

	// input channels
	ui8 bEnabled1 = Settings.CH1.Enabled == CSettings::AnalogChannel::_YES;
	ui8 bEnabled2 = Settings.CH2.Enabled == CSettings::AnalogChannel::_YES;
	ui16 clrPoint = _Interpolate( Settings.CH1.u16Color, Settings.CH2.u16Color );

	// calibration
	CSettings::Calibrator::FastCalc calCh1, calCh2;
	Settings.CH1Calib.Prepare( &Settings.CH1, calCh1 );
	Settings.CH2Calib.Prepare( &Settings.CH2, calCh2 );

	// range
	int nMaxIndex = BIOS::ADC::NumSamples;
	int nIndex = Settings.Time.Shift;
	int nColMax = m_rcClient.Width(); 

	//if ( m_bNeedRedraw ) // does not work reliably
	{
		CRect rc = m_rcClient;
		rc.Inflate( 1, 1, 1, 1 );
		BIOS::LCD::Rectangle( rc, RGB565(b0b0b0) );
	}

	if ( Settings.Disp.Persist == CSettings::Display::_PerNo || m_bPersistReset || m_bNeedRedraw )
		BIOS::LCD::Bar( m_rcClient, 0x0101 );

	m_bPersistReset = false;
	m_bNeedRedraw = false;

	if ( !bEnabled1 || !bEnabled2 ) 
	{
		BIOS::LCD::Print( m_rcClient.left, m_rcClient.top, RGB565(ffffff), RGBTRANS, (char*)"Enable both channels" );
		return;
	}

	
	for (ui16 x=0; x<nColMax; x++, nIndex++)
	{
		ui32 ui32Sample = nIndex < nMaxIndex ?  Sampler::GetAt(nIndex) : 0;
		
		int nSampleY2 = 0, nSampleY1 = 0;

		if ( bEnabled1 )
		{
			si16 ch1 = (ui8)((ui32Sample) & 0xff);
			ch1 = Settings.CH1Calib.Correct( calCh1, ch1 );
			if ( ch1 < 0 ) 
				ch1 = 0;
			if ( ch1 > 255 ) 
				ch1 = 255;
			nSampleY1 = (ch1*(DivsY*BlkY))>>8;
		}
		if ( bEnabled2 )
		{
			si16 ch2 = (ui8)((ui32Sample>>8) & 0xff);
			ch2 = Settings.CH2Calib.Correct( calCh2, ch2 );
			if ( ch2 < 0 ) 
				ch2 = 0;
			if ( ch2 > 255 ) 
				ch2 = 255;
			nSampleY2 = (ch2*(DivsY*BlkY))>>8;
		}
		if ( Settings.Disp.Axes == CSettings::Display::_XY )
			BIOS::LCD::PutPixel( m_rcClient.left + nSampleY1 + BlkX, m_rcClient.bottom - nSampleY2, clrPoint );
		else
			BIOS::LCD::PutPixel( m_rcClient.left + nSampleY2 + BlkX, m_rcClient.bottom - nSampleY1, clrPoint );
	}
}

void CWndOscGraph::OnPaintTY()
{
	ui16 column[CWndGraph::DivsY*CWndGraph::BlkY];
	if ( !CWnd::GetOverlay().IsValid() /*&& m_bNeedRedraw*/ )
	{
		CRect rc = m_rcClient;
		rc.Inflate( 1, 1, 1, 1 );
		BIOS::LCD::Rectangle( rc, RGB565(b0b0b0) );
		m_bNeedRedraw = false;
	}

	ui16 clr1 = Settings.CH1.u16Color;
	ui16 clr2 = Settings.CH2.u16Color;
	ui16 clr3 = Settings.CH3.u16Color;
	ui16 clr4 = Settings.CH4.u16Color;
	ui8 en1 = Settings.CH1.Enabled == CSettings::AnalogChannel::_YES;
	ui8 en2 = Settings.CH2.Enabled == CSettings::AnalogChannel::_YES;
	ui8 en3 = Settings.CH3.Enabled == CSettings::DigitalChannel::_YES;
	ui8 en4 = Settings.CH4.Enabled == CSettings::DigitalChannel::_YES;

	ui16 clrShade1 = 0, clrShade2 = 0, clrShade12 = 0;

	ui8 bTrigger = (BIOS::SYS::GetTick() - Settings.Trig.nLastChange) < 5000;
	ui16 nTriggerTime = (Settings.Trig.nTime - Settings.Time.Shift);
	if (!bTrigger)
		nTriggerTime = -1;

	int nCut = CWnd::GetOverlay().IsValid() ? CWnd::GetOverlay().left - m_rcClient.left : m_rcClient.Width();
	int nCutTop = CWnd::GetOverlay().IsValid() ? CWnd::GetOverlay().bottom - m_rcClient.top : 0;
	if ( nCutTop >= m_rcClient.Height() )
		nCut = m_rcClient.Width();
	int nFirstTop = CWnd::GetOverlay().IsValid() ? CWnd::GetOverlay().top - m_rcClient.top : 0;

	int nMax = m_rcClient.Width(); 
	if ( CWnd::GetOverlay().IsValid() && CWnd::GetOverlay().left - m_rcClient.left <= 0 )
		return;

	CSettings::Calibrator::FastCalc Ch1fast, Ch2fast;
	Settings.CH1Calib.Prepare( &Settings.CH1, Ch1fast );
	Settings.CH2Calib.Prepare( &Settings.CH2, Ch2fast );

	// if window is outside...
	int nMaxIndex = BIOS::ADC::NumSamples;
	bool bLines = Settings.Disp.Draw != CSettings::Display::_Points;
	bool bFill = Settings.Disp.Draw == CSettings::Display::_Fill;
	bool bAverage1 = Settings.Disp.Average == CSettings::Display::_AvgCh1;
	bool bAverage2 = Settings.Disp.Average == CSettings::Display::_AvgCh2;
	int nPrev1 = -1, nPrev2 = -1, nPrevm = -1, nPrev3 = -1, nPrev4 = -1;

	int nMarkerT1 = -1, nMarkerT2 = -1, nMarkerY1 = -1, nMarkerY2 = -1;
	bool bAreaT = false;
	bool enmath = false;
	int nSampleY2 = 0, nSampleY1 = 0;

	int nY3High = 0;
	int nY3Low = 0;
	int nY4High = 0;
	int nY4Low = 0;
	int nSampleY3 = 0, nSampleY4 = 0;

	if ( en3 )
	{
		nY3Low = Settings.CH3.u16Position;
		nY3High = Settings.CH3.Polarity == CSettings::DigitalChannel::_POS ? 
			nY3Low + 32 : nY3Low - 32;
		CUtils::Clamp<int>( nY3Low, 0, 255 );
		CUtils::Clamp<int>( nY3High, 0, 255 );
		nY3Low = (nY3Low*(DivsY*BlkY))>>8;
		nY3High = (nY3High*(DivsY*BlkY))>>8;
	}
	if ( en4 )
	{
		nY4Low = Settings.CH4.u16Position;
		nY4High = Settings.CH4.Polarity == CSettings::DigitalChannel::_POS ? 
			nY4Low + 32 : nY4Low - 32;
		CUtils::Clamp<int>( nY4Low, 0, 255 );
		CUtils::Clamp<int>( nY4High, 0, 255 );
		nY4Low = (nY4Low*(DivsY*BlkY))>>8;
		nY4High = (nY4High*(DivsY*BlkY))>>8;
	}

	if ( bFill )
	{
		clrShade1 = _Interpolate( clr1, 0x0101 );
		clrShade2 = _Interpolate( clr2, 0x0101 );
		clrShade12 = _Interpolate( _Interpolate(clr2, 0x0101), clr1 );
	}

	bool bUsingMask = false;
	if ( MainWnd.mWndToolBar.GetCurrentLayout() == &MainWnd.m_wndMenuMask )
		bUsingMask = true;

	if ( MainWnd.mWndToolBar.GetCurrentLayout() == &MainWnd.m_wndMenuCursor )
		SetupMarkers( Ch1fast, Ch2fast, nMarkerT1, nMarkerT2, nMarkerY1, nMarkerY2 );
	if ( MainWnd.mWndToolBar.GetCurrentLayout() == &MainWnd.m_wndMenuMeas )
		SetupSelection( bAreaT, nMarkerT1, nMarkerT2 );
	if ( MainWnd.mWndToolBar.GetCurrentLayout() == &MainWnd.m_wndMenuMath && 
		 Settings.Math.Type != CSettings::MathOperator::_Off)
	{
		enmath = true;
		MathSetup( &Ch1fast, &Ch2fast );
	}

	ui16 clrm = Settings.Math.uiColor;
	int nIndex = Settings.Time.Shift;

	for (ui16 x=0; x<nMax; x++, nIndex++)
	{
		int clrCol = (nTriggerTime != x) ? 0x0101 : 0x00;

		if ( bAreaT && nIndex > nMarkerT1 && nIndex < nMarkerT2 )
			clrCol = RGB565(101060);
		if ( nMarkerT1 == nIndex )
			clrCol = Settings.MarkT1.u16Color;
		if ( nMarkerT2 == nIndex )
			clrCol = Settings.MarkT2.u16Color;
			
		_PrepareColumn( column, x, clrCol );
		
		if ( bUsingMask )
		{
			if ( x == 80 && MainWnd.m_wndMenuMask.m_Display != CWndMenuMask::DisplayNo )
			{
				int* nPass = NULL;
				int* nFail = NULL;
				CCoreOscilloscope::GetMaskStats( &nPass, &nFail );
				BIOS::LCD::Printf(m_rcClient.left+2, m_rcClient.bottom-16-14*2, 
					RGB565(ffffff), 0x0101, "Pass: %d", *nPass);
				BIOS::LCD::Printf(m_rcClient.left+2, m_rcClient.bottom-16-14, 
					RGB565(ffffff), 0x0101, "Fail: %d", *nFail);
				BIOS::LCD::Printf(m_rcClient.left+2, m_rcClient.bottom-16, 
					RGB565(ffffff), 0x0101, "%1f%%", (*nPass)*100.0f/(*nFail + *nPass));
			}
			ui8* pLow = NULL;
			ui8* pHigh = NULL;
			CCoreOscilloscope::GetMaskAt( x, &pLow, &pHigh );
			int nLow = pLow ? (((int)*pLow) * DivsY*BlkY)/256 : 0;
			int nHigh = pHigh ? (((int)*pHigh) * DivsY*BlkY)/256 : DivsY*BlkY;
			for ( int i = 0; i < DivsY*BlkY; i++ )
			{
				if ( i <= nLow )
				{
					if ( nLow - i > 4 )
						column[i] |= RGB565(800000);
					else if ( nLow - i > 3 )
						column[i] |= RGB565(a00000);
					else if ( nLow - i > 2 )
						column[i] |= RGB565(b00000);
					else if ( nLow - i > 1 )
						column[i] |= RGB565(d00000);
					else
						column[i] |= RGB565(ff0000);
				}
				if ( i >= nHigh )
				{
					if ( i - nHigh > 4 )
						column[i] |= RGB565(800000);
					else if ( i - nHigh > 3 )
						column[i] |= RGB565(a00000);
					else if ( i - nHigh > 2 )
						column[i] |= RGB565(b00000);
					else if ( i - nHigh > 1 )
						column[i] |= RGB565(d00000);
					else
						column[i] |= RGB565(ff0000);
				}
			}
		}

		BIOS::ADC::TSample Sample;
		Sample.value = nIndex < nMaxIndex ? Sampler::GetAt(nIndex) : 0;

		if ( en1 )
		{
			si16 ch1 = Sample.CH1;
			ch1 = Settings.CH1Calib.Correct( Ch1fast, ch1 );
			CUtils::Clamp<si16>( ch1, 0, 255 );

			if ( bAverage1 )
			{
				ui16& nMemory = m_arrAverageBuf[x];
				int nNew = (nMemory*7 + (ch1<<8)*1)/8;  // 7:1
				nMemory = (ui16)nNew;
				ch1 = nMemory >> 8;
			}

			nSampleY1 = (ch1*(DivsY*BlkY))>>8;
		}
		if ( en2 )
		{
			si16 ch2 = Sample.CH2;
			ch2 = Settings.CH2Calib.Correct( Ch2fast, ch2 );
			CUtils::Clamp<si16>( ch2, 0, 255 );

			if ( bAverage2 )
			{
				ui16& nMemory = m_arrAverageBuf[x];
				int nNew = (nMemory*7 + (ch2<<8)*1)/8;
				nMemory = (ui16)nNew;
				ch2 = nMemory >> 8;
			}

			nSampleY2 = (ch2*(DivsY*BlkY))>>8;
		}
		if ( en3 )
		{
			nSampleY3 = Sample.CH3 ? nY3High : nY3Low;		
		}
		if ( en4 )
		{
			nSampleY4 = Sample.CH4 ? nY4High : nY4Low;
		}

		if ( bFill )
		{
			if ( en1 && en2 )
			{
				if ( nSampleY1 > nSampleY2 )
				{
					int _y;
					for ( _y = 0; _y < nSampleY2; _y++ )
						column[_y] = clrShade12;
					for (; _y < nSampleY1; _y++ )
						column[_y] = clrShade1;
				} else
				{
					int _y;
					for (_y = 0; _y < nSampleY1; _y++)
						column[_y] = clrShade12;
					for (; _y < nSampleY2; _y++ )
						column[_y] = clrShade2;
				}
			} else 
			if ( en1 )
			{
				for ( int _y = 0; _y < nSampleY1; _y++ )
					column[_y] = clrShade1;
			} else
			if ( en2 )
			{
				for ( int _y = 0; _y < nSampleY2; _y++ )
					column[_y] = clrShade2;
			} 
		}

		if ( en3 )
		{
			if ( !bLines )
				column[nSampleY3] = clr3;
			else
			{
				if ( nPrev3 == -1 )
					nPrev3 = nSampleY3;
				int nBottom = min(nSampleY3, nPrev3);
				int nTop = max(nSampleY3, nPrev3);
				for ( int _y = nBottom; _y <= nTop; _y++)
					column[_y] = clr3;
				nPrev3 = nSampleY3;
			}
		}
		if ( en4 )
		{
			if ( !bLines )
				column[nSampleY4] = clr4;
			else
			{
				if ( nPrev4 == -1 )
					nPrev4 = nSampleY4;
				int nBottom = min(nSampleY4, nPrev4);
				int nTop = max(nSampleY4, nPrev4);
				for ( int _y = nBottom; _y <= nTop; _y++)
					column[_y] = clr4;
				nPrev4 = nSampleY4;
			}
		}
		if ( en2 )
		{
			if ( !bLines )
				column[nSampleY2] = clr2;
			else
			{
				if ( nPrev2 == -1 )
					nPrev2 = nSampleY2;
				int nBottom = min(nSampleY2, nPrev2);
				int nTop = max(nSampleY2, nPrev2);
				for ( int _y = nBottom; _y <= nTop; _y++)
					column[_y] = clr2;
				nPrev2 = nSampleY2;
			}
		}

		if ( en1 )
		{
			if ( !bLines )
				column[nSampleY1] = clr1;
			else
			{
				if ( nPrev1 == -1 )
					nPrev1 = nSampleY1;
				int nBottom = min(nSampleY1, nPrev1);
				int nTop = max(nSampleY1, nPrev1);
				for ( int _y = nBottom; _y <= nTop; _y++)
					column[_y] = clr1;
				nPrev1 = nSampleY1;
			}
		}

		if ( enmath )
		{
			int chm = MathCalc( Sample.value );
			CUtils::Clamp<int>( chm, 0, 255 );
			int y = (chm*(DivsY*BlkY))>>8;

			if ( !bLines )
				column[y] = clrm;
			else
			{
				if ( nPrevm == -1 )
					nPrevm = y;
				for ( int _y = min(y, nPrevm); _y <= max(y, nPrevm); _y++)
					column[_y] = clrm;
				nPrevm = y;
			}
		}

		if ( bTrigger && (x & 1) == 1 )
		{
			ui16 y = (Settings.Trig.nLevel*(DivsY*BlkY))>>8;
			column[y] = RGB565(404040);
		}
		if ( nMarkerY1 > 0 )
			column[nMarkerY1] = Settings.MarkY1.u16Color;
		if ( nMarkerY2 > 0 )
			column[nMarkerY2] = Settings.MarkY2.u16Color;
 
		if ( x <= nCut )
		{	
			BIOS::LCD::BufferBegin(CRect(m_rcClient.left + x, m_rcClient.top, m_rcClient.left + x + 1, m_rcClient.top + DivsY*BlkY));
			BIOS::LCD::BufferWrite( column, DivsY*BlkY );
			BIOS::LCD::BufferEnd();
		}
		else
		{
			if ( nCutTop > 0 )
			{
				BIOS::LCD::BufferBegin( CRect(m_rcClient.left + x, m_rcClient.top + nCutTop, m_rcClient.left + x + 1, m_rcClient.top + nCutTop + DivsY*BlkY - nCutTop));
				BIOS::LCD::BufferWrite( column, DivsY*BlkY - nCutTop);
				BIOS::LCD::BufferEnd();
			}
			if ( nFirstTop > 0 )
			{
				BIOS::LCD::BufferBegin( CRect(m_rcClient.left + x, m_rcClient.top, m_rcClient.left + x + 1, m_rcClient.top + nFirstTop));
				BIOS::LCD::BufferWrite( column + DivsY*BlkY - nFirstTop, nFirstTop );
				BIOS::LCD::BufferEnd();
			}
		}
	}
}

void CWndOscGraph::SetupSelection( bool& bSelection, int& nMarkerT1, int& nMarkerT2 )
{
	bSelection = ( Settings.MarkT1.Mode == CSettings::Marker::_On && 
		Settings.MarkT2.Mode == CSettings::Marker::_On );
	
	if ( !bSelection )
		return;
	if ( Settings.MarkT1.nValue >= Settings.MarkT2.nValue )
		return;

	nMarkerT1 = Settings.MarkT1.nValue;
	nMarkerT2 = Settings.MarkT2.nValue;
}

void CWndOscGraph::SetupMarkers( CSettings::Calibrator::FastCalc& Ch1fast, CSettings::Calibrator::FastCalc& Ch2fast,
	int& nMarkerT1, int& nMarkerT2, int& nMarkerY1, int& nMarkerY2 )
{
	if ( Settings.MarkT1.Mode == CSettings::Marker::_On )
	{
		nMarkerT1 = Settings.MarkT1.nValue;
	}
	if ( Settings.MarkT2.Mode == CSettings::Marker::_On )
	{
		nMarkerT2 = Settings.MarkT2.nValue;
	}
	if ( Settings.MarkY1.Mode == CSettings::Marker::_On )
	{
		nMarkerY1 = Settings.MarkY1.nValue;
		if ( Settings.MarkY1.Source == CSettings::Marker::_CH1 )
		{
			nMarkerY1 = Settings.CH1Calib.Correct( Ch1fast, nMarkerY1 );
		} else
		{
			nMarkerY1 = Settings.CH2Calib.Correct( Ch2fast, nMarkerY1 );
		}
		nMarkerY1 = (nMarkerY1*(DivsY*BlkY))>>8;
	}
	if ( Settings.MarkY2.Mode == CSettings::Marker::_On )
	{
		nMarkerY2 = Settings.MarkY2.nValue;
		if ( Settings.MarkY2.Source == CSettings::Marker::_CH1 )
		{
			nMarkerY2 = Settings.CH1Calib.Correct( Ch1fast, nMarkerY2 );
		} else
		{
			nMarkerY2 = Settings.CH2Calib.Correct( Ch2fast, nMarkerY2 );
		}
		nMarkerY2 = (nMarkerY2*(DivsY*BlkY))>>8;
	}
	if ( Settings.MarkY1.Mode == CSettings::Marker::_Auto )
	{
		nMarkerY1 = Sampler::GetAt( Settings.MarkT1.nValue );
		if ( Settings.MarkY1.Source == CSettings::Marker::_CH1 )
		{
			nMarkerY1 = (ui8)((nMarkerY1) & 0xff);
			Settings.MarkY1.nValue = nMarkerY1;
			nMarkerY1 = Settings.CH1Calib.Correct( Ch1fast, nMarkerY1 );
		} else
		{
			nMarkerY1 = (ui8)((nMarkerY1>>8) & 0xff);
			Settings.MarkY1.nValue = nMarkerY1;
			nMarkerY1 = Settings.CH2Calib.Correct( Ch2fast, nMarkerY1 );
		}
		nMarkerY1 = (nMarkerY1*(DivsY*BlkY))>>8;
	}
	if ( Settings.MarkY2.Mode == CSettings::Marker::_Auto )
	{
		nMarkerY2 = Sampler::GetAt( Settings.MarkT2.nValue );
		if ( Settings.MarkY2.Source == CSettings::Marker::_CH1 )
		{
			nMarkerY2 = (ui8)((nMarkerY2) & 0xff);
			Settings.MarkY2.nValue = nMarkerY2;
			nMarkerY2 = Settings.CH1Calib.Correct( Ch1fast, nMarkerY2 );
		} else
		{
			nMarkerY2 = (ui8)((nMarkerY2>>8) & 0xff);
			Settings.MarkY2.nValue = nMarkerY2;
			nMarkerY2 = Settings.CH2Calib.Correct( Ch2fast, nMarkerY2 );
		}
		nMarkerY2 = (nMarkerY2*(DivsY*BlkY))>>8;
	}
	if ( nMarkerY1 >= DivsY*BlkY )
		nMarkerY1 = -1;
	if ( nMarkerY2 >= DivsY*BlkY )
		nMarkerY2 = -1;
}

void CWndOscGraph::GetCurrentRange(int& nBegin, int& nEnd)
{
	nBegin = Settings.Time.Shift;
	nEnd = nBegin + m_rcClient.Width();
	nEnd = min( nEnd, (int)BIOS::ADC::NumSamples );
}

// Move to better place

void CWndOscGraph::ClearAverage()
{
	memset(m_arrAverageBuf, 0, sizeof(m_arrAverageBuf));
}
void CWndOscGraph::ClearPersist()
{
	m_bPersistReset = true;
}

