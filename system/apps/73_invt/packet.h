template<int N>
class CPacket
{
private:
  uint8_t mData[N];
  uint8_t mLength{0};
  const uint8_t mMaxLength{N};
  
public:
  uint8_t getLength() const
  {
    return mLength;
  }

  bool isFull()
  {
    return mLength >= mMaxLength;
  }
  
  void clear()
  {
    mLength = 0;
  }

  void add(uint8_t data)
  {
    _ASSERT(!isFull());
    mData[mLength++] = data;    
  }

  void add(uint16_t data)
  {
    add((uint8_t)(data >> 8));
    add((uint8_t)(data & 0xff));    
  }

  uint16_t getWord(int i) const
  {
    _ASSERT(i+1 < mLength);
    return (mData[i]<<8) | mData[i+1];
  }
  
  uint8_t& operator[](int i)
  {
    _ASSERT(i < mLength);
    return mData[i];
  }

  const uint8_t& operator[](int i) const
  {
    _ASSERT(i < mLength);
    return mData[i];
  }

  const uint8_t* getData() const
  {
    return mData;
  }
};

typedef CPacket<32> CVfdPacket;
