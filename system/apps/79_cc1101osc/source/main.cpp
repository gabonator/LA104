#include <library.h>
#include "terminal.h"
#include "memory.h"
#include "cc1101.h"
#include "app.h"
#include "rpc.h"
#include "evaluator.h"
#include "webusb/webusb.h"

char command[128] = {0};
char buffer[128];
const char anim[] = "|/-\\|/-\\";
int animphase = 0;

extern "C" void dbg(const char*id)
{
  if (strlen(buffer) < 40)
  {
    strcat(buffer, id);
    if (strlen(buffer) >= 40)
      strcat(buffer, "...");
  }
}

extern "C" void __yield()
{
  EVERY(1000)
  {
    BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(00ff00), RGB565(404040), "%c", anim[animphase++&7]);
  }
}

usbd_device* _usbd_dev = nullptr;

CEvaluator evaluator;

__attribute__((__section__(".entry")))
int main(void) 
{
  buffer[0] = 0;

  CRect rcClient(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
  GUI::Background(rcClient, RGB565(404040), RGB565(101010));

  rcClient.bottom = 14;
  GUI::Background(rcClient, RGB565(4040b0), RGB565(404040));
  BIOS::LCD::Print(8, rcClient.top, RGB565(ffffff), RGBTRANS, "CC1101 WebUsb interface");

  rcClient.bottom = BIOS::LCD::Height;
  rcClient.top = BIOS::LCD::Height-14;
  BIOS::LCD::Bar(rcClient, RGB565(404040));

  CRect rcWindow(20, 14+80, BIOS::LCD::Width-20, BIOS::LCD::Height-80);
  GUI::Window(rcWindow, RGB565(ffffff));
  BIOS::LCD::Print(rcWindow.left+8, rcWindow.top+2, RGB565(000000), RGBTRANS, "Info");
  BIOS::LCD::Print(rcWindow.left+8, rcWindow.top+30, RGB565(000000), RGBTRANS, "Connect the device to computer");

  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  _usbd_dev = usb_setup();
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    memcpy(command, buf, len);
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });

  BIOS::KEY::EKey key;
  APP::Init();

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    if (buffer[0])
    {
      BIOS::LCD::Print(8, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), buffer);
      buffer[0] = 0;
    }

    if (command[0])
    {
      int result = evaluator.Evaluate(command);
      command[0] = 0;
      TERMINAL::Print("{ret:%d}", result);
    }
    EVERY(1000)
    {
      BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "%c", anim[animphase++&7]);
    }
    APP::Do();
  }
  APP::End();

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

