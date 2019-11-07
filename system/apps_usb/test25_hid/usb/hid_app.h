#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include <stdint.h>
#include <usb_lib.h>
#include "common.h"

extern uint8_t kb_led_state;
extern volatile uint8_t joy1_tx_complete;
extern volatile uint8_t joy2_tx_complete;
extern volatile uint8_t kb_tx_complete;
extern volatile uint8_t mouse_tx_complete;
extern usb_kb_report_ready_cb_f kb_report_ready_cb;
extern usb_mouse_report_ready_cb_f mouse_report_ready_cb;
extern usb_joy_report_ready_cb_f joy_report_ready_cb;


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

void dbgPrint(const char* msg, ...);
void dbgPushPrint(const char* msg, ...);
void InitializeFinish(int);

void Get_SerialNum();

#endif  /*__HW_CONFIG_H*/