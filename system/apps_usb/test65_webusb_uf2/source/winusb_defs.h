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

/* Microsoft OS 1.0 descriptors */

/* Extended Compat ID OS Feature Descriptor Specification */
#define WINUSB_REQ_GET_COMPATIBLE_ID_FEATURE_DESCRIPTOR 0x04
#define WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR 0x05

/* Table 2. Function Section */
struct winusb_compatible_id_function_section {
    uint8_t  bInterfaceNumber;
    uint8_t  reserved0[1];
    const char compatibleId[8];
    const char subCompatibleId[8];
    uint8_t  reserved1[6];
} __attribute__((packed));

#define WINUSB_COMPATIBLE_ID_FUNCTION_SECTION_SIZE 24

/* Table 1. Header Section */
struct winusb_compatible_id_descriptor {
    uint32_t dwLength;
    uint16_t bcdVersion;
    uint16_t wIndex;
    uint8_t  bNumSections;
    uint8_t  reserved[7];
    struct winusb_compatible_id_function_section functions[];
} __attribute__((packed));

#define WINUSB_COMPATIBLE_ID_HEADER_SIZE 16

/* Microsoft OS 2.0 Descriptors Specification */

/* Table 8. Microsoft OS 2.0 descriptor wIndex values */
#define WINUSB_REQ_MS_OS_20_DESCRIPTOR_INDEX            0x07
#define WINUSB_REQ_MS_OS_20_SET_ALT_ENUMERATION         0x08

/* Table 9. Microsoft OS 2.0 descriptor wDescriptorType values */
#define WINUSB_DT_MS_OS_20_SET_HEADER_DESCRIPTOR        0x00
#define WINUSB_DT_MS_OS_20_SUBSET_HEADER_CONFIGURATION  0x01
#define WINUSB_DT_MS_OS_20_SUBSET_HEADER_FUNCTION       0x02
#define WINUSB_DT_MS_OS_20_FEATURE_COMPATIBLE_ID        0x03
#define WINUSB_DT_MS_OS_20_FEATURE_REG_PROPERTY         0x04
#define WINUSB_DT_MS_OS_20_FEATURE_MIN_RESUME_TIME      0x05
#define WINUSB_DT_MS_OS_20_FEATURE_MODEL_ID             0x06
#define WINUSB_DT_MS_OS_20_FEATURE_CCGP_DEVICE          0x07

/* Table 3. Microsoft OS 2.0 descriptor platform capability UUID */
#define WINUSB_OS_20_UUID {0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C, 0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F}

/* Table 5. Descriptor set information structure */
struct winusb_descriptor_set_information {
    uint32_t dwWindowsVersion;
    uint16_t wMSOSDescriptorSetTotalLength;
    uint8_t  bMS_VendorCode;
    uint8_t  bAltEnumCode;
} __attribute__((packed));

/* Table 4. Microsoft OS 2.0 platform capability descriptor header */
struct winusb_platform_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDevCapabilityType;
    uint8_t bReserved;
    uint8_t platformCapabilityUUID[16];
    struct winusb_descriptor_set_information descriptor_set_information[];
} __attribute__((packed));

/* Table 10. Microsoft OS 2.0 descriptor set header */
struct winusb_descriptor_set_header {
    uint16_t wLength;
    uint16_t wDescriptorType;
    uint32_t dwWindowsVersion;
    uint16_t wTotalLength;
} __attribute__((packed));

/* Table 11. Configuration subset header */
struct winusb_configuration_subset_header {
    uint16_t wLength;
    uint16_t wDescriptorType;
    uint8_t  bConfigurationValue;
    uint8_t  bReserved;
    uint16_t wTotalLength;
} __attribute__((packed));

/* Table 12. Function subset header */
struct winusb_function_subset_header {
    uint16_t wLength;
    uint16_t wDescriptorType;
    uint8_t  bFirstInterface;
    uint8_t  bReserved;
    uint16_t wSubsetLength;
} __attribute__((packed));

/* Table 13. Microsoft OS 2.0 compatible ID descriptor */
struct winusb_20_compatible_id_feature_descriptor {
    uint16_t wLength;
    uint16_t wDescriptorType;
    const char compatibleId[8];
    const char subCompatibleId[8];;
} __attribute__((packed));

/* Table 15. wPropertyDataType values for the Microsoft OS 2.0 registry property descriptor */
#define WINUSB_PROP_DATA_TYPE_REG_SZ                    1
#define WINUSB_PROP_DATA_TYPE_REG_EXPAND_SZ             2
#define WINUSB_PROP_DATA_TYPE_REG_BINARY                3
#define WINUSB_PROP_DATA_TYPE_REG_DWORD_LITTLE_ENDIAN   4
#define WINUSB_PROP_DATA_TYPE_REG_DWORD_BIG_ENDIAN      5
#define WINUSB_PROP_DATA_TYPE_REG_LINK                  6
#define WINUSB_PROP_DATA_TYPE_REG_REG_MULTI_SZ          7

/* Table 16. Microsoft OS 2.0 minimum USB recovery time descriptor */
struct winusb_minimum_recovery_time_descriptor {
    uint16_t wLength;
    uint16_t wDescriptorType;
    uint8_t  bResumeRecoveryTime;
    uint8_t  bResumeSignalingTime;
} __attribute__((packed));

/* Table 16. Microsoft OS 2.0 model ID descriptor */
struct winusb_model_id_descriptor {
    uint16_t wLength;
    uint16_t wDescriptorType;
    uint8_t  modelId[16];
} __attribute__((packed));

/* Table 17. Microsoft OS 2.0 CCGP device descriptor */
struct winusb_ccgp_device_descriptor {
    uint16_t wLength;
    uint16_t wDescriptorType;
} __attribute__((packed));

#endif
