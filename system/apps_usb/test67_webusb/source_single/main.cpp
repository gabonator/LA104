#include <library.h>
extern "C" 
{
#include "webusb.h"
}

__attribute__((__section__(".entry")))
int main(void) 
{
  static usbd_device* usbd_dev = nullptr;

  BIOS::OS::TInterruptHandler isrOld = BIOS::OS::GetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  BIOS::DBG::Print("USB begin\n");

  usbd_dev = usb_setup();

  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    BIOS::DBG::Print("{%s}", buf);
    cdc_transmit(buf, len); // echo
  });

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(usbd_dev);
  });

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    key = BIOS::KEY::GetKey();

    EVERY(1000)
    {
      BIOS::DBG::Print(".");
      static int counter = 0;
      char msg[32];
      sprintf(msg, "Ahoj-%d, ", counter++);
      cdc_transmit((uint8_t*)msg, strlen(msg));
    }
  }

  BIOS::DBG::Print("USB end\n");
  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);
  return 0;
}
