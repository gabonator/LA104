/**
 ******************************************************************************
 * @file    usb_endp.c
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Endpoint routines
 ******************************************************************************
 */
#include <usb_lib.h>
#include <usb_core.h>
#include <usb_mem.h>
#include "cdcusb_desc.h"
#include "cdcusb_app.h"
#include "cdcusb_conf.h"
//#include "cdcusb_istr.h"
//#include "cdcusb_pwr.h"
#include "../COMMON/commonusb_pwr.h"

void cdcEP3_OUT_Callback(void);
void cdcEP1_IN_Callback (void);
void cdcSOF_Callback(void);

void (*cdcpEpInt_IN[7])(void) = {
	cdcEP1_IN_Callback,
	cdcEP2_IN_Callback,
	cdcEP3_IN_Callback,
	cdcEP4_IN_Callback,
	cdcEP5_IN_Callback,
	cdcEP6_IN_Callback,
	cdcEP7_IN_Callback,
};

void (*cdcpEpInt_OUT[7])(void) = {
	cdcEP1_OUT_Callback,
	cdcEP2_OUT_Callback,
	cdcEP3_OUT_Callback,
	cdcEP4_OUT_Callback,
	cdcEP5_OUT_Callback,
	cdcEP6_OUT_Callback,
	cdcEP7_OUT_Callback,
};

void (*cdcpCallbacks[8])(void) = {
  0,
  0,
  0,
  0,
  0,
  0,
  cdcSOF_Callback,
  0
};

int FrameCount = 0;
__IO DEVICE_STATE cdcbDeviceState = UNCONNECTED; /* USB device status */

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define CDC_IN_FRAME_INTERVAL	20

extern RingBuffer_t USB_Tx;
uint8_t USB_Rx_Tmp_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];

/******************************************************************************
 * @brief	EP1_IN_Callback
 */
void cdcEP1_IN_Callback (void)
{
	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;
	RingBuffer_t * ring = &USB_Tx;
	if (USB_Tx_Active)
	{
		if (ring->Length == 0)
			USB_Tx_Active = false;
		else
		{
			if (ring->Length > VIRTUAL_COM_PORT_DATA_SIZE)
			{
				USB_Tx_ptr = ring->OutIndex;
				USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

				ring->OutIndex += VIRTUAL_COM_PORT_DATA_SIZE;
				ring->Length -= VIRTUAL_COM_PORT_DATA_SIZE;
			}
			else 
			{
				USB_Tx_ptr = ring->OutIndex;
				USB_Tx_length = ring->Length;

				ring->OutIndex += ring->Length;
				ring->Length = 0;
			}
			UserToPMABufferCopy(&ring->Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
			SetEPTxCount(ENDP1, USB_Tx_length);
			SetEPTxValid(ENDP1); 
		}
	}
}

/******************************************************************************
 * @brief	EP3_OUT_Callback
 */
void cdcEP3_OUT_Callback(void)
{
	uint16_t USB_Rx_Cnt;

	/* Get the received data buffer and update the counter */
	USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Tmp_Buffer);

	/*	USB data will be immediately processed,
		this allow next USB traffic being NAKed till the end of the USART Xfer
	*/
	USB_Receive_Data(USB_Rx_Tmp_Buffer, USB_Rx_Cnt);

	/* Enable the receive of data on EP3 */
	SetEPRxValid(ENDP3);
}


/******************************************************************************
 * @brief	SOF_Callback / INTR_SOFINTR_Callback
 */
void cdcSOF_Callback(void)
{
	if (cdcbDeviceState == CONFIGURED)
	{
		if (FrameCount++ == CDC_IN_FRAME_INTERVAL)
		{	/* Reset the frame counter */
			FrameCount = 0;

			/* Check the data to be sent through IN pipe */
			Handle_USBAsynchXfer();
		}
	}
}
