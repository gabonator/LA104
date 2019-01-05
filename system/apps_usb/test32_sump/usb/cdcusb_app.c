#include "cdcusb_app.h"
#include "cdcusb_desc.h"
#include "cdcusb_conf.h"
#include "atomic.h"

bool lineCodingUpdated = false;
extern LINE_CODING linecoding;

bool USB_IsConnected()
{
  return cdcbDeviceState == CONFIGURED;
}

uint32_t swapEndian(uint32_t x)
{
    unsigned char *ptr = (unsigned char *)&x;
    return (ptr[3] << 24) | (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];
//    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

bool USB_HasNewLineCoding(int* baudrate)
{
    if (!lineCodingUpdated)
        return false;

    lineCodingUpdated = false;
    *baudrate = swapEndian(linecoding.bitrate);
    return true;
}

void Get_SerialNum()
{
}

void USB_Config()
{
}
