#include <library.h>

typedef void (*THandler)(void);

RingBuffer<uint8_t, 128> bufferReceive;
volatile uint8_t* pTransmitBuffer = nullptr;
volatile int nTransmitCount = 0;

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
  
  void USB_Receive(uint_fast8_t data)
  {
    BIOS::GPIO::UART::Write(data);
  }

  // UART -> USB
  int onTransmitAvailable()
  {
    return nTransmitCount;
  }

  int onTransmitGet()
  {
    int aux = *pTransmitBuffer++; 
    nTransmitCount--;
    return aux;
  }

  // USB -> UART
  bool onReceiveAvailable(int bytesToRead)
  {
    return (int)bufferReceive.available() >= bytesToRead;
  }

  void onReceivePut(int data)
  {
    bufferReceive.push(data);
  }
}

extern "C" void USB_Transmit(uint_fast8_t data);
extern "C" void USB_Init();
extern "C" bool USB_HasNewLineCoding(int*);
extern "C" bool USB_IsConnected();
extern "C" void USB_TxBufferHandler();

extern "C" void IRQ_Disable(void);
extern "C" void IRQ_Enable(void);


// sump
extern "C"
{
  void write_uart_s(const char* msg)
  { 
    BIOS::DBG::Print(msg);
  }
  void write_uart_u(int n)
  {
    BIOS::DBG::Print("%d", n);
  }

  int CDC_Transmit_FS(uint8_t* buffer, int len)
  {
    while (nTransmitCount);

    pTransmitBuffer = buffer;
    nTransmitCount = len;
    USB_TxBufferHandler();
    return 0;
  }
/*
  void dbg(uint8_t* p, int l)
  {
BIOS::DBG::Print("sent '%s' len %d.", p, l);
  }
  void dbg1(char c)
  {
BIOS::DBG::Print("<%c>", c);
  }
*/
}

extern "C" void sump_cmd(char c);
extern "C" void poll_sump();

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    USB::Disable();
    USB::Enable();
    USB_Init();

    KEY::EKey key;
    bool prevConnected = false;

    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Input);

    DBG::Print("SUMP logic analyser ready.\n");
    DBG::Print("Please set 50kHz sampling speed.\n");

    while ((key = KEY::GetKey()) != KEY::Escape)
    {
        // interrupt lock?
        while (bufferReceive.size())
        {
          sump_cmd(bufferReceive.pull());
        }

        poll_sump();

        bool connected = USB_IsConnected();
        if (connected != prevConnected)
        {
            prevConnected = connected;
            DBG::Print(connected ? "Connected.\n" : "Disconnected.\n");
        }
    }

    USB::Disable();
    USB::InitializeMass();
    return 0;
}
