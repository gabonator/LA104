#include "FirFilter.h"

#include <Framework.h>
#include <MainWnd.h>


CFirFilter::CFirFilter()
{
	GetFirLength() = 0;
}	

// shared memory functions
ui8& CFirFilter::GetFirLength()
{
	return m_nLength;
}

si8& CFirFilter::GetFirCoefficient(int nAt)
{
	BIOS::ADC::TSample& ts1 = (BIOS::ADC::TSample&)Sampler::GetAt( nAt + 1024 + 256 );
	return ((si8*)&ts1)[3];

}

ui8& CFirFilter::GetRingBuffer(int nAt)
{
	BIOS::ADC::TSample& ts1 = (BIOS::ADC::TSample&)Sampler::GetAt( nAt + 1024 );
	return ((ui8*)&ts1)[3];
}

void CFirFilter::PushRingBuffer(int n)
{
	CUtils::Clamp<int>(n, 0, 255);
	ui8 n8 = (ui8)n;
	GetRingBuffer(m_nRingPos) = n8;
	if ( ++m_nRingPos == GetFirLength() )
		m_nRingPos = 0;
}

void CFirFilter::CreateSampleFilter()
{
	GetFirLength() = 2;
	GetFirCoefficient(0) = +1;
	GetFirCoefficient(1) = -1;
	const char* strContent = 
		"# DS203 FIR filter file\n"
		"# Name: Simple derivation\n"
		"#\n"
		"# Filter length\n"
		"# 1..255\n"
		"2\n"
		"# Filter coefficients\n"
		"# in range <-128, 127>\n"
		"1\n"
		"-1\n";

	CBufferedWriter f;
	f.Open((PSTR)"FILTER  TXT");
	f << strContent;
	f.Close();
	MainWnd.m_wndMessage.Show( &MainWnd, "Information", "FIR Filter 'Filter.txt' was created", RGB565(ffff00) );
}

bool CFirFilter::LoadFilter(const char* strName)
{
	CBufferedReader f;
	if ( !f.Open( (PSTR)strName ) )
		return false;
	char line[32];

	do
		f >> line;
	while ( line[0] == '#' );

	int nCount = CUtils::atoi( line );
	_ASSERT( nCount >= 1 && nCount < 256 );
	GetFirLength() = nCount;
	for ( int i=0; i < GetFirLength(); i++ )
	{
		do
			f >> line;
		while ( line[0] == '#' );

		int nCoef = CUtils::atoi( line );
		_ASSERT( nCoef >= -128 && nCoef < 127 );
		GetFirCoefficient(i) = nCoef;
	}
	f.Close();
	return true;
}

void CFirFilter::Reset()
{
	m_nRingPos = 0;
	m_nCounter = 0;
	if ( GetFirLength() == 0 )
	{
		if ( !LoadFilter("FILTER  TXT") )
			CreateSampleFilter();
	}
	for (int i=0; i<GetFirLength(); i++)
		PushRingBuffer(0);
}

void CFirFilter::operator << (int o)
{
	m_nCounter++;
	PushRingBuffer(o);		
}

void CFirFilter::operator >> (int &o)
{
	o = 0;
	if ( m_nCounter < m_nLength )
		return;

	int nFirBase = m_nRingPos+m_nLength-1;
	int nFirOfs = (nFirBase-1) % m_nLength;
	for ( int i = 0; i < m_nLength; i++ )
	{
		o += ((int)GetRingBuffer( nFirOfs )) * GetFirCoefficient(i);
		if ( ++nFirOfs == m_nLength )
			nFirOfs = 0;
	}
}
