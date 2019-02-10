class CLayoutPageCc1101 : public CWnd
{
    CRect mRcContent;
    CRect mRcPins;
    CLayoutSignal mSignal;
    int mRow{0};

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mStorage.mDeviceRadio.Load(nullptr); // So the list will be populated with valid data
        CWnd::Create(pszId, dwFlags, rc, pParent);

        mRcContent = m_rcClient;
        mRcContent.bottom = m_rcClient.bottom-40-14;
        mRcContent.right = mRcContent.right - 100;
        CRect rcSignal = CRect(m_rcClient.left, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
        mRcPins = CRect(mRcContent.right, m_rcClient.top, m_rcClient.right, mRcContent.bottom+14-1);
        
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsTick | CWnd::WsNoActivate, rcSignal, this);
    }
    
private:
    bool IsActive()
    {
        return mStorage.mDeviceCurrent == &mStorage.mDeviceRadio;
    }
    
    void DrawStatusPage(const CRect& rcContent)
    {
        GUI::Background(rcContent, RGB565(404040), RGB565(101010));
        
        int x, y;
        int _x = 8, _y = 40;
        x = _x; y = _y;
        //x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "?");
        //_y += 16; x = _x; y = _y;
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
            //x += BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, action);
            //x += BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
        }
        

        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Modulation: ");
        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, mStorage.mDeviceRadio.GetModulation());
        _y += 16; x = _x; y = _y;
        
        int32_t freq = mStorage.mDeviceRadio.GetFrequency();
        int freqDecimals = freq / 1000000;
        int freqFraction = freq % 1000000;
        
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Center freq: ");
        if (HasFocus() && mRow == 1)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d.%02d MHz", freqDecimals, freqFraction / 10000);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d.%02d MHz", freqDecimals, freqFraction / 10000);
        }
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Bandwidth: ");
        if (HasFocus() && mRow == 2)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d kHz", mStorage.mDeviceRadio.GetBandwidth() / 1000);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d kHz", mStorage.mDeviceRadio.GetBandwidth() / 1000);
        }
        _y += 16; x = _x; y = _y;
        int rate = mStorage.mDeviceRadio.GetDataRate();
        int rateDecimals = rate / 1000;
        int rateFraction = rate % 1000;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Data rate: ");
        x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d.%d kbps", rateDecimals, rateFraction/100);
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Attenuation: ");
        if (HasFocus() && mRow == 3)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d dB", mStorage.mDeviceRadio.GetGain());
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d dB", mStorage.mDeviceRadio.GetGain());
        }
    }
    
    void DrawPinsPage(const CRect& rcPins)
    {
        int _x, _y;
        _x = rcPins.left + 8;
        _y = 40;

        GUI::Background(rcPins, RGB565(505050), RGB565(202020));
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P1: MOSI");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P2: /CS");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P3: SCK");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P4: GDO0");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "CH1: MISO");
        _y += 16;
    }

    virtual void OnPaint() override
    {
        DrawStatusPage(mRcContent);
        DrawPinsPage(mRcPins);
    }
    
    virtual void OnKey(ui16 nKey) override
    {
        if (nKey == BIOS::KEY::Left)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mStorage.mDeviceRadio.DeltaFreq(-40); break;
                case 2: mStorage.mDeviceRadio.DeltaBandwidth(+1); break;
                case 3: mStorage.mDeviceRadio.DeltaGain(+1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }
        
        if (nKey == BIOS::KEY::Right)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mStorage.mDeviceRadio.DeltaFreq(+40); break;
                case 2: mStorage.mDeviceRadio.DeltaBandwidth(-1); break;
                case 3: mStorage.mDeviceRadio.DeltaGain(-1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }

        if (nKey == BIOS::KEY::Enter)
        {
            switch (mRow)
            {
                case 0:
                    if (IsActive())
                    {
                        mStorage.mDeviceCurrent->Deinit();
                        mStorage.mDeviceCurrent = nullptr;
                    } else
                    {
                        if (mStorage.mDeviceCurrent)
                            mStorage.mDeviceCurrent->Deinit();
                        mStorage.mDeviceCurrent = &mStorage.mDeviceRadio;
                        if (!mStorage.mDeviceCurrent->Init())
                            mStorage.mDeviceCurrent = nullptr;
                    }
                    Invalidate();
                    break;
            }

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
            if (mRow < 3)
            {
                mRow++;
                DrawStatusPage(mRcContent);
                return;
            }
        }
        CWnd::OnKey(nKey);
    }
};
