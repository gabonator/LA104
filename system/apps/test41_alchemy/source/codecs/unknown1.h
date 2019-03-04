class CCodecUnknown1 : public CCodec
{
  uint8_t mNibbles[32]; // TODO: range protection
  int mBits{0};
  int mNibbleCount{0};

  virtual const char* Id()
  {
      return "Unknown protocol 1 (manchester)";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        mBits = 0;
        if (count < 20)
            return false;
     
        // preamble
        if (data[0] + data[1] < 1800 || data[0] + data[1] > 2100) 
          return false;
        if (data[2] + data[3] < 1800 || data[2] + data[3] > 2100) 
          return false;

        int compensation = (data[3] - data[2])/2;
        uint8_t buffer[200];
        int bufferi = 0;
        int last = -1;
        auto PushManchester = [&](int n)
        {
//printf("%c", n?'H':'L');
            if (last == -1)
            {
                last = n;
                return true;
            }
            if (last == 1 && n == 0)
            {
                buffer[bufferi++] = 0;
            }
            else if (last == 0 && n == 1)
            {
                buffer[bufferi++] = 1;
            }
            else
                return false;
            
            if (bufferi >= COUNT(buffer))
            {
                _ASSERT(!"Buffer too small");
                return false;
            }
            
            last = -1;
            return true;
        };


        for (int i=1, b=1; i<count-1; i++, b=1-b)
        {
          int d = data[i];
          if ((i & 1) == 0)
            d += compensation;
          else 
            d -= compensation;

          if (d > 300 && d < 600)
          {
            if (i==1) 
              continue;
            if (!PushManchester(b))
              return false;
          } else
          if (d > 800 && d < 1200)
          {
            if (!PushManchester(b))
              return false;
            if (i==1) 
              continue;
	            if (!PushManchester(b))
              return false;
          } else
            return false;
        }
        if (!PushManchester(0))
          return false;

        mBits = bufferi;
        mNibbleCount = 0;
        for (int i=0; i<bufferi-3 && mNibbleCount < COUNT(mNibbles); i+=4, mNibbleCount++)
            mNibbles[mNibbleCount] = buffer[i] + buffer[i+1]*2 + buffer[i+2]*4 + buffer[i+3]*8;

        return mNibbleCount >= 10;
  }

  virtual void GetShortInfo(char* info) 
  {
      char* p = info;
      p += sprintf(p, "Raw: ");
      for (int i=0; i<min(40, mNibbleCount); i++)
          p += sprintf(p, "%x", mNibbles[i]);

      if (mNibbleCount>40)
          p += sprintf(p, "...");

      p += sprintf(p, " (%d bits)", mBits);
  }

  virtual void DisplayShortInfo(const CRect& rcRect) 
  {
    char info[128];
    GetShortInfo(info);
    BIOS::LCD::Print(rcRect.left, rcRect.top, RGB565(b0b0b0), RGBTRANS, info);
  }

  virtual void DisplayFullInfo(const CRect& rcRect) 
  {
    DisplayShortInfo(rcRect);
  }
};
