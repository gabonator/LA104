
#include <library.h>
#include "ringbuf.h"
#include "webusb/webusb.h"
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"

#define WRITER

#ifdef WRITER
#include "../../../os_host/source/framework/BufferedIo.h"
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedWriter mWriter;
#endif

RingBufCPP<uint8_t, 1024> serialReadPrority;
volatile bool serialReadPriortyReady = false;
volatile bool serialReadPriortyProcess = false;

/*extern*/ RingBufCPP<uint8_t, 1024> serialRead;
/*extern*/ RingBufCPP<uint8_t, 1024> serialWrite;

int totalRx = 0;
int totalTx = 0;

const char anim[] = "|/-\\|/-\\";
int animphase = 0;


usb_cdc_line_coding localLineCoding = {0};
int localControlState{-1};
int ctsdtr{0};
extern "C" struct usb_cdc_line_coding line_coding;

extern "C" void cdc_request(int req, int value)
{
  if (req == USB_CDC_REQ_SET_LINE_CODING)
    memcpy(&localLineCoding, &line_coding, sizeof(usb_cdc_line_coding));
  if (req == USB_CDC_REQ_SET_CONTROL_LINE_STATE)
  {
    BIOS::GPIO::DigitalWrite(BIOS::GPIO::P3, value & 1);
    BIOS::GPIO::DigitalWrite(BIOS::GPIO::P4, value & 2);
    localControlState = value;  
  }
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
  const int32_t timeoutMilliseconds = 10000;

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
      return false; // problem!

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

void sync()
{
  if (localLineCoding.dwDTERate != 0)
  {
    CONSOLE::Print("baud=%d, fmt=%d, parity=%d, data=%d\n", 
      localLineCoding.dwDTERate, localLineCoding.bCharFormat, 
      localLineCoding.bParityType, localLineCoding.bDataBits);

    BIOS::GPIO::UART::Setup(localLineCoding.dwDTERate, BIOS::GPIO::UART::length8);

    localLineCoding.dwDTERate = 0;
  }

  while (serialRead.size() > 0)
//    serialRead.pull();
    BIOS::GPIO::UART::Write(serialRead.pull());
  while (BIOS::GPIO::UART::Available())
    serialWrite.push(BIOS::GPIO::UART::Read());
}

usbd_device* _usbd_dev = nullptr;

__attribute__((__section__(".entry")))
int main(void) 
{

  APP::Init("WebUsbCdc");

#ifdef WRITER
  _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
  BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
  mWriter.Open((char*)"webcdc.txt");
#endif

    BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Output);
    BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Output);
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
    BIOS::GPIO::UART::Setup(115200, BIOS::GPIO::UART::length8);

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

//    if (serialRead.size() > 256)
//    {
//      CONSOLE::Color(RGB565(ff0000));
//      CONSOLE::Print("<read %d>", serialRead.size());
//    } 

#ifdef WRITER
    if (serialRead.size() > 0)
    {
      mWriter << "RX: ";
      for (int i=0; i<serialRead.size(); i++)
      {
        char temp[8];
        sprintf(temp, "0x%02x, ", *serialRead.peek(i));
        mWriter << temp;
      }
      mWriter << "\n";
    }
#endif

    sync();

//    if (serialWrite.size() > 256)
//    {
//      CONSOLE::Color(RGB565(ff0000));
//      CONSOLE::Print("<write %d>", serialWrite.size());
//    }

    if (serialWrite.size() > 0)
    {
      EVERY(100)
      {
#ifdef WRITER
        mWriter << "TX: ";
        for (int i=0; i<serialWrite.size(); i++)
        {
          char temp[8];
          sprintf(temp, "0x%02x, ", *serialWrite.peek(i));
          mWriter << temp;
        }
        mWriter << "\n";
#endif
        totalTx += serialWrite.size();
        BulkTransfer(serialWrite, serialWrite.size());
        //serialWrite.empty(); 
      }
    }

    EVERY(1000)
    {
      char status[64];
      sprintf(status, "RX: %d, TX: %d, Mode: %d, Error: %d", totalRx, totalTx, localControlState, error);
      APP::Status(status);

      BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "%c", anim[animphase++&7]);
    }
  }

#ifdef WRITER
  mWriter.Close();
  BIOS::FAT::SetSharedBuffer(nullptr);
#endif

  BIOS::DBG::Print("USB end\n");
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);
  return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

