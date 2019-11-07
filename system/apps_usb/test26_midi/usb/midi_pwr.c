#include "midi_app.h"

#include <stdint.h>
#include <usb_core.h>

extern DEVICE_INFO Device_Info;

void Enter_LowPowerMode(void)
{	
	/* Set the device state to suspend */
	bDeviceState = SUSPENDED;
}

void Leave_LowPowerMode(void)
{
	/* Set the device state to the correct state */
	if (Device_Info.Current_Configuration != 0)
		bDeviceState = CONFIGURED;
	else
		bDeviceState = ATTACHED;
}
