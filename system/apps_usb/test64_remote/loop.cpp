#include "terminal.h"

typedef void (*THandler)(void);

RingBuffer<uint8_t, 32> bufferReceive;
RingBuffer<uint8_t, 32> bufferTransmit;

CTerminal gTerminal;
extern "C" int USB_State();

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
    gTerminal.write(data);
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
extern "C" bool USB_IsConnected();
extern "C" void USB_TxBufferHandler();
extern "C" void IRQ_Disable(void);
extern "C" void IRQ_Enable(void);

using namespace BIOS;

bool prevConnected = false;

void Load()
{
  USB::Enable();
  USB_Init();
}

void Unload()
{
  USB::InitializeMass();
  gTerminal.end();
}

void Loop()
{
  while (bufferReceive.size())
  {
    int data = bufferReceive.pull();
    gTerminal.write(data);
  }

  while (gTerminal.available() && bufferTransmit.available())
  {
    int data = gTerminal.read();
    bufferTransmit.push(data);
  }

  if (bufferTransmit.size() > 0)
    USB_TxBufferHandler();
/*
  EVERY(1000)
  {
    DBG::Print("%d", USB_State());
  }
*/
  bool connected = USB_IsConnected();
  if (connected != prevConnected)
  {
      prevConnected = connected;
      DBG::Print(connected ? "$USB Remote control connected.\n" : "$USB Remote control disconnected.\n");
      if (connected)
        gTerminal.begin();
      else
        gTerminal.end();
  }
}
