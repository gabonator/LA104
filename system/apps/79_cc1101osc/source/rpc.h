//#include "../../../os_host/source/main/Shared.h"

namespace RPC
{
  uint32_t GetRpcAdress(char* command)
  {
    #define EXPORT(symbol) if (strcmp(command, #symbol)==0) return (uint32_t)symbol;

    EXPORT(CC1101::SetFrequency);
    EXPORT(CC1101::GetFrequency);
    EXPORT(CC1101::SetBandwidth);
    EXPORT(CC1101::GetBandwidth);
    EXPORT(CC1101::SetBandwidth);
    EXPORT(CC1101::GetBandwidth);
    EXPORT(CC1101::SetGain);
    EXPORT(CC1101::GetGain);
    EXPORT(CC1101::SetDataRate);
    EXPORT(CC1101::GetDataRate);
    EXPORT(CC1101::SetOutputPower);

    EXPORT(CC1101::Init);
    EXPORT(CC1101::Start);
    EXPORT(CC1101::Stop);
    EXPORT(CC1101::Transfer);
    EXPORT(CC1101::Status);
    EXPORT(CC1101::Send);
    EXPORT(CC1101::Calibrate);

    EXPORT(CC1101::Strobe);
    EXPORT(CC1101::Write);
    EXPORT(CC1101::Read);

    EXPORT(APP::Info);
    EXPORT(APP::GetConfigPtr);
    EXPORT(APP::SetPrescaler);

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
    return GetRpcAdress(name);
  }
}
