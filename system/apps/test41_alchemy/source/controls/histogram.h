class CHistogramView : public CWnd
{
    struct TDrawState
    {
        static constexpr int bars = 56+22;
        uint8_t histoDurationHigh[bars];
        uint8_t histoDurationLow[bars];
        int divide;
        int highlight;
        int i;
        int x;
        bool finished{true};
    } mDrawState;

    virtual void OnPaint() override
    {
        BIOS::LCD::Bar(m_rcClient, RGB565(101010));
        BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
        
        Redraw();
    }

public:
    virtual void OnTimer() override
    {
        DrawAsync();
        if (mDrawState.finished)
            KillTimer();
    }
    
    void Redraw()
    {        
        uint16_t* arrIntervals = mStorage.mSignalData;
        const int& nCount = mStorage.mSignalLength;

        mDrawState.finished = false;
        memset(mDrawState.histoDurationHigh, 0, sizeof(mDrawState.histoDurationHigh));
        memset(mDrawState.histoDurationLow, 0, sizeof(mDrawState.histoDurationLow));
                
        switch (mSettings.mHistogramScaleX)
        {
            case 5: mDrawState.divide = 200; mDrawState.highlight = 5; break; // 11 ms
            case 4: mDrawState.divide = 125; mDrawState.highlight = 8; break; // 7 ms
            case 3: mDrawState.divide = 100; mDrawState.highlight = 10; break; // 5 ms
            case 2: mDrawState.divide = 50; mDrawState.highlight = 20; break; // 2 ms
            case 1: mDrawState.divide = 25; mDrawState.highlight = 40; break; // 1 ms
            default: mDrawState.divide = 100; mDrawState.highlight = 10; break;
        }
        
        for (int i=0; i<nCount; i++)
        {
            int slot = arrIntervals[i] / mDrawState.divide; // 0..60

            if (slot >= COUNT(mDrawState.histoDurationHigh))
                slot = COUNT(mDrawState.histoDurationHigh)-1; // TODO!
            
            if ((i%2) == 0)
                mDrawState.histoDurationLow[slot]++;
            else
                mDrawState.histoDurationHigh[slot]++;
        }
        mDrawState.i = 0;
        mDrawState.x = m_rcClient.left+1;
        DrawAsync();
        KillTimer();
        if (!mDrawState.finished)
            SetTimer(1);
    }

    void DrawAsync()
    {
        const int& magnify = mSettings.mHistogramScaleY;

        // 228px
        //_ASSERT(m_rcClient.Width() == 228);
        for (; mDrawState.x<m_rcClient.right-5; mDrawState.i++, mDrawState.x += 4)
        {
            if (mRuntime.mEnabled && mSettings.mDeviceCurrent && mSettings.mDeviceCurrent->Read())
                return;
            
            CRect rc(mDrawState.x, m_rcClient.top+1, mDrawState.x+3, m_rcClient.bottom-1);
            CRect rcCurrent = rc;
            CRect rcOverflow = rc;
            rcOverflow.bottom = rcOverflow.top + 4;
            BIOS::LCD::Bar(rcOverflow, RGB565(ff0000));
            
            rcCurrent.top = rcCurrent.bottom - mDrawState.histoDurationLow[mDrawState.i]*magnify/2;
            if (rcCurrent.top < rc.top)
            {
                rcCurrent.top = rc.top+4;
                BIOS::LCD::Bar(rcCurrent, RGB565(00ff00));
                continue;
            }

            if (rcCurrent.Height() > 0)
                BIOS::LCD::Bar(rcCurrent, RGB565(00ff00));
            
            rcCurrent.bottom = rcCurrent.top;
            rcCurrent.top = rcCurrent.bottom - mDrawState.histoDurationHigh[mDrawState.i]*magnify/2;
            if (rcCurrent.top < rc.top)
            {
                rcCurrent.top = rc.top+4;
                BIOS::LCD::Bar(rcCurrent, RGB565(0080ff));
                continue;
            }
            
            if (rcCurrent.Height() > 0)
                BIOS::LCD::Bar(rcCurrent, RGB565(0080ff));

            rcCurrent.bottom = rcCurrent.top;
            rcCurrent.top = rc.top;
            //if (rcCurrent.top < rc.top)
            //    continue;
            if (mDrawState.i % mDrawState.highlight == 0)
                BIOS::LCD::Bar(rcCurrent, RGB565(3b3b3b));
            else
                BIOS::LCD::Bar(rcCurrent, RGB565(303030));
        }
    }
};
