/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <library.h>

extern "C" 
{
#include "target.h"
#include "usb_conf.h"
#include "webusb.h"
#include "winusb.h"
#include "cdc.h"
#include <libopencm3/stm32/desig.h>
#include "usb21_standard.h"
}

extern "C"
void dbg(char*p)
{
  BIOS::DBG::Print("{%s}", p);
}

const usbd_driver* target_usb_init(void) 
{
    return &st_usbfs_v1_usb_driver;
}

__attribute__((__section__(".entry")))
int main(void) 
{
  static usbd_device* usbd_dev = nullptr;

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {});
  BIOS::DBG::Print("USB begin\n");

  /* Setup USB */
  {
    char serial[USB_SERIAL_NUM_LENGTH+1];
    serial[0] = '\0';
    desig_get_unique_id_as_string(serial, USB_SERIAL_NUM_LENGTH);
    usb_set_serial_number(serial);
  }

  usbd_dev = usb_setup();

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
    }
  }

  BIOS::DBG::Print("USB end\n");
  return 0;
}
