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
            x += BIOS::LCD::Print( x, y, clr, RGBTRANS, mStorage.mEnabled ? "auto" : "paused");
        } else
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), clr, mStorage.mEnabled ? "auto" : "paused");
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
        ui16 clr = HasFocus() ? RGB565(404040) : m_color;

        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Zoom");
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
        if ( nKey == BIOS::KEY::Right && mStorage.mSignalScaleX < 200 )
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
        ui16 clr = HasFocus() ? RGB565(404040) : m_color;

        CWndMenuItem::OnPaint();
        int x = m_rcClient.left + 12 + MarginLeft;
        int y = m_rcClient.top;
        x += BIOS::LCD::Print( x, y, clr, RGBTRANS, "Window \x1d");
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
        static long last = 0;
        long now = BIOS::SYS::GetTick();
        int step = 0;
        if (nKey == BIOS::KEY::Left || nKey == BIOS::KEY::Right)
        {
            if (last == 0)
                last = now;
            long diff = now - last;
            
            if (diff < 50)
                step = mStorage.mSignalScaleX*20;
            else if (diff < 100)
                step = mStorage.mSignalScaleX*10;
            else if (diff < 200)
                step = mStorage.mSignalScaleX*5;
            else
                step = mStorage.mSignalScaleX;
            
            last = now;
        }
        
        if ( nKey == BIOS::KEY::Left && mStorage.mSignalOffset >= 0 )
        {
            mStorage.mSignalOffset -= step;
            mStorage.mSignalOffset = max(0, mStorage.mSignalOffset);
            
            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        if ( nKey == BIOS::KEY::Right)
        {
            mStorage.mSignalOffset += step;

            Invalidate();
            SendMessage(m_pParent, ToWord('i', 'u'), 0);
        }
        CWndMenuItem::OnKey(nKey);
    }
    
};
