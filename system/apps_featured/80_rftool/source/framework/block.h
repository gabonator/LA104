class CBlock : public CWnd
{
public:
	virtual void OnPaint() override
	{
		CRect rcInside(m_rcClient);
		rcInside.Deflate(1, 1, 1, 1);
		GUI::Background(rcInside, RGB565(101010), RGB565(101010));

		if (HasFocus())
		{
			BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
			BIOS::LCD::Print(m_rcClient.left, m_rcClient.top-14, RGB565(ffffff), RGB565(b0b0b0), m_pszId);
		}
		else
		{
			BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
			BIOS::LCD::Print(m_rcClient.left, m_rcClient.top-14, RGB565(000000), RGB565(b0b0b0), m_pszId);
		}
	}
};
