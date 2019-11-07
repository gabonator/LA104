/**
  ******************************************************************************
  * @file    usb_desc.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptor Header for Virtual COM Port Device
  ******************************************************************************
  */

#ifndef __USB_DESC_H
#define __USB_DESC_H

#include <stdint.h>

#define USB_DEVICE_DESCRIPTOR_TYPE			0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE	0x02
#define USB_STRING_DESCRIPTOR_TYPE			0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE		0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE		0x05

#define VIRTUAL_COM_PORT_DATA_SIZE			64
#define VIRTUAL_COM_PORT_INT_SIZE			8

#define VIRTUAL_COM_PORT_SIZ_DEVICE_DESC	18
#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC	67
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID	4
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR	38
#define VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT	50
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL	20

extern const uint8_t CDC_DeviceDescriptor [VIRTUAL_COM_PORT_SIZ_DEVICE_DESC];
extern const uint8_t CDC_ConfigDescriptor [VIRTUAL_COM_PORT_SIZ_CONFIG_DESC];

extern const uint8_t CDC_StringLangID [VIRTUAL_COM_PORT_SIZ_STRING_LANGID];
extern const uint8_t CDC_StringVendor [VIRTUAL_COM_PORT_SIZ_STRING_VENDOR];
extern const uint8_t CDC_StringProduct [VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT];
extern       uint8_t CDC_StringSerial [VIRTUAL_COM_PORT_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */