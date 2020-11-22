#include <library.h>
#include "ringbuf.h"
#include "avrisp.h"
#include "webusb/webusb.h"

extern RingBufCPP<uint8_t, 2048> serialRead;
extern RingBufCPP<uint8_t, 2048> serialWrite;

char temp[512];
int totalRx = 0;
int totalTx = 0;

const char anim[] = "|/-\\|/-\\";
int animphase = 0;

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

extern "C" void __yield()
{
  EVERY(1000)
  {
    BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(00ff00), RGB565(404040), "%c", anim[animphase++&7]);
  }
}

usbd_device* _usbd_dev = nullptr;

__attribute__((__section__(".entry")))
int main(void) 
{
  CRect rcClient(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
  GUI::Background(rcClient, RGB565(404040), RGB565(101010));

  rcClient.bottom = 14;
  GUI::Background(rcClient, RGB565(4040b0), RGB565(404040));
  BIOS::LCD::Print(8, rcClient.top, RGB565(ffffff), RGBTRANS, "AVRISP");

  rcClient.bottom = BIOS::LCD::Height;
  rcClient.top = BIOS::LCD::Height-14;
  BIOS::LCD::Bar(rcClient, RGB565(404040));

  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  _usbd_dev = usb_setup();
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    for (int i=0; i<len; i++)
      serialRead.push(buf[i]);
    totalRx += len;
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });

  BIOS::KEY::EKey key;

  int pmode, error;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    if (serialRead.size() > 256)
      BIOS::DBG::Print("<read %d>", serialRead.size());
    avrisp_loop(pmode, error);

    if (serialWrite.size() > 256)
      BIOS::DBG::Print("<write %d>", serialWrite.size());

    if (serialWrite.size() > 0)
    {
      totalTx += serialWrite.size();
      BulkTransfer(serialWrite, serialWrite.size());
/*
      int i = 0;
      while (serialWrite.size() > 0)
        temp[i++] = serialWrite.pull();
      temp[i] = 0;
      cdc_transmit((uint8_t*)temp, i);
      totalTx += i;
*/
    }

    EVERY(1000)
    {
      BIOS::LCD::Printf(0, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "RX: %d, TX: %d, Mode: %d, Error: %d", totalRx, totalTx, pmode, error);
      BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "%c", anim[animphase++&7]);
    }
  }

  BIOS::DBG::Print("USB end\n");
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);
  // TODO: reset usb configuration
//  BIOS::USB::InitializeMass();
  return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

