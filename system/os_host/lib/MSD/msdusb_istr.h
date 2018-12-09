/******************** (C) COPYRIGHT 2014 e-Design Co., Ltd. ********************
 File Name : USB_istr.h  
 Version   : STM32 USB Disk Ver 3.4       Author : MCD Application Team & bure
*******************************************************************************/
#ifndef __USB_ISTR_H
#define __USB_ISTR_H

#include "msdusb_conf.h"

void CTR_HP(void);
void massUSB_Istr(void);

/* function prototypes Automatically built defining related macros */

#ifdef CTR_CALLBACK
void massCTR_Callback(void);
#endif

#ifdef DOVR_CALLBACK
void massDOVR_Callback(void);
#endif

#ifdef ERR_CALLBACK
void massERR_Callback(void);
#endif

#ifdef WKUP_CALLBACK
void massWKUP_Callback(void);
#endif

#ifdef SUSP_CALLBACK
void massSUSP_Callback(void);
#endif

#ifdef RESET_CALLBACK
void massRESET_Callback(void);
#endif

#ifdef SOF_CALLBACK
void massSOF_Callback(void);
#endif

#ifdef ESOF_CALLBACK
void massESOF_Callback(void);
#endif

void massEP1_IN_Callback(void);
void massEP2_IN_Callback(void);
void massEP3_IN_Callback(void);
void massEP4_IN_Callback(void);
void massEP5_IN_Callback(void);
void massEP6_IN_Callback(void);
void massEP7_IN_Callback(void);

void massEP1_OUT_Callback(void);
void massEP2_OUT_Callback(void);
void massEP3_OUT_Callback(void);
void massEP4_OUT_Callback(void);
void massEP5_OUT_Callback(void);
void massEP6_OUT_Callback(void);
void massEP7_OUT_Callback(void);

#endif
/*********************************  END OF FILE  ******************************/
