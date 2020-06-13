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
    
    bool Up()
    {
        if (mFocus > 0)
        {
            OnScrollChange(mFocus);
            mFocus--;
            OnScrollChange(mFocus);
            return true;
        } else
        if (mScroll > 0)
        {
            OnScrollChange(-1);
            mScroll--;
            return true;
        }
        return false;
    }
    
    bool Down()
    {
        if (mFocus + mScroll < mMax-1)
        {
            if (mFocus >= mRows-1)
            {
                OnScrollChange(-1);
                mScroll++;
            }
            else
            {
                OnScrollChange(mFocus);
                mFocus++;
                OnScrollChange(mFocus);
            }
            return true;
        } else
        if (mFocus + mScroll < mMax-1 + mExtraBottom)
        {
            OnScrollChange(mFocus);
            mFocus++;
            return true;
        }
        return false;
    }
    
    virtual void OnScrollChange(int i)
    {
        if (!mRedrawMask)
            return;
        if (i>0)
            *mRedrawMask |= 1<<i;
        else
            *mRedrawMask = -1;
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
        if (mFocus >= end - begin + mExtraBottom)
            mFocus = end - begin-1 + mExtraBottom;
        // TODO: if count = 0?
    }
};
