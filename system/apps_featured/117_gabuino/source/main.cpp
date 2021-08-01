#include <library.h>
#include "terminal.h"
#include "memory.h"
#include "rpc.h"
#include "evaluator.h"
#include "webusb/webusb.h"

uint8_t appblob[4096*2] __attribute__((section(".usersection")));

char command[128] = {0};
const char anim[] = "|/-\\|/-\\";
int animphase = 0;

extern "C" void __yield()
{
  EVERY(1000)
  {
    BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(00ff00), RGB565(404040), "%c", anim[animphase++&7]);
  }
}

usbd_device* _usbd_dev = nullptr;

CEvaluator evaluator;

void _PrepareRun()
{
  command[0] = 0; // otherwise it would infinitelly call Execute
}

void EventLoop()
{

    if (command[0])
    {
      int result = evaluator.Evaluate(command);
      command[0] = 0;
      TERMINAL::Print("{ret:%d}", result);
    }

    EVERY(1000)
    {
      if (MEMORY::running)
        BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(ff5050), RGB565(404040), "%c", anim[animphase++&7]);
      else
        BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(404040), "%c", anim[animphase++&7]);
    }
}

void _yield()
{
	EventLoop();
}

__attribute__((__section__(".entry")))
int main(void) 
{

  CRect rcClient(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
  GUI::Background(rcClient, RGB565(404040), RGB565(101010));

  rcClient.bottom = 14;
  GUI::Background(rcClient, RGB565(4040b0), RGB565(404040));
  BIOS::LCD::Print(8, rcClient.top, RGB565(ffffff), RGBTRANS, "Gabuino");

  rcClient.bottom = BIOS::LCD::Height;
  rcClient.top = BIOS::LCD::Height-14;
  BIOS::LCD::Bar(rcClient, RGB565(404040));
  BIOS::LCD::Print(8, rcClient.top, RGB565(ffffff), RGBTRANS, "Waiting for USB connection");

  BIOS::OS::SetInterruptVector(BIOS::OS::IHardFaultException, []() {
    for (int i=0; i<100; i++)
      BIOS::DBG::Print("Hard fault! ");
    
    // https://www.programmersought.com/article/90375648886/
    // NVIC_SystemReset();
    __asm("LDR R0, =0xE000ED0C");
    __asm("LDR R1, =0x05FA0004");
    __asm("STR R1, [R0]");
    while (1);
    // TODO: reset hard fault vector back!
  });

  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  _usbd_dev = usb_setup();

  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
/*
//    MEMORY::isr = true;
    if (MEMORY::running)
    {
      // save stack - only available commands DBG::Stop, DBG::Trace, DBG::Pause
      MEMORY::SaveStack(); // not possible, to recover regs, but possible to recover stack
    }
 */
//    if (/*MEMORY::running &&*/ memcmp(buf, "DBG::Stop()", 11) == 0)
    if (MEMORY::writeCount > 0 && len > 0)
    {
      MEMORY::HandleWrite(buf, len);
      if (MEMORY::writeCount == 0)
        strcpy(command, "MEM::Done();");
      return;
    }

    if (memcmp(buf, "DBG::Frame()", 12) == 0)
    {
      MEMORY::_Frame();
    }

    if (MEMORY::running)
    {
        //save stack?
      if (memcmp(buf, "DBG::Stop()", 11) == 0)
      {
        evaluator.Evaluate((char*)buf);
        return;
      }
    }

    memcpy(command, buf, len);
//    MEMORY::isr = false;
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });

  BIOS::KEY::EKey key;

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    EventLoop();
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

