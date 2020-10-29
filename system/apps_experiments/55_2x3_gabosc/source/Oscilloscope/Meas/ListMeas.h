#ifndef __LISTMEAS_H__
#define __LISTMEAS_H__

class CWndListMeas : public CListBox
{
public:
	CSettings::Measure* m_pMeas;

	CProviderEnum	m_proEnabled;
	CProviderEnum	m_proSource;
	CProviderEnum	m_proType;
	CProviderEnum	m_proRange;

	CLPItem			m_itmEnabled;
	CLPItem			m_itmSource;
	CLPItem			m_itmType;
	CLPItem			m_itmRange;

public:
	void Create( CSettings::Measure* pMeas, CWnd* pParent )
	{
		m_pMeas = pMeas;
		CListBox::Create( "Measure", WsVisible | WsModal, CRect(100, 30, 316, 160), RGB565(8080b0), pParent );

		m_proEnabled.Create( (const char**)CSettings::Measure::ppszTextEnabled,
			(NATIVEENUM*)&pMeas->Enabled, CSettings::Measure::_MaxEnabled );

		m_proSource.Create( (const char**)CSettings::Measure::ppszTextSource,
			(NATIVEENUM*)&pMeas->Source, CSettings::Measure::_MaxSource );

		m_proType.Create( (const char**)CSettings::Measure::ppszTextType,
			(NATIVEENUM*)&pMeas->Type, CSettings::Measure::_MaxType );

		m_proRange.Create( (const char**)CSettings::Measure::ppszTextRange,
			(NATIVEENUM*)&pMeas->Range, CSettings::Measure::_MaxRange );

		m_itmEnabled.Create( "Enable", CWnd::WsVisible, &m_proEnabled, this );
		m_itmSource.Create( "Source", CWnd::WsVisible, &m_proSource, this );
		m_itmType.Create( "Type", CWnd::WsVisible, &m_proType, this );
		m_itmRange.Create( "Range", CWnd::WsVisible, &m_proRange, this );
	}
};

#endif