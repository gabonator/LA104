#include "../../../os_host/source/gui/Controls.h"

class CItemMode : public CWndMenuItem
{
    uint16_t m_color;
    
public:
    void Create(uint16_t c, CWnd *pParent)
    {
        m_color = c;
        CWndMenuItem::Create( NULL, m_color, 2, pParent);
    }
    
    virtual void OnPaint()
    {
        ui16 clr = m_color;
        
        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Running");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "auto");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), clr, "auto");
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
        }
    }
};


class CItemHistogramX : public CWndMenuItem
{
    uint16_t m_color;
    
public:
    void Create(uint16_t c, CWnd *pParent)
    {
        m_color = c;
        CWndMenuItem::Create( NULL, m_color, 2, pParent);
        CWndMenuItem::SetColorPtr( &m_color );
    }
    
    virtual void OnPaint()
    {
        static const char* ranges[] = {"", "1", "2", "5", "7", "11"};

        ui16 clr = m_color;
        
        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Histo \x1d");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, ranges[mStorage.mHistogramScaleX]);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, " ms");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Print( x, y, RGB565(ffffff), clr, ranges[mStorage.mHistogramScaleX]);
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "ms");
        }
    }
    
    virtual void OnKey(ui16 nKey)
    {
        if ( nKey == BIOS::KEY::Right && mStorage.mHistogramScaleX > 1 )
        {
            mStorage.mHistogramScaleX--;
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Left && mStorage.mHistogramScaleX < 5 )
        {
            mStorage.mHistogramScaleX++;
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        CWndMenuItem::OnKey(nKey);
    }
};

class CItemHistogramY : public CWndMenuItem
{
    uint16_t m_color;
    
public:
    void Create(uint16_t c, CWnd *pParent)
    {
        m_color = c;
        CWndMenuItem::Create( NULL, m_color, 2, pParent);
        CWndMenuItem::SetColorPtr( &m_color );
    }
    
    virtual void OnPaint()
    {
        ui16 clr = m_color;
        
        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Histo \x1b");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%d", mStorage.mHistogramScaleY);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, " px");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Printf( x, y, RGB565(ffffff), clr, "%d", mStorage.mHistogramScaleY);
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "px");
        }
    }
    
    virtual void OnKey(ui16 nKey)
    {
        if ( nKey == BIOS::KEY::Left && mStorage.mHistogramScaleY > 1 )
        {
            mStorage.mHistogramScaleY--;
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right && mStorage.mHistogramScaleY < 5 )
        {
            mStorage.mHistogramScaleY++;
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        CWndMenuItem::OnKey(nKey);
    }
    
};

class CItemSignalX : public CWndMenuItem
{
    uint16_t m_color;
    
public:
    void Create(uint16_t c, CWnd *pParent)
    {
        m_color = c;
        CWndMenuItem::Create( NULL, m_color, 2, pParent);
        CWndMenuItem::SetColorPtr( &m_color );
    }
    
    virtual void OnPaint()
    {
        ui16 clr = m_color;
        
        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Signal \x1d");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        int range = 228 * mStorage.mSignalScaleX / 100;
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%d.%d", range/10, range%10);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, " ms");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Printf( x, y, RGB565(ffffff), clr, "%d.%d", range/10, range%10);
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "ms");
        }
    }
    
    virtual void OnKey(ui16 nKey)
    {
        if ( nKey == BIOS::KEY::Left && mStorage.mSignalScaleX > 1 )
        {
            if (mStorage.mSignalScaleX > 50)
                mStorage.mSignalScaleX -= 4;
            else if (mStorage.mSignalScaleX > 20)
                mStorage.mSignalScaleX -= 2;
            else
                mStorage.mSignalScaleX --;
            
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right && mStorage.mSignalScaleX < 100 )
        {
            if (mStorage.mSignalScaleX > 50)
                mStorage.mSignalScaleX += 4;
            else if (mStorage.mSignalScaleX > 20)
                mStorage.mSignalScaleX += 2;
            else
                mStorage.mSignalScaleX ++;
            
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        CWndMenuItem::OnKey(nKey);
    }
    
};

class CItemSignalOfs : public CWndMenuItem
{
    uint16_t m_color;
    
public:
    void Create(uint16_t c, CWnd *pParent)
    {
        m_color = c;
        CWndMenuItem::Create( NULL, m_color, 2, pParent);
        CWndMenuItem::SetColorPtr( &m_color );
    }
    
    virtual void OnPaint()
    {
        ui16 clr = m_color;
        
        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Signal \x1d");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        int num = mStorage.mSignalOffset/100;
        int numDecimals = num/10;
        int numFract = num%10;
        
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "+%d.%d", numDecimals, numFract );
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, " ms");
        } else
        {
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Printf( x, y, RGB565(ffffff), clr, "%d.%d", numDecimals, numFract);
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "ms");
        }
    }
    
    virtual void OnKey(ui16 nKey)
    {
        if ( nKey == BIOS::KEY::Left && mStorage.mSignalOffset >= 0 )
        {
            const int step = mStorage.mSignalScaleX*5;
            
            mStorage.mSignalOffset -= step;
            mStorage.mSignalOffset = max(0, mStorage.mSignalOffset);
            
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right)
        {
            const int step = mStorage.mSignalScaleX*5;
            
            mStorage.mSignalOffset += step;

            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        CWndMenuItem::OnKey(nKey);
    }
    
};

class CHistogramView : public CWnd
{
    virtual void OnPaint() override
    {
       // BIOS::LCD::Bar(m_rcClient, RGB565(000000));
        BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
    }

public:
    void Redraw(int* arrIntervals, int nCount)
    {
        constexpr int bars = 56;
        uint8_t histoDuration[bars];
        uint8_t histoDurationHigh[bars];
        uint8_t histoDurationLow[bars];
        
        memset(histoDurationHigh, 0, sizeof(histoDurationHigh));
        memset(histoDurationLow, 0, sizeof(histoDurationLow));
        
        int divide = 100;
        int highlight = 10;
        int magnify = mStorage.mHistogramScaleY;
        
        switch (mStorage.mHistogramScaleX)
        {
            case 5: divide = 200; highlight = 5; break; // 11 ms
            case 4: divide = 125; highlight = 8; break; // 7 ms
            case 3: divide = 100; highlight = 10; break; // 5 ms
            case 2: divide = 50; highlight = 20; break; // 2 ms
            case 1: divide = 25; highlight = 40; break; // 1 ms
        }
        
        for (int i=0; i<nCount; i++)
        {
            int slot = arrIntervals[i] / divide; // 0..60
            if (slot >= COUNT(histoDuration))
            {
                slot = COUNT(histoDuration)-1; // TODO!
            }
            
            if ((i%2) == 0)
                histoDurationLow[slot]++;
            else
                histoDurationHigh[slot]++;
        }

        // 228px
        _ASSERT(m_rcClient.Width() == 228);
        for (int i=0, x=m_rcClient.left+1; x<m_rcClient.right-5; i++, x += 4)
        {
            CRect rc(x, m_rcClient.top+1, x+3, m_rcClient.bottom-1);
            CRect rcCurrent = rc;
            CRect rcOverflow = rc;
            rcOverflow.bottom = rcOverflow.top + 4;
            BIOS::LCD::Bar(rcOverflow, RGB565(ff0000));
            
            rcCurrent.top = rcCurrent.bottom - histoDurationLow[i]*magnify;
            if (rcCurrent.top < rc.top)
                continue;
            BIOS::LCD::Bar(rcCurrent, RGB565(00ff00));
            rcCurrent.bottom = rcCurrent.top;
            rcCurrent.top = rcCurrent.bottom - histoDurationHigh[i]*magnify;
            if (rcCurrent.top < rc.top)
                continue;
            BIOS::LCD::Bar(rcCurrent, RGB565(0080ff));

            rcCurrent.bottom = rcCurrent.top;
            rcCurrent.top = rc.top;
            //if (rcCurrent.top < rc.top)
            //    continue;
            if (i % highlight == 0)
                BIOS::LCD::Bar(rcCurrent, RGB565(383838));
            else
                BIOS::LCD::Bar(rcCurrent, RGB565(303030));
        }
    }
};

constexpr int w = RGB565(ffffff);
constexpr int k = RGB565(303030);
constexpr int z = RGB565(303030);
constexpr int h = RGB565(404040);
constexpr uint16_t blkSwapa[] = {z, z, z, z, z, w, w, w, w, w, w, w, w, w, w, w, w, w, z, z, z, z, z};
constexpr uint16_t blk1a[]    = {z, z, z, z, z, w, k, k, k, k, k, k, k, k, k, k, k, k, z, z, z, z, z};
constexpr uint16_t blk0a[]    = {z, z, z, z, z, k, k, k, k, k, k, k, k, k, k, k, k, w, z, z, z, z, z};
constexpr uint16_t blk1b[]    = {h, h, h, h, h, w, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h};
constexpr uint16_t blk0b[]    = {h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, w, h, h, h, h, h};
constexpr uint16_t blkSwapb[] = {h, h, h, h, h, w, w, w, w, w, w, w, w, w, w, w, w, w, h, h, h, h, h};

class CSignalView : public CWnd
{
    virtual void OnPaint() override
    {
        BIOS::LCD::Bar(m_rcClient, RGB565(000000));
        BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
    }
    int mScroll {0};
    int mStep {20};
    
public:
    void Redraw(int* arrIntervals, int nCount)
    {
        mStep = mStorage.mSignalScaleX;
        
        int index = mScroll;
        int remain = 0;
        int lastlevel = 1;
        int x;
        CRect rcBlock(m_rcClient);
        rcBlock.top++;
        rcBlock.bottom--;
        int sum = 0;
        int tog = 0;
        int lastt = -1;

        int toSkip = mStorage.mSignalOffset;
        while (toSkip > 0 && index < nCount)
        {
            int current = arrIntervals[index++];
            if (current < toSkip)
            {
                toSkip -= current;
                sum += current;
                continue;
            }
            sum += toSkip;
            remain = current-toSkip;
            break;
        }
        tog = (sum / 1000) & 1;
        sum = sum % 1000;
        lastlevel = 1-(index&1);

        for (x=m_rcClient.left+1; x<m_rcClient.right-1 && index < nCount; x++, remain -= mStep)
        {
            sum += mStep;
            
            if (sum >= 1000)
            {
                sum -= 1000;
                tog = 1 - tog;
            }
            if (remain <= 0)
            {
                if (lastt != -1)
                {
                    int v = arrIntervals[index-1];
                    char num[8];
                    sprintf(num, "%d", v);
                    int l = strlen(num);
                    if (x-lastt>(l+1)*8)
                        BIOS::LCD::Print((lastt+x)/2-l*4, m_rcClient.top+6, RGB565(b0b0b0), RGBTRANS, num);
                }
                lastt = x;
                remain = arrIntervals[index++];
            }
            
            rcBlock.left = x;
            rcBlock.right = x+1;
            BIOS::LCD::BufferBegin(rcBlock);
            
            int curlevel = 1-(index & 1);

            if (tog)
            {
                if (curlevel != lastlevel)
                {
                    lastlevel = curlevel;
                    BIOS::LCD::BufferWrite((uint16_t*)blkSwapa, COUNT(blkSwapa));
                } else if (curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk1a, COUNT(blk1a));
                else if (!curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk0a, COUNT(blk0a));
            } else
            {
                if (curlevel != lastlevel)
                {
                    lastlevel = curlevel;
                    BIOS::LCD::BufferWrite((uint16_t*)blkSwapb, COUNT(blkSwapb));
                } else if (curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk1b, COUNT(blk1b));
                else if (!curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk0b, COUNT(blk0b));
            }
        }
        if (x<320)
        {
            CRect rcClear(x, m_rcClient.top+1, m_rcClient.right-1, m_rcClient.bottom-1);
            BIOS::LCD::Bar(rcClear, k);
        }

    }
};

class CLayoutMeasMenu : public CWnd
{
    friend class CLayoutMeas;
    
    CItemMode mWndMode; // Scan/pause/single
    CItemHistogramX mWndHistogramX; // Histo resol
    CItemHistogramY mWndHistogramY; // Histo sensit
    CItemSignalOfs mWndSignalOffset;  // Window
    CItemSignalX mWndSignalZoom; // Zoom
    
public:
    void Create(CWnd* pParent, const CRect& rc)
    {
        CWnd::Create("CLayoutMeasMenu", CWnd::WsVisible, rc , pParent);

        mWndMode.Create(RGB565(b0b0b0), this);
        mWndHistogramX.Create(RGB565(b0b0b0), this);
        mWndHistogramY.Create(RGB565(b0b0b0), this);
        mWndSignalOffset.Create(RGB565(b0b0b0), this);
        mWndSignalZoom.Create(RGB565(b0b0b0), this);
   }
    
    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
    {
        m_pParent->OnMessage(pSender, code, data);
    }
};

class CLayoutMeas : public CWnd
{
    CLayoutMeasMenu mMenu;
    CHistogramView mHistogram;
    CSignalView mSignal;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        constexpr int menuWidth = 80+CWndMenuItem::MarginLeft;
        constexpr int padding = 4;
        constexpr int histogramHeight = 100;
        constexpr int spacing = 40;
        constexpr int signalHeight = 25;
        constexpr int caption = 14;

        CWnd::Create(pszId, dwFlags, rc, pParent);
        mMenu.Create(this, CRect(BIOS::LCD::Width-menuWidth, 20, BIOS::LCD::Width, BIOS::LCD::Height));
        
        CRect rcHistogram(m_rcClient.left + padding, m_rcClient.top + padding + caption,
                          m_rcClient.right - menuWidth - padding, m_rcClient.top + padding + histogramHeight);
        CRect rcSignal(rcHistogram.left, rcHistogram.bottom + spacing + caption,
                       rcHistogram.right, rcHistogram.bottom + spacing + signalHeight + caption);
        
        mHistogram.Create("Histogram", CWnd::WsNoActivate | CWnd::WsVisible, rcHistogram, this);
        mSignal.Create("Signal", CWnd::WsNoActivate | CWnd::WsVisible, rcSignal, this);
    }

    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        int x = 8;
        int y = m_rcClient.top + 4+100+4;
        BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, sigLTL );
        BIOS::LCD::Draw(x, y, RGB565(00ff00), RGBTRANS, sigXTX );
        x += 20;
        x += BIOS::LCD::Print(x, y, RGB565(00ff00), RGBTRANS, "High ");
        
        x = 8+80;
        BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, sigTLT );
        BIOS::LCD::Draw(x, y, RGB565(0080ff), RGBTRANS, sigXLX );
        x += 20;
        x += BIOS::LCD::Print(x, y, RGB565(0080ff), RGBTRANS, "Low ");

        x = 4;
        y = mHistogram.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Histogram ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        
        x = 4; y = mSignal.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Signal ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
    }
    
    void DrawSignalStats(int* p, int n)
    {
        int total = 0;
        for (int i=0; i<n; i++)
            total += p[i];

        CRect rcStatus(m_rcClient);
        rcStatus.top = rcStatus.bottom - 16;
        GUI::Background(rcStatus, RGB565(404040), RGB565(101010));
        BIOS::LCD::Printf(8, m_rcClient.bottom-16, RGB565(ffffff), RGBTRANS, "%3d    %3d  ", total/1000, n);
        BIOS::LCD::Printf(8, m_rcClient.bottom-16, RGB565(b0b0b0), RGBTRANS, "    ms     t");

    }
    
    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
    {
        if (code == ToWord('D', 'A')) // new data
        {
            mHistogram.Redraw(mStorage.mSignalData, mStorage.mSignalLength);
            mSignal.Redraw(mStorage.mSignalData, mStorage.mSignalLength);
            DrawSignalStats(mStorage.mSignalData, mStorage.mSignalLength);
        }

        if (pSender == &mMenu.mWndHistogramX || pSender == &mMenu.mWndHistogramY)
        {
            mHistogram.Redraw(mStorage.mSignalData, mStorage.mSignalLength);
        }
        if (pSender == &mMenu.mWndSignalZoom || pSender == &mMenu.mWndSignalOffset)
        {
            mSignal.Redraw(mStorage.mSignalData, mStorage.mSignalLength);
        }

    }

};
