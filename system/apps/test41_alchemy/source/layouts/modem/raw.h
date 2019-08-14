class CLayoutPageRaw : public CWnd
{
    CLayoutSignal mSignal;
    CRect mRcContent;
    CRect mRcPins;
    int mRow{0};
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        CRect rcSignal = CRect(m_rcClient.left, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsTick | CWnd::WsNoActivate, rcSignal, this);
        mRcContent = m_rcClient;
        mRcContent.bottom = m_rcClient.bottom-40-14;
        mRcContent.right = mRcContent.right - 100;

        mRcPins = CRect(mRcContent.right, m_rcClient.top, m_rcClient.right, mRcContent.bottom+14-1);
    }
    
    bool IsActive()
    {
        return mSettings.mDeviceCurrent == &mDeviceRaw;
    }
    
    void DrawStatusPage(const CRect& rcContent)
    {
        GUI::Background(rcContent, RGB565(404040), RGB565(101010));
        
        int x, y;
        int _x = 8, _y = 40;        
        
        x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, IsActive() ? "Modem online:" : "Modem offline:");
        x += 8;
        const char* action = IsActive() ? "Disconnect" : "Connect";
        if (HasFocus() && mRow == 0)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), action);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else {
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, action);
        }

        _y += 16; x = _x; y = _y;
        
        if (HasFocus() && mRow == 1)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "[%c]", mDeviceRaw.Configuration().mInvert ? 'X' : ' ');
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
            x -= 8;
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "[%c]", mDeviceRaw.Configuration().mInvert ? 'X' : ' ');
        }
        
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, " Invert");
    }
    
    void DrawPinsPage(const CRect& rcPins)
    {
        int _x, _y;
        _x = rcPins.left + 8;
        _y = 40;
        
        GUI::Background(rcPins, RGB565(505050), RGB565(202020));
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P2: Input");
        _y += 16;
        //BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P4: Output");
    }

    virtual void OnPaint() override
    {
//        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        DrawPinsPage(mRcPins);
        DrawStatusPage(mRcContent);
    }
    
    virtual void OnKey(int nKey) override
    {
        if (nKey == BIOS::KEY::Enter)
        {
            switch (mRow)
            {
                case 0:
                    if (IsActive())
                    {
                        mSettings.mDeviceCurrent->Deinit();
                        mSettings.mDeviceCurrent = nullptr;
                    } else
                    {
                        if (mSettings.mDeviceCurrent)
                            mSettings.mDeviceCurrent->Deinit();
                        mSettings.mDeviceCurrent = &mDeviceRaw;
                        if (!mSettings.mDeviceCurrent->Init())
                            mSettings.mDeviceCurrent = nullptr;
                    }
                    Invalidate();
                    break;

                case 1: mDeviceRaw.Configuration().mInvert = !mDeviceRaw.Configuration().mInvert; break;
            }
            DrawStatusPage(mRcContent);
        }
        if (nKey == BIOS::KEY::Up)
        {
            if (mRow > 0)
            {
                mRow--;
                DrawStatusPage(mRcContent);
                return;
            }
        }
        if (nKey == BIOS::KEY::Down)
        {
            if (mRow < 1)
            {
                mRow++;
                DrawStatusPage(mRcContent);
                return;
            }
        }
        CWnd::OnKey(nKey);
    }

};
