class CLayoutPageCc1101 : public CWnd
{
    CRect mRcContent;
    CRect mRcPins;
    CRect mRcSignal;
    int mRow{0};
    
    void DrawStatusPage(const CRect& rcContent)
    {
        GUI::Background(rcContent, RGB565(404040), RGB565(101010));
        
        int x, y;
        int _x = 8, _y = 40;
        x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Status: ");
        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, mStorage.mModemConfig.mOnline ? "Online" : "Offline");
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Config: ");
        if (HasFocus() && mRow == 0)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), "KEELOQ01.CFG");
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else {
            //x += BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, "KEELOQ01.CFG");
            //x += BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
        }
        
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Modulation: ");
        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, mStorage.mModemConfig.GetModulation());
        _y += 16; x = _x; y = _y;
        
        int32_t freq = mStorage.mModemConfig.GetFrequency();
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
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d kHz", mStorage.mModemConfig.GetBandwidth() / 1000);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d kHz", mStorage.mModemConfig.GetBandwidth() / 1000);
        }
        _y += 16; x = _x; y = _y;
        int rate = mStorage.mModemConfig.GetDataRate();
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
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d dB", mStorage.mModemConfig.GetGain());
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d dB", mStorage.mModemConfig.GetGain());
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

    void DrawSignalPage(const CRect& rcSignal)
    {
        CRect rcSignalBar = rcSignal;
        rcSignalBar.top -= 14;
        rcSignalBar.bottom = rcSignal.top;
        
        GUI::Background(rcSignalBar, RGB565(404040), RGB565(101010));

        CRect rcSignalDelim = rcSignalBar;
        rcSignalDelim.top = rcSignalDelim.bottom - 1;
        rcSignalDelim.bottom = rcSignalDelim.top + 1;
        BIOS::LCD::Bar(rcSignalDelim, RGB565(b0b0b0));
        
        int x = rcSignalBar.left;
        int y = rcSignalBar.top;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Received signal");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        
        BIOS::LCD::Bar(rcSignal, RGB565(404040));
    }

    virtual void OnPaint() override
    {
        // TODO: rects into create
        mRcContent = m_rcClient;
        mRcContent.bottom = m_rcClient.bottom-40-14;
        mRcContent.right = mRcContent.right - 100;
        DrawStatusPage(mRcContent);
        
        mRcSignal = CRect(m_rcClient.left, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
        DrawSignalPage(mRcSignal);
        
        mRcPins = CRect(mRcContent.right, m_rcClient.top, m_rcClient.right, mRcContent.bottom+14-1);
        DrawPinsPage(mRcPins);
    }
    
    virtual void OnKey(ui16 nKey) override
    {
        if (nKey == BIOS::KEY::Left)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mStorage.mModemConfig.DeltaFreq(-40); break;
                case 2: mStorage.mModemConfig.DeltaBandwidth(+1); break;
                case 3: mStorage.mModemConfig.DeltaGain(+1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }
        
        if (nKey == BIOS::KEY::Right)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mStorage.mModemConfig.DeltaFreq(+40); break;
                case 2: mStorage.mModemConfig.DeltaBandwidth(-1); break;
                case 3: mStorage.mModemConfig.DeltaGain(-1); break;
            }
            DrawStatusPage(mRcContent);
            return;
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
    
    virtual void WindowMessage(int nMsg, int nParam = 0) override
    {
        if (nMsg == CWnd::WmTick)
        {
            enum TLast {Z, L, H, LH};
            int current = BIOS::GPIO::DigitalRead(BIOS::GPIO::P4); // TODO!
            static int x = BIOS::LCD::Width-1;
            static enum TLast last = L;
            static int counter = 0;
            static int highs = 0;
            if (last != LH)
            {
                if (current == 0 && last == H)
                    last = LH;
                if (current == 1 && last == L)
                    last = LH;
            }
            if (current)
                highs++;
            if (counter++ < 80)
                return;
            counter = 0;
            
            uint16_t pixBuf[40];
            for (int i=0; i<40; i++)
                pixBuf[i] = RGB565(404040);
            
            if (last == LH)
            {
                int c = highs < 40 ? RGB565(b0b0b0) : RGB565(ffffff);
                for (int i=5;i<35; i++)
                    pixBuf[i] = c;
            } else if (last == L)
                pixBuf[5] = RGB565(ffffff);
            else if (last == H)
                pixBuf[34] = RGB565(ffffff);
            
            BIOS::LCD::BufferBegin(CRect(x, BIOS::LCD::Height-40, x+1, BIOS::LCD::Height));
            BIOS::LCD::BufferWrite(pixBuf, COUNT(pixBuf));
            if (++x >= BIOS::LCD::Width)
                x = 0;
            
            last = current == 0 ? L : H;
            highs = 0;
        }
        CWnd::WindowMessage(nMsg, nParam);
    }
};

class CSubmenuModem : public CSubMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"CC1101", TItem::Default};
            case 1: return TItem{"Infra", TItem::Default};
            case 2: return TItem{"Raw", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CLayoutPageInfra : public CWnd
{
    virtual void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        BIOS::LCD::Print(m_rcClient.Center().x - 15*4, m_rcClient.Center().y-4,
                         RGB565(ffffff), RGBTRANS, "Not implemented");
    }
};

class CLayoutPageRaw : public CWnd
{
    virtual void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        BIOS::LCD::Print(m_rcClient.Center().x - 15*4, m_rcClient.Center().y-4,
                         RGB565(ffffff), RGBTRANS, "Not implemented");
    }
};

class CLayoutModem : public CWnd
{
    CSubmenuModem mMenu;
    CLayoutPageCc1101 mRadio;
    CLayoutPageInfra mInfra;
    CLayoutPageRaw mRaw;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mDeviceModem.Init();
        mStorage.mModemConfig.Load((char*)""); // TODO:

        BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Input);
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        CRect rcMenu(rc);
        rcMenu.bottom = rcMenu.top + 16;
        
        mMenu.Create("ModemMenu", CWnd::WsVisible, rcMenu, this);
        
        CRect rcContent(rc);
        rcContent.top = rcMenu.bottom;
        
        mRadio.Create("Radio", CWnd::WsVisible | CWnd::WsTick, rcContent, this);
        mInfra.Create("Infra", CWnd::WsHidden, rcContent, this);
        mRaw.Create("Raw", CWnd::WsHidden, rcContent, this);
    }

    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
    {
        if (pSender == &mMenu)
        {
            mRadio.ShowWindow(data == 0);
            mInfra.ShowWindow(data == 1);
            mRaw.ShowWindow(data == 2);
            Invalidate();
        }
    }

};
