/******************** (C) COPYRIGHT 2014 e-Design Co., Ltd. ********************
 File Name : USB_istr.c
 Version   : STM32 USB Disk Ver 3.4        Author : MCD Application Team & bure
*******************************************************************************/
#include <usb_type.h>
#include <usb_regs.h>
#include <usb_init.h>
#include <usb_int.h>
#include "msdusb_pwr.h"
#include "msdusb_istr.h"
#include "msdusb_bot.h"
#include "msdusb_conf.h"

//volatile u16 wIstr;           /* ISTR register last read value */
volatile u8 massbIntPackSOF = 0;  /* SOFs received between 2 consecutive packets */
extern DEVICE_PROP massDevice_Property;

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
/*******************************************************************************
  USB_Istr: ISTR events interrupt service routine
*******************************************************************************/
void massUSB_Istr(void)
{
  u16 wIstr = _GetISTR();
#if (IMR_MSK & ISTR_RESET)
  if (wIstr & ISTR_RESET & wInterrupt_Mask){
//    _SetISTR((u16)CLR_RESET);
    massDevice_Property.Reset();
    _SetISTR((u16)CLR_RESET);
//#ifdef RESET_CALLBACK
//    RESET_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_DOVR)
  if (wIstr & ISTR_DOVR & wInterrupt_Mask){
    _SetISTR((u16)CLR_DOVR);
//#ifdef DOVR_CALLBACK
//    DOVR_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_ERR)
  if (wIstr & ISTR_ERR & wInterrupt_Mask){
    _SetISTR((u16)CLR_ERR);
//#ifdef ERR_CALLBACK
//    ERR_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_WKUP)
  if (wIstr & ISTR_WKUP & wInterrupt_Mask){
//    _SetISTR((u16)CLR_WKUP);
    Resume(RESUME_EXTERNAL);
    _SetISTR((u16)CLR_WKUP);
//#ifdef WKUP_CALLBACK
//    WKUP_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_SUSP)
  if (wIstr & ISTR_SUSP & wInterrupt_Mask){ // check if SUSPEND is possible
    if (fSuspendEnabled)  MASS_Suspend();
    else                  Resume(RESUME_LATER); // if not possible then resume after xx ms
    _SetISTR((u16)CLR_SUSP); // clear of the ISTR bit must be done after setting of CNTR_FSUSP
//#ifdef SUSP_CALLBACK
//    SUSP_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_SOF)
  if (wIstr & ISTR_SOF & wInterrupt_Mask){
    _SetISTR((u16)CLR_SOF);
    massbIntPackSOF++;
//#ifdef SOF_CALLBACK
//    SOF_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_ESOF)
  if (wIstr & ISTR_ESOF & wInterrupt_Mask){
//    _SetISTR((u16)CLR_ESOF);  // resume handling timing is made with ESOFs
    Resume(RESUME_ESOF);      // request without change of the machine state
    _SetISTR((u16)CLR_ESOF);  // resume handling timing is made with ESOFs
//#ifdef ESOF_CALLBACK
//    ESOF_Callback();
//#endif
  }
#endif
//-----------------------------------------------------------------------------
#if (IMR_MSK & ISTR_CTR)
  if (wIstr & ISTR_CTR & wInterrupt_Mask){
    /* servicing of the endpoint correct transfer interrupt */
    /* clear of the CTR flag into the sub */
    CTR_LP();
//#ifdef CTR_CALLBACK
//    CTR_Callback();
//#endif
  }
#endif
} /* USB_Istr */
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

