class CCodecTvRemote1 : public CCodec
{
  uint32_t mData;
  int mBits{0};

  virtual const char* Id()
  {
      return "TV remote control 1";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        mData = 0;
        mBits = 0;
        if (count < 20)
            return false;
     
        // preamble
        if (data[0] < 9000 || data[0] > 10000) // 9460
          return false;
        if (data[1] < 4000 || data[1] > 5000) // 4600
          return false;

        for (int i=2; i<count-1; i+=2)
        {
          if (data[i] <= 450 || data[i] >= 700) // 620..660
            return false;
          if (data[i+1] >= 400 && data[i+1] <= 550) // 480..500
          {
            // bit 0
            mData <<= 1;
            mBits++;
          }
          else if (data[i+1] >= 1500 && data[i+1] <= 1750) // 1660
          {
            // bit 1
            mData <<= 1;
            mData |= 1;
            mBits++;
          }
          else
            return false;
        }

    return mBits >= 16;
  }

  virtual void GetShortInfo(char* info) 
  {
      if (mBits == 32)
      {
          const char* key = ButtonName(mData);
          if (key)
          {
              sprintf(info, "%s (%08x)", key, mData);
              return;
          }
      }
      sprintf(info, "Raw: %08x (%d bits)", mData, mBits);
  }

  virtual void DisplayShortInfo(const CRect& rcRect) 
  {
    char info[64];
    GetShortInfo(info);
    BIOS::LCD::Print(rcRect.left, rcRect.top, RGB565(b0b0b0), RGBTRANS, info);
  }

  virtual void DisplayFullInfo(const CRect& rcRect) 
  {
    DisplayShortInfo(rcRect);
  }

  const char* ButtonName(uint32_t id)
  {
    switch (id)
    {
      default: return nullptr;
    }
  }
};
