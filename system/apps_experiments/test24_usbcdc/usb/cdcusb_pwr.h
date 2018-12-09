/**
 ******************************************************************************
 * @file    usb_pwr.h
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Connection/disconnection & power management header
 ******************************************************************************
 */
#ifndef __USB_PWR_H
#define __USB_PWR_H

#include <stdbool.h>
#include <usb_core.h>
#include "cdcusb_app.h"
#define PWR_Regulator_ON          ((uint32_t)0x00000000)
#define PWR_Regulator_LowPower    ((uint32_t)0x00000001)

typedef enum _RESUME_STATE {
	RESUME_EXTERNAL,
	RESUME_INTERNAL,
	RESUME_LATER,
	RESUME_WAIT,
	RESUME_START,
	RESUME_ON,
	RESUME_OFF,
	RESUME_ESOF
} RESUME_STATE;

void CDC_Suspend(void);
void CDC_Resume_Init(void);
void CDC_Resume(RESUME_STATE eResumeSetVal);
USB_RESULT CDC_PowerOn(void);
USB_RESULT CDC_PowerOff(void);

#endif  /*__USB_PWR_H*/