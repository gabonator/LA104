#include <library.h>

typedef void (*THandler)(void);

RingBuffer<uint8_t, 1024> bufferReceive;
RingBuffer<uint8_t, 1024> bufferTransmit;

extern "C" 
{
  void _Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
        THandler arrHandlerIn[], THandler arrHandlerOut[], THandler callbacks[], THandler Leave)
  {
    BIOS::USB::Initialize(pDeviceInfo, pDevice, pDeviceProperty, pUserStandardRequests, arrHandlerIn, arrHandlerOut, callbacks, Leave);
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
    return bufferTransmit.size();
  }

  int onTransmitGet()
  {
    return bufferTransmit.pull();
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

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    USB::Enable();
    USB_Init();

    KEY::EKey key;
    bool prevConnected = false;

    int baudrate = 9600;
    GPIO::UART::EConfig defaultConfig = (GPIO::UART::EConfig)(
        GPIO::UART::length8 | GPIO::UART::stopBits1 | GPIO::UART::parityNone | GPIO::UART::flowNone);

    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
    BIOS::GPIO::UART::Setup(baudrate, defaultConfig);

    DBG::Print("Virtual serial port ready.\n");

    while ((key = KEY::GetKey()) != KEY::Escape)
    {
#ifdef ECHOLOOPBACK
        if (bufferReceive.size())
        {
          while (bufferReceive.size() && bufferTransmit.available())
          {
            int data = bufferReceive.pull();
            BIOS::DBG::Print("%c", data);
            bufferTransmit.push(data);
          }
        }
#else
        // interrupt lock?
        while (bufferReceive.size())
        {
          int data = bufferReceive.pull();
//          DBG::Print("%c", data);
          GPIO::UART::Write(data);
	}

        while (GPIO::UART::Available() && bufferTransmit.available())
        {
          int data = GPIO::UART::Read();
//          DBG::Print("<%c>", data);
          bufferTransmit.push(data);
        }

#endif
        if (bufferTransmit.size() > 0)
          USB_TxBufferHandler();

        bool connected = USB_IsConnected();
        if (connected != prevConnected)
        {
            prevConnected = connected;
            DBG::Print(connected ? "Connected.\n" : "Disconnected.\n");
        }

        int baudrate = 0;
        if (USB_HasNewLineCoding(&baudrate))
        {
            DBG::Print("Setting baudrate %d.\n", baudrate);
            GPIO::UART::Setup(baudrate, defaultConfig);
        }
    }

    USB::InitializeMass();
    return 0;
}
