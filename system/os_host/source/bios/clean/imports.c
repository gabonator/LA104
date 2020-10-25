#include "BIOS.h"

void Set_Pixel(uint_fast16_t Color)
{
}

void ReadStart(void)
{
}

void ReadFinish(void)
{
}

uint32_t ReadPixel()
{
    return 0;
}

void Set_Block(int x1, int y1, int x2, int y2)
{
}

void xBeep(bool b)
{
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
}

void EnableUsb(bool enable)
{
}

void HardwareInit()
{
}

uint32_t GetKeys()
{
  return 0;
}

bool ExtFlashSecWr(uint8_t* pBuffer, uint32_t WriteAddr)
{
  return 1;
}

bool ExtFlashDataRd(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Length)
{
  return 1;
}

uintptr_t GetAttribute(enum EAttribute attribute)
{
  switch (attribute)
  {
    case DeviceType: return (uintptr_t)"CLEAN";
    case VersionDfu: return (uintptr_t)nullptr;
    case VersionHardware: return (uintptr_t)(char*)nullptr;
    case VersionSystem: return (uintptr_t)(char*)nullptr;
    case VersionFpga: return (uintptr_t)(char*)nullptr;
    case SerialNumber: return (uintptr_t)(uint32_t)0;
    case LicenseNumber: return (uintptr_t)0;
    case LicenseValid: return (uintptr_t)0;
    case DisplayType: return (uintptr_t)0;
    case DiskType: return (uintptr_t)0;
    default: return 0;
  }
}
