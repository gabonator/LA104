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

	virtual void OnKey(int nKey) override
	{
		if ( nKey == BIOS::KEY::Left )
		{
			switch( (int)Settings.Gen.Wave ){
				case CSettings::Generator::_Dc:
					break;
				case CSettings::Generator::_SinHq:
					Settings.Gen.Wave = CSettings::Generator::_Dc;
					break;
				case CSettings::Generator::_SinLq:
					Settings.Gen.Wave = CSettings::Generator::_SinHq;
					break;
				case CSettings::Generator::_Tri:
					Settings.Gen.Wave = CSettings::Generator::_SinLq;
					break;
				case CSettings::Generator::_Saw:
					Settings.Gen.Wave = CSettings::Generator::_Tri;
					break;
				case CSettings::Generator::_Square:
					Settings.Gen.Wave = CSettings::Generator::_Saw;
					break;
				case CSettings::Generator::_Cardiac:
					Settings.Gen.Wave = CSettings::Generator::_Square;
					break;
				case CSettings::Generator::_Volatile:
					Settings.Gen.Wave = CSettings::Generator::_Cardiac;
					break;
				default:
					break;
			}
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), 0);
		}
		if ( nKey == BIOS::KEY::Right )
		{
			switch( (int)Settings.Gen.Wave ){
				case CSettings::Generator::_Dc:
					Settings.Gen.Wave = CSettings::Generator::_SinHq;
					break;
				case CSettings::Generator::_SinHq:
					Settings.Gen.Wave = CSettings::Generator::_SinLq;
					break;
				case CSettings::Generator::_SinLq:
					Settings.Gen.Wave = CSettings::Generator::_Tri;
					break;
				case CSettings::Generator::_Tri:
					Settings.Gen.Wave = CSettings::Generator::_Saw;
					break;
				case CSettings::Generator::_Saw:
					Settings.Gen.Wave = CSettings::Generator::_Square;
					break;
				case CSettings::Generator::_Square:
					Settings.Gen.Wave = CSettings::Generator::_Cardiac;
					break;
				case CSettings::Generator::_Cardiac:
					Settings.Gen.Wave = CSettings::Generator::_Volatile;
					break;
				case CSettings::Generator::_Volatile:
					break;
				default:
					break;
			}
			Invalidate();
			SendMessage(m_pParent, ToWord('i', 'u'), 0);
		}
		CWndMenuItem::OnKey( nKey );
	}
};

#endif
