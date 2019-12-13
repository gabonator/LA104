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

#include <stdint.h>
#include <string.h>
#include <logger.h>
#include "usb_conf.h"
#include "usb21_standard.h"

#define DESCRIPTOR_CALLBACK_TYPE (USB_REQ_TYPE_IN | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE)
#define DESCRIPTOR_CALLBACK_MASK (USB_REQ_TYPE_DIRECTION | USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)

#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })

static uint16_t build_bos_descriptor(const struct usb_bos_descriptor *bos,
									 uint8_t *buf, uint16_t len) {
	uint8_t *tmpbuf = buf;
	uint16_t i, bytes_to_copy, total = 0, totallen = 0;

	bytes_to_copy = MIN(len, bos->bLength);
	memcpy(buf, bos, bytes_to_copy);
	buf += bytes_to_copy;
	len -= bytes_to_copy;
	total += bytes_to_copy;
	totallen += bos->bLength;

	/* For each device capability */
	for (i = 0; i < bos->bNumDeviceCaps; i++) {
		/* Copy device capability descriptor. */
		const struct usb_device_capability_descriptor *cap =
			bos->capabilities[i];
		bytes_to_copy = MIN(len, cap->bLength);
		memcpy(buf, cap, bytes_to_copy);
		buf += bytes_to_copy;
		len -= bytes_to_copy;
		total += bytes_to_copy;
		totallen += cap->bLength;
	}
	/* Fill in wTotalLength. */
	*(uint16_t *)(tmpbuf + 2) = totallen;
	return total;
}

static const struct usb_bos_descriptor* usb21_bos;

static int usb21_standard_get_descriptor(usbd_device* usbd_dev,
											struct usb_setup_data *req,
											uint8_t **buf, uint16_t *len,
											usbd_control_complete_callback* complete) {
	//  Handle a BOS request: 
	//  >> typ 80, req 06, val 0f00, idx 0000, len 0005, GET_DES_BOS t 0f i 00
	(void)complete;
	(void)usbd_dev;
	int descr_type = req->wValue >> 8;
	//  If not BOS request, hand off to next interface.
    if (descr_type != USB_DT_BOS) { return USBD_REQ_NEXT_CALLBACK; }
	if (!usb21_bos) {
		debug_println("*** usb21_descriptor no bos "); debug_flush(); ////
		return USBD_REQ_NOTSUPP;
	}
	if (req->bRequest == USB_REQ_GET_DESCRIPTOR) {
		dump_usb_request("bos", req); debug_flush(); ////
		*len = MIN(*len, build_bos_descriptor(usb21_bos, *buf, *len));

		uint8_t *b = *buf; int i;
		debug_print_unsigned(*len); debug_print(" / ");
		for (i = 0; i < *len; i++) { debug_printhex(b[i]); debug_print(" "); } debug_println(""); debug_flush(); ////

		return USBD_REQ_HANDLED;
	}
	return USBD_REQ_NEXT_CALLBACK;
}

static void usb21_set_config(usbd_device* usbd_dev, uint16_t wValue) {
    //  debug_println("usb21_set_config"); // debug_flush(); ////
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		DESCRIPTOR_CALLBACK_TYPE,
		DESCRIPTOR_CALLBACK_MASK,
		&usb21_standard_get_descriptor);
	if (status < 0) { debug_println("*** usb21_set_config failed"); debug_flush(); }
}

void usb21_setup(usbd_device* usbd_dev, const struct usb_bos_descriptor* binary_object_store) {
    // debug_println("usb21_setup"); // debug_flush(); ////
	usb21_bos = binary_object_store;

	/* Register the control request handler _before_ the config is set */
	usb21_set_config(usbd_dev, 0x0000);
	int status = aggregate_register_config_callback(usbd_dev, usb21_set_config);
	if (status < 0) { debug_println("*** usb21_setup failed"); debug_flush(); }
}

#ifdef NOTUSED

Captured BOS from Blue Pill: (57 bytes)

05 0f 39 00 02 
18 10 05 00 
38 b6 08 34 a9 09 a0 47 8b fd a0 76 88 15 b6 65 
00 01 
22 
01 

1c 10 05 00 
df 60 dd d8 89 45 c7 4c 9c d2 65 9d 9e 64 8a 9f 
00 00 03 06 
b2 00 21 00

Captured BOS from microbit:

Binary Object Store descriptor
05:0f:39:00:02:

WebUSB Platform Capability Descriptor: https://wicg.github.io/webusb/#webusb-platform-capability-descriptor
bLength: 18:
bDescriptorType: 10:
bDevCapabilityType: 05:
bReserved: 00:
PlatformCapability UUID:
platformCapabilityUUID: 38:b6:08:34:a9:09:a0:47:8b:fd:a0:76:88:15:b6:65:
bcdVersion: 00:01:
bVendorCode: 21:
iLandingPage: 00:

Microsoft OS 2.0 Platform Capability Descriptor:
bLength: 1c:
bDescriptorType: 10:
bDevCapabilityType: 05:
bReserved: 00:
MS OS 2.0 Platform Capability ID:
platformCapabilityUUID: df:60:dd:d8:89:45:c7:4c:9c:d2:65:9d:9e:64:8a:9f:
Windows version:
bcdVersion: 00:00:03:06:
Descriptor set length, Vendor code, Alternate enumeration code:
aa:00:20:00
#endif  //  NOTUSED
