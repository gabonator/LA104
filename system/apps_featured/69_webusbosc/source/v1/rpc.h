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

#if defined(LA104)
    if (strcmp(command, "SPI::begin")==0)
      return (uint32_t)SPI::begin;
    if (strcmp(command, "SPI::write")==0)
      return (uint32_t)static_cast<void(*)(uint8_t*, int)>(SPI::write); 

    if (strcmp(command, "IO::PinMode")==0)
      return (uint32_t)BIOS::GPIO::PinMode;
    if (strcmp(command, "IO::DigitalWrite")==0)
      return (uint32_t)BIOS::GPIO::DigitalWrite;
    if (strcmp(command, "IO::DigitalRead")==0)
      return (uint32_t)BIOS::GPIO::DigitalRead;
#endif

#if defined(DS203)
    // Oscilloscope
    if (strcmp(command, "OSC::Enable")==0)
      return (uint32_t)BIOS::ADC::Enable;  // malo by zavolat aj reset!
    if (strcmp(command, "OSC::Transfer")==0)
      return (uint32_t)BIOS::ADC::Transfer;
    if (strcmp(command, "OSC::Ready")==0) 
      return (uint32_t)BIOS::ADC::Ready;     // vracia false?
    if (strcmp(command, "OSC::ConfigureInput")==0)
      return (uint32_t)BIOS::ADC::ConfigureInput;
    if (strcmp(command, "OSC::ConfigureTrigger")==0)
      return (uint32_t)BIOS::ADC::ConfigureTrigger;
    if (strcmp(command, "OSC::GetPointer")==0)
      return (uint32_t)BIOS::ADC::GetPointer;
    if (strcmp(command, "OSC::ConfigureTimebase")==0)
      return (uint32_t)BIOS::ADC::ConfigureTimebase2;
    if (strcmp(command, "OSC::Restart")==0)
      return (uint32_t)BIOS::ADC::Restart;

    // Signal generator
    if (strcmp(command, "GEN::SetFrequency")==0)
      return (uint32_t)BIOS::DAC::SetFrequency;
    if (strcmp(command, "GEN::GetFrequency")==0)
      return (uint32_t)BIOS::DAC::GetFrequency;
    if (strcmp(command, "GEN::SetDuty")==0)
      return (uint32_t)BIOS::DAC::SetDuty;
    if (strcmp(command, "GEN::SetWave")==0)
      return (uint32_t)BIOS::DAC::SetWave;
#endif

    if (strcmp(command, "LCD::BufferBegin")==0)
      return (uint32_t)static_cast<void(*)(CRect const&)>(BIOS::LCD::BufferBegin);
    if (strcmp(command, "LCD::BufferWrite")==0)
      return (uint32_t)static_cast<void(*)(unsigned short*, int)>(BIOS::LCD::BufferWrite);
    if (strcmp(command, "LCD::Background")==0)
      return (uint32_t)static_cast<void(*)(CRect const&, uint32_t, uint32_t)>(GUI::Background);
    if (strcmp(command, "LCD::Bar")==0)
      return (uint32_t)static_cast<void(*)(CRect const&, uint16_t)>(BIOS::LCD::Bar);
    if (strcmp(command, "LCD::DrawSymbol")==0)
      return (uint32_t)static_cast<int(*)(int, int, uint16_t, uint16_t, const char*)>(BIOS::LCD::Draw);
    if (strcmp(command, "LCD::Print")==0)
      return (uint32_t)static_cast<int(*)(int, int, uint16_t, uint16_t, const char*)>(BIOS::LCD::Print);

    return 0;
  }

  uint32_t GetProcAddress(char* name)
  {
    uint32_t addr = GetRpcAdress(name);
    if (addr)
      return addr;

//    if (memcmp(name, "_ZN4BIOS", 8) == 0)
//      addr = ::GetProcAddress(name);

    return addr;
  }
}
