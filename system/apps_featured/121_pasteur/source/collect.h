//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>

class CTempRingBuffer
{
    enum { mLength = 1024 };
    float mBuffer[mLength];
    int mIndex{0};
    
public:
    CTempRingBuffer()
    {
        for (int i=0; i<mLength; i++)
            mBuffer[i] = 0;
    }
    void operator <<(float temperature)
    {
        mIndex--;
        if (mIndex <= 0)
            mIndex = mLength-1;
        mBuffer[mIndex] = temperature;
    }
    float operator[](int index) const
    {
        return mBuffer[(mIndex+index)%mLength];
    }
    int GetLength() const
    {
        return mLength;
    }
};

CTempRingBuffer mBuffer;

class CFindMax
{
    //const CTempRingBuffer& mBuffer;
    // Buffer samples the temperature in 5 second intervals
    // and holds the lowest temperature observed during this period
    static constexpr int mSamplePeriod = 5.f;

public:
//    CFindMax(CTempRingBuffer& buffer) : mBuffer{buffer}
//    {
//    }
    
    void FindMax(float& maxPu, float& maxTemp, int& maxDuration)
    {
        maxPu = 0;
        maxTemp = 0;
        maxDuration = 0;
        
        // Take latest temperature
        float temp = mBuffer[0];
        while (temp > 0)
        {
            int duration = 0;
            // Find how long the temperature was higher or equal
            float nextTemperature = FindRun(temp, duration);
            
            // calculate PU for temp, duration
            float pu = CalculatePu(temp, duration);
            if (pu > maxPu)
            {
                maxPu = pu;
                maxTemp = temp;
                maxDuration = duration;
            }
            if (pu < 1)
                break;
            temp = nextTemperature;
        }
    }
    
    float CalculatePu(float temperatureCelsius, float durationSeconds)
    {
        return (durationSeconds / 60.f) * pow(1.393f, temperatureCelsius-60.f);
    }
    
    float FindRun(float temp, int& duration)
    {
        for (int i=0; i<mBuffer.GetLength(); i++)
            if (mBuffer[i] >= temp)
                duration += mSamplePeriod;
            else
                return mBuffer[i];
                
        // no lower temperature than temp in buffer, do not continue
        return 0.f;
    }
};

CFindMax FindMax;
/*
class CGenerator
{
public:
    float operator[](int seconds)
    {
        constexpr int period = 200;
        constexpr float minTemp = 50;
        constexpr float maxTemp = 90;
        float phase;
        
        seconds %= period;
        if (seconds < period/2)
            phase = seconds/float(period/2);
        else
            phase = 2-seconds/float(period/2);

        return minTemp + phase*(maxTemp-minTemp) - rand()%5;
    }
};

int main()
{
    CTempRingBuffer Buffer;
    CFindMax FindMax{Buffer};
    CGenerator Generator;
    const int test[]= {70, 71, 72, 73, 72, 72, 73, 72, 72, 71, 0};
    
//    for (int i=0; test[i]; i++)
    for (int i=0; i<1000; i+=5)
    {
        float maxPu, maxTemp, maxDuration;
        float temp;
        //temp = test[i];
        temp = Generator[i];
        Buffer << temp;
        FindMax.FindMax(maxPu, maxTemp, maxDuration);
        //for (int j=0; j<10; j++)
        //    printf("%d ", (int)Buffer[j]);
        printf("%02d:%02d temp=%.1f PU=%.0f (%.1f C, %.0f s)\n",
            i/60, i%60, temp, maxPu, maxTemp, maxDuration);
    }

    return 0;
}
*/



class CCollect
{
  int mSamples{0};
  float mCollectMin{0};
  float mCollectMax{0};

  float mMin{0};
  float mMax{0};
  float mMaxPu{0};
  float mMaxMaxPu{0};
  float mMaxTemp{0};
  int mMaxDuration{0};

public:
  void Add(int value)
  {
    float temp = value/16.0f;
    if (mSamples == 0)
      mCollectMin = mCollectMax = temp;
    mSamples++;
    mCollectMin = min(mCollectMin, temp);
    mCollectMax = max(mCollectMax, temp);
  }
  void Finish()
  {
    if (mSamples > 0)
    {
      mMax = mCollectMax;
      mMin = mCollectMin;
      mBuffer << mMin;
        FindMax.FindMax(mMaxPu, mMaxTemp, mMaxDuration);
        mMaxMaxPu = max(mMaxMaxPu, mMaxPu);
    }
    mSamples = 0;
  }

  char* Info()
  {
      // TOP: PU 15 (75.3 deg, 120 s)
      // BOTTOM: min 45 deg, max 49deg, maxPu: 17
      
      BIOS::LCD::Printf(8, BIOS::LCD::Height-16, RGB565(b0b0b0), RGB565(000000),
            "min: %.1f\xf8" "C, max: %.1f\xf8" "C, maxPU: %d  ",
            mMin, mMax, (int)mMaxMaxPu);

    static char info[64];
    sprintf(info, "PU: %d (%.1f" "\xf8" " %ds)  ",
      (int)mMaxPu, mMaxTemp, mMaxDuration);
    return info;
  }
};

CCollect Collect;
