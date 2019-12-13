#include "webusb.h"
const usbd_driver* target_usb_init(void);
#define debug_println(x)
#define debug_print(x)
#define debug_printhex(x)
#define debug_flush()
#define debug_print_unsigned(x)
#define dbg(x)
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
//  CDC code from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c

#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)
#define USB_CDC_REQ_GET_LINE_CODING		0x21

//  Line config to be returned.
static const struct usb_cdc_line_coding line_coding = {
	.dwDTERate = 115200,
	.bCharFormat = USB_CDC_1_STOP_BITS,
	.bParityType = USB_CDC_NO_PARITY,
	.bDataBits = 0x08
};


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

	usbd_ep_write_packet(usbd_dev, DATA_IN, cdcbuf, pos); ////  Echo the packet.
	
    debug_print("["); debug_println(cdcbuf); debug_print("]"); // debug_flush(); ////
dbg(cdcbuf);

}

static void
cdcacm_comm_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {

dbg("comm2");
	debug_println("comm"); debug_flush();
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
/*
int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback) {

typedef int (*usbd_control_callback)(usbd_device *usbd_dev,
		struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
		usbd_control_complete_callback *complete);

*/

	if (status < 0) { debug_println("*** cdcacm_set_config failed"); debug_flush(); }
}

void cdc_setup(usbd_device* usbd_dev) {
    //  debug_println("*** cdc_setup"); ////
	int status = aggregate_register_config_callback(usbd_dev, cdcacm_set_config);
	if (status < 0) { debug_println("*** cdc_setup failed"); debug_flush(); }
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


#define DESCRIPTOR_CALLBACK_TYPE (USB_REQ_TYPE_IN | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_DEVICE)
#define DESCRIPTOR_CALLBACK_MASK (USB_REQ_TYPE_DIRECTION | USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)


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
#undef CONTROL_CALLBACK_TYPE
#undef CONTROL_CALLBACK_MASK
#undef DESCRIPTOR_CALLBACK_TYPE
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

//  Microsoft Platform Descriptor.  From http://download.microsoft.com/download/3/5/6/3563ED4A-F318-4B66-A181-AB1D8F6FD42D/MS_OS_2_0_desc.docx
const struct microsoft_platform_descriptor microsoft_platform_capability_descriptor = {
	.bLength = MICROSOFT_PLATFORM_DESCRIPTOR_SIZE,
	.bDescriptorType = USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType = USB_DC_PLATFORM,
	.bReserved = 0,
	.platformCapabilityUUID = MSOS20_PLATFORM_UUID,
	.dwWindowsVersion = MSOS20_WINDOWS_VERSION,  //  Windows version e.g. 0x00, 0x00, 0x03, 0x06
	.wMSOSDescriptorSetTotalLength = MSOS20_DESCRIPTOR_SET_SIZE, //  Descriptor set length e.g. 0xb2
	.bMS_VendorCode = WINUSB_MS_VENDOR_CODE,     //  Vendor code e.g. 0x21.  Host will call WinUSB to fetch descriptor.
	.bAltEnumCode = 0  //  Alternate enumeration code e.g. 0x00
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
				dump_usb_request("weburl", req); debug_flush(); ////
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

#undef CONTROL_CALLBACK_TYPE
#undef CONTROL_CALLBACK_MASK
#undef DESCRIPTOR_CALLBACK_MASK
extern int usb_descriptor_type(uint16_t wValue);
extern int usb_descriptor_index(uint16_t wValue);
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


#define CONTROL_CALLBACK_TYPE USB_REQ_TYPE_VENDOR
#define CONTROL_CALLBACK_MASK USB_REQ_TYPE_TYPE
#define DESCRIPTOR_CALLBACK_TYPE USB_REQ_TYPE_DEVICE
#define DESCRIPTOR_CALLBACK_MASK USB_REQ_TYPE_RECIPIENT


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

const usbd_driver* target_usb_init(void)
{
    return &st_usbfs_v1_usb_driver;
}
