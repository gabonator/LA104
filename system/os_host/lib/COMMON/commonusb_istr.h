#pragma once
/**
  ******************************************************************************
  * @file    usb_istr.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   This file includes the peripherals header files in the user application.
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

/* Includes ------------------------------------------------------------------*/
#include "commonusb_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void common_USB_Istr(void);

#ifdef CTR_CALLBACK
void common_CTR_Callback(void);
#endif

#ifdef DOVR_CALLBACK
void common_DOVR_Callback(void);
#endif

#ifdef ERR_CALLBACK
void common_ERR_Callback(void);
#endif

#ifdef WKUP_CALLBACK
void common_WKUP_Callback(void);
#endif

#ifdef SUSP_CALLBACK
void common_SUSP_Callback(void);
#endif

#ifdef RESET_CALLBACK
void common_RESET_Callback(void);
#endif

#ifdef SOF_CALLBACK
void common_SOF_Callback(void);
#endif

#ifdef ESOF_CALLBACK
void common_ESOF_Callback(void);
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/