#pragma once

class CProviderEnum : public CValueProvider
{
	const char ** m_ppszText;
	NATIVEENUM* m_pVal;
	NATIVEENUM m_Max;

public:
	void Create(const char** ppszText, NATIVEENUM* val, NATIVEENUM max)
	{
		m_ppszText = ppszText;
		m_pVal = val;
		m_Max = max;
	}

	virtual VPNavigate operator +(si8 d)
	{
		return ( *m_pVal + d >= 0 && *m_pVal + d <= m_Max ) ? Yes : No; 
	}

	virtual void operator++(int)
	{
		(*m_pVal)++;
	}

	virtual void operator--(int)
	{
		(*m_pVal)--;
	}

	virtual void OnPaint(const CRect& rcRect, ui8 bFocus)
	{
		ui16 clr = bFocus ? RGB565(ffffff) : RGB565(000000);
		ui16 x = rcRect.left;
		x += BIOS::LCD::Print( x, rcRect.top, clr, RGBTRANS, 
			((const char**)m_ppszText)[ *m_pVal ]);
	}	

	virtual ui16 GetWidth()
	{
		return (ui16)strlen(((const char**)m_ppszText)[ *m_pVal ]) << 3;
	}

	virtual void Set(ui32 val)
	{
		*m_pVal = (NATIVEENUM)val;
	}

	virtual ui32 Get()
	{
		return (ui32)*m_pVal;
	}
};

class CProviderNum : public CValueProvider
{
	si16 *m_pVal;
	si16 m_nMin;
	si16 m_nMax;

public:
	void Create(si16* val, si16 _min, si16 _max)
	{
		m_pVal = val;
		m_nMin = _min;
		m_nMax = _max;
	}

	virtual VPNavigate operator +(si8 d)
	{
		si16 n = (*m_pVal) + d;
		return ( n >= m_nMin && n <= m_nMax ) ? Yes : No; 
	}

	virtual void operator++(int)
	{
		(*m_pVal)++;
	}

	virtual void operator--(int)
	{
		(*m_pVal)--;
	}

	virtual void OnPaint(const CRect& rcRect, ui8 bFocus)
	{
		ui16 clr = bFocus ? RGB565(ffffff) : RGB565(000000);
		_ASSERT( m_pVal );
		if ( *m_pVal < m_nMin || *m_pVal > m_nMax )
			BIOS::LCD::Print( rcRect.left, rcRect.top, clr, RGBTRANS, 
				"*" );
		else
			BIOS::LCD::Print( rcRect.left, rcRect.top, clr, RGBTRANS, 
				CUtils::itoa( *m_pVal ) );
	}	

	virtual ui16 GetWidth()
	{
		return (ui16)strlen( CUtils::itoa( *m_pVal ) )<<3;
	}
	
	void SetMin( si16 nMin )
	{
		m_nMin = nMin;
	}
	void SetMax( si16 nMax )
	{
		m_nMax = nMax;
	}
};

template <class T>
class CProviderNumAny : public CValueProvider
{
	T *m_pVal;
	T m_nMin;
	T m_nMax;

public:
	void Create(T* val, T _min, T _max)
	{
		m_pVal = val;
		m_nMin = _min;
		m_nMax = _max;
	}

	virtual VPNavigate operator +(si8 d)
	{
		T n = (*m_pVal) + d;
		return ( n >= m_nMin && n <= m_nMax ) ? Yes : No; 
	}

	virtual void operator++(int)
	{
		(*m_pVal)++;
	}

	virtual void operator--(int)
	{
		(*m_pVal)--;
	}

	virtual void OnPaint(const CRect& rcRect, ui8 bFocus)
	{
		ui16 clr = bFocus ? RGB565(ffffff) : RGB565(000000);
		_ASSERT( m_pVal );
		if ( *m_pVal < m_nMin || *m_pVal > m_nMax )
			BIOS::LCD::Print( rcRect.left, rcRect.top, clr, RGBTRANS, 
				"*" );
		else
			BIOS::LCD::Print( rcRect.left, rcRect.top, clr, RGBTRANS, 
				CUtils::itoa( *m_pVal ) );
	}	

	virtual ui16 GetWidth()
	{
		return (ui16)strlen( CUtils::itoa( *m_pVal ) )<<3;
	}
};

class CProviderColor : public CValueProvider
{
	ui16 *m_pVal;

public:
	void Create(ui16* pColor)
	{
		m_pVal = pColor;
	}

	virtual VPNavigate operator +(si8 d)
	{
		return Disabled; 
	}

	virtual void OnPaint(const CRect& rcRect, ui8 bFocus)
	{
		ui16 clr = *m_pVal;
		ui16 x = rcRect.left;
		x -= 8;
		x += BIOS::LCD::Draw( x, rcRect.top, clr, RGBTRANS, CShapes::sel_left );
		x += BIOS::LCD::Print( x, rcRect.top, RGBTRANS, clr, 
			CUtils::clrhex( clr ) );
		x += BIOS::LCD::Draw( x, rcRect.top, clr, RGBTRANS, CShapes::sel_right );
	}	
	
	virtual ui16 GetWidth()
	{
		return 3*8;
	}
};

class CProviderBtn : public CValueProvider
{
public:
	const char* m_pName;

public:
	void Create(const char* pName)
	{
		m_pName = pName;
	}

	virtual VPNavigate operator +(si8 d)
	{
		return Disabled;
	}

	virtual void OnPaint(const CRect& rcRect, ui8 bFocus)
	{
		ui16 clr = bFocus ? RGB565(ffffff) : RGB565(000000);

		ui16 x = rcRect.left;
		x -= 8;
		x += BIOS::LCD::Draw( x, rcRect.top, clr, RGBTRANS, CShapes::sel_left );
		x += BIOS::LCD::Print( x, rcRect.top, RGBTRANS, clr, m_pName );
		x += BIOS::LCD::Draw( x, rcRect.top, clr, RGBTRANS, CShapes::sel_right );
	}	
	
	virtual ui16 GetWidth()
	{
		return 0;
	}

	virtual ui32 Get() 
	{ 
		return 0; 
	}
};

class CProviderRgb : public CValueProvider
{
public:
	enum EComponent { Red, Green, Blue } m_Component;
	ui16* m_pColor;

public:
	void Create(ui16* pColor, EComponent Component)
	{
		m_pColor = pColor;
		m_Component = Component;
	}

	ui8 GetValue()
	{
		switch (m_Component)
		{
			case Red:
				return (*m_pColor & 0x1f) >> 1;
			case Green: 
				return ((*m_pColor>>5) & 0x3f) >> 2;
			case Blue: 
				return ((*m_pColor>>11) & 0x1f) >> 1;
		}
		_ASSERT(0);
		return 0;
	}

	void SetValue(ui8 n)
	{
		switch (m_Component)
		{
			case Red:
				*m_pColor &= ~0x1f;
				n = (n == 15) ? 31 : (n<<1);
				*m_pColor |= n;
				return;
			case Green: 
				*m_pColor &= ~(0x3f<<5);
				n = (n == 15) ? 63 : (n<<2);
				*m_pColor |= n << 5;
				return;
			case Blue: 
				*m_pColor &= ~(0x1f<<11);
				n = (n == 15) ? 31 : (n<<1);
				*m_pColor |= n << 11;
				return;
		}
		_ASSERT(0);
	}

	virtual VPNavigate operator +(si8 d)
	{
		if (d > 0 && GetValue() < 15)
			return Yes;
		if (d < 0 && GetValue() > 0)
			return Yes;
		return No; 
	}

	virtual void operator++(int)
	{
		ui8 val = GetValue();
		if ( val < 15 )
			SetValue(val+1);
	}

	virtual void operator--(int)
	{
		ui8 val = GetValue();
		if ( val > 0 )
			SetValue(val-1);
	}

	virtual void OnPaint(const CRect& rcRect, ui8 bFocus)
	{
		ui16 clr = bFocus ? RGB565(ffffff) : RGB565(000000);
		char temp[2];
		temp[0] = CUtils::tohex(GetValue());
		temp[1] = 0;

		BIOS::LCD::Print( rcRect.left, rcRect.top, clr, RGBTRANS, 
			temp );
	}
	
	virtual ui16 GetWidth()
	{
		return 8;
	}
};

/*

class CLEnumItem : public CListItem
{
	const char ** m_ppszText;
	NATIVEENUM* m_pVal;
	NATIVEENUM m_Max;

public:
	void Create(const char* pszId, const char** ppszText, NATIVEENUM* val, NATIVEENUM max, CWnd* pParent)
	{
		m_ppszText = ppszText;
		m_pVal = val;
		m_Max = max;

		CListItem::Create( pszId, WsVisible, pParent );
	}

	virtual void OnPaint()
	{
		ui16 clr = HasFocus() ? RGB565(ffffff) : RGB565(000000);
		//ui16 bgr = HasFocus() ? RGB565(ffffff) : RGB565(b0b0b0);
		ui16 clrleft = *m_pVal > 0 ? clr : RGB565(808080);
		ui16 clrright = *m_pVal < m_Max ? clr : RGB565(808080);
		
		CListItem::OnPaint();

		int x = m_rcClient.left+4;
		BIOS::Print( x, m_rcClient.top, clr, RGBTRANS, m_pszId);
	
		x = m_rcClient.left + 120;
		x -= 8;
		x += BIOS::Draw( x, m_rcClient.top, clrleft, RGBTRANS, more_left );
		x += BIOS::Print( x, m_rcClient.top, clr, RGBTRANS, 
			((const char**)m_ppszText)[ *m_pVal ]);
		x += BIOS::Draw( x, m_rcClient.top, clrright, RGBTRANS, more_right );
	}	

	virtual void OnKey(ui16 nKey)
	{
		if ( nKey & BIOS::KeyLeft && *m_pVal > 0 )
		{
			(*m_pVal)--;
			Invalidate();
			SendMessage(m_pParent->m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey & BIOS::KeyRight && *m_pVal < m_Max )
		{
			(*m_pVal)++;
			Invalidate();
			SendMessage(m_pParent->m_pParent, ToWord('u', 'p'), 0);
		}
		CListItem::OnKey( nKey );
	}

};

class CLNumberItem : public CListItem
{
	ui16 *m_pVal;
	ui16 m_nMin;
	ui16 m_nMax;

public:
	void Create(const char* pszId, ui16* val, ui16 _min, ui16 _max, CWnd* pParent)
	{
		m_pVal = val;
		m_nMin = _min;
		m_nMax = _max;

		CListItem::Create( pszId, WsVisible, pParent );
	}

	virtual void OnPaint()
	{
		ui16 clr = HasFocus() ? RGB565(ffffff) : RGB565(000000);
		//ui16 bgr = HasFocus() ? RGB565(ffffff) : RGB565(b0b0b0);
		ui16 clrleft = *m_pVal > m_nMin ? clr : RGB565(808080);
		ui16 clrright = *m_pVal < m_nMax ? clr : RGB565(808080);
		
		CListItem::OnPaint();

		int x = m_rcClient.left+4;
		BIOS::Print( x, m_rcClient.top, clr, RGBTRANS, m_pszId);

		x = m_rcClient.left + 120;
		x -= 8;
		x += BIOS::Draw( x, m_rcClient.top, clrleft, RGBTRANS, more_left );
		x += BIOS::Print( x, m_rcClient.top, clr, RGBTRANS, 
			CUtils::itoa( *m_pVal ) );
		x += BIOS::Draw( x, m_rcClient.top, clrright, RGBTRANS, more_right );
	}

	virtual void OnKey(ui16 nKey)
	{
		if ( nKey & BIOS::KeyLeft && *m_pVal > m_nMin )
		{
			(*m_pVal)--;
			Invalidate();
			SendMessage(m_pParent->m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey & BIOS::KeyRight && *m_pVal < m_nMax )
		{
			(*m_pVal)++;
			Invalidate();
			SendMessage(m_pParent->m_pParent, ToWord('u', 'p'), 0);
		}
		CListItem::OnKey( nKey );
	}
};

class CLColorItem : public CListItem
{
	ui16 *m_pVal;

public:
	void Create(const char* pszId, ui16* pColor, CWnd* pParent)
	{
		m_pVal = pColor;
		CListItem::Create( pszId, WsVisible | WsNoActivate, pParent );
	}

	virtual void OnPaint()
	{
		ui16 clr = HasFocus() ? RGB565(ffffff) : RGB565(000000);
		//ui16 bgr = HasFocus() ? RGB565(ffffff) : RGB565(b0b0b0);
		
		CListItem::OnPaint();

		int x = m_rcClient.left+4;
		BIOS::Print( x, m_rcClient.top, clr, RGBTRANS, m_pszId);

		clr = *m_pVal;

		x = m_rcClient.left + 120;
		x -= 8;
		x += BIOS::Draw( x, m_rcClient.top+1, clr, RGBTRANS, sel_left );
		x += BIOS::Print( x, m_rcClient.top+1, RGBTRANS, clr, 
			CUtils::clrhex( clr ) );
		x += BIOS::Draw( x, m_rcClient.top+1, clr, RGBTRANS, sel_right );
	}
};

class CLRgbComponentItem : public CListItem
{
public:
	enum EComponent { Red, Green, Blue } m_Component;
	ui16* m_pColor;

public:
	void Create(const char* pszId, ui16* pColor, EComponent Component, CWnd* pParent)
	{
		m_pColor = pColor;
		m_Component = Component;
		CListItem::Create( pszId, WsVisible, pParent );
	}

	ui8 GetValue()
	{
		switch (m_Component)
		{
			case Red:
				return (*m_pColor & 0x1f) >> 1;
			case Green: 
				return ((*m_pColor>>5) & 0x3f) >> 2;
			case Blue: 
				return ((*m_pColor>>11) & 0x1f) >> 1;
		}
		_ASSERT(0);
		return 0;
	}

	void SetValue(ui8 n)
	{
		switch (m_Component)
		{
			case Red:
				*m_pColor &= ~0x1f;
				n = (n == 15) ? 31 : (n<<1);
				*m_pColor |= n;
				return;
			case Green: 
				*m_pColor &= ~(0x3f<<5);
				n = (n == 15) ? 63 : (n<<2);
				*m_pColor |= n << 5;
				return;
			case Blue: 
				*m_pColor &= ~(0x1f<<11);
				n = (n == 15) ? 31 : (n<<1);
				*m_pColor |= n << 11;
				return;
		}
		_ASSERT(0);
	}

	virtual void OnPaint()
	{
		ui16 clr = HasFocus() ? RGB565(ffffff) : RGB565(000000);
		//ui16 bgr = HasFocus() ? RGB565(ffffff) : RGB565(b0b0b0);
		ui8 value = GetValue();
		ui16 clrleft = value > 0 ? clr : RGB565(808080);
		ui16 clrright = value < 15 ? clr : RGB565(808080);
		
		CListItem::OnPaint();

		int x = m_rcClient.left+4;
		BIOS::Print( x, m_rcClient.top, clr, RGBTRANS, m_pszId);

		char temp[2];
		temp[0] = CUtils::tohex( value );
		temp[1] = 0;

		x = m_rcClient.left + 120;
		x -= 8;
		x += BIOS::Draw( x, m_rcClient.top, clrleft, RGBTRANS, more_left );
		x += BIOS::Print( x, m_rcClient.top+1, clr, RGBTRANS, temp);
		x += BIOS::Draw( x, m_rcClient.top, clrright, RGBTRANS, more_right );
	}

	virtual void OnKey(ui16 nKey)
	{
		ui8 value = GetValue();
		if ( nKey & BIOS::KeyLeft && value > 0 )
		{
			SetValue( value-1 );
			Invalidate();
			SendMessage(m_pParent->m_pParent, ToWord('u', 'p'), 0);
		}
		if ( nKey & BIOS::KeyRight && value < 15 )
		{
			SetValue( value+1 );
			Invalidate();
			SendMessage(m_pParent->m_pParent, ToWord('u', 'p'), 0);
		}
		CListItem::OnKey( nKey );
	}
};
*/
