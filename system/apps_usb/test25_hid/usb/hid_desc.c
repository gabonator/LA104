/**
 ******************************************************************************
 * @file	usb_desc.c
 * @author	MCD Application Team
 * @version V4.0.0
 * @date	21-January-2013
 * @brief	Descriptors for Virtual Com Port Demo
 ******************************************************************************
 **/
#include <usb_lib.h>
#include "hid_desc.h"
#include "hid_app.h"

const uint8_t ARC_device_descriptor[ARC_SIZE_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
#ifndef CONFIG_ANNOYATRON
    0xd6,                       /*idVendor (0xfede)*/
    0xf6,
    0xda,                       /*idProduct = 0xbeda*/
    0xbe,
#else
    0x87,                       /*idVendor (0xfede)*/
    0x80,
    0x01,                       /*idProduct = 0xbeda*/
    0x80,
#endif
    0x30,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Device Descriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t ARC_config_descriptor[ARC_SIZE_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    ARC_SIZE_CONFIG_DESC,               /* wTotalLength: Bytes returned */
    0x00,
#ifndef CONFIG_ANNOYATRON
#ifdef CONFIG_ARCHID_VCD
/*P*/    0x06,         /*bNumInterfaces: nbr of interfaces */
#else
/*P*/    0x04,         /*bNumInterfaces: nbr of interfaces */
#endif
#else  // CONFIG_ANNOYATRON
/*P*/    0x02,         /*bNumInterfaces: nbr of interfaces */
#endif  // CONFIG_ANNOYATRON
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
    0xE0,         /*bmAttributes: Self powered */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** ifc 1:KEYBOARD          ****************/
    /************** Descriptor of interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    ARC_KB_SIZE_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x40,          /*wMaxPacketSize: 64 bytes max ///8 Byte max */
    0x00,
    24,          /*bInterval: Polling Interval (24 ms)*/
    /* 34 */

    /************** ifc 2:MOUSE             ****************/
    /************** Descriptor of interface ****************/
    /* 34 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x01,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 43 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x00,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    ARC_MOUSE_SIZE_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of endpoint ********************/
    /* 52 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x82,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x04,          /*wMaxPacketSize: 4 bytes max */
    0x00,
    1,          /*bInterval: Polling Interval (1 ms)*/
    /* 59 */

    /*P*/
#ifndef CONFIG_ANNOYATRON
    /************** ifc 3:JOYSTICK1         ****************/
    /************** Descriptor of interface ****************/

    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x02,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 68 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x10,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    ARC_JOYSTICK_SIZE_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of endpoint ********************/
    /* 77 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x83,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 4 bytes max */
    0x00,
    10,          /*bInterval: Polling Interval (10 ms)*/
    /* 84 */

    /************** ifc 4:JOYSTICK2         ****************/
    /************** Descriptor of interface ****************/

    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x03,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 93 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x10,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    ARC_JOYSTICK_SIZE_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of endpoint ********************/
    /* 102 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x84,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 4 bytes max */
    0x00,
    10,          /*bInterval: Polling Interval (10 ms)*/
    /* 109 */
    /*P*/

#ifdef CONFIG_ARCHID_VCD
    /*P*/
    /************** ifc 4:VCD                ****************/
    /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    /*P*/0x04,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */
    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,
    /*Call Management Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    /*P*/0x05,   /* bDataInterface: 5 *////////////0x01,   /* bDataInterface: 1 */
    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */
    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    /*P*/0x04,   /* bMasterInterface: Communication class interface *////////////0x00,   /* bMasterInterface: Communication class interface */
    /*P*/0x05,   /* bSlaveInterface0: Data Class Interface *//////////0x01,   /* bSlaveInterface0: Data Class Interface */
    /*Endpoint 5 Descriptor (was 2) IN */
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x85,   /* bEndpointAddress: (IN5) */
    0x03,   /* bmAttributes: Interrupt */
    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */
    0x00,
    0xFF,   /* bInterval: */
    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    /*P*/0x05,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */
    /*Endpoint 6 Descriptor (was 3) OUT */
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x06,   /* bEndpointAddress: (OUT6) */
    0x02,   /* bmAttributes: Bulk */
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */
    /*Endpoint 7 Descriptor (was 1) IN */
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x87,   /* bEndpointAddress: (IN7) */
    0x02,   /* bmAttributes: Bulk */
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
    0x00,
    0x00    /* bInterval */

#endif
#endif // CONFIG_ANNOYATRON
}; /* config descriptor */

const uint8_t ARC_KB_report_descriptor[ARC_KB_SIZE_REPORT_DESC] =
  {
      0x05, 0x01,                         // Usage Page (Generic Desktop)
      0x09, 0x06,                         // Usage (Keyboard)
      0xA1, 0x01,                         // Collection (Application)
      0x05, 0x07,                         //     Usage Page (Key Codes)
      0x19, 0xe0,                         //     Usage Minimum (224)
      0x29, 0xe7,                         //     Usage Maximum (231)
      0x15, 0x00,                         //     Logical Minimum (0)
      0x25, 0x01,                         //     Logical Maximum (1)

      0x75, 0x01,                         //     Report Size (1)
      0x95, 0x08,                         //     Report Count (8)
      0x81, 0x02,                         //     Input (Data, Variable, Absolute)

      0x95, 0x01,                         //     Report Count (1)
      0x75, 0x08,                         //     Report Size (8)
      0x81, 0x01,                         //     Input (Constant) reserved byte(1)

      0x95, USB_KB_REPORT_KEYMAP_SIZE,            //     Report Count (normally 6)
      0x75, 0x08,                         //     Report Size (8)
      0x26, 0xff, 0x00,
      0x05, 0x07,                         //     Usage Page (Key codes)
      0x19, 0x00,                         //     Usage Minimum (0)
      0x29, 0xbc,                         //     Usage Maximum (188)
      0x81, 0x00,                         //     Input (Data, Array) Key array(6 bytes)

      0x95, 0x03,                         //     Report Count (3)
      0x75, 0x01,                         //     Report Size (1)
      0x05, 0x08,                         //     Usage Page (Page# for LEDs)
      0x19, 0x01,                         //     Usage Minimum (1)
      0x29, 0x03,                         //     Usage Maximum (3)
      0x91, 0x02,                         //     Output (Data, Variable, Absolute), Led report
      0x95, 0x05,                         //     Report Count (5)
      0x75, 0x01,                         //     Report Size (1)
      0x91, 0x01,                         //     Output (Data, Variable, Absolute), Led report padding

      0xC0                                // End Collection (Application)

  };

const uint8_t ARC_MOUSE_report_descriptor[ARC_MOUSE_SIZE_REPORT_DESC] =
  {
      0x05,          /*Usage Page(Generic Desktop)*/
      0x01,
      0x09,          /*Usage(Mouse)*/
      0x02,
      0xA1,          /*Collection(Logical)*/
      0x01,
      0x09,          /*Usage(Pointer)*/
      0x01,
      /* 8 */
      0xA1,          /*Collection(Linked)*/
      0x00,
      0x05,          /*Usage Page(Buttons)*/
      0x09,
      0x19,          /*Usage Minimum(1)*/
      0x01,
      0x29,          /*Usage Maximum(3)*/
      0x03,
      /* 16 */
      0x15,          /*Logical Minimum(0)*/
      0x00,
      0x25,          /*Logical Maximum(1)*/
      0x01,
      0x95,          /*Report Count(3)*/
      0x03,
      0x75,          /*Report Size(1)*/
      0x01,
      /* 24 */
      0x81,          /*Input(Variable)*/
      0x02,
      0x95,          /*Report Count(1)*/
      0x01,
      0x75,          /*Report Size(5)*/
      0x05,
      0x81,          /*Input(Constant,Array)*/
      0x01,
      /* 32 */
      0x05,          /*Usage Page(Generic Desktop)*/
      0x01,
      0x09,          /*Usage(X axis)*/
      0x30,
      0x09,          /*Usage(Y axis)*/
      0x31,
      0x09,          /*Usage(Wheel)*/
      0x38,
      /* 40 */
      0x15,          /*Logical Minimum(-127)*/
      0x81,
      0x25,          /*Logical Maximum(127)*/
      0x7F,
      0x75,          /*Report Size(8)*/
      0x08,
      0x95,          /*Report Count(3)*/
      0x03,
      /* 48 */
      0x81,          /*Input(Variable, Relative)*/
      0x06,
      0xC0,          /*End Collection*/
      0x09,
      0x3c,
      0x05,
      0xff,
      0x09,
      /* 56 */
      0x01,
      0x15,
      0x00,
      0x25,
      0x01,
      0x75,
      0x01,
      0x95,
      /* 64 */
      0x02,
      0xb1,
      0x22,
      0x75,
      0x06,
      0x95,
      0x01,
      0xb1,
      /* 72 */
      0x01,
      0xc0

  };

const uint8_t ARC_JOYSTICK_report_descriptor[ARC_JOYSTICK_SIZE_REPORT_DESC] =
  {
      0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
      0x09, 0x04,                    // USAGE (Joystick)
      0xa1, 0x01,                    // COLLECTION (Application)
      0x15, 0x81,                    //   LOGICAL_MINIMUM (-127)
      0x25, 0x7f,                    //   LOGICAL_MAXIMUM (127)
      0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
      0x09, 0x01,                    //   USAGE (Pointer)
      0xa1, 0x00,                    //   COLLECTION (Physical)
      0x09, 0x30,                    //     USAGE (X)
      0x09, 0x31,                    //     USAGE (Y)
      0x75, 0x08,                    //     REPORT_SIZE (8)
      0x95, 0x02,                    //     REPORT_COUNT (2)
      0x81, 0x02,                    //     INPUT (Data,Var,Abs)
      0xc0,                          //   END_COLLECTION
      0x05, 0x09,                    //   USAGE_PAGE (Button)
      0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
      0x29, 0x08,                    //   USAGE_MAXIMUM (Button 8)
      0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
      0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
      0x75, 0x01,                    //   REPORT_SIZE (1)
      0x95, 0x10,                    //   REPORT_COUNT (16)
      0x55, 0x00,                    //   UNIT_EXPONENT (0)
      0x65, 0x00,                    //   UNIT (None)
      0x81, 0x02,                    //   INPUT (Data,Var,Abs)
      0xc0                           // END_COLLECTION
      };

/* USB String Descriptors (optional) */
const uint8_t ARC_string_lang_ID[ARC_SIZE_STRING_LANGID] =
  {
    ARC_SIZE_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t ARC_string_vendor[ARC_SIZE_STRING_VENDOR] =
  {
    ARC_SIZE_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
#ifndef CONFIG_ANNOYATRON
    'p', 0, 'e', 0, 'l', 0, 'l', 0, 'e', 0, 'p', 0, 'l', 0,
    'u', 0, 't', 0, 't', 0, '.', 0, 'c', 0, 'o', 0, 'm', 0
#else
    'I', 0, 'n', 0, 't', 0, 'e', 0, 'l', 0, ' ', 0, 'C', 0,
    'o', 0, 'r', 0, 'p', 0, ' ', 0, 'L', 0, 'T', 0, 'D', 0
#endif
  };

const uint8_t ARC_string_product[ARC_SIZE_STRING_PRODUCT] =
  {
    ARC_SIZE_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
#ifndef CONFIG_ANNOYATRON
    'A', 0, 'r', 0, 'c', 0, 'a', 0, 'd', 0, 'e', 0, '-', 0,
    'c', 0, 'o', 0, 'n', 0, 't', 0, 'r', 0, 'o', 0, 'l', 0,
#else
    'U', 0, 'S', 0, 'B', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0,
    't', 0, 'r', 0, 'o', 0, 'l', 0, 'H', 0, 'U', 0, 'B', 0,
#endif
  };
uint8_t ARC_string_serial[ARC_SIZE_STRING_SERIAL] =
  {
    ARC_SIZE_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'p', 0, 'e', 0, 'l', 0, 'l', 0, 'e', 0
  };

