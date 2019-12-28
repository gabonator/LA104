#include "Bios.h"

//extern const unsigned char font[256*14];
//extern const unsigned char* font;
extern const void* ptrFont;

extern "C" volatile char lastChar;

namespace BIOS
{
  namespace SYS
  {
    char* GetDeviceType()
    {
    #ifdef LA104
      return (char*)"LA104";
    #elif defined(DS213)
      return (char*)"DS213";
    #elif defined(DS203)
      return (char*)"DS203";
    #endif
      return nullptr;
    }

    uintptr_t GetAttribute(EAttribute EAttribute)
    {
      switch (EAttribute)
      {
        case EAttribute::BiosVersion: return 0x0101;

        case EAttribute::CharRom: return (uintptr_t)ptrFont;
        case EAttribute::LastChar: return (uintptr_t)&lastChar;
        case EAttribute::ScreenWidth: return BIOS::LCD::Width;
        case EAttribute::ScreenHeight: return BIOS::LCD::Height;

        case EAttribute::DeviceType: return (uintptr_t)GetDeviceType();


        case EAttribute::BuildRevision: return (uintptr_t)__GITREVISION__;
        case EAttribute::BuildDate: return (uintptr_t)(__DATE__ " " __TIME__); 
        case EAttribute::BuildUser: return (uintptr_t)__USER__;
        case EAttribute::BuildSystem: return (uintptr_t)__OSTYPE__;

        case EAttribute::VersionDfu: return (uintptr_t)nullptr;
        case EAttribute::VersionHardware: return (uintptr_t)nullptr;
        case EAttribute::VersionSystem: return (uintptr_t)nullptr;
        case EAttribute::VersionFpga: return (uintptr_t)nullptr;

        case EAttribute::SerialNumber: return (uintptr_t)nullptr;
        case EAttribute::DisplayType: return (uintptr_t)nullptr;
        case EAttribute::DiskType: return (uintptr_t)nullptr;
      }
      _ASSERT(0);
      return 0;
    }
  }
}