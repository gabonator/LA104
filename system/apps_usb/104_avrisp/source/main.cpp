#include <library.h>
#include "ringbuf.h"
#include "avrisp.h"
#include "webusb/webusb.h"
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"

//uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
//CBufferedWriter mWriter;

RingBufCPP<uint8_t, 512> serialReadPrority;
volatile bool serialReadPriortyReady = false;
volatile bool serialReadPriortyProcess = false;

extern RingBufCPP<uint8_t, 512> serialRead;
extern RingBufCPP<uint8_t, 512> serialWrite;

char temp[512];
int totalRx = 0;
int totalTx = 0;

const char anim[] = "|/-\\|/-\\";
int animphase = 0;

void debugprint(const char * format, ...)
{
/*
  char buf[128];
  va_list args;
  va_start( args, format );
  vsprintf( buf, format, args );
  CONSOLE::Color(RGB565(b0b0b0));
  CONSOLE::Print(buf);
  CONSOLE::Color(RGB565(ffffff));
*/
}


inline void enable_irq(void)
{
  __asm("cpsie i");
}

inline void disable_irq(void)
{
  __asm("cpsid i");
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
//  _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
//  BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
//  mWriter.Open((char*)"avrisp.txt");

  APP::Init("AVRISP");

  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  _usbd_dev = usb_setup();
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    serialReadPriortyReady = false;

    for (int i=0; i<len; i++)
      serialReadPrority.push(buf[i]);

    totalRx += len;
    serialReadPriortyReady = len!=64;
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });

  BIOS::KEY::EKey key;

  int pmode, error;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {

    if (serialReadPriortyReady)
    {
      disable_irq();
      while (serialReadPrority.size() > 0)
        serialRead.push(serialReadPrority.pull());
      serialReadPriortyReady = false;
      enable_irq();
    }


    if (serialRead.size() > 256)
    {
      CONSOLE::Color(RGB565(ff0000));
      CONSOLE::Print("<read %d>", serialRead.size());
    }
    if (serialRead.size() > 0)
    {
/*
      mWriter << "RX: ";
      for (int i=0; i<serialRead.size(); i++)
      {
        char temp[8];
        sprintf(temp, "0x%02x, ", *serialRead.peek(i));
        mWriter << temp;
      }
      mWriter << "\n";
*/

      uint8_t* c1 = serialRead.peek(0);
      uint8_t* c2 = serialRead.peek(1);
      uint8_t* c3 = serialRead.peek(2);
      uint8_t* c4 = serialRead.peek(3);
      uint8_t* c5 = serialRead.peek(4);
      uint8_t* c6 = serialRead.peek(5);
      uint8_t* c7 = serialRead.peek(6);
      CONSOLE::Print("[rx: %d]:", serialRead.size());
      if (c1) CONSOLE::Print(" %d", *c1);
//      if (c2) CONSOLE::Print(" %d", *c2);
//      if (c3) CONSOLE::Print(" %d", *c3);
//      if (c4) CONSOLE::Print(" %d", *c4);
//      if (c5) CONSOLE::Print(" %d", *c5);
//      if (c6) CONSOLE::Print(" %d", *c6);
//      if (c7) CONSOLE::Print(" %d", *c7);
      CONSOLE::Print(";");
    }
    avrisp_loop(pmode, error);

    if (serialWrite.size() > 256)
    {
      CONSOLE::Color(RGB565(ff0000));
      CONSOLE::Print("<write %d>", serialWrite.size());
    }

    if (serialWrite.size() > 0)
    {
/*
      mWriter << "TX: ";
      for (int i=0; i<serialWrite.size(); i++)
      {
        char temp[8];
        sprintf(temp, "0x%02x, ", *serialWrite.peek(i));
        mWriter << temp;
      }
      mWriter << "\n";
*/
      totalTx += serialWrite.size();
      BulkTransfer(serialWrite, serialWrite.size());
    }

    EVERY(1000)
    {
      char status[64];
      sprintf(status, "RX: %d, TX: %d, Mode: %d, Error: %d", totalRx, totalTx, pmode, error);
      APP::Status(status);

      BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "%c", anim[animphase++&7]);
    }
  }

  BIOS::DBG::Print("USB end\n");
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);
//  mWriter.Close();
//  BIOS::FAT::SetSharedBuffer(nullptr);
  // TODO: reset usb configuration
//  BIOS::USB::InitializeMass();
  return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

