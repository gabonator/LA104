class CCodecSamsung : public CCodec
{
  uint32_t mData;
  int mBits{0};

  virtual const char* Id()
  {
      return "Samsung TV remote control";
  }

  virtual bool Decode(uint16_t* data, int count)
  {
        mData = 0;
        mBits = 0;
        if (count < 20)
            return false;
     
        // preamble
        if (data[0] < 4000 || data[0] > 5000) // 4700
          return false;
        if (data[1] < 4000 || data[1] > 5000) // 4600
          return false;

        for (int i=2; i<count-1; i+=2)
        {
          if (data[i] <= 450 || data[i] >= 700) // 620..640
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
      case 0xE0E040BF: return "on/off";
      case 0xE0E0807F: return "source";
      case 0xE0E0F00F: return "mute";
      case 0xE0E020DF: return "1";
      case 0xE0E0A05F: return "2";
      case 0xE0E0609F: return "3";
      case 0xE0E010EF: return "4";
      case 0xE0E0906F: return "5";
      case 0xE0E050AF: return "6";
      case 0xE0E030CF: return "7";
      case 0xE0E0B04F: return "8";
      case 0xE0E0708F: return "9";
      case 0xE0E08877: return "0";
      case 0xE0E034CB: return "txt/mix";
      case 0xE0E0C837: return "pre-ch";
      case 0xE0E0E01F: return "volume +";
      case 0xE0E0D02F: return "volume -";
      case 0xE0E048B7: return "channel +";
      case 0xE0E008F7: return "channel -";
      case 0xE0E0D629: return "ch list";
      case 0xE0E058A7: return "menu";
      case 0xE0E031CE: return "media p";
      case 0xE0E0F20D: return "guide";
      case 0xE0E0F807: return "info";
      case 0xE0E0D22D: return "tools";
      case 0xE0E006F9: return "up";
      case 0xE0E0A659: return "left";
      case 0xE0E046B9: return "right";
      case 0xE0E08679: return "down";
      case 0xE0E016E9: return "enter";
      case 0xE0E01AE5: return "return";
      case 0xE0E0B44B: return "exit";
      case 0xE0E0FC03: return "e-manual";
      case 0xE0E07C83: return "pic size";
      case 0xE0E0A45B: return "ad/subt";
      case 0xE0E0629D: return "stop";
      case 0xE0E0E21D: return "play";
      case 0xE0E052AD: return "pause";
      case 0xE0E0A25D: return "rewind";
      case 0xE0E012ED: return "forward";
      case 0xE0E036C9: return "a";
      case 0xE0E028D7: return "b";
      case 0xE0E0A857: return "c";
      case 0xE0E06897: return "d";
      case 0xE0E0F906: return "3D";
      case 0xE0E09E61: return "Smart hub";
      default: return nullptr;
    }
  }
};
