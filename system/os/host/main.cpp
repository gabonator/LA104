#include "STM32F10x.h"
#include "source/bios/Bios.h"
#include "source/Manager.h"

extern "C" void Hardware_Init();

int main()
{
  Hardware_Init();
  BIOS::FAT::Init();

  while (1)
  {
    uint32_t dwExecute = 0;

    {
      CWndUserManager manager;
      manager.Create(nullptr, CWnd::WsVisible);
      manager.SetFocus();

      manager.OnMessage(nullptr, ToWord('L', 'E'), 0);
      manager.WindowMessage(CWnd::WmPaint, 0);

      while (dwExecute == 0)
      {
        BIOS::KEY::EKey key = BIOS::KEY::GetKey();
        if (key != BIOS::KEY::None)
          manager.WindowMessage(CWnd::WmKey, key);
        manager.WindowMessage(CWnd::WmTick, 0);

        dwExecute = manager.GetExecutionEntry();
      }
      // TODO: remember executed file & path to highlight it in next pass
    }
  
    if (dwExecute)
    {
      int nRet = BIOS::SYS::Execute( dwExecute );
      BIOS::DBG::Print("Return code=%d.\n", nRet);
      //manager.Invalidate();
    }
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