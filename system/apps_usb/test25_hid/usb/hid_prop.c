/**
 ******************************************************************************
 * @file	usb_prop.c
 * @author	MCD Application Team
 * @version V4.0.0
 * @date	21-January-2013
 * @brief	All processing related to Virtual Com Port Demo
 ******************************************************************************
 */
#include <usb_lib.h>
#include "hid_conf.h"
#include "hid_prop.h"
#include "hid_desc.h"
#include "hid_app.h"

uint32_t ProtocolValue;

DEVICE_INFO Device_Info;

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    ARC_init,
    ARC_Reset,
    ARC_Status_In,
    ARC_Status_Out,
    ARC_Data_Setup,
    ARC_NoData_Setup,
    ARC_Get_Interface_Setting,
    ARC_GetDeviceDescriptor,
    ARC_GetConfigDescriptor,
    ARC_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    ARC_GetConfiguration,
    ARC_SetConfiguration,
    ARC_GetInterface,
    ARC_SetInterface,
    ARC_GetStatus,
    ARC_ClearFeature,
    ARC_SetEndPointFeature,
    ARC_SetDeviceFeature,
    ARC_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)ARC_device_descriptor,
    ARC_SIZE_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)ARC_config_descriptor,
    ARC_SIZE_CONFIG_DESC
  };

ONE_DESCRIPTOR ARC_KB_Report_Descriptor =
  {
    (uint8_t *)ARC_KB_report_descriptor,
    ARC_KB_SIZE_REPORT_DESC
  };

ONE_DESCRIPTOR ARC_MOUSE_Report_Descriptor =
  {
    (uint8_t *)ARC_MOUSE_report_descriptor,
    ARC_MOUSE_SIZE_REPORT_DESC
  };

ONE_DESCRIPTOR ARC_JOYSTICK_Report_Descriptor =
  {
    (uint8_t *)ARC_JOYSTICK_report_descriptor,
    ARC_JOYSTICK_SIZE_REPORT_DESC
  };

ONE_DESCRIPTOR ARC_Hid_Descriptor =
  {
    (uint8_t*)ARC_config_descriptor + ARC_OFFS_HID_DESC,
    ARC_SIZE_HID_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(uint8_t*)ARC_string_lang_ID, ARC_SIZE_STRING_LANGID},
    {(uint8_t*)ARC_string_vendor, ARC_SIZE_STRING_VENDOR},
    {(uint8_t*)ARC_string_product, ARC_SIZE_STRING_PRODUCT},
    {(uint8_t*)ARC_string_serial, ARC_SIZE_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : ARC_init.
* Description    : Joystick Mouse init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ARC_init(void)
{
  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pDeviceInfo->Current_Configuration = 0;
  /* Connect the device */
  //PowerOn();

  /* Perform basic device initialization operations */
  //USB_SIL_Init();
  InitializeFinish(IMR_MSK);


  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : ARC_Reset.
* Description    : Joystick Mouse reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ARC_Reset(void)
{
  /* Set ARC_DEVICE as not configured */
  pDeviceInfo->Current_Configuration = 0;
  pDeviceInfo->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pDeviceInfo->Current_Feature = ARC_config_descriptor[7];
  SetBTABLE(BTABLE_ADDRESS);
  /* Initialize Endpoint 0 */

  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, USB_KB_REPORT_KEYMAP_SIZE);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_INTERRUPT);
  SetEPTxAddr(ENDP2, ENDP2_TXADDR);
  SetEPTxCount(ENDP2, 8);
  SetEPRxStatus(ENDP2, EP_RX_DIS);
  SetEPTxStatus(ENDP2, EP_TX_NAK);

#ifndef CONFIG_ANNOYATRON

  /* Initialize Endpoint 3 */
  SetEPType(ENDP3, EP_INTERRUPT);
  SetEPTxAddr(ENDP3, ENDP3_TXADDR);
  SetEPTxCount(ENDP3, 8);
  SetEPRxStatus(ENDP3, EP_RX_DIS);
  SetEPTxStatus(ENDP3, EP_TX_NAK);

  /* Initialize Endpoint 4 */
  SetEPType(ENDP4, EP_INTERRUPT);
  SetEPTxAddr(ENDP4, ENDP4_TXADDR);
  SetEPTxCount(ENDP4, 8);
  SetEPRxStatus(ENDP4, EP_RX_DIS);
  SetEPTxStatus(ENDP4, EP_TX_NAK);

#ifdef CONFIG_ARCHID_VCD

  /* Initialize Endpoint 7 */
  SetEPType(ENDP7, EP_BULK);
  SetEPTxAddr(ENDP7, ENDP7_TXADDR);
  SetEPTxStatus(ENDP7, EP_TX_NAK);
  SetEPRxStatus(ENDP7, EP_RX_DIS);

  /* Initialize Endpoint 5 */
  SetEPType(ENDP5, EP_INTERRUPT);
  SetEPTxAddr(ENDP5, ENDP5_TXADDR);
  SetEPRxStatus(ENDP5, EP_RX_DIS);
  SetEPTxStatus(ENDP5, EP_TX_NAK);

  /* Initialize Endpoint 6 */
  SetEPType(ENDP6, EP_BULK);
  SetEPRxAddr(ENDP6, ENDP6_RXADDR);
  SetEPRxCount(ENDP6, VIRTUAL_COM_PORT_DATA_SIZE);
  SetEPRxStatus(ENDP6, EP_RX_VALID);
  SetEPTxStatus(ENDP6, EP_TX_DIS);

#endif
#endif // CONFIG_ANNOYATRON

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  bDeviceState = ATTACHED;
}
/*******************************************************************************
* Function Name  : ARC_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ARC_SetConfiguration(void)
{
//dbgPrint("setcon,");
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}
/*******************************************************************************
* Function Name  : ARC_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ARC_SetDeviceAddress (void)
{
//dbgPrint("setda,");

  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : ARC_Status_In.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ARC_Status_In(void)
{
//dbgPrint("stain,");

  static uint8_t old_led_state = 0;
  if (old_led_state != kb_led_state) {
    //print("led state:%s %s\n", kb_led_state & 1 ? "NUM":"", kb_led_state & 2 ? "CAPS":"");
    old_led_state = kb_led_state;
  }
#ifdef CONFIG_ARCHID_VCD
  if (Request == SET_LINE_CODING)
  {
    Request = 0;
  }
#endif
}

/*******************************************************************************
* Function Name  : ARC_Status_Out
* Description    : Joystick status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ARC_Status_Out (void)
{
//dbgPrint("staout,");
}

uint8_t *ARC_set_configuration(uint16_t Length)
{
//dbgPrint("setc,");

  pDeviceInfo->Ctrl_Info.Usb_wLength = 1;
  return &kb_led_state;
}

#ifdef CONFIG_ARCHID_VCD
/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetLineCoding.
* Description    : send the linecoding structure to the PC host.
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.
*******************************************************************************/
uint8_t *ARC_VCP_GetLineCoding(uint16_t Length)
{
  if (Length == 0)
  {
    pDeviceInfo->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(uint8_t *)&linecoding;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetLineCoding.
* Description    : Set the linecoding structure fields.
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.
*******************************************************************************/
uint8_t *ARC_VCP_SetLineCoding(uint16_t Length)
{
  if (Length == 0)
  {
    pDeviceInfo->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(uint8_t *)&linecoding;
}
#endif

/*******************************************************************************
* Function Name  : ARC_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT ARC_Data_Setup(uint8_t RequestNo)
{
//dbgPrint("data,");

  uint8_t *(*CopyRoutine)(uint16_t);
  CopyRoutine = NULL;

  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pDeviceInfo->USBwIndex0 >= 0 || pDeviceInfo->USBwIndex0 <= 3))
  {
    if (pDeviceInfo->USBwValue1 == REPORT_DESCRIPTOR)
    {
      if (pDeviceInfo->USBwIndex0 == 0) {
        CopyRoutine = ARC_GetKBReportDescriptor;
      } else if (pDeviceInfo->USBwIndex0 == 1) {
        CopyRoutine = ARC_GetMouseReportDescriptor;
      } else { // ix 2 | | 3
        CopyRoutine = ARC_GetJoystickReportDescriptor;
      }
    }
    else if (pDeviceInfo->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = ARC_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
           && RequestNo == GET_PROTOCOL)
  {
    CopyRoutine = ARC_GetProtocolValue;
  }
  /*** SET_CONFIGURATION ***/
  else if (RequestNo == SET_CONFIGURATION) {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = ARC_set_configuration;
    }
  }
#ifdef CONFIG_ARCHID_VCD

  else if (RequestNo == GET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = ARC_VCP_GetLineCoding;
    }
  }
  else if (RequestNo == SET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = ARC_VCP_SetLineCoding;
    }
    Request = SET_LINE_CODING;
  }
#endif



  if (CopyRoutine == NULL)
  {
//dbgPrint("!notsupported,");
    return USB_UNSUPPORT;
  }
  pDeviceInfo->Ctrl_Info.CopyData = CopyRoutine;
  pDeviceInfo->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : ARC_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT ARC_NoData_Setup(uint8_t RequestNo)
{
//dbgPrint("nodata,");

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
    if (RequestNo == SET_PROTOCOL)
    {
      return ARC_SetProtocol();
    }
#ifdef CONFIG_ARCHID_VCD
    else if (RequestNo == SET_COMM_FEATURE)
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)
    {
      return USB_SUCCESS;
    }
#endif
  }
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : ARC_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *ARC_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : ARC_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *ARC_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : ARC_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *ARC_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pDeviceInfo->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : ARC_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *ARC_GetKBReportDescriptor(uint16_t Length)
{
//dbgPrint("getkbd,");
  return Standard_GetDescriptorData(Length, &ARC_KB_Report_Descriptor);
}
uint8_t *ARC_GetMouseReportDescriptor(uint16_t Length)
{
//dbgPrint("getmsd,");
  return Standard_GetDescriptorData(Length, &ARC_MOUSE_Report_Descriptor);
}
uint8_t *ARC_GetJoystickReportDescriptor(uint16_t Length)
{
//dbgPrint("getjod,");
  return Standard_GetDescriptorData(Length, &ARC_JOYSTICK_Report_Descriptor);
}

/*******************************************************************************
* Function Name  : ARC_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *ARC_GetHIDDescriptor(uint16_t Length)
{
//dbgPrint("gethidd,");
  return Standard_GetDescriptorData(Length, &ARC_Hid_Descriptor);
}

/*******************************************************************************
* Function Name  : ARC_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT ARC_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
//dbgPrint("getis,");

  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
#if defined(CONFIG_ARCHID_VCD) && !defined(CONFIG_ANNOYATRON)
  else if (Interface > 1)
#else
    else if (Interface > 0)
#endif
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : ARC_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT ARC_SetProtocol(void)
{
//dbgPrint("setpro,");

  uint8_t wValue0 = pDeviceInfo->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : ARC_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t *ARC_GetProtocolValue(uint16_t Length)
{
//dbgPrint("getpro,");

  if (Length == 0)
  {
    pDeviceInfo->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
