#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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

#ifndef LIBOPENCM3_CM3_COMMON_H
#define LIBOPENCM3_CM3_COMMON_H


/* This must be placed around external function declaration for C++
 * support. */
#ifdef __cplusplus
# define BEGIN_DECLS extern "C" {
# define END_DECLS }
#else
# define BEGIN_DECLS
# define END_DECLS
#endif

/* Full-featured deprecation attribute with fallback for older compilers. */

#ifdef __GNUC__
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 4)
#		define LIBOPENCM3_DEPRECATED(x) __attribute__((deprecated(x)))
#	else
#		define LIBOPENCM3_DEPRECATED(x) __attribute__((deprecated))
#	endif
#else
#	define LIBOPENCM3_DEPRECATED(x)
#endif


/* Generic memory-mapped I/O accessor functions */
#define MMIO8(addr)		(*(volatile uint8_t *)(addr))
#define MMIO16(addr)		(*(volatile uint16_t *)(addr))
#define MMIO32(addr)		(*(volatile uint32_t *)(addr))
#define MMIO64(addr)		(*(volatile uint64_t *)(addr))

/* Generic bit-band I/O accessor functions */
#define BBIO_SRAM(addr, bit) \
	MMIO32((((uint32_t)addr) & 0x0FFFFF) * 32 + 0x22000000 + (bit) * 4)

#define BBIO_PERIPH(addr, bit) \
	MMIO32((((uint32_t)addr) & 0x0FFFFF) * 32 + 0x42000000 + (bit) * 4)

/* Generic bit definition */
#define BIT0  (1<<0)
#define BIT1  (1<<1)
#define BIT2  (1<<2)
#define BIT3  (1<<3)
#define BIT4  (1<<4)
#define BIT5  (1<<5)
#define BIT6  (1<<6)
#define BIT7  (1<<7)
#define BIT8  (1<<8)
#define BIT9  (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)
#define BIT16 (1<<16)
#define BIT17 (1<<17)
#define BIT18 (1<<18)
#define BIT19 (1<<19)
#define BIT20 (1<<20)
#define BIT21 (1<<21)
#define BIT22 (1<<22)
#define BIT23 (1<<23)
#define BIT24 (1<<24)
#define BIT25 (1<<25)
#define BIT26 (1<<26)
#define BIT27 (1<<27)
#define BIT28 (1<<28)
#define BIT29 (1<<29)
#define BIT30 (1<<30)
#define BIT31 (1<<31)

#endif
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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

#ifndef LIBOPENCM3_CM3_MEMORYMAP_H
#define LIBOPENCM3_CM3_MEMORYMAP_H

/* --- ARM Cortex-M0, M3 and M4 specific definitions ----------------------- */

/* Private peripheral bus - Internal */
#define PPBI_BASE                       (0xE0000000U)

/* Those defined only on ARMv7 and above */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
/* ITM: Instrumentation Trace Macrocell */
#define ITM_BASE                        (PPBI_BASE + 0x0000)

/* DWT: Data Watchpoint and Trace unit */
#define DWT_BASE                        (PPBI_BASE + 0x1000)

/* FPB: Flash Patch and Breakpoint unit */
#define FPB_BASE                        (PPBI_BASE + 0x2000)
#endif

/* PPBI_BASE + 0x3000 (0xE000 3000 - 0xE000 DFFF): Reserved */

#define SCS_BASE                        (PPBI_BASE + 0xE000)

/* PPBI_BASE + 0xF000 (0xE000 F000 - 0xE003 FFFF): Reserved */

/* Those defined only on ARMv7 and above */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#define TPIU_BASE                       (PPBI_BASE + 0x40000)
#endif

/* --- SCS: System Control Space --- */

/* Those defined only on ARMv7 and above */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
/* ITR: Interrupt Type Register */
#define ITR_BASE                        (SCS_BASE + 0x0000)
#endif

/* SYS_TICK: System Timer */
#define SYS_TICK_BASE                   (SCS_BASE + 0x0010)

/* NVIC: Nested Vector Interrupt Controller */
#define NVIC_BASE                       (SCS_BASE + 0x0100)

/* SCB: System Control Block */
#define SCB_BASE                        (SCS_BASE + 0x0D00)

/* MPU: Memory protection unit */
#define MPU_BASE                        (SCS_BASE + 0x0D90)

/* Those defined only on CM0*/
#if defined(__ARM_ARCH_6M__)
/* DEBUG: Debug control and configuration */
#define DEBUG_BASE                      (SCS_BASE + 0x0DF0)
#endif

/* Those defined only on ARMv7 and above */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
/* STE: Software Trigger Interrupt Register */
#define STIR_BASE                       (SCS_BASE + 0x0F00)
/* ID: ID space */
#define ID_BASE                         (SCS_BASE + 0x0FD0)
#endif

#endif
/** @defgroup debugging Debugging

@brief Macros and functions to aid in debugging

@version 1.0.0

@date 25 September 2012

Two preprocessor defines control the behavior of assertion check macros in
this module. They allow the choice between generated code size and ease of
debugging.

If NDEBUG is defined, all assertion checks are disabled and macros do not
generate any code.

If CM3_ASSERT_VERBOSE is defined, information regarding the position of
assertion checks will be stored in the binary, allowing for more
informative error messages, but also significantly increased code size. As
default assertion checks do not use this information it is only useful if
the application linked with libopencm3 defines its own
cm3_assert_failed_verbose() implementation.

LGPL License Terms @ref lgpl_license
*/

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

/**@{*/

#ifndef LIBOPENCM3_CM3_ASSERT_H
#define LIBOPENCM3_CM3_ASSERT_H


#define CM3_LIKELY(expr) (__builtin_expect(!!(expr), 1))

#ifdef NDEBUG
# define cm3_assert(expr) (void)0
# define cm3_assert_not_reached() do { } while (1)
#else
# ifdef CM3_ASSERT_VERBOSE
#  define cm3_assert(expr) do { \
				if (CM3_LIKELY(expr)) { \
					(void)0; \
				} else { \
					cm3_assert_failed_verbose( \
						__FILE__, __LINE__, \
						__func__, #expr); \
				} \
			   } while (0)
#  define cm3_assert_not_reached() \
	cm3_assert_failed_verbose( \
			__FILE__, __LINE__, \
			__func__, 0)
# else
/** @brief Check if assertion is true.
 *
 * If NDEBUG macro is defined, this macro generates no code. Otherwise
 * cm3_assert_failed() or cm3_assert_failed_verbose() is called if assertion
 * is false.
 *
 * The purpose of this macro is to aid in debugging libopencm3 and
 * applications using it. It can be used for example to check if function
 * arguments are within expected ranges and stop execution in case an
 * unexpected state is reached.
 *
 * @param expr expression to check */
#  define cm3_assert(expr) do { \
				if (CM3_LIKELY(expr)) { \
					(void)0; \
				} else { \
					cm3_assert_failed(); \
				} \
			} while (0)
/** @brief Check if unreachable code is reached.
 *
 * If NDEBUG macro is defined, this macro generates code for an infinite loop.
 * Otherwise cm3_assert_failed() or cm3_assert_failed_verbose() is called if
 * the macro is ever reached.
 *
 * The purpose of this macro is to aid in debugging libopencm3 and
 * applications using it. It can be used for example to stop execution if an
 * unreachable portion of code is reached. */
#  define cm3_assert_not_reached() cm3_assert_failed()
# endif
#endif

BEGIN_DECLS

/** @brief Called on a failed assertion.
 *
 * Halts execution in an infinite loop. This function never returns.
 *
 * Defined as a weak symbol, so applications can define their own
 * implementation. Usually, a custom implementation of this function should
 * report an error in some way (print a message to a debug console, display,
 * LED, ...) and halt execution or reboot the device. */
void cm3_assert_failed(void) __attribute__((__noreturn__));

/** @brief Called on a failed assertion with verbose messages enabled.
 *
 * Halts execution in an infinite loop. This function never returns.
 *
 * Defined as a weak symbol, so applications can define their own
 * implementation. Usually, a custom implementation of this function should
 * report an error in some way (print a message to a debug console, display,
 * LED, ...) and halt execution or reboot the device.
 *
 * @param file File name where the failed assertion occurred
 * @param line Line number where the failed assertion occurred
 * @param func Name of the function where the failed assertion occurred
 * @param assert_expr Expression that evaluated to false (can be NULL) */
void cm3_assert_failed_verbose(const char *file, int line, const char *func,
		const char *assert_expr) __attribute__((__noreturn__));

END_DECLS

#endif

/**@}*/
/** @defgroup usb_type_defines USB Standard Structure Definitions

@brief <b>Defined Constants and Types for the USB Standard Structure
Definitions</b>

@ingroup USB_defines

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010
Gareth McMullin <gareth@blacksphere.co.nz>

@date 10 March 2013

A set of structure definitions for the USB control structures
defined in chapter 9 of the "Universal Serial Bus Specification Revision 2.0"
Available from the USB Implementers Forum - http://www.usb.org/

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

#ifndef __USBSTD_H
#define __USBSTD_H


/*
 * This file contains structure definitions for the USB control structures
 * defined in chapter 9 of the "Universal Serial Bus Specification Revision 2.0"
 * Available from the USB Implementers Forum - http://www.usb.org/
 */

/* USB Setup Data structure - Table 9-2 */
struct usb_setup_data {
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} __attribute__((packed));

/* Class Definition */
#define USB_CLASS_VENDOR			0xFF

/* bmRequestType bit definitions */
/* bit 7 : direction */
#define USB_REQ_TYPE_DIRECTION			0x80
#define USB_REQ_TYPE_IN				0x80
/* bits 6..5 : type */
#define USB_REQ_TYPE_TYPE			0x60
#define USB_REQ_TYPE_STANDARD			0x00
#define USB_REQ_TYPE_CLASS			0x20
#define USB_REQ_TYPE_VENDOR			0x40
/* bits 4..0 : recipient */
#define USB_REQ_TYPE_RECIPIENT			0x1F
#define USB_REQ_TYPE_DEVICE			0x00
#define USB_REQ_TYPE_INTERFACE			0x01
#define USB_REQ_TYPE_ENDPOINT			0x02
#define USB_REQ_TYPE_OTHER			0x03

/* USB Standard Request Codes - Table 9-4 */
#define USB_REQ_GET_STATUS			0
#define USB_REQ_CLEAR_FEATURE			1
/* Reserved for future use: 2 */
#define USB_REQ_SET_FEATURE			3
/* Reserved for future use: 3 */
#define USB_REQ_SET_ADDRESS			5
#define USB_REQ_GET_DESCRIPTOR			6
#define USB_REQ_SET_DESCRIPTOR			7
#define USB_REQ_GET_CONFIGURATION		8
#define USB_REQ_SET_CONFIGURATION		9
#define USB_REQ_GET_INTERFACE			10
#define USB_REQ_SET_INTERFACE			11
#define USB_REQ_SET_SYNCH_FRAME			12

/* USB Descriptor Types - Table 9-5 */
#define USB_DT_DEVICE				1
#define USB_DT_CONFIGURATION			2
#define USB_DT_STRING				3
#define USB_DT_INTERFACE			4
#define USB_DT_ENDPOINT				5
#define USB_DT_DEVICE_QUALIFIER			6
#define USB_DT_OTHER_SPEED_CONFIGURATION	7
#define USB_DT_INTERFACE_POWER			8
/* From ECNs */
#define USB_DT_OTG				9
#define USB_DT_DEBUG				10
#define USB_DT_INTERFACE_ASSOCIATION		11
/* USB 3.1 Descriptor Types - Table 9-6 */
#define USB_DT_BOS							15
#define USB_DT_DEVICE_CAPABILITY			16
#define USB_DT_SUPERSPEED_USB_ENDPOINT_COMPANION 48
#define USB_DT_SUPERSPEEDPLUS_ISOCHRONOUS_ENDPOINT_COMPANION 49

/* USB Standard Feature Selectors - Table 9-6 */
#define USB_FEAT_ENDPOINT_HALT			0
#define USB_FEAT_DEVICE_REMOTE_WAKEUP		1
#define USB_FEAT_TEST_MODE			2

/* Information Returned by a GetStatus() Request to a Device - Figure 9-4 */
#define USB_DEV_STATUS_SELF_POWERED		0x01
#define USB_DEV_STATUS_REMOTE_WAKEUP		0x02

/* USB Standard Device Descriptor - Table 9-8 */
struct usb_device_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} __attribute__((packed));

#define USB_DT_DEVICE_SIZE sizeof(struct usb_device_descriptor)

/* USB Device_Qualifier Descriptor - Table 9-9
 * Not used in this implementation.
 */
struct usb_device_qualifier_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint8_t bNumConfigurations;
	uint8_t bReserved;
} __attribute__((packed));

/* USB Standard Configuration Descriptor - Table 9-10 */
struct usb_config_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;

	/* Descriptor ends here.  The following are used internally: */
	const struct usb_interface {
		uint8_t *cur_altsetting;
		uint8_t num_altsetting;
		const struct usb_iface_assoc_descriptor *iface_assoc;
		const struct usb_interface_descriptor *altsetting;
	} *interface;
} __attribute__((packed));
#define USB_DT_CONFIGURATION_SIZE		9

/* USB Configuration Descriptor bmAttributes bit definitions */
#define USB_CONFIG_ATTR_DEFAULT			0x80	/** always required (USB2.0 table 9-10) */
#define USB_CONFIG_ATTR_SELF_POWERED		0x40
#define USB_CONFIG_ATTR_REMOTE_WAKEUP		0x20

/* Other Speed Configuration is the same as Configuration Descriptor.
 *  - Table 9-11
 */

/* USB Standard Interface Descriptor - Table 9-12 */
struct usb_interface_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;

	/* Descriptor ends here.  The following are used internally: */
	const struct usb_endpoint_descriptor *endpoint;
	const void *extra;
	int extralen;
} __attribute__((packed));
#define USB_DT_INTERFACE_SIZE			9

/* USB Standard Endpoint Descriptor - Table 9-13 */
struct usb_endpoint_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;

	/* Descriptor ends here.  The following are used internally: */
	const void *extra;
	int extralen;
} __attribute__((packed));
#define USB_DT_ENDPOINT_SIZE		7

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

#define USB_DT_BOS_SIZE 5

/* USB Device Capability Types - USB 3.1 Table 9-14 */
#define USB_DC_WIRELESS_USB 1
#define USB_DC_USB_2_0_EXTENSION 2
#define USB_DC_SUPERSPEED_USB 3
#define USB_DC_CONTAINER_ID 4
#define USB_DC_PLATFORM 5
#define USB_DC_POWER_DELIVERY_CAPABILITY 6
#define USB_DC_BATTERY_INFO_CAPABILITY 7
#define USB_DC_PD_CONSUMER_PORT_CAPABILITY 8
#define USB_DC_PD_PROVIDER_PORT_CAPABILITY 9
#define USB_DC_SUPERSPEED_PLUS 10
#define USB_DC_PRECISION_TIME_MEASUREMENT 11
#define USB_DC_WIRELESS_USB_EXT 12

/* USB bEndpointAddress helper macros */
#define USB_ENDPOINT_ADDR_OUT(x) (x)
#define USB_ENDPOINT_ADDR_IN(x) (0x80 | (x))

/* USB Endpoint Descriptor bmAttributes bit definitions - Table 9-13 */
/* bits 1..0 : transfer type */
#define USB_ENDPOINT_ATTR_CONTROL		0x00
#define USB_ENDPOINT_ATTR_ISOCHRONOUS		0x01
#define USB_ENDPOINT_ATTR_BULK			0x02
#define USB_ENDPOINT_ATTR_INTERRUPT		0x03
#define USB_ENDPOINT_ATTR_TYPE		0x03
/* bits 3..2 : Sync type (only if ISOCHRONOUS) */
#define USB_ENDPOINT_ATTR_NOSYNC		0x00
#define USB_ENDPOINT_ATTR_ASYNC			0x04
#define USB_ENDPOINT_ATTR_ADAPTIVE		0x08
#define USB_ENDPOINT_ATTR_SYNC			0x0C
#define USB_ENDPOINT_ATTR_SYNCTYPE		0x0C
/* bits 5..4 : usage type (only if ISOCHRONOUS) */
#define USB_ENDPOINT_ATTR_DATA			0x00
#define USB_ENDPOINT_ATTR_FEEDBACK		0x10
#define USB_ENDPOINT_ATTR_IMPLICIT_FEEDBACK_DATA 0x20
#define USB_ENDPOINT_ATTR_USAGETYPE		0x30

/* Table 9-15 specifies String Descriptor Zero.
 * Table 9-16 specified UNICODE String Descriptor.
 */
struct usb_string_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wData[];
} __attribute__((packed));

/* From ECN: Interface Association Descriptors, Table 9-Z */
struct usb_iface_assoc_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bFirstInterface;
	uint8_t bInterfaceCount;
	uint8_t bFunctionClass;
	uint8_t bFunctionSubClass;
	uint8_t bFunctionProtocol;
	uint8_t iFunction;
} __attribute__((packed));
#define USB_DT_INTERFACE_ASSOCIATION_SIZE \
				sizeof(struct usb_iface_assoc_descriptor)

enum usb_language_id {
	USB_LANGID_ENGLISH_US = 0x409,
};
#endif

/**@}*/

/** @defgroup usb_cdc_defines USB CDC Type Definitions

@brief <b>Defined Constants and Types for the USB CDC Type Definitions</b>

@ingroup USB_defines

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

#ifndef __CDC_H
#define __CDC_H

/* Definitions of Communications Device Class from
 * "Universal Serial Bus Class Definitions for Communications Devices
 * Revision 1.2"
 */

/* Table 2: Communications Device Class Code */
#define USB_CLASS_CDC			0x02

/* Table 4: Class Subclass Code */
#define USB_CDC_SUBCLASS_DLCM		0x01
#define USB_CDC_SUBCLASS_ACM		0x02
/* ... */

/* Table 5 Communications Interface Class Control Protocol Codes */
#define USB_CDC_PROTOCOL_NONE		0x00
#define USB_CDC_PROTOCOL_AT		0x01
/* ... */

/* Table 6: Data Interface Class Code */
#define USB_CLASS_DATA			0x0A

/* Table 12: Type Values for the bDescriptorType Field */
#define CS_INTERFACE			0x24
#define CS_ENDPOINT			0x25

/* Table 13: bDescriptor SubType in Communications Class Functional
 * Descriptors */
#define USB_CDC_TYPE_HEADER		0x00
#define USB_CDC_TYPE_CALL_MANAGEMENT	0x01
#define USB_CDC_TYPE_ACM		0x02
/* ... */
#define USB_CDC_TYPE_UNION		0x06
/* ... */

/* Table 15: Class-Specific Descriptor Header Format */
struct usb_cdc_header_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdCDC;
} __attribute__((packed));

/* Table 16: Union Interface Functional Descriptor */
struct usb_cdc_union_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bControlInterface;
	uint8_t bSubordinateInterface0;
	/* ... */
} __attribute__((packed));


/* Definitions for Abstract Control Model devices from:
 * "Universal Serial Bus Communications Class Subclass Specification for
 * PSTN Devices"
 */

/* Table 3: Call Management Functional Descriptor */
struct usb_cdc_call_management_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} __attribute__((packed));

/* Table 4: Abstract Control Management Functional Descriptor */
struct usb_cdc_acm_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
} __attribute__((packed));

/* Table 13: Class-Specific Request Codes for PSTN subclasses */
/* ... */
#define USB_CDC_REQ_SET_LINE_CODING		0x20
/* ... */
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE	0x22
/* ... */

/* Table 17: Line Coding Structure */
struct usb_cdc_line_coding {
	uint32_t dwDTERate;
	uint8_t bCharFormat;
	uint8_t bParityType;
	uint8_t bDataBits;
} __attribute__((packed));

enum usb_cdc_line_coding_bCharFormat {
	USB_CDC_1_STOP_BITS			= 0,
	USB_CDC_1_5_STOP_BITS			= 1,
	USB_CDC_2_STOP_BITS			= 2,
};

enum usb_cdc_line_coding_bParityType {
	USB_CDC_NO_PARITY			= 0,
	USB_CDC_ODD_PARITY			= 1,
	USB_CDC_EVEN_PARITY			= 2,
	USB_CDC_MARK_PARITY			= 3,
	USB_CDC_SPACE_PARITY			= 4,
};

/* Table 30: Class-Specific Notification Codes for PSTN subclasses */
/* ... */
#define USB_CDC_NOTIFY_SERIAL_STATE		0x20
/* ... */

/* Notification Structure */
struct usb_cdc_notification {
	uint8_t bmRequestType;
	uint8_t bNotification;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} __attribute__((packed));

#endif

/**@}*/

/** @defgroup usb_driver_defines USB Drivers

@brief <b>Defined Constants and Types for the USB Drivers</b>

@ingroup USB_defines

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

#ifndef __USBD_H
#define __USBD_H


BEGIN_DECLS


enum usbd_request_return_codes {
	USBD_REQ_NOTSUPP	= 0,
	USBD_REQ_HANDLED	= 1,
	USBD_REQ_NEXT_CALLBACK	= 2,
};

typedef struct _usbd_driver usbd_driver;
typedef struct _usbd_device usbd_device;

extern const usbd_driver st_usbfs_v1_usb_driver;
extern const usbd_driver stm32f107_usb_driver;
extern const usbd_driver stm32f207_usb_driver;
extern const usbd_driver st_usbfs_v2_usb_driver;
#define otgfs_usb_driver stm32f107_usb_driver
#define otghs_usb_driver stm32f207_usb_driver
extern const usbd_driver efm32lg_usb_driver;

/* <usb.c> */
/**
 * Main initialization entry point.
 *
 * Initialize the USB firmware library to implement the USB device described
 * by the descriptors provided.
 *
 * It is required that the 48MHz USB clock is already available.
 *
 * @param driver TODO
 * @param dev Pointer to USB device descriptor. This must not be changed while
 *            the device is in use.
 * @param conf Pointer to array of USB configuration descriptors. These must
 *             not be changed while the device is in use. The length of this
 *             array is determined by the bNumConfigurations field in the
 *             device descriptor.
 * @param strings TODO
 * @param control_buffer Pointer to array that would hold the data
 *                       received during control requests with DATA
 *                       stage
 * @param control_buffer_size Size of control_buffer
 * @return the usb device initialized for use. (currently cannot fail).
 */
extern usbd_device * usbd_init(const usbd_driver *driver,
			       const struct usb_device_descriptor *dev,
			       const struct usb_config_descriptor *conf,
                   const struct usb_bos_descriptor *bos,
			       const char **strings, int num_strings,
			       uint8_t *control_buffer,
			       uint16_t control_buffer_size);

/** Registers a reset callback */
extern void usbd_register_reset_callback(usbd_device *usbd_dev,
					 void (*callback)(void));
/** Registers a suspend callback */
extern void usbd_register_suspend_callback(usbd_device *usbd_dev,
					   void (*callback)(void));
/** Registers a resume callback */
extern void usbd_register_resume_callback(usbd_device *usbd_dev,
					  void (*callback)(void));
/** Registers a SOF callback */
extern void usbd_register_sof_callback(usbd_device *usbd_dev,
				       void (*callback)(void));

typedef void (*usbd_control_complete_callback)(usbd_device *usbd_dev,
		struct usb_setup_data *req);

typedef int (*usbd_control_callback)(usbd_device *usbd_dev,
		struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
		usbd_control_complete_callback *complete);

typedef void (*usbd_set_config_callback)(usbd_device *usbd_dev,
					 uint16_t wValue);

typedef void (*usbd_set_altsetting_callback)(usbd_device *usbd_dev,
					     uint16_t wIndex, uint16_t wValue);

typedef void (*usbd_endpoint_callback)(usbd_device *usbd_dev, uint8_t ep);

/* <usb_control.c> */
/** Registers a control callback.
 *
 * The specified callback will be called if (type == (bmRequestType
 * & type_mask))
 * @param type Handled request type
 * @param type_mask Mask to apply before matching request type
 * @return 0 if successful
 */
extern int usbd_register_control_callback(usbd_device *usbd_dev, uint8_t type,
					  uint8_t type_mask,
					  usbd_control_callback callback);

/* <usb_standard.c> */
/** Registers a "Set Config" callback
 * @return 0 if successful
 */
extern int usbd_register_set_config_callback(usbd_device *usbd_dev,
					  usbd_set_config_callback callback);
/** Registers a "Set Interface" (alternate setting) callback */
extern void usbd_register_set_altsetting_callback(usbd_device *usbd_dev,
					usbd_set_altsetting_callback callback);

/** Registers a non-contiguous string descriptor */
extern void usbd_register_extra_string(usbd_device *usbd_dev, int index, const char* string);

/* Functions to be provided by the hardware abstraction layer */
extern void usbd_poll(usbd_device *usbd_dev);

/** Disconnect, if supported by the driver */
extern void usbd_disconnect(usbd_device *usbd_dev, bool disconnected);

/** Setup an endpoint
 * @param addr Full EP address including direction (e.g. 0x01 or 0x81)
 * @param type Value for bmAttributes (USB_ENDPOINT_ATTR_*)
 */
extern void usbd_ep_setup(usbd_device *usbd_dev, uint8_t addr, uint8_t type,
		uint16_t max_size, usbd_endpoint_callback callback);

/** Write a packet
 * @param addr EP address (direction is ignored)
 * @param len # of bytes
 * @return 0 if failed, len if successful
 */
extern uint16_t usbd_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
				const void *buf, uint16_t len);

/** Read a packet
 * @param addr EP address
 * @param len # of bytes
 * @return Actual # of bytes read
 */
extern uint16_t usbd_ep_read_packet(usbd_device *usbd_dev, uint8_t addr,
			       void *buf, uint16_t len);
/** Set/clear STALL condition on an endpoint
 * @param addr Full EP address (with direction bit)
 * @param stall if 0, clear STALL, else set stall.
 */
extern void usbd_ep_stall_set(usbd_device *usbd_dev, uint8_t addr,
			      uint8_t stall);

/** Get STALL status of an endpoint
 * @param addr Full EP address (with direction bit)
 * @return nonzero if endpoint is stalled
 */
extern uint8_t usbd_ep_stall_get(usbd_device *usbd_dev, uint8_t addr);

/** Set an Out endpoint to NAK
 * @param addr EP address
 * @param nak if nonzero, set NAK
 */
extern void usbd_ep_nak_set(usbd_device *usbd_dev, uint8_t addr, uint8_t nak);

END_DECLS

#endif

/**@}*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
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

#ifndef LIBOPENCM3_DESIG_H
#define LIBOPENCM3_DESIG_H


/* --- Device Electronic Signature -------------------------------- */

/* Flash size register */
#define DESIG_FLASH_SIZE		MMIO16(DESIG_FLASH_SIZE_BASE + 0x00)

BEGIN_DECLS

/**
 * Read the on board flash size
 * @return flash size in KB
 */
uint16_t desig_get_flash_size(void);

/**
 * Read the full 96 bit unique identifier
 * Note: ST specifies that bits 31..16 are _also_ reserved for future use
 * @param result pointer to at least 3xuint32_ts (96 bits)
 */
void desig_get_unique_id(uint32_t *result);

/**
 * Read the full 96 bit unique identifier and return it as a
 * zero-terminated string
 * @param string memory region to write the result to
 8 @param string_len the size of string in bytes
 */
void desig_get_unique_id_as_string(char *string,
				   unsigned int string_len);

END_DECLS

#endif
/* This provides unification of USB code for supported STM32F subfamilies */

/*
 * This file is part of the libopencm3 project.
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

#ifndef LIBOPENCM3_ST_USBFS_H
#define LIBOPENCM3_ST_USBFS_H


#if defined(STM32F0)
#elif defined(STM32F1)
#elif defined(STM32F3)
#elif defined(STM32L0)
#elif defined(STM32L1)
#else
#       error "STM32 family not defined or not supported."
#endif

#endif
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2011 Fergus Noble <fergusnoble@gmail.com>
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

#ifndef LIBOPENCM3_MEMORYMAP_COMMON_H
#define LIBOPENCM3_MEMORYMAP_COMMON_H

#if defined(STM32F0)
#elif defined(STM32F1)
#elif defined(STM32F2)
#elif defined(STM32F3)
#elif defined(STM32F4)
#elif defined(STM32F7)
#elif defined(STM32L0)
#elif defined(STM32L1)
#elif defined(STM32L4)
#else
#	error "stm32 family not defined."
#endif

#endif /* LIBOPENCM3_MEMORYMAP_COMMON_H */
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Piotr Esden-Tempski <piotr@esden.net>
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

#ifndef LIBOPENCM3_TOOLS_H
#define LIBOPENCM3_TOOLS_H

/*
 * Register accessors / manipulators
 */

/* Get register content. */
#define GET_REG(REG)		((uint16_t) *(REG))

/* Set register content. */
#define SET_REG(REG, VAL)	(*(REG) = (uint16_t)(VAL))

/* Clear register bit. */
#define CLR_REG_BIT(REG, BIT)	SET_REG((REG), (~(BIT)))

/* Clear register bit masking out some bits that must not be touched. */
#define CLR_REG_BIT_MSK_AND_SET(REG, MSK, BIT, EXTRA_BITS) \
	SET_REG((REG), (GET_REG((REG)) & (MSK) & (~(BIT))) | (EXTRA_BITS))

#define CLR_REG_BIT_MSK(REG, MSK, BIT) \
		CLR_REG_BIT_MSK_AND_SET((REG), (MSK), (BIT), 0)

/* Get masked out bit value. */
#define GET_REG_BIT(REG, BIT)	(GET_REG(REG) & (BIT))

/*
 * Set/reset a bit in a masked window by using toggle mechanism.
 *
 * This means that we look at the bits in the bit window designated by
 * the mask. If the bit in the masked window is not matching the
 * bit mask BIT then we write 1 and if the bit in the masked window is
 * matching the bit mask BIT we write 0.
 *
 * TODO: We may need a faster implementation of that one?
 */
#define TOG_SET_REG_BIT_MSK_AND_SET(REG, MSK, BIT, EXTRA_BITS)		\
do {									\
	register uint16_t toggle_mask = GET_REG(REG) & (MSK);		\
	register uint16_t bit_selector;					\
	for (bit_selector = 1; bit_selector; bit_selector <<= 1) {	\
		if ((bit_selector & (BIT)) != 0) {			\
			toggle_mask ^= bit_selector;			\
		}							\
	}								\
	SET_REG((REG), toggle_mask | (EXTRA_BITS));			\
} while (0)

#define TOG_SET_REG_BIT_MSK(REG, MSK, BIT) \
	TOG_SET_REG_BIT_MSK_AND_SET((REG), (MSK), (BIT), 0)

#endif
/*
 * This file is part of the libopencm3 project.
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
/* THIS FILE SHOULD NOT BE INCLUDED DIRECTLY !
 * Use top-level <libopencm3/stm32/st_usbfs.h>
 */

#ifndef LIBOPENCM3_ST_USBFS_H
#else


#endif
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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

#ifndef LIBOPENCM3_MEMORYMAP_H
#define LIBOPENCM3_MEMORYMAP_H


/* --- STM32 specific peripheral definitions ------------------------------- */

/* Memory map for all buses */
#define FLASH_BASE			(0x08000000U)
#define PERIPH_BASE			(0x40000000U)
#define INFO_BASE			(0x1ffff000U)
#define PERIPH_BASE_APB1		(PERIPH_BASE + 0x00000)
#define PERIPH_BASE_APB2		(PERIPH_BASE + 0x10000)
#define PERIPH_BASE_AHB			(PERIPH_BASE + 0x18000)

/* Register boundary addresses */

/* APB1 */
#define TIM2_BASE			(PERIPH_BASE_APB1 + 0x0000)
#define TIM3_BASE			(PERIPH_BASE_APB1 + 0x0400)
#define TIM4_BASE			(PERIPH_BASE_APB1 + 0x0800)
#define TIM5_BASE			(PERIPH_BASE_APB1 + 0x0c00)
#define TIM6_BASE			(PERIPH_BASE_APB1 + 0x1000)
#define TIM7_BASE			(PERIPH_BASE_APB1 + 0x1400)
#define TIM12_BASE			(PERIPH_BASE_APB1 + 0x1800)
#define TIM13_BASE			(PERIPH_BASE_APB1 + 0x1c00)
#define TIM14_BASE			(PERIPH_BASE_APB1 + 0x2000)
/* PERIPH_BASE_APB1 + 0x2400 (0x4000 2400 - 0x4000 27FF): Reserved */
#define RTC_BASE			(PERIPH_BASE_APB1 + 0x2800)
#define WWDG_BASE			(PERIPH_BASE_APB1 + 0x2c00)
#define IWDG_BASE			(PERIPH_BASE_APB1 + 0x3000)
/* PERIPH_BASE_APB1 + 0x3400 (0x4000 3400 - 0x4000 37FF): Reserved */
#define SPI2_BASE			(PERIPH_BASE_APB1 + 0x3800)
#define SPI3_BASE			(PERIPH_BASE_APB1 + 0x3c00)
/* PERIPH_BASE_APB1 + 0x4000 (0x4000 4000 - 0x4000 3FFF): Reserved */
#define USART2_BASE			(PERIPH_BASE_APB1 + 0x4400)
#define USART3_BASE			(PERIPH_BASE_APB1 + 0x4800)
#define UART4_BASE			(PERIPH_BASE_APB1 + 0x4c00)
#define UART5_BASE			(PERIPH_BASE_APB1 + 0x5000)
#define I2C1_BASE			(PERIPH_BASE_APB1 + 0x5400)
#define I2C2_BASE			(PERIPH_BASE_APB1 + 0x5800)
#define USB_DEV_FS_BASE			(PERIPH_BASE_APB1 + 0x5c00)
#define USB_PMA_BASE			(PERIPH_BASE_APB1 + 0x6000)
#define USB_CAN_SRAM_BASE		(PERIPH_BASE_APB1 + 0x6000)
#define BX_CAN1_BASE			(PERIPH_BASE_APB1 + 0x6400)
#define BX_CAN2_BASE			(PERIPH_BASE_APB1 + 0x6800)
/* PERIPH_BASE_APB1 + 0x6800 (0x4000 6800 - 0x4000 6BFF): Reserved? Typo? */
#define BACKUP_REGS_BASE		(PERIPH_BASE_APB1 + 0x6c00)
#define POWER_CONTROL_BASE		(PERIPH_BASE_APB1 + 0x7000)
#define DAC_BASE			(PERIPH_BASE_APB1 + 0x7400)
#define CEC_BASE			(PERIPH_BASE_APB1 + 0x7800)
/* PERIPH_BASE_APB1 + 0x7c00 (0x4000 7c00 - 0x4000 FFFF): Reserved */

/* APB2 */
#define AFIO_BASE			(PERIPH_BASE_APB2 + 0x0000)
#define EXTI_BASE			(PERIPH_BASE_APB2 + 0x0400)
#define GPIO_PORT_A_BASE		(PERIPH_BASE_APB2 + 0x0800)
#define GPIO_PORT_B_BASE		(PERIPH_BASE_APB2 + 0x0c00)
#define GPIO_PORT_C_BASE		(PERIPH_BASE_APB2 + 0x1000)
#define GPIO_PORT_D_BASE		(PERIPH_BASE_APB2 + 0x1400)
#define GPIO_PORT_E_BASE		(PERIPH_BASE_APB2 + 0x1800)
#define GPIO_PORT_F_BASE		(PERIPH_BASE_APB2 + 0x1c00)
#define GPIO_PORT_G_BASE		(PERIPH_BASE_APB2 + 0x2000)
#define ADC1_BASE			(PERIPH_BASE_APB2 + 0x2400)
#define ADC2_BASE			(PERIPH_BASE_APB2 + 0x2800)
#define TIM1_BASE			(PERIPH_BASE_APB2 + 0x2c00)
#define SPI1_BASE			(PERIPH_BASE_APB2 + 0x3000)
#define TIM8_BASE			(PERIPH_BASE_APB2 + 0x3400)
#define USART1_BASE			(PERIPH_BASE_APB2 + 0x3800)
#define ADC3_BASE			(PERIPH_BASE_APB2 + 0x3c00)
#define TIM15_BASE			(PERIPH_BASE_APB2 + 0x4000)
#define TIM16_BASE			(PERIPH_BASE_APB2 + 0x4400)
#define TIM17_BASE			(PERIPH_BASE_APB2 + 0x4800)
#define TIM9_BASE			(PERIPH_BASE_APB2 + 0x4c00)
#define TIM10_BASE			(PERIPH_BASE_APB2 + 0x5000)
#define TIM11_BASE			(PERIPH_BASE_APB2 + 0x5400)
/* PERIPH_BASE_APB2 + 0x5800 (0x4001 5800 - 0x4001 7FFF): Reserved */

/* AHB */
#define SDIO_BASE			(PERIPH_BASE_AHB + 0x00000)
/* PERIPH_BASE_AHB + 0x0400 (0x4001 8400 - 0x4001 7FFF): Reserved */
#define DMA1_BASE			(PERIPH_BASE_AHB + 0x08000)
#define DMA2_BASE			(PERIPH_BASE_AHB + 0x08400)
/* PERIPH_BASE_AHB + 0x8800 (0x4002 0800 - 0x4002 0FFF): Reserved */
#define RCC_BASE			(PERIPH_BASE_AHB + 0x09000)
/* PERIPH_BASE_AHB + 0x9400 (0x4002 1400 - 0x4002 1FFF): Reserved */
#define FLASH_MEM_INTERFACE_BASE	(PERIPH_BASE_AHB + 0x0a000)
#define CRC_BASE			(PERIPH_BASE_AHB + 0x0b000)
/* PERIPH_BASE_AHB + 0xb400 (0x4002 3400 - 0x4002 7FFF): Reserved */
#define ETHERNET_BASE			(PERIPH_BASE_AHB + 0x10000)
/* PERIPH_BASE_AHB + 0x18000 (0x4003 0000 - 0x4FFF FFFF): Reserved */
#define USB_OTG_FS_BASE			(PERIPH_BASE_AHB + 0xffe8000)

/* PPIB */
#define DBGMCU_BASE			(PPBI_BASE + 0x00042000)

/* FSMC */
#define FSMC_BASE			(PERIPH_BASE +  0x60000000)

/* Device Electronic Signature */
#define DESIG_FLASH_SIZE_BASE		(INFO_BASE + 0x7e0)
#define DESIG_UNIQUE_ID_BASE		(INFO_BASE + 0x7e8)
/* Ignore the "reserved for future use" half of the first word */
#define DESIG_UNIQUE_ID0		MMIO32(DESIG_UNIQUE_ID_BASE)
#define DESIG_UNIQUE_ID1		MMIO32(DESIG_UNIQUE_ID_BASE + 4)
#define DESIG_UNIQUE_ID2		MMIO32(DESIG_UNIQUE_ID_BASE + 8)


#endif
/** @defgroup rcc_defines RCC Defines
 *
 * @brief <b>Defined Constants and Types for the STM32F1xx Reset and Clock
 * Control</b>
 *
 * @ingroup STM32F1xx_defines
 *
 * @version 1.0.0
 *
 * @author @htmlonly &copy; @endhtmlonly 2009
 * Federico Ruiz-Ugalde \<memeruiz at gmail dot com\>
 * @author @htmlonly &copy; @endhtmlonly 2009
 * Uwe Hermann <uwe@hermann-uwe.de>
 *
 * @date 18 August 2012
 *
 * LGPL License Terms @ref lgpl_license
 *  */
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2009 Federico Ruiz-Ugalde <memeruiz at gmail dot com>
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

#ifndef LIBOPENCM3_RCC_H
#define LIBOPENCM3_RCC_H

/* Note: Regs/bits marked (**) only exist in "connectivity line" STM32s. */
/* Note: Regs/bits marked (XX) do NOT exist in "connectivity line" STM32s. */

/* --- RCC registers ------------------------------------------------------- */

#define RCC_CR					MMIO32(RCC_BASE + 0x00)
#define RCC_CFGR				MMIO32(RCC_BASE + 0x04)
#define RCC_CIR					MMIO32(RCC_BASE + 0x08)
#define RCC_APB2RSTR				MMIO32(RCC_BASE + 0x0c)
#define RCC_APB1RSTR				MMIO32(RCC_BASE + 0x10)
#define RCC_AHBENR				MMIO32(RCC_BASE + 0x14)
#define RCC_APB2ENR				MMIO32(RCC_BASE + 0x18)
#define RCC_APB1ENR				MMIO32(RCC_BASE + 0x1c)
#define RCC_BDCR				MMIO32(RCC_BASE + 0x20)
#define RCC_CSR					MMIO32(RCC_BASE + 0x24)
#define RCC_AHBRSTR				MMIO32(RCC_BASE + 0x28) /*(**)*/
#define RCC_CFGR2				MMIO32(RCC_BASE + 0x2c) /*(**)*/

/* --- RCC_CR values ------------------------------------------------------- */

#define RCC_CR_PLL3RDY				(1 << 29) /* (**) */
#define RCC_CR_PLL3ON				(1 << 28) /* (**) */
#define RCC_CR_PLL2RDY				(1 << 27) /* (**) */
#define RCC_CR_PLL2ON				(1 << 26) /* (**) */
#define RCC_CR_PLLRDY				(1 << 25)
#define RCC_CR_PLLON				(1 << 24)
#define RCC_CR_CSSON				(1 << 19)
#define RCC_CR_HSEBYP				(1 << 18)
#define RCC_CR_HSERDY				(1 << 17)
#define RCC_CR_HSEON				(1 << 16)
/* HSICAL: [15:8] */
/* HSITRIM: [7:3] */
#define RCC_CR_HSIRDY				(1 << 1)
#define RCC_CR_HSION				(1 << 0)

/* --- RCC_CFGR values ----------------------------------------------------- */

#define RCC_CFGR_OTGFSPRE			(1 << 22) /* Connectivity line */
#define RCC_CFGR_USBPRE				(1 << 22) /* LD,MD, HD, XL */

#define RCC_CFGR_PLLMUL_SHIFT			18
#define RCC_CFGR_PLLMUL				(0xF << RCC_CFGR_PLLMUL_SHIFT)

#define RCC_CFGR_PLLXTPRE			(1 << 17)
#define RCC_CFGR_PLLSRC				(1 << 16)

#define RCC_CFGR_ADCPRE_SHIFT			14
#define RCC_CFGR_ADCPRE				(3 << RCC_CFGR_ADCPRE_SHIFT)

#define RCC_CFGR_PPRE2_SHIFT			11
#define RCC_CFGR_PPRE2				(7 << RCC_CFGR_PPRE2_SHIFT)

#define RCC_CFGR_PPRE1_SHIFT			8
#define RCC_CFGR_PPRE1				(7 << RCC_CFGR_PPRE1_SHIFT)

#define RCC_CFGR_HPRE_SHIFT			4
#define RCC_CFGR_HPRE				(0xF << RCC_CFGR_HPRE_SHIFT)

#define RCC_CFGR_SWS_SHIFT			2
#define RCC_CFGR_SWS				(3 << RCC_CFGR_SWS_SHIFT)

#define RCC_CFGR_SW_SHIFT			0
#define RCC_CFGR_SW				(3 << RCC_CFGR_SW_SHIFT)

/* MCO: Microcontroller clock output */
/** @defgroup rcc_cfgr_co RCC_CFGR Microcontroller Clock Output Source
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_MCO_SHIFT			24
#define RCC_CFGR_MCO_MASK			0xf
#define RCC_CFGR_MCO_NOCLK			0x0
#define RCC_CFGR_MCO_SYSCLK			0x4
#define RCC_CFGR_MCO_HSICLK			0x5
#define RCC_CFGR_MCO_HSECLK			0x6
#define RCC_CFGR_MCO_PLLCLK_DIV2		0x7
#define RCC_CFGR_MCO_PLL2CLK			0x8 /* (**) */
#define RCC_CFGR_MCO_PLL3CLK_DIV2		0x9 /* (**) */
#define RCC_CFGR_MCO_XT1			0xa /* (**) */
#define RCC_CFGR_MCO_PLL3			0xb /* (**) */
/**@}*/

/* USBPRE: USB prescaler (RCC_CFGR[22]) */
/** @defgroup rcc_cfgr_usbpre RCC_CFGR USB prescale Factors
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_USBPRE_PLL_CLK_DIV1_5		0x0
#define RCC_CFGR_USBPRE_PLL_CLK_NODIV		0x1
/**@}*/

/* OTGFSPRE: USB OTG FS prescaler (RCC_CFGR[22]; only in conn. line STM32s) */
#define RCC_CFGR_USBPRE_PLL_VCO_CLK_DIV3	0x0
#define RCC_CFGR_USBPRE_PLL_VCO_CLK_DIV2	0x1

/* PLLMUL: PLL multiplication factor */
/** @defgroup rcc_cfgr_pmf RCC_CFGR PLL Multiplication Factor
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL2		0x0 /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL3		0x1 /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL4		0x2
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL5		0x3
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL6		0x4
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL7		0x5
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL8		0x6
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL9		0x7
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL10		0x8 /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL11		0x9 /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL12		0xa /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL13		0xb /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL14		0xc /* (XX) */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL15		0xd /* 0xd: PLL x 15 */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL6_5		0xd /* 0xd: PLL x 6.5 for conn.
						       line */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL16		0xe /* (XX) */
/* #define PLLMUL_PLL_CLK_MUL16		0xf */ /* (XX) */ /* Errata? 17? */
/**@}*/

/* TODO: conn. line differs. */
/* PLLXTPRE: HSE divider for PLL entry */
/** @defgroup rcc_cfgr_hsepre RCC_CFGR HSE Divider for PLL
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_PLLXTPRE_HSE_CLK		0x0
#define RCC_CFGR_PLLXTPRE_HSE_CLK_DIV2		0x1
/**@}*/

/* PLLSRC: PLL entry clock source */
/** @defgroup rcc_cfgr_pcs RCC_CFGR PLL Clock Source
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_PLLSRC_HSI_CLK_DIV2		0x0
#define RCC_CFGR_PLLSRC_HSE_CLK			0x1
#define RCC_CFGR_PLLSRC_PREDIV1_CLK		0x1 /* On conn. line */
/**@}*/

/* ADCPRE: ADC prescaler */
/****************************************************************************/
/** @defgroup rcc_cfgr_adcpre RCC ADC clock prescaler enable values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_ADCPRE_PCLK2_DIV2		0x0
#define RCC_CFGR_ADCPRE_PCLK2_DIV4		0x1
#define RCC_CFGR_ADCPRE_PCLK2_DIV6		0x2
#define RCC_CFGR_ADCPRE_PCLK2_DIV8		0x3
/**@}*/

/* PPRE2: APB high-speed prescaler (APB2) */
/** @defgroup rcc_cfgr_apb2pre RCC_CFGR APB2 prescale Factors
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_PPRE2_HCLK_NODIV		0x0
#define RCC_CFGR_PPRE2_HCLK_DIV2		0x4
#define RCC_CFGR_PPRE2_HCLK_DIV4		0x5
#define RCC_CFGR_PPRE2_HCLK_DIV8		0x6
#define RCC_CFGR_PPRE2_HCLK_DIV16		0x7
/**@}*/

/* PPRE1: APB low-speed prescaler (APB1) */
/** @defgroup rcc_cfgr_apb1pre RCC_CFGR APB1 prescale Factors
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_PPRE1_HCLK_NODIV		0x0
#define RCC_CFGR_PPRE1_HCLK_DIV2		0x4
#define RCC_CFGR_PPRE1_HCLK_DIV4		0x5
#define RCC_CFGR_PPRE1_HCLK_DIV8		0x6
#define RCC_CFGR_PPRE1_HCLK_DIV16		0x7
/**@}*/

/* HPRE: AHB prescaler */
/** @defgroup rcc_cfgr_ahbpre RCC_CFGR AHB prescale Factors
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_HPRE_SYSCLK_NODIV		0x0
#define RCC_CFGR_HPRE_SYSCLK_DIV2		0x8
#define RCC_CFGR_HPRE_SYSCLK_DIV4		0x9
#define RCC_CFGR_HPRE_SYSCLK_DIV8		0xa
#define RCC_CFGR_HPRE_SYSCLK_DIV16		0xb
#define RCC_CFGR_HPRE_SYSCLK_DIV64		0xc
#define RCC_CFGR_HPRE_SYSCLK_DIV128		0xd
#define RCC_CFGR_HPRE_SYSCLK_DIV256		0xe
#define RCC_CFGR_HPRE_SYSCLK_DIV512		0xf
/**@}*/

/* SWS: System clock switch status */
#define RCC_CFGR_SWS_SYSCLKSEL_HSICLK		0x0
#define RCC_CFGR_SWS_SYSCLKSEL_HSECLK		0x1
#define RCC_CFGR_SWS_SYSCLKSEL_PLLCLK		0x2

/* SW: System clock switch */
/** @defgroup rcc_cfgr_scs RCC_CFGR System Clock Selection
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_CFGR_SW_SYSCLKSEL_HSICLK		0x0
#define RCC_CFGR_SW_SYSCLKSEL_HSECLK		0x1
#define RCC_CFGR_SW_SYSCLKSEL_PLLCLK		0x2
/**@}*/

/* --- RCC_CIR values ------------------------------------------------------ */

/* Clock security system interrupt clear bit */
#define RCC_CIR_CSSC				(1 << 23)

/* OSC ready interrupt clear bits */
#define RCC_CIR_PLL3RDYC			(1 << 22) /* (**) */
#define RCC_CIR_PLL2RDYC			(1 << 21) /* (**) */
#define RCC_CIR_PLLRDYC				(1 << 20)
#define RCC_CIR_HSERDYC				(1 << 19)
#define RCC_CIR_HSIRDYC				(1 << 18)
#define RCC_CIR_LSERDYC				(1 << 17)
#define RCC_CIR_LSIRDYC				(1 << 16)

/* OSC ready interrupt enable bits */
#define RCC_CIR_PLL3RDYIE			(1 << 14) /* (**) */
#define RCC_CIR_PLL2RDYIE			(1 << 13) /* (**) */
#define RCC_CIR_PLLRDYIE			(1 << 12)
#define RCC_CIR_HSERDYIE			(1 << 11)
#define RCC_CIR_HSIRDYIE			(1 << 10)
#define RCC_CIR_LSERDYIE			(1 << 9)
#define RCC_CIR_LSIRDYIE			(1 << 8)

/* Clock security system interrupt flag bit */
#define RCC_CIR_CSSF				(1 << 7)

/* OSC ready interrupt flag bits */
#define RCC_CIR_PLL3RDYF			(1 << 6) /* (**) */
#define RCC_CIR_PLL2RDYF			(1 << 5) /* (**) */
#define RCC_CIR_PLLRDYF				(1 << 4)
#define RCC_CIR_HSERDYF				(1 << 3)
#define RCC_CIR_HSIRDYF				(1 << 2)
#define RCC_CIR_LSERDYF				(1 << 1)
#define RCC_CIR_LSIRDYF				(1 << 0)

/* --- RCC_APB2RSTR values ------------------------------------------------- */

/** @defgroup rcc_apb2rstr_rst RCC_APB2RSTR reset values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_APB2RSTR_TIM17RST			(1 << 18)
#define RCC_APB2RSTR_TIM16RST			(1 << 17)
#define RCC_APB2RSTR_TIM15RST			(1 << 16)
#define RCC_APB2RSTR_ADC3RST			(1 << 15) /* (XX) */
#define RCC_APB2RSTR_USART1RST			(1 << 14)
#define RCC_APB2RSTR_TIM8RST			(1 << 13) /* (XX) */
#define RCC_APB2RSTR_SPI1RST			(1 << 12)
#define RCC_APB2RSTR_TIM1RST			(1 << 11)
#define RCC_APB2RSTR_ADC2RST			(1 << 10)
#define RCC_APB2RSTR_ADC1RST			(1 << 9)
#define RCC_APB2RSTR_IOPGRST			(1 << 8)  /* (XX) */
#define RCC_APB2RSTR_IOPFRST			(1 << 7)  /* (XX) */
#define RCC_APB2RSTR_IOPERST			(1 << 6)
#define RCC_APB2RSTR_IOPDRST			(1 << 5)
#define RCC_APB2RSTR_IOPCRST			(1 << 4)
#define RCC_APB2RSTR_IOPBRST			(1 << 3)
#define RCC_APB2RSTR_IOPARST			(1 << 2)
#define RCC_APB2RSTR_AFIORST			(1 << 0)
/**@}*/

/* --- RCC_APB1RSTR values ------------------------------------------------- */

/** @defgroup rcc_apb1rstr_rst RCC_APB1RSTR reset values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_APB1RSTR_DACRST			(1 << 29)
#define RCC_APB1RSTR_PWRRST			(1 << 28)
#define RCC_APB1RSTR_BKPRST			(1 << 27)
#define RCC_APB1RSTR_CAN2RST			(1 << 26) /* (**) */
#define RCC_APB1RSTR_CAN1RST			(1 << 25) /* (**) */
#define RCC_APB1RSTR_CANRST			(1 << 25) /* (XX) Alias for
							     CAN1RST */
#define RCC_APB1RSTR_USBRST			(1 << 23) /* (XX) */
#define RCC_APB1RSTR_I2C2RST			(1 << 22)
#define RCC_APB1RSTR_I2C1RST			(1 << 21)
#define RCC_APB1RSTR_UART5RST			(1 << 20)
#define RCC_APB1RSTR_UART4RST			(1 << 19)
#define RCC_APB1RSTR_USART3RST			(1 << 18)
#define RCC_APB1RSTR_USART2RST			(1 << 17)
#define RCC_APB1RSTR_SPI3RST			(1 << 15)
#define RCC_APB1RSTR_SPI2RST			(1 << 14)
#define RCC_APB1RSTR_WWDGRST			(1 << 11)
#define RCC_APB1RSTR_TIM7RST			(1 << 5)
#define RCC_APB1RSTR_TIM6RST			(1 << 4)
#define RCC_APB1RSTR_TIM5RST			(1 << 3)
#define RCC_APB1RSTR_TIM4RST			(1 << 2)
#define RCC_APB1RSTR_TIM3RST			(1 << 1)
#define RCC_APB1RSTR_TIM2RST			(1 << 0)
/**@}*/

/* --- RCC_AHBENR values --------------------------------------------------- */

/** @defgroup rcc_ahbenr_en RCC_AHBENR enable values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_AHBENR_ETHMACENRX			(1 << 16)
#define RCC_AHBENR_ETHMACENTX			(1 << 15)
#define RCC_AHBENR_ETHMACEN			(1 << 14)
#define RCC_AHBENR_OTGFSEN			(1 << 12)
#define RCC_AHBENR_SDIOEN			(1 << 10)
#define RCC_AHBENR_FSMCEN			(1 << 8)
#define RCC_AHBENR_CRCEN			(1 << 6)
#define RCC_AHBENR_FLITFEN			(1 << 4)
#define RCC_AHBENR_SRAMEN			(1 << 2)
#define RCC_AHBENR_DMA2EN			(1 << 1)
#define RCC_AHBENR_DMA1EN			(1 << 0)
/**@}*/

/* --- RCC_APB2ENR values -------------------------------------------------- */

/** @defgroup rcc_apb2enr_en RCC_APB2ENR enable values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_APB2ENR_TIM17EN			(1 << 17)
#define RCC_APB2ENR_TIM16EN			(1 << 16)
#define RCC_APB2ENR_TIM15EN			(1 << 16)
#define RCC_APB2ENR_ADC3EN			(1 << 15) /* (XX) */
#define RCC_APB2ENR_USART1EN			(1 << 14)
#define RCC_APB2ENR_TIM8EN			(1 << 13) /* (XX) */
#define RCC_APB2ENR_SPI1EN			(1 << 12)
#define RCC_APB2ENR_TIM1EN			(1 << 11)
#define RCC_APB2ENR_ADC2EN			(1 << 10)
#define RCC_APB2ENR_ADC1EN			(1 << 9)
#define RCC_APB2ENR_IOPGEN			(1 << 8)  /* (XX) */
#define RCC_APB2ENR_IOPFEN			(1 << 7)  /* (XX) */
#define RCC_APB2ENR_IOPEEN			(1 << 6)
#define RCC_APB2ENR_IOPDEN			(1 << 5)
#define RCC_APB2ENR_IOPCEN			(1 << 4)
#define RCC_APB2ENR_IOPBEN			(1 << 3)
#define RCC_APB2ENR_IOPAEN			(1 << 2)
#define RCC_APB2ENR_AFIOEN			(1 << 0)
/**@}*/

/* --- RCC_APB1ENR values -------------------------------------------------- */

/** @defgroup rcc_apb1enr_en RCC_APB1ENR enable values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_APB1ENR_DACEN			(1 << 29)
#define RCC_APB1ENR_PWREN			(1 << 28)
#define RCC_APB1ENR_BKPEN			(1 << 27)
#define RCC_APB1ENR_CAN2EN			(1 << 26) /* (**) */
#define RCC_APB1ENR_CAN1EN			(1 << 25) /* (**) */
#define RCC_APB1ENR_CANEN			(1 << 25) /* (XX) Alias for
							     CAN1EN */
#define RCC_APB1ENR_USBEN			(1 << 23) /* (XX) */
#define RCC_APB1ENR_I2C2EN			(1 << 22)
#define RCC_APB1ENR_I2C1EN			(1 << 21)
#define RCC_APB1ENR_UART5EN			(1 << 20)
#define RCC_APB1ENR_UART4EN			(1 << 19)
#define RCC_APB1ENR_USART3EN			(1 << 18)
#define RCC_APB1ENR_USART2EN			(1 << 17)
#define RCC_APB1ENR_SPI3EN			(1 << 15)
#define RCC_APB1ENR_SPI2EN			(1 << 14)
#define RCC_APB1ENR_WWDGEN			(1 << 11)
#define RCC_APB1ENR_TIM7EN			(1 << 5)
#define RCC_APB1ENR_TIM6EN			(1 << 4)
#define RCC_APB1ENR_TIM5EN			(1 << 3)
#define RCC_APB1ENR_TIM4EN			(1 << 2)
#define RCC_APB1ENR_TIM3EN			(1 << 1)
#define RCC_APB1ENR_TIM2EN			(1 << 0)
/**@}*/

/* --- RCC_BDCR values ----------------------------------------------------- */

#define RCC_BDCR_BDRST				(1 << 16)
#define RCC_BDCR_RTCEN				(1 << 15)
/* RCC_BDCR[9:8]: RTCSEL */
#define RCC_BDCR_LSEBYP				(1 << 2)
#define RCC_BDCR_LSERDY				(1 << 1)
#define RCC_BDCR_LSEON				(1 << 0)

/* --- RCC_CSR values ------------------------------------------------------ */

#define RCC_CSR_LPWRRSTF			(1 << 31)
#define RCC_CSR_WWDGRSTF			(1 << 30)
#define RCC_CSR_IWDGRSTF			(1 << 29)
#define RCC_CSR_SFTRSTF				(1 << 28)
#define RCC_CSR_PORRSTF				(1 << 27)
#define RCC_CSR_PINRSTF				(1 << 26)
#define RCC_CSR_RMVF				(1 << 24)
#define RCC_CSR_LSIRDY				(1 << 1)
#define RCC_CSR_LSION				(1 << 0)

/* --- RCC_AHBRSTR values -------------------------------------------------- */

/** @defgroup rcc_ahbrstr_rst RCC_AHBRSTR reset values
@ingroup STM32F1xx_rcc_defines

@{*/
#define RCC_AHBRSTR_ETHMACRST			(1 << 14)
#define RCC_AHBRSTR_OTGFSRST			(1 << 12)
/**@}*/

/* --- RCC_CFGR2 values ---------------------------------------------------- */

/* I2S3SRC: I2S3 clock source */
#define RCC_CFGR2_I2S3SRC_SYSCLK		0x0
#define RCC_CFGR2_I2S3SRC_PLL3_VCO_CLK		0x1

/* I2S2SRC: I2S2 clock source */
#define RCC_CFGR2_I2S2SRC_SYSCLK		0x0
#define RCC_CFGR2_I2S2SRC_PLL3_VCO_CLK		0x1
#define RCC_CFGR2_I2S2SRC			(1 << 17)

/* PREDIV1SRC: PREDIV1 entry clock source */
#define RCC_CFGR2_PREDIV1SRC_HSE_CLK		0x0
#define RCC_CFGR2_PREDIV1SRC_PLL2_CLK		0x1
#define RCC_CFGR2_PREDIV1SRC			(1 << 16)

#define RCC_CFGR2_PLL3MUL_SHIFT			12
#define RCC_CFGR2_PLL3MUL			(0xF << RCC_CFGR2_PLL3MUL_SHIFT)

#define RCC_CFGR2_PLL2MUL_SHIFT			8
#define RCC_CFGR2_PLL2MUL			(0xF << RCC_CFGR2_PLL2MUL_SHIFT)

#define RCC_CFGR2_PREDIV2_SHIFT			4
#define RCC_CFGR2_PREDIV2			(0xF << RCC_CFGR2_PREDIV2_SHIFT)

#define RCC_CFGR2_PREDIV1_SHIFT			0
#define RCC_CFGR2_PREDIV1			(0xF << RCC_CFGR2_PREDIV1_SHIFT)

/* PLL3MUL: PLL3 multiplication factor */
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL8		0x6
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL9		0x7
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL10	0x8
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL11	0x9
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL12	0xa
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL13	0xb
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL14	0xc
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL16	0xe
#define RCC_CFGR2_PLL3MUL_PLL3_CLK_MUL20	0xf

/* PLL2MUL: PLL2 multiplication factor */
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL8		0x6
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL9		0x7
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL10	0x8
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL11	0x9
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL12	0xa
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL13	0xb
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL14	0xc
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL16	0xe
#define RCC_CFGR2_PLL2MUL_PLL2_CLK_MUL20	0xf

/* PREDIV: PREDIV division factor */
#define RCC_CFGR2_PREDIV_NODIV			0x0
#define RCC_CFGR2_PREDIV_DIV2			0x1
#define RCC_CFGR2_PREDIV_DIV3			0x2
#define RCC_CFGR2_PREDIV_DIV4			0x3
#define RCC_CFGR2_PREDIV_DIV5			0x4
#define RCC_CFGR2_PREDIV_DIV6			0x5
#define RCC_CFGR2_PREDIV_DIV7			0x6
#define RCC_CFGR2_PREDIV_DIV8			0x7
#define RCC_CFGR2_PREDIV_DIV9			0x8
#define RCC_CFGR2_PREDIV_DIV10			0x9
#define RCC_CFGR2_PREDIV_DIV11			0xa
#define RCC_CFGR2_PREDIV_DIV12			0xb
#define RCC_CFGR2_PREDIV_DIV13			0xc
#define RCC_CFGR2_PREDIV_DIV14			0xd
#define RCC_CFGR2_PREDIV_DIV15			0xe
#define RCC_CFGR2_PREDIV_DIV16			0xf

/* PREDIV2: PREDIV2 division factor */
#define RCC_CFGR2_PREDIV2_NODIV			0x0
#define RCC_CFGR2_PREDIV2_DIV2			0x1
#define RCC_CFGR2_PREDIV2_DIV3			0x2
#define RCC_CFGR2_PREDIV2_DIV4			0x3
#define RCC_CFGR2_PREDIV2_DIV5			0x4
#define RCC_CFGR2_PREDIV2_DIV6			0x5
#define RCC_CFGR2_PREDIV2_DIV7			0x6
#define RCC_CFGR2_PREDIV2_DIV8			0x7
#define RCC_CFGR2_PREDIV2_DIV9			0x8
#define RCC_CFGR2_PREDIV2_DIV10			0x9
#define RCC_CFGR2_PREDIV2_DIV11			0xa
#define RCC_CFGR2_PREDIV2_DIV12			0xb
#define RCC_CFGR2_PREDIV2_DIV13			0xc
#define RCC_CFGR2_PREDIV2_DIV14			0xd
#define RCC_CFGR2_PREDIV2_DIV15			0xe
#define RCC_CFGR2_PREDIV2_DIV16			0xf

/* --- Variable definitions ------------------------------------------------ */
extern uint32_t rcc_ahb_frequency;
extern uint32_t rcc_apb1_frequency;
extern uint32_t rcc_apb2_frequency;

/* --- Function prototypes ------------------------------------------------- */

enum rcc_osc {
	RCC_PLL, RCC_PLL2, RCC_PLL3, RCC_HSE, RCC_HSI, RCC_LSE, RCC_LSI
};

#define _REG_BIT(base, bit)		(((base) << 5) + (bit))

/* V = value line F100
 * N = standard line F101, F102, F103
 * C = communication line F105, F107
 */
enum rcc_periph_clken {

	/* AHB peripherals */
	RCC_DMA1	= _REG_BIT(0x14, 0),/*VNC*/
	RCC_DMA2	= _REG_BIT(0x14, 1),/*VNC*/
	RCC_SRAM	= _REG_BIT(0x14, 2),/*VNC*/
	RCC_FLTF	= _REG_BIT(0x14, 4),/*VNC*/
	RCC_CRC		= _REG_BIT(0x14, 6),/*VNC*/
	RCC_FSMC	= _REG_BIT(0x14, 8),/*VN-*/
	RCC_SDIO	= _REG_BIT(0x14, 10),/*-N-*/
	RCC_OTGFS	= _REG_BIT(0x14, 12),/*--C*/
	RCC_ETHMAC	= _REG_BIT(0x14, 14),/*--C*/
	RCC_ETHMACTX	= _REG_BIT(0x14, 15),/*--C*/
	RCC_ETHMACRX	= _REG_BIT(0x14, 16),/*--C*/

	/* APB2 peripherals */
	RCC_AFIO	= _REG_BIT(0x18, 0),/*VNC*/
	RCC_GPIOA	= _REG_BIT(0x18, 2),/*VNC*/
	RCC_GPIOB	= _REG_BIT(0x18, 3),/*VNC*/
	RCC_GPIOC	= _REG_BIT(0x18, 4),/*VNC*/
	RCC_GPIOD	= _REG_BIT(0x18, 5),/*VNC*/
	RCC_GPIOE	= _REG_BIT(0x18, 6),/*VNC*/
	RCC_GPIOF	= _REG_BIT(0x18, 7),/*VN-*/
	RCC_GPIOG	= _REG_BIT(0x18, 8),/*VN-*/
	RCC_ADC1	= _REG_BIT(0x18, 9),/*VNC*/
	RCC_ADC2	= _REG_BIT(0x18, 10),/*-NC*/
	RCC_TIM1	= _REG_BIT(0x18, 11),/*VNC*/
	RCC_SPI1	= _REG_BIT(0x18, 12),/*VNC*/
	RCC_TIM8	= _REG_BIT(0x18, 13),/*-N-*/
	RCC_USART1	= _REG_BIT(0x18, 14),/*VNC*/
	RCC_ADC3	= _REG_BIT(0x18, 15),/*-N-*/
	RCC_TIM15	= _REG_BIT(0x18, 16),/*V--*/
	RCC_TIM16	= _REG_BIT(0x18, 17),/*V--*/
	RCC_TIM17	= _REG_BIT(0x18, 18),/*V--*/
	RCC_TIM9	= _REG_BIT(0x18, 19),/*-N-*/
	RCC_TIM10	= _REG_BIT(0x18, 20),/*-N-*/
	RCC_TIM11	= _REG_BIT(0x18, 21),/*-N-*/

	/* APB1 peripherals */
	RCC_TIM2	= _REG_BIT(0x1C, 0),/*VNC*/
	RCC_TIM3	= _REG_BIT(0x1C, 1),/*VNC*/
	RCC_TIM4	= _REG_BIT(0x1C, 2),/*VNC*/
	RCC_TIM5	= _REG_BIT(0x1C, 3),/*VNC*/
	RCC_TIM6	= _REG_BIT(0x1C, 4),/*VNC*/
	RCC_TIM7	= _REG_BIT(0x1C, 5),/*VNC*/
	RCC_TIM12	= _REG_BIT(0x1C, 6),/*VN-*/
	RCC_TIM13	= _REG_BIT(0x1C, 7),/*VN-*/
	RCC_TIM14	= _REG_BIT(0x1C, 8),/*VN-*/
	RCC_WWDG	= _REG_BIT(0x1C, 11),/*VNC*/
	RCC_SPI2	= _REG_BIT(0x1C, 14),/*VNC*/
	RCC_SPI3	= _REG_BIT(0x1C, 15),/*VNC*/
	RCC_USART2	= _REG_BIT(0x1C, 17),/*VNC*/
	RCC_USART3	= _REG_BIT(0x1C, 18),/*VNC*/
	RCC_UART4	= _REG_BIT(0x1C, 19),/*VNC*/
	RCC_UART5	= _REG_BIT(0x1C, 20),/*VNC*/
	RCC_I2C1	= _REG_BIT(0x1C, 21),/*VNC*/
	RCC_I2C2	= _REG_BIT(0x1C, 22),/*VNC*/
	RCC_USB		= _REG_BIT(0x1C, 23),/*-N-*/
	RCC_CAN		= _REG_BIT(0x1C, 25),/*-N-*/
	RCC_CAN1	= _REG_BIT(0x1C, 25),/*--C*/
	RCC_CAN2	= _REG_BIT(0x1C, 26),/*--C*/
	RCC_BKP		= _REG_BIT(0x1C, 27),/*VNC*/
	RCC_PWR		= _REG_BIT(0x1C, 28),/*VNC*/
	RCC_DAC		= _REG_BIT(0x1C, 29),/*VNC*/
	RCC_CEC		= _REG_BIT(0x1C, 30),/*V--*/
};

enum rcc_periph_rst {

	/* AHB peripherals */
	RST_OTGFS	= _REG_BIT(0x28, 12),/*--C*/
	RST_ETHMAC	= _REG_BIT(0x28, 14),/*--C*/

	/* APB2 peripherals */
	RST_AFIO	= _REG_BIT(0x0c, 0),/*VNC*/
	RST_GPIOA	= _REG_BIT(0x0c, 2),/*VNC*/
	RST_GPIOB	= _REG_BIT(0x0c, 3),/*VNC*/
	RST_GPIOC	= _REG_BIT(0x0c, 4),/*VNC*/
	RST_GPIOD	= _REG_BIT(0x0c, 5),/*VNC*/
	RST_GPIOE	= _REG_BIT(0x0c, 6),/*VNC*/
	RST_GPIOF	= _REG_BIT(0x0c, 7),/*VN-*/
	RST_GPIOG	= _REG_BIT(0x0c, 8),/*VN-*/
	RST_ADC1	= _REG_BIT(0x0c, 9),/*VNC*/
	RST_ADC2	= _REG_BIT(0x0c, 10),/*-NC*/
	RST_TIM1	= _REG_BIT(0x0c, 11),/*VNC*/
	RST_SPI1	= _REG_BIT(0x0c, 12),/*VNC*/
	RST_TIM8	= _REG_BIT(0x0c, 13),/*-N-*/
	RST_USART1	= _REG_BIT(0x0c, 14),/*VNC*/
	RST_ADC3	= _REG_BIT(0x0c, 15),/*-N-*/
	RST_TIM15	= _REG_BIT(0x0c, 16),/*V--*/
	RST_TIM16	= _REG_BIT(0x0c, 17),/*V--*/
	RST_TIM17	= _REG_BIT(0x0c, 18),/*V--*/
	RST_TIM9	= _REG_BIT(0x0c, 19),/*-N-*/
	RST_TIM10	= _REG_BIT(0x0c, 20),/*-N-*/
	RST_TIM11	= _REG_BIT(0x0c, 21),/*-N-*/

	/* APB1 peripherals */
	RST_TIM2	= _REG_BIT(0x10, 0),/*VNC*/
	RST_TIM3	= _REG_BIT(0x10, 1),/*VNC*/
	RST_TIM4	= _REG_BIT(0x10, 2),/*VNC*/
	RST_TIM5	= _REG_BIT(0x10, 3),/*VNC*/
	RST_TIM6	= _REG_BIT(0x10, 4),/*VNC*/
	RST_TIM7	= _REG_BIT(0x10, 5),/*VNC*/
	RST_TIM12	= _REG_BIT(0x10, 6),/*VN-*/
	RST_TIM13	= _REG_BIT(0x10, 7),/*VN-*/
	RST_TIM14	= _REG_BIT(0x10, 8),/*VN-*/
	RST_WWDG	= _REG_BIT(0x10, 11),/*VNC*/
	RST_SPI2	= _REG_BIT(0x10, 14),/*VNC*/
	RST_SPI3	= _REG_BIT(0x10, 15),/*VNC*/
	RST_USART2	= _REG_BIT(0x10, 17),/*VNC*/
	RST_USART3	= _REG_BIT(0x10, 18),/*VNC*/
	RST_UART4	= _REG_BIT(0x10, 19),/*VNC*/
	RST_UART5	= _REG_BIT(0x10, 20),/*VNC*/
	RST_I2C1	= _REG_BIT(0x10, 21),/*VNC*/
	RST_I2C2	= _REG_BIT(0x10, 22),/*VNC*/
	RST_USB		= _REG_BIT(0x10, 23),/*-N-*/
	RST_CAN		= _REG_BIT(0x10, 24),/*-N-*/
	RST_CAN1	= _REG_BIT(0x10, 24),/*--C*/
	RST_CAN2	= _REG_BIT(0x10, 25),/*--C*/
	RST_BKP		= _REG_BIT(0x10, 27),/*VNC*/
	RST_PWR		= _REG_BIT(0x10, 28),/*VNC*/
	RST_DAC		= _REG_BIT(0x10, 29),/*VNC*/
	RST_CEC		= _REG_BIT(0x10, 30),/*V--*/
};


BEGIN_DECLS

void rcc_osc_ready_int_clear(enum rcc_osc osc);
void rcc_osc_ready_int_enable(enum rcc_osc osc);
void rcc_osc_ready_int_disable(enum rcc_osc osc);
int rcc_osc_ready_int_flag(enum rcc_osc osc);
void rcc_css_int_clear(void);
int rcc_css_int_flag(void);
void rcc_wait_for_osc_ready(enum rcc_osc osc);
void rcc_osc_on(enum rcc_osc osc);
void rcc_osc_off(enum rcc_osc osc);
void rcc_css_enable(void);
void rcc_css_disable(void);
void rcc_osc_bypass_enable(enum rcc_osc osc);
void rcc_osc_bypass_disable(enum rcc_osc osc);
void rcc_set_sysclk_source(uint32_t clk);
void rcc_set_pll_multiplication_factor(uint32_t mul);
void rcc_set_pll2_multiplication_factor(uint32_t mul);
void rcc_set_pll3_multiplication_factor(uint32_t mul);
void rcc_set_pll_source(uint32_t pllsrc);
void rcc_set_pllxtpre(uint32_t pllxtpre);
uint32_t rcc_rtc_clock_enabled_flag(void);
void rcc_enable_rtc_clock(void);
void rcc_set_rtc_clock_source(enum rcc_osc clock_source);
void rcc_set_adcpre(uint32_t adcpre);
void rcc_set_ppre2(uint32_t ppre2);
void rcc_set_ppre1(uint32_t ppre1);
void rcc_set_hpre(uint32_t hpre);
void rcc_set_usbpre(uint32_t usbpre);
void rcc_set_prediv1(uint32_t prediv);
void rcc_set_prediv2(uint32_t prediv);
void rcc_set_prediv1_source(uint32_t rccsrc);
uint32_t rcc_system_clock_source(void);
void rcc_clock_setup_in_hsi_out_64mhz(void);
void rcc_clock_setup_in_hsi_out_48mhz(void);
void rcc_clock_setup_in_hsi_out_24mhz(void);
void rcc_clock_setup_in_hse_8mhz_out_24mhz(void);
void rcc_clock_setup_in_hse_8mhz_out_72mhz(void);
void rcc_clock_setup_in_hse_12mhz_out_72mhz(void);
void rcc_clock_setup_in_hse_16mhz_out_72mhz(void);
void rcc_clock_setup_in_hse_25mhz_out_72mhz(void);
void rcc_backupdomain_reset(void);

END_DECLS

#endif
/**@}*/

/* This provides unification of code over STM32F subfamilies */

/*
 * This file is part of the libopencm3 project.
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


#if defined(STM32F0)
#elif defined(STM32F1)
#elif defined(STM32F2)
#elif defined(STM32F3)
#elif defined(STM32F4)
#elif defined(STM32L0)
#elif defined(STM32L1)
#elif defined(STM32L4)
#else
#       error "stm32 family not defined."
#endif

/** @addtogroup rcc_defines
 *
 * @author @htmlonly &copy; @endhtmlonly 2013
 * Frantisek Burian <BuFran@seznam.cz>
 */
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Frantisek Burian <BuFran@seznam.cz>
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

/* THIS FILE SHOULD NOT BE INCLUDED DIRECTLY, BUT ONLY VIA RCC.H
 * The order of header inclusion is important. rcc.h defines the device
 * specific enumerations before including this header file.
 */

/** @cond */
#ifdef LIBOPENCM3_RCC_H
/** @endcond */

#ifndef LIBOPENCM3_RCC_COMMON_ALL_H
#define LIBOPENCM3_RCC_COMMON_ALL_H

/**@{*/

BEGIN_DECLS

void rcc_peripheral_enable_clock(volatile uint32_t *reg, uint32_t en);
void rcc_peripheral_disable_clock(volatile uint32_t *reg, uint32_t en);
void rcc_peripheral_reset(volatile uint32_t *reg, uint32_t reset);
void rcc_peripheral_clear_reset(volatile uint32_t *reg, uint32_t clear_reset);

void rcc_periph_clock_enable(enum rcc_periph_clken clken);
void rcc_periph_clock_disable(enum rcc_periph_clken clken);
void rcc_periph_reset_pulse(enum rcc_periph_rst rst);
void rcc_periph_reset_hold(enum rcc_periph_rst rst);
void rcc_periph_reset_release(enum rcc_periph_rst rst);

void rcc_set_mco(uint32_t mcosrc);

END_DECLS
/**@}*/

#endif
/** @cond */
#else
#endif
/** @endcond */

/** @defgroup usb_defines USB Defines

@brief <b>Defined Constants and Types for the STM32F* USB drivers</b>

@ingroup STM32Fx_defines

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2009
Piotr Esden-Tempski <piotr@esden.net>

@date 11 March 2013

LGPL License Terms @ref lgpl_license
 */
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Piotr Esden-Tempski <piotr@esden.net>
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

/* THIS FILE SHOULD NOT BE INCLUDED DIRECTLY !
 * Use top-level <libopencm3/stm32/st_usbfs.h>
 */

/**@{*/

/** @cond */
#ifdef LIBOPENCM3_ST_USBFS_H
/** @endcond */
#ifndef LIBOPENCM3_ST_USBFS_COMMON_H
#define LIBOPENCM3_ST_USBFS_COMMON_H


/*****************************************************************************/
/* Module definitions                                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Register definitions                                                      */
/*****************************************************************************/

/* --- USB general registers ----------------------------------------------- */

/* USB Control register */
#define USB_CNTR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x40))
/* USB Interrupt status register */
#define USB_ISTR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x44))
/* USB Frame number register */
#define USB_FNR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x48))
/* USB Device address register */
#define USB_DADDR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x4C))
/* USB Buffer table address register */
#define USB_BTABLE_REG		(&MMIO32(USB_DEV_FS_BASE + 0x50))

/* USB EP register */
#define USB_EP_REG(EP)		(&MMIO32(USB_DEV_FS_BASE) + (EP))


/*****************************************************************************/
/* Register values                                                           */
/*****************************************************************************/

/* --- USB control register masks / bits ----------------------------------- */

/* Interrupt mask bits, set to 1 to enable interrupt generation */
#define USB_CNTR_CTRM		0x8000
#define USB_CNTR_PMAOVRM	0x4000
#define USB_CNTR_ERRM		0x2000
#define USB_CNTR_WKUPM		0x1000
#define USB_CNTR_SUSPM		0x0800
#define USB_CNTR_RESETM		0x0400
#define USB_CNTR_SOFM		0x0200
#define USB_CNTR_ESOFM		0x0100

/* Request/Force bits */
#define USB_CNTR_RESUME		0x0010 /* Resume request */
#define USB_CNTR_FSUSP		0x0008 /* Force suspend */
#define USB_CNTR_LP_MODE	0x0004 /* Low-power mode */
#define USB_CNTR_PWDN		0x0002 /* Power down */
#define USB_CNTR_FRES		0x0001 /* Force reset */

/* --- USB interrupt status register masks / bits -------------------------- */

#define USB_ISTR_CTR		0x8000 /* Correct Transfer */
#define USB_ISTR_PMAOVR		0x4000 /* Packet Memory Area Over / Underrun */
#define USB_ISTR_ERR		0x2000 /* Error */
#define USB_ISTR_WKUP		0x1000 /* Wake up */
#define USB_ISTR_SUSP		0x0800 /* Suspend mode request */
#define USB_ISTR_RESET		0x0400 /* USB RESET request */
#define USB_ISTR_SOF		0x0200 /* Start Of Frame */
#define USB_ISTR_ESOF		0x0100 /* Expected Start Of Frame */
#define USB_ISTR_DIR		0x0010 /* Direction of transaction */
#define USB_ISTR_EP_ID		0x000F /* Endpoint Identifier */

/* --- USB interrupt status register manipulators -------------------------- */

/* Note: CTR is read only! */
#define USB_CLR_ISTR_PMAOVR()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_PMAOVR)
#define USB_CLR_ISTR_ERR()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_ERR)
#define USB_CLR_ISTR_WKUP()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_WKUP)
#define USB_CLR_ISTR_SUSP()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_SUSP)
#define USB_CLR_ISTR_RESET()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_RESET)
#define USB_CLR_ISTR_SOF()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_SOF)
#define USB_CLR_ISTR_ESOF()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_ESOF)

/* --- USB Frame Number Register bits -------------------------------------- */

#define USB_FNR_RXDP		(1 << 15)
#define USB_FNR_RXDM		(1 << 14)
#define USB_FNR_LCK		(1 << 13)

#define USB_FNR_LSOF_SHIFT	11
#define USB_FNR_LSOF		(3 << USB_FNR_LSOF_SHIFT)

#define USB_FNR_FN		(0x7FF << 0)

/* --- USB device address register masks / bits ---------------------------- */

#define USB_DADDR_EF		(1 << 7)
#define USB_DADDR_ADDR		0x007F

/* USB_BTABLE Values ------------------------------------------------------- */

#define USB_BTABLE_BTABLE	0xFFF8

/* --- USB device address register manipulators ---------------------------- */

/* --- USB endpoint register offsets --------------------------------------- */

#define USB_EP0			0
#define USB_EP1			1
#define USB_EP2			2
#define USB_EP3			3
#define USB_EP4			4
#define USB_EP5			5
#define USB_EP6			6
#define USB_EP7			7

/* --- USB endpoint register masks / bits ---------------------------------- */

/* Masks and toggle bits */
#define USB_EP_RX_CTR		0x8000 /* Correct transfer RX */
#define USB_EP_RX_DTOG		0x4000 /* Data toggle RX */
#define USB_EP_RX_STAT		0x3000 /* Endpoint status for RX */

#define USB_EP_SETUP		0x0800 /* Setup transaction completed */
#define USB_EP_TYPE		0x0600 /* Endpoint type */
#define USB_EP_KIND		0x0100 /* Endpoint kind.
				* When set and type=bulk    -> double buffer
				* When set and type=control -> status out
				*/

#define USB_EP_TX_CTR		0x0080 /* Correct transfer TX */
#define USB_EP_TX_DTOG		0x0040 /* Data toggle TX */
#define USB_EP_TX_STAT		0x0030 /* Endpoint status for TX */

#define USB_EP_ADDR		0x000F /* Endpoint Address */

/* Masking all toggle bits */
#define USB_EP_NTOGGLE_MSK	(USB_EP_RX_CTR | \
				 USB_EP_SETUP | \
				 USB_EP_TYPE | \
				 USB_EP_KIND | \
				 USB_EP_TX_CTR | \
				 USB_EP_ADDR)

/* All non toggle bits plus EP_RX toggle bits */
#define USB_EP_RX_STAT_TOG_MSK	(USB_EP_RX_STAT | USB_EP_NTOGGLE_MSK)
/* All non toggle bits plus EP_TX toggle bits */
#define USB_EP_TX_STAT_TOG_MSK	(USB_EP_TX_STAT | USB_EP_NTOGGLE_MSK)

/* Endpoint status bits for USB_EP_RX_STAT bit field */
#define USB_EP_RX_STAT_DISABLED	0x0000
#define USB_EP_RX_STAT_STALL	0x1000
#define USB_EP_RX_STAT_NAK	0x2000
#define USB_EP_RX_STAT_VALID	0x3000

/* Endpoint status bits for USB_EP_TX_STAT bit field */
#define USB_EP_TX_STAT_DISABLED	0x0000
#define USB_EP_TX_STAT_STALL	0x0010
#define USB_EP_TX_STAT_NAK	0x0020
#define USB_EP_TX_STAT_VALID	0x0030

/* Endpoint type bits for USB_EP_TYPE bit field */
#define USB_EP_TYPE_BULK	0x0000
#define USB_EP_TYPE_CONTROL	0x0200
#define USB_EP_TYPE_ISO		0x0400
#define USB_EP_TYPE_INTERRUPT	0x0600

/* --- USB endpoint register manipulators ---------------------------------- */

/*
 * Set USB endpoint tx/rx status.
 *
 * USB status field is changed using an awkward toggle mechanism, that
 * is why we use some helper macros for that.
 */
#define USB_SET_EP_RX_STAT(EP, STAT) \
	TOG_SET_REG_BIT_MSK_AND_SET(USB_EP_REG(EP), \
		USB_EP_RX_STAT_TOG_MSK, STAT, USB_EP_RX_CTR | USB_EP_TX_CTR)

#define USB_SET_EP_TX_STAT(EP, STAT) \
	TOG_SET_REG_BIT_MSK_AND_SET(USB_EP_REG(EP), \
		USB_EP_TX_STAT_TOG_MSK, STAT, USB_EP_RX_CTR | USB_EP_TX_CTR)

/*
 * Macros for clearing and setting USB endpoint register bits that do
 * not use the toggle mechanism.
 *
 * Because the register contains some bits that use the toggle
 * mechanism we need a helper macro here. Otherwise the code gets really messy.
 */
#define USB_CLR_EP_NTOGGLE_BIT_AND_SET(EP, BIT, EXTRA_BITS) \
	CLR_REG_BIT_MSK_AND_SET(USB_EP_REG(EP), \
		USB_EP_NTOGGLE_MSK, BIT, EXTRA_BITS)

#define USB_CLR_EP_RX_CTR(EP) \
	USB_CLR_EP_NTOGGLE_BIT_AND_SET(EP, USB_EP_RX_CTR, USB_EP_TX_CTR)

#define USB_CLR_EP_TX_CTR(EP) \
	USB_CLR_EP_NTOGGLE_BIT_AND_SET(EP, USB_EP_TX_CTR, USB_EP_RX_CTR)


#define USB_SET_EP_TYPE(EP, TYPE) \
	SET_REG(USB_EP_REG(EP), \
		(GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & \
		(~USB_EP_TYPE))) | TYPE)

#define USB_SET_EP_KIND(EP) \
	SET_REG(USB_EP_REG(EP), \
		(GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & \
		(~USB_EP_KIND))) | USB_EP_KIND)

#define USB_CLR_EP_KIND(EP) \
	SET_REG(USB_EP_REG(EP), \
		(GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & (~USB_EP_KIND))))

#define USB_SET_EP_STAT_OUT(EP)	USB_SET_EP_KIND(EP)
#define USB_CLR_EP_STAT_OUT(EP)	USB_CLR_EP_KIND(EP)

#define USB_SET_EP_ADDR(EP, ADDR) \
	SET_REG(USB_EP_REG(EP), \
		((GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & \
		(~USB_EP_ADDR))) | ADDR))

/* Macros for clearing DTOG bits */
#define USB_CLR_EP_TX_DTOG(EP) \
	SET_REG(USB_EP_REG(EP), \
		GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK | USB_EP_TX_DTOG))

#define USB_CLR_EP_RX_DTOG(EP) \
	SET_REG(USB_EP_REG(EP), \
		GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK | USB_EP_RX_DTOG))


/* --- USB BTABLE registers ------------------------------------------------ */

#define USB_GET_BTABLE		GET_REG(USB_BTABLE_REG)

/* --- USB BTABLE manipulators --------------------------------------------- */

#define USB_GET_EP_TX_ADDR(EP)		GET_REG(USB_EP_TX_ADDR(EP))
#define USB_GET_EP_TX_COUNT(EP)		GET_REG(USB_EP_TX_COUNT(EP))
#define USB_GET_EP_RX_ADDR(EP)		GET_REG(USB_EP_RX_ADDR(EP))
#define USB_GET_EP_RX_COUNT(EP)		GET_REG(USB_EP_RX_COUNT(EP))
#define USB_SET_EP_TX_ADDR(EP, ADDR)	SET_REG(USB_EP_TX_ADDR(EP), ADDR)
#define USB_SET_EP_TX_COUNT(EP, COUNT)	SET_REG(USB_EP_TX_COUNT(EP), COUNT)
#define USB_SET_EP_RX_ADDR(EP, ADDR)	SET_REG(USB_EP_RX_ADDR(EP), ADDR)
#define USB_SET_EP_RX_COUNT(EP, COUNT)	SET_REG(USB_EP_RX_COUNT(EP), COUNT)



/**@}*/

#endif
/** @cond */
#else
#endif
/** @endcond */

/** @addtogroup usb_defines
 */
/*
 * This file is part of the libopencm3 project.
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

/* THIS FILE SHOULD NOT BE INCLUDED DIRECTLY !
 * Use top-level <libopencm3/stm32/st_usbfs.h>
 *
 * Additional definitions for F1, F3, L1 devices:
 * -F102, F103 (RM0008)
 * -F302x{B,C}; *NOT* F302x{6,8,D,E} !! (USB_BTABLE access issues) (RM0365)
 * -F303x{B,C}; *NOT* F303x{D,E} !! (USB_BTABLE access issues) (RM0316)
 * -F37x (RM0313)
 * -L1xx (RM0038)
 */

/** @cond */
#ifdef LIBOPENCM3_ST_USBFS_H
/** @endcond */
#ifndef LIBOPENCM3_ST_USBFS_V1_H
#define LIBOPENCM3_ST_USBFS_V1_H


/* --- USB BTABLE Registers ------------------------------------------------ */

#define USB_EP_TX_ADDR(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 0) * 2))

#define USB_EP_TX_COUNT(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 2) * 2))

#define USB_EP_RX_ADDR(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 4) * 2))

#define USB_EP_RX_COUNT(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 6) * 2))

/* --- USB BTABLE manipulators --------------------------------------------- */

#define USB_GET_EP_TX_BUFF(EP) \
	(USB_PMA_BASE + (uint8_t *)(USB_GET_EP_TX_ADDR(EP) * 2))

#define USB_GET_EP_RX_BUFF(EP) \
	(USB_PMA_BASE + (uint8_t *)(USB_GET_EP_RX_ADDR(EP) * 2))

#endif
/** @cond */
#else
#endif
/** @endcond */

/** @defgroup usb_private_defines USB Private Structures

@brief <b>Defined Constants and Types for the USB Private Structures</b>

@ingroup USB_defines

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

#ifndef __USB_PRIVATE_H
#define __USB_PRIVATE_H

#define MAX_USER_CONTROL_CALLBACK	4
#define MAX_USER_SET_CONFIG_CALLBACK	4

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/** Internal collection of device information. */
struct _usbd_device {
	const struct usb_device_descriptor *desc;
	const struct usb_config_descriptor *config;
    const struct usb_bos_descriptor *bos;
	const char **strings;
	int num_strings;

	uint8_t *ctrl_buf;  /**< Internal buffer used for control transfers */
	uint16_t ctrl_buf_len;

	uint8_t current_address;
	uint8_t current_config;

	uint16_t pm_top;    /**< Top of allocated endpoint buffer memory */

	/* User callback functions for various USB events */
	void (*user_callback_reset)(void);
	void (*user_callback_suspend)(void);
	void (*user_callback_resume)(void);
	void (*user_callback_sof)(void);

	struct usb_control_state {
		enum {
			IDLE, STALLED,
			DATA_IN, LAST_DATA_IN, STATUS_IN,
			DATA_OUT, LAST_DATA_OUT, STATUS_OUT,
		} state;
		struct usb_setup_data req __attribute__((aligned(4)));
		uint8_t *ctrl_buf;
		uint16_t ctrl_len;
		usbd_control_complete_callback complete;
		bool needs_zlp;
	} control_state;

	struct user_control_callback {
		usbd_control_callback cb;
		uint8_t type;
		uint8_t type_mask;
	} user_control_callback[MAX_USER_CONTROL_CALLBACK];

	usbd_endpoint_callback user_callback_ctr[8][3];

	/* User callback function for some standard USB function hooks */
	usbd_set_config_callback user_callback_set_config[MAX_USER_SET_CONFIG_CALLBACK];

	usbd_set_altsetting_callback user_callback_set_altsetting;

	const struct _usbd_driver *driver;

	/* Extra, non-contiguous user string descriptor index and value */
	int extra_string_idx;
	const char* extra_string;

	/* private driver data */

	uint16_t fifo_mem_top;
	uint16_t fifo_mem_top_ep0;
	uint8_t force_nak[4];
	/*
	 * We keep a backup copy of the out endpoint size registers to restore
	 * them after a transaction.
	 */
	uint32_t doeptsiz[4];
	/*
	 * Received packet size for each endpoint. This is assigned in
	 * stm32f107_poll() which reads the packet status push register GRXSTSP
	 * for use in stm32f107_ep_read_packet().
	 */
	uint16_t rxbcnt;
};

enum _usbd_transaction {
	USB_TRANSACTION_IN,
	USB_TRANSACTION_OUT,
	USB_TRANSACTION_SETUP,
};

/* Do not appear to belong to the API, so are omitted from docs */
/**@}*/

void _usbd_control_in(usbd_device *usbd_dev, uint8_t ea);
void _usbd_control_out(usbd_device *usbd_dev, uint8_t ea);
void _usbd_control_setup(usbd_device *usbd_dev, uint8_t ea);

int _usbd_standard_request_device(usbd_device *usbd_dev,
				  struct usb_setup_data *req, uint8_t **buf,
				  uint16_t *len);
int _usbd_standard_request_interface(usbd_device *usbd_dev,
				     struct usb_setup_data *req, uint8_t **buf,
				     uint16_t *len);
int _usbd_standard_request_endpoint(usbd_device *usbd_dev,
				    struct usb_setup_data *req, uint8_t **buf,
				    uint16_t *len);
int _usbd_standard_request(usbd_device *usbd_dev, struct usb_setup_data *req,
			   uint8_t **buf, uint16_t *len);

void _usbd_reset(usbd_device *usbd_dev);

/* Functions provided by the hardware abstraction. */
struct _usbd_driver {
	usbd_device *(*init)(void);
	void (*set_address)(usbd_device *usbd_dev, uint8_t addr);
	void (*ep_setup)(usbd_device *usbd_dev, uint8_t addr, uint8_t type,
			 uint16_t max_size, usbd_endpoint_callback cb);
	void (*ep_reset)(usbd_device *usbd_dev);
	void (*ep_stall_set)(usbd_device *usbd_dev, uint8_t addr,
			     uint8_t stall);
	void (*ep_nak_set)(usbd_device *usbd_dev, uint8_t addr, uint8_t nak);
	uint8_t (*ep_stall_get)(usbd_device *usbd_dev, uint8_t addr);
	uint16_t (*ep_write_packet)(usbd_device *usbd_dev, uint8_t addr,
				    const void *buf, uint16_t len);
	uint16_t (*ep_read_packet)(usbd_device *usbd_dev, uint8_t addr,
				   void *buf, uint16_t len);
	void (*poll)(usbd_device *usbd_dev);
	void (*disconnect)(usbd_device *usbd_dev, bool disconnected);
	uint32_t base_address;
	bool set_address_before_status;
	uint16_t rx_fifo_size;
};

#endif

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

/*
 * This is a "private" header file for usbd implementations.
 * As opposed to the "public" headers under include that describe the hardware,
 * this is purely an implementation detail of usbd drivers.
 */

#ifndef ST_USBFS_CORE
#define ST_USBFS_CORE


#define USBD_PM_TOP 0x40

void st_usbfs_set_address(usbd_device *dev, uint8_t addr);
void st_usbfs_set_ep_rx_bufsize(usbd_device *dev, uint8_t ep, uint32_t size);

void st_usbfs_ep_setup(usbd_device *usbd_dev, uint8_t addr,
		uint8_t type, uint16_t max_size,
		void (*callback) (usbd_device *usbd_dev,
		uint8_t ep));

void st_usbfs_endpoints_reset(usbd_device *usbd_dev);
void st_usbfs_ep_stall_set(usbd_device *usbd_dev, uint8_t addr, uint8_t stall);
uint8_t st_usbfs_ep_stall_get(usbd_device *usbd_dev, uint8_t addr);
void st_usbfs_ep_nak_set(usbd_device *usbd_dev, uint8_t addr, uint8_t nak);
uint16_t st_usbfs_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
				  const void *buf, uint16_t len);
uint16_t st_usbfs_ep_read_packet(usbd_device *usbd_dev, uint8_t addr,
				 void *buf, uint16_t len);
void st_usbfs_poll(usbd_device *usbd_dev);

/* These must be implemented by the device specific driver */

/**
 * Copy a data buffer to packet memory.
 *
 * @param vPM Destination pointer into packet memory.
 * @param buf Source pointer to data buffer.
 * @param len Number of bytes to copy.
 */
void st_usbfs_copy_from_pm(void *buf, const volatile void *vPM, uint16_t len);

/**
 * Copy a data buffer from packet memory.
 *
 * @param vPM Destination pointer into packet memory.
 * @param buf Source pointer to data buffer.
 * @param len Number of bytes to copy.
 */
void st_usbfs_copy_to_pm(volatile void *vPM, const void *buf, uint16_t len);

extern uint8_t st_usbfs_force_nak[8];
extern struct _usbd_device st_usbfs_dev;

#endif
