#include <library.h>
#include <stm32f10x.h>

// TODO: cleanup code

typedef void (*THandler)(void);

extern "C" 
{
  void _Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
        THandler arrHandlerIn[], THandler arrHandlerOut[], THandler callbacks[], THandler pLeaveLowPowerMode)
  {
    BIOS::USB::Initialize(pDeviceInfo, pDevice, pDeviceProperty, pUserStandardRequests, arrHandlerIn, arrHandlerOut, callbacks, pLeaveLowPowerMode);
  }

  void InitializeFinish(int imr_msk)
  {
    BIOS::USB::InitializeFinish(imr_msk);
  }
}

RingBuffer<uint8_t, 128> bufferReceive;
RingBuffer<uint8_t, 128> bufferTransmit;

extern "C" {

char dbgPushBuf[512];

void dbgPrint(const char* msg)
{
  strcat(dbgPushBuf, msg);
}

void dbgHex(int n)
{
  char buf[16];
  sprintf(buf, "%02x ", n);
  strcat(dbgPushBuf, buf);
}

extern int32_t MIOS32_IRQ_Disable(void);
extern int32_t MIOS32_IRQ_Enable(void);
extern void MIOS32_USB_MIDI_TxBufferHandler();

int onTransmitAvailable()
{
  return bufferTransmit.size();
}

uint_fast8_t onTransmitGet()
{
  return bufferTransmit.pull();
}

uint_fast8_t onTransmitPeek()
{
  return bufferTransmit.peek();
}

void onReceive(uint8_t value)
{
  bufferReceive.push(value);
}

bool onReceiveAvailable(int bytes)
{
  return true;
}
}

extern const u8 mios32_midi_pcktype_num_bytes[16];


void processMidiData(uint_fast8_t data, void(*send)(uint_fast8_t write))
{
  static int lastCommand = 0;
  static int command[4];
  static int pos = 0;

  if (data & 0x80)
  {
    // we are skipping sysex values anyway...
    if (data == 0xf8 || data == 0xfe) 
      return;

    lastCommand = data;
    pos = 0;
    command[pos++] = data;
    return;
  }
  if (pos == 0 && lastCommand != 0)
  {
    pos = 0;
    command[pos++] = lastCommand;
  }
  if (pos > 0)
  {
    int required = mios32_midi_pcktype_num_bytes[lastCommand >> 4];
    command[pos++] = data;
    if (pos >= required)
    {
//BIOS::DBG::Print("{");
      for (int i=0; i<required; i++)
{
//BIOS::DBG::Print("%2x ", command[i]);
       send(command[i]);
}
//BIOS::DBG::Print("}");
      pos = 0;
    }
  }
}

extern "C" void InitUsb();

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
    BIOS::GPIO::UART::Setup(31250, BIOS::GPIO::UART::length8);

    USB::Enable();
    InitUsb();

    KEY::EKey key;
    long lastSend = 0;

    while ((key = KEY::GetKey()) != KEY::Escape)
    {
      long current = SYS::GetTick(); 

      if (current-lastSend > 10)
      {
        lastSend = current;
        if (bufferTransmit.size() > 0)
          MIOS32_USB_MIDI_TxBufferHandler();
      }

      if (BIOS::GPIO::UART::Available())
      {
       // disabling disables also the serial receive isr!!! we are losing data!
//        MIOS32_IRQ_Disable();
        while (BIOS::GPIO::UART::Available())
        {
          uint_fast8_t value = BIOS::GPIO::UART::Read();
          processMidiData(value, [](uint_fast8_t value) {
            bufferTransmit.push(value);
          });
        }
//        MIOS32_IRQ_Enable();
      }

      if (bufferReceive.size())   
      {
        MIOS32_IRQ_Disable();
        while (bufferReceive.size())
        {
          uint_fast8_t value = bufferReceive.pull();
          BIOS::GPIO::UART::Write(value);
        }
        MIOS32_IRQ_Enable();
      }

      int l = strlen(dbgPushBuf);
      if (l > 0)
      {
        for (int i=0; i<l; i+=32)
        {
          char c = dbgPushBuf[i+32];
          dbgPushBuf[i+32] = 0;
          BIOS::DBG::Print(dbgPushBuf+i); // cannot print more than 64 chas
          dbgPushBuf[i+32] = c;
        }
        strcpy(dbgPushBuf, "");
      }
    }

    BIOS::USB::InitializeMass();
    return 0;
}
