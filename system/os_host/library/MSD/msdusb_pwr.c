#include "msdusb_app.h"

#include <stdint.h>
#include <usb_core.h>

extern DEVICE_INFO massDevice_Info;

void mass_Enter_LowPowerMode(void)
{	/* Set the device state to suspend */
//TODO: check, called by mass storage or custom usb driver
//dbgPushPrint("enter_lp");
	massDeviceState = SUSPENDED;
}

void mass_Leave_LowPowerMode(void)
{
	/* Set the device state to the correct state */
	if (massDevice_Info.Current_Configuration != 0)
		massDeviceState = CONFIGURED;
	else
		massDeviceState = ATTACHED;

//dbgPushPrint("leave_lp %d ", cdcbDeviceState);
}
