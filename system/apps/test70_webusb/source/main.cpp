#include <library.h>
#include "webusb/webusb.h"
#include "terminal.h"
#include "memory.h"
#include "analyser.h"
#include "rpc.h"
#include "evaluator.h"

char _rxBuffer[128] = {0};
const char anim[] = "|/-\\|/-\\";
int animphase = 0;

usbd_device* _usbd_dev = nullptr;

CEvaluator evaluator;

__attribute__((__section__(".entry")))
int main(void)
{
  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});

  CRect rcClient(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
  GUI::Background(rcClient, RGB565(404040), RGB565(101010));

  rcClient.bottom = 14;
  GUI::Background(rcClient, RGB565(4040b0), RGB565(404040));
  BIOS::LCD::Print(8, rcClient.top, RGB565(ffffff), RGBTRANS, "WebUsb interface");

  rcClient.bottom = BIOS::LCD::Height;
  rcClient.top = BIOS::LCD::Height-14;
  BIOS::LCD::Bar(rcClient, RGB565(404040));

  CRect rcWindow(20, 14+90, BIOS::LCD::Width-20, BIOS::LCD::Height-90);
  GUI::Window(rcWindow, RGB565(ffffff));
  BIOS::LCD::Print(rcWindow.left+8, rcWindow.top+2, RGB565(000000), RGBTRANS, "Info");
  BIOS::LCD::Print(rcWindow.left+8, rcWindow.top+26, RGB565(000000), RGBTRANS, "Connect the device to computer");

  _usbd_dev = usb_setup();
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    if (_rxBuffer[0])
      BIOS::DBG::Print("RX-Fail ");
    strcpy(_rxBuffer, (char*)buf);
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });

  BIOS::KEY::EKey key;
  BIOS::LCD::BufferBegin(CRect(0, 0, 100, 100));

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    if (_rxBuffer[0])
    {
      int result = evaluator.Evaluate(_rxBuffer);
      _rxBuffer[0] = 0;
      TERMINAL::Print("{ret:%d}", result);
    }
    EVERY(1000)
    {
      BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "%c", anim[animphase++&7]);
    }
  }

  BIOS::DBG::Print("USB end\n");
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);
  return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

