#ifndef __ITEMGENFREQUENCY_H__
#define __ITEMGENFREQUENCY_H__

#define Hz *1.0f
#define KHz *1000.0f
#define MHz *1000000.0f

class CItemFrequency : public CWndMenuItem
{
public:
	virtual void Create(const char* pszId, ui16 clr, ui8 rows, CWnd *pParent) 
	{
		CWndMenuItem::Create(pszId, clr, rows, pParent);
	}
/*
	void UpdateAdc()
	{
		float fVolt = m_nVolt / 10.0f;
		int nAdc = Settings.DacCalib.Get( fVolt );
		BIOS::GEN::ConfigureDc( nAdc );
	}
*/
	virtual void OnKey(int nKey) override
	{	
		if ( !( nKey == BIOS::KEY::Left ) &&
			!( nKey == BIOS::KEY::Right ) )
		{
			CWndMenuItem::OnKey( nKey );
			return;
		}

		float fFreq = GetFrequency();
		float fOldFreq = fFreq;
		float fStep = 1.0f;
		_ASSERT( fFreq >= 0 );
		if ( fFreq <= 100 Hz)
			fStep = 1 Hz;
		else if ( fFreq <= 1 KHz )
			fStep = 10 Hz;
		else if ( fFreq <= 10 KHz )
			fStep = 100 Hz;
		else if ( fFreq <= 100 KHz )
			fStep = 1 KHz;
		else if ( fFreq <= 1 MHz )
			fStep = 10 KHz;
		else if ( fFreq <= 10 MHz )
			fStep = 100 KHz;
		else if ( fFreq <= 100 MHz )
			fStep = 1 MHz;

		if ( nKey == BIOS::KEY::Left )
		{
			if ( fFreq > fStep )
				fFreq -= fStep;
			if ( fFreq <= 0.0f )
				fFreq = 1.0f;
		}
		if ( nKey == BIOS::KEY::Right )
		{
			if ( fFreq < 20 MHz )
				fFreq += fStep;
			if ( fFreq <= 0.0f )
				fFreq = 1.0f;
		}

		fFreq = ((int)(fFreq / fStep)) * fStep;
		if ( fFreq < 1.0f )
			fFreq = 1.0f;

		if ( fFreq != fOldFreq )
		{
			bool bSet = SetFrequency( fFreq );
			float fNewFreq = GetFrequency();

			if ( fFreq > fOldFreq && fNewFreq <= fOldFreq )
				bSet = false;
			if ( fFreq < fOldFreq && fNewFreq >= fOldFreq )
				bSet = false;

			if ( !bSet )
			{
				if ( nKey == BIOS::KEY::Left )
				{
					if ( Settings.Gen.nArr < 0x10000 )
					{
						Settings.Gen.nArr++;
						if ( Settings.Gen.Wave == CSettings::Generator::_Square )
							CCoreGenerator::ConfigureSq( Settings.Gen.nPsc, Settings.Gen.nArr, Settings.Gen.nCcr );
						else
							CCoreGenerator::ConfigureWaveRate( Settings.Gen.nArr );
					}
				} else {
					if ( Settings.Gen.nArr > 1 )
					{
						Settings.Gen.nArr--;
						if ( Settings.Gen.Wave == CSettings::Generator::_Square )
							CCoreGenerator::ConfigureSq( Settings.Gen.nPsc, Settings.Gen.nArr, Settings.Gen.nCcr );
						else
							CCoreGenerator::ConfigureWaveRate( Settings.Gen.nArr );
					}
				}
			}
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), 0);
		}
		CWndMenuItem::OnKey( nKey );
	}

	float GetFrequency()
	{
		#define CPUCLOCK (72 MHz)
		#define MHz *1000000.0f

		if ( Settings.Gen.Wave == CSettings::Generator::_Square )
		{
			float fFreq = CPUCLOCK / ( Settings.Gen.nArr + 1 ) / ( Settings.Gen.nPsc + 1 );
			return fFreq;
		}
		if ( Settings.Gen.nSamples <= 0 ) 
			return 0;
		float fFreq = CPUCLOCK / 20 / ( Settings.Gen.nArr + 1 ) / Settings.Gen.nSamples; 
		return fFreq;
	}

	bool SetFrequency(float fFreq)
	{
		int nPsc = 20;
		int nSamples = Settings.Gen.nSamples;
		if ( Settings.Gen.Wave == CSettings::Generator::_Square )
		{
			nSamples = 1;
			// calculate PSC
			nPsc = 1;
			if ( fFreq <= 10 Hz )
				nPsc = 240;
			else if ( fFreq <= 2 KHz )
				nPsc = 180;
			else if ( fFreq <= 20 KHz )
				nPsc = 18;
			Settings.Gen.nPsc = nPsc - 1;
		}

		int nArr = (int)((CPUCLOCK) / ( nPsc * fFreq * nSamples )) - 1;

		if ( nArr <= 1 )
			nArr = 1;
		if ( Settings.Gen.Wave == CSettings::Generator::_Square && nArr < 8 )
			nArr = 8;

		bool bChanged = nArr != Settings.Gen.nArr;
		Settings.Gen.nArr = nArr;
		CCoreGenerator::SetDuty(Settings.Gen.nDuty);

		//BIOS::GEN::ConfigureWaveRate( Settings.Gen.nArr );

		#undef CPUCLOCK
		#undef MHz
		return bChanged;
	}

	virtual void OnPaint()
	{
		CWndMenuItem::OnPaint();
		float fFrequency = GetFrequency();
		fFrequency = (float)(int)fFrequency;
		BIOS::LCD::Print( m_rcClient.left + 12 + 16, m_rcClient.top + 16, RGB565(000000), RGBTRANS, 
			CUtils::FormatFrequency(fFrequency) );
    // rounding error !?
	}

};

#undef Hz
#undef KHz
#undef MHz

#endif
