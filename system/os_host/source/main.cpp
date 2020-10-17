//#include "../library/CMSIS/Device/STM32F10x/Include/STM32F10x.h"
#include "source/bios/Bios.h"
#include "source/main/Execute.h"

void InvalidateFat();

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
  // TODO: move to ds203 startup code
  BIOS::SYS::Beep(0);

  BIOS::SYS::Init();
  BIOS::FAT::Init();
#ifndef DISABLE_USB
  BIOS::USB::Enable();
//  BIOS::USB::InitializeSerial();
  BIOS::USB::InitializeMass();
#endif

  BIOS::OS::SetArgument((char*)"");

  char shell[64] = "shell.elf";

#ifdef LA104
  while (BIOS::KEY::GetKey() == BIOS::KEY::F3)
#endif
#ifdef DS213
  while (BIOS::KEY::GetKey() == BIOS::KEY::F4)
#endif
#ifdef DS203
  while (BIOS::KEY::GetKey() == BIOS::KEY::F4)
#endif
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
      // TODO: Will fit in stack?

      CTokenizer tok(BIOS::OS::GetArgument());
      tok.GetToken(filename, 63);

      uint8_t gFlashSharedBuffer[BIOS::MEMORY::SharedBufferSize]; 
      BIOS::MEMORY::SetSharedBuffer(gFlashSharedBuffer);

      uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize]; 
      BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

      address = ElfExecute(filename);

      BIOS::MEMORY::SetSharedBuffer(nullptr);
      BIOS::FAT::SetSharedBuffer(nullptr);
    }

    if (!address)
    {
      BIOS::DBG::Print("Press F1 to retry.\n");
      while (BIOS::KEY::GetKey() != BIOS::KEY::F1);
      continue;
    }
                           
    // keep argument - for file viewer
    //BIOS::OS::SetArgument((char*)""); 
    BIOS::OS::HasArgument();
    BIOS::SYS::Execute(address);

//    if (strlen(BIOS::OS::GetArgument()) > 0)
    if (BIOS::OS::HasArgument())
      strcpy(shell, filename);
    else
      BIOS::OS::SetArgument((char*)"");
  }

  return 0;
}

extern "C" 
{

void Halt()
{
  BIOS::DBG::Print("Application halted");
  while(1);
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
  // prevent sprintf buffer overflow
  BIOS::DBG::Print("\n\nAssertion failed in ");
  BIOS::DBG::Print(file);
  BIOS::DBG::Print(" [%d]: ", line);
  BIOS::DBG::Print(cond);
  BIOS::DBG::Print("\n");
  Halt();
}

void NMIException(void)
{
  BIOS::DBG::Print("NMIException");    
  Halt();
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
  BIOS::DBG::Print("MemManageException\n");    
  Halt();
}
void BusFaultException(void)
{
  BIOS::DBG::Print("BusFaultException\n");    
  Halt();
}

void UsageFaultException(void)
{
  BIOS::DBG::Print("UsageFaultException\n");    
  Halt();
}

#include "crash.h"

}