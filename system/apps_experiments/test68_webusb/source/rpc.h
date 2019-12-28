//#include "../../../os_host/source/main/Shared.h"

namespace RPC
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

  void MemoryWrite(uint32_t address, char* buf)
  {
    _ASSERT(buf);
    uint8_t* ptr = (uint8_t*)address;

    while (buf[0] && buf[1])
    {
      *ptr++ = (hex(buf[0]) << 4) | hex(buf[1]);
      buf+=2;
    }
  }

  void MemoryRead(uint32_t ptr, int n)
  {
    TERMINAL::Print("{raw:%d}", n);
    uint8_t* p = (uint8_t*)ptr;
    while (n--)
    {
      TERMINAL::Print("%02x", *p++);
//      TERMINAL::Write((uint8_t*)ptr, n);
    }
  }
}

namespace RPC
{
/*
p = RPC::GetBufferPtr();
RPC::MemoryWrite(p, "fffffff0f0f0f0f0f0f0f");
SPI::begin(0);
SPI::write(p, 32);

// RGB 00.....00

00924924924924db6db600
00db6db692492492492400
00db6db6 924924 92492400

00924924924924db6db600

00924924db6db692492400

00db6db6
0:0x92,0x49,0x24,
255:0xDB,0x6D,0xB6
*/

  uint32_t GetRpcAdress(char* command)
  {
    if (strcmp(command, "RPC::GetBufferPtr")==0)
      return (uint32_t)RPC::GetBufferPtr;
    if (strcmp(command, "RPC::GetBufferSize")==0)
      return (uint32_t)RPC::GetBufferSize;
    if (strcmp(command, "RPC::MemoryWrite")==0)
      return (uint32_t)RPC::MemoryWrite;
    if (strcmp(command, "RPC::MemoryRead")==0)
      return (uint32_t)RPC::MemoryRead;
    if (strcmp(command, "RPC::Print")==0)
      return (uint32_t)BIOS::DBG::Print;

    if (strcmp(command, "SPI::begin")==0)
      return (uint32_t)SPI::begin;
    if (strcmp(command, "SPI::write")==0)
      return (uint32_t)static_cast<void(*)(uint8_t*, int)>(SPI::write); 

#ifdef LA104
    if (strcmp(command, "IO::PinMode")==0)
      return (uint32_t)BIOS::GPIO::PinMode;
    if (strcmp(command, "IO::DigitalWrite")==0)
      return (uint32_t)BIOS::GPIO::DigitalWrite;
    if (strcmp(command, "IO::DigitalRead")==0)
      return (uint32_t)BIOS::GPIO::DigitalRead;
#endif

    if (strcmp(command, "LCD::BufferBegin")==0)
      return (uint32_t)(uint32_t)static_cast<void(*)(CRect const&)>(BIOS::LCD::BufferBegin);
    if (strcmp(command, "LCD::BufferWrite")==0)
      return (uint32_t)static_cast<void(*)(unsigned short*, int)>(BIOS::LCD::BufferWrite);

    return 0;
  }

  uint32_t GetProcAddress(char* name)
  {
    if (memcmp(name, "RPC::", 5) == 0)
      return GetRpcAdress(name);
                            
    if (memcmp(name, "SPI::", 5) == 0)
      return GetRpcAdress(name);

    if (memcmp(name, "IO::", 4) == 0)
      return GetRpcAdress(name);

    if (memcmp(name, "LCD::", 5) == 0)
      return GetRpcAdress(name);

//    if (memcmp(name, "_ZN4BIOS", 8) == 0)
//      return ::GetProcAddress(name);

    return 0;
  }
}
