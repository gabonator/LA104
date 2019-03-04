class CCodecUnknown3 : public CCodec
{
  uint8_t mNibbles[32]; // TODO: range protection
  int mBits{0};
  int mNibbleCount{0};

  virtual const char* Id()
  {
      return "Unknown protocol 3 (gap code)";
  }

  virtual bool Decode(uint16_t* data, int count)
  {              
        mBits = 0;
        if (count < 20)
            return false;
//printf("count=%d, %d, ", count, data[0]);
     
        // preamble
        if (data[0] < 340 || data[0] > 700)
          return false;
        auto PushBit = [&](int v)
        {
//  printf("%d", v);
          uint8_t& nibble = mNibbles[mBits/4];
          if ((mBits&3) == 0)
            nibble = 0;

          nibble <<= 1;
          nibble |= v;
          mBits++;
        };

//        bool skipZero = true;
        for (int i=0, b=1; i<count; i++, b=1-b)
        {
          int d = data[i];
          if (b==1) // high
          {
            if (d < 360 || d > 700)
              return false;
          } else // low
          {
            if (d >= 1500 && d < 2100)
            {
  //            if (i>6)
//                skipZero = false;
                PushBit(0);
//              else 
//                return false; // first three pulses are long
            }
            else if (d >= 4000 && d < 5000)
            {
//              if (!skipZero)
                PushBit(1);
            }
            else 
              return false;
          }
        }
        mNibbleCount = (mBits+3)/4;
        return mNibbleCount >= 8;
  }

  virtual void GetShortInfo(char* info) 
  {
//"36>10010110 0010 001101100000 111111111100   11.8",
/*
"36>10010110 0010 010001000000 111111111010   3.4",
"36>10010110 0010 000011000000 111111110010   4.8",
"36>10010110 0010 001111000000 111111110000   7.0 ?",
"36>10010110 0010 001110010000 111111111100   7.8 ",
"36>10010110 0010 101010100000 111111111101   8.5",
"36>10010110 0010 000101100000 111111111111   11.4",
"36>10010110 0010 001101100000 111111111100   11.8",

*/
      char* p = info;
      /*
      if (mBits >= 34 && mBits <= 36)
      {
        int temp = 0;

        for (int i=0; i<mBits; i++)
        {
if ((i&3)==0)
  printf(" ");
         printf("%d", GetBit(i));
        }
  printf("\n");



        for (int i=12+12-1; i>=12; i--)
        {
          int b = GetBit(i+36-mBits);
         printf("%d", b);
          temp <<= 1;
          temp |= 1-b;
        }
  printf("\n");
        p += sprintf(p, "Temp: %d.%d C", temp/10, temp%10);
      }
       */
      p += sprintf(p, "Raw: ");
      for (int i=0; i<mNibbleCount; i++)
          p += sprintf(p, "%x", mNibbles[i]);

      p += sprintf(p, " (%d bits)", mBits);
  }

  int GetBit(int i)
  {
    if (i<0)
      return 0;

    return (mNibbles[i/4] & (1<<(i&3))) ? 1 : 0;
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
