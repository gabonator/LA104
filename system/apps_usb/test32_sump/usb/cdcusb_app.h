#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include <stdint.h>
#include <usb_lib.h>

typedef struct __attribute__((packed))
{
	uint32_t bitrate;
	uint8_t format;
	uint8_t paritytype;
	uint8_t datatype;
} LINE_CODING;

#define USART_RX_DATA_SIZE	256
#define USART_TX_DATA_SIZE	256
#define USB_RX_DATA_SIZE	256
#define USB_TX_DATA_SIZE	256

void USB_Config(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);

void USB_Cable_Config (FunctionalState NewState);


void Get_SerialNum(void);

uint32_t millis(void);
void delay(uint32_t ms);
void IRQ_Disable(void);
void IRQ_Enable(void);

void EnterSleepMode(void);

/////////////
typedef enum _DEVICE_STATE {
	UNCONNECTED,
	ATTACHED,
	POWERED,
	SUSPENDED,
	ADDRESSED,
	CONFIGURED
} DEVICE_STATE;

#define false FALSE
#define true TRUE

typedef RESULT USB_RESULT;

extern DEVICE_INFO cdcDevice_Info;
extern DEVICE_STATE cdcbDeviceState;
extern bool USB_Tx_Active;

void dbgPrint(const char* msg, ...);
void dbgPushPrint(const char* msg, ...);
void InitializeFinish(int);
#endif  /*__HW_CONFIG_H*/