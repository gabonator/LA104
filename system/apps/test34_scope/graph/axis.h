class CXAxis : public CWnd
{
    friend class CYAxis;
    
protected:
    static int nicenumber(int n)
    {
        int mantisa = n, exponent = 1;
        while (mantisa > 10)
        {
            exponent *= 10;
            mantisa /= 10;
        }
        
        if (mantisa >= 5)
            mantisa = 5;
        else
            if (mantisa >= 2)
                mantisa = 2;
            else
                mantisa = 1;
        
        return mantisa * exponent;
    }
    
    virtual void FormatNumber(char* buf, int i)
    {
        if (i<1000)
        {
            sprintf(buf, "%d", i);
            return;
        }
        
        if (i<1000000)
        {
            sprintf(buf, "%dk", i/1000);
            return;
        }
        
        sprintf(buf, "%dM", i/1000000);
    }
    
    virtual int reconstruct(int i)
    {
        return i;
    }
    
    virtual void Redraw(int value, int origValue)
    {
        int step = nicenumber(value/2);
        //BIOS::DBG::Print("orig=%d max=%d step=%d\n", origValue, value, step);
        CRect rcDeflated(m_rcClient);
        rcDeflated.Deflate(2, 0, 2, 0);
        BIOS::LCD::Bar(rcDeflated, RGB565(b0b0b0));
        
        for (int _i=0; _i<=value; _i += step)
        {
            int i = reconstruct(_i);
            if (i>origValue)
                break;
            
            int x = m_rcClient.left + m_rcClient.Width()*i/origValue;
            char formatted[16];
            FormatNumber(formatted, _i);
            int width = strlen(formatted)*8;
            
            int tx = x;
            if (tx - width/2 < rcDeflated.left)
                tx = m_rcClient.left + 2 + width/2;
            
            if (tx-width/2+width > rcDeflated.right)
                tx = m_rcClient.right - 2 - width/2;
            
            BIOS::LCD::Print(tx - width/2, m_rcClient.bottom-14, RGB565(404040), RGB565(b0b0b0), formatted);
            
            if (x >= rcDeflated.left && x < rcDeflated.right)
                BIOS::LCD::Bar(x, m_rcClient.top, x+1, m_rcClient.top+3, RGB565(ffffff));
        }
    }
    
public:
    virtual void Update(int value)
    {
        Redraw(value, value);
    }
};

class CYAxis : public CWnd
{
    Range mRange;
    
protected:
    virtual int forward(int i)
    {
        return i;
    }
    
    virtual int backward(int i)
    {
        return i;
    }
    
    virtual void FormatNumber(char* buf, int i)
    {
        sprintf(buf, "%d", i);
    }

public:
    virtual void OnPaint()
    {
    }
    
    void Update(const Range& range)
    {
        if (mRange == range)
            return;
        mRange = range;
        
        BIOS::LCD::Bar(m_rcClient, RGB565(b0b0b0));
        
        auto drawTick = [&](int i)
        {
            int y = m_rcClient.bottom - (backward(i)-range.mMin)*m_rcClient.Height()/(range.mMax-range.mMin);
            if (y < m_rcClient.bottom)
                BIOS::LCD::Bar(m_rcClient.right-2, y, m_rcClient.right, y+1, RGB565(ffffff));

            char buffer[16];
            FormatNumber(buffer, i);

            int ty = y-6;
            if (ty<m_rcClient.top)
                ty = m_rcClient.top;
            if (ty+14>m_rcClient.bottom)
                ty = m_rcClient.bottom-14;
            
            BIOS::LCD::Print(m_rcClient.right - strlen(buffer)*8-3, ty, RGB565(404040), RGB565(b0b0b0), buffer);
        };

        int rangeMaxModified = forward(range.mMax/2);
        int step = CXAxis::nicenumber(rangeMaxModified);
        int total = std::max<>(forward(range.mMax), forward(-range.mMin));

        for (int i=0; i<total; i += step)
        {
            if (i < range.mMax)
                drawTick(i);
//            if (i > 0 && -i > range.mMin)
//                drawTick(-i);
        }
    }
};

