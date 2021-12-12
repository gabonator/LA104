#include <library.h>
#include "webusb.h"

__attribute__((__section__(".entry")))
int main(void) 
{
  static usbd_device* usbd_dev = nullptr;

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  BIOS::DBG::Print("USB begin\n");

  /* Setup USB */
  
  bool sendMessage = false;
  static char message[65] = {0};
  cdc_set_receive_callback([](uint8_t* buf, int len)
  {
    memcpy(message, buf, len);
    message[len] = 0;
  });

  usbd_dev = usb_setup();

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
    usbd_poll(usbd_dev);
  });

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    if (message[0])
    {
      char temp[65];
      memcpy(temp, message, sizeof(temp));
      message[0] = 0;
      BIOS::DBG::Print(temp);
      sendMessage = temp[0] == '1';
    }

    EVERY(1000)
    {
      BIOS::DBG::Print(".");
      if (sendMessage)
        cdc_transmit((const uint8_t*)"Hello", 5);
    }                    
  }

  BIOS::DBG::Print("USB end\n");
  return 0;
}
