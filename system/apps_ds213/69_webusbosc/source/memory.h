namespace MEMORY
{
  uint8_t buffer[2048];

  uint32_t GetBufferSize()
  {
    return sizeof(buffer);
  }

  uint32_t GetBufferPtr()
  {
    return (uint32_t)buffer;
  }

  int hex(char ch)
  {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    _ASSERT(0);
    return 0;
  }

  void Write(uint32_t address, char* buf)
  {
    _ASSERT(buf);
    uint8_t* ptr = (uint8_t*)address;

    while (buf[0] && buf[1])
    {
      *ptr++ = (hex(buf[0]) << 4) | hex(buf[1]);
      buf+=2;
    }
  }

  void Read(uint32_t ptr, int n)
  {
    TERMINAL::Print("{raw:%d}", n);
    uint8_t* p = (uint8_t*)ptr;
    while (n--)
    {
      TERMINAL::Print("%02x", *p++);
//      TERMINAL::Write((uint8_t*)ptr, n);
    }
  }

  uint32_t Read32(uint32_t ptr)
  {
    uint32_t* p = (uint32_t*)ptr;
    return *p;
  }

  uint32_t Write32(uint32_t ptr, uint32_t value)
  {
    uint32_t* p = (uint32_t*)ptr;
    *p = value;
    return *p;
  }

  uint32_t Mask32(uint32_t ptr, uint32_t mask, uint32_t value)
  {
    uint32_t* p = (uint32_t*)ptr;
    *p &= ~mask;
    *p |= value;
    return *p;
  }
}
