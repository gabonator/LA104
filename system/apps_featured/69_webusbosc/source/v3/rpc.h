//#include "../../../os_host/source/main/Shared.h"

namespace RPC
{
  uint32_t GetRpcAdress(char* command)
  {
    if (strcmp(command, "DBG::Print")==0)
      return (uint32_t)BIOS::DBG::Print;

    if (strcmp(command, "MEM::GetBufferPtr")==0)
      return (uint32_t)MEMORY::GetBufferPtr;
    if (strcmp(command, "MEM::GetBufferSize")==0)
      return (uint32_t)MEMORY::GetBufferSize;
    if (strcmp(command, "MEM::Write")==0)
      return (uint32_t)MEMORY::Write;
    if (strcmp(command, "MEM::Read")==0)
      return (uint32_t)MEMORY::Read;
    if (strcmp(command, "MEM::Read32")==0)
      return (uint32_t)MEMORY::Read32;
    if (strcmp(command, "MEM::Write32")==0)
      return (uint32_t)MEMORY::Write32;
    if (strcmp(command, "MEM::Mask32")==0)
      return (uint32_t)MEMORY::Mask32;

#if defined(LA104)
    if (strcmp(command, "IO::PinMode")==0)
      return (uint32_t)BIOS::GPIO::PinMode;
    if (strcmp(command, "IO::DigitalWrite")==0)
      return (uint32_t)BIOS::GPIO::DigitalWrite;
    if (strcmp(command, "IO::DigitalRead")==0)
      return (uint32_t)BIOS::GPIO::DigitalRead;
#endif

#if defined(DS203) || defined(DS213)
    // Oscilloscope
    if (strcmp(command, "OSC::Enable")==0)
      return (uint32_t)BIOS::ADC::Enable;  // malo by zavolat aj reset!
    if (strcmp(command, "OSC::Transfer")==0)
      return (uint32_t)BIOS::ADC::Transfer;
    if (strcmp(command, "OSC::Ready")==0) 
      return (uint32_t)BIOS::ADC::Ready2;     // vracia false?
    if (strcmp(command, "OSC::ConfigureInput")==0)
      return (uint32_t)BIOS::ADC::ConfigureInput2;
    if (strcmp(command, "OSC::ConfigureTrigger")==0)
      return (uint32_t)BIOS::ADC::ConfigureTrigger2;
    if (strcmp(command, "OSC::GetPointer")==0)
      return (uint32_t)BIOS::ADC::GetPointer;
    if (strcmp(command, "OSC::ConfigureTimebase")==0)
      return (uint32_t)BIOS::ADC::ConfigureTimebase2;
    if (strcmp(command, "OSC::Restart")==0)
      return (uint32_t)BIOS::ADC::Restart;

    if (strcmp(command, "OSC::SetParam")==0)
      return (uint32_t)BIOS::ADC::SetParam;

    // Signal generator
    if (strcmp(command, "GEN::SetFrequency")==0)
      return (uint32_t)BIOS::DAC::SetFrequency;
    if (strcmp(command, "GEN::GetFrequency")==0)
      return (uint32_t)BIOS::DAC::GetFrequency;
    if (strcmp(command, "GEN::SetDuty")==0)
      return (uint32_t)BIOS::DAC::SetDuty;
    if (strcmp(command, "GEN::SetMode")==0)
      return (uint32_t)BIOS::DAC::SetMode;
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
