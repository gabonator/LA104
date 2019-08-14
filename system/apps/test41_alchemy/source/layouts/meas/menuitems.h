class CItemMode : public CWndMenuItem
{
    uint16_t m_color;
    
public:
    void Create(uint16_t c, CWnd *pParent)
    {
        m_color = c;
        CWndMenuItem::Create( NULL, m_color, 2, pParent);
    }
    
    virtual void OnPaint() override
    {
        ui16 clr = HasFocus() ? RGB565(404040) : m_color;
        
        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Trigger");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, mRuntime.mEnabled ? "auto" : "paused");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), clr, mRuntime.mEnabled ? "auto" : "paused");
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

        ui16 clr = HasFocus() ? RGB565(404040) : m_color;

        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Histo \x1d");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        if ( !HasFocus() )
        {
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, ranges[mSettings.mHistogramScaleX]);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, " ms");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Print( x, y, RGB565(ffffff), clr, ranges[mSettings.mHistogramScaleX]);
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "ms");
        }
    }
    
    virtual void OnKey(int nKey) override
    {
        if ( nKey == BIOS::KEY::Right && mSettings.mHistogramScaleX > 1 )
        {
            mSettings.mHistogramScaleX--;
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Left && mSettings.mHistogramScaleX < 5 )
        {
            mSettings.mHistogramScaleX++;
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
        ui16 clr = HasFocus() ? RGB565(404040) : m_color;

        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Histo \x12");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        if ( !HasFocus() )
        {
            if (mSettings.mHistogramScaleY == 1)
                x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, ".5");
            else
                x += BIOS::LCD::Printf( x, y, clr, RGBTRANS, "%d", mSettings.mHistogramScaleY/2);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, " px");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            if (mSettings.mHistogramScaleY == 1)
                x += BIOS::LCD::Printf( x, y, RGB565(ffffff), clr, ".5");
            else
                x += BIOS::LCD::Printf( x, y, RGB565(ffffff), clr, "%d", mSettings.mHistogramScaleY/2);
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_right);
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "px");
        }
    }
    
    virtual void OnKey(int nKey) override
    {
        if ( nKey == BIOS::KEY::Left && mSettings.mHistogramScaleY > 1 )
        {
            mSettings.mHistogramScaleY--;
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right && mSettings.mHistogramScaleY < 5 )
        {
            mSettings.mHistogramScaleY++;
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
        ui16 clr = HasFocus() ? RGB565(404040) : m_color;

        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Zoom");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        int range = 228 * mSettings.mSignalScaleX / 100;
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
    
    virtual void OnKey(int nKey) override
    {
        if ( nKey == BIOS::KEY::Left && mSettings.mSignalScaleX > 1 )
        {
            if (mSettings.mSignalScaleX > 50)
                mSettings.mSignalScaleX -= 4;
            else if (mSettings.mSignalScaleX > 20)
                mSettings.mSignalScaleX -= 2;
            else
                mSettings.mSignalScaleX --;
            
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right && mSettings.mSignalScaleX < 200 )
        {
            if (mSettings.mSignalScaleX > 50)
                mSettings.mSignalScaleX += 4;
            else if (mSettings.mSignalScaleX > 20)
                mSettings.mSignalScaleX += 2;
            else
                mSettings.mSignalScaleX ++;
            
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
        ui16 clr = HasFocus() ? RGB565(404040) : m_color;

        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Window \x1d");
        x += 8;
        x = m_rcClient.left + 12 + MarginLeft;
        y += 16;
        int num = mSettings.mSignalOffset/100;
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
    
    virtual void OnKey(int nKey) override
    {
        static long last = 0;
        long now = BIOS::SYS::GetTick();
        int step = 0;
        if (nKey == BIOS::KEY::Left || nKey == BIOS::KEY::Right)
        {
            if (last == 0)
                last = now;
            long diff = now - last;
            
            if (diff < 50)
                step = mSettings.mSignalScaleX*20;
            else if (diff < 100)
                step = mSettings.mSignalScaleX*10;
            else if (diff < 200)
                step = mSettings.mSignalScaleX*5;
            else
                step = mSettings.mSignalScaleX;
            
            last = now;
        }
        
        if ( nKey == BIOS::KEY::Left && mSettings.mSignalOffset >= 0 )
        {
            mSettings.mSignalOffset -= step;
            mSettings.mSignalOffset = max(0, mSettings.mSignalOffset);
            
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right)
        {
            mSettings.mSignalOffset += step;

            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        CWndMenuItem::OnKey(nKey);
    }
    
};
