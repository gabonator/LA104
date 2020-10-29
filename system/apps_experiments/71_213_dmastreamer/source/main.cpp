
#include <library.h>
#include "streamer/streamer.h"
#include "ringbuf.h"
#include "webusb/webusb.h"

using namespace BIOS;

RingBufCPP<uint8_t, 2048+256> buffer;

volatile int overrun = 0;
volatile int dropped = 0;
volatile int ready = 0;

void process_samples(const uint32_t *data)
{
  const uint32_t *end = data + ADC_FIFO_HALFSIZE;
  while (data < end)
  {   
    int sample = *data++ & 0xff;
    if (buffer.isFull())
      overrun++;
    else
      buffer.push(sample);
  }
}

BIOS::OS::TInterruptHandler isrOld = nullptr;
usbd_device* _usbd_dev = nullptr;

void usbInit()
{
  isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  _usbd_dev = usb_setup();
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    ready = 1;
  });
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });
}

void usbDeinit()
{
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);
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

bool cdc_transmitBulk(uint8_t* buffer, int length)
{
  while (length > 0)
  {
    int toSend = min(CDC_MAX_USB_PACKET_SIZE, length);
    if (!cdc_waitReady())
      return false;

    cdc_transmit(buffer, toSend);
    buffer += toSend;
    length -= toSend;
  }
  return true;
}

template <typename T> bool cdc_transmitBulk(T& buffer, int length)
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

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    constexpr int Width = 220;
    constexpr int Height = 120;
    CRect rcMain(BIOS::LCD::Width/2 - Width/2, BIOS::LCD::Height/2 - Height/2, BIOS::LCD::Width/2 + Width/2, BIOS::LCD::Height/2 + Height/2);
    BIOS::LCD::Bar(rcMain, RGB565(0080d0));

    BIOS::DBG::Print("[usb init]\n");
    usbInit();
    BIOS::DBG::Print("[adc init]\n");
    adcConfigure();
    BIOS::DBG::Print("[streamer init]\n");
    streamerInit();
    streamerStart();

    BIOS::DBG::Print("[main loop]\n");

    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      EVERY(1000)
      {
        if (overrun)
        {
          int temp = overrun;
          overrun = 0;
          BIOS::DBG::Print("[%d samples lost!]", temp);
          buffer.empty();
        }
        if (dropped)
        {
          BIOS::DBG::Print("[%d dropped frames]", dropped);
          dropped = 0;
        }
      }

      if (buffer.size() >= 2048)
      {
        if (ready)
        {
           if (!cdc_transmitBulk(buffer, 2048))
             dropped++;
        }
      }
    }
    streamerStop();
    streamerDeinit();
    usbDeinit();
    return 0;
}
