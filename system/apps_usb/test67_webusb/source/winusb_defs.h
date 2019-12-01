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

#include <stdint.h>

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
