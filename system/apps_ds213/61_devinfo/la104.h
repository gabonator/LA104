#include "info.h"

class CLA104 : public CDeviceInfo
{
public:
  virtual char* DfuVersion();
  virtual char* HardwareVersion();
  virtual char* SystemVersion();
  virtual char* FpgaVersion();
  virtual char* SerialNumber();
  virtual char* DisplayType();
  virtual char* DiskType();
};
 