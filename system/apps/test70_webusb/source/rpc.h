#include "../../../os_host/source/main/Shared.h"

namespace RPC
{
  uint32_t GetRpcAdress(char* command)
  {
    if (command[0] != 'M' && command[0] != 'L' && command[0] != 'A')
      return 0;

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

    if (strcmp(command, "LCD::Width")==0)
      return (uint32_t)static_cast<int(*)()>([](){ return BIOS::LCD::Width; });
    if (strcmp(command, "LCD::Height")==0)
      return (uint32_t)static_cast<int(*)()>([](){ return BIOS::LCD::Height; });
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

    if (strcmp(command, "ANAL::Configure")==0)
      return (uint32_t)ANALYSER::Configure;
    if (strcmp(command, "ANAL::Status")==0)
      return (uint32_t)ANALYSER::Status;
    if (strcmp(command, "ANAL::Restart")==0)
      return (uint32_t)ANALYSER::Restart;
    if (strcmp(command, "ANAL::Get")==0)
      return (uint32_t)ANALYSER::Get;


    return 0;
  }

  uint32_t GetProcAddress(char* name)
  {
    uint32_t addr = GetRpcAdress(name);
    if (addr)
      return addr;

    if (memcmp(name, "_ZN4BIOS", 8) == 0)
      addr = ::GetProcAddress(name);

    return addr;
  }
}
