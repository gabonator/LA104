class CStatSeries : public CSeriesBase
{
    struct meas
    {
        int min, max, avg;
    };
    meas mMemory[200];

    int mMemoryPos = 0;
    int mDecimate = 1;
    int mCounter = 0;

public:
    virtual int GetLength()
    {
        return mMemoryPos;
    }
    
    virtual int GetMaxLength()
    {
        return COUNT(mMemory);
    }
    
    virtual int operator[](int i)
    {
        return mMemory[i].avg;
    }
    
    virtual void operator<<(int v)
    {
        meas& cur = mMemory[mMemoryPos];
        
        if (mCounter == 0)
            cur = {v, v, v};
        
        if (++mCounter < mDecimate)
        {
            meas& cur = mMemory[mMemoryPos];
            cur.min = std::min<>(cur.min, v);
            cur.max = std::min<>(cur.max, v);
            cur.avg += v;
            return;
        }
        
        mCounter = 0;
        cur.avg /= mDecimate;
        
        if (++mMemoryPos >= COUNT(mMemory))
        {
            mDecimate *= 2;
            for (int i=0; i<COUNT(mMemory)/2; i++)
            {
                meas m1 = mMemory[i*2];
                meas m2 = mMemory[i*2+1];
                mMemory[i] = {std::min<>(m1.min, m2.min), (m1.avg+m2.avg)/2, std::max<>(m1.max, m2.max)};
            }
            mMemoryPos = COUNT(mMemory)/2;
        }
    }
};

class CSimpleSeries : public CSeriesBase
{
    int16_t mMemory[200];
    int mAccumulator = 0;
    int mMemoryPos = 0;
    int mDecimate = 1;
    int mCounter = 0;
    
public:
    virtual int GetLength()
    {
        return mMemoryPos;
    }
    
    virtual int GetMaxLength()
    {
        return COUNT(mMemory);
    }
    
    virtual int operator[](int i)
    {
        return mMemory[i];
    }
    
    virtual void operator<<(int v)
    {
        if (mCounter == 0)
            mAccumulator = v;
        
        if (++mCounter < mDecimate)
        {
            mAccumulator += v;
            return;
        }
        
        int16_t& cur = mMemory[mMemoryPos];
        mCounter = 0;
        cur = mAccumulator/mDecimate;
        
        if (++mMemoryPos >= COUNT(mMemory))
        {
            mDecimate *= 2;
            for (int i=0; i<COUNT(mMemory)/2; i++)
                mMemory[i] = (mMemory[i*2] + mMemory[i*2+1])/2;

            mMemoryPos = COUNT(mMemory)/2;
        }
    }
};

