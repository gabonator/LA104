#include <stdint.h>
#include <usb_type.h>
#include <usb_core.h>
#include "usb/sump_app.h"
#include "usb/sump_prop.h"

extern void (*cdcpEpInt_IN[7])(void);
extern void (*cdcpEpInt_OUT[7])(void);
extern void (*cdcpCallbacks[8])(void);
extern void cdcUSB_Istr(void);

extern DEVICE_INFO cdcDevice_Info;
extern LINE_CODING linecoding;
extern DEVICE cdcDevice_Table;
extern DEVICE_PROP cdcDevice_Property;
extern USER_STANDARD_REQUESTS cdcUser_Standard_Requests;

DEVICE_INFO *pInformation;
DEVICE* pDevice_Table;

typedef void (*THandler)(void);

void _Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler callbacks[], THandler LeaveLowPower);

void USB_Init()
{
  pInformation = &cdcDevice_Info; // for internal use
  pDevice_Table = &cdcDevice_Table; // for internal use
  _Initialize(&cdcDevice_Info, &cdcDevice_Table, &cdcDevice_Property, &cdcUser_Standard_Requests, cdcpEpInt_IN, cdcpEpInt_OUT, cdcpCallbacks, Leave_LowPowerMode);
}

