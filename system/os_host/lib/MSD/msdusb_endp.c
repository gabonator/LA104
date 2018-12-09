#include <stm32f10x.h>
#include "msdusb_conf.h"
#include "msdusb_bot.h"
#include <usb_core.h>

void massEP1_IN_Callback(void);
void massEP2_OUT_Callback(void);

void (*masspEpInt_IN[7])(void) ={
    massEP1_IN_Callback,
    massEP2_IN_Callback,
    massEP3_IN_Callback,
    massEP4_IN_Callback,
    massEP5_IN_Callback,
    massEP6_IN_Callback,
    massEP7_IN_Callback,
  };
void (*masspEpInt_OUT[7])(void) ={
    massEP1_OUT_Callback,
    massEP2_OUT_Callback,
    massEP3_OUT_Callback,
    massEP4_OUT_Callback,
    massEP5_OUT_Callback,
    massEP6_OUT_Callback,
    massEP7_OUT_Callback,
  };

void (*masspCallbacks[8])(void) = {0, 0, 0, 0, 0, 0, 0, 0};

/*******************************************************************************
  EP1_IN_Callback: EP1 IN Callback Routine
*******************************************************************************/
void massEP1_IN_Callback(void)
{
  Mass_Storage_In();
}
/*******************************************************************************
  EP2_OUT_Callback: EP2 OUT Callback Routine.
*******************************************************************************/
void massEP2_OUT_Callback(void)
{
  Mass_Storage_Out();
}
/*********************************  END OF FILE  ******************************/

