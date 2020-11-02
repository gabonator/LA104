#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include <usb_lib.h>
#include <usb_type.h>
#include <stdint.h>
#include <stdbool.h>
#include "../COMMON/commonusb_app.h"

#define USART_RX_DATA_SIZE	256
#define USART_TX_DATA_SIZE	256
#define USB_RX_DATA_SIZE	256
#define USB_TX_DATA_SIZE	256

typedef struct RingBuffer_s {
	uint8_t * Buffer;
	uint16_t Size;
	uint16_t InIndex;
	uint16_t OutIndex;
	uint16_t Length;
} RingBuffer_t;

void USB_Config(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);

void USB_Cable_Config (FunctionalState NewState);

bool USB_Get_Data(uint8_t * data);
void USB_Receive_Data(uint8_t * src, uint8_t length);
void USB_Send_Data(const char * src, uint16_t length);
void USB_Send(const char * src);
bool USB_Send_Byte(uint8_t data);

void USART_Config(void);

bool USART_Get_Data(uint8_t * data);
void USART_Receive_Data(void);
void USART_Send_Data(const char * src, uint16_t length);
void USART_Send(const char * src);
bool USART_Send_Byte(uint8_t data);

void Handle_USBAsynchXfer (void);
void cdcGet_SerialNum(void);

uint32_t millis(void);
void delay(uint32_t ms);

void EnterSleepMode(void);

/////////////

typedef RESULT USB_RESULT;

extern DEVICE_INFO cdcDevice_Info;
extern __IO DEVICE_STATE cdcbDeviceState;
extern bool USB_Tx_Active;

#endif  /*__HW_CONFIG_H*/