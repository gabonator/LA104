#pragma once

#include <stdint.h>
#include <usb_lib.h>

#define false FALSE
#define true TRUE

typedef RESULT USB_RESULT;

typedef enum _DEVICE_STATE {
	UNCONNECTED,
	ATTACHED,
	POWERED,
	SUSPENDED,
	ADDRESSED,
	CONFIGURED
} DEVICE_STATE;

extern DEVICE_INFO* pDeviceInfo;
extern DEVICE_STATE bDeviceState;
//extern bool USB_Tx_Active;

s32 MIOS32_USB_MIDI_ChangeConnectionState(u8 connected);
s32 MIOS32_USB_MIDI_CheckAvailable(void);
s32 MIOS32_SYS_SerialNumberGet(char *str);
void MIOS32_USB_MIDI_TxBufferHandler();
void MIOS32_USB_MIDI_RxBufferHandler();
s32 MIOS32_USB_ForceSingleUSB(void);


void dbgPrint(const char* msg);
void dbgHex(int);
void InitializeFinish(int);

void Get_SerialNum();
