class CCodecWeather1 : public CCodec
{
  uint8_t mNibbles[32]; // TODO: range protection
  int mBits{0};

  virtual const char* Id()
  {
      return "Weather wireless station 1";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        mBits = 0;
        if (count < 20)
            return false;
     
        // preamble
        if (data[0] < 360 || data[0] > 600) 
          return false;
        if (data[1] < 7500 || data[1] > 9500)
          return false;

        for (int i=2; i<count-1; i+=2)       	
        {
          if (data[i] <= 360 || data[i] >= 600) // 620..660
            return false;

          if (data[i+1] >= 1800 && data[i+1] <= 2300) // 480..500
          {
            // bit 0
            if ((mBits & 3) == 0)
              mNibbles[mBits / 4] = 0;
            mNibbles[mBits / 4] <<= 1;
            mBits++;
          }
          else if (data[i+1] >= 3800 && data[i+1] <= 4300) // 1660
          {
            // bit 1
            if ((mBits & 3) == 0)
              mNibbles[mBits / 4] = 0;
            mNibbles[mBits / 4] <<= 1;
            mNibbles[mBits / 4] |= 1;
            mBits++;
          }
          else
            return false;
        }

    return mBits >= 16;
  }

  virtual void GetShortInfo(char* info) 
  {
      if (mBits == 9*4 && mNibbles[7] == 15 && mNibbles[6] == 15)
      {
          int temperature = (mNibbles[5]*256 + mNibbles[4]*16 + mNibbles[3])/10;
          sprintf(info, "Temperature: %d.%d \xf8" "C", temperature / 10, temperature % 10);
          return;
      }

      char* p = info;
      p += sprintf(p, "Raw: ");
      for (int i=0; i<mBits/4; i++)
          p += sprintf(p, "%x", mNibbles[i]);
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
};
