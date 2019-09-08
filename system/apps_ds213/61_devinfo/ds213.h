#include "info.h"

class CDS213 : public CDeviceInfo
{
public:
  CDS213();
  virtual char* DfuVersion();
  virtual char* HardwareVersion();
  virtual char* SystemVersion();
  virtual char* FpgaVersion();
  virtual char* SerialNumber();
  virtual char* DisplayType();
  virtual char* DiskType();
};
 