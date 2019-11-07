#include "cdcusb_app.h"
#include "cdcusb_desc.h"
#include "cdcusb_conf.h"
#include "atomic.h"

bool lineCodingUpdated = false;
extern LINE_CODING linecoding;

int USB_State()
{
  return cdcbDeviceState;
}

bool USB_IsConnected()
{
  return cdcbDeviceState == CONFIGURED;
}

void Get_SerialNum()
{
}

void USB_Config()
{
}
