struct TKeyValue
{
  char* key;
  uintptr_t value;
};

class CAttributes : public CArray<TKeyValue>
{
public:
	typedef TKeyValue TAttribute;
	
public:
  CAttributes(TAttribute* attr, int count) : CArray<TKeyValue>(attr, count)
	{		
	}
	
  uintptr_t& operator[](const char* key)
  {
    for (int i=0; i<GetSize(); i++)
      if (strcmp(CArray<TKeyValue>::operator[](i).key, key) == 0)
        return CArray<TKeyValue>::operator[](i).value;

    Add(TKeyValue{(char*)key, 0});
    return GetLast().value;
  }

  int operator[](const char* key) const
  {
    for (int i=0; i<GetSize(); i++)
      if (strcmp(CArray<TKeyValue>::operator[](i).key, key) == 0)
        return CArray<TKeyValue>::operator[](i).value;

    _ASSERT(0);
    return 0;
  }

    const TKeyValue& operator[](int i) const
    {
        return CArray<TKeyValue>::operator[](i);
    }

    TKeyValue& operator[](int i)
    {
        return CArray<TKeyValue>::operator[](i);
    }

  int indexOf(const char* key)
  {
    for (int i=0; i<GetSize(); i++)
      if (strcmp(CArray<TKeyValue>::operator[](i).key, key) == 0)
        return i;
    return -1;
  }
};

class CProtocol
{
public:
    virtual int Frequency() = 0;
    /*
    // TODO: remove
    virtual int MinIndentifyCount() { return 0; }
    virtual int MinDemodulateCount() { return 0; }
    virtual int AttackPoint(CArray<int>& pulse) { return 0; }
    virtual bool Identify(CArray<int>& pulse) { return false; }
    */
    virtual void Example(CAttributes& attributes) = 0;
    virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) = 0;
    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) = 0;
    virtual void Synthesize(CAttributes& attr) {}

    virtual void GetName(char*) = 0;
    virtual void GetDescription(CAttributes& attributes, char* desc) = 0;
    virtual const char* GetString(int i) = 0;
    virtual int PulseDivisor() = 0;

  void PulseToBitstream(const CArray<uint16_t>& pulse, CArray<uint8_t>& bitstream, int interval)
  {
    int n = 0;

    for (int i=0; i<pulse.GetSize(); i++)
    {
      for (int j=0; j<pulse[i]; j+=interval, n++)
      {
        int bit = 1-(i&1);
        int byteIndex = n >> 3;
        int bitIndex = n & 7;
        if (bitIndex == 0)
          bitstream.Add(0);

        bitstream[byteIndex] |= bit << (7-bitIndex);
      }
    }
  }
    
    void BitstreamToAttributes(CArray<uint8_t>& b, int bitLength, CAttributes& attributes)
    {
        attributes["length"] = bitLength; // count of bits
        uint32_t data=0;
        int bytes = b.GetSize();
        for (int i=0; i<bytes; i++) // per each byte
        {
            bool last = i==bytes-1;
            data |= b[i] << (8*(3-(i&3)));
            if ((i&3)==3 || last)
            {
                switch (i/4) // store as dword
                {
                    case 0: attributes["data_0"] = data; break;
                    case 1: attributes["data_1"] = data; break;
                    case 2: attributes["data_2"] = data; break;
                    default: _ASSERT(0);
                }
                data = 0;
            }
        }
    }
    
    void AttributesToBitstream(const CAttributes& attributes, CArray<uint8_t>& b, int& bitLength)
    {
        bitLength = attributes["length"];
        uint32_t data;
        int bytes = (bitLength+7)/8;
        for (int i=0; i<bytes; i++)
        {
            if ((i&3) == 0)
            {
                switch (i/4)
                {
                case 0: data = attributes["data_0"]; break;
                case 1: data = attributes["data_1"]; break;
                case 2: data = attributes["data_2"]; break;
                default: _ASSERT(0);
                }
            }
            b.Add(data >> 24);
            data <<= 8;
        }
    }


};
