class CWeather : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 433876000UL;
	}
    
    virtual void Example(CAttributes& attributes) override
    {
        // -512 (-51.2 C) ... 999 (+99.9 C)
        attributes["length"] = 171; // 17.1 C
        attributes["data_0"] = 0xce01f0ff; // 99 %
        attributes["data_1"] = 0x80000000;
        Synthesize(attributes);
    }

    virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
    {
        uint8_t nibblesData[16];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

        int length = 0;
        if (!PulseToNibbles(pulse, b, length))
            return false;

        BitstreamToAttributes(b, length, attributes);
        Analyse(attributes);
        return true;
    }
    
    uint32_t GetNibble(const CAttributes& attributes, int i)
    {
        uint32_t d = 0;
        if (i<8)
            d = attributes["data_0"];
        else if (i<16)
            d = attributes["data_1"];
        else
            _ASSERT(0);
        
        i &= 7;
        d >>= (7-i)*4;
        d &= 15;
        //int q = d;
        //_ASSERT(q >= 0 && q <= 15);
        return d;
    }

    void SetNibble(CAttributes& attributes, int i, uint32_t v)
    {
        int index = -1;
        if (i<8)
            index = attributes.indexOf("data_0");
        else if (i<16)
            index = attributes.indexOf("data_1");
        else
            _ASSERT(0);

        _ASSERT(index >= 0);
        _ASSERT(v >= 0 && v <= 15);

        i &= 7;
        int shift = (7-i)*4;
        
        uintptr_t& d = attributes[index].value;
        d &= ~(0xf << shift);
        d |= v << shift;
    }

    virtual void Analyse(CAttributes& attributes)
    {
        uint8_t b[9];   //appData.mAttributesBuffer
        for (int i=0; i<9; i++)
            b[i] = GetNibble(attributes, i);
        
        int sum_nibbles = b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + b[7];

        /* IIIICCII B???TTTT TTTTTSSS HHHHHHH1 XXXX */
        //int negative_sign = (b[5] & 7); // TODO: inversed bits??
        int temp          = b[3] | (b[4]<<4) | (b[5]<<8);
        int humidity      = ((b[6] | (b[7]<<4)) & 0x7f) - 28;
        int sensor_id     = b[0] | ((b[1] & 3) << 4);
        int battery_low   = b[2] & 1;
        int channel       = Reverse2(b[1] >> 2);
        bool crcOk        = b[8] == (sum_nibbles & 0xF);
    
        if (temp & 0x800)
            temp |= ~0x7ff;

        attributes["temperature10"] = temp;
        attributes["humidity"] = humidity;
        attributes["id"] = sensor_id;
        attributes["battery_low"] = battery_low;
        attributes["channel"] = channel;
        attributes["valid"] = crcOk;
    }

    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
    {
        uint8_t nibblesData[16];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

        int length = 0;
        AttributesToBitstream(attr, b, length);

        NibblesToPulse(b, length, pulse);
        return true;
    }
    
    virtual void Synthesize(CAttributes& attr) override
    {
        int temp = attr["temperature10"];
        int hum = (attr["humidity"] + 28) | 128;

        uint8_t nibblesData[9];
        CArray<uint8_t> nibbles(nibblesData, COUNT(nibblesData));

        nibbles.Add(attr["id"] & 0xf);
        nibbles.Add((Reverse2(attr["channel"])<<2) | (attr["id"] >> 4));
        nibbles.Add(attr["battery_low"] | (attr["junk"] << 1));
        nibbles.Add(temp & 15);
        nibbles.Add((temp >> 4) & 15);
        nibbles.Add((temp >> 8) & 15);
        nibbles.Add(hum & 15);
        nibbles.Add((hum >> 4) & 15);
        nibbles.Add(Sum(nibbles) & 15);

        for (int i=0; i<nibbles.GetSize(); i++)
            SetNibble(attr, i, nibbles[i]);
    }

    virtual int PulseDivisor() override { return 500; }
    
private:
  int Sum(const CArray<uint8_t>& arr)
  {
    int sum = 0;
    for (int i=0; i<arr.GetSize(); i++)
      sum += arr[i];
    return sum;
  }

  int PulseLen(int microseconds)
  {
    return (microseconds+250)/500;
  }

  int PulseDuration(int ticks)
  {
      return ticks*500;
  }

    bool PulseToNibbles(const CArray<uint16_t>& pulse, CArray<uint8_t>& nibbles, int& length)
    {
        if (pulse.GetSize() < 9*4*2)
            return false;

        static const int bitorder[] = {0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x04, 0x08};
        int nibble = 0, base = 0;
        if (PulseLen(pulse[0]) != 1)
            return false;
        if (PulseLen(pulse[1]) >= 10 && PulseLen(pulse[1]) <= 20)
        {
            base = 2;
            if (pulse.GetSize()-base < 9*4*2)
                return false;
        }
        else if (PulseLen(pulse[1]) != 4 && PulseLen(pulse[1]) != 8)
            return false;
        
        for (int i=0; i<9*4; i++)
        {
            int spacer = PulseLen(pulse[base+i*2]);
            int data = PulseLen(pulse[base+i*2+1]);
            if (spacer != 1)
                return false; //throw "wrong spacer value";
            if (data != 4 && data != 8)
                return false; //throw "wrong data value";

            int bit = data == 4 ? 0 : 1;
            int bitpos = i & 7;

            if (bit)
                nibble |= bitorder[bitpos];
            
            if (bitpos == 7)
            {
                nibbles.Add(nibble);
                nibble = 0;
            }
            length++;
        }
        // odd count
        nibbles.Add(nibble);
        return true;
    }

  bool NibblesToPulse(const CArray<uint8_t>& nibbles, int length, CArray<uint16_t>& pulse)
  {
    pulse.Add(PulseDuration(1));
    pulse.Add(PulseDuration(18));
    pulse.Add(PulseDuration(1));

    static const int bitorder[] = {0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x04, 0x08};

    for (int i=0; i<length; i++)
    {
        int bit = nibbles[i/8] & bitorder[i%8];
        pulse.Add(PulseDuration(bit ? 8 : 4));
        pulse.Add(PulseDuration(1));
    }

    return true;
  }

  int Reverse2(int x)
  {
    return ((x & 1) ? 2 : 0) + ((x & 2) ? 1 : 0);
  }
    
    virtual void GetName(char* name) override
    {
        strcpy(name, "TFA-Twin-Plus-30.3049");
    }

    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        if (attributes["humidity"] == 99)
        {
            sprintf(desc, "Ch: <%d> Temp: <%d.%d\xf8""C>",
            (int)attributes["channel"], (int)attributes["temperature10"] / 10, (int)attributes["temperature10"] % 10);
        }
        else
        {
            sprintf(desc, "Ch: <%d> Temp: <%d.%d\xf8""C> Humidity: <%d%%>",
                    (int)attributes["channel"], (int)attributes["temperature10"] / 10, (int)attributes["temperature10"] % 10, (int)attributes["humidity"]);
        }
    }

    virtual const char* GetString(int i) override { return nullptr; }
};

