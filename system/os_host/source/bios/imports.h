#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" 
{
#endif

  enum EKeyMask 
  {
    KeyUp = 1<<8,
    KeyDown = 1<<9,
    KeyRight = 1<<10,
    KeyLeft = 1<<11,
    KeyF1 = 1<<0,
    KeyF2 = 1<<1,
    KeyF3 = 1<<2,
    KeyF4 = 1<<3
  };

  enum EAttribute {BiosVersion, CharRom, LastChar, ScreenWidth, ScreenHeight, DeviceType,
      BuildRevision, BuildDate, BuildUser, BuildSystem, 
      VersionDfu, VersionHardware, VersionSystem, VersionFpga,
      SerialNumber, LicenseNumber, LicenseValid, DisplayType, DiskType,
      DiskSectorSize, DiskSectorCount,
      GpioStatus, KeyMask, 
      FlashReadRange, FlashWriteRange, FlashAlertRange,
      BatteryVoltage, Charging,
      GpioI2cSpeed};

  void Set_Posi(uint_fast16_t x, uint_fast16_t y);
  void Set_Pixel(uint_fast16_t Color);
  void Set_Area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void Set_Block(int x1, int y1, int x2, int y2);
  void ReadStart(void);
  void ReadFinish(void);
  uint32_t ReadPixel(void);

  bool ExtFlashSecWr(uint8_t* pBuffer, uint32_t WriteAddr);
  bool ExtFlashDataRd(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Lenght);
  void EnableUsb(bool enable);
  void HardwareInit();
  void xBeep(bool on);

  uint32_t GetKeys();
  char GetLastChar();

  uintptr_t GetAttribute(enum EAttribute attr);

#ifdef __cplusplus
}
#endif