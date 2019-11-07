/**
  ******************************************************************************
  * @file    usb_conf.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Joystick Mouse demo configuration file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#define CONFIG_ANNOYATRON

#include "hid_app.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#ifndef CONFIG_ANNOYATRON
#ifdef CONFIG_ARCHID_VCD
#define EP_NUM     (8)
#else
#define EP_NUM     (5)
#endif
#else // CONFIG_ANNOYATRON
#define EP_NUM     (3)
#endif // CONFIG_ANNOYATRON

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

#define ENDP0_RXADDR        (0x40)//(0x18)
#define ENDP0_TXADDR        (0x80)//(0x58)
// keyboard endpoint
#define ENDP1_TXADDR        (0xc0)
// mouse endpoint
#define ENDP2_TXADDR        (0x100)

#ifndef CONFIG_ANNOYATRON
// joystick1 endpoint
#define ENDP3_TXADDR        (0x108)
// joystick2 endpoint
#define ENDP4_TXADDR        (0x110)

// vcd endpoints
#ifdef CONFIG_ARCHID_VCD
#define ENDP5_TXADDR        (0x118)
#define ENDP6_RXADDR        (0x120)
#define ENDP7_TXADDR        (0x160)
#endif
#endif // CONFIG_ANNOYATRON

/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )

#ifdef CONFIG_ARCHID_VCD
/*#define CTR_CALLBACK*/
/*#define DOVR_CALLBACK*/
/*#define ERR_CALLBACK*/
/*#define WKUP_CALLBACK*/
/*#define SUSP_CALLBACK*/
/*#define RESET_CALLBACK*/
#define SOF_CALLBACK
/*#define ESOF_CALLBACK*/
#endif

/* CTR service routines */
/* associated to defined endpoints */
// #define  EP1_IN_Callback   NOP_Process
// #define  EP2_IN_Callback   NOP_Process
#ifndef CONFIG_ANNOYATRON
//#define  EP3_IN_Callback   NOP_Process
//#define  EP4_IN_Callback   NOP_Process

#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process

#ifdef CONFIG_ARCHID_VCD
//#define  EP7_IN_Callback   NOP_Process
#else
#define  EP7_IN_Callback   NOP_Process
#endif

#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#ifdef CONFIG_ARCHID_VCD
//#define  EP6_OUT_Callback   NOP_Process
#else
#define  EP6_OUT_Callback   NOP_Process
#endif
#define  EP7_OUT_Callback   NOP_Process

#else // CONFIG_ANNOYATRON

#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process

#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process

#endif  // CONFIG_ANNOYATRON
#endif /*__USB_CONF_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
