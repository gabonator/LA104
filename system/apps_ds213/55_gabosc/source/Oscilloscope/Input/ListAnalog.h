#ifndef __LISTAINPUT_H__
#define __LISTAINPUT_H__

class CWndListAInput : public CListBox
{
public:
	CSettings::AnalogChannel* m_pInfo;
	
	CProviderEnum	m_proEnabled;
	CProviderEnum	m_proCoupling;
	CProviderEnum	m_proResolution;
	CProviderEnum	m_proProbe;
	CProviderNum	m_proPosition;
	CProviderColor	m_proColour;
	CProviderRgb	m_proRed, m_proGrn, m_proBlu;

	CLPItem			m_itmEnabled;
	CLPItem			m_itmCoupling;
	CLPItem			m_itmResolution;
	CLPItem			m_itmProbe;
	CLPItem			m_itmPosition;
	CLPItem			m_itmColour;
	CLPItem			m_itmRed, m_itmGrn, m_itmBlu;

public:
	void Create( CSettings::AnalogChannel* pInfo, CWnd* pParent )
	{
		m_pInfo = pInfo;
		CListBox::Create( pInfo->pszFullName, WsVisible | WsModal, CRect(120, 30, 319, 201), pInfo->u16Color, pParent );


		m_proEnabled.Create( (const char**)CSettings::AnalogChannel::ppszTextEnabled,
			(NATIVEENUM*)&pInfo->Enabled, CSettings::AnalogChannel::_EnabledMax );
		m_proCoupling.Create( (const char**)CSettings::AnalogChannel::ppszTextCoupling,
			(NATIVEENUM*)&pInfo->Coupling, CSettings::AnalogChannel::_CouplingMax );
		m_proResolution.Create( (const char**)CSettings::AnalogChannel::ppszTextResolution,
			(NATIVEENUM*)&pInfo->Resolution, CSettings::AnalogChannel::_ResolutionMax );
		m_proProbe.Create( (const char**)CSettings::AnalogChannel::ppszTextProbe,
			(NATIVEENUM*)&pInfo->Probe, CSettings::AnalogChannel::_ProbeMax );
		m_proPosition.Create( (si16*)&pInfo->u16Position, -20, 300 );
		m_proColour.Create( &pInfo->u16Color );
		m_proRed.Create( &pInfo->u16Color, CProviderRgb::Red );
		m_proGrn.Create( &pInfo->u16Color, CProviderRgb::Green );
		m_proBlu.Create( &pInfo->u16Color, CProviderRgb::Blue );

		m_itmEnabled.Create( "Enabled", CWnd::WsVisible, &m_proEnabled, this );	
		
		m_itmCoupling.Create( "Coupling", CWnd::WsVisible, &m_proCoupling, this );
		m_itmResolution.Create( "Resolution", CWnd::WsVisible, &m_proResolution, this );
		m_itmProbe.Create( "Probe", CWnd::WsVisible, &m_proProbe, this );
		m_itmPosition.Create( "V. Position", CWnd::WsVisible, &m_proPosition, this );
		m_itmColour.Create( "Colour", CWnd::WsVisible | CWnd::WsNoActivate, &m_proColour, this );
		m_itmRed.Create(" - Red", CWnd::WsVisible, &m_proRed, this);
		m_itmGrn.Create(" - Green", CWnd::WsVisible, &m_proGrn, this);
		m_itmBlu.Create(" - Blue", CWnd::WsVisible, &m_proBlu, this);
	}
};

#endif