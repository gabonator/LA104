class CVw : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 434420000;
	}
	
  virtual int MinIndentifyCount() override
  {
    return 0;
  }

  virtual int MinDemodulateCount() override
  {
    return 0;
  }

  virtual bool Identify(CArray<int>& pulse) override
  {
    return false;
  }

  virtual int AttackPoint(CArray<int>& pulse) override
  {
    return 0;
  }

  virtual void Example(CAttributes& attributes) override
  {
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["length"] = 64;
    attributes["data64_0"] = 0x12345678;
    attributes["data64_1"] = 0xabcdef01;
    attributes["cmd"] = 0;
  }

  virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
  {
    int nibblesData[16];
    CArray<int> b(nibblesData, COUNT(nibblesData));

    int length = 0;
    if (!PulseToBytes(pulse, b, length))
      return false;

    if (length == 79)
    {
      // trailing 7 bits is opcode
      if (b[9] == 0x13)
        attributes["opcode"] = 1;
      if (b[9] == 0x3f)
        attributes["opcode"] = 2;
      if (b[9] == 0x64)
        attributes["opcode"] = 3;
    }

    attributes["length"] = length; // count of bits
    uint32_t data=0;
    int bytes = b.GetSize();
    for (int i=0; i<bytes; i++) // per each byte
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
    Analyse(attributes);
    return true;
  }

  void Analyse(CAttributes& attributes)
  {
  }

  virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
  {
    int nibblesData[16];
    CArray<int> b(nibblesData, COUNT(nibblesData));
    int length = attr["length"];
    uint32_t data;
    int bytes = (length+7)/8;
    for (int i=0; i<bytes; i++)
    {
      if ((i&3) == 0)
      {
        switch (i/4)
        {
          case 0: data = attr["data64_0"]; break;
          case 1: data = attr["data64_1"]; break;
          case 2: data = attr["data64_2"]; break;
          default: _ASSERT(0);
        }
      }
      b.Add(data >> 24);
      data <<= 8;
    }

    return BytesToPulse(b, length, pulse);
  }

private:

  int PulseLen(int microseconds)
  {
      return (microseconds+250/2)/250;
  }

  int PulseDuration(int ticks)
  {
      return ticks*250;
  }

  bool match(const CArray<uint16_t>& pulse, int& i, const char* pattern)
  {
    int l = strlen(pattern);
    for (int j=0; j<l; j++)
      if (PulseLen(pulse[i+j]) != pattern[j] - '0')
        return false;

    i += l;
    return true;
  }

  bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<int>& bytes, int& length)
  {
/*
222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
423333332
22422222222222222222222422224222222224224444222242242222224224422442222222242222222244222222224444222244224222244222222224222244"
*/
    int i;
    for (i=0; i<pulse.GetSize()-4; i++)
    {
      int t = PulseLen(pulse[i]);
      if (t == 4)
        break;
      if (t != 2)
        return false;
    }

    if (i < 50) // should be 252
      return false;

    if (!match(pulse, i, "423333332"))
      return false;

    length = 0;
    int bits = 0;
    for (; i<pulse.GetSize(); i++)
    {
      int l = PulseLen(pulse[i]);

      if (l == 2)
      {
        bool last = i==pulse.GetSize()-1;
        if (last)
          return false;

        int l1 = PulseLen(pulse[++i]);
        if (l1 != 2)
          return false;

        length++;
        bits <<= 1;
        bits |= 0;
      }
      else if (l == 4)
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
        bytes.Add(bits);
        bits = 0;
      }
    }
    if ((length & 7) != 0)
    {
      bytes.Add(bits);
    }
    return true;
  }

  bool BytesToPulse(const CArray<int>& bytes, int length, CArray<uint16_t>& pulse)
  {                                   
    const int preambule = 252; // parne cislo?
    for (int i=0; i<preambule; i++)
      pulse.Add(PulseDuration(2));

    const char* preamble = "423333332";
    for (int i=0; i<strlen(preamble); i++)
      pulse.Add(PulseDuration(preamble[i] - '0'));

    for (int i=0; i<length; i++)
    {
      int bit = bytes[i/8] >> ((i&7)&1);
      if (bit==0)
      {
        pulse.Add(PulseDuration(2));
        pulse.Add(PulseDuration(2));
      } else
      {
        pulse.Add(PulseDuration(4));
      }
    }
    return true;
  }

    virtual void GetName(char* name) override
    {
        strcpy(name, "Vw keyfob");
    }
    
    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
/*
        const char* opcode = "?";
        if (attributes.indexOf("opcode") != -1)
        {
          switch (attributes["opcode"])
          {
            case 0x13: opcode = "open"; break;
            case 0x3f: opcode = "close"; break;
            case 0x64: opcode = "trunk"; break;
          }
        }
        sprintf(desc, "%d bits: <%08x %08x> op: %s", attributes["length"], attributes["data64_0"], attributes["data64_1"], opcode);
*/
        sprintf(desc, "%d bits: <%08x %08x %04x>", attributes["length"], attributes["data64_0"], attributes["data64_1"], attributes["data64_2"]);
    }
};


/*
  if (r.substr(-7) == "AABAABB") // open 
  if (r.substr(-7) == "ABBBBBA") // close
  if (r.substr(-7) == "BBAABAA") // trunk

c8

11001000

 001 0011
 011 1110
 110 0100

 13, 3f, 64
 
13
3f

*/