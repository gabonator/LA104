#pragma once
#include <library.h>

class CDeviceInfo
{
public:
  virtual char* DfuVersion() = 0;
  virtual char* HardwareVersion() = 0;
  virtual char* SystemVersion() = 0;
  virtual char* FpgaVersion() = 0;
  virtual char* SerialNumber() = 0;
  virtual char* DisplayType() = 0;
  virtual char* DiskType() = 0;
};

class CFirmwareInfo
{
public:
  virtual char* BuildRevision();
  virtual char* BuildDate();
};

