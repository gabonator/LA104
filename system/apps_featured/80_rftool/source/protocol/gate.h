class CGate : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 868280000UL; // not verified
	}
	/*
  virtual int MinIndentifyCount() override
  {
    return 4;
  }

  virtual int MinDemodulateCount() override
  {
    return 3+9*4*2;
  }

//440, 8820, 440, 1940, 420, 1960, 420, 3980, 440, 3980, 440,

  virtual bool Identify(CArray<int>& pulse) override
  {
    return false;
  }

  virtual int AttackPoint(CArray<int>& pulse) override
  {
    return 0;
  }
*/
  virtual void Example(CAttributes& attributes) override
  {
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["temperature10"] = 171; // 17.1 C
    attributes["humidity"] = 99; // 99 %
    attributes["id"] = 38;
	attributes["battery_low"] = 0;
    attributes["channel"] = 1;
    attributes["junk"] = 2;
  }

//// 1 10 2 1 1 2 1 1 2 1 1 1 1 1 1 1 1 2 2 2 1 1 2 1 1 2 2 2 2 1 1 2 2 2 1 1 2 2 2 2 2 2 2 1 1 2 2 1 1 1 1 1 1 2 1 1 2 1 1 2 1 1 2 1 1 2 2 1

  virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
  {
    uint8_t nibblesData[20];
    CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

    int length;

    if (!PulseToBytes(pulse, b, length))
      return false;

    BitstreamToAttributes(b, length, attributes);

    return true;
  }

  virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
  {
      uint8_t nibblesData[20];
      CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

      int length = 0;
      AttributesToBitstream(attr, b, length);
      NibblesToPulse(b, length, pulse);


    return true;
  }
    
    virtual int PulseDivisor() override { return 1000; }

private:
  int PulseLen(int microseconds)
  {
    return (microseconds+500)/1000;
  }

  int PulseDuration(int ticks)
  {
      return ticks*1000;
  }
    

    bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
    {
        // 1 10 2 1 1 2 1 1 2 1 1 1 1 1 1 1 1 2 2 2 1 1 2 1 1 2 2 2 2 1 1 2 2 2 1 1 2 2 2 2 2 2 2 1 1 2 2 1 1 1 1 1 1 2 1 1 2 1 1 2 1 1 2 1 1 2 2 1
        if (pulse.GetSize() < 20)
            return false;

        int base = 0;

        if (PulseLen(pulse[0]) == 2)
        {
        } else
        if (PulseLen(pulse[0]) == 1 && (PulseLen(pulse[1]) >= 9 && PulseLen(pulse[1]) <= 11))
        {
          base = 2;
        } else
          return false;
	
        if (PulseLen(pulse[base++]) != 2)
            return false;

        length = 0;
        int nibble = 0;
        int i = 0;
        for (i=base; i<pulse.GetSize(); i++)
        {
            int p = PulseLen(pulse[base++]);

            if (p == 1)
            {
            } else
            if (p == 2)
            {
                nibble |= 1<<(7-(length&7));
            } else
            {
              if (p >= 9 && p <= 11)
                break;
              return false;
            }

            length++;

            if ((length&7) == 0)
            {
                bytes.Add(nibble);
                if (bytes.GetMaxSize() == bytes.GetSize())
                    return true; // full buffer?
                nibble = 0;
            }
        }

        if ((length&7) != 0) 
        {
            bytes.Add(nibble);
        }
 
        return true;
    }

  bool NibblesToPulse(const CArray<uint8_t>& nibbles, int length, CArray<uint16_t>& pulse)
  {
    //pulse.Add(PulseDuration(1));
    //pulse.Add(PulseDuration(9)); // 9..11

      int pulseLong = PulseDuration(2);
      int pulseShort = PulseDuration(1);

      pulse.Add(pulseLong);

      for (int i=0; i<length; i++)
      {
          int mask = 1<<(7-(i&7));
          int bit = nibbles[i/8] & mask;
          
          pulse.Add(bit ? pulseLong : pulseShort);
      }
      //pulse.Add(PulseDuration(1)); // asi na konci
    return true;
  }

  int Reverse2(int x)
  {
    return ((x & 1) ? 2 : 0) + ((x & 2) ? 1 : 0);
  }
    
    virtual void GetName(char* name) override
    {
        strcpy(name, "868 MHz remote key");
    }
    
    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        sprintf(desc, "Data: <%08x %08x>",
                (int)attributes["data_0"], (int)attributes["data_1"]);
    }
    virtual const char* GetString(int i) override { return nullptr; }
};

