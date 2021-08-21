//#include "../../../os_host/source/main/Shared.h"

namespace RPC
{
  uint32_t GetRpcAdress(char* command)
  {
    if (strcmp(command, "SYS::GetProcAddress")==0)
      return (uint32_t)MEMORY::GetProcAddress2;

    if (strcmp(command, "MEM::Write")==0)
      return (uint32_t)MEMORY::Write;
    if (strcmp(command, "MEM::Read")==0)
      return (uint32_t)MEMORY::Read;
    if (strcmp(command, "MEM::Zero")==0)
      return (uint32_t)MEMORY::Zero;
    if (strcmp(command, "MEM::Done")==0)
      return (uint32_t)MEMORY::Done;

    if (strcmp(command, "DBG::Exec")==0)
      return (uint32_t)MEMORY::Exec;
    if (strcmp(command, "DBG::Stop")==0)
      return (uint32_t)MEMORY::Stop;
    if (strcmp(command, "DBG::Frame")==0)
      return (uint32_t)MEMORY::Frame;
    if (strcmp(command, "DBG::Running")==0)
      return (uint32_t)MEMORY::Running;
    if (strcmp(command, "DBG::ReturnValue")==0)
      return (uint32_t)MEMORY::ReturnValue;

    if (strcmp(command, "DBG::Screenshot")==0)
      return (uint32_t)MEMORY::Screenshot;

    if (strcmp(command, "DBG::Resume")==0)
      return (uint32_t)MEMORY::Resume;

//    if (strcmp(command, "DBG::GetDebugBuffer")==0)
//      return (uint32_t)MEMORY::GetDebugBuffer;
//    if (strcmp(command, "DBG::Trace")==0)
//      return (uint32_t)MEMORY::Trace;

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
