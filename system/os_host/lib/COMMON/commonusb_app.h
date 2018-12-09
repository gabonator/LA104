#pragma once
#include <stdint.h>
#include <usb_core.h>

typedef void (*TCallback)(void);

enum ECommonCallback { ccCTR, ccDOVR, ccERR, ccWKUP, ccSUSP, ccRESET, ccSOF, ccESOF };
extern TCallback common_Callback[8];  // iba 7!

void commmon_Initialize(DEVICE_PROP* prop, TCallback callback[]);

typedef enum _DEVICE_STATE {
	UNCONNECTED,
	ATTACHED,
	POWERED,
	SUSPENDED,
	ADDRESSED,
	CONFIGURED
} DEVICE_STATE;

