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
#include "midi_conf.h"
#include "midi_prop.h"
#include "midi_desc.h"
#include "midi_app.h"

static void MIOS32_USB_CB_Init(void);
static void MIOS32_USB_CB_Reset(void);
static void MIOS32_USB_CB_SetConfiguration(void);
static void MIOS32_USB_CB_SetDeviceAddress (void);
static void MIOS32_USB_CB_Status_In(void);
static void MIOS32_USB_CB_Status_Out(void);
static RESULT MIOS32_USB_CB_Data_Setup(u8 RequestNo);
static RESULT MIOS32_USB_CB_NoData_Setup(u8 RequestNo);
static u8 *MIOS32_USB_CB_GetDeviceDescriptor(u16 Length);
static u8 *MIOS32_USB_CB_GetConfigDescriptor(u16 Length);
static u8 *MIOS32_USB_CB_GetStringDescriptor(u16 Length);
static RESULT MIOS32_USB_CB_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);


DEVICE_INFO Device_Info;

DEVICE Device_Table = {
  MIOS32_USB_EP_NUM,
  1
};

DEVICE_PROP Device_Property = {
  MIOS32_USB_CB_Init, // MIOS32_USB_CB_Init,
  MIOS32_USB_CB_Reset,
  MIOS32_USB_CB_Status_In,
  MIOS32_USB_CB_Status_Out,
  MIOS32_USB_CB_Data_Setup,
  MIOS32_USB_CB_NoData_Setup,
  MIOS32_USB_CB_Get_Interface_Setting,
  MIOS32_USB_CB_GetDeviceDescriptor,
  MIOS32_USB_CB_GetConfigDescriptor,
  MIOS32_USB_CB_GetStringDescriptor,
  0,
  0x40 /*MAX PACKET SIZE*/
};

USER_STANDARD_REQUESTS User_Standard_Requests = {
  NOP_Process, // MIOS32_USB_CB_GetConfiguration,
  MIOS32_USB_CB_SetConfiguration,
  NOP_Process, // MIOS32_USB_CB_GetInterface,
  NOP_Process, // MIOS32_USB_CB_SetInterface,
  NOP_Process, // MIOS32_USB_CB_GetStatus,
  NOP_Process, // MIOS32_USB_CB_ClearFeature,
  NOP_Process, // MIOS32_USB_CB_SetEndPointFeature,
  NOP_Process, // MIOS32_USB_CB_SetDeviceFeature,
  MIOS32_USB_CB_SetDeviceAddress
};

void MIOS32_USB_CB_Init(void)
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
void MIOS32_USB_CB_Reset(void)
{
  // Set MIOS32 Device as not configured
  pInformation->Current_Configuration = 0;

  // Current Feature initialization
  pInformation->Current_Feature = MIOS32_USB_ConfigDescriptor[7];

  // Set MIOS32 Device with the default Interface
  pInformation->Current_Interface = 0;

#ifdef STM32F10X_CL   
  // EP0 is already configured in DFU_Init() by USB_SIL_Init() function
#else 
  SetBTABLE(MIOS32_USB_BTABLE_ADDRESS);

  // Initialize Endpoint 0
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, MIOS32_USB_ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, MIOS32_USB_ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);
#endif
  

#ifndef MIOS32_DONT_USE_USB_MIDI
#if MIOS32_USB_MIDI_DATA_OUT_EP != 0x02
# error "Please adapt the code for different endpoints"
#endif
#if MIOS32_USB_MIDI_DATA_IN_EP != 0x81
# error "Please adapt the code for different endpoints"
#endif

# ifdef STM32F10X_CL   
  // Init EP1 IN
  OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, MIOS32_USB_MIDI_DATA_IN_SIZE);
  
  // Init EP2 OUT
  OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, MIOS32_USB_MIDI_DATA_OUT_SIZE);
# else
  // Initialize Endpoint 1/2
  SetEPType(ENDP1, EP_BULK);
  SetEPType(ENDP2, EP_BULK);

  SetEPTxAddr(ENDP1, MIOS32_USB_ENDP1_TXADDR);
  SetEPTxCount(ENDP1, MIOS32_USB_MIDI_DATA_OUT_SIZE);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  SetEPRxAddr(ENDP2, MIOS32_USB_ENDP2_RXADDR);
  SetEPRxCount(ENDP2, MIOS32_USB_MIDI_DATA_IN_SIZE);
  SetEPRxValid(ENDP2);
# endif
#endif


#ifdef MIOS32_USE_USB_COM
# ifdef STM32F10X_CL   
  // TODO...
# else
  // Initialize Endpoint 5
  SetEPType(ENDP5, EP_INTERRUPT);
  SetEPTxAddr(ENDP5, MIOS32_USB_ENDP5_TXADDR);
  SetEPRxStatus(ENDP5, EP_RX_DIS);
  SetEPTxStatus(ENDP5, EP_TX_NAK);

  // Initialize Endpoint 3
  SetEPType(ENDP3, EP_BULK);
  SetEPRxAddr(ENDP3, MIOS32_USB_ENDP3_RXADDR);
  SetEPRxCount(ENDP3, MIOS32_USB_COM_DATA_OUT_SIZE);
  SetEPRxStatus(ENDP3, EP_RX_VALID);
  SetEPTxStatus(ENDP3, EP_TX_DIS);

  // Initialize Endpoint 4
  SetEPType(ENDP4, EP_BULK);
  SetEPTxAddr(ENDP4, MIOS32_USB_ENDP4_TXADDR);
  SetEPTxStatus(ENDP4, EP_TX_NAK);
  SetEPRxStatus(ENDP4, EP_RX_DIS);
# endif
#endif

  // Set this device to response on default address
#ifndef STM32F10X_CL   
  SetDeviceAddress(0);
#endif

#ifndef MIOS32_DONT_USE_USB_MIDI
  // propagate connection state to USB MIDI driver
  MIOS32_USB_MIDI_ChangeConnectionState(0); // not connected
#endif
#ifdef MIOS32_USE_USB_COM
  // propagate connection state to USB COM driver
  MIOS32_USB_COM_ChangeConnectionState(0); // not connected
#endif

  bDeviceState = ATTACHED;
}

static void MIOS32_USB_CB_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0) {
#ifndef MIOS32_DONT_USE_USB_MIDI
    // propagate connection state to USB MIDI driver
    MIOS32_USB_MIDI_ChangeConnectionState(1); // connected
#endif
#ifdef MIOS32_USE_USB_COM
    // propagate connection state to USB COM driver
    MIOS32_USB_COM_ChangeConnectionState(1); // connected
#endif

    bDeviceState = CONFIGURED;
  }
}

// update the device state to addressed
static void MIOS32_USB_CB_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

// status IN routine
static void MIOS32_USB_CB_Status_In(void)
{
#ifdef MIOS32_USE_USB_COM
  MIOS32_USB_COM_CB_StatusIn();
#endif
}

// status OUT routine
static void MIOS32_USB_CB_Status_Out(void)
{
}

static RESULT MIOS32_USB_CB_Data_Setup(u8 RequestNo)
{
#ifdef MIOS32_USE_USB_COM
  RESULT res;
  if( (res=MIOS32_USB_COM_CB_Data_Setup(RequestNo)) != USB_UNSUPPORT )
    return res;
#endif
  return USB_UNSUPPORT;
}

// handles the non data class specific requests.
static RESULT MIOS32_USB_CB_NoData_Setup(u8 RequestNo)
{
#ifdef MIOS32_USE_USB_COM
  RESULT res;
  if( (res=MIOS32_USB_COM_CB_NoData_Setup(RequestNo)) != USB_UNSUPPORT )
    return res;
#endif

  return USB_UNSUPPORT;
}

// gets the device descriptor.
static u8 *MIOS32_USB_CB_GetDeviceDescriptor(u16 Length)
{
  ONE_DESCRIPTOR desc = {(u8 *)MIOS32_USB_DeviceDescriptor, MIOS32_USB_SIZ_DEVICE_DESC};
  return Standard_GetDescriptorData(Length, &desc);
}

// gets the configuration descriptor.
static u8 *MIOS32_USB_CB_GetConfigDescriptor(u16 Length)
{
#if !defined(MIOS32_DONT_USE_USB_MIDI) && MIOS32_USB_MIDI_NUM_PORTS > 1
  if( MIOS32_USB_ForceSingleUSB() ) {
    ONE_DESCRIPTOR desc = {(u8 *)MIOS32_USB_ConfigDescriptor_SingleUSB, MIOS32_USB_SIZ_CONFIG_DESC_SINGLE_USB};
    return Standard_GetDescriptorData(Length, &desc);
  }
#endif

  ONE_DESCRIPTOR desc = {(u8 *)MIOS32_USB_ConfigDescriptor, MIOS32_USB_SIZ_CONFIG_DESC};
  return Standard_GetDescriptorData(Length, &desc);
}

// gets the string descriptors according to the needed index
static u8 *MIOS32_USB_CB_GetStringDescriptor(u16 Length)
{
  const u8 vendor_str[] = MIOS32_USB_VENDOR_STR;
  const u8 product_str[] = MIOS32_USB_PRODUCT_STR;

  u8 buffer[200];
  u16 len = 0;
  int i;

  switch( pInformation->USBwValue0 ) {
    case 0: // Language
      // buffer[0] and [1] initialized below
      buffer[2] = 0x09;        // CharSet
      buffer[3] = 0x04;        // U.S.
      len = 4;
      break;

    case 1: // Vendor
      // buffer[0] and [1] initialized below
      for(i=0, len=2; vendor_str[i] != '\0' && len<200; ++i) {
	buffer[len++] = vendor_str[i];
	buffer[len++] = 0;
      }
      break;

    case 2: { // Product
      // buffer[0] and [1] initialized below

      // check for user defined product string
      char *product_str_ptr = (char *)product_str;
#ifdef MIOS32_SYS_ADDR_USB_DEV_NAME
      char *product_str_user = (char *)MIOS32_SYS_ADDR_USB_DEV_NAME;
      int j;
      u8 valid_str = 1;
      for(j=0, len=0; j<MIOS32_SYS_USB_DEV_NAME_LEN && valid_str && product_str_user[j]; ++j, ++len) {
	if( product_str_user[j] < 0x20 || product_str_user[j] >= 0x80 )
	  valid_str = 0;
      }
      if( valid_str && len )
	product_str_ptr = product_str_user;
#endif

      for(i=0, len=2; product_str_ptr[i] != '\0' && len<200; ++i) {
	buffer[len++] = product_str_ptr[i];
	buffer[len++] = 0;
      }
    } break;

    case 3: { // Serial Number
        u8 serial_number_str[40];
	if( MIOS32_SYS_SerialNumberGet((char *)serial_number_str) >= 0 ) {
	  for(i=0, len=2; serial_number_str[i] != '\0' && len<200; ++i) {
	    buffer[len++] = serial_number_str[i];
	    buffer[len++] = 0;
	  }
	} else
	  return NULL;
      }
      break;
    default: // string ID not supported
      return NULL;
  }

  buffer[0] = len; // Descriptor Length
  buffer[1] = DSCR_STRING; // Descriptor Type
  ONE_DESCRIPTOR desc = {(u8 *)buffer, len};
  return Standard_GetDescriptorData(Length, &desc);
}

// test the interface and the alternate setting according to the supported one.
static RESULT MIOS32_USB_CB_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if( AlternateSetting > 0 ) {
    return USB_UNSUPPORT;
  } else if( Interface >= MIOS32_USB_NUM_INTERFACES ) {
    return USB_UNSUPPORT;
  }

  return USB_SUCCESS;
}
