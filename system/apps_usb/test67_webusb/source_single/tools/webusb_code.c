//#include "webusb.h"

#define bool uint8_t 
#define true 1
#define false 0

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


// Arbitrary
#define WEBUSB_VENDOR_CODE 0x22  //  Don't use 0x21, reserved for WinUSB.

extern const struct webusb_platform_descriptor webusb_platform_capability_descriptor;
extern const struct webusb_platform_descriptor webusb_platform_capability_descriptor_no_landing_page;
extern const struct microsoft_platform_descriptor microsoft_platform_capability_descriptor;

extern void webusb_setup(usbd_device* usbd_dev, const char* https_url);

#endif
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

#ifndef WEBUSB_DEFS_H_INCLUDED
#define WEBUSB_DEFS_H_INCLUDED


#define WEBUSB_REQ_GET_URL             0x02

#define WEBUSB_DT_DESCRIPTOR_SET_HEADER 0
#define WEBUSB_DT_CONFIGURATION_SUBSET_HEADER 1
#define WEBUSB_DT_FUNCTION_SUBSET_HEADER 2
#define WEBUSB_DT_URL 3

#define WEBUSB_URL_SCHEME_HTTP 0
#define WEBUSB_URL_SCHEME_HTTPS 1

struct webusb_platform_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint8_t bReserved;
	uint8_t platformCapabilityUUID[16];
	uint16_t bcdVersion;
	uint8_t bVendorCode;
	uint8_t iLandingPage;
} __attribute__((packed));

// from https://wicg.github.io/webusb/#webusb-platform-capability-descriptor
// see also this (for endianness explanation)
// https://github.com/WICG/webusb/issues/115#issuecomment-352206549
#define WEBUSB_UUID { 0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47,0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65 }
#define WEBUSB_PLATFORM_DESCRIPTOR_SIZE sizeof(struct webusb_platform_descriptor)

struct webusb_url_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bScheme;
	char URL[];
} __attribute__((packed));

#define WEBUSB_DT_URL_DESCRIPTOR_SIZE 3

#endif
#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

typedef void (*cdc_receive_callback)(uint8_t* buf, int len);

extern void cdc_setup(usbd_device* usbd_dev);
extern void cdc_set_receive_callback(cdc_receive_callback callback);
void cdc_transmit(uint8_t* buffer, int len);
#endif  //  CDC_H_INCLUDED
//  CDC code from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c
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


const usbd_driver* target_usb_init(void) 
{
    return &st_usbfs_v1_usb_driver;
}

static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue
);

static const char* origin_url = "visualbluepill.github.io";

static char serial_number[USB_SERIAL_NUM_LENGTH+1];

static const char *usb_strings[] = {
    "Devanarchy",              //  USB Manufacturer
    "DAPBoot DFU Bootloader",  //  USB Product
    serial_number,             //  Serial number
    //"Blue Pill DFU",         //  DFU
    "DAPBoot DFU",             //  DFU
    "Blue Pill MSC",           //  MSC
    "Blue Pill Serial Port",   //  Serial Port
    "Blue Pill COMM",          //  COMM
    "Blue Pill DATA",          //  DATA
};

#define MSC_VENDOR_ID "BluePill"  //  Max 8 chars
#define MSC_PRODUCT_ID "UF2 Bootloader"  //  Max 16 chars
#define MSC_PRODUCT_REVISION_LEVEL "2.1"  //  Max 4 chars
#define USB_CLASS_MISCELLANEOUS 0xef  //  Copy from microbit.

enum usb_strings_index {  //  Index of USB strings.  Must sync with above, starts from 1.
    USB_STRINGS_MANUFACTURER = 1,
    USB_STRINGS_PRODUCT,
    USB_STRINGS_SERIAL_NUMBER,
    USB_STRINGS_DFU,
    USB_STRINGS_MSC,
    USB_STRINGS_SERIAL_PORT,
    USB_STRINGS_COMM,
    USB_STRINGS_DATA,
};

//  USB Device
static const struct usb_device_descriptor dev = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0210,  //  USB Version 2.1.  Need to handle special requests e.g. BOS.
	.bDeviceClass = USB_CLASS_CDC,  //  Set the class to CDC if it's only serial.  Serial interface will not start on Windows when class = 0.
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = MAX_USB_PACKET_SIZE,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = 0x0220,  //  Device Release number 2.2
    .iManufacturer = USB_STRINGS_MANUFACTURER,
    .iProduct = USB_STRINGS_PRODUCT,
    .iSerialNumber = USB_STRINGS_SERIAL_NUMBER,
    .bNumConfigurations = 1,
};

//  CDC Endpoints

/*
 * This notification endpoint isn't implemented. According to CDC spec it's
 * optional, but its absence causes a NULL pointer dereference in the
 * Linux cdc_acm driver. (Gareth McMullin <gareth@blacksphere.co.nz>)
 */
static const struct usb_endpoint_descriptor comm_endp[] = {
	{
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = COMM_IN,
		.bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
		.wMaxPacketSize = COMM_PACKET_SIZE,  //  Smaller than others
		.bInterval = 255,
	}
};

static const struct usb_endpoint_descriptor data_endp[] = {
	{
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = DATA_OUT,
		.bmAttributes = USB_ENDPOINT_ATTR_BULK,
		.wMaxPacketSize = MAX_USB_PACKET_SIZE,
		.bInterval = 1,
	}, {
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = DATA_IN,
		.bmAttributes = USB_ENDPOINT_ATTR_BULK,
		.wMaxPacketSize = MAX_USB_PACKET_SIZE,
		.bInterval = 1,
	}
};

static const struct {
	struct usb_cdc_header_descriptor header;
	struct usb_cdc_call_management_descriptor call_mgmt;
	struct usb_cdc_acm_descriptor acm;
	struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) cdcacm_functional_descriptors = {
	.header = {
		.bFunctionLength = sizeof(struct usb_cdc_header_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_HEADER,
		.bcdCDC = 0x0110,
	},
	.call_mgmt = {
		.bFunctionLength =
			sizeof(struct usb_cdc_call_management_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
		.bmCapabilities = 0,
		.bDataInterface = INTF_DATA,  //  DATA Interface
	},
	.acm = {
		.bFunctionLength = sizeof(struct usb_cdc_acm_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_ACM,
		.bmCapabilities = 0,
	},
	.cdc_union = {
		.bFunctionLength = sizeof(struct usb_cdc_union_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_UNION,
		.bControlInterface = INTF_COMM,       //  COMM Interface
		.bSubordinateInterface0 = INTF_DATA,  //  DATA Interface
	 }
};

//  CDC Interfaces
static const struct usb_iface_assoc_descriptor cdc_iface_assoc = {  //  Copied from microbit.  Mandatory for composite device.
	.bLength = USB_DT_INTERFACE_ASSOCIATION_SIZE,
	.bDescriptorType = USB_DT_INTERFACE_ASSOCIATION,
	.bFirstInterface = INTF_COMM,  //  First associated interface (INTF_COMM and INTF_DATA)
	.bInterfaceCount = 2,          //  Total number of associated interfaces (INTF_COMM and INTF_DATA), ID must be consecutive.
	.bFunctionClass = USB_CLASS_CDC,
	.bFunctionSubClass = USB_CDC_SUBCLASS_ACM,
	.bFunctionProtocol = USB_CDC_PROTOCOL_AT,
	.iFunction = USB_STRINGS_SERIAL_PORT  //  Name of Serial Port
};

static const struct usb_interface_descriptor comm_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = INTF_COMM,
    .bAlternateSetting = 0,
    .bNumEndpoints = 1,
    .bInterfaceClass = USB_CLASS_CDC,
    .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
    .bInterfaceProtocol = USB_CDC_PROTOCOL_AT,
    .iInterface = USB_STRINGS_COMM,  //  Name of COMM
    .endpoint = comm_endp,           //  COMM Endpoint
    .extra = &cdcacm_functional_descriptors,
    .extralen = sizeof(cdcacm_functional_descriptors)
};

static const struct usb_interface_descriptor data_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = INTF_DATA,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = USB_CLASS_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USB_STRINGS_DATA,  //  Name of DATA
    .endpoint = data_endp,           //  DATA Endpoints
};

//  All USB Interfaces
static const struct usb_interface interfaces[] = {
    {
        .num_altsetting = 1,
        .altsetting = &comm_iface,  //  Index must sync with INTF_COMM.
    }, 
    {
        .num_altsetting = 1,
        .altsetting = &data_iface,  //  Index must sync with INTF_DATA.
    },
};

//  USB Config
static const struct usb_config_descriptor config = {
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = sizeof(interfaces) / sizeof(struct usb_interface),
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80,  //  Bus-powered, i.e. it draws power from USB bus.
    .bMaxPower = 0xfa,     //  500 mA. Copied from microbit.
    .interface = interfaces,
};

//  BOS Capabilities for WebUSB and Microsoft Platform
static const struct usb_device_capability_descriptor* capabilities[] = {
	(const struct usb_device_capability_descriptor*) 
        &webusb_platform_capability_descriptor,
};

//  BOS Descriptor for WebUSB and Microsoft Platform
static const struct usb_bos_descriptor bos_descriptor = {
	.bLength = USB_DT_BOS_SIZE,
	.bDescriptorType = USB_DT_BOS,
	.bNumDeviceCaps = sizeof(capabilities) / sizeof(capabilities[0]),
	.capabilities = capabilities
};

/* Buffer to be used for control requests. */
static uint8_t usbd_control_buffer[USB_CONTROL_BUF_SIZE] __attribute__ ((aligned (2)));
usbd_device* usbd_dev = NULL;

usbd_device* usb_setup(void) {

    char serial[USB_SERIAL_NUM_LENGTH+1];
    serial[0] = '\0';
    desig_get_unique_id_as_string(serial, USB_SERIAL_NUM_LENGTH);
    usb_set_serial_number(serial);


    int num_strings = sizeof(usb_strings) / sizeof(const char*);
    // debug_print("usb_setup num_strings "); debug_print_int(num_strings); debug_println(""); // debug_flush(); ////
    const usbd_driver* driver = target_usb_init();

    usbd_dev = usbd_init(driver, &dev, &config, 

  &bos_descriptor, // gabo

        usb_strings, num_strings,
        usbd_control_buffer, sizeof(usbd_control_buffer));

    //  The following USB setup functions will call aggregate_register_callback() to register callbacks.
    cdc_setup(usbd_dev);
    //  Define USB 2.1 BOS interface used by WebUSB.
	webusb_setup(usbd_dev, origin_url);

    //  Set the aggregate callback.    
	int status = usbd_register_set_config_callback(usbd_dev, set_aggregate_callback);
    if (status < 0) { debug_println("*** usb_setup failed"); debug_flush(); }

    //  For WinUSB: Windows probes the compatible ID before setting the configuration, so also register the callback now.
    set_aggregate_callback(usbd_dev, (uint16_t) -1);
    return usbd_dev;
}


struct control_callback_struct {
    uint8_t type;
    uint8_t type_mask;
    usbd_control_callback cb;
};

#define MAX_CONTROL_CALLBACK 10
static struct control_callback_struct control_callback[MAX_CONTROL_CALLBACK];
static usbd_set_config_callback config_callback[MAX_CONTROL_CALLBACK];

int aggregate_register_config_callback(
    usbd_device *usbd_dev,
	usbd_set_config_callback callback) {
    //  Register the USB config callback.  We do this to overcome the 4 callback limit.
	int i;
	for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
		if (config_callback[i]) {
            if (config_callback[i] == callback) { return 0; }  //  Skip duplicate.
			continue;
		}
		config_callback[i] = callback;
		return 0;
	}
    debug_println("*** ERROR: Too many config callbacks"); debug_flush();
	return -1;
}

int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback) {
    // Register application callback function for handling USB control requests.  We aggregate here so we can handle more than 4 callbacks.
    // debug_println("aggregate_register_callback"); ////
	int i;
	for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
		if (control_callback[i].cb) { 
            //  If already exists, skip.
            if (control_callback[i].type == type &&
                control_callback[i].type_mask == type_mask &&
                control_callback[i].cb == callback) { 
                    //  debug_println("callback exists"); ////
                    return 0;
                }
            continue;  //  Continue checking.
        }
		control_callback[i].type = type;
		control_callback[i].type_mask = type_mask;
		control_callback[i].cb = callback;
		return 0;
	}
    debug_println("*** ERROR: Too many control callbacks"); debug_flush();
	return -1;
}

static uint8_t usb_descriptor_type(uint16_t wValue) {
	return wValue >> 8;
}

static uint8_t usb_descriptor_index(uint16_t wValue) {
	return wValue & 0xFF;
}

uint16_t device_address = (uint16_t) -1;

static int aggregate_callback(
    usbd_device *usbd_dev,
	struct usb_setup_data *req, 
    uint8_t **buf, 
    uint16_t *len,
	usbd_control_complete_callback *complete) {
    //  This callback is called whenever a USB request is received.  We route to the right driver callbacks.
	int i, result = 0;
    //  Call the callbacks registered by the drivers.
    for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
        if (control_callback[i].cb == NULL) { break; }
        if ((req->bmRequestType & control_callback[i].type_mask) == control_callback[i].type) {
            result = control_callback[i].cb(
                usbd_dev, 
                req,
                buf,
                len,
                complete);
            if (result == USBD_REQ_HANDLED ||
                result == USBD_REQ_NOTSUPP) {
                return result;
            }
        }
    }
    if (!(req->bmRequestType == 0x80 && req->bRequest == 0x06)) {
        //  Dump the packet if not GET_DESCRIPTOR.
//	    dump_usb_request(">> ", req); debug_flush(); ////
    } 
	return USBD_REQ_NEXT_CALLBACK;
}

static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue) {
    //  This callback is called when the device is updated.  We set our control callback.
    if (wValue != (uint16_t) -1) {  //  If this is an actual callback, not a call by usb_setup()...
        //  Call the config functions before setting our callback.
        debug_println("set_aggregate_callback"); ////
        int i;
        for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
            if (!config_callback[i]) { break; }
            (config_callback[i])(usbd_dev, wValue);
        }
    }
    //  Set our callback.
	int status;
        status = usbd_register_control_callback(
		usbd_dev,
        0,  //  Register for all notifications.
        0,
		aggregate_callback);
	if (status < 0) { debug_println("*** ERROR: set_aggregate_callback failed"); debug_flush(); }  
}

void usb_set_serial_number(const char* serial) {
    serial_number[0] = '\0';
    if (serial) {
        strncpy(serial_number, serial, USB_SERIAL_NUM_LENGTH);
        serial_number[USB_SERIAL_NUM_LENGTH] = '\0';
    }
}
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

//  Test WebUSB in Chrome console:
//  navigator.usb.requestDevice({ filters: [] }).then(console.log)
//  navigator.usb.getDevices().then(console.log)


#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_DEVICE)
#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)

#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })

//  WebUSB Descriptor with landing page.
const struct webusb_platform_descriptor webusb_platform_capability_descriptor = {
	.bLength = WEBUSB_PLATFORM_DESCRIPTOR_SIZE,
	.bDescriptorType = USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType = USB_DC_PLATFORM,
	.bReserved = 0,
	.platformCapabilityUUID = WEBUSB_UUID,
	.bcdVersion = 0x0100,
	.bVendorCode = WEBUSB_VENDOR_CODE,
	.iLandingPage = 1
};

//  WebUSB Descriptor without landing page.
const struct webusb_platform_descriptor webusb_platform_capability_descriptor_no_landing_page = {
	.bLength = WEBUSB_PLATFORM_DESCRIPTOR_SIZE,
	.bDescriptorType = USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType = USB_DC_PLATFORM,
	.bReserved = 0,
	.platformCapabilityUUID = WEBUSB_UUID,
	.bcdVersion = 0x0100,
	.bVendorCode = WEBUSB_VENDOR_CODE,
	.iLandingPage = 0
};

static const char* webusb_https_url;

static int webusb_control_vendor_request(usbd_device *usbd_dev,
									 struct usb_setup_data *req,
									 uint8_t **buf, uint16_t *len,
									 usbd_control_complete_callback* complete) {
	//  Handle a request for the WebUSB URL:
	//  >> typ c0, req 22, val 0001, idx 0002, len 00ff
	(void)complete;
	(void)usbd_dev;
	//  For WebUSB, only request types C0 and C1 are allowed.
	if (req->bmRequestType != 0xc0 && req->bmRequestType != 0xc1) { return USBD_REQ_NEXT_CALLBACK; }
	if (req->bRequest != WEBUSB_VENDOR_CODE) { return USBD_REQ_NEXT_CALLBACK; }
	int status = USBD_REQ_NOTSUPP;
	switch (req->wIndex) {
		case WEBUSB_REQ_GET_URL: {
			struct webusb_url_descriptor* url = (struct webusb_url_descriptor*)(*buf);
			uint16_t index = req->wValue;
			if (index == 0) {
    			debug_print("*** webusb notsupp index "); debug_print_unsigned(index); debug_println(""); debug_flush(); ////
				return USBD_REQ_NOTSUPP;
			}
			if (index == 1) {
//				dump_usb_request("weburl", req); debug_flush(); ////
				size_t url_len = strlen(webusb_https_url);
				url->bLength = WEBUSB_DT_URL_DESCRIPTOR_SIZE + url_len;
				url->bDescriptorType = WEBUSB_DT_URL;
				url->bScheme = WEBUSB_URL_SCHEME_HTTPS;
				memcpy(&url->URL, webusb_https_url, url_len);
				*len = MIN(*len, url->bLength);
				status = USBD_REQ_HANDLED;
			} else {
				// TODO: stall instead?
    			debug_print("*** webusb notsupp index "); debug_print_unsigned(index); debug_println(""); debug_flush(); ////
				status = USBD_REQ_NOTSUPP;
			}
			break;
		}
		default: {
    		debug_print("*** webusb notsupp wIndex "); debug_print_unsigned(req->wIndex); debug_println(""); debug_flush(); ////
			status = USBD_REQ_NOTSUPP;
			break;
		}
	}

	return status;
}

static void webusb_set_config(usbd_device* usbd_dev, uint16_t wValue) {
    //  debug_println("webusb_set_config"); // debug_flush(); ////
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		webusb_control_vendor_request);
	if (status < 0) { debug_println("*** webusb_set_config failed"); debug_flush(); }
}

void webusb_setup(usbd_device* usbd_dev, const char* https_url) {
    // debug_println("webusb_setup"); // debug_flush(); ////
	webusb_https_url = https_url;

	//  Register the callback now because WebUSB requests come earlier.
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		webusb_control_vendor_request);
	if (status < 0) { debug_println("*** webusb_setup failed"); debug_flush(); }

    //  Re-register the callback in case the USB restarts.
	status = aggregate_register_config_callback(usbd_dev, webusb_set_config);
	if (status < 0) { debug_println("*** webusb_setup failed"); debug_flush(); }
}

//  CDC code from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c

//#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
//#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)
#define USB_CDC_REQ_GET_LINE_CODING		0x21

//  Line config to be returned.
static const struct usb_cdc_line_coding line_coding = {
	.dwDTERate = 115200,
	.bCharFormat = USB_CDC_1_STOP_BITS,
	.bParityType = USB_CDC_NO_PARITY,
	.bDataBits = 0x08
};

cdc_receive_callback receive_callback = NULL;
usbd_device* transmit_device = NULL;

int cdcacm_control_request(
  usbd_device *usbd_dev __attribute__((unused)),
  struct usb_setup_data *req,
  uint8_t **buf __attribute__((unused)),
  uint16_t *len,
  void (**complete)(
    usbd_device *usbd_dev,
    struct usb_setup_data *req
  ) __attribute__((unused))
) {
	//  Handle USB Control Requests
	//  dump_usb_request("*** cdc", req); ////
	switch (req->bRequest) {
		case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {
			/* From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
			* This Linux cdc_acm driver requires this to be implemented
			* even though it's optional in the CDC spec, and we don't
			* advertise it in the ACM functional descriptor. */
#ifdef NOTUSED
			char local_buf[10];
			struct usb_cdc_notification *notif = (void *)local_buf;

			/* We echo signals back to host as notification. */
			notif->bmRequestType = 0xA1;
			notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
			notif->wValue = 0;
			notif->wIndex = 0;
			notif->wLength = 2;
			local_buf[8] = req->wValue & 3;
			local_buf[9] = 0;
			// usbd_ep_write_packet(0x83, buf, 10);
#endif  //  NOTUSED
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_GET_LINE_CODING: {
			//  Windows requires this request, not Mac or Linux.
			//  From https://github.com/PX4/Bootloader/blob/master/stm32/cdcacm.c
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
				debug_print("*** cdcacm_control notsupp line_coding "); debug_print_unsigned(sizeof(struct usb_cdc_line_coding)); 
				debug_print(", len "); debug_print_unsigned(*len);
				debug_println(""); debug_flush(); ////
				return USBD_REQ_NOTSUPP;
			}
			*buf = (uint8_t *) &line_coding;
			*len = sizeof(struct usb_cdc_line_coding);
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_SET_LINE_CODING: {
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
				debug_print("*** cdcacm_control notsupp line_coding "); debug_print_unsigned(sizeof(struct usb_cdc_line_coding)); 
				debug_print(", len "); debug_print_unsigned(*len);
				debug_println(""); debug_flush(); ////
				return USBD_REQ_NOTSUPP;
			}
			return USBD_REQ_HANDLED;
		}
	}
	//  dump_usb_request("*** cdc next", req); ////
	return USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
}

//  TODO: TX Up to MAX_USB_PACKET_SIZE
//  usbd_ep_write_packet(usbd_dev, DATA_IN, txbuf, txlen)

static char cdcbuf[MAX_USB_PACKET_SIZE + 1];   // rx buffer

/*
 * USB Receive Callback:
 */
static void
cdcacm_data_rx_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
	uint16_t len = usbd_ep_read_packet(usbd_dev, DATA_OUT, cdcbuf, MAX_USB_PACKET_SIZE);
    if (len == 0) { return; }
    uint16_t pos = (len < MAX_USB_PACKET_SIZE) ? len : MAX_USB_PACKET_SIZE;
    cdcbuf[pos] = 0;

//	usbd_ep_write_packet(usbd_dev, DATA_IN, cdcbuf, pos); ////  Echo the packet.
	
//    debug_print("["); debug_println(cdcbuf); debug_print("]"); // debug_flush(); ////
//dbg(cdcbuf);
  if (receive_callback)
    receive_callback((uint8_t*)cdcbuf, pos);
}

static void
cdcacm_comm_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
//	debug_println("comm"); debug_flush();
}

/*
 * USB Configuration:
 */
static void
cdcacm_set_config(
  usbd_device *usbd_dev,
  uint16_t wValue __attribute__((unused))
) {
	//  From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
    //  debug_println("*** cdcacm_set_config"); ////
	usbd_ep_setup(usbd_dev, DATA_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, DATA_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, NULL);
	usbd_ep_setup(usbd_dev, COMM_IN, USB_ENDPOINT_ATTR_INTERRUPT, COMM_PACKET_SIZE, cdcacm_comm_cb);
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		cdcacm_control_request);

	if (status < 0) { debug_println("*** cdcacm_set_config failed"); debug_flush(); }
}

void cdc_setup(usbd_device* usbd_dev) {
    //  debug_println("*** cdc_setup"); ////
	int status = aggregate_register_config_callback(usbd_dev, cdcacm_set_config);
	if (status < 0) { debug_println("*** cdc_setup failed"); debug_flush(); }
  transmit_device = usbd_dev;
}

void cdc_set_receive_callback(cdc_receive_callback callback)
{
  receive_callback = callback;
}

void cdc_transmit(uint8_t* buffer, int len)
{
  usbd_ep_write_packet(transmit_device, DATA_IN, buffer, len);
}
/** @defgroup usb_drivers_file Generic USB Drivers

@ingroup USB

@brief <b>Generic USB Drivers</b>

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010
Gareth McMullin <gareth@blacksphere.co.nz>

@date 10 March 2013

LGPL License Terms @ref lgpl_license
*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/


usbd_device *usbd_init(const usbd_driver *driver,
		       const struct usb_device_descriptor *dev,
		       const struct usb_config_descriptor *conf,
               const struct usb_bos_descriptor *bos,
		       const char **strings, int num_strings,
		       uint8_t *control_buffer, uint16_t control_buffer_size)
{
	usbd_device *usbd_dev;

	usbd_dev = driver->init();

	usbd_dev->driver = driver;
	usbd_dev->desc = dev;
	usbd_dev->config = conf;
    usbd_dev->bos = bos;
	usbd_dev->strings = strings;
	usbd_dev->num_strings = num_strings;
	usbd_dev->extra_string_idx = -1;
	usbd_dev->extra_string = NULL;
	usbd_dev->ctrl_buf = control_buffer;
	usbd_dev->ctrl_buf_len = control_buffer_size;

	usbd_dev->user_callback_ctr[0][USB_TRANSACTION_SETUP] =
	    _usbd_control_setup;
	usbd_dev->user_callback_ctr[0][USB_TRANSACTION_OUT] =
	    _usbd_control_out;
	usbd_dev->user_callback_ctr[0][USB_TRANSACTION_IN] =
	    _usbd_control_in;

	int i;
	for (i = 0; i < MAX_USER_SET_CONFIG_CALLBACK; i++) {
		usbd_dev->user_callback_set_config[i] = NULL;
	}

	return usbd_dev;
}

void usbd_register_reset_callback(usbd_device *usbd_dev, void (*callback)(void))
{
	usbd_dev->user_callback_reset = callback;
}

void usbd_register_suspend_callback(usbd_device *usbd_dev,
				    void (*callback)(void))
{
	usbd_dev->user_callback_suspend = callback;
}

void usbd_register_resume_callback(usbd_device *usbd_dev,
				   void (*callback)(void))
{
	usbd_dev->user_callback_resume = callback;
}

void usbd_register_sof_callback(usbd_device *usbd_dev, void (*callback)(void))
{
	usbd_dev->user_callback_sof = callback;
}

void usbd_register_extra_string(usbd_device *usbd_dev, int index, const char* string)
{
	if (string != NULL && index > 0)
	{
		usbd_dev->extra_string_idx = index;
		usbd_dev->extra_string = string;
	}
	else
	{
		usbd_dev->extra_string_idx = -1;
	}
}

void _usbd_reset(usbd_device *usbd_dev)
{
	usbd_dev->current_address = 0;
	usbd_dev->current_config = 0;
	usbd_ep_setup(usbd_dev, 0, USB_ENDPOINT_ATTR_CONTROL, usbd_dev->desc->bMaxPacketSize0, NULL);
	usbd_dev->driver->set_address(usbd_dev, 0);

	if (usbd_dev->user_callback_reset) {
		usbd_dev->user_callback_reset();
	}
}

/* Functions to wrap the low-level driver */
void usbd_poll(usbd_device *usbd_dev)
{
	usbd_dev->driver->poll(usbd_dev);
}

void usbd_disconnect(usbd_device *usbd_dev, bool disconnected)
{
	/* not all drivers support disconnection */
	if (usbd_dev->driver->disconnect) {
		usbd_dev->driver->disconnect(usbd_dev, disconnected);
	}
}

void usbd_ep_setup(usbd_device *usbd_dev, uint8_t addr, uint8_t type,
		   uint16_t max_size, usbd_endpoint_callback callback)
{
	usbd_dev->driver->ep_setup(usbd_dev, addr, type, max_size, callback);
}

uint16_t usbd_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
			 const void *buf, uint16_t len)
{
	return usbd_dev->driver->ep_write_packet(usbd_dev, addr, buf, len);
}

uint16_t usbd_ep_read_packet(usbd_device *usbd_dev, uint8_t addr, void *buf,
			     uint16_t len)
{
	return usbd_dev->driver->ep_read_packet(usbd_dev, addr, buf, len);
}

void usbd_ep_stall_set(usbd_device *usbd_dev, uint8_t addr, uint8_t stall)
{
	usbd_dev->driver->ep_stall_set(usbd_dev, addr, stall);
}

uint8_t usbd_ep_stall_get(usbd_device *usbd_dev, uint8_t addr)
{
	return usbd_dev->driver->ep_stall_get(usbd_dev, addr);
}

void usbd_ep_nak_set(usbd_device *usbd_dev, uint8_t addr, uint8_t nak)
{
	usbd_dev->driver->ep_nak_set(usbd_dev, addr, nak);
}

/**@}*/

/** @defgroup usb_control_file Generic USB Control Requests

@ingroup USB

@brief <b>Generic USB Control Requests</b>

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010
Gareth McMullin <gareth@blacksphere.co.nz>

@date 10 March 2013

LGPL License Terms @ref lgpl_license
*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/


/*
 * According to the USB 2.0 specification, section 8.5.3, when a control
 * transfer is stalled, the pipe becomes idle. We provide one utility to stall
 * a transaction to reduce boilerplate code.
 */
static void stall_transaction(usbd_device *usbd_dev)
{
	usbd_ep_stall_set(usbd_dev, 0, 1);
	usbd_dev->control_state.state = IDLE;
}

/**
 * If we're replying with _some_ data, but less than the host is expecting,
 * then we normally just do a short transfer.  But if it's short, but a
 * multiple of the endpoint max packet size, we need an explicit ZLP.
 * @param len how much data we want to transfer
 * @param wLength how much the host asked for
 * @param ep_size
 * @return
 */
static bool needs_zlp(uint16_t len, uint16_t wLength, uint8_t ep_size)
{
	if (len < wLength) {
		if (len && (len % ep_size == 0)) {
			return true;
		}
	}
	return false;
}

/* Register application callback function for handling USB control requests. */
int usbd_register_control_callback(usbd_device *usbd_dev, uint8_t type,
				   uint8_t type_mask,
				   usbd_control_callback callback)
{
	int i;

	for (i = 0; i < MAX_USER_CONTROL_CALLBACK; i++) {
		if (usbd_dev->user_control_callback[i].cb) {
			continue;
		}

		usbd_dev->user_control_callback[i].type = type;
		usbd_dev->user_control_callback[i].type_mask = type_mask;
		usbd_dev->user_control_callback[i].cb = callback;
		return 0;
	}

	return -1;
}

static void usb_control_send_chunk(usbd_device *usbd_dev)
{
	if (usbd_dev->desc->bMaxPacketSize0 <
			usbd_dev->control_state.ctrl_len) {
		/* Data stage, normal transmission */
		usbd_ep_write_packet(usbd_dev, 0,
				     usbd_dev->control_state.ctrl_buf,
				     usbd_dev->desc->bMaxPacketSize0);
		usbd_dev->control_state.state = DATA_IN;
		usbd_dev->control_state.ctrl_buf +=
			usbd_dev->desc->bMaxPacketSize0;
		usbd_dev->control_state.ctrl_len -=
			usbd_dev->desc->bMaxPacketSize0;
	} else {
		/* Data stage, end of transmission */
		usbd_ep_write_packet(usbd_dev, 0,
				     usbd_dev->control_state.ctrl_buf,
				     usbd_dev->control_state.ctrl_len);

		usbd_dev->control_state.state =
			usbd_dev->control_state.needs_zlp ?
			DATA_IN : LAST_DATA_IN;
		usbd_dev->control_state.needs_zlp = false;
		usbd_dev->control_state.ctrl_len = 0;
		usbd_dev->control_state.ctrl_buf = NULL;
	}
}

static int usb_control_recv_chunk(usbd_device *usbd_dev)
{
	uint16_t packetsize = MIN(usbd_dev->desc->bMaxPacketSize0,
			usbd_dev->control_state.req.wLength -
			usbd_dev->control_state.ctrl_len);
	uint16_t size = usbd_ep_read_packet(usbd_dev, 0,
				       usbd_dev->control_state.ctrl_buf +
				       usbd_dev->control_state.ctrl_len,
				       packetsize);

	if (size != packetsize) {
		stall_transaction(usbd_dev);
		return -1;
	}

	usbd_dev->control_state.ctrl_len += size;

	return packetsize;
}

static int usb_control_request_dispatch(usbd_device *usbd_dev,
					struct usb_setup_data *req)
{
	int i, result = 0;
	struct user_control_callback *cb = usbd_dev->user_control_callback;

	/* Call user command hook function. */
	for (i = 0; i < MAX_USER_CONTROL_CALLBACK; i++) {
		if (cb[i].cb == NULL) {
			break;
		}

		if ((req->bmRequestType & cb[i].type_mask) == cb[i].type) {
			result = cb[i].cb(usbd_dev, req,
					  &(usbd_dev->control_state.ctrl_buf),
					  &(usbd_dev->control_state.ctrl_len),
					  &(usbd_dev->control_state.complete));
			if (result == USBD_REQ_HANDLED ||
			    result == USBD_REQ_NOTSUPP) {
				return result;
			}
		}
	}

	/* Try standard request if not already handled. */
	return _usbd_standard_request(usbd_dev, req,
				      &(usbd_dev->control_state.ctrl_buf),
				      &(usbd_dev->control_state.ctrl_len));
}

/* Handle commands and read requests. */
static void usb_control_setup_read(usbd_device *usbd_dev,
		struct usb_setup_data *req)
{
	usbd_dev->control_state.ctrl_buf = usbd_dev->ctrl_buf;
	usbd_dev->control_state.ctrl_len = req->wLength;

	if (usb_control_request_dispatch(usbd_dev, req)) {
		if (req->wLength) {
			usbd_dev->control_state.needs_zlp =
				needs_zlp(usbd_dev->control_state.ctrl_len,
					req->wLength,
					usbd_dev->desc->bMaxPacketSize0);
			/* Go to data out stage if handled. */
			usb_control_send_chunk(usbd_dev);
		} else {
			/* Go to status stage if handled. */
			usbd_ep_write_packet(usbd_dev, 0, NULL, 0);
			usbd_dev->control_state.state = STATUS_IN;
		}
	} else {
		/* Stall endpoint on failure. */
		stall_transaction(usbd_dev);
	}
}

static void usb_control_setup_write(usbd_device *usbd_dev,
				    struct usb_setup_data *req)
{
	if (req->wLength > usbd_dev->ctrl_buf_len) {
		stall_transaction(usbd_dev);
		return;
	}

	/* Buffer into which to write received data. */
	usbd_dev->control_state.ctrl_buf = usbd_dev->ctrl_buf;
	usbd_dev->control_state.ctrl_len = 0;
	/* Wait for DATA OUT stage. */
	if (req->wLength > usbd_dev->desc->bMaxPacketSize0) {
		usbd_dev->control_state.state = DATA_OUT;
	} else {
		usbd_dev->control_state.state = LAST_DATA_OUT;
	}
}

/* Do not appear to belong to the API, so are omitted from docs */
/**@}*/

void _usbd_control_setup(usbd_device *usbd_dev, uint8_t ea)
{
	struct usb_setup_data *req = &usbd_dev->control_state.req;
	(void)ea;

	usbd_dev->control_state.complete = NULL;

	if (usbd_ep_read_packet(usbd_dev, 0, req, 8) != 8) {
		stall_transaction(usbd_dev);
		return;
	}

	if (req->wLength == 0) {
		usb_control_setup_read(usbd_dev, req);
	} else if (req->bmRequestType & 0x80) {
		usb_control_setup_read(usbd_dev, req);
	} else {
		usb_control_setup_write(usbd_dev, req);
	}
}

void _usbd_control_out(usbd_device *usbd_dev, uint8_t ea)
{
	(void)ea;

	switch (usbd_dev->control_state.state) {
	case DATA_OUT:
		if (usb_control_recv_chunk(usbd_dev) < 0) {
			break;
		}
		if ((usbd_dev->control_state.req.wLength -
					usbd_dev->control_state.ctrl_len) <=
					usbd_dev->desc->bMaxPacketSize0) {
			usbd_dev->control_state.state = LAST_DATA_OUT;
		}
		break;
	case LAST_DATA_OUT:
		if (usb_control_recv_chunk(usbd_dev) < 0) {
			break;
		}
		/*
		 * We have now received the full data payload.
		 * Invoke callback to process.
		 */
		if (usb_control_request_dispatch(usbd_dev,
					&(usbd_dev->control_state.req))) {
			/* Got to status stage on success. */
			usbd_ep_write_packet(usbd_dev, 0, NULL, 0);
			usbd_dev->control_state.state = STATUS_IN;
		} else {
			stall_transaction(usbd_dev);
		}
		break;
	case STATUS_OUT:
		usbd_ep_read_packet(usbd_dev, 0, NULL, 0);
		usbd_dev->control_state.state = IDLE;
		if (usbd_dev->control_state.complete) {
			usbd_dev->control_state.complete(usbd_dev,
					&(usbd_dev->control_state.req));
		}
		usbd_dev->control_state.complete = NULL;
		break;
	default:
		stall_transaction(usbd_dev);
	}
}

void _usbd_control_in(usbd_device *usbd_dev, uint8_t ea)
{
	(void)ea;
	struct usb_setup_data *req = &(usbd_dev->control_state.req);

	switch (usbd_dev->control_state.state) {
	case DATA_IN:
		usb_control_send_chunk(usbd_dev);
		break;
	case LAST_DATA_IN:
		usbd_dev->control_state.state = STATUS_OUT;
		break;
	case STATUS_IN:
		if (usbd_dev->control_state.complete) {
			usbd_dev->control_state.complete(usbd_dev,
					&(usbd_dev->control_state.req));
		}

		/* Exception: Handle SET ADDRESS function here... */
		if ((req->bmRequestType == 0) &&
		    (req->bRequest == USB_REQ_SET_ADDRESS)) {
			usbd_dev->driver->set_address(usbd_dev, req->wValue);
		}
		usbd_dev->control_state.state = IDLE;
		break;
	default:
		stall_transaction(usbd_dev);
	}
}

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 * Copyright (C) 2015 Robin Kreis <r.kreis@uni-bremen.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


/* TODO - can't these be inside the impls, not globals from the core? */
uint8_t st_usbfs_force_nak[8];
struct _usbd_device st_usbfs_dev;

void st_usbfs_set_address(usbd_device *dev, uint8_t addr)
{
	(void)dev;
	/* Set device address and enable. */
	SET_REG(USB_DADDR_REG, (addr & USB_DADDR_ADDR) | USB_DADDR_EF);
}

/**
 * Set the receive buffer size for a given USB endpoint.
 *
 * @param ep Index of endpoint to configure.
 * @param size Size in bytes of the RX buffer.
 */
void st_usbfs_set_ep_rx_bufsize(usbd_device *dev, uint8_t ep, uint32_t size)
{
	(void)dev;
	if (size > 62) {
		if (size & 0x1f) {
			size -= 32;
		}
		USB_SET_EP_RX_COUNT(ep, (size << 5) | 0x8000);
	} else {
		if (size & 1) {
			size++;
		}
		USB_SET_EP_RX_COUNT(ep, size << 10);
	}
}

void st_usbfs_ep_setup(usbd_device *dev, uint8_t addr, uint8_t type,
		uint16_t max_size,
		void (*callback) (usbd_device *usbd_dev,
		uint8_t ep))
{
	/* Translate USB standard type codes to STM32. */
	const uint16_t typelookup[] = {
		[USB_ENDPOINT_ATTR_CONTROL] = USB_EP_TYPE_CONTROL,
		[USB_ENDPOINT_ATTR_ISOCHRONOUS] = USB_EP_TYPE_ISO,
		[USB_ENDPOINT_ATTR_BULK] = USB_EP_TYPE_BULK,
		[USB_ENDPOINT_ATTR_INTERRUPT] = USB_EP_TYPE_INTERRUPT,
	};
	uint8_t dir = addr & 0x80;
	addr &= 0x7f;

	/* Assign address. */
	USB_SET_EP_ADDR(addr, addr);
	USB_SET_EP_TYPE(addr, typelookup[type]);

	if (dir || (addr == 0)) {
		USB_SET_EP_TX_ADDR(addr, dev->pm_top);
		if (callback) {
			dev->user_callback_ctr[addr][USB_TRANSACTION_IN] =
			    (void *)callback;
		}
		USB_CLR_EP_TX_DTOG(addr);
		USB_SET_EP_TX_STAT(addr, USB_EP_TX_STAT_NAK);
		dev->pm_top += max_size;
	}

	if (!dir) {
		USB_SET_EP_RX_ADDR(addr, dev->pm_top);
		st_usbfs_set_ep_rx_bufsize(dev, addr, max_size);
		if (callback) {
			dev->user_callback_ctr[addr][USB_TRANSACTION_OUT] =
			    (void *)callback;
		}
		USB_CLR_EP_RX_DTOG(addr);
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
		dev->pm_top += max_size;
	}
}

void st_usbfs_endpoints_reset(usbd_device *dev)
{
	int i;

	/* Reset all endpoints. */
	for (i = 1; i < 8; i++) {
		USB_SET_EP_TX_STAT(i, USB_EP_TX_STAT_DISABLED);
		USB_SET_EP_RX_STAT(i, USB_EP_RX_STAT_DISABLED);
	}
	dev->pm_top = USBD_PM_TOP + (2 * dev->desc->bMaxPacketSize0);
}

void st_usbfs_ep_stall_set(usbd_device *dev, uint8_t addr,
				   uint8_t stall)
{
	(void)dev;
	if (addr == 0) {
		USB_SET_EP_TX_STAT(addr, stall ? USB_EP_TX_STAT_STALL :
				   USB_EP_TX_STAT_NAK);
	}

	if (addr & 0x80) {
		addr &= 0x7F;

		USB_SET_EP_TX_STAT(addr, stall ? USB_EP_TX_STAT_STALL :
				   USB_EP_TX_STAT_NAK);

		/* Reset to DATA0 if clearing stall condition. */
		if (!stall) {
			USB_CLR_EP_TX_DTOG(addr);
		}
	} else {
		/* Reset to DATA0 if clearing stall condition. */
		if (!stall) {
			USB_CLR_EP_RX_DTOG(addr);
		}

		USB_SET_EP_RX_STAT(addr, stall ? USB_EP_RX_STAT_STALL :
				   USB_EP_RX_STAT_VALID);
	}
}

uint8_t st_usbfs_ep_stall_get(usbd_device *dev, uint8_t addr)
{
	(void)dev;
	if (addr & 0x80) {
		if ((*USB_EP_REG(addr & 0x7F) & USB_EP_TX_STAT) ==
		    USB_EP_TX_STAT_STALL) {
			return 1;
		}
	} else {
		if ((*USB_EP_REG(addr) & USB_EP_RX_STAT) ==
		    USB_EP_RX_STAT_STALL) {
			return 1;
		}
	}
	return 0;
}

void st_usbfs_ep_nak_set(usbd_device *dev, uint8_t addr, uint8_t nak)
{
	(void)dev;
	/* It does not make sense to force NAK on IN endpoints. */
	if (addr & 0x80) {
		return;
	}

	st_usbfs_force_nak[addr] = nak;

	if (nak) {
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_NAK);
	} else {
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
	}
}

uint16_t st_usbfs_ep_write_packet(usbd_device *dev, uint8_t addr,
				     const void *buf, uint16_t len)
{
	(void)dev;
	addr &= 0x7F;

	if ((*USB_EP_REG(addr) & USB_EP_TX_STAT) == USB_EP_TX_STAT_VALID) {
		return 0;
	}

	st_usbfs_copy_to_pm(USB_GET_EP_TX_BUFF(addr), buf, len);
	USB_SET_EP_TX_COUNT(addr, len);
	USB_SET_EP_TX_STAT(addr, USB_EP_TX_STAT_VALID);

	return len;
}

uint16_t st_usbfs_ep_read_packet(usbd_device *dev, uint8_t addr,
					 void *buf, uint16_t len)
{
	(void)dev;
	if ((*USB_EP_REG(addr) & USB_EP_RX_STAT) == USB_EP_RX_STAT_VALID) {
		return 0;
	}

	len = MIN(USB_GET_EP_RX_COUNT(addr) & 0x3ff, len);
	st_usbfs_copy_from_pm(buf, USB_GET_EP_RX_BUFF(addr), len);
	USB_CLR_EP_RX_CTR(addr);

	if (!st_usbfs_force_nak[addr]) {
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
	}

	return len;
}

void st_usbfs_poll(usbd_device *dev)
{
	uint16_t istr = *USB_ISTR_REG;

	if (istr & USB_ISTR_RESET) {
		USB_CLR_ISTR_RESET();
		dev->pm_top = USBD_PM_TOP;
		_usbd_reset(dev);
		return;
	}

	if (istr & USB_ISTR_CTR) {
		uint8_t ep = istr & USB_ISTR_EP_ID;
		bool out = (istr & USB_ISTR_DIR) ? true : false;
		uint8_t type;
		uint16_t epreg = *USB_EP_REG(ep);

		/* If DIR is set in USB_ISTR, we must check if CTR_TX is set in
		 * the EP reg and if control_state.state is LAST_DATA_IN we
		 * must deal with the IN transaction first. */
		if ((ep == 0x00) && out && (epreg & USB_EP_TX_CTR) &&
			dev->control_state.state == LAST_DATA_IN) {
			type = USB_TRANSACTION_IN;
		} else if (out) { /* OUT or SETUP transaction */
			type = (epreg & USB_EP_SETUP) ? USB_TRANSACTION_SETUP : USB_TRANSACTION_OUT;
		} else { /* IN transaction */
			type = USB_TRANSACTION_IN;
			USB_CLR_EP_TX_CTR(ep);
		}

		if (dev->user_callback_ctr[ep][type]) {
			dev->user_callback_ctr[ep][type] (dev, ep);
		} else {
			USB_CLR_EP_RX_CTR(ep);
		}
	}

	if (istr & USB_ISTR_SUSP) {
		USB_CLR_ISTR_SUSP();
		if (dev->user_callback_suspend) {
			dev->user_callback_suspend();
		}
	}

	if (istr & USB_ISTR_WKUP) {
		USB_CLR_ISTR_WKUP();
		if (dev->user_callback_resume) {
			dev->user_callback_resume();
		}
	}

	if (istr & USB_ISTR_SOF) {
		USB_CLR_ISTR_SOF();
		if (dev->user_callback_sof) {
			dev->user_callback_sof();
		}
	}

	if (dev->user_callback_sof) {
		*USB_CNTR_REG |= USB_CNTR_SOFM;
	} else {
		*USB_CNTR_REG &= ~USB_CNTR_SOFM;
	}
}
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Frantisek Burian <bufran@seznam.cz>
 * .. file is merged from many other copyrighted files of stm32 family
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
/**@{*/


/*---------------------------------------------------------------------------*/
/** @brief RCC Enable Peripheral Clocks.
 *
 * Enable the clock on particular peripherals. There are three registers
 * involved, each one controlling the enabling of clocks associated with the
 * AHB, APB1 and APB2 respectively. Several peripherals could be enabled
 * simultaneously <em>only if they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Clock Enable Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 *
 * @param[in] en Unsigned int32. Logical OR of all enables to be set
 * @li If register is RCC_AHBER, from @ref rcc_ahbenr_en
 * @li If register is RCC_APB1ENR, from @ref rcc_apb1enr_en
 * @li If register is RCC_APB2ENR, from @ref rcc_apb2enr_en
 */

void rcc_peripheral_enable_clock(volatile uint32_t *reg, uint32_t en)
{
	*reg |= en;
}

/*---------------------------------------------------------------------------*/
/** @brief RCC Disable Peripheral Clocks.
 *
 * Enable the clock on particular peripherals. There are three registers
 * involved, each one controlling the enabling of clocks associated with
 * the AHB, APB1 and APB2 respectively. Several peripherals could be disabled
 * simultaneously <em>only if they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Clock Enable Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 * @param[in] en Unsigned int32. Logical OR of all enables to be used for
 * disabling.
 * @li If register is RCC_AHBER, from @ref rcc_ahbenr_en
 * @li If register is RCC_APB1ENR, from @ref rcc_apb1enr_en
 * @li If register is RCC_APB2ENR, from @ref rcc_apb2enr_en
 */
void rcc_peripheral_disable_clock(volatile uint32_t *reg, uint32_t en)
{
	*reg &= ~en;
}

/*---------------------------------------------------------------------------*/
/** @brief RCC Reset Peripherals.
 *
 * Reset particular peripherals. There are three registers involved, each one
 * controlling reset of peripherals associated with the AHB, APB1 and APB2
 * respectively. Several peripherals could be reset simultaneously <em>only if
 * they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Reset Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 * @param[in] reset Unsigned int32. Logical OR of all resets.
 * @li If register is RCC_AHBRSTR, from @ref rcc_ahbrstr_rst
 * @li If register is RCC_APB1RSTR, from @ref rcc_apb1rstr_rst
 * @li If register is RCC_APB2RSTR, from @ref rcc_apb2rstr_rst
 */
void rcc_peripheral_reset(volatile uint32_t *reg, uint32_t reset)
{
	*reg |= reset;
}

/*---------------------------------------------------------------------------*/
/** @brief RCC Remove Reset on Peripherals.
 *
 * Remove the reset on particular peripherals. There are three registers
 * involved, each one controlling reset of peripherals associated with the AHB,
 * APB1 and APB2 respectively. Several peripherals could have the reset removed
 * simultaneously <em>only if they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Reset Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 * @param[in] clear_reset Unsigned int32. Logical OR of all resets to be
 * removed:
 * @li If register is RCC_AHBRSTR, from @ref rcc_ahbrstr_rst
 * @li If register is RCC_APB1RSTR, from @ref rcc_apb1rstr_rst
 * @li If register is RCC_APB2RSTR, from @ref rcc_apb2rstr_rst
 */
void rcc_peripheral_clear_reset(volatile uint32_t *reg, uint32_t clear_reset)
{
	*reg &= ~clear_reset;
}

#define _RCC_REG(i)		MMIO32(RCC_BASE + ((i) >> 5))
#define _RCC_BIT(i)		(1 << ((i) & 0x1f))

/*---------------------------------------------------------------------------*/
/** @brief Enable Peripheral Clock in running mode.
 *
 * Enable the clock on particular peripheral.
 *
 * @param[in] clken rcc_periph_clken Peripheral RCC
 *
 * For available constants, see #rcc_periph_clken (RCC_UART1 for example)
 */

void rcc_periph_clock_enable(enum rcc_periph_clken clken)
{
	_RCC_REG(clken) |= _RCC_BIT(clken);
}

/*---------------------------------------------------------------------------*/
/** @brief Disable Peripheral Clock in running mode.
 * Disable the clock on particular peripheral.
 *
 * @param[in] clken rcc_periph_clken Peripheral RCC
 *
 * For available constants, see #rcc_periph_clken (RCC_UART1 for example)
 */

void rcc_periph_clock_disable(enum rcc_periph_clken clken)
{
	_RCC_REG(clken) &= ~_RCC_BIT(clken);
}

/*---------------------------------------------------------------------------*/
/** @brief Reset Peripheral, pulsed
 *
 * Reset particular peripheral, and restore to working state.
 *
 * @param[in] rst rcc_periph_rst Peripheral reset
 *
 * For available constants, see #rcc_periph_rst (RST_UART1 for example)
 */

void rcc_periph_reset_pulse(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) |= _RCC_BIT(rst);
	_RCC_REG(rst) &= ~_RCC_BIT(rst);
}

/*---------------------------------------------------------------------------*/
/** @brief Reset Peripheral, hold
 *
 * Reset particular peripheral, and hold in reset state.
 *
 * @param[in] rst rcc_periph_rst Peripheral reset
 *
 * For available constants, see #rcc_periph_rst (RST_UART1 for example)
 */

void rcc_periph_reset_hold(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) |= _RCC_BIT(rst);
}

/*---------------------------------------------------------------------------*/
/** @brief Reset Peripheral, release
 *
 * Restore peripheral from reset state to working state.
 *
 * @param[in] rst rcc_periph_rst Peripheral reset
 *
 * For available constants, see #rcc_periph_rst (RST_UART1 for example)
 */

void rcc_periph_reset_release(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) &= ~_RCC_BIT(rst);
}

/** @brief Select the source of Microcontroller Clock Output
 *
 * Exact sources available depend on your target.  On devices with multiple
 * MCO pins, this function controls MCO1
 *
 * @parame[in] mcosrc the unshifted source bits
 */

void rcc_set_mco(uint32_t mcosrc)
{
	RCC_CFGR = (RCC_CFGR & ~(RCC_CFGR_MCO_MASK << RCC_CFGR_MCO_SHIFT)) |
			(mcosrc << RCC_CFGR_MCO_SHIFT);
}

/**@}*/

#undef _RCC_REG
#undef _RCC_BIT
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Karl Palsson <karlp@ŧweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


uint16_t desig_get_flash_size(void)
{
	return DESIG_FLASH_SIZE;
}

void desig_get_unique_id(uint32_t *result)
{
	*result++ = DESIG_UNIQUE_ID2;
	*result++ = DESIG_UNIQUE_ID1;
	*result = DESIG_UNIQUE_ID0;
}

void desig_get_unique_id_as_string(char *string,
				   unsigned int string_len)
{
	int i, len;
	uint32_t dev_id_buf[3];
	uint8_t *device_id = (uint8_t*)dev_id_buf;
	const char chars[] = "0123456789ABCDEF";

	desig_get_unique_id(dev_id_buf);

	/* Each byte produces two characters */
	len = (2 * sizeof(dev_id_buf) < string_len) ?
		2 * sizeof(dev_id_buf) : string_len - 1;

	for (i = 0; i < len; i += 2) {
		string[i]     = chars[(device_id[i / 2] >> 4) & 0x0F];
		string[i + 1] = chars[(device_id[i / 2] >> 0) & 0x0F];
	}

	string[len] = '\0';
}

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


static usbd_device *st_usbfs_v1_usbd_init(void);

const struct _usbd_driver st_usbfs_v1_usb_driver = {
	.init = st_usbfs_v1_usbd_init,
	.set_address = st_usbfs_set_address,
	.ep_setup = st_usbfs_ep_setup,
	.ep_reset = st_usbfs_endpoints_reset,
	.ep_stall_set = st_usbfs_ep_stall_set,
	.ep_stall_get = st_usbfs_ep_stall_get,
	.ep_nak_set = st_usbfs_ep_nak_set,
	.ep_write_packet = st_usbfs_ep_write_packet,
	.ep_read_packet = st_usbfs_ep_read_packet,
	.poll = st_usbfs_poll,
};

/** Initialize the USB device controller hardware of the STM32. */
static usbd_device *st_usbfs_v1_usbd_init(void)
{
	rcc_periph_clock_enable(RCC_USB);
	SET_REG(USB_CNTR_REG, 0);
	SET_REG(USB_BTABLE_REG, 0);
	SET_REG(USB_ISTR_REG, 0);

	/* Enable RESET, SUSPEND, RESUME and CTR interrupts. */
	SET_REG(USB_CNTR_REG, USB_CNTR_RESETM | USB_CNTR_CTRM |
		USB_CNTR_SUSPM | USB_CNTR_WKUPM);
	return &st_usbfs_dev;
}

void st_usbfs_copy_to_pm(volatile void *vPM, const void *buf, uint16_t len)
{
	const uint16_t *lbuf = buf;
	volatile uint32_t *PM = vPM;
	for (len = (len + 1) >> 1; len; len--) {
		*PM++ = *lbuf++;
	}
}

/**
 * Copy a data buffer from packet memory.
 *
 * @param buf Source pointer to data buffer.
 * @param vPM Destination pointer into packet memory.
 * @param len Number of bytes to copy.
 */
void st_usbfs_copy_from_pm(void *buf, const volatile void *vPM, uint16_t len)
{
	uint16_t *lbuf = buf;
	const volatile uint16_t *PM = vPM;
	uint8_t odd = len & 1;

	for (len >>= 1; len; PM += 2, lbuf++, len--) {
		*lbuf = *PM;
	}

	if (odd) {
		*(uint8_t *) lbuf = *(uint8_t *) PM;
	}
}
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Tomaz Solc <tomaz.solc@tablix.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


void __attribute__((weak)) cm3_assert_failed(void)
{
	while (1);
}

void __attribute__((weak)) cm3_assert_failed_verbose(
		const char *file __attribute__((unused)),
		int line __attribute__((unused)),
		const char *func __attribute__((unused)),
		const char *assert_expr __attribute__((unused)))
{
	cm3_assert_failed();
}

int usbd_register_set_config_callback(usbd_device *usbd_dev,
				       usbd_set_config_callback callback)
{
	int i;

	for (i = 0; i < MAX_USER_SET_CONFIG_CALLBACK; i++) {
		if (usbd_dev->user_callback_set_config[i]) {
			continue;
		}

		usbd_dev->user_callback_set_config[i] = callback;
		return 0;
	}

	return -1;
}
