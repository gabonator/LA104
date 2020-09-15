class CScroller
{
public:
    CScroller(int count, int* redrawMask) : mRedrawMask(redrawMask), mRows(count)
    {
    }
    
    int* mRedrawMask{nullptr};
    int mRows{0};
    int mMax{0};
    int mFocus{0};
    int mScroll{0};
    int mExtraBottom{0};
    int mExtraTop{0};

    bool Up()
    {
        if (mFocus > 0)
        {
            OnScrollChange(mFocus - mExtraTop);
            mFocus--;
            OnScrollChange(mFocus - mExtraTop);
            return true;
        } else
        if (mScroll > 0)
        {
            OnScrollChange();
            mScroll--;
            return true;
        }
        return false;
    }
    
    bool Down()
    {
        if (mFocus - mExtraTop + mScroll < mMax-1)
        {
            if (mFocus - mExtraTop >= mRows-1)
            {
                OnScrollChange();
                mScroll++;
            }
            else
            {
                OnScrollChange(mFocus - mExtraTop);
                mFocus++;
                OnScrollChange(mFocus - mExtraTop);
            }
            return true;
        } else
        if (mFocus - mExtraTop + mScroll < mMax-1 + mExtraBottom)
        {
            OnScrollChange(mFocus - mExtraTop);
            mFocus++;
            return true;
        }
        return false;
    }
    
    virtual void OnScrollChange(int i = -999)
    {
        if (!mRedrawMask)
            return;
        if (i == -999)
            *mRedrawMask = -1;
        else if (i >= 0)
            *mRedrawMask |= 1<<i;
    }
    
    void SetCount(int n, int nExtraBottom)
    {
        mMax = n;
        mExtraBottom = nExtraBottom;
    }
    
    void GetRange(int& begin, int& end)
    {
        if (mScroll+mRows > mMax)
            mScroll = 0;
        
        begin = mScroll;
        end = min(begin + mRows, mMax);
        if (mFocus - mExtraTop >= end - begin + mExtraBottom)
            mFocus = end - begin-1 + mExtraBottom + mExtraTop;
        // TODO: if count = 0?
    }
};
