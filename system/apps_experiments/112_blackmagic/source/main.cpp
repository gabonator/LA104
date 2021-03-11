#include <library.h>
extern "C" {
#include "blackmagic/gdb_main.h"
#include "blackmagic/gdb_packet.h"
#include "blackmagic/target.h"
}
#include "blackmagic/exception.h"
#include "platform.h"

#include "webusb/webusb.h"

usbd_device* _usbd_dev = nullptr;


char command[128] = {0};

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int main() {
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

    platform_init();

    BIOS::DBG::Print("[run]");
    while (BIOS::KEY::GetKey() == BIOS::KEY::None)  
    {
        volatile exception e;

        if (command[0])
        {
          BIOS::DBG::Print("rx:'%s' ", command);
          command[0] = 0;
        }

    	TRY_CATCH(e, EXCEPTION_ALL) {
    		gdb_main();
    	}
    	if (e.type) {
    		gdb_putpacketz("EFF");
    		target_list_free();
    		BIOS::DBG::Print("TARGET LOST.");
    	}
    }
    BIOS::DBG::Print("[done]");

    BIOS::DBG::Print("USB end\n");
    BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, isrOld);

    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
   BIOS::DBG::Print("Assertion failed");
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

