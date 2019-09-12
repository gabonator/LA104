#ifndef __ITEMGENWAVEFORM_H__
#define __ITEMGENWAVEFORM_H__

class CItemGenWaveform : public CItemProvider
{
	CProviderEnum	m_proWave;

public:
	void Create( CWnd* pParent )
	{
		m_proWave.Create( (const char**)CSettings::Generator::ppszTextWave,
			(NATIVEENUM*)&Settings.Gen.Wave, CSettings::Generator::_WaveMax );
		CItemProvider::Create( &m_proWave, "Waveform", RGB565(b00040), pParent );
	}
};

#endif