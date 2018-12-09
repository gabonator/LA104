#include "Bios.h"
#include "imports.h"
#include "system_stm32f10x.h"
#include "lib/STM32F10x_StdPeriph_Driver/inc/misc.h"

extern "C" {
//#include "USB_type.h"
//#include "USB_lib.h"
#include "usb_core.h"
#include "usb_init.h"
}

extern "C" void USB_Connect(uint8_t Status);

typedef struct {
    uint32_t EPR[8];
    uint32_t RESERVED[8];
    uint32_t CNTR;
    uint32_t ISTR;
    uint32_t FNR;
    uint32_t DADDR;
    uint32_t BTABLE;
} USB_TypeDef;

#define USB_BASE ((uint32_t)0x40005C00)
volatile USB_TypeDef *USB = (USB_TypeDef *)USB_BASE;

void EnableUsb()
{
    // TODO: CHECK 44!!!!
    GPIOA->CRH |= 0x000BB440;                // USB DN & DP Pins AF_Mode
    GPIOA->CRH &= 0xFFFBB44F;
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;
    USB->CNTR   = USB_CNTR_FRES; /* Force USB Reset */
    USB->BTABLE = 0;
    USB->DADDR  = 0;
    USB->ISTR   = 0;
    USB->CNTR   = USB_CNTR_RESETM;
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

extern "C" {
  void USB_Init(void);
  void Disk_Init(void);

  // placement for non instantiaed required attributes
  void (*pEpInt_IN[7])(void) = {};
  void (*pEpInt_OUT[7])(void) = {};
  DEVICE Device_Table;

  // imports for mass device
  extern DEVICE_INFO massDevice_Info;
  extern DEVICE massDevice_Table;
  extern DEVICE_PROP massDevice_Property;
  extern USER_STANDARD_REQUESTS massUser_Standard_Requests;
  extern void (*masspEpInt_IN[7])(void);
  extern void (*masspEpInt_OUT[7])(void);
  extern void massUSB_Istr(void);

  // imports for cdc
  extern DEVICE_INFO cdcDevice_Info;
  extern DEVICE cdcDevice_Table;
  extern DEVICE_PROP cdcDevice_Property;
  extern USER_STANDARD_REQUESTS cdcUser_Standard_Requests;
  extern void (*cdcpEpInt_IN[7])(void);
  extern void (*cdcpEpInt_OUT[7])(void);
  extern void cdcUSB_Istr(void);

  // placement for instantiated handlers
  BIOS::USB::THandler pUsbIstrHandler = nullptr;

  void USB_Istr(void)
  {
    if (pUsbIstrHandler)
      pUsbIstrHandler();
  }
   
  void CTR_LP(void);
}




namespace BIOS
{
  namespace USB
  {
//    void Initialize(DEVICE* pDevice, DEVICE_PROP* pDeviceProperty, USER_STANDARD_REQUESTS *pUserStandardRequests,
//      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler istrHandler)

    void Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler istrHandler)
    {
      pUsbIstrHandler = istrHandler;      
      for (int i=0; i<7; i++)
      {
        pEpInt_IN[i] = arrHandlerIn[i];
        pEpInt_OUT[i] = arrHandlerOut[i];
      }

      Device_Table = *(DEVICE*)pDevice;
      pInformation = (DEVICE_INFO*)pDeviceInfo;//&Device_Info;
      pInformation->ControlState = IN_DATA; //2;
      pProperty = (DEVICE_PROP*)pDeviceProperty;
      pUser_Standard_Requests = (USER_STANDARD_REQUESTS*)pUserStandardRequests;
      pProperty->Init();
    }

    void InitializeMass()
    {
      Initialize(&massDevice_Info, &massDevice_Table, &massDevice_Property, &massUser_Standard_Requests, masspEpInt_IN, masspEpInt_OUT, massUSB_Istr);
      Disk_Init();
    }

    void InitializeSerial()
    {
      Initialize(&cdcDevice_Info, &cdcDevice_Table, &cdcDevice_Property, &cdcUser_Standard_Requests, cdcpEpInt_IN, cdcpEpInt_OUT, cdcUSB_Istr);
    }

    void Enable()
    {
      EnableUsb(true);
//      EnableUsb();
    }

    void Disable()
    {
      EnableUsb(false);
    }

    void CTR_LP()
    {
      ::CTR_LP();
    }

  }
}