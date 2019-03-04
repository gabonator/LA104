class CCodecUnknown4 : public CCodec
{
  uint8_t mNibbles[32]; // TODO: range protection
  int mBits{0};
  int mNibbleCount{0};

  virtual const char* Id()
  {
      return "Unknown protocol 4 (pulse code)";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        mBits = 0;
        if (count < 20)
            return false;
     
        // preamble
        if (data[0] + data[1] < 2000 || data[0] + data[1] > 2800) 
          return false;
        if (data[2] + data[3] < 2000 || data[2] + data[3] > 2800) 
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
            if (d < 1000 || d > 1400)
              return false;
          } else // high
          {
            if (d >= 1000 && d < 1400)
            {
              if (i>3)
                PushBit(0);
            }
            else if (d >= 300 && d < 600)
            {
              if (i>3)
                PushBit(1);
              else
                return false;
            }
            else 
              return false;
          }
        }
        mNibbleCount = mBits/4;
        return mNibbleCount >= 5;
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
