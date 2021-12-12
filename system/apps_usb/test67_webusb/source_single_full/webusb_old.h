#include "opencm3.h"
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
extern void dump_usb_request(const char *msg, struct usb_setup_data *req);
extern int aggregate_register_config_callback(
    usbd_device *usbd_dev,
	usbd_set_config_callback callback);
extern int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback);

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
#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

extern void cdc_setup(usbd_device* usbd_dev);

#endif  //  CDC_H_INCLUDED
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

#if 0
struct usb_device_capability_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
} __attribute__((packed));

struct usb_bos_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumDeviceCaps;
	/* Descriptor ends here.  The following are used internally: */
	const struct usb_device_capability_descriptor **capabilities;
} __attribute__((packed));
#endif

#define USB_DT_BOS_SIZE 5

/* USB Device Capability Types - USB 3.1 Table 9-14 */
#define USB_DC_PLATFORM					5

extern void usb21_setup(usbd_device* usbd_dev, const struct usb_bos_descriptor* binary_object_store);

#endif
