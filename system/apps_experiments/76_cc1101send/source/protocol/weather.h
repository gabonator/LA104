class CWeather : public CProtocol
{
public:
	virtual int Frequency()
	{
		return 433876000UL;
	}
	
  virtual int MinIndentifyCount()
  {
    return 4;
  }

  virtual int MinDemodulateCount()
  {
    return 3+9*4*2;
  }

  virtual bool Identify(CArray<int>& pulse)
  {
    auto IsSpacer = [](int l){ return l >= 18 && l <= 25; };
    auto IsShort = [](int l){ return l >= 4 && l <= 5; };
    auto IsLong = [](int l){ return l >= 8 && l <= 10; };
    
    if (PulseLen(pulse[0]) == 1 && 
        IsSpacer(PulseLen(pulse[1])) &&
        PulseLen(pulse[2]) == 1 &&
        (IsShort(PulseLen(pulse[3])) ||
        IsLong(PulseLen(pulse[3]))))
    {
      return true;
    }
    return false;
  }

  virtual int AttackPoint(CArray<int>& pulse)
  {
    if (pulse.GetSize() < 2 + 8*4*2)
      return 0;
    // points to CRC, min length 8*5ms, max length 8*9ms
    // packet is sent 4 times!!!
    return 2000000; // 2 s noise 
  }

  virtual void Example(CAttributes& attributes)
  {
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["temperature10"] = 171; // 17.1 C
    attributes["humidity"] = 99; // 99 %
    attributes["id"] = 38;
	attributes["battery_low"] = 0;
    attributes["channel"] = 1;
    attributes["junk"] = 2;
  }

  virtual bool Demodulate(const CArray<int>& pulse, CAttributes& attributes)
  {
    int nibblesData[9];
    CArray<int> b(nibblesData, COUNT(nibblesData));

    if (!PulseToNibbles(pulse, b))
      return false;

    int sum_nibbles = b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + b[7];

    /* IIIICCII B???TTTT TTTTTSSS HHHHHHH1 XXXX */
    int negative_sign = (b[5] & 7);
    int temp          = b[3] | (b[4]<<4) | ((b[5]>>3)<<8);
    int humidity      = ((b[6] | (b[7]<<4)) & 0x7f) - 28;
    int sensor_id     = b[0] | ((b[1] >> 2) << 4);
    int battery_low   = b[2] & 1;
    int channel       = Reverse2(b[1] >> 2);
    int unk           = b[2] >> 1;
    bool crcOk        = b[8] == (sum_nibbles & 0xF);

    int tempC10 = negative_sign ? -( (1<<9) - temp ) : temp;

    attributes["temperature10"] = tempC10;
    attributes["humidity"] = humidity;
    attributes["id"] = sensor_id;
    attributes["battery_low"] = battery_low;
    attributes["channel"] = channel;
    attributes["junk"] = unk;
    attributes["valid"] = crcOk;
    return true;
  }

  virtual bool Modulate(const CAttributes& attr, CArray<int>& pulse)
  {
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
    return true;
  }

private:
  int Sum(const CArray<int>& arr)
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

  bool PulseToNibbles(const CArray<int>& pulse, CArray<int>& nibbles)
  {
    if (pulse.GetSize() < 9*4*2+3)
      return false;

    int nibble = 0;
    for (int i=0; i<9*4; i++)
    {
      int spacer = PulseLen(pulse[2+i*2]);
      int data = PulseLen(pulse[2+i*2+1]);
      if (spacer != 1)
        return false; //throw "wrong spacer value";
      if (data != 4 && data != 8)
        return false; //throw "wrong data value";

      int bit = data == 4 ? 0 : 1;
      int bitpos = i & 3;
     
      nibble |= bit << bitpos;
      if (bitpos == 3)
      {
        nibbles.Add(nibble);
        nibble = 0;
      }
    }
    return true;
  }

  bool NibblesToPulse(const CArray<int>& nibbles, CArray<int>& pulse)
  {
    if (nibbles.GetSize() != 9)
      return false;

    pulse.Add(PulseDuration(1));
    pulse.Add(PulseDuration(18));
    pulse.Add(PulseDuration(1));

    for (int i=0; i<nibbles.GetSize(); i++)
    {
      for (int j=0; j<4; j++)
      {
        pulse.Add(PulseDuration((nibbles[i] & (1<<j)) ? 8 : 4));
        pulse.Add(PulseDuration(1));
      }
    }

    return true;
  }

  int Reverse2(int x)
  {
    return ((x & 1) ? 2 : 0) + ((x & 2) ? 1 : 0);
  }
};

