#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include "usb_conf.h"
#include "cdc.h"

#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)
#define USB_CDC_REQ_GET_LINE_CODING		0x21

cdc_receive_callback receive_callback = (cdc_receive_callback)0;
volatile bool cdc_sending = false;
usbd_device* transmit_device = (usbd_device*)0;

bool cdc_waitSync()
{
  for (uint32_t i=0; i<100000; i++) 
    if (!cdc_sending)
      return true;
  return false;
}

bool cdc_transmit(const uint8_t* buffer, int len)
{
  if (!cdc_waitSync())
  {
    // handle unreceived transmission
    return false;
  }

  cdc_sending = true;
  usbd_ep_write_packet(transmit_device, DATA_IN, buffer, len);
  return true;
}


static void cdcacm_data_tx_cb(usbd_device *usbd_dev, uint8_t ep)
{
  cdc_sending = false;
}

void cdc_set_receive_callback(cdc_receive_callback callback)
{
  receive_callback = callback;
}

//  Line config to be returned.
static const struct usb_cdc_line_coding line_coding = {
	.dwDTERate = 115200,
	.bCharFormat = USB_CDC_1_STOP_BITS,
	.bParityType = USB_CDC_NO_PARITY,
	.bDataBits = 0x08
};

int cdcacm_control_request(
  usbd_device *usbd_dev __attribute__((unused)),
  struct usb_setup_data *req,
  uint8_t **buf __attribute__((unused)),
  uint16_t *len,
  void (**complete)(
    usbd_device *usbd_dev,
    struct usb_setup_data *req
  ) __attribute__((unused))
) {
	//  Handle USB Control Requests
	switch (req->bRequest) {
		case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {
			/* From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
			* This Linux cdc_acm driver requires this to be implemented
			* even though it's optional in the CDC spec, and we don't
			* advertise it in the ACM functional descriptor. */
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_GET_LINE_CODING: {
			//  Windows requires this request, not Mac or Linux.
			//  From https://github.com/PX4/Bootloader/blob/master/stm32/cdcacm.c
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
			        assert(0);
				return USBD_REQ_NOTSUPP;
			}
			*buf = (uint8_t *) &line_coding;
			*len = sizeof(struct usb_cdc_line_coding);
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_SET_LINE_CODING: {
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
			        assert(0);
				return USBD_REQ_NOTSUPP;
			}
			return USBD_REQ_HANDLED;
		}
	}
	return USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
}

static char cdcbuf[MAX_USB_PACKET_SIZE];   // rx buffer

/*
 * USB Receive Callback:
 */
static void
cdcacm_data_rx_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
    uint16_t len = usbd_ep_read_packet(usbd_dev, DATA_OUT, cdcbuf, MAX_USB_PACKET_SIZE);
    if (len == 0) { return; }
    uint16_t pos = (len < MAX_USB_PACKET_SIZE) ? len : MAX_USB_PACKET_SIZE;
	
  if (receive_callback)
    receive_callback((uint8_t*)cdcbuf, pos);
}

static void
cdcacm_comm_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
}

/*
 * USB Configuration:
 */
static void
cdcacm_set_config(
  usbd_device *usbd_dev,
  uint16_t wValue __attribute__((unused))
) {
	usbd_ep_setup(usbd_dev, DATA_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, DATA_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_tx_cb);
	usbd_ep_setup(usbd_dev, COMM_IN, USB_ENDPOINT_ATTR_INTERRUPT, COMM_PACKET_SIZE, cdcacm_comm_cb);
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		cdcacm_control_request);

	if (status < 0) { assert(0); }
}

void cdc_setup(usbd_device* usbd_dev) {
	int status = aggregate_register_config_callback(usbd_dev, cdcacm_set_config);
	if (status < 0) { assert(0); }
	transmit_device = usbd_dev;
}
