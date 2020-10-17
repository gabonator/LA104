#include "Bios.h"

#include "imports.h"

static_assert((int)BIOS::SYS::EAttribute::DiskSectorCount == (int)EAttribute::DiskSectorCount);

//extern const unsigned char font[256*14];
//extern const unsigned char* font;
extern const void* ptrFont;
#ifdef LA104
extern uint32_t gGpioStatusCode;
extern uint32_t gFlashReadRange[2];
extern uint32_t gFlashWriteRange[2];
extern uint32_t gFlashAlertRange[2];
#endif
extern "C" uint32_t gKeyMask;
extern "C" volatile char lastChar;

namespace BIOS
{
  namespace SYS
  {
    uintptr_t GetAttribute(EAttribute attribute)
    {
      switch (attribute)
      {
        case EAttribute::BiosVersion: return 0x0103;

        case EAttribute::CharRom: return (uintptr_t)ptrFont;
        case EAttribute::LastChar: return (uintptr_t)&lastChar;
        case EAttribute::ScreenWidth: return BIOS::LCD::Width;
        case EAttribute::ScreenHeight: return BIOS::LCD::Height;

        case EAttribute::DeviceType: return (uintptr_t)::GetAttribute((::EAttribute)attribute);

        case EAttribute::BuildRevision: return (uintptr_t)__GITREVISION__;
        case EAttribute::BuildDate: return (uintptr_t)(__DATE__ " " __TIME__); 
        case EAttribute::BuildUser: return (uintptr_t)__USER__;
        case EAttribute::BuildSystem: return (uintptr_t)__OSTYPE__;

        case EAttribute::VersionDfu:
        case EAttribute::VersionHardware:
        case EAttribute::VersionSystem:
        case EAttribute::VersionFpga:

        case EAttribute::SerialNumber:
        case EAttribute::LicenseNumber:
        case EAttribute::LicenseValid:
        case EAttribute::DisplayType:
        case EAttribute::DiskType:
          return (uintptr_t)::GetAttribute((::EAttribute)attribute);

        case EAttribute::DiskSectorSize: return BIOS::FAT::SectorSize;
        case EAttribute::DiskSectorCount: return BIOS::FAT::SectorCount;
#ifdef LA104
        case EAttribute::GpioStatus: return (uintptr_t)&gGpioStatusCode;
        case EAttribute::FlashReadRange: return (uintptr_t)gFlashReadRange;
        case EAttribute::FlashWriteRange: return (uintptr_t)gFlashWriteRange;
        case EAttribute::FlashAlertRange: return (uintptr_t)gFlashAlertRange;
#endif
        case EAttribute::KeyMask: return (uintptr_t)&gKeyMask;

        default:
          _ASSERT(0);
          return (uintptr_t)nullptr;
      }
    }
  }
}