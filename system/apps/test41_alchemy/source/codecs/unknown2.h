class CCodecUnknown2 : public CCodec
{
  uint8_t mNibbles[32]; // TODO: range protection
  int mBits{0};
  int mNibbleCount{0};

  virtual const char* Id()
  {
      return "Unknown protocol 2 (pulse code)";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        mBits = 0;
        if (count < 20)
            return false;
     
        // preamble
        if (data[0] + data[1] < 1800 || data[0] + data[1] > 2600) 
          return false;
        if (data[2] + data[3] < 1800 || data[2] + data[3] > 2600) 
          return false;

        int compensation = (data[3] - data[2])/2;

        auto PushBit = [&](int v)
        {
          uint8_t& nibble = mNibbles[mBits/4];
          if ((mBits&3) == 0)
            nibble = 0;

          nibble <<= 1;
          nibble |= v;
          mBits++;
        };

        for (int i=1, b=0; i<count; i++, b=1-b)
        {
          int d = data[i];
          if ((i & 1) == 0)
            d += compensation;
          else 
            d -= compensation;

          if (b==0) // low
          {
            if (d < 1100 || d > 1300)
              return false;
          } else // high
          {
            if (d >= 1100 && d < 1300)
            {
              if (i>5)
                PushBit(0);
            }
            else if (d >= 400 && d < 550)
            {
              if (i>5)
                PushBit(1);
              else
                return false; // first two: 11
            }
            else 
              return false;
          }
        }
        mNibbleCount = mBits/4;
        return mNibbleCount >= 10;
  }

  virtual void GetShortInfo(char* info) 
  {
      char* p = info;
      p += sprintf(p, "Raw: ");
      for (int i=0; i<mNibbleCount; i++)
          p += sprintf(p, "%x", mNibbles[i]);

      p += sprintf(p, " (%d bits)", mBits);
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
