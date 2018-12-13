#include <library.h>
#include <stm32f10x.h>

// Based on incredible MIOS project by Thorsten Klose: https://github.com/polluxsynth/mios32.git
// TODO: cleanup code

typedef void (*THandler)(void);

extern "C" 
{
  void _Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
        THandler arrHandlerIn[], THandler arrHandlerOut[], THandler callbacks[])
  {
    BIOS::USB::Initialize(pDeviceInfo, pDevice, pDeviceProperty, pUserStandardRequests, arrHandlerIn, arrHandlerOut, callbacks);
  }

  void InitializeFinish(int imr_msk)
  {
    BIOS::USB::InitializeFinish(imr_msk);
  }
}

extern "C" {
#include "usb/common.h"

char dbgPushBuf[256];

void dbgPrint(const char* msg)
{
  strcat(dbgPushBuf, msg);
}

void dbgHex(int n)
{
  char buf[16];
  sprintf(buf, "%02x ", n);
  strcat(dbgPushBuf, buf);
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
      if (strlen(dbgPushBuf) > 0)
      {
        BIOS::DBG::Print(dbgPushBuf);
        strcpy(dbgPushBuf, "");
      }
    }

    BIOS::USB::InitializeMass();
    return 0;
}
