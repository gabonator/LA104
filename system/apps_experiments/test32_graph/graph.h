struct Range
{
    int mMin;
    int mMax;
    
    bool operator ==(const Range& r)
    {
        return mMin == r.mMin && mMax == r.mMax;
    }
};

class CSeriesBase
{
public:
    virtual int GetLength() = 0;
    virtual int GetMaxLength() = 0;
    virtual int operator[](int i) = 0;
    virtual void operator <<(int v) = 0;
};

class CGraph : public CWnd
{
    Range mRange;

    CSeriesBase* mSeriesData[4];
    CArray<CSeriesBase*> mSeries;
    
    int mPrecalcWindowTop;
    int mPrecalcWindowBottom;
    int mPrecalcBaseX;
    int mPrecalcBaseY;
    int mPrecalcMaxLength;
    
    int mPrevCount{0};
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        mSeries.Init(mSeriesData, COUNT(mSeriesData));
        
        mRange.mMin = 0;
        mRange.mMax = 20;
        mPrecalcWindowTop = m_rcClient.top*256+1;
        mPrecalcWindowBottom = m_rcClient.bottom*256-256-1;
        mPrecalcBaseX = m_rcClient.left*256;
        mPrecalcBaseY = (m_rcClient.bottom-1*0)*256;
    }
    
    void AddSeries(CSeriesBase* series)
    {
        mSeries.Add(series);
        mPrecalcMaxLength = series->GetMaxLength();
    }
    
    bool CheckRange(int y)
    {
        if (y >= mRange.mMin && y < mRange.mMax)
            return false;
        
        while (y < mRange.mMin)
        {
            int h = mRange.mMax - mRange.mMin;
            mRange.mMin -= h/4;
        }

        while (y > mRange.mMax)
        {
            int h = mRange.mMax - mRange.mMin;
            mRange.mMax += h/4;
        }
        return true;
    }
    
    const Range& GetRange()
    {
        return mRange;
    }
    
    int CalcY(int v)
    {
        int y = mPrecalcBaseY - (v-mRange.mMin)*256*m_rcClient.Height()/(mRange.mMax-mRange.mMin);
        return std::min<int>(std::max<int>(mPrecalcWindowTop, y), mPrecalcWindowBottom);
    }
    
    int CalcX(int i)
    {
        return mPrecalcBaseX+i*256*m_rcClient.Width()/mPrecalcMaxLength;
    }
    
    void DrawLine(int i, int s)
    {
        int color = CPalette::GetColor(s);

        CSeriesBase& series = *mSeries[s];
        CPoint pt0(CalcX(i-1), CalcY(series[i-1]));
        CPoint pt1(CalcX(i), CalcY(series[i]));
        drawline_aa(pt0.x, pt0.y, pt1.x, pt1.y, color);
    }
    
    void OnPaint()
    {
        CPoint mLast;

        BIOS::LCD::Bar(m_rcClient, RGB565(a0a0a0));

        int zero = (CalcY(0) >> 8)+1; // +1 ??
        if (zero >= m_rcClient.top && zero < m_rcClient.bottom)
        {
            BIOS::LCD::Bar(m_rcClient.left, zero, m_rcClient.right, zero+1, RGB565(ffffff));
        }

        for (int s=0; s<mSeries.GetSize(); s++)
        {
            int color = CPalette::GetColor(s);
            mLast = CPoint(0, 0);
            CSeriesBase& series = *mSeries[s];
            for (int i=0; i<series.GetLength(); i++)
            {
                CPoint mCurrent(CalcX(i), CalcY(series[i]));
                if (mLast != CPoint(0, 0))
                    drawline_aa(mLast.x, mLast.y, mCurrent.x, mCurrent.y, color);
                mLast = mCurrent;
            }
        }
    }

    void Redraw()
    {
        int curCount = mSeries[0]->GetLength();
        bool changed = curCount != mPrevCount;
        
        if (!changed)
            return;
        
        bool decimated = (curCount < mPrevCount);
        
        if (decimated)
        {
            Invalidate();
        }
        else
        {
            bool scaleChanged = false;
            for (int i=mPrevCount; i<curCount; i++)
                for (int s=0; s<mSeries.GetSize(); s++)
                {
                    if (mSeries[s]->GetLength() > i)
                        scaleChanged |= CheckRange(mSeries[s]->operator[](i));
                }
            
            if (scaleChanged)
            {
                Invalidate();
            } else
            {
                for (int s=0; s<mSeries.GetSize(); s++)
                {
                    if (mSeries[s]->GetLength()>1)
                        DrawLine(mSeries[s]->GetLength()-1, s);
                }
            }
        }
        mPrevCount = curCount;
    }
};
