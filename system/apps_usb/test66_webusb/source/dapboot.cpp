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

#include <Library.h>

extern "C" 
{
//#include "uf2.h"
#include "dapboot.h"
#include "target.h"
#include "usb_conf.h"
#include "webusb.h"
#include "winusb.h"
#include "config.h"
#include <libopencm3/usb/msc.h>
#include <libopencm3/stm32/f1/memorymap.h>
}

#include <stdint.h>
#define USB_ISTR_CTR		0x8000 /* Correct Transfer */
#define USB_ISTR_PMAOVR		0x4000 /* Packet Memory Area Over / Underrun */
#define USB_ISTR_ERR		0x2000 /* Error */
#define USB_ISTR_WKUP		0x1000 /* Wake up */
#define USB_ISTR_SUSP		0x0800 /* Suspend mode request */
#define USB_ISTR_RESET		0x0400 /* USB RESET request */
#define USB_ISTR_SOF		0x0200 /* Start Of Frame */
#define USB_ISTR_ESOF		0x0100 /* Expected Start Of Frame */
#define USB_ISTR_DIR		0x0010 /* Direction of transaction */
#define USB_ISTR_EP_ID		0x000F /* Endpoint Identifier */
#define RegBase  (0x40005C00L)  /* USB_IP Peripheral Registers base address */
#define ISTR    ((volatile unsigned *)(RegBase + 0x44))
#define _GetISTR()   ((uint16_t) *ISTR)
#define _SetISTR(wRegValue)  (*ISTR   = (uint16_t)wRegValue)
#define CNTR    ((volatile unsigned *)(RegBase + 0x40))
#define _SetCNTR(wRegValue)  (*CNTR   = (uint16_t)wRegValue)


uint32_t msTimer;
extern int msc_started;

int write_block(uint32_t lba, const uint8_t *copy_from)
{
//  BIOS::DBG::Print("write(%d)\n", lba);
  return 0;
}

int read_block(uint32_t block_no, uint8_t *data)
{
//  BIOS::DBG::Print("read(%d)\n", block_no);
  return 0;
}
int isr = 0;


usbd_device* usbd_dev;

void resetUsb()
{
//  _SetCNTR(0);
#define CNTR_FRES   (0x0001) /* Force USB RESet */

	_SetCNTR(CNTR_FRES);				/* disable all interrupts and force USB reset */
	_SetISTR(0);						/* clear interrupt status register */

  BIOS::OS::SetInterruptVector(BIOS::OS::IUSB_LP_CAN_RX0_IRQ, []() {
            usbd_poll(usbd_dev);

return;
//    BIOS::DBG::Print("istr=%d",  _GetISTR());
//    _SetISTR(0);
//    isr++;
/*
if (isr > 1000)
{
  isr = 0;
  BIOS::DBG::Print("ISR!=%d",  _GetISTR());
_SetISTR(0);
}
*/
//EVERY(100)
//{
  BIOS::DBG::Print("<ISR=%d>",  _GetISTR());
//}
  });
}

extern "C" void dbg(char* msg);
extern "C" void delay(int);

void delay(int n)
{
  BIOS::SYS::DelayMs(n);
}

void dbg(char* msg)
{
return;
if (msg == "sof") return;

  BIOS::DBG::Print("{%s}", msg);
}

void logUsb()
{
	#define USB_ISTR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x44))
	uint16_t istr = *USB_ISTR_REG;

	if (istr & USB_ISTR_RESET) BIOS::DBG::Print("[reset]");
	if (istr & USB_ISTR_CTR) BIOS::DBG::Print("[ctr]");
	if (istr & USB_ISTR_SUSP) BIOS::DBG::Print("[susp]");
	if (istr & USB_ISTR_WKUP) BIOS::DBG::Print("[wkup]");
	if (istr & USB_ISTR_SOF) 
         {
_SetISTR(~USB_ISTR_SOF);
EVERY(1000)
{
  BIOS::DBG::Print("[sof]");
}
}
}


__attribute__((__section__(".entry")))
int main(void) 
{
  resetUsb();

  BIOS::DBG::Print("USB begin\n");
        /* Setup USB */
        {
            char serial[USB_SERIAL_NUM_LENGTH+1];
            serial[0] = '\0';
            target_get_serial_number(serial, USB_SERIAL_NUM_LENGTH);
            usb_set_serial_number(serial);
        }

	usbd_dev = usb_setup();
        //dfu_setup(usbd_dev, &target_manifest_app, NULL, NULL);
//       	usb_msc_init(usbd_dev, 0x82, 64, 0x01, 64, "Example Ltd", "UF2 Bootloader",
//		    "42.00", UF2_NUM_BLOCKS, read_block, write_block);
//	  webusb_setup(usbd_dev);
        winusb_setup(usbd_dev);
        
        BIOS::KEY::EKey key;
        while (1)//(key = BIOS::KEY::GetKey())) 
        {
            key = BIOS::KEY::GetKey();

            if (key == BIOS::KEY::Escape)
            {
               break;
            }

//            logUsb();
//            usbd_poll(usbd_dev);

EVERY(1000)
{
  BIOS::DBG::Print("%d ", isr);
  isr = 0;
}
        }

  BIOS::DBG::Print("USB end\n");
    return 0;
}
