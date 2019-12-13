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

#ifndef WEBUSB_H_INCLUDED
#define WEBUSB_H_INCLUDED

#include <libopencm3/usb/usbd.h>
#include "webusb_defs.h"
#include "winusb_defs.h"

// Arbitrary
#define WEBUSB_VENDOR_CODE 0x22  //  Don't use 0x21, reserved for WinUSB.

extern const struct webusb_platform_descriptor webusb_platform_capability_descriptor;
extern const struct webusb_platform_descriptor webusb_platform_capability_descriptor_no_landing_page;
extern const struct microsoft_platform_descriptor microsoft_platform_capability_descriptor;

extern void webusb_setup(usbd_device* usbd_dev, const char* https_url);

#endif
