#include "Bios.h"

extern unsigned char font[256*14];
extern "C" volatile char lastChar;

namespace BIOS
{
  namespace PRIVATE
  {
    uintptr_t GetInternal(EInternal eInternal)
    {
      switch (eInternal)
      {
        case EInternal::CharRom: return (uintptr_t)font;
        case EInternal::LastChar: return (uintptr_t)&lastChar;
      }
      _ASSERT(0);
      return 0;
    }
  }
}