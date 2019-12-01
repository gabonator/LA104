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

#include <libopencm3/usb/usbd.h>
#include <logger.h>
#include "usb_conf.h"
#include "winusb.h"

#define CONTROL_CALLBACK_TYPE USB_REQ_TYPE_VENDOR
#define CONTROL_CALLBACK_MASK USB_REQ_TYPE_TYPE
#define DESCRIPTOR_CALLBACK_TYPE USB_REQ_TYPE_DEVICE
#define DESCRIPTOR_CALLBACK_MASK USB_REQ_TYPE_RECIPIENT

#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })
static int usb_descriptor_type(uint16_t wValue) { return wValue >> 8; }
static int usb_descriptor_index(uint16_t wValue) { return wValue & 0xFF; }

//  Microsoft OS 2.0 Descriptor Set. Values from https://github.com/intel/zephyr.js/blob/master/src/zjs_webusb.c
//  See also http://download.microsoft.com/download/3/5/6/3563ED4A-F318-4B66-A181-AB1D8F6FD42D/MS_OS_2_0_desc.docx
//  http://searchingforbit.blogspot.com/2014/05/winusb-communication-with-stm32-round-2.html

static struct msos20_descriptor_set_struct msos20_descriptor_set = {
	//  Descriptor set header
	.set_header_descriptor = {
		.wLength          = MSOS20_SET_HEADER_DESCRIPTOR_SIZE,     //  Should be 10
		.wDescriptorType  = MSOS20_SET_HEADER_DESCRIPTOR,  //  MS OS 2.0 descriptor set header
		.dwWindowsVersion = MSOS20_WINDOWS_VERSION,       //  Windows version
		.wTotalLength     = MSOS20_DESCRIPTOR_SET_SIZE,       //  Size of entire MS OS 2.0 descriptor set
	},
	//  Configuration subset header: Which USB Configuration this descriptor will apply.
	.subset_header_configuration = {
		.wLength             = MSOS20_SUBSET_HEADER_CONFIGURATION_SIZE,  //  Should be 8
		.wDescriptorType     = MSOS20_SUBSET_HEADER_CONFIGURATION,
		.bConfigurationValue = 0,  //  Configuration ID = 0
		.bReserved           = 0,
		.wTotalLength        = MSOS20_SUBSET_CONFIGURATION_SIZE,  //  Should be 0xA8 (168).  Size of entire configuration subset including this header.
	},
	//  Function subset header: Which USB Interface this descriptor will apply.
	.subset_header_function = {
		.wLength         = MSOS20_SUBSET_HEADER_FUNCTION_SIZE,  //  Should be 8
		.wDescriptorType = MSOS20_SUBSET_HEADER_FUNCTION,
		.bFirstInterface = 0,  //  Note - this is rewritten in winusb_setup with the correct interface number
		.bReserved       = 0,
		.wSubsetLength   = MSOS20_SUBSET_FUNCTION_SIZE,  //  Should be 0xA0 (160).  Size of entire function subset including this header.
	},
	//  Compatible ID descriptor
	.feature_compatible_id = {
		.wLength         = MSOS20_FEATURE_COMPATIBLE_ID_SIZE,  //  Should be 20
		.wDescriptorType = MSOS20_FEATURE_COMPATIBLE_ID,
		.CompatibleID    = { 'W', 'I', 'N', 'U', 'S', 'B', 0, 0 },  // Compatible ID
		.SubCompatibleID = { 0, 0, 0, 0, 0, 0, 0, 0 },           // SubCompatible ID
	},
	//  Registry property descriptor: The properties that will be written to Windows registry.
	.feature_reg_property = {
		.wLength             = MSOS20_FEATURE_REG_PROPERTY_SIZE,  //  e.g. 0x84
		.wDescriptorType     = MSOS20_FEATURE_REG_PROPERTY,
		.wPropertyDataType   = WINUSB_EXTENDED_PROPERTIES_MULTISZ_DATA_TYPE,  //  Type of registry property
		.wPropertyNameLength = MSOS20_PROPERTY_NAME_LENGTH,  //  Length of property name
		.propertyName = {  // Property name: DeviceInterfaceGUIDs
			'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00,
			'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,
			'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00,
			'D', 0x00, 's', 0x00, 0x00, 0x00
		},
		.wPropertyDataLength = MSOS20_PROPERTY_DATA_LENGTH,  //  Length of property data		
		.propertyData = {  // Property data: {9D32F82C-1FB2-4486-8501-B6145B5BA336}
			'{', 0x00, '9', 0x00, 'D', 0x00, '3', 0x00, '2', 0x00, 'F', 0x00,
			'8', 0x00, '2', 0x00, 'C', 0x00, '-', 0x00, '1', 0x00, 'F', 0x00,
			'B', 0x00, '2', 0x00, '-', 0x00, '4', 0x00, '4', 0x00, '8', 0x00,
			'6', 0x00, '-', 0x00, '8', 0x00, '5', 0x00, '0', 0x00, '1', 0x00,
			'-', 0x00, 'B', 0x00, '6', 0x00, '1', 0x00, '4', 0x00, '5', 0x00,
			'B', 0x00, '5', 0x00, 'B', 0x00, 'A', 0x00, '3', 0x00, '3', 0x00,
			'6', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
		}
	}
};

static struct winusb_compatible_id_descriptor winusb_wcid = {
	.header = {
		.dwLength = sizeof(struct winusb_compatible_id_descriptor_header) +
					1 * sizeof(struct winusb_compatible_id_function_section),
		.bcdVersion = WINUSB_BCD_VERSION,
		.wIndex = WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR,
		.bNumSections = 1,
		.reserved = { 0, 0, 0, 0, 0, 0, 0 },
	},
	.functions = {
		{
			// note - bInterfaceNumber is rewritten in winusb_setup with the correct interface number
			.bInterfaceNumber = 0,
			.reserved0 = { 1 },
			.compatibleId = "WINUSB",
			.subCompatibleId = "",
			.reserved1 = { 0, 0, 0, 0, 0, 0}
		},
	}
};

static const struct usb_string_descriptor winusb_string_descriptor = {
	.bLength = 0x12,
	.bDescriptorType = USB_DT_STRING,
	.wData = WINUSB_EXTRA_STRING
};

static const struct winusb_extended_properties_descriptor guid = {
	.header = {
		.dwLength = sizeof(struct winusb_extended_properties_descriptor_header)
					+ 1 * sizeof (struct winusb_extended_properties_feature_descriptor),
		.bcdVersion = WINUSB_BCD_VERSION,
		.wIndex = WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR,
		.wNumFeatures = 1,
	},
	.features = {
		{
			.dwLength = sizeof(struct winusb_extended_properties_feature_descriptor),
			.dwPropertyDataType = WINUSB_EXTENDED_PROPERTIES_MULTISZ_DATA_TYPE,
			.wNameLength = WINUSB_EXTENDED_PROPERTIES_GUID_NAME_SIZE_C,
			.name = WINUSB_EXTENDED_PROPERTIES_GUID_NAME,
			.dwPropertyDataLength = WINUSB_EXTENDED_PROPERTIES_GUID_DATA_SIZE_C,
			.propertyData = WINUSB_EXTENDED_PROPERTIES_GUID_DATA,
		},
	}
};

static int winusb_descriptor_request(usbd_device *usbd_dev,
					struct usb_setup_data *req,
					uint8_t **buf, uint16_t *len,
					usbd_control_complete_callback* complete) {
	(void)complete;
	(void)usbd_dev;
	//  Skip non-standard requests not meant for me.
	if ((req->bmRequestType & USB_REQ_TYPE_TYPE) != USB_REQ_TYPE_STANDARD) { return USBD_REQ_NEXT_CALLBACK; }
	if (req->bRequest == USB_REQ_GET_DESCRIPTOR && usb_descriptor_type(req->wValue) == USB_DT_STRING) {
		if (usb_descriptor_index(req->wValue) == WINUSB_EXTRA_STRING_INDEX) {
			dump_usb_request("winee", req); debug_flush(); ////
			*buf = (uint8_t*) &winusb_string_descriptor;
			*len = MIN(*len, winusb_string_descriptor.bLength);
			return USBD_REQ_HANDLED;
		}
	}
	return USBD_REQ_NEXT_CALLBACK;
}

static int winusb_control_vendor_request(usbd_device *usbd_dev,
					struct usb_setup_data *req,
					uint8_t **buf, uint16_t *len,
					usbd_control_complete_callback* complete) {  (void)complete; (void)usbd_dev;
	//  Handle requests for MS OS 2.0 Descriptors, MS OS 1.0 Compatible ID and Extended Properties, like:
	//  >>  typ c0, req 21, val 0000, idx 0007, len 00b2 (MS OS 2.0 Descriptors)
	//  >>  type 0xc0, req 0x21, val 0, idx 4, len 16, type 0x00, index 0x00 (MS OS 1.0 Compatible ID)
	//  >>  type 0xc1, req 0x21, val 0, idx 5, len 10, type 0x00, index 0x00 (MS OS 1.0 Extended Properties)
	//  For WinUSB, only request types C0 and C1 are allowed.  Request code must be the MS vendor code (0x21).
	if (req->bmRequestType != 0xc0 && req->bmRequestType != 0xc1) { return USBD_REQ_NEXT_CALLBACK; }
	if (req->bRequest != WINUSB_MS_VENDOR_CODE) { return USBD_REQ_NEXT_CALLBACK; }

	int status = USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
	if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_DEVICE) &&
		(req->wIndex == MSOS20_REQ_GET_DESCRIPTOR)) {
		//  Request for the MS OS 2.0 Descriptor referenced by the BOS, e.g.
		//  >>  typ c0, req 21, val 0000, idx 0007, len 00b2
		//  See http://download.microsoft.com/download/3/5/6/3563ED4A-F318-4B66-A181-AB1D8F6FD42D/MS_OS_2_0_desc.docx
		dump_usb_request("windes", req); debug_flush(); ////
		*buf = (uint8_t*) &msos20_descriptor_set;
		*len = MIN(*len, MSOS20_DESCRIPTOR_SET_SIZE);
		status = USBD_REQ_HANDLED;

		uint8_t *b = (uint8_t*) &msos20_descriptor_set; int i;
		debug_print_unsigned(MSOS20_DESCRIPTOR_SET_SIZE); debug_print(" / ");
		for (i = 0; i < MSOS20_DESCRIPTOR_SET_SIZE; i++) { debug_printhex(b[i]); debug_print(" "); } debug_println(""); debug_flush(); ////

	} else if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_DEVICE) &&
		(req->wIndex == WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR)) {
		//  Request for the MS OS 1.0 Compatible ID feature ("WINUSB"), referenced by the Extended Properties e.g.
		//  >>  type 0xc0, req 0x21, val 0, idx 4, len 16, type 0x00, index 0x00
		dump_usb_request("winid", req); debug_flush(); ////
		*buf = (uint8_t*)(&winusb_wcid);
		*len = MIN(*len, winusb_wcid.header.dwLength);
		status = USBD_REQ_HANDLED;

	} else if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_INTERFACE) &&
		(req->wIndex == WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR) &&
		(usb_descriptor_index(req->wValue) == winusb_wcid.functions[0].bInterfaceNumber)) {
		//  Request for the MS OS 1.0 Extended Properties, which includes the Compatible ID feature e.g.
		//  >>  type 0xc1, req 0x21, val 0, idx 5, len 10, type 0x00, index 0x00
		//  From http://searchingforbit.blogspot.com/2014/05/winusb-communication-with-stm32-round-2.html:
		//  Note that Windows queries this descriptor only once. It can be a hassle during development. Information that OS descriptors have been queried for some device is stored in registry under
		//  HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\usbflags\VVVVPPPPRRRR (VVVV - vendor ID; PPPP - product ID; RRRR - revision).
		//  Delete VVVVPPPPRRRR key and also uninstall the device with utility like USDDeview to always get fresh device plug in behavior.
		dump_usb_request("winprp", req); debug_flush(); ////
		*buf = (uint8_t*)(&guid);
		*len = MIN(*len, guid.header.dwLength);
		status = USBD_REQ_HANDLED;

	} else {
		status = USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
	}
	return status;
}

static void winusb_set_config(usbd_device* usbd_dev, uint16_t wValue) {
	//  debug_println("winusb_set_config"); // debug_flush(); ////
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		winusb_control_vendor_request);
	if (status < 0) {
    	debug_println("*** winusb_set_config failed"); debug_flush(); ////
	}
}

void winusb_setup(usbd_device* usbd_dev, uint8_t interface) {
	//  debug_println("winusb_setup"); // debug_flush(); ////
	//  Send to host the USB Interface ID for the DFU Interface, which will support WinUSB.
	winusb_wcid.functions[0].bInterfaceNumber = interface;
	msos20_descriptor_set.subset_header_function.bFirstInterface = interface;
	int status = aggregate_register_config_callback(usbd_dev, winusb_set_config);

	//  Windows probes the compatible ID before setting the configuration, so also register the callback now.
	int status1 = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		winusb_control_vendor_request);
	int status2 = aggregate_register_callback(
		usbd_dev,
		DESCRIPTOR_CALLBACK_TYPE,
		DESCRIPTOR_CALLBACK_MASK,
		winusb_descriptor_request);
	if (status < 0 || status1 < 0 || status2 < 0) { debug_println("*** winusb_setup failed"); debug_flush(); }
}

#ifdef NOTUSED
Generated MS OS 2.0 Descriptor Set: 178 bytes

    0x0A, 0x00,              // wLength
    0x00, 0x00,              // MS OS 2.0 descriptor set header
    0x00, 0x00, 0x03, 0x06,  // Windows 8.1
    0xB2, 0x00,              // Size, MS OS 2.0 descriptor set

    // Configuration subset header
    0x08, 0x00,  // wLength
    0x01, 0x00,  // wDescriptorType
    0x00,        // bConfigurationValue
    0x00,        // bReserved
    0xA8, 0x00,  // wTotalLength of this subset header

    // Function subset header
    0x08, 0x00,  // wLength
    0x02, 0x00,  // wDescriptorType
    0x00,        // bFirstInterface
    0x00,        // bReserved
    0xA0, 0x00,  // wTotalLength of this subset header

    // Compatible ID descriptor
    0x14, 0x00,                                      // wLength
    0x03, 0x00,                                      // wDescriptorType
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,        // compatible ID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // subCompatibleID

    // Extended properties descriptor with interface GUID
    0x84, 0x00,  // wLength
    0x04, 0x00,  // wDescriptorType
    0x07, 0x00,  // wPropertyDataType
    0x2A, 0x00,  // wPropertyNameLength
    // Property name : DeviceInterfaceGUIDs
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00,
    'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,
    'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00,
    'D', 0x00, 's', 0x00, 0x00, 0x00,
    0x50, 0x00,  // wPropertyDataLength
    // Property data: {9D32F82C-1FB2-4486-8501-B6145B5BA336}
    '{', 0x00, '9', 0x00, 'D', 0x00, '3', 0x00, '2', 0x00, 'F', 0x00,
    '8', 0x00, '2', 0x00, 'C', 0x00, '-', 0x00, '1', 0x00, 'F', 0x00,
    'B', 0x00, '2', 0x00, '-', 0x00, '4', 0x00, '4', 0x00, '8', 0x00,
    '6', 0x00, '-', 0x00, '8', 0x00, '5', 0x00, '0', 0x00, '1', 0x00,
    '-', 0x00, 'B', 0x00, '6', 0x00, '1', 0x00, '4', 0x00, '5', 0x00,
    'B', 0x00, '5', 0x00, 'B', 0x00, 'A', 0x00, '3', 0x00, '3', 0x00,
    '6', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif  //  NOTUSED
