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

#ifndef USB_CONF_H_INCLUDED
#define USB_CONF_H_INCLUDED

#include <libopencm3/usb/usbd.h>

#define USB_VID                 0x1209
#define USB_PID                 0xdb42
#define USB_SERIAL_NUM_LENGTH   24
#define USB_CONTROL_BUF_SIZE    256  //  Previously 1024
#define MAX_USB_PACKET_SIZE     64   //  Previously 32

#define USB21_INTERFACE                       //  Enable USB 2.1 with WebUSB and BOS support.
#define SERIAL_USB_INTERFACE              //  Enable only serial USB interface.  No DFU and storage.

//  Index of each USB interface.  Must be consecutive and must sync with interfaces[].
#ifdef ALL_USB_INTERFACES
#define INTF_DFU                0
#define INTF_MSC                1
#define INTF_COMM               2
#define INTF_DATA               3
#endif  //  ALL_USB_INTERFACES

#ifdef STORAGE_AND_SERIAL_USB_INTERFACE
#define INTF_MSC                0
#define INTF_COMM               1  //  COMM must be immediately before DATA because of Associated Interface Descriptor.
#define INTF_DATA               2
#endif  //  STORAGE_AND_SERIAL_USB_INTERFACE

#ifdef SERIAL_USB_INTERFACE
#define INTF_COMM               0  //  COMM must be immediately before DATA because of Associated Interface Descriptor.
#define INTF_DATA               1
#endif  //  SERIAL_USB_INTERFACE

//  USB Endpoints.
#define MSC_OUT                 0x01
#define DATA_OUT                0x03

#define MSC_IN                  0x82
#define DATA_IN                 0x84
#define COMM_IN                 0x85

#ifdef NOTUSED
#define DATA_IN                 0x82
#define COMM_IN                 0x83
#define MSC_IN                  0x84

#define DATA_OUT                0x01
#define MSC_OUT                 0x02
#endif  //  NOTUSED

extern void usb_set_serial_number(const char* serial);
extern usbd_device* usb_setup(void);
extern void msc_setup(usbd_device* usbd_dev0);
extern uint16_t send_msc_packet(const void *buf, int len);
extern int aggregate_register_config_callback(
    usbd_device *usbd_dev,
	usbd_set_config_callback callback);
extern int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback);


////

#define debug_println(x...) 
#define debug_flush() 
#define debug_print(x...)
#define debug_print_unsigned(x...)
#define debug_printhex(x...)
#ifndef NULL
#define NULL 0
#endif

void dbg(char*);

#endif
