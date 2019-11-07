#include "commonusb_app.h"

TCallback common_Callback[8] = {
    0, // CTR_CALLBACK   
    0, // DOVR_CALLBACK
    0, // ERR_CALLBACK
    0, // WKUP_CALLBACK
    0, // SUSB_CALLBACK
    0, // RESET_CALLBACK
    0, // SOF_CALLBACK
    0, // ESOF_CALLBACK
  };

// wInterrupt_Mask - problem
// USB_SIL_Init - musime importovat a asi aj dalsie

extern DEVICE_PROP* common_Device_Property;

void common_Initialize(DEVICE_PROP* prop, TCallback callback[])
{
  common_Device_Property = prop;
  for (int i=0; i<sizeof(common_Callback)/sizeof(common_Callback[0]); i++)
    common_Callback[i] = callback[i];
}