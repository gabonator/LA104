/**
 ******************************************************************************
 * @file		usb_prop.h
 * @author	MCD Application Team
 * @version V4.0.0
 * @date		21-January-2013
 * @brief	 All processing related to Virtual COM Port Demo (Endpoint 0)
 ******************************************************************************
 */
#ifndef __usb_prop_H
#define __usb_prop_H

#include <usb_core.h>
#include "sump_app.h"

#define CDC_GetConfiguration		NOP_Process
//	#define CDC_SetConfiguration	NOP_Process
#define CDC_GetInterface			NOP_Process
#define CDC_SetInterface			NOP_Process
#define CDC_GetStatus				NOP_Process
#define CDC_ClearFeature			NOP_Process
#define CDC_SetEndPointFeature		NOP_Process
#define CDC_SetDeviceFeature		NOP_Process
//	#define CDC_SetDeviceAddress	NOP_Process

#define SEND_ENCAPSULATED_COMMAND	0x00
#define GET_ENCAPSULATED_RESPONSE	0x01
#define SET_COMM_FEATURE			0x02
#define GET_COMM_FEATURE			0x03
#define CLEAR_COMM_FEATURE			0x04
#define SET_LINE_CODING				0x20
#define GET_LINE_CODING				0x21
#define SET_CONTROL_LINE_STATE		0x22
#define SEND_BREAK					0x23

void CDC_Init(void);
void CDC_Reset(void);
void CDC_SetConfiguration(void);
void CDC_SetDeviceAddress (void);
void CDC_Status_In (void);
void CDC_Status_Out (void);
RESULT CDC_Data_Setup(uint8_t);
RESULT CDC_NoData_Setup(uint8_t);
RESULT CDC_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);

uint8_t * CDC_GetDeviceDescriptor(uint16_t length);
uint8_t * CDC_GetConfigDescriptor(uint16_t length);
uint8_t * CDC_GetStringDescriptor(uint16_t length);

uint8_t * CDC_GetLineCoding(uint16_t length);
uint8_t * CDC_SetLineCoding(uint16_t length);

#endif /* __usb_prop_H */