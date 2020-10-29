#pragma once
#include <Framework.h>

/*
class CFirFilter
{
public:
	void Reset() {}
	void operator << (int o) {o=o;}
	void operator >> (int &o) {o=0;}
};
*/

class CFirFilter
{
public:
	int m_nRingPos;
	int m_nCounter; // redundant
	ui8 m_nLength;

	CFirFilter();

	// shared memory functions
	ui8& GetFirLength();
	si8& GetFirCoefficient(int nAt);
	ui8& GetRingBuffer(int nAt);
	void PushRingBuffer(int n);

public:
	bool LoadFilter(const char* strName);
	void CreateSampleFilter();

	void Reset();
	void operator << (int o);
	void operator >> (int &o);
};
