#include "Bios.h"
#include "imports.h"
#include "system_stm32f10x.h"
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"

extern "C" {
#include "library/COMMON/commonusb_app.h"
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

void DummyFunction()
{
}
/*
void EnableUsb()
{
    // TODO: CHECK 44!!!!
    GPIOA->CRH |= 0x000BB440;                // USB DN & DP Pins AF_Mode
    GPIOA->CRH &= 0xFFFBB44F;
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;
    USB->CNTR   = USB_CNTR_FRES; // Force USB Reset
    USB->BTABLE = 0;
    USB->DADDR  = 0;
    USB->ISTR   = 0;
    USB->CNTR   = USB_CNTR_RESETM;
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
*/
extern "C" {
  void USB_Init(void);
  void Disk_Init(void);
  void common_PowerOn();
  void USB_SIL_Init(uint16_t);

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
  extern void (*masspCallbacks[8])(void);

  // imports for cdc
  extern DEVICE_INFO cdcDevice_Info;
  extern DEVICE cdcDevice_Table;
  extern DEVICE_PROP cdcDevice_Property;
  extern USER_STANDARD_REQUESTS cdcUser_Standard_Requests;
  extern void (*cdcpEpInt_IN[7])(void);
  extern void (*cdcpEpInt_OUT[7])(void);
  extern void (*cdcpCallbacks[8])(void);

  extern void commonUSB_Istr(void);

  void USB_Istr(void)
  {
    commonUSB_Istr();
  }
}

namespace BIOS
{
  namespace USB
  {
//    void Initialize(DEVICE* pDevice, DEVICE_PROP* pDeviceProperty, USER_STANDARD_REQUESTS *pUserStandardRequests,
//      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler istrHandler)

    void Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler arrCallbacks[])
    {
      commmon_Initialize((DEVICE_PROP*)pDeviceProperty, arrCallbacks);

      for (int i=0; i<7; i++)
      {
        pEpInt_IN[i] = arrHandlerIn[i];
        pEpInt_OUT[i] = arrHandlerOut[i];

        if (!pEpInt_IN[i])
          pEpInt_IN[i] = DummyFunction;
        if (!pEpInt_OUT[i])
          pEpInt_OUT[i] = DummyFunction;
      }
      // endpoint routines must point to a valid function

      Device_Table = *(DEVICE*)pDevice;
      pInformation = (DEVICE_INFO*)pDeviceInfo;//&Device_Info;
      pInformation->ControlState = IN_DATA; //2;
      pProperty = (DEVICE_PROP*)pDeviceProperty;
      pUser_Standard_Requests = (USER_STANDARD_REQUESTS*)pUserStandardRequests;
      pProperty->Init();
    }

    void InitializeMass()
    {
      Disable();
      Initialize(&massDevice_Info, &massDevice_Table, &massDevice_Property, &massUser_Standard_Requests, masspEpInt_IN, masspEpInt_OUT, masspCallbacks);
      Disk_Init();
    }

    void InitializeSerial()
    {
      Disable();
      Initialize(&cdcDevice_Info, &cdcDevice_Table, &cdcDevice_Property, &cdcUser_Standard_Requests, cdcpEpInt_IN, cdcpEpInt_OUT, cdcpCallbacks);
    }

    void Enable()
    {
      EnableUsb(true);
//      EnableUsb();
    }

    void Disable()
    {
      EnableUsb(false);
      BIOS::SYS::DelayMs(500);
    }

    void InitializeFinish(int imr_msk)
    {
	common_PowerOn();				/* Connect the device */
	USB_SIL_Init(imr_msk);			/* Perform basic device initialization operations */
    }
  }
}