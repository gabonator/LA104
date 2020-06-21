constexpr int w = RGB565(ffffff);
constexpr int k = RGB565(303030);
constexpr int z = RGB565(303030);
constexpr int h = RGB565(404040);
constexpr uint16_t blkSwapa[] = {z, z, z, z, z, z, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, z, z, z, z, z, z};
constexpr uint16_t blk1a[]    = {z, z, z, z, z, z, w, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, z, z, z, z, z, z};
constexpr uint16_t blk0a[]    = {z, z, z, z, z, z, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, k, w, z, z, z, z, z, z};
constexpr uint16_t blk1b[]    = {h, h, h, h, h, h, w, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h};
constexpr uint16_t blk0b[]    = {h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, h, w, h, h, h, h, h, h};
constexpr uint16_t blkSwapb[] = {h, h, h, h, h, h, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, h, h, h, h, h, h};

uint16_t waveData[400];

class CSignalView : public CWnd
{
    virtual void OnPaint() override
    {
        //BIOS::LCD::Bar(m_rcClient, RGB565(101010));
        //BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
        Redraw();
    }

    struct {
      bool finished;
      int index;
      int remain;
      int lastlevel;
      int x;
      int sum;
      int tog;
      int lastt;
    } mDrawState;
    
public:
    int mSettings_mSignalOffset{0};
    int mSettings_mSignalScaleX{200};
    int mStart;
    int mTotal;
    CArray<uint16_t> mWave;

public:
    CSignalView()
    {
        // TODO: pozor! ctor!
        mWave.Init(waveData, COUNT(waveData));
    }
    CArray<uint16_t>& GetWave()
    {
        return mWave;
    }
    virtual void OnTimer() override
    {
        DrawAsync();
        if (mDrawState.finished)
            KillTimer();
    }

    void Redraw()
    {
        uint16_t* arrIntervals = mWave.GetData();
        const int& nCount = mWave.GetSize();
        
        mDrawState.finished = false;
        mDrawState.index = 0;
        mDrawState.remain = 0;
        mDrawState.lastlevel = 1;
        mDrawState.sum = 0;
        mDrawState.tog = 0;
        mDrawState.lastt = -1;

        mTotal = 0;
        for (int i=0; i<nCount; i++)
            mTotal += arrIntervals[i];
        
        int toSkip = mSettings_mSignalOffset;
        while (toSkip > 0 && mDrawState.index < nCount)
        {
            int current = arrIntervals[mDrawState.index++];
            if (current < toSkip)
            {
                toSkip -= current;
                mDrawState.sum += current;
                continue;
            }
            mDrawState.sum += toSkip;
            mDrawState.remain = current-toSkip;
            break;
        }
        
        mStart = mDrawState.sum;

        mDrawState.tog = (mDrawState.sum / 1000) & 1;
        mDrawState.sum = mDrawState.sum % 1000;
        mDrawState.lastlevel = 1-(mDrawState.index&1);
        mDrawState.finished = false;
        mDrawState.x = m_rcClient.left+1;

        DrawAsync();
        KillTimer();
        if (!mDrawState.finished)
            SetTimer(1);
    }

    void GetRange(int& left, int& right, int& total)
    {
        total = mTotal;
        left = mStart;
        right = left + m_rcClient.Width()*mSettings_mSignalScaleX;
    }
    
    void DrawAsync()
    {
        uint16_t* arrIntervals = mWave.GetData();
        const int& nCount = mWave.GetSize();
        const int& nStep = mSettings_mSignalScaleX;

        CRect rcBlock(m_rcClient);
        rcBlock.top++;
        rcBlock.bottom--;
        rcBlock.left = mDrawState.x;
        rcBlock.right = mDrawState.x+1;

        for (; mDrawState.x<m_rcClient.right-1 && mDrawState.index < nCount; mDrawState.x++, mDrawState.remain -= nStep)
        {
            mDrawState.sum += nStep;
            
            if (mDrawState.sum >= 1000)
            {
                mDrawState.sum -= 1000;
                mDrawState.tog = 1 - mDrawState.tog;
            }
            if (mDrawState.remain <= 0)
            {
                if (mDrawState.lastt != -1)
                {
                    int v = arrIntervals[mDrawState.index-1];
                    char num[8];
                    sprintf(num, "%d", v);
                    int l = strlen(num);
                    if (mDrawState.x-mDrawState.lastt>(l+1)*8)
                        BIOS::LCD::Print((mDrawState.lastt+mDrawState.x)/2-l*4, m_rcClient.top+9, RGB565(b0b0b0), RGBTRANS, num);
                }
                mDrawState.lastt = mDrawState.x;
                mDrawState.remain = arrIntervals[mDrawState.index++];
            }
            
            BIOS::LCD::BufferBegin(rcBlock);
            rcBlock.left++;
            rcBlock.right++;

            int curlevel = 1-(mDrawState.index & 1);

            if (mDrawState.tog)
            {
                if (curlevel != mDrawState.lastlevel)
                {
                    mDrawState.lastlevel = curlevel;
                    BIOS::LCD::BufferWrite((uint16_t*)blkSwapa, COUNT(blkSwapa));
                } else if (curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk1a, COUNT(blk1a));
                else if (!curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk0a, COUNT(blk0a));
            } else
            {
                if (curlevel != mDrawState.lastlevel)
                {
                    mDrawState.lastlevel = curlevel;
                    BIOS::LCD::BufferWrite((uint16_t*)blkSwapb, COUNT(blkSwapb));
                } else if (curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk1b, COUNT(blk1b));
                else if (!curlevel)
                    BIOS::LCD::BufferWrite((uint16_t*)blk0b, COUNT(blk0b));
            }
        }
        if (mDrawState.x<m_rcClient.right)
        {
            CRect rcClear(mDrawState.x, m_rcClient.top+1, m_rcClient.right-1, m_rcClient.bottom-1);
            BIOS::LCD::Bar(rcClear, k);
        }
        mDrawState.finished = true;
    }
};
