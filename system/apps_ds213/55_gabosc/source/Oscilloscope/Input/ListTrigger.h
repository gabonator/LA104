#ifndef __LISTTRIGGER_H__
#define __LISTTRIGGER_H__

class CWndListTrigger : public CListBox
{
public:
	CProviderEnum	m_proSync;
	CProviderEnum	m_proType;
	CProviderEnum	m_proSource;
	CProviderNum	m_proLevel;
	CProviderNum	m_proTime;
	CProviderNum	m_proHoldoff;
	CProviderNum	m_proTrigPosition;

	CLPItem			m_itmSync;
	CLPItem			m_itmType;
	CLPItem			m_itmSource;
	CLPItem			m_itmLevel;
	CLPItem			m_itmTime;
	CLPItem			m_itmHoldoff;
	CLPItem			m_itmTrigPosition;
	// DSO does not allow to change the time offset

public:
	void Create( CWnd* pParent )
	{
		CListBox::Create( "Trigger", WsVisible | WsModal, CRect(120, 55, 319, 201), RGB565(404040), pParent );

		m_proSync.Create( (const char**)CSettings::Trigger::ppszTextSync,
			(NATIVEENUM*)&Settings.Trig.Sync, CSettings::Trigger::_SyncMax );
		m_proType.Create( (const char**)CSettings::Trigger::ppszTextType,
			(NATIVEENUM*)&Settings.Trig.Type, CSettings::Trigger::_TypeMax );
		m_proSource.Create( (const char**)CSettings::Trigger::ppszTextSource,
			(NATIVEENUM*)&Settings.Trig.Source, CSettings::Trigger::_SourceMax );
		m_proLevel.Create( &Settings.Trig.nLevel, 0, 255 );
		m_proTime.Create( &Settings.Trig.nTime, 0, 4096 );
		m_proHoldoff.Create( &Settings.Trig.nHoldOff, 0, 4096 );
		m_proTrigPosition.Create( &Settings.Trig.nPosition, 0, 4096 );

		m_itmSync.Create( "Mode", CWnd::WsVisible, &m_proSync, this );
		m_itmType.Create( "Type", CWnd::WsVisible, &m_proType, this );
		m_itmSource.Create( "Source", CWnd::WsVisible, &m_proSource, this );
		m_itmLevel.Create( "Threshold", CWnd::WsVisible, &m_proLevel, this );
		m_itmTime.Create( "Time", CWnd::WsVisible, &m_proTime, this );
		m_itmHoldoff.Create( "HoldOff", CWnd::WsVisible, &m_proHoldoff, this );
		m_itmTrigPosition.Create( "Trig. pos", CWnd::WsVisible, &m_proTrigPosition, this );
	}
};

#endif