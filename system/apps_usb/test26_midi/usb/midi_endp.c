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
#include "midi_desc.h"
#include "midi_app.h"
#include "midi_conf.h"

extern volatile u8 rx_buffer_new_data;
extern volatile u8 tx_buffer_busy;
void MIOS32_USB_MIDI_EP1_IN_Callback(void);
void MIOS32_USB_MIDI_EP2_OUT_Callback(void);

void (*pEpInt_IN[7])(void) = {
	MIOS32_USB_MIDI_EP1_IN_Callback,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
};

void (*pEpInt_OUT[7])(void) = {
	NOP_Process,
	MIOS32_USB_MIDI_EP2_OUT_Callback,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
	NOP_Process,
};

void (*pCallbacks[8])(void) = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

void MIOS32_USB_MIDI_EP1_IN_Callback()
{
  // package has been sent
  tx_buffer_busy = 0;
  
  // check for next package
  MIOS32_USB_MIDI_TxBufferHandler();
}

void MIOS32_USB_MIDI_EP2_OUT_Callback()
{
  // put package into buffer
  rx_buffer_new_data = 1;
  MIOS32_USB_MIDI_RxBufferHandler();
}

