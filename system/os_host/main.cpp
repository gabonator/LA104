#include "STM32F10x.h"
#include "source/bios/Bios.h"
#include "source/Execute.h"

/*
  shell.elf
  apps/generator/midi.elf
  fileman.elf apps/generator/midi -e apps/generator/midi.elf
  apps/generator.midi data/holiday.mid
*/

extern "C" uint32_t /*__attribute__((naked))*/ GetStackPointer();

class CTokenizer
{
  char* mString;

public:
  CTokenizer(char* str)
  {
    mString = str;
  }

  void GetToken(char* out, int maxLength)
  {
    for (int i=0; i<maxLength; i++)
    {
      out[i] = mString[i];
      if (mString[i] == 0)
      {
        mString += i;
        return;
      }
      if (mString[i] == ' ')
      {
        out[i] = 0;
        mString += i;
        mString++;
        return;
      }
    }
    mString += maxLength;
    out[maxLength] = 0;
  }
};

int main()
{
  BIOS::SYS::Init();
  BIOS::FAT::Init();
  BIOS::USB::Enable();
//  BIOS::USB::InitializeSerial();
  BIOS::USB::InitializeMass();
  BIOS::OS::SetArgument((char*)"");

  char shell[64] = "shell.elf";

  while (BIOS::KEY::GetKey() == BIOS::KEY::F3)
  {
      BIOS::DBG::Print("Safe mode, press F1 to continue.\n");
      while (BIOS::KEY::GetKey() != BIOS::KEY::F1);
  }

  while (1)
  {
    if (strlen(BIOS::OS::GetArgument()) == 0)
      BIOS::OS::SetArgument(shell);

    uint32_t address = 0;
    char filename[64];
    {
      CTokenizer tok(BIOS::OS::GetArgument());
      tok.GetToken(filename, 63);
      address = ElfExecute(filename);
    }

    if (!address)
    {
      BIOS::DBG::Print("Press F1 to retry.\n");
      while (BIOS::KEY::GetKey() != BIOS::KEY::F1);
      continue;
    }

    BIOS::OS::SetArgument((char*)"");
    BIOS::SYS::Execute(address);
    if (strlen(BIOS::OS::GetArgument()) > 0)
      strcpy(shell, filename);
  }
  return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}

extern "C" 
{
void NMIException(void)
{
   BIOS::DBG::Print("NMIException");    
   while(1);
}
/*
void HardFaultException(void)
{
   BIOS::DBG::Print("HardFaultException");    
   while(1);
}
*/
void MemManageException(void)
{
   BIOS::DBG::Print("MemManageException");    
   while(1);
}
void BusFaultException(void)
{
   BIOS::DBG::Print("BusFaultException");    
   while(1);
}

void UsageFaultException(void)
{
   BIOS::DBG::Print("UsageFaultException");    
   while(1);
}

#include "crash.h"

}