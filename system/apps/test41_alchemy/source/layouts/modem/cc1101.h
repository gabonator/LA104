
class CModalFrequency : public CWnd
{
    typedef void(*THandler)(char*);
    char mValue[16]{"344.13"};
    char mInitial[16];
    char mSuffix[16] = "MHz";
    int mCursor = 0;
    THandler mHandler{nullptr};
    
public:
    void Create(CWnd* pParent)
    {
        constexpr int width = 180;
        constexpr int height = 70;
        CRect rcModal((BIOS::LCD::Width - width) / 2, (BIOS::LCD::Height - height) / 2,
                      (BIOS::LCD::Width + width) / 2, (BIOS::LCD::Height + height) / 2);
        CWnd::Create("FrequencyModal", CWnd::WsVisible | CWnd::WsModal, rcModal, pParent);
    }
    
    void DoModal(char* value, THandler handler)
    {
        strcpy(mValue, value);
        strcpy(mInitial, value);
        mHandler = handler;
        StartModal();
    }
    
    void OnPaint()
    {
        GUI::Window(m_rcClient, RGB565(ffffff));
        const char* title = "Center frequency";
        BIOS::LCD::Print(m_rcClient.CenterX() - strlen(title)*4, m_rcClient.top + 2, RGB565(000000), RGBTRANS, title);
        DrawValue();

        int x = m_rcClient.left + 12;
        int y = m_rcClient.bottom - 20;
        x += BIOS::LCD::Draw(x, y, RGB565(d0d0d0), RGB565(b0b0b0), CShapes_sel_left);
        x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGB565(d0d0d0), "OK");
        x += BIOS::LCD::Draw(x, y, RGB565(d0d0d0), RGB565(b0b0b0), CShapes_sel_right);
        x = m_rcClient.right - 50;
        x += BIOS::LCD::Draw(x, y, RGB565(d0d0d0), RGB565(b0b0b0), CShapes_sel_left);
        x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGB565(d0d0d0), "ESC");
        x += BIOS::LCD::Draw(x, y, RGB565(d0d0d0), RGB565(b0b0b0), CShapes_sel_right);
    }
    
    void DrawValue()
    {
        int width = strlen(mValue)*16 + 8 + 8 + strlen(mSuffix);
        int x = m_rcClient.CenterX() - width/2;
        int y = m_rcClient.top + 30;
        BIOS::LCD::Bar(CRect(x, y, x + width, y+14), RGB565(b0b0b0));
        
        for (int i=0; i<(int)strlen(mValue); i++)
        {
            if (mCursor == i)
            {
                x -= 2;
                x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGB565(b0b0b0), CShapes_sel_left);
                x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%c", mValue[i]);
                x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGB565(b0b0b0), CShapes_sel_right);
                x -= 2;
            } else
            {
                x += 2;
                x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(b0b0b0), "%c", mValue[i]);
                x += 2;
            }
        }
        x += 8;
        x += BIOS::LCD::Print(x, y, RGB565(808080), RGB565(b0b0b0), mSuffix);

    }
    
    virtual void OnKey(int key) override
    {
        switch (key)
        {
            case BIOS::KEY::EKey::Left:
                if (mCursor > 0)
                {
                    mCursor--;
                    if (mValue[mCursor] == '.')
                        mCursor--;
                    DrawValue();
                }
                break;

            case BIOS::KEY::EKey::Right:
                if (mCursor < (int)strlen(mValue)-1)
                {
                    mCursor++;
                    if (mValue[mCursor] == '.')
                        mCursor++;
                    DrawValue();
                }
                break;

            case BIOS::KEY::EKey::Up:
            {
                int n = mCursor;
                do {
                    mValue[n]++;
                    if (mValue[n] > '9')
                    {
                        mValue[n] = '0';
                        if (n>0)
                        {
                            n--;
                            if (mValue[n] == '.')
                                n--;
                            continue;
                        }
                    }
                    break;
                } while (1);
                DrawValue();
                if (mHandler)
                    mHandler(mValue);
                break;
            }
                
            case BIOS::KEY::EKey::Down:
            {
                int n = mCursor;
                do {
                    mValue[n]--;
                    if (mValue[n] < '0')
                    {
                        mValue[n] = '9';
                        if (n>0)
                        {
                            n--;
                            if (mValue[n] == '.')
                                n--;
                            continue;
                        }
                    }
                    break;
                } while (1);
                Invalidate();
                if (mHandler)
                    mHandler(mValue);
                break;
            }
                
            case BIOS::KEY::EKey::Enter:
                // approve;
                mHandler = nullptr;
                StopModal();
                break;

            case BIOS::KEY::EKey::Escape:
                // reset
                if (mHandler)
                    mHandler(mInitial);
                mHandler = nullptr;
                StopModal();
                break;
        }
    }
};

class CLayoutPageCc1101 : public CWnd
{
    CRect mRcContent;
    CRect mRcPins;
    CLayoutSignal mSignal;
    int mRow{0};
    
    CModalFrequency mModalFrequency;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mDeviceRadio.Load(nullptr); // So the list will be populated with valid data
        CWnd::Create(pszId, dwFlags, rc, pParent);

        mRcContent = m_rcClient;
        mRcContent.bottom = m_rcClient.bottom-40-14;
        mRcContent.right = mRcContent.right - 100;
        CRect rcSignal = CRect(m_rcClient.left, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
        mRcPins = CRect(mRcContent.right, m_rcClient.top, m_rcClient.right, mRcContent.bottom+14-1);
        
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsTick | CWnd::WsNoActivate, rcSignal, this);
        SetTimer(100); // release timer on exit
    }
    
private:
    bool IsActive()
    {
        return mSettings.mDeviceCurrent == &mDeviceRadio;
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
        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, mDeviceRadio.GetModulation());
        _y += 16; x = _x; y = _y;
        
        int32_t freq = mDeviceRadio.GetFrequency();
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
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d kHz", mDeviceRadio.GetBandwidth() / 1000);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d kHz", mDeviceRadio.GetBandwidth() / 1000);
        }
        _y += 16; x = _x; y = _y;
        int rate = mDeviceRadio.GetDataRate();
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
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d dB", mDeviceRadio.GetGain());
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d dB", mDeviceRadio.GetGain());
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
//        DrawRssi();
    }
    
    virtual void OnTimer() override
    {
        CRect rcRssi(m_rcClient.right-80, mRcContent.bottom + 4-10+10, m_rcClient.right-10, mRcContent.bottom + 10-10+9);

        BIOS::LCD::Bar(rcRssi, RGB565(606060));
        rcRssi.Deflate(1, 1, 1, 1);
        int k = mDeviceRadio.GetRssi();
        
        rcRssi.right = rcRssi.left + rcRssi.Width() * k / 256;
        if (rcRssi.Width() < 1)
            return;
        
        BIOS::LCD::Bar(rcRssi, RGB565(f0f020));
    }
    
    virtual void OnKey(int nKey) override
    {
        if (nKey == BIOS::KEY::Left)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mDeviceRadio.DeltaFreq(-40); break;
                case 2: mDeviceRadio.DeltaBandwidth(+1); break;
                case 3: mDeviceRadio.DeltaGain(+1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }
        
        if (nKey == BIOS::KEY::Right)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mDeviceRadio.DeltaFreq(+40); break;
                case 2: mDeviceRadio.DeltaBandwidth(-1); break;
                case 3: mDeviceRadio.DeltaGain(-1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }

        if (nKey == BIOS::KEY::Enter)
        {
            switch (mRow)
            {
                case 1:
                {
                    int32_t freq = mDeviceRadio.GetFrequency();
                    int freqDecimals = freq / 1000000;
                    int freqFraction = freq % 1000000;
                    char strFrequency[16];
                    sprintf(strFrequency, "%03d.%02d", freqDecimals, (freqFraction) / 10000);

                    mModalFrequency.Create(this);
                    mModalFrequency.DoModal(strFrequency, [](char* strFreq)
                    {
                        int freqDecimal = atoi(strFreq);
                        int freqFraction = atoi(strFreq+4);
                        int32_t freq = freqDecimal*1000000 + freqFraction * 10000;
                        mDeviceRadio.SetFreq(freq);
                    });

                    break;
                }
                case 0:
                    if (IsActive())
                    {
                        mSettings.mDeviceCurrent->Deinit();
                        mSettings.mDeviceCurrent = nullptr;
                    } else
                    {
                        if (mSettings.mDeviceCurrent)
                            mSettings.mDeviceCurrent->Deinit();
                        mSettings.mDeviceCurrent = &mDeviceRadio;
                        if (!mSettings.mDeviceCurrent->Init())
                            mSettings.mDeviceCurrent = nullptr;
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
