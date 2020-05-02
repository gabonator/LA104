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
    return 16;
  }

  virtual bool Identify(CArray<int>& pulse)
  {
    if (PulseLen(pulse[0]) == 1 && 
        PulseLen(pulse[1]) == 1 &&
        PulseLen(pulse[2]) == 1 &&
        PulseLen(pulse[3]) == 1 &&
        PulseLen(pulse[4]) == 1)
    {
      return true;
    }
    return false;
  }

  virtual int AttackPoint(CArray<int>& pulse)
  {
    if (pulse.GetSize() < 8)
      return 0;
    return 8*1000; // 8 ms
  }

  virtual void Example(CAttributes& attributes)
  {
    attributes[""] = 2;
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
    attributes["unk"] = unk;
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
    nibbles.Add(attr["battery_low"] | (attr["unk"] << 1));
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

