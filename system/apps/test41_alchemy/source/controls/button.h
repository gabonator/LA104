class CButton : public CWnd
{
    virtual void OnPaint() override
    {
        if (GetFocus() == this)
        {
            BIOS::LCD::RoundRect(m_rcClient, RGB565(ffffff));
            BIOS::LCD::Print(m_rcClient.left+4, m_rcClient.top+1, RGB565(000000), RGBTRANS, m_pszId);
        } else
        {
            BIOS::LCD::RoundRect(m_rcClient, RGB565(b0b0b0));
            BIOS::LCD::Print(m_rcClient.left+4, m_rcClient.top+1, RGB565(ffffff), RGBTRANS, m_pszId);
        }
    }
    
    virtual void OnKey(int nKey) override
    {
        if (nKey == BIOS::KEY::F1)
        {
            SendMessage(GetParent(), 0, 0);
            return;
        }
        CWnd::OnKey(nKey);
    }
};

