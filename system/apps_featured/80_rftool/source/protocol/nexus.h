class CNexus : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 433876000UL; // not verified
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

  virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
  {
    uint8_t nibblesData[20];
    CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

    int length;

    if (!PulseToBytes(pulse, b, length))
      return false;
      /*
//TODO: !!!!!!
    attributes["length"] = length;

    int bytes = b.GetSize();
    int data = 0;
    for (int i=0; i<bytes; i++)
    {
      bool last = i==bytes-1;
      data <<= 8;
      data |= b[i];
      if ((i&3)==3 || last)
      {
        switch (i/4) // store as dword
        {
          case 0: attributes["data64_0"] = data; break;
          case 1: attributes["data64_1"] = data; break;
          case 2: attributes["data64_2"] = data; break;
          default: _ASSERT(0);
        }
        data = 0;
      }
    }
      */
      BitstreamToAttributes(b, length, attributes);
      Analyse(attributes, b, length);
      return true;
  }
    
    bool Analyse(CAttributes& attributes, CArray<uint8_t>& b, int length)
    {
      if (length < 36)
          return false;
        
      if (b.GetSize() != 5)
      {
          _ASSERT(0);
          return false;
      }

        if ((b[3]&0xF0) != 0xF0)
            return false;

        /* Nibble 0,1 contains id */
        attributes["id"] = b[0];

        /* Nibble 2 is battery and channel */
        attributes["battery_low"] = (b[1]&0x80) ? true : false;
        attributes["channel"] = ((b[1]&0x30) >> 4) + 1;

        /* Nibble 3,4,5 contains 12 bits of temperature
         * The temperature is signed and scaled by 10 */
        int temp = (int16_t)((uint16_t)(b[1] << 12) | (b[2] << 4));
        temp = temp >> 4;
        attributes["temperature10"] = temp;

        /* Nibble 6,7 is humidity */
        attributes["humidity"] = (uint8_t)(((b[3]&0x0F)<<4)|(b[4]>>4));

    return true;
  }

  virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
  {
/*
    int temp = attr["temperature10"];
    int hum = (attr["humidity"] + 28) | 128;

    int nibblesData[9];
    CArray<int> nibbles(nibblesData, COUNT(nibblesData));

    nibbles.Add(attr["id"] & 0xf);
    nibbles.Add(Reverse2(attr["channel"]) | (attr["id"] >> 4) << 2);
    nibbles.Add(attr["battery_low"] | (attr["junk"] << 1));
    nibbles.Add(temp & 15);
    nibbles.Add((temp >> 4) & 15);
    nibbles.Add((temp >> 8) & 15);
    nibbles.Add(hum & 15);
    nibbles.Add((hum >> 4) & 15);
    
    nibbles.Add(Sum(nibbles) & 15);

    NibblesToPulse(nibbles, pulse);
*/
    return true;
  }
    
    virtual int PulseDivisor() override { return 500; }

private:
  int PulseLen(int microseconds)
  {
    return (microseconds+250)/500;
  }

  int PulseDuration(int ticks)
  {
      return ticks*500;
  }

    bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
    {
        if (pulse.GetSize() < 9*4*2)
            return false;

        int base = 0;

        if (PulseLen(pulse[0]) != 1)
            return false;
        if (PulseLen(pulse[1]) >= 7 && PulseLen(pulse[1]) <= 9)
        {
            base = 2;
            if (pulse.GetSize()-base < 9*4*2)
                return false;
        }
        else if (PulseLen(pulse[1]) != 2 && PulseLen(pulse[1]) != 4)
            return false;

        length = 0;
        int nibble = 0;
        int i = 0;
        for (i=0; i<9*4; i++)
        {
            if (PulseLen(pulse[base++]) != 1) 
                return false;

            int spacer = PulseLen(pulse[base++]);

            nibble <<= 1;
            if (spacer == 2)
            {
            } else
            if (spacer == 4)
            {
              nibble |= 1;
            } else
              return false;

            length++;

            if ((i&7) == 7)
            {
                bytes.Add(nibble);
                if (bytes.GetMaxSize() == bytes.GetSize())
                    return true;
                nibble = 0;
            }
        }

        if ((i&7) != 0) // TODO!
        {
            bytes.Add(nibble);
            if (bytes.GetMaxSize() == bytes.GetSize())
                return true;
            nibble = 0;
        }
 
        return true;
    }

  bool NibblesToPulse(const CArray<int>& nibbles, CArray<uint16_t>& pulse)
  {
    if (nibbles.GetSize() != 9)
      return false;

    pulse.Add(PulseDuration(1));
    pulse.Add(PulseDuration(8));

    for (int i=0; i<nibbles.GetSize(); i++)
    {
      for (int j=0; j<4; j++)
      {
        pulse.Add(PulseDuration(1));
        pulse.Add(PulseDuration((nibbles[i] & (1<<j)) ? 4 : 2));
      }
    }

    return true;
  }

  int Reverse2(int x)
  {
    return ((x & 1) ? 2 : 0) + ((x & 2) ? 1 : 0);
  }
    
    virtual void GetName(char* name) override
    {
        strcpy(name, "Nexus Temperature & Hum");
    }
    
    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        if (attributes.indexOf("temperature10") != -1)
        {
            if (attributes["humidity"] == 0)
            {
                sprintf(desc, "Ch: <%d> Temp: <%d.%d\xf8""C> Humidity: <%d%%>",
                (int)attributes["channel"], (int)attributes["temperature10"] / 10, (int)attributes["temperature10"] % 10, (int)attributes["humidity"]);
            } else
            {
                sprintf(desc, "Ch: <%d> Temp: <%d.%d\xf8""C>",
                (int)attributes["channel"], (int)attributes["temperature10"] / 10, (int)attributes["temperature10"] % 10);
            }
        } else
        {
            sprintf(desc, "%d bits: <%08x> <%08x>",
                (int)attributes["length"], (int)attributes["data_0"], (int)attributes["data_1"]);
        }
    }
    virtual const char* GetString(int i) override { return nullptr; }
};

