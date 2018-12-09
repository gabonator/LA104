#include <library.h>
#include <stm32f10x.h>

typedef void (*THandler)(void);

extern "C" 
{
  void _Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
        THandler arrHandlerIn[], THandler arrHandlerOut[], THandler istrHandler)
  {
    BIOS::USB::Initialize(pDeviceInfo, pDevice, pDeviceProperty, pUserStandardRequests, arrHandlerIn, arrHandlerOut, istrHandler);
  }

  void _CTR_LP()
  {
    BIOS::USB::CTR_LP();
  }
}

extern "C" void InitUsb();

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    USB::Enable();
    InitUsb();

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::Escape)
    {
    }

    BIOS::USB::InitializeMass();
    return 0;
}
