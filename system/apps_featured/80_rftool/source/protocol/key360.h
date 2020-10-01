class CKey360 : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 433920000UL;
	}

  virtual void Example(CAttributes& attributes) override
  {
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["length"] = 64;
    attributes["data_0"] = 0x12345678;
    attributes["data_1"] = 0xabcdef01;
  }

  virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
  {
    uint8_t nibblesData[16];
    CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

    int length = 0;
    if (!PulseToBytes(pulse, b, length))
      return false;
      
    BitstreamToAttributes(b, length, attributes);
    Analyse(attributes);
    return true;
  }

  void Analyse(CAttributes& attributes)
  {
      uint32_t d0 = attributes["data_0"];

      if (attributes["length"] == 64 && (d0 >> 24) == 0)
      {
          attributes["key"] = (d0 >> 16) & 255;
          attributes["counter"] = (d0 >> 8) & 255;
      }
  }

  virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
  {
      uint8_t nibblesData[16];
      CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

      int length = 0;
      AttributesToBitstream(attr, b, length);
      return BytesToPulse(b, length, pulse);

  }
    
    virtual int PulseDivisor() override { return 360; }

private:

  int PulseLen(int microseconds)
  {
      return (microseconds+360/2)/360;
  }

  int PulseDuration(int ticks)
  {
      return ticks*360;
  }

  bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
  {
    int i;
    for (i=0; i<pulse.GetSize()-4; i++)
    {
      int t = PulseLen(pulse[i]);
      if (t >= 10 && t <= 12)
      {
        i++;
        break;
      }
      if (t != 1)
        return false;
    }

    if (i < 10) // should be 23
      return false;

    length = 0;
    int bits = 0;
    for (; i<pulse.GetSize()-1; i+=2)
    {
      int p0 = PulseLen(pulse[i]);
      int p1 = PulseLen(pulse[i+1]);
      if (p0==2 && p1==1)
      {
        length++;
        bits <<= 1;
        bits |= 0;
      }
      else if (p0==1 && p1==2)
      {
        length++;
        bits <<= 1;
        bits |= 1;
      }
      else        
      {
        return false;
      }

      if ((length & 7) == 0)
      {
        // swap nibbles
        bits = reverse(bits);
        bytes.Add(bits);
        bits = 0;
      }
    }
    if ((length & 7) != 0)
    {
      bits = reverse(bits);
      bytes.Add(bits);
    }
    return true;
  }

  unsigned char reverse(unsigned char b) {
     b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
     b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
     b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
     return b;
  }

  bool BytesToPulse(const CArray<uint8_t>& bytes, int length, CArray<uint16_t>& pulse)
  {                                   
    const int preambule = 23; // parne cislo?
    for (int i=0; i<preambule; i++)
      pulse.Add(PulseDuration(1));

    pulse.Add(PulseDuration(11));

    for (int i=0; i<length; i++)
    {
      int bit = (bytes[i/8] >> (i&7)) & 1;
      if (bit)
      {
        pulse.Add(PulseDuration(1));
        pulse.Add(PulseDuration(2));
      } else
      {
        pulse.Add(PulseDuration(2));
        pulse.Add(PulseDuration(1));
      }
    }
    return true;
  }

    virtual void GetName(char* name) override
    {
        strcpy(name, "Key360");
    }
    
    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        uint32_t d0 = attributes["data_0"];
        uint32_t d1 = attributes["data_1"];

        if (attributes.indexOf("key") != -1)
        {
            sprintf(desc, "key: <%02x> counter: <%02x> bits: <%08x>", (int)attributes["key"], (int)attributes["counter"], (int)d1);
        } else {
            sprintf(desc, "%d bits: <%08x %08x>", (int)attributes["length"], (int)d0, (int)d1);
        }
    }
    
    virtual const char* GetString(int i) override { return nullptr; }
};

