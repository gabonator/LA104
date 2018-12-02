#include "Bios.h"

extern "C" void USB_Connect(uint8_t Status);

namespace BIOS
{
  namespace USB
  {
    void Enable()
    {
      USB_Connect(1);
    }

    void Disable()
    {
      USB_Connect(0);
    }
  }
}