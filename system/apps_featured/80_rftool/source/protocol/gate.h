class CGate : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 868280000UL; // not verified
	}

  virtual void Example(CAttributes& attributes) override
  {
    attributes["length"] = 64;
    attributes["data_0"] = 0x12345678;
    attributes["data_1"] = 0x12345678;
  }

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
      int pulseLong = PulseDuration(2);
      int pulseShort = PulseDuration(1);

//      pulse.Add(pulseShort);
//      pulse.Add(pulseShort*9);
      pulse.Add(pulseLong);

      for (int i=0; i<length; i++)
      {
          int mask = 1<<(7-(i&7));
          int bit = nibbles[i/8] & mask;
          
          pulse.Add(bit ? pulseLong : pulseShort);
      }
pulse.Add(pulseLong); // trailing zeroes
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
