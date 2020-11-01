#include <library.h>
#include "parser.h"
#include "terminal.h"
#include "spi.h"
#include "rpc.h"
#include "evaluator.h"

extern "C" 
{
#include "webusb.h"
}

usbd_device* _usbd_dev = nullptr;

CLineParser parser;
CEvaluator evaluator;

__attribute__((__section__(".entry")))
int main(void) 
{
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
    if (parser.isFull())
    {
      int result = evaluator.Evaluate(parser.get());
      parser.reset();
      TERMINAL::Print("{ret:%d}", result);
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

