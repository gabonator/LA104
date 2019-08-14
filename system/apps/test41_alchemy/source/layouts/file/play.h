class CLayoutFilePlay : public CWnd
{
    CSignalView mSignal;
    CButton mPlay;
    CButton mLoop;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsNoActivate, CRect(0, BIOS::LCD::Height-33, BIOS::LCD::Width, BIOS::LCD::Height), this);
        mPlay.Create("Play", CWnd::WsVisible, CRect(8, 40, 8+(5+1)*8, 40+16), this);
        mLoop.Create("Loop", CWnd::WsVisible, CRect(8, 40+20, 8+(5+1)*8, 40+20+16), this);
    }
    
    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        int x = 0;
        int y = mSignal.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Signal ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
    }
    
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mPlay)
        {
            if (mSettings.mDeviceCurrent == &mDeviceInfra)
            {
                mRuntime.mTransmitted = true;
                mDeviceInfra.Send(mStorage.mSignalData, mStorage.mSignalLength); // TODO: virtual
            }
        }
    }
};
