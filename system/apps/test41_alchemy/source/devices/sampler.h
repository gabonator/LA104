class CSampler // CSampler?
{
public:
    virtual bool Read() = 0;
    virtual bool Init() = 0;
    virtual void Deinit() = 0;
    
private: // TODO: remove
    uint16_t tempBuffer[10];
    int tempReceived{0};

private:
    constexpr static int ticks20us = 126; // ked ukazuje viac, treba mensie cislo
    // 130           960.. 980
    // 126           980..1000  ... ok
    // 125          1000..1020
    // 121, 1000 -> 1020..1040
    // 120       -> 1040..1060
    // 118          1040..1060
    
    
    // 127          240..280
    //              500..560
    //              1000..1120
    int On()
    {
        int i;
        for (i=0; i<10000; i+=20)
        {
            volatile int j=ticks20us;
            while (j--);
            
            if (!Read())
                return i;
        }
        return i;
    }
    
    int Off()
    {
        int i;
        for (i=0; i<10000; i+=20)
        {
            volatile int j=ticks20us;
            while (j--);
            
            if (Read())
                return i;
        }
        return i;
    }
    
protected:
    void delayMicroseconds(int us)
    {
        us = us*12;
        while (us--)
        {
            __asm__("");
        }
    }
    
public:
    bool Receive(uint16_t* pBuffer, int nBufferSize, int& nReceived)
    {
#ifdef __APPLE__
        int sample[] = {520, 500, 420, 520, 400, 520, 440, 520, 400, 520, 400, 540, 400, 520, 440, 500, 440, 480, 440, 500, 440, 500, 460, 440, 460, 500, 420, 500, 440, 520, 420, 520, 420, 500, 460, 460, 480, 440, 480, 440, 500, 460, 480, 460, 460, 460, 480, 920, 980, 940, 960, 460, 500, 440, 500, 440, 480, 460, 480, 920, 500, 460, 500, 420, 980, 920, 980, 940, 480, 440, 500, 460, 460, 460, 960, 940, 980, 920, 500, 460, 480, 440, 480, 440, 960, 460, 520, 920, 960, 480, 480, 420, 500, 460, 480, 440, 500, 920, 480, 440, 500, 440, 500, 420, 500, 440, 500, 440, 980, 960, 440, 460, 480, 460, 500, 420, 520, 440, 480, 460, 480, 420, 500, 460, 480, 440, 500, 440, 500, 440, 480, 440, 480, 460, 980, 440, 480, 960, 960, 440, 500, 460, 460, 960, 460, 460, 500, 420, 980, 440, 500, 440, 500, 920, 960, 960, 960, 940, 980, 920, 960, 460, 500, 940, 960, 960, 460, 460, 960, 460, 480};
        memcpy(pBuffer, sample, sizeof(sample));
        nReceived = COUNT(sample);
        return true;
        for (nReceived=0; nReceived<30; nReceived+=2)
        {
            pBuffer[nReceived] = 250-30+rand()%60;
            if ((rand() & 1) == 0)
                pBuffer[nReceived+1] = 2000 - 50 + rand()%100;
            else
                pBuffer[nReceived+1] = 4000 - 100 + rand()%200;
        }
        return true;
#endif
        nReceived = 0;
        
        while (1)
        {
            int nOn = On();
            
            if (nOn >= mStorage_mMinGapTime)
                break;

            pBuffer[nReceived++] = nOn;
            if (nReceived >= nBufferSize)
                break;
            
            int nOff = Off();
            
            if (nOff >= mStorage_mMinGapTime)
                break;
            
            pBuffer[nReceived++] = nOff;
            if (nReceived >= nBufferSize)
                break;
        }
        
        if (nReceived < COUNT(tempBuffer))
        {
            memcpy(pBuffer, tempBuffer, sizeof(tempBuffer));
            nReceived = tempReceived; // TODO: when switching btwn different modems invalidate temp buf
            return false;
        }
        
        memcpy(tempBuffer, pBuffer, sizeof(tempBuffer));
        tempReceived = nBufferSize;
        return true;
    }
};
