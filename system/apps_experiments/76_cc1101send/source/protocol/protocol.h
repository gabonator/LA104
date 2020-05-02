struct TKeyValue
{
  char* key;
  int value;
};

class CAttributes : public CArray<TKeyValue>
{
public:
	typedef TKeyValue TAttribute;
	
public:
  CAttributes(TAttribute* attr, int count) : CArray<TKeyValue>(attr, count)
	{		
	}
	
  int& operator[](const char* key)
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
};

class CProtocol
{
public:
	virtual int Frequency() = 0;
  virtual int MinIndentifyCount() = 0;
  virtual int MinDemodulateCount() = 0;
  virtual bool Identify(CArray<int>& pulse) = 0;
  virtual void Example(CAttributes& attributes) = 0;
  virtual bool Demodulate(const CArray<int>& pulse, CAttributes& attributes) = 0;
  virtual bool Modulate(const CAttributes& attr, CArray<int>& pulse) = 0;
  virtual int AttackPoint(CArray<int>& pulse) = 0;

  void PulseToBitstream(const CArray<int>& pulse, CArray<uint8_t>& bitstream, int interval)
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
};
