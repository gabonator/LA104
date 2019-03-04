class CCodecOregon : public CCodec
{
  uint8_t mNibbles[64];
  int mNibbleCount{0};
  int mBits{0};

  virtual const char* Id()
  {
      return "Oregon scientific weather station";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        int last = -1;

        if (count < 20)
            return false;
      
        uint8_t buffer[200];
        int bufferi = 0;
        auto PushManchester = [&](int n)
        {
            if (last == -1)
            {
                last = n;
                return true;
            }
            if (last == 1 && n == 0)
            {
                buffer[bufferi++] = 1;
            }
            else if (last == 0 && n == 1)
            {
                buffer[bufferi++] = 0;
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

        int compensation = (data[3] - data[2])/2;
        
        for (int i=0, b=1; i<count; i++, b=1-b)
        {
            int v = data[i];

            if ((i & 1) == 0)
                v += compensation;
            else 
                v -= compensation;

            if (v >= 300 && v < 700)
            {
                if (!PushManchester(b))
                    return false;
            }
            if (v >= 800 && v < 1100)
            {
                if (!PushManchester(b))
                    return false;
                if (!PushManchester(b))
                    return false;
            }
        }
        
        PushManchester(0);
        
        int i = 0;
        for (int i=0; i<bufferi; i++)
            if (buffer[i] == 0)
                break;
        
        for (; i<bufferi; i++)
            if (buffer[i] == 0)
                break;
        
        mBits = bufferi;
        mNibbleCount = 0;
        for (; i<bufferi-3; i+=4, mNibbleCount++)
            mNibbles[mNibbleCount] = buffer[i] + buffer[i+1]*2 + buffer[i+2]*4 + buffer[i+3]*8;

        return mNibbleCount >= 10;
  }

  virtual void GetShortInfo(char* info) 
  {
      strcpy(info, "?");
      
        if (mNibbleCount == 20 && mNibbles[0] == 0xA && mNibbles[1] == 0xF) // AF
        {
            // Oregon temperature sensor
            int temperature10 = mNibbles[11]*100 + mNibbles[10]*10 + mNibbles[9]*1;
            if (mNibbles[12] != 0)
                temperature10 = -temperature10;
            int humidity = mNibbles[14]*10 + mNibbles[13];
            sprintf(info, "Temperature: %d.%d\xf8" "C, Humidity: %d%%", temperature10/10, temperature10%10, humidity);
            return;
        }
        if (mNibbleCount == 23 && mNibbles[0] == 0xA && mNibbles[1] == 0x2) // A2
        {
            // Oregon temperature sensor
            int rainTotal = (int)(((mNibbles[18]*100000)+(mNibbles[17]*10000)+(mNibbles[16]*1000)+(mNibbles[15]*100)+(mNibbles[14]*10)+mNibbles[13])*24.2f/1000);
            int rainRate = (int)(((mNibbles[7]*100000)+(mNibbles[8]*10000)+(mNibbles[9]*1000)+(mNibbles[10]*100)+(mNibbles[11]*10)+mNibbles[12])*24.2f/1000);

            sprintf(info, "Total rain: %dmm, Rain rate: %dmm/hr", rainTotal, rainRate);
            return;
        }
        if (mNibbleCount == 22 && mNibbles[0] == 0xA && mNibbles[1] == 0x1) // A1
        {
            // Wind sensor
    	    int windspeed = int( ((mNibbles[16]*10)+ mNibbles[15])*3.6/10 );
    	    int gusts = int( ((mNibbles[13]*10)+ mNibbles[12])*3.6/10 );
   	    int heading = mNibbles[9];
            sprintf(info, "Wind: %d kmh (%d), Heading: %d", windspeed, gusts, heading);
            return;
        }
      
      
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
    int x = rcRect.left;
    int y = rcRect.top;
    x += BIOS::LCD::Printf(x, y, RGB565(B0B0B0), RGBTRANS, "Manchester coding %d bits", mBits);
    x = rcRect.left;
    y += 16;
    x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGBTRANS, "Raw: ");
    for (int i=0; i<mNibbleCount; i++)
    {
        x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGBTRANS, "%x", mNibbles[i]);
        if (x >= rcRect.right)
        {
            x = rcRect.left;
            y += 16;
        }
    }

    x = rcRect.left;
    y += 16;

    char info[64];
    GetShortInfo(info);
    BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, info);
  }
};
