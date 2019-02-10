class CLayoutSignal : public CWnd
{
    virtual void WindowMessage(int nMsg, int nParam = 0) override
    {
        if (nMsg == CWnd::WmTick && mStorage.mDeviceCurrent)
        {
            enum TLast {Z, L, H, LH};
            int current = mStorage.mDeviceCurrent->Read();
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
    
    virtual void OnPaint() override
    {
        CRect rcSignalBar = m_rcClient;
        rcSignalBar.top -= 14;
        rcSignalBar.bottom = m_rcClient.top;
        
        GUI::Background(rcSignalBar, RGB565(404040), RGB565(101010));
        
        CRect rcSignalDelim = rcSignalBar;
        rcSignalDelim.top = rcSignalDelim.bottom - 1;
        rcSignalDelim.bottom = rcSignalDelim.top + 1;
        BIOS::LCD::Bar(rcSignalDelim, RGB565(b0b0b0));

        int x = rcSignalBar.left;
        int y = rcSignalBar.top;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Received signal");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        
        //BIOS::LCD::Bar(m_rcClient, RGB565(404040)); // TODO: OnEnter
    }
};
