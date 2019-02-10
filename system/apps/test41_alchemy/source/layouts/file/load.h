class CLayoutFileLoad : public CWnd
{
    CSignalView mSignal;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsNoActivate, CRect(0, BIOS::LCD::Height-33, BIOS::LCD::Width, BIOS::LCD::Height), this);
    }
    
    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        BIOS::LCD::Print(m_rcClient.Center().x - 15*4, m_rcClient.Center().y-4,
                         RGB565(ffffff), RGBTRANS, "Not implemented");
        
        int x = 0;
        int y = mSignal.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Signal ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
    }
};
