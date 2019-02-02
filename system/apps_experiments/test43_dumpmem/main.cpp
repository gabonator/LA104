#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print("[%d]: %s\n", line, cond);
#ifdef __APPLE__
    kill(getpid(), SIGSTOP);
#endif
    while (1);
}

bool DumpMem(const char* filename, uint32_t address, uint32_t size)
{
  CBufferedWriter writer;
  if (!writer.Open( (char*)filename ))
    return false;

  writer << CStream((uint8_t*)address, size);
  writer.Close();
  return true;
}


#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
  BIOS::LCD::Clear(RGB565(404040));
  BIOS::DBG::Print("Dumping SRAM...");
  if (DumpMem("ram.dat", 0x20000000, 48*1024))
    BIOS::DBG::Print(" Ok\n");
  else
    BIOS::DBG::Print(" Error\n");

  BIOS::DBG::Print("Dumping FLASH...");
  if (DumpMem("rom1.dat", 0x08000000, 256*1024))
    BIOS::DBG::Print(" Ok\n");
  else
    BIOS::DBG::Print(" Error\n");

  BIOS::DBG::Print("Dumping extended FLASH...");
  if (DumpMem("rom2.dat", 0x08040000, 256*1024))
    BIOS::DBG::Print(" Ok\n");
  else
    BIOS::DBG::Print(" Error\n");

  return 0;
}

