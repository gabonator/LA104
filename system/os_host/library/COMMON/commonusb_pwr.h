#pragma once
/**
 ******************************************************************************
 * @file    usb_pwr.h
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Connection/disconnection & power management header
 ******************************************************************************
 */

#include <stdbool.h>
#include <usb_core.h>
//#include "cdcusb_app.h"

typedef enum _RESUME_STATE {
	RESUME_EXTERNAL,
	RESUME_INTERNAL,
	RESUME_LATER,
	RESUME_WAIT,
	RESUME_START,
	RESUME_ON,
	RESUME_OFF,
	RESUME_ESOF
} common_RESUME_STATE;

typedef RESULT USB_RESULT;

void common_Suspend(void);
void common_Resume_Init(void);
void common_Resume(common_RESUME_STATE eResumeSetVal);
USB_RESULT common_PowerOn(void);
USB_RESULT common_PowerOff(void);
void common_USB_Cable_Config (FunctionalState NewState);

// imports
extern void USB_Leave_LowPowerMode();
