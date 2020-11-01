#ifndef __LISTDINPUT_H__
#define __LISTDINPUT_H__

class CWndListDInput : public CListBox
{
public:
	CSettings::DigitalChannel* m_pInfo;
	
	CProviderEnum	m_proEnabled;
	CProviderEnum	m_proPolarity;
	CProviderNum	m_proPosition;
	CProviderColor	m_proColour;
	CProviderRgb	m_proRed, m_proGrn, m_proBlu;

	CLPItem			m_itmEnabled;
	CLPItem			m_itmPolarity;
	CLPItem			m_itmPosition;
	CLPItem			m_itmColour;
	CLPItem			m_itmRed, m_itmGrn, m_itmBlu;

public:
	void Create( CSettings::DigitalChannel* pInfo, CWnd* pParent )
	{
		m_pInfo = pInfo;
		CListBox::Create( pInfo->pszFullName, WsVisible | WsModal, CRect(120, 30, 319, 201), pInfo->u16Color, pParent );

		m_proEnabled.Create( (const char**)CSettings::AnalogChannel::ppszTextEnabled,
			(NATIVEENUM*)&pInfo->Enabled, CSettings::AnalogChannel::_EnabledMax );
		m_proPolarity.Create( (const char**)CSettings::DigitalChannel::ppszTextPolarity,
			(NATIVEENUM*)&pInfo->Polarity, CSettings::DigitalChannel::_PolarityMax );
		
		m_proPosition.Create( (si16*)&pInfo->u16Position, 0, 255 );
		m_proColour.Create( &pInfo->u16Color );
		m_proRed.Create( &pInfo->u16Color, CProviderRgb::Red );
		m_proGrn.Create( &pInfo->u16Color, CProviderRgb::Green );
		m_proBlu.Create( &pInfo->u16Color, CProviderRgb::Blue );

		m_itmEnabled.Create( "Enabled", CWnd::WsVisible, &m_proEnabled, this );
		m_itmPolarity.Create( "Polarity", CWnd::WsVisible, &m_proPolarity, this );
		m_itmPosition.Create( "V. Position", CWnd::WsVisible, &m_proPosition, this );
		m_itmColour.Create( "Colour", CWnd::WsVisible | CWnd::WsNoActivate, &m_proColour, this );
		m_itmRed.Create(" - Red", CWnd::WsVisible, &m_proRed, this);
		m_itmGrn.Create(" - Green", CWnd::WsVisible, &m_proGrn, this);
		m_itmBlu.Create(" - Blue", CWnd::WsVisible, &m_proBlu, this);
	}
};

#endif