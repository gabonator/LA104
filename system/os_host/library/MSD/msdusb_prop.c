/******************** (C) COPYRIGHT 2014 e-Design Co., Ltd. ********************
 File Name : USB_prop.c
 Version   : STM32 USB Disk Ver 3.4       Author : MCD Application Team & bure
*******************************************************************************/
#include <USB_lib.h>
#include "msdusb_desc.h"
#include "../COMMON/commonusb_pwr.h"
#include "msdusb_bot.h"
#include "msdusb_prop.h"
#include "msdusb_conf.h"
#include "msdusb_app.h"

u32 Max_Lun = 0;

extern DEVICE_STATE massDeviceState; /* USB device status */

DEVICE_INFO massDevice_Info;

DEVICE massDevice_Table = { EP_NUM, 1 };

DEVICE_PROP massDevice_Property ={
    MASS_init,
    MASS_Reset,
    MASS_Status_In,
    MASS_Status_Out,
    MASS_Data_Setup,
    MASS_NoData_Setup,
    MASS_Get_Interface_Setting,
    MASS_GetDeviceDescriptor,
    MASS_GetConfigDescriptor,
    MASS_GetStringDescriptor,
    0,
    0x40 // MAX PACKET SIZE
  };
USER_STANDARD_REQUESTS massUser_Standard_Requests ={
    Mass_Storage_GetConfiguration,
    Mass_Storage_SetConfiguration,
    Mass_Storage_GetInterface,
    Mass_Storage_SetInterface,
    Mass_Storage_GetStatus,
    Mass_Storage_ClearFeature,
    Mass_Storage_SetEndPointFeature,
    Mass_Storage_SetDeviceFeature,
    Mass_Storage_SetDeviceAddress
  };
ONE_DESCRIPTOR massDevice_Descriptor ={
    (u8*)MASS_DeviceDescriptor,
    MASS_SIZ_DEVICE_DESC
  };
ONE_DESCRIPTOR massConfig_Descriptor ={
    (u8*)MASS_ConfigDescriptor,
    MASS_SIZ_CONFIG_DESC
  };
ONE_DESCRIPTOR massString_Descriptor[5] ={
    {(u8*)MASS_StringLangID, MASS_SIZ_STRING_LANGID},
    {(u8*)MASS_StringVendor, MASS_SIZ_STRING_VENDOR},
    {(u8*)MASS_StringProduct, MASS_SIZ_STRING_PRODUCT},
    {(u8*)MASS_StringSerial, MASS_SIZ_STRING_SERIAL},
    {(u8*)MASS_StringInterface, MASS_SIZ_STRING_INTERFACE},
  };

/*******************************************************************************
  MASS_init: Mass Storage init routine.
*******************************************************************************/
void MASS_init()
{
  MASS_Get_SerialNum(); // Update the serial number string descriptor with the data from the unique ID
  pInformation->Current_Configuration = 0;
  common_PowerOn();       // Connect the device
  USB_SIL_Init(IMR_MSK);			/* Perform basic device initialization operations */
  massDeviceState = UNCONNECTED;
}
/*******************************************************************************
  MASS_Reset: Mass Storage reset routine.
*******************************************************************************/
void MASS_Reset()
{
  massDevice_Info.Current_Configuration = 0; // Set the device as not configured
  pInformation->Current_Feature = MASS_ConfigDescriptor[7]; // Current Feature initialization
  SetBTABLE(BTABLE_ADDRESS);
  // Initialize Endpoint 0
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, massDevice_Property.MaxPacketSize);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxValid(ENDP0);
  //Initialize Endpoint 1
  SetEPType(ENDP1, EP_BULK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  // Initialize Endpoint 2
  SetEPType(ENDP2, EP_BULK);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, massDevice_Property.MaxPacketSize);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
  SetEPTxStatus(ENDP2, EP_TX_DIS);

  SetEPRxCount(ENDP0, massDevice_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);
  // Set the device to response on default address
  SetDeviceAddress(0);

  massDeviceState = ATTACHED;

  CBW.dSignature = BOT_CBW_SIGNATURE;
  Bot_State = BOT_IDLE;
}

/*******************************************************************************
  Mass_Storage_SetConfiguration: Handle the SetConfiguration request.
*******************************************************************************/
void Mass_Storage_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0){
    massDeviceState = CONFIGURED; // Device configured
    ClearDTOG_TX(ENDP1);
    ClearDTOG_RX(ENDP2);
    Bot_State = BOT_IDLE;      // set the Bot state machine to the IDLE state
  }
}
/*******************************************************************************
  Mass_Storage_ClearFeature: Handle the ClearFeature request.
*******************************************************************************/
void Mass_Storage_ClearFeature(void)
{
  /* when the host send a CBW with invalid signature or invalid length the two
     Endpoints (IN & OUT) shall stall until receiving a Mass Storage Reset    */
  if (CBW.dSignature != BOT_CBW_SIGNATURE)
    Bot_Abort(BOTH_DIR);
}
/*******************************************************************************
  Mass_Storage_SetConfiguration: Udpade the device state to addressed.
*******************************************************************************/
void Mass_Storage_SetDeviceAddress (void)
{
  massDeviceState = ADDRESSED;
}
/*******************************************************************************
  MASS_Status_In: Mass Storage Status IN routine.
*******************************************************************************/
void MASS_Status_In(void)
{
  return;
}
/*******************************************************************************
  MASS_Status_Out: Mass Storage Status OUT routine.
*******************************************************************************/
void MASS_Status_Out(void)
{
  return;
}
/*******************************************************************************
  MASS_Data_Setup: Handle the data class specific requests.
*******************************************************************************/
RESULT MASS_Data_Setup(u8 RequestNo)
{
  u8* (*CopyRoutine)(u16);

  CopyRoutine = NULL;
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == GET_MAX_LUN) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x01))
  {
    CopyRoutine = Get_Max_Lun;
  } else   return USB_UNSUPPORT;
  if (CopyRoutine == NULL)  return USB_UNSUPPORT;
  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}
/*******************************************************************************
  MASS_NoData_Setup: Handle the no data class specific requests.
*******************************************************************************/
RESULT MASS_NoData_Setup(u8 RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == MASS_STORAGE_RESET) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x00))
  {
    ClearDTOG_TX(ENDP1);  // Initialize Endpoint 1
    ClearDTOG_RX(ENDP2);  // Initialize Endpoint 2
    CBW.dSignature = BOT_CBW_SIGNATURE; // intialise the CBW signature to enable the clear feature
    Bot_State = BOT_IDLE;
    return USB_SUCCESS;
  }
  return USB_UNSUPPORT;
}
/*******************************************************************************
  MASS_Get_Interface_Setting: Test the interface and the alternate setting
                              according to the supported one.
*******************************************************************************/
RESULT MASS_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)  return USB_UNSUPPORT;// in this application we don't have AlternateSetting
  else if (Interface > 0)  return USB_UNSUPPORT;  // in this application we have only 1 interfaces
  return USB_SUCCESS;
}
/*******************************************************************************
  MASS_GetDeviceDescriptor: Get the device descriptor.
*******************************************************************************/
u8 *MASS_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &massDevice_Descriptor );
}
/*******************************************************************************
   MASS_GetConfigDescriptor: Get the configuration descriptor.
*******************************************************************************/
u8 *MASS_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &massConfig_Descriptor );
}

/*******************************************************************************
  MASS_GetStringDescriptor: Get the string descriptors according to
                            the needed index.
*******************************************************************************/
u8 *MASS_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 5)  return NULL;
  else  return Standard_GetDescriptorData(Length, &massString_Descriptor[wValue0]);
}
/*******************************************************************************
  Get_Max_Lun: Handle the Get Max Lun request.
*******************************************************************************/
u8 *Get_Max_Lun(u16 Length)
{
  if (Length == 0){
    pInformation->Ctrl_Info.Usb_wLength = LUN_DATA_LENGTH;
    return 0;
  } else  return((u8*)(&Max_Lun));
}
/*******************************************************************************
Get_SerialNum :   Create the serial number string descriptor.
*******************************************************************************/
void MASS_Get_SerialNum(void)
{
/*
   u32 Device_Serial0, Device_Serial1, Device_Serial2;

   Device_Serial0 = *(vu32*)(0x1FFFF7E8);
   Device_Serial1 = *(vu32*)(0x1FFFF7EC);
   Device_Serial2 = *(vu32*)(0x1FFFF7F0);

   if (Device_Serial0 != 0)
   {
     uint64_t serial = Device_Serial0 | ((uint64_t)(Device_Serial1 + Device_Serial2)) << 32;
     for (int i=0; i<12; i++)
       MASS_StringSerial[2+i*2] = 'A' + ((serial>>(i*4))%15);
   }
*/
}
/*********************************  END OF FILE  ******************************/
