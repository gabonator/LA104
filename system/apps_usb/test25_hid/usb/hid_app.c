#include "hid_app.h"
#include "hid_desc.h"
#include "hid_conf.h"

volatile uint8_t kb_tx_complete = 1;
volatile uint8_t mouse_tx_complete = 1;
volatile uint8_t joy1_tx_complete = 1;
volatile uint8_t joy2_tx_complete = 1;
usb_joy_report_ready_cb_f joy_report_ready_cb = NULL;
usb_kb_report_ready_cb_f kb_report_ready_cb = NULL;
usb_mouse_report_ready_cb_f mouse_report_ready_cb = NULL;
uint8_t kb_led_state = 0;

DEVICE_STATE bDeviceState;


void USB_ARC_set_kb_callback(usb_kb_report_ready_cb_f cb) {
  kb_report_ready_cb = cb;
}

void USB_ARC_set_mouse_callback(usb_mouse_report_ready_cb_f cb) {
  mouse_report_ready_cb = cb;
}

void USB_ARC_set_joystick_callback(usb_joy_report_ready_cb_f cb) {
  joy_report_ready_cb = cb;
}

bool USB_ARC_KB_tx(usb_kb_report *report)
{
  // byte 0:   modifiers
  // byte 1:   reserved (0x00)
  // byte 2-x: keypresses
  report->reserved = 0;

  uint32_t spoon_guard = 1000000;
  while(kb_tx_complete==0 && --spoon_guard);
  if (spoon_guard <= 0)
    return false;
  //ASSERT(spoon_guard > 0);

  /* Reset the control token to inform upper layer that a transfer is ongoing */
  kb_tx_complete = 0;

  /* Copy keyboard vector info in ENDP1 Tx Packet Memory Area*/
  USB_SIL_Write(EP1_IN, report->raw, sizeof(report->raw));

  /* Enable endpoint for transmission */
  SetEPTxValid(ENDP1);
  return true;
}

bool USB_ARC_MOUSE_tx(usb_mouse_report *report)
{
  uint32_t spoon_guard = 1000000;
  while(mouse_tx_complete==0 && --spoon_guard);
  if (spoon_guard <= 0)
    return false;

  //ASSERT(spoon_guard > 0);

  /* Reset the control token to inform upper layer that a transfer is ongoing */
  mouse_tx_complete = 0;

  /* Copy mouse position info in ENDP2 Tx Packet Memory Area*/
  USB_SIL_Write(EP2_IN, report->raw, sizeof(report->raw));

  /* Enable endpoint for transmission */
  SetEPTxValid(ENDP2);
  return true;
}

bool USB_ARC_JOYSTICK_tx(usb_joystick j, usb_joystick_report *report)
{
  uint32_t spoon_guard = 1000000;
  if (j == JOYSTICK1) {
    while(joy1_tx_complete==0 && --spoon_guard);
  } else {
    while(joy2_tx_complete==0 && --spoon_guard);
  }
  if (spoon_guard <= 0)
    return false;
  //ASSERT(spoon_guard > 0);

  /* Reset the control token to inform upper layer that a transfer is ongoing */
  if (j == JOYSTICK1) {
    joy1_tx_complete= 0;
  } else {
    joy2_tx_complete = 0;
  }

  /* Copy joystick info in ENDP3/4 Tx Packet Memory Area*/
  USB_SIL_Write(j == JOYSTICK1 ? EP3_IN : EP4_IN, report->raw, sizeof(report->raw));

  /* Enable endpoint for transmission */
  SetEPTxValid(j == JOYSTICK1 ? ENDP3 : ENDP4);
  return true;
}
