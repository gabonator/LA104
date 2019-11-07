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
#include "hid_desc.h"
#include "hid_app.h"
#include "hid_conf.h"

void EP1_IN_Callback(void);
void EP2_IN_Callback(void);
void EP3_IN_Callback(void);
void EP4_IN_Callback(void);

void (*pEpInt_IN[7])(void) = {
	EP1_IN_Callback,
	EP2_IN_Callback,
	EP3_IN_Callback,
	EP4_IN_Callback,
	EP5_IN_Callback,
	EP6_IN_Callback,
	EP7_IN_Callback,
};

void (*pEpInt_OUT[7])(void) = {
	EP1_OUT_Callback,
	EP2_OUT_Callback,
	EP3_OUT_Callback,
	EP4_OUT_Callback,
	EP5_OUT_Callback,
	EP6_OUT_Callback,
	EP7_OUT_Callback,
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

//keyboard
void EP1_IN_Callback(void)
{
  /* Set the transfer complete token to inform upper layer that the current 
  transfer has been complete */
  kb_tx_complete = 1;
  if (kb_report_ready_cb) kb_report_ready_cb();
}

//mouse
void EP2_IN_Callback(void)
{
  /* Set the transfer complete token to inform upper layer that the current
  transfer has been complete */
  mouse_tx_complete = 1;
  if (mouse_report_ready_cb) mouse_report_ready_cb();
}

#ifndef CONFIG_ANNOYATRON
//joystick1
void EP3_IN_Callback(void)
{
  /* Set the transfer complete token to inform upper layer that the current
  transfer has been complete */
  joy1_tx_complete = 1;
  if (joy_report_ready_cb) joy_report_ready_cb(JOYSTICK1);
}

//joystick2
void EP4_IN_Callback(void)
{
  /* Set the transfer complete token to inform upper layer that the current
  transfer has been complete */
  joy2_tx_complete = 1;
  if (joy_report_ready_cb) joy_report_ready_cb(JOYSTICK2);
}
#endif