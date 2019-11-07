/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

#define CONFIG_ANNOYATRON

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define ARC_SIZE_HID_DESC                   0x09
#define ARC_OFFS_HID_DESC                   0x12

#define ARC_SIZE_DEVICE_DESC                18
#ifndef CONFIG_ANNOYATRON
#ifdef CONFIG_ARCHID_VCD
#define ARC_SIZE_CONFIG_DESC                59+50+58
#else
#define ARC_SIZE_CONFIG_DESC                59+50
#endif
#else // CONFIG_ANNOYATRON
#define ARC_SIZE_CONFIG_DESC                59
#endif // CONFIG_ANNOYATRON
#define ARC_KB_SIZE_REPORT_DESC             62
#define ARC_MOUSE_SIZE_REPORT_DESC          74
#define ARC_JOYSTICK_SIZE_REPORT_DESC       48
#define ARC_SIZE_STRING_LANGID              4
#define ARC_SIZE_STRING_VENDOR              30
#define ARC_SIZE_STRING_PRODUCT             31
#define ARC_SIZE_STRING_SERIAL              12

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#ifdef CONFIG_ARCHID_VCD

#define VIRTUAL_COM_PORT_DATA_SIZE              64
#define VIRTUAL_COM_PORT_INT_SIZE               8

#endif

/* Exported functions ------------------------------------------------------- */
extern const uint8_t ARC_device_descriptor[ARC_SIZE_DEVICE_DESC];
extern const uint8_t ARC_config_descriptor[ARC_SIZE_CONFIG_DESC];
extern const uint8_t ARC_KB_report_descriptor[ARC_KB_SIZE_REPORT_DESC];
extern const uint8_t ARC_MOUSE_report_descriptor[ARC_MOUSE_SIZE_REPORT_DESC];
extern const uint8_t ARC_JOYSTICK_report_descriptor[ARC_JOYSTICK_SIZE_REPORT_DESC];
extern const uint8_t ARC_string_lang_ID[ARC_SIZE_STRING_LANGID];
extern const uint8_t ARC_string_vendor[ARC_SIZE_STRING_VENDOR];
extern const uint8_t ARC_string_product[ARC_SIZE_STRING_PRODUCT];
extern uint8_t ARC_string_serial[ARC_SIZE_STRING_SERIAL];

#endif /* __USB_DESC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
