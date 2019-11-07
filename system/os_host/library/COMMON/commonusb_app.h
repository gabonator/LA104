#pragma once
#include <stdint.h>
#include <usb_core.h>

typedef void (*TCallback)(void);

enum ECommonCallback { ccCTR, ccDOVR, ccERR, ccWKUP, ccSUSP, ccRESET, ccSOF, ccESOF };
extern TCallback common_Callback[8];  // iba 7!

void common_Initialize(DEVICE_PROP* prop, TCallback callback[]);

extern DEVICE_INFO* pInformation;
