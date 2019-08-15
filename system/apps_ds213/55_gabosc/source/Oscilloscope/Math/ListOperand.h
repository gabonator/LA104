#ifndef __LISTOPERAND_H__
#define __LISTOPERAND_H__

class CWndListOperand : public CListBox
{
public:
	CSettings::MathOperand* m_pOper;

	CProviderEnum	m_proType;
	CLPItem			m_itmType;

	CProviderNumAny<int>	m_proScale;
	CLPItem			m_itmScale;

	CProviderNumAny<int>	m_proOffset;
	CLPItem			m_itmOffset;

public:
	void Create( CSettings::MathOperand* pOper, CWnd* pParent )
	{
		m_pOper = pOper;
		CListBox::Create( pOper->strName, WsVisible | WsModal, CRect(100, 30, 300, 140), RGB565(606060), pParent );

		m_proType.Create( (const char**)CSettings::MathOperand::ppszTextType,
			(NATIVEENUM*)&pOper->Type, CSettings::MathOperand::_TypeMax );
		m_itmType.Create( "Type", CWnd::WsVisible, &m_proType, this );

		m_proScale.Create(&pOper->nScale, -200, 200);
		m_itmScale.Create("Scale (%)", CWnd::WsVisible, &m_proScale, this);

		m_proOffset.Create(&pOper->nConstant, -255, 255);
		m_itmOffset.Create("Constant", CWnd::WsVisible, &m_proOffset, this);
		
		// update visibility
		bool bShowOfs = ( m_pOper->Type == CSettings::MathOperand::_Constant ||
			  m_pOper->Type == CSettings::MathOperand::_Fx );
		bool bShowSca = (!bShowOfs ) || (m_pOper->Type == CSettings::MathOperand::_Fx );
		m_itmOffset.ShowWindow( bShowOfs );
		m_itmScale.ShowWindow( bShowSca );
	}

	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
		if ( code == ToWord('u', 'p') && pSender == &m_itmType )
		{
			bool bChanged = false;
			bool bShowOfs = ( m_pOper->Type == CSettings::MathOperand::_Constant ||
				m_pOper->Type == CSettings::MathOperand::_Fx );
			bool bShowSca = (!bShowOfs) || (m_pOper->Type == CSettings::MathOperand::_Fx );
			if ( bShowOfs != m_itmOffset.IsVisible() )
			{
				m_itmOffset.ShowWindow( bShowOfs );
				bChanged = true;
			}
			if ( bShowSca != m_itmScale.IsVisible() )
			{
				m_itmScale.ShowWindow( bShowSca );
				bChanged = true;
			}
			if ( bChanged )
				Invalidate();
		}
		CListBox::OnMessage( pSender, code, data );
	}

};

#endif
