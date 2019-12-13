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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <logger.h>
#include "target.h"
#include "cdc.h"
#include "webusb.h"
#include "winusb.h"
#include "usb21_standard.h"
#include "usb_conf.h"

static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue
);

#ifdef USB21_INTERFACE
static const char* origin_url = "visualbluepill.github.io";
#endif  //  USB21_INTERFACE

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
#ifdef USB21_INTERFACE
    .bcdUSB = 0x0210,  //  USB Version 2.1.  Need to handle special requests e.g. BOS.
#else
    .bcdUSB = 0x0200,  //  USB Version 2.0.  No need to handle special requests e.g. BOS.
#endif  //  USB21_INTERFACE

#ifdef SERIAL_USB_INTERFACE  //  If we are providing serial interface only...
	.bDeviceClass = USB_CLASS_CDC,  //  Set the class to CDC if it's only serial.  Serial interface will not start on Windows when class = 0.
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
#else  //  If we are providing multiple interfaces...
    .bDeviceClass = USB_CLASS_MISCELLANEOUS,  //  Copied from microbit. For composite device, let host probe the interfaces.
    .bDeviceSubClass = 2,  //  Common Class
    .bDeviceProtocol = 1,  //  Interface Association Descriptor
#endif  //  SERIAL_USB_INTERFACE
    .bMaxPacketSize0 = MAX_USB_PACKET_SIZE,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = 0x0220,  //  Device Release number 2.2
    .iManufacturer = USB_STRINGS_MANUFACTURER,
    .iProduct = USB_STRINGS_PRODUCT,
    .iSerialNumber = USB_STRINGS_SERIAL_NUMBER,
    .bNumConfigurations = 1,
};

#ifdef INTF_MSC
//  MSC Endpoints
static const struct usb_endpoint_descriptor msc_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = MSC_OUT,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = MAX_USB_PACKET_SIZE,
	.bInterval = 0,
}, {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = MSC_IN,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = MAX_USB_PACKET_SIZE,
	.bInterval = 0,
}};
#endif  //  INTF_MSC

#ifdef INTF_COMM
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
#endif  //  INTF_MSC

#ifdef INTF_DFU
//  DFU Interface
static const struct usb_interface_descriptor dfu_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = INTF_DFU,
    .bAlternateSetting = 0,
    .bNumEndpoints = 0,
    .bInterfaceClass = 0xFE,
    .bInterfaceSubClass = 1,
    .bInterfaceProtocol = 2,
    .iInterface = USB_STRINGS_DFU,  //  Name of DFU
    .endpoint = NULL,
    .extra = &dfu_function,
    .extralen = sizeof(dfu_function),
};
#endif  //  INTF_DFU

#ifdef INTF_MSC
//  MSC Interface
static const struct usb_interface_descriptor msc_iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = INTF_MSC,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_MSC,
	.bInterfaceSubClass = USB_MSC_SUBCLASS_SCSI,
	.bInterfaceProtocol = USB_MSC_PROTOCOL_BBB,
    .iInterface = USB_STRINGS_MSC,  //  Name of MSC
	.endpoint = msc_endp,  //  MSC Endpoints
	.extra = NULL,
	.extralen = 0
};
#endif  //  INTF_MSC

#ifdef INTF_COMM
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
#endif  //  INTF_COMM

//  All USB Interfaces
static const struct usb_interface interfaces[] = {
#ifdef INTF_DFU    
    {
        .num_altsetting = 1,
        .altsetting = &dfu_iface,  //  Index must sync with INTF_DFU.
    }, 
#endif  //  INTF_DFU
#ifdef INTF_MSC    
    {
        .num_altsetting = 1,
        .altsetting = &msc_iface,  //  Index must sync with INTF_MSC.
    }, 	
#endif  //  INTF_MSC
#ifdef INTF_COMM
    {
        .num_altsetting = 1,
#ifndef SERIAL_USB_INTERFACE
	    .iface_assoc = &cdc_iface_assoc,  //  Mandatory for composite device with multiple interfaces.
#endif  //  SERIAL_USB_INTERFACE
        .altsetting = &comm_iface,  //  Index must sync with INTF_COMM.
    }, 
    {
        .num_altsetting = 1,
        .altsetting = &data_iface,  //  Index must sync with INTF_DATA.
    },
#endif  //  INTF_COMM
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

#ifdef USB21_INTERFACE
//  BOS Capabilities for WebUSB and Microsoft Platform
static const struct usb_device_capability_descriptor* capabilities[] = {
	(const struct usb_device_capability_descriptor*) 
        &webusb_platform_capability_descriptor,
	(const struct usb_device_capability_descriptor*) 
        &microsoft_platform_capability_descriptor,
};

//  BOS Descriptor for WebUSB and Microsoft Platform
static const struct usb_bos_descriptor bos_descriptor = {
	.bLength = USB_DT_BOS_SIZE,
	.bDescriptorType = USB_DT_BOS,
	.bNumDeviceCaps = sizeof(capabilities) / sizeof(capabilities[0]),
	.capabilities = capabilities
};
#endif  //  USB21_INTERFACE

/* Buffer to be used for control requests. */
static uint8_t usbd_control_buffer[USB_CONTROL_BUF_SIZE] __attribute__ ((aligned (2)));
usbd_device* usbd_dev = NULL;

usbd_device* usb_setup(void) {
    int num_strings = sizeof(usb_strings) / sizeof(const char*);
    // debug_print("usb_setup num_strings "); debug_print_int(num_strings); debug_println(""); // debug_flush(); ////
    const usbd_driver* driver = target_usb_init();
/*
extern usbd_device * usbd_init(const usbd_driver *driver,
			       const struct usb_device_descriptor *dev,
			       const struct usb_config_descriptor *conf,
                   const struct usb_bos_descriptor *bos,
			       const char **strings, int num_strings,
			       uint8_t *control_buffer,
			       uint16_t control_buffer_size);
*/


    usbd_dev = usbd_init(driver, &dev, &config, 

  &bos_descriptor, // gabo

        usb_strings, num_strings,
        usbd_control_buffer, sizeof(usbd_control_buffer));

    //  The following USB setup functions will call aggregate_register_callback() to register callbacks.
#ifdef INTF_DFU    
    dfu_setup(usbd_dev, &target_manifest_app, NULL, NULL);
#endif  //  INTF_DFU
#ifdef INTF_MSC    
    msc_setup(usbd_dev);
#endif  //  INTF_MSC
#ifdef INTF_COMM    
    cdc_setup(usbd_dev);
#endif  //  INTF_COMM

#ifdef USB21_INTERFACE
    //  Define USB 2.1 BOS interface used by WebUSB.
	usb21_setup(usbd_dev, &bos_descriptor);
	webusb_setup(usbd_dev, origin_url);
//GABO: pozor, nemame DFU!!!
#warning nemame dfu
#define INTF_DFU 0
	winusb_setup(usbd_dev, INTF_DFU);  //  Previously INTF_DFU
#endif  //  USB21_INTERFACE

    //  Set the aggregate callback.    
	int status = usbd_register_set_config_callback(usbd_dev, set_aggregate_callback);
    if (status < 0) { debug_println("*** usb_setup failed"); debug_flush(); }

    //  For WinUSB: Windows probes the compatible ID before setting the configuration, so also register the callback now.
    set_aggregate_callback(usbd_dev, (uint16_t) -1);
    return usbd_dev;
}

#ifdef INTF_MSC    
extern usbd_mass_storage *custom_usb_msc_init(usbd_device *usbd_dev,
				 uint8_t ep_in, uint8_t ep_in_size,
				 uint8_t ep_out, uint8_t ep_out_size,
				 const char *vendor_id,
				 const char *product_id,
				 const char *product_revision_level,
				 const uint32_t block_count,
				 int (*read_block)(uint32_t lba, uint8_t *copy_to),
				 int (*write_block)(uint32_t lba, const uint8_t *copy_from),
				 uint8_t msc_interface_index0);

void msc_setup(usbd_device* usbd_dev0) {
    //  debug_println("msc_setup"); ////
#ifdef RAM_DISK
    ramdisk_init();
#endif  //  RAM_DISK
    
    custom_usb_msc_init(usbd_dev0, MSC_IN, MAX_USB_PACKET_SIZE, MSC_OUT, MAX_USB_PACKET_SIZE, 
        MSC_VENDOR_ID, MSC_PRODUCT_ID, MSC_PRODUCT_REVISION_LEVEL, 
#ifdef RAM_DISK    
        ramdisk_blocks(), ramdisk_read, ramdisk_write,
#else
        UF2_NUM_BLOCKS, read_block, write_block,
#endif  //  RAM_DISK        
        INTF_MSC
    );
}
#endif  //  INTF_MSC

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
	    dump_usb_request(">> ", req); debug_flush(); ////
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

void dump_usb_request(const char *msg, struct usb_setup_data *req) {
/*
    uint8_t desc_type = usb_descriptor_type(req->wValue);
    uint8_t desc_index = usb_descriptor_index(req->wValue);
    debug_print(msg);
    debug_print(" typ "); debug_printhex(req->bmRequestType);
    debug_print(", req "); debug_printhex(req->bRequest);
    debug_print(", val "); debug_printhex(req->wValue >> 8); debug_printhex(req->wValue & 0xff);
    debug_print(", idx "); debug_printhex(req->wIndex >> 8); debug_printhex(req->wIndex & 0xff);
    debug_print(", len "); debug_printhex(req->wLength >> 8); debug_printhex(req->wLength & 0xff);

    if (req->bmRequestType == 0x00 || req->bmRequestType == 0x80) {
        //  Dump USB standard requests.
        if (req->bmRequestType == 0x80 && req->bRequest == 0x06) {
            debug_print(", GET_DES");
            switch(desc_type) {
                case 1: debug_print("_DEV"); break;
                case 2: debug_print("_CFG"); break;
                case 3: debug_print("_STR"); break;
                case 4: debug_print("_INF"); break;
                case 5: debug_print("_ENP"); break;
                case 15: debug_print("_BOS"); break;
            }
        } else if (req->bmRequestType == 0x00 && req->bRequest == 0x05) {
            //  Note: We should see SET_ADDRESS only once per session.  If we see this again, it means
            //  we have previously returned invalid data to the host and the host is attempting to reset our connection.
            debug_print(", SET_ADR    ");
        } else if (req->bmRequestType == 0x00 && req->bRequest == 0x09) {
            debug_print(", SET_CFG    ");
        } else if (req->bmRequestType == 0x80 && req->bRequest == 0x09) {
            debug_print(", SET_CFG    ");
        } else {
            debug_print(",");
        }
        debug_print(" t "); debug_printhex(desc_type); 	
        debug_print(" i "); debug_printhex(desc_index); 	
    }
    debug_println("");
*/
}

/* CDC, MSC and DFU OK.  WebUSB failed.
> Executing task in folder bluepill-bootloader: c:\openocd\bin\openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -f scripts/connect.ocd <

GNU MCU Eclipse 64-bits Open On-Chip Debugger 0.10.0+dev-00487-gaf359c18 (2018-05-12-19:30)
Licensed under GNU GPL v2
For bug reports, read
                                                                                                            http://openocd.org/doc/doxygen/bugs.html
WARNING: interface/stlink-v2.cfg is deprecated, please switch to interface/stlink.cfg
Info : auto-selecting first available session transport "hla_swd". To override use 'transport select <transport>'.
Info : The selected transport took over low-level target control. The results might differ compared to plain
JTAG/SWD
adapter speed: 1000 kHz
adapter_nsrst_delay: 100
none separate
debug_level: 0
semihosting is enabled
NOTE: Trash this window before uploading a program to the Blue Pill
Restarting the Blue Pill...
----platform_setup
----bootloader
target_get_force_bootloader
target_get_serial_number
usb_set_serial_number
usb_setup
usbd polling...
>>  typ 00, req 05, val 0005, idx 0000, len 0000, SET_ADR     t 00 i 05
bos typ 80, req 06, val 0f00, idx 0000, len 0005, GET_DES_BOS t 0f i 00
5 / 05 0f 39 00 02
bos typ 80, req 06, val 0f00, idx 0000, len 0039, GET_DES_BOS t 0f i 00
57 / 05 0f 39 00 02 18 10 05 00 38 b6 08 34 a9 09 a0 47 8b fd a0 76 88 15 b6 65 00 01 22 01 1c 10 05 00 df 60 dd d8 89 45 c7 4c 9c d2 65 9d 9e 64 8a 9f 00 00 03 06 b2 00 21 00
windes typ c0, req 21, val 0000, idx 0007, len 00b2
178 / 0a 00 00 00 00 00 03 06 b2 00 08 00 01 00 00 00 a8 00 08 00 02 00 00 00 a0 00 14 00 03 00 57 49 4e 55 53 42 00 00 00 00 00 00 00 00 00 00 84 00 04 00 07 00 2a 00 44 00 65 00 76 00 69 00 63 00 65 00 49 00 6e 00 74 00 65 00 72 00 66 00 61 00 63 00 65 00 47 00 55 00 49 00 44 00 73 00 00 00 50 00 7b 00 39 00 44 00 33 00 32
00 46 00 38 00 32 00 43 00 2d 00 31 00 46 00 42 00 32 00 2d 00 34 00 34 00 38 00 36 00 2d 00 38 00 35 00 30 00 31 00 2d 00 42 00 36 00 31 00 34 00 35 00 42 00 35 00 42 00 41 00 33 00 33 00 36 00 7d 00 00 00 00 00
>>  typ 00, req 09, val 0001, idx 0000, len 0000, SET_CFG     t 00 i 01
set_aggregate_callback
>>  typ 80, req 00, val 0000, idx 0000, len 0002, t 00 i 00
bos typ 80, req 06, val 0f00, idx 0000, len 0005, GET_DES_BOS t 0f i 00
5 / 05 0f 39 00 02
bos typ 80, req 06, val 0f00, idx 0000, len 0039, GET_DES_BOS t 0f i 00
57 / 05 0f 39 00 02 18 10 05 00 38 b6 08 34 a9 09 a0 47 8b fd a0 76 88 15 b6 65 00 01 22 01 1c 10 05 00 df 60 dd d8 89 45 c7 4c 9c d2 65 9d 9e 64 8a 9f 00 00 03 06 b2 00 21 00
weburl typ c0, req 22, val 0001, idx 0002, len 00ff


> Executing task in folder bluepill-bootloader: c:\openocd\bin\openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -f scripts/connect.ocd <

GNU MCU Eclipse 64-bits Open On-Chip Debugger 0.10.0+dev-00487-gaf359c18 (2018-05-12-19:30)
Licensed under GNU GPL v2
For bug reports, read
                                                                                                        http://openocd.org/doc/doxygen/bugs.html
WARNING: interface/stlink-v2.cfg is deprecated, please switch to interface/stlink.cfg
Info : auto-selecting first available session transport "hla_swd". To override use 'transport select <transport>'.
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
adapter speed: 1000 kHz
adapter_nsrst_delay: 100
none separate
debug_level: 0
semihosting is enabled
NOTE: Trash this window before uploading a program to the Blue Pill
Restarting the Blue Pill...
----platform_setup
----bootloader
target_get_force_bootloader
target_get_serial_number
usb_set_serial_number
usb_setup
usbd polling...
>>  typ 80, req 06, val 0100, idx 0000, len 0040, GET_DES_DEV t 01 i 00
>>  typ 00, req 05, val 0005, idx 0000, len 0000, SET_ADR     t 00 i 05
>>  typ 80, req 06, val 0100, idx 0000, len 0012, GET_DES_DEV t 01 i 00
>>  typ 80, req 06, val 0200, idx 0000, len 00ff, GET_DES_CFG t 02 i 00
u21 typ 80, req 06, val 0f00, idx 0000, len 0005, GET_DES_BOS t 0f i 00
u21 typ 80, req 06, val 0f00, idx 0000, len 001d, GET_DES_BOS t 0f i 00
>>  typ 80, req 06, val 0303, idx 0409, len 00ff, GET_DES_STR t 03 i 03
>>  typ 80, req 06, val 0300, idx 0000, len 00ff, GET_DES_STR t 03 i 00
>>  typ 80, req 06, val 0302, idx 0409, len 00ff, GET_DES_STR t 03 i 02
>>  typ 80, req 06, val 0100, idx 0000, len 0012, GET_DES_DEV t 01 i 00
>>  typ 80, req 06, val 0200, idx 0000, len 0009, GET_DES_CFG t 02 i 00
>>  typ 80, req 06, val 0200, idx 0000, len 0074, GET_DES_CFG t 02 i 00
>>  typ 00, req 09, val 0001, idx 0000, len 0000, SET_CFG     t 00 i 01
set_aggregate_callback
dfu_set_config
webusb_set_config
winctl typ c0, req 21, val 0000, idx 0004, len 0010
winctl typ c0, req 21, val 0000, idx 0004, len 0028
>>  typ 80, req 06, val 0305, idx 0409, len 0004, GET_DES_STR t 03 i 05
>>  typ 80, req 06, val 0305, idx 0409, len 001c, GET_DES_STR t 03 i 05
>>  typ c1, req 21, val 0000, idx 0005, len 000a
>>  typ 80, req 06, val 0306, idx 0409, len 0004, GET_DES_STR t 03 i 06
>>  typ 80, req 06, val 0306, idx 0409, len 002c, GET_DES_STR t 03 i 06
>>  typ c1, req 21, val 0001, idx 0005, len 000a
>>  typ 80, req 06, val 0304, idx 0409, len 0004, GET_DES_STR t 03 i 04
>>  typ 80, req 06, val 0304, idx 0409, len 0018, GET_DES_STR t 03 i 04
winctl typ c1, req 21, val 0003, idx 0005, len 000a
winctl typ c1, req 21, val 0003, idx 0005, len 0092
>>  typ 80, req 06, val 0300, idx 0000, len 0002, GET_DES_STR t 03 i 00
>>  typ 80, req 06, val 0300, idx 0000, len 0004, GET_DES_STR t 03 i 00
>>  typ 80, req 06, val 0303, idx 0409, len 0002, GET_DES_STR t 03 i 03
>>  typ 80, req 06, val 0303, idx 0409, len 0032, GET_DES_STR t 03 i 03
>>  typ 80, req 06, val 0100, idx 0000, len 0040, GET_DES_DEV t 01 i 00
>>  typ 00, req 05, val 0006, idx 0000, len 0000, SET_ADR     t 00 i 06
>>  typ 80, req 06, val 0100, idx 0000, len 0012, GET_DES_DEV t 01 i 00
>>  typ 00, req 09, val 0001, idx 0000, len 0000, SET_CFG     t 00 i 01
set_aggregate_callback
dfu_set_config
webusb_set_config
>>  typ 80, req 06, val 0100, idx 0000, len 0040, GET_DES_DEV t 01 i 00
>>  typ 00, req 05, val 0005, idx 0000, len 0000, SET_ADR     t 00 i 05
>>  typ 80, req 06, val 0100, idx 0000, len 0012, GET_DES_DEV t 01 i 00
>>  typ 00, req 09, val 0001, idx 0000, len 0000, SET_CFG     t 00 i 01
set_aggregate_callback
dfu_set_config
webusb_set_config
>>  typ 80, req 06, val 0100, idx 0000, len 0040, GET_DES_DEV t 01 i 00
>>  typ 00, req 05, val 0006, idx 0000, len 0000, SET_ADR     t 00 i 06
>>  typ 80, req 06, val 0100, idx 0000, len 0012, GET_DES_DEV t 01 i 00
>>  typ 00, req 09, val 0001, idx 0000, len 0000, SET_CFG     t 00 i 01
set_aggregate_callback
dfu_set_config
webusb_set_config
>>  typ 80, req 06, val 0306, idx 0409, len 0004, GET_DES_STR t 03 i 06
>>  typ 80, req 06, val 0306, idx 0409, len 002c, GET_DES_STR t 03 i 06
[a
][s
][d
][f
][
][s
][a
][s
][a
][s
][
][f
][d
][f
][f
][
][d
][s
][d
][s
]
*/
