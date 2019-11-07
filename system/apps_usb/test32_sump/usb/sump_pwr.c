#include "sump_app.h"

#include <stdint.h>
#include <usb_core.h>

extern DEVICE_INFO cdcDevice_Info;

void Enter_LowPowerMode(void)
{	
	/* Set the device state to suspend */
	cdcbDeviceState = SUSPENDED;
}

void Leave_LowPowerMode(void)
{
	/* Set the device state to the correct state */
	if (cdcDevice_Info.Current_Configuration != 0)
		cdcbDeviceState = CONFIGURED;
	else
		cdcbDeviceState = ATTACHED;
}
