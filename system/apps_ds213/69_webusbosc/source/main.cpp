#include <Library.h>
#include "parser.h"
#include "terminal.h"
#include "adc.h"
#include "spi.h"
#include "rpc.h"
#include "evaluator.h"

extern "C" 
{
#include "webusb.h"
}

char buffer[256];
extern "C" void dbg(const char*id)
{
  strcat(buffer, id);
/*
return;
  char tmp[20];
  sprintf(tmp, "%d:%s%02x ", BIOS::SYS::GetTick(), id, n);
  strcat(buffer, tmp);
  if (strlen(buffer)>100)
  {
   BIOS::DBG::Print("~");
   BIOS::DBG::Print(buffer);
   buffer[0] = 0;
  }*/
}
extern "C" void __yield()
{
  EVERY(1000)
  {
    BIOS::DBG::Print("#");
  }
}

usbd_device* _usbd_dev = nullptr;

CLineParser parser;
CEvaluator evaluator;

__attribute__((__section__(".entry")))
int main(void) 
{
  buffer[0] = 0;

  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  BIOS::DBG::Print("USB begin\n");

  _usbd_dev = usb_setup();
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    parser.push((char*)buf);
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(_usbd_dev);
  });

  BIOS::KEY::EKey key;
  BIOS::LCD::BufferBegin(CRect(0, 0, 100, 100));

//  parser.push((char*)"_ZN4BIOS3DBG5PrintEPKcz(\"Ready...\n\");");
//  parser.push((char*)"RPC::Print(\"Ready...\n\");");

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    if (buffer[0])
    {
      BIOS::DBG::Print(buffer);
      BIOS::DBG::Print("\n");
      buffer[0] = 0;
    }
//    BIOS::SYS::DelayMs(100);

    if (parser.isFull())
    {
      int result = evaluator.Evaluate(parser.get());
      parser.reset();
      TERMINAL::Print("{ret:%d}", result);
    }
    EVERY(1000)
    {
      BIOS::DBG::Print(".");
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

