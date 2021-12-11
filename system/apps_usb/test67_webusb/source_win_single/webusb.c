#include "opencm3.h"
#include "webusb.h"
#include "webstrings.h"
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

#define INTF_COMM               0  //  COMM must be immediately before DATA because of Associated Interface Descriptor.
#define INTF_DATA               1

//  USB Endpoints.
#define MSC_OUT                 0x01
#define DATA_OUT                0x03

#define MSC_IN                  0x82
#define DATA_IN                 0x84
#define COMM_IN                 0x85

extern void usb_set_serial_number(const char* serial);
extern usbd_device* usb_setup(void);
extern void msc_setup(usbd_device* usbd_dev0);
extern uint16_t send_msc_packet(const void *buf, int len);
extern void dump_usb_request(const char *msg, struct usb_setup_data *req);
extern int aggregate_register_config_callback(
    usbd_device *usbd_dev,
	usbd_set_config_callback callback);
extern int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback);

#define assert(cond) if (!(cond)) { AssertionFailedHandler( #cond, __FILE__, __LINE__);}
extern void AssertionFailedHandler(char* cond, char* file, int line);

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

#ifndef WINUSB_DEFS_H_INCLUDED
#define WINUSB_DEFS_H_INCLUDED


//  Microsoft OS 2.0 descriptors.  See http://download.microsoft.com/download/3/5/6/3563ED4A-F318-4B66-A181-AB1D8F6FD42D/MS_OS_2_0_desc.docx

#define MSOS20_PLATFORM_UUID   { 0xdf, 0x60, 0xdd, 0xd8, 0x89, 0x45, 0xc7, 0x4c, 0x9c, 0xd2, 0x65, 0x9d, 0x9e, 0x64, 0x8a, 0x9f }
#define MSOS20_WINDOWS_VERSION { 0x00, 0x00, 0x03, 0x06 }  //  Windows 8.1
#define MSOS20_REQ_GET_DESCRIPTOR 0x07  //  Request for descriptor

#define MSOS20_SET_HEADER_DESCRIPTOR 	   0x00
#define MSOS20_SUBSET_HEADER_CONFIGURATION 0x01
#define MSOS20_SUBSET_HEADER_FUNCTION	0x02
#define MSOS20_FEATURE_COMPATIBLE_ID	0x03
#define MSOS20_FEATURE_REG_PROPERTY		0x04
#define MSOS20_FEATURE_MIN_RESUME_TIME	0x05
#define MSOS20_FEATURE_MODEL_ID			0x06
#define MSOS20_FEATURE_CCGP_DEVICE		0x07
#define MSOS20_FEATURE_VENDOR_REVISION	0x08

#define MSOS20_PROPERTY_NAME_LENGTH     0x2A
#define MSOS20_PROPERTY_DATA_LENGTH     0x50

//  Platform descriptor
struct microsoft_platform_descriptor {
	uint8_t bLength;  //  Should be MICROSOFT_PLATFORM_DESCRIPTOR_SIZE (28)
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint8_t bReserved;
	uint8_t platformCapabilityUUID[16];	
	uint8_t dwWindowsVersion[4];  //  Windows version e.g. 0x00, 0x00, 0x03, 0x06
	uint16_t wMSOSDescriptorSetTotalLength; //  Descriptor set length e.g. 0xb2
	uint8_t bMS_VendorCode; //  Vendor code e.g. 0x20
	uint8_t bAltEnumCode;   //  Alternate enumeration code e.g. 0x00
} __attribute__((packed));
#define MICROSOFT_PLATFORM_DESCRIPTOR_SIZE sizeof(struct microsoft_platform_descriptor)  //  Should be 28

//  Descriptor set header
struct msos20_set_header_descriptor_struct {
	uint16_t wLength;              //  Should be MSOS20_SET_HEADER_DESCRIPTOR_SIZE (10)
	uint16_t wDescriptorType;      //  Should be MSOS20_SET_HEADER_DESCRIPTOR. MS OS 2.0 descriptor set header
	uint8_t  dwWindowsVersion[4];  //  Should be MSOS20_WINDOWS_VERSION. Windows version
	uint16_t wTotalLength;         //  Should be MSOS20_DESCRIPTOR_SET_SIZE (0xB2 or 178). Size of entire MS OS 2.0 descriptor set including this header.
} __attribute__((packed));
#define MSOS20_SET_HEADER_DESCRIPTOR_SIZE sizeof(struct msos20_set_header_descriptor_struct)  //  Should be 10

//  Configuration subset header: Which USB Configuration this descriptor will apply.
struct msos20_subset_header_configuration_struct {
	uint16_t wLength;          //  Should be MSOS20_SUBSET_HEADER_CONFIGURATION_SIZE (8)
	uint16_t wDescriptorType;  //  Should be MSOS20_SUBSET_HEADER_CONFIGURATION
	uint8_t  bConfigurationValue;  //  Configuration ID = 0
	uint8_t  bReserved;
	uint16_t wTotalLength;    //  Should be MSOS20_SUBSET_CONFIGURATION_SIZE (0xA8 or 168). Size of entire configuration subset including this header.
} __attribute__((packed));
#define MSOS20_SUBSET_HEADER_CONFIGURATION_SIZE sizeof(struct msos20_subset_header_configuration_struct)  //  Should be 8

//  Function subset header: Which USB Interface this descriptor will apply.
struct msos20_subset_header_function_struct {
	uint16_t wLength;          //  Should be MSOS20_SUBSET_HEADER_FUNCTION_SIZE (8)
	uint16_t wDescriptorType;  //  Should be MSOS20_SUBSET_HEADER_FUNCTION
	uint8_t  bFirstInterface;  //  Note - this is rewritten in winusb_setup with the correct interface number
	uint8_t  bReserved;
	uint16_t wSubsetLength;    //  Should be MSOS20_SUBSET_FUNCTION_SIZE (0xA0 or 160). Size of entire function subset including this header.
} __attribute__((packed));
#define MSOS20_SUBSET_HEADER_FUNCTION_SIZE sizeof(struct msos20_subset_header_function_struct)  //  Should be 8

//  Compatible ID descriptor
struct msos20_feature_compatible_id_struct {
	uint16_t wLength;          //  Should be MSOS20_FEATURE_COMPATBLE_ID_SIZE (20)
	uint16_t wDescriptorType;  //  Should be MSOS20_FEATURE_COMPATBLE_ID
	uint8_t  CompatibleID[8];  //  Compatible ID
	uint8_t  SubCompatibleID[8]; //  SubCompatible ID
} __attribute__((packed));
#define MSOS20_FEATURE_COMPATIBLE_ID_SIZE sizeof(struct msos20_feature_compatible_id_struct)  //  Should be 20

//  Registry property descriptor: The properties that will be written to Windows registry.
struct msos20_feature_reg_property_struct {
	uint16_t wLength;              //  Should be MSOS20_FEATURE_REG_PROPERTY_SIZE e.g. 0x84
	uint16_t wDescriptorType;      //  Should be MSOS20_FEATURE_REG_PROPERTY
	uint16_t wPropertyDataType;    //  Should be WINUSB_EXTENDED_PROPERTIES_MULTISZ_DATA_TYPE. Type of registry property
	uint16_t wPropertyNameLength;  //  Should be MSOS20_PROPERTY_NAME_LENGTH. Length of property name
	uint8_t  propertyName[MSOS20_PROPERTY_NAME_LENGTH];  // Property name (DeviceInterfaceGUIDs)
	uint16_t wPropertyDataLength;  //  Should be MSOS20_PROPERTY_DATA_LENGTH. Length of property data	
	uint8_t  propertyData[MSOS20_PROPERTY_DATA_LENGTH];  // Property data
} __attribute__((packed));
#define MSOS20_FEATURE_REG_PROPERTY_SIZE sizeof(struct msos20_feature_reg_property_struct)  //  Should be 0x84 (132)

//  Should be 0xA0 (160). Size of entire function subset including header.
#define MSOS20_SUBSET_FUNCTION_SIZE (MSOS20_SUBSET_HEADER_FUNCTION_SIZE + MSOS20_FEATURE_COMPATIBLE_ID_SIZE + MSOS20_FEATURE_REG_PROPERTY_SIZE)

//  Should be 0xA8 (168). Size of entire configuration subset including header.
#define MSOS20_SUBSET_CONFIGURATION_SIZE (MSOS20_SUBSET_FUNCTION_SIZE + MSOS20_SUBSET_HEADER_CONFIGURATION_SIZE)

//  Descriptor set
struct msos20_descriptor_set_struct {
	//  Descriptor set header
	struct msos20_set_header_descriptor_struct       set_header_descriptor;

	//  Configuration subset header: Which USB Configuration this descriptor will apply.
	struct msos20_subset_header_configuration_struct subset_header_configuration;

	//  Function subset header: Which USB Interface this descriptor will apply.
	struct msos20_subset_header_function_struct      subset_header_function;

	//  Compatible ID descriptor
	struct msos20_feature_compatible_id_struct       feature_compatible_id; 

	//  Registry property descriptor: The properties that will be written to Windows registry.
	struct msos20_feature_reg_property_struct        feature_reg_property;
} __attribute__((packed));
#define MSOS20_DESCRIPTOR_SET_SIZE sizeof(struct msos20_descriptor_set_struct)  //  Should be 0xB2 (178)

//  Microsoft OS 1.0 descriptors

/* Extended Compat ID OS Feature Descriptor Specification */
#define WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR 0x04
#define WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR 0x05
#define WINUSB_BCD_VERSION 0x0100

// Apparently using DeviceInterfaceGUID does not always work on Windows 7.
// DeviceInterfaceGUIDs does seem to work.
#define WINUSB_EXTENDED_PROPERTIES_GUID_NAME        u"DeviceInterfaceGUIDs"
#define WINUSB_EXTENDED_PROPERTIES_GUID_NAME_SIZE_C sizeof(WINUSB_EXTENDED_PROPERTIES_GUID_NAME)
#define WINUSB_EXTENDED_PROPERTIES_GUID_NAME_SIZE_U (sizeof(WINUSB_EXTENDED_PROPERTIES_GUID_NAME) / 2)

// extra null is intentional - it's an array of GUIDs with 1 item
#define WINUSB_EXTENDED_PROPERTIES_GUID_DATA        u"{0263b512-88cb-4136-9613-5c8e109d8ef5}\x00"
#define WINUSB_EXTENDED_PROPERTIES_GUID_DATA_SIZE_C sizeof(WINUSB_EXTENDED_PROPERTIES_GUID_DATA)
#define WINUSB_EXTENDED_PROPERTIES_GUID_DATA_SIZE_U (sizeof(WINUSB_EXTENDED_PROPERTIES_GUID_DATA) / 2)
#define WINUSB_EXTENDED_PROPERTIES_MULTISZ_DATA_TYPE  7

#define WINUSB_EXTRA_STRING_INDEX 0xee

/* Table 2. Function Section */
struct winusb_compatible_id_function_section {
	uint8_t  bInterfaceNumber;
	uint8_t  reserved0[1];
	char compatibleId[8];
	char subCompatibleId[8];
	uint8_t  reserved1[6];
} __attribute__((packed));

/* Table 1. Header Section */
struct winusb_compatible_id_descriptor_header {
	uint32_t dwLength;
	uint16_t bcdVersion;
	uint16_t wIndex;
	uint8_t  bNumSections;
	uint8_t  reserved[7];
} __attribute__((packed));

struct winusb_compatible_id_descriptor {
	struct winusb_compatible_id_descriptor_header header;
	struct winusb_compatible_id_function_section functions[];
} __attribute__((packed));

struct winusb_extended_properties_feature_descriptor {
	uint32_t dwLength;
	uint32_t dwPropertyDataType;
	uint16_t wNameLength;
	uint16_t name[WINUSB_EXTENDED_PROPERTIES_GUID_NAME_SIZE_U];
	uint32_t dwPropertyDataLength;
	uint16_t propertyData[WINUSB_EXTENDED_PROPERTIES_GUID_DATA_SIZE_U];
} __attribute__((packed));

struct winusb_extended_properties_descriptor_header {
	uint32_t dwLength;
	uint16_t bcdVersion;
	uint16_t wIndex;
	uint16_t wNumFeatures;
} __attribute__((packed));

struct winusb_extended_properties_descriptor {
	struct winusb_extended_properties_descriptor_header header;
	struct winusb_extended_properties_feature_descriptor features[];
} __attribute__((packed));

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

#ifndef WINUSB_H_INCLUDED
#define WINUSB_H_INCLUDED


// Arbitrary, but must be equivalent to the last character in extra string
#define WINUSB_MS_VENDOR_CODE '!'  //  0x21
#define WINUSB_EXTRA_STRING {'M', 'S', 'F', 'T', '1', '0', '0', WINUSB_MS_VENDOR_CODE}

extern void winusb_setup(usbd_device* usbd_dev, uint8_t interface);

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

#ifndef USB21_STANDARD_H_INCLUDED
#define USB21_STANDARD_H_INCLUDED


/* USB 3.1 Descriptor Types - Table 9-6 */
#define USB_DT_BOS						15
#define USB_DT_DEVICE_CAPABILITY		16

#define USB_DT_BOS_SIZE 5

/* USB Device Capability Types - USB 3.1 Table 9-14 */
#define USB_DC_PLATFORM					5

extern void usb21_setup(usbd_device* usbd_dev, const struct usb_bos_descriptor* binary_object_store);

#endif
#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

typedef void (*cdc_receive_callback)(uint8_t* buf, int len);
extern void cdc_set_receive_callback(cdc_receive_callback callback);
bool cdc_transmit(const uint8_t* buffer, int len);
extern void cdc_setup(usbd_device* usbd_dev);

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



static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue
);


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
    const usbd_driver* driver = &st_usbfs_v1_usb_driver;

    usbd_dev = usbd_init(driver, &dev, &config, 

  &bos_descriptor, // gabo

        usb_strings, num_strings,
        usbd_control_buffer, sizeof(usbd_control_buffer));

    //  The following USB setup functions will call aggregate_register_callback() to register callbacks.
    cdc_setup(usbd_dev);

#ifdef USB21_INTERFACE
    //  Define USB 2.1 BOS interface used by WebUSB.
	usb21_setup(usbd_dev, &bos_descriptor);
	webusb_setup(usbd_dev, origin_url);
	winusb_setup(usbd_dev, /*INTF_DFU*/ 0);  //  Previously INTF_DFU
#endif  //  USB21_INTERFACE

    //  Set the aggregate callback.    
	int status = usbd_register_set_config_callback(usbd_dev, set_aggregate_callback);
    if (status < 0) return NULL;

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
	int i;
	for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
		if (config_callback[i]) {
            if (config_callback[i] == callback) { return 0; }  //  Skip duplicate.
			continue;
		}
		config_callback[i] = callback;
		return 0;
	}
	return -1;
}

int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback) {
	int i;
	for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
		if (control_callback[i].cb) { 
            //  If already exists, skip.
            if (control_callback[i].type == type &&
                control_callback[i].type_mask == type_mask &&
                control_callback[i].cb == callback) { 
                    return 0;
                }
            continue;  //  Continue checking.
        }
		control_callback[i].type = type;
		control_callback[i].type_mask = type_mask;
		control_callback[i].cb = callback;
		return 0;
	}
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
	return USBD_REQ_NEXT_CALLBACK;
}

static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue) {
    //  This callback is called when the device is updated.  We set our control callback.
    if (wValue != (uint16_t) -1) {  //  If this is an actual callback, not a call by usb_setup()...
        //  Call the config functions before setting our callback.
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
	if (status < 0) { assert(0); }  
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
				return USBD_REQ_NOTSUPP;
			}
			if (index == 1) {
				size_t url_len = strlen(webusb_https_url);
				url->bLength = WEBUSB_DT_URL_DESCRIPTOR_SIZE + url_len;
				url->bDescriptorType = WEBUSB_DT_URL;
				url->bScheme = WEBUSB_URL_SCHEME_HTTPS;
				memcpy(&url->URL, webusb_https_url, url_len);
				*len = MIN(*len, url->bLength);
				status = USBD_REQ_HANDLED;
			} else {
				// TODO: stall instead?
				status = USBD_REQ_NOTSUPP;
			}
			break;
		}
		default: {
			status = USBD_REQ_NOTSUPP;
			break;
		}
	}

	return status;
}

static void webusb_set_config(usbd_device* usbd_dev, uint16_t wValue) {
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		webusb_control_vendor_request);
	if (status < 0) { assert(0); }
}

void webusb_setup(usbd_device* usbd_dev, const char* https_url) {
	webusb_https_url = https_url;

	//  Register the callback now because WebUSB requests come earlier.
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		webusb_control_vendor_request);
	if (status < 0) { assert(0); }

    //  Re-register the callback in case the USB restarts.
	status = aggregate_register_config_callback(usbd_dev, webusb_set_config);
	if (status < 0) { assert(0); }
}

#undef MIN
#undef CONTROL_CALLBACK_TYPE
#undef CONTROL_CALLBACK_MASK

#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)
#define USB_CDC_REQ_GET_LINE_CODING		0x21

cdc_receive_callback receive_callback = (cdc_receive_callback)0;
volatile bool cdc_sending = false;
usbd_device* transmit_device = (usbd_device*)0;

bool cdc_waitSync()
{
  for (uint32_t i=0; i<100000; i++) 
    if (!cdc_sending)
      return true;
  return false;
}

bool cdc_transmit(const uint8_t* buffer, int len)
{
  if (!cdc_waitSync())
  {
    // handle unreceived transmission
    return false;
  }

  cdc_sending = true;
  usbd_ep_write_packet(transmit_device, DATA_IN, buffer, len);
  return true;
}


static void cdcacm_data_tx_cb(usbd_device *usbd_dev, uint8_t ep)
{
  cdc_sending = false;
}

void cdc_set_receive_callback(cdc_receive_callback callback)
{
  receive_callback = callback;
}

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
	switch (req->bRequest) {
		case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {
			/* From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
			* This Linux cdc_acm driver requires this to be implemented
			* even though it's optional in the CDC spec, and we don't
			* advertise it in the ACM functional descriptor. */
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_GET_LINE_CODING: {
			//  Windows requires this request, not Mac or Linux.
			//  From https://github.com/PX4/Bootloader/blob/master/stm32/cdcacm.c
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
			        assert(0);
				return USBD_REQ_NOTSUPP;
			}
			*buf = (uint8_t *) &line_coding;
			*len = sizeof(struct usb_cdc_line_coding);
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_SET_LINE_CODING: {
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
			        assert(0);
				return USBD_REQ_NOTSUPP;
			}
			return USBD_REQ_HANDLED;
		}
	}
	return USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
}

static char cdcbuf[MAX_USB_PACKET_SIZE];   // rx buffer

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
	
  if (receive_callback)
    receive_callback((uint8_t*)cdcbuf, pos);
}

static void
cdcacm_comm_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
}

/*
 * USB Configuration:
 */
static void
cdcacm_set_config(
  usbd_device *usbd_dev,
  uint16_t wValue __attribute__((unused))
) {
	usbd_ep_setup(usbd_dev, DATA_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, DATA_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_tx_cb);
	usbd_ep_setup(usbd_dev, COMM_IN, USB_ENDPOINT_ATTR_INTERRUPT, COMM_PACKET_SIZE, cdcacm_comm_cb);
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		cdcacm_control_request);

	if (status < 0) { assert(0); }
}

void cdc_setup(usbd_device* usbd_dev) {
	int status = aggregate_register_config_callback(usbd_dev, cdcacm_set_config);
	if (status < 0) { assert(0); }
	transmit_device = usbd_dev;
}
#undef MIN
#undef CONTROL_CALLBACK_TYPE
#undef CONTROL_CALLBACK_MASK
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

#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })

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
		*buf = (uint8_t*) &msos20_descriptor_set;
		*len = MIN(*len, MSOS20_DESCRIPTOR_SET_SIZE);
		status = USBD_REQ_HANDLED;

		uint8_t *b = (uint8_t*) &msos20_descriptor_set; int i;		

	} else if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_DEVICE) &&
		(req->wIndex == WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR)) {
		//  Request for the MS OS 1.0 Compatible ID feature ("WINUSB"), referenced by the Extended Properties e.g.
		//  >>  type 0xc0, req 0x21, val 0, idx 4, len 16, type 0x00, index 0x00
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
		*buf = (uint8_t*)(&guid);
		*len = MIN(*len, guid.header.dwLength);
		status = USBD_REQ_HANDLED;

	} else {
		status = USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
	}
	return status;
}

static void winusb_set_config(usbd_device* usbd_dev, uint16_t wValue) {
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		winusb_control_vendor_request);
	if (status < 0) {
    	assert(0);
	}
}

void winusb_setup(usbd_device* usbd_dev, uint8_t interface) {
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
	if (status < 0 || status1 < 0 || status2 < 0) { assert(0); }
}

#undef DESCRIPTOR_CALLBACK_TYPE
#undef DESCRIPTOR_CALLBACK_MASK
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
		return USBD_REQ_NOTSUPP;
	}
	if (req->bRequest == USB_REQ_GET_DESCRIPTOR) {
		*len = MIN(*len, build_bos_descriptor(usb21_bos, *buf, *len));
		return USBD_REQ_HANDLED;
	}
	return USBD_REQ_NEXT_CALLBACK;
}

static void usb21_set_config(usbd_device* usbd_dev, uint16_t wValue) {
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		DESCRIPTOR_CALLBACK_TYPE,
		DESCRIPTOR_CALLBACK_MASK,
		&usb21_standard_get_descriptor);
	if (status < 0) { assert(0); }
}

void usb21_setup(usbd_device* usbd_dev, const struct usb_bos_descriptor* binary_object_store) {
	usb21_bos = binary_object_store;

	/* Register the control request handler _before_ the config is set */
	usb21_set_config(usbd_dev, 0x0000);
	int status = aggregate_register_config_callback(usbd_dev, usb21_set_config);
	if (status < 0) { assert(0); }
}

