#ifndef __LISTMARKER_H__
#define __LISTMARKER_H__

class CWndListMarker : public CListBox
{
public:
	CSettings::Marker* m_pMarker;

	CProviderEnum	m_proMode;
	CProviderEnum	m_proSource;
	CProviderEnum	m_proDisplay;
	CProviderNumAny<int>	m_proValueRaw;

	CLPItem			m_itmMode;
	CLPItem			m_itmSource;
	CLPItem			m_itmDisplay;
	CLPItem			m_itmValueRaw;
	CLStaticItem	m_itmValue;

	CSettings::Marker::EFind m_modeFind;
	CProviderEnum	m_proFind;
	CLPItem			m_itmFind;

public:

	CWndListMarker()
	{
		m_modeFind = CSettings::Marker::_MaxFind;
	}

	void Create( CSettings::Marker* pMarker, CWnd* pParent )
	{
		m_pMarker = pMarker;
		CListBox::Create( pMarker->strFullName, WsVisible | WsModal, CRect(100, 30, 300, 201), pMarker->u16Color, pParent );

		if ( pMarker->Type == CSettings::Marker::_Time )
		{
			m_proMode.Create( (const char**)CSettings::Marker::ppszTextMode,
				(NATIVEENUM*)&pMarker->Mode, CSettings::Marker::_ModeMaxTime );
		} else
		{
			m_proMode.Create( (const char**)CSettings::Marker::ppszTextMode,
				(NATIVEENUM*)&pMarker->Mode, CSettings::Marker::_ModeMaxVoltage );
			m_proSource.Create( (const char**)CSettings::Marker::ppszTextSource,
				(NATIVEENUM*)&pMarker->Source, CSettings::Marker::_SourceMax );
			m_proFind.Create( (const char**)CSettings::Marker::ppszTextFind,
				(NATIVEENUM*)&m_modeFind, CSettings::Marker::_FindMax );
		}

		m_proDisplay.Create( (const char**)CSettings::Marker::ppszTextDisplay,
			(NATIVEENUM*)&pMarker->Display, CSettings::Marker::_DisplayMax );

		m_proValueRaw.Create( &pMarker->nValue, 0, 1000 );

		m_itmMode.Create( "Mode", CWnd::WsVisible, &m_proMode, this );	
		if ( pMarker->Type == CSettings::Marker::_Voltage )
			m_itmSource.Create( "Source", CWnd::WsVisible, &m_proSource, this );
		m_itmDisplay.Create( "Display", CWnd::WsVisible, &m_proDisplay, this );
		m_itmValueRaw.Create( "Raw value", CWnd::WsVisible, &m_proValueRaw, this );
		m_itmValue.Create( "Real value    0.000", CWnd::WsVisible | CWnd::WsNoActivate, this);
		if ( pMarker->Type == CSettings::Marker::_Voltage )
			m_itmFind.Create( "Find", CWnd::WsVisible, &m_proFind, this );
	}
};

#endif