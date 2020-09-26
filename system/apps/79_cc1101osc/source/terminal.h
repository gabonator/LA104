#include "webusb/webusb.h"

namespace TERMINAL
{

void Write(uint8_t* p, int n)
{
  cdc_transmit(p, n);
}

void Print(const char* msg, ...)
{
  char buf[64];
  char* bbuf = buf; 

  va_list args;

  va_start( args, msg );
  vsprintf( bbuf, msg, args );

  _ASSERT(strlen(buf) < sizeof(buf)-8);
  Write((uint8_t*)buf, strlen(buf));
}

bool cdc_waitReady()
{
  const int32_t timeoutMilliseconds = 200;

  int32_t timeout = BIOS::SYS::GetTick() + timeoutMilliseconds;
  while (timeout - BIOS::SYS::GetTick() > 0)
  {
    if (cdc_transmitReady())
      return true;
  }
  return false;
}

bool BulkTransfer(int length, void(*prepare)(uint8_t* buffer, int bytes))
{
  uint8_t buffer[CDC_MAX_USB_PACKET_SIZE];
  bool zlp = length && (length % CDC_MAX_USB_PACKET_SIZE == 0);

  while (length > 0)
  {
    int toSend = min(CDC_MAX_USB_PACKET_SIZE, length);
    if (!cdc_waitReady())
      return false;

    prepare(buffer, toSend);
    cdc_transmit(buffer, toSend);

    length -= toSend;
  }

  if (zlp)
  {
    if (!cdc_waitReady())
      return false;

    if (!cdc_transmit(buffer, 0))
      return false;
  }

  return true;
}
/*
template <typename T> bool BulkTransfer(T& buffer, int length)
{
  bool zlp = length && (length % CDC_MAX_USB_PACKET_SIZE == 0);
  uint8_t temp[CDC_MAX_USB_PACKET_SIZE];

  while (length > 0)
  {
    int toSend = min(CDC_MAX_USB_PACKET_SIZE, length);

    for (int i=0; i<toSend; i++, length--)
      temp[i] = buffer.pull();

    if (!cdc_waitReady())
      return false;

    if (!cdc_transmit(temp, toSend))
      return false;
  }

  if (zlp)
  {
    if (!cdc_waitReady())
      return false;

    if (!cdc_transmit(temp, 0))
      return false;
  }

  return true;
}
*/
template <typename T> bool BulkTransfer(T& buffer, int length)
{
  length *= 2;
  bool zlp = length && (length % CDC_MAX_USB_PACKET_SIZE == 0);
  uint8_t temp[CDC_MAX_USB_PACKET_SIZE];

  while (length > 0)
  {
    int toSend = min(CDC_MAX_USB_PACKET_SIZE, length);

    for (int i=0; i<toSend; i+=2, length-=2)
    {
      uint16_t w = buffer.pull();
      temp[i] = w >> 8;
      temp[i+1] = w & 255;
    }

    if (!cdc_waitReady())
      return false;

    if (!cdc_transmit(temp, toSend))
      return false;
  }

  if (zlp)
  {
    if (!cdc_waitReady())
      return false;

    if (!cdc_transmit(temp, 0))
      return false;
  }

  return true;
}

}
