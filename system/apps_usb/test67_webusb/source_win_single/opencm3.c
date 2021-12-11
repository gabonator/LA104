#include "opencm3.h"
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Tomaz Solc <tomaz.solc@tablix.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


void __attribute__((weak)) cm3_assert_failed(void)
{
	while (1);
}

void __attribute__((weak)) cm3_assert_failed_verbose(
		const char *file __attribute__((unused)),
		int line __attribute__((unused)),
		const char *func __attribute__((unused)),
		const char *assert_expr __attribute__((unused)))
{
	cm3_assert_failed();
}
/** @defgroup usb_control_file Generic USB Control Requests

@ingroup USB

@brief <b>Generic USB Control Requests</b>

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010
Gareth McMullin <gareth@blacksphere.co.nz>

@date 10 March 2013

LGPL License Terms @ref lgpl_license
*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/


/*
 * According to the USB 2.0 specification, section 8.5.3, when a control
 * transfer is stalled, the pipe becomes idle. We provide one utility to stall
 * a transaction to reduce boilerplate code.
 */
static void stall_transaction(usbd_device *usbd_dev)
{
	usbd_ep_stall_set(usbd_dev, 0, 1);
	usbd_dev->control_state.state = IDLE;
}

/**
 * If we're replying with _some_ data, but less than the host is expecting,
 * then we normally just do a short transfer.  But if it's short, but a
 * multiple of the endpoint max packet size, we need an explicit ZLP.
 * @param len how much data we want to transfer
 * @param wLength how much the host asked for
 * @param ep_size
 * @return
 */
static bool needs_zlp(uint16_t len, uint16_t wLength, uint8_t ep_size)
{
	if (len < wLength) {
		if (len && (len % ep_size == 0)) {
			return true;
		}
	}
	return false;
}

/* Register application callback function for handling USB control requests. */
int usbd_register_control_callback(usbd_device *usbd_dev, uint8_t type,
				   uint8_t type_mask,
				   usbd_control_callback callback)
{
	int i;

	for (i = 0; i < MAX_USER_CONTROL_CALLBACK; i++) {
		if (usbd_dev->user_control_callback[i].cb) {
			continue;
		}

		usbd_dev->user_control_callback[i].type = type;
		usbd_dev->user_control_callback[i].type_mask = type_mask;
		usbd_dev->user_control_callback[i].cb = callback;
		return 0;
	}

	return -1;
}

static void usb_control_send_chunk(usbd_device *usbd_dev)
{
	if (usbd_dev->desc->bMaxPacketSize0 <
			usbd_dev->control_state.ctrl_len) {
		/* Data stage, normal transmission */
		usbd_ep_write_packet(usbd_dev, 0,
				     usbd_dev->control_state.ctrl_buf,
				     usbd_dev->desc->bMaxPacketSize0);
		usbd_dev->control_state.state = DATA_IN;
		usbd_dev->control_state.ctrl_buf +=
			usbd_dev->desc->bMaxPacketSize0;
		usbd_dev->control_state.ctrl_len -=
			usbd_dev->desc->bMaxPacketSize0;
	} else {
		/* Data stage, end of transmission */
		usbd_ep_write_packet(usbd_dev, 0,
				     usbd_dev->control_state.ctrl_buf,
				     usbd_dev->control_state.ctrl_len);

		usbd_dev->control_state.state =
			usbd_dev->control_state.needs_zlp ?
			DATA_IN : LAST_DATA_IN;
		usbd_dev->control_state.needs_zlp = false;
		usbd_dev->control_state.ctrl_len = 0;
		usbd_dev->control_state.ctrl_buf = NULL;
	}
}

static int usb_control_recv_chunk(usbd_device *usbd_dev)
{
	uint16_t packetsize = MIN(usbd_dev->desc->bMaxPacketSize0,
			usbd_dev->control_state.req.wLength -
			usbd_dev->control_state.ctrl_len);
	uint16_t size = usbd_ep_read_packet(usbd_dev, 0,
				       usbd_dev->control_state.ctrl_buf +
				       usbd_dev->control_state.ctrl_len,
				       packetsize);

	if (size != packetsize) {
		stall_transaction(usbd_dev);
		return -1;
	}

	usbd_dev->control_state.ctrl_len += size;

	return packetsize;
}

static int usb_control_request_dispatch(usbd_device *usbd_dev,
					struct usb_setup_data *req)
{
	int i, result = 0;
	struct user_control_callback *cb = usbd_dev->user_control_callback;

	/* Call user command hook function. */
	for (i = 0; i < MAX_USER_CONTROL_CALLBACK; i++) {
		if (cb[i].cb == NULL) {
			break;
		}

		if ((req->bmRequestType & cb[i].type_mask) == cb[i].type) {
			result = cb[i].cb(usbd_dev, req,
					  &(usbd_dev->control_state.ctrl_buf),
					  &(usbd_dev->control_state.ctrl_len),
					  &(usbd_dev->control_state.complete));
			if (result == USBD_REQ_HANDLED ||
			    result == USBD_REQ_NOTSUPP) {
				return result;
			}
		}
	}

	/* Try standard request if not already handled. */
	return _usbd_standard_request(usbd_dev, req,
				      &(usbd_dev->control_state.ctrl_buf),
				      &(usbd_dev->control_state.ctrl_len));
}

/* Handle commands and read requests. */
static void usb_control_setup_read(usbd_device *usbd_dev,
		struct usb_setup_data *req)
{
	usbd_dev->control_state.ctrl_buf = usbd_dev->ctrl_buf;
	usbd_dev->control_state.ctrl_len = req->wLength;

	if (usb_control_request_dispatch(usbd_dev, req)) {
		if (req->wLength) {
			usbd_dev->control_state.needs_zlp =
				needs_zlp(usbd_dev->control_state.ctrl_len,
					req->wLength,
					usbd_dev->desc->bMaxPacketSize0);
			/* Go to data out stage if handled. */
			usb_control_send_chunk(usbd_dev);
		} else {
			/* Go to status stage if handled. */
			usbd_ep_write_packet(usbd_dev, 0, NULL, 0);
			usbd_dev->control_state.state = STATUS_IN;
		}
	} else {
		/* Stall endpoint on failure. */
		stall_transaction(usbd_dev);
	}
}

static void usb_control_setup_write(usbd_device *usbd_dev,
				    struct usb_setup_data *req)
{
	if (req->wLength > usbd_dev->ctrl_buf_len) {
		stall_transaction(usbd_dev);
		return;
	}

	/* Buffer into which to write received data. */
	usbd_dev->control_state.ctrl_buf = usbd_dev->ctrl_buf;
	usbd_dev->control_state.ctrl_len = 0;
	/* Wait for DATA OUT stage. */
	if (req->wLength > usbd_dev->desc->bMaxPacketSize0) {
		usbd_dev->control_state.state = DATA_OUT;
	} else {
		usbd_dev->control_state.state = LAST_DATA_OUT;
	}
}

/* Do not appear to belong to the API, so are omitted from docs */
/**@}*/

void _usbd_control_setup(usbd_device *usbd_dev, uint8_t ea)
{
	struct usb_setup_data *req = &usbd_dev->control_state.req;
	(void)ea;

	usbd_dev->control_state.complete = NULL;

	if (usbd_ep_read_packet(usbd_dev, 0, req, 8) != 8) {
		stall_transaction(usbd_dev);
		return;
	}

	if (req->wLength == 0) {
		usb_control_setup_read(usbd_dev, req);
	} else if (req->bmRequestType & 0x80) {
		usb_control_setup_read(usbd_dev, req);
	} else {
		usb_control_setup_write(usbd_dev, req);
	}
}

void _usbd_control_out(usbd_device *usbd_dev, uint8_t ea)
{
	(void)ea;

	switch (usbd_dev->control_state.state) {
	case DATA_OUT:
		if (usb_control_recv_chunk(usbd_dev) < 0) {
			break;
		}
		if ((usbd_dev->control_state.req.wLength -
					usbd_dev->control_state.ctrl_len) <=
					usbd_dev->desc->bMaxPacketSize0) {
			usbd_dev->control_state.state = LAST_DATA_OUT;
		}
		break;
	case LAST_DATA_OUT:
		if (usb_control_recv_chunk(usbd_dev) < 0) {
			break;
		}
		/*
		 * We have now received the full data payload.
		 * Invoke callback to process.
		 */
		if (usb_control_request_dispatch(usbd_dev,
					&(usbd_dev->control_state.req))) {
			/* Got to status stage on success. */
			usbd_ep_write_packet(usbd_dev, 0, NULL, 0);
			usbd_dev->control_state.state = STATUS_IN;
		} else {
			stall_transaction(usbd_dev);
		}
		break;
	case STATUS_OUT:
		usbd_ep_read_packet(usbd_dev, 0, NULL, 0);
		usbd_dev->control_state.state = IDLE;
		if (usbd_dev->control_state.complete) {
			usbd_dev->control_state.complete(usbd_dev,
					&(usbd_dev->control_state.req));
		}
		usbd_dev->control_state.complete = NULL;
		break;
	default:
		stall_transaction(usbd_dev);
	}
}

void _usbd_control_in(usbd_device *usbd_dev, uint8_t ea)
{
	(void)ea;
	struct usb_setup_data *req = &(usbd_dev->control_state.req);

	switch (usbd_dev->control_state.state) {
	case DATA_IN:
		usb_control_send_chunk(usbd_dev);
		break;
	case LAST_DATA_IN:
		usbd_dev->control_state.state = STATUS_OUT;
		break;
	case STATUS_IN:
		if (usbd_dev->control_state.complete) {
			usbd_dev->control_state.complete(usbd_dev,
					&(usbd_dev->control_state.req));
		}

		/* Exception: Handle SET ADDRESS function here... */
		if ((req->bmRequestType == 0) &&
		    (req->bRequest == USB_REQ_SET_ADDRESS)) {
			usbd_dev->driver->set_address(usbd_dev, req->wValue);
		}
		usbd_dev->control_state.state = IDLE;
		break;
	default:
		stall_transaction(usbd_dev);
	}
}

/** @defgroup usb_drivers_file Generic USB Drivers

@ingroup USB

@brief <b>Generic USB Drivers</b>

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010
Gareth McMullin <gareth@blacksphere.co.nz>

@date 10 March 2013

LGPL License Terms @ref lgpl_license
*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/


usbd_device *usbd_init(const usbd_driver *driver,
		       const struct usb_device_descriptor *dev,
		       const struct usb_config_descriptor *conf,
               const struct usb_bos_descriptor *bos,
		       const char **strings, int num_strings,
		       uint8_t *control_buffer, uint16_t control_buffer_size)
{
	usbd_device *usbd_dev;

	usbd_dev = driver->init();

	usbd_dev->driver = driver;
	usbd_dev->desc = dev;
	usbd_dev->config = conf;
    usbd_dev->bos = bos;
	usbd_dev->strings = strings;
	usbd_dev->num_strings = num_strings;
	usbd_dev->extra_string_idx = -1;
	usbd_dev->extra_string = NULL;
	usbd_dev->ctrl_buf = control_buffer;
	usbd_dev->ctrl_buf_len = control_buffer_size;

	usbd_dev->user_callback_ctr[0][USB_TRANSACTION_SETUP] =
	    _usbd_control_setup;
	usbd_dev->user_callback_ctr[0][USB_TRANSACTION_OUT] =
	    _usbd_control_out;
	usbd_dev->user_callback_ctr[0][USB_TRANSACTION_IN] =
	    _usbd_control_in;

	int i;
	for (i = 0; i < MAX_USER_SET_CONFIG_CALLBACK; i++) {
		usbd_dev->user_callback_set_config[i] = NULL;
	}

	return usbd_dev;
}

void usbd_register_reset_callback(usbd_device *usbd_dev, void (*callback)(void))
{
	usbd_dev->user_callback_reset = callback;
}

void usbd_register_suspend_callback(usbd_device *usbd_dev,
				    void (*callback)(void))
{
	usbd_dev->user_callback_suspend = callback;
}

void usbd_register_resume_callback(usbd_device *usbd_dev,
				   void (*callback)(void))
{
	usbd_dev->user_callback_resume = callback;
}

void usbd_register_sof_callback(usbd_device *usbd_dev, void (*callback)(void))
{
	usbd_dev->user_callback_sof = callback;
}

void usbd_register_extra_string(usbd_device *usbd_dev, int index, const char* string)
{
	if (string != NULL && index > 0)
	{
		usbd_dev->extra_string_idx = index;
		usbd_dev->extra_string = string;
	}
	else
	{
		usbd_dev->extra_string_idx = -1;
	}
}

void _usbd_reset(usbd_device *usbd_dev)
{
	usbd_dev->current_address = 0;
	usbd_dev->current_config = 0;
	usbd_ep_setup(usbd_dev, 0, USB_ENDPOINT_ATTR_CONTROL, usbd_dev->desc->bMaxPacketSize0, NULL);
	usbd_dev->driver->set_address(usbd_dev, 0);

	if (usbd_dev->user_callback_reset) {
		usbd_dev->user_callback_reset();
	}
}

/* Functions to wrap the low-level driver */
void usbd_poll(usbd_device *usbd_dev)
{
	usbd_dev->driver->poll(usbd_dev);
}

void usbd_disconnect(usbd_device *usbd_dev, bool disconnected)
{
	/* not all drivers support disconnection */
	if (usbd_dev->driver->disconnect) {
		usbd_dev->driver->disconnect(usbd_dev, disconnected);
	}
}

void usbd_ep_setup(usbd_device *usbd_dev, uint8_t addr, uint8_t type,
		   uint16_t max_size, usbd_endpoint_callback callback)
{
	usbd_dev->driver->ep_setup(usbd_dev, addr, type, max_size, callback);
}

uint16_t usbd_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
			 const void *buf, uint16_t len)
{
	return usbd_dev->driver->ep_write_packet(usbd_dev, addr, buf, len);
}

uint16_t usbd_ep_read_packet(usbd_device *usbd_dev, uint8_t addr, void *buf,
			     uint16_t len)
{
	return usbd_dev->driver->ep_read_packet(usbd_dev, addr, buf, len);
}

void usbd_ep_stall_set(usbd_device *usbd_dev, uint8_t addr, uint8_t stall)
{
	usbd_dev->driver->ep_stall_set(usbd_dev, addr, stall);
}

uint8_t usbd_ep_stall_get(usbd_device *usbd_dev, uint8_t addr)
{
	return usbd_dev->driver->ep_stall_get(usbd_dev, addr);
}

void usbd_ep_nak_set(usbd_device *usbd_dev, uint8_t addr, uint8_t nak)
{
	usbd_dev->driver->ep_nak_set(usbd_dev, addr, nak);
}

/**@}*/

/** @defgroup usb_standard_file Generic USB Standard Request Interface

@ingroup USB

@brief <b>Generic USB Standard Request Interface</b>

@version 1.0.0

@author @htmlonly &copy; @endhtmlonly 2010
Gareth McMullin <gareth@blacksphere.co.nz>

@date 10 March 2013

LGPL License Terms @ref lgpl_license
*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/


int usbd_register_set_config_callback(usbd_device *usbd_dev,
				       usbd_set_config_callback callback)
{
	int i;

	for (i = 0; i < MAX_USER_SET_CONFIG_CALLBACK; i++) {
		if (usbd_dev->user_callback_set_config[i]) {
			continue;
		}

		usbd_dev->user_callback_set_config[i] = callback;
		return 0;
	}

	return -1;
}

void usbd_register_set_altsetting_callback(usbd_device *usbd_dev,
					usbd_set_altsetting_callback callback)
{
	usbd_dev->user_callback_set_altsetting = callback;
}

static uint16_t build_config_descriptor(usbd_device *usbd_dev,
				   uint8_t index, uint8_t *buf, uint16_t len)
{
	uint8_t *tmpbuf = buf;
	const struct usb_config_descriptor *cfg = &usbd_dev->config[index];
	uint16_t count, total = 0, totallen = 0;
	uint16_t i, j, k;

	memcpy(buf, cfg, count = MIN(len, cfg->bLength));
	buf += count;
	len -= count;
	total += count;
	totallen += cfg->bLength;

	/* For each interface... */
	for (i = 0; i < cfg->bNumInterfaces; i++) {
		/* Interface Association Descriptor, if any */
		if (cfg->interface[i].iface_assoc) {
			const struct usb_iface_assoc_descriptor *assoc =
					cfg->interface[i].iface_assoc;
			memcpy(buf, assoc, count = MIN(len, assoc->bLength));
			buf += count;
			len -= count;
			total += count;
			totallen += assoc->bLength;
		}
		/* For each alternate setting... */
		for (j = 0; j < cfg->interface[i].num_altsetting; j++) {
			const struct usb_interface_descriptor *iface =
					&cfg->interface[i].altsetting[j];
			/* Copy interface descriptor. */
			memcpy(buf, iface, count = MIN(len, iface->bLength));
			buf += count;
			len -= count;
			total += count;
			totallen += iface->bLength;
			/* Copy extra bytes (function descriptors). */
			if (iface->extra) {
				memcpy(buf, iface->extra,
				       count = MIN(len, iface->extralen));
				buf += count;
				len -= count;
				total += count;
				totallen += iface->extralen;
			}
			/* For each endpoint... */
			for (k = 0; k < iface->bNumEndpoints; k++) {
				const struct usb_endpoint_descriptor *ep =
				    &iface->endpoint[k];
				memcpy(buf, ep, count = MIN(len, ep->bLength));
				buf += count;
				len -= count;
				total += count;
				totallen += ep->bLength;
				/* Copy extra bytes (class specific). */
				if (ep->extra) {
					memcpy(buf, ep->extra,
					       count = MIN(len, ep->extralen));
					buf += count;
					len -= count;
					total += count;
					totallen += ep->extralen;
				}
			}
		}
	}

	/* Fill in wTotalLength. */
	*(uint16_t *)(tmpbuf + 2) = totallen;

	return total;
}

static uint16_t build_bos_descriptor(const struct usb_bos_descriptor *bos,
                                     uint8_t *buf, uint16_t len)
{
	uint8_t *tmpbuf = buf;
	uint16_t count, total = 0, totallen = 0;
	uint16_t i;

	memcpy(buf, bos, count = MIN(len, bos->bLength));
	buf += count;
	len -= count;
	total += count;
	totallen += bos->bLength;

	/* For each device capability */
	for (i = 0; i < bos->bNumDeviceCaps; i++) {
        /* Copy device capability descriptor. */
        const struct usb_device_capability_descriptor *cap =
            bos->capabilities[i];
        
        memcpy(buf, cap, count = MIN(len, cap->bLength));
        buf += count;
        len -= count;
        total += count;
        totallen += cap->bLength;
	}

	/* Fill in wTotalLength. */
	*(uint16_t *)(tmpbuf + 2) = totallen;

	return total;
}

static int usb_descriptor_type(uint16_t wValue)
{
	return wValue >> 8;
}

static int usb_descriptor_index(uint16_t wValue)
{
	return wValue & 0xFF;
}

static int usb_standard_get_descriptor(usbd_device *usbd_dev,
				       struct usb_setup_data *req,
				       uint8_t **buf, uint16_t *len)
{
	int i, array_idx, descr_idx;
	struct usb_string_descriptor *sd;

	descr_idx = usb_descriptor_index(req->wValue);

	switch (usb_descriptor_type(req->wValue)) {
	case USB_DT_DEVICE:
		*buf = (uint8_t *) usbd_dev->desc;
		*len = MIN(*len, usbd_dev->desc->bLength);
		return USBD_REQ_HANDLED;
	case USB_DT_CONFIGURATION:
		*buf = usbd_dev->ctrl_buf;
		*len = build_config_descriptor(usbd_dev, descr_idx, *buf, *len);
		return USBD_REQ_HANDLED;
    case USB_DT_BOS:
        if (!usbd_dev->bos) {
            return USBD_REQ_NOTSUPP;
        }
        *buf = usbd_dev->ctrl_buf;
        *len = build_bos_descriptor(usbd_dev->bos, *buf, *len);
        return USBD_REQ_HANDLED;
	case USB_DT_STRING:
		sd = (struct usb_string_descriptor *)usbd_dev->ctrl_buf;

		if (descr_idx == 0) {
			/* Send sane Language ID descriptor... */
			sd->wData[0] = USB_LANGID_ENGLISH_US;
			sd->bLength = sizeof(sd->bLength) +
				      sizeof(sd->bDescriptorType) +
				      sizeof(sd->wData[0]);

			*len = MIN(*len, sd->bLength);
		} else if (descr_idx == usbd_dev->extra_string_idx) {
			/* This string is returned as UTF16, hence the
			 * multiplication
			 */
			sd->bLength = strlen(usbd_dev->extra_string) * 2 +
				      sizeof(sd->bLength) +
				      sizeof(sd->bDescriptorType);

			*len = MIN(*len, sd->bLength);

			for (i = 0; i < (*len / 2) - 1; i++) {
				sd->wData[i] =
					usbd_dev->extra_string[i];
			}
        } else {
			array_idx = descr_idx - 1;

			if (!usbd_dev->strings) {
				/* Device doesn't support strings. */
				return USBD_REQ_NOTSUPP;
			}

			/* Check that string index is in range. */
			if (array_idx >= usbd_dev->num_strings) {
				return USBD_REQ_NOTSUPP;
			}

			/* Strings with Language ID differnet from
			 * USB_LANGID_ENGLISH_US are not supported */
			if (req->wIndex != USB_LANGID_ENGLISH_US) {
				return USBD_REQ_NOTSUPP;
			}

			/* This string is returned as UTF16, hence the
			 * multiplication
			 */
			sd->bLength = strlen(usbd_dev->strings[array_idx]) * 2 +
				      sizeof(sd->bLength) +
				      sizeof(sd->bDescriptorType);

			*len = MIN(*len, sd->bLength);

			for (i = 0; i < (*len / 2) - 1; i++) {
				sd->wData[i] =
					usbd_dev->strings[array_idx][i];
			}
		}

		sd->bDescriptorType = USB_DT_STRING;
		*buf = (uint8_t *)sd;

		return USBD_REQ_HANDLED;
	}
	return USBD_REQ_NOTSUPP;
}

static int usb_standard_set_address(usbd_device *usbd_dev,
				    struct usb_setup_data *req, uint8_t **buf,
				    uint16_t *len)
{
	(void)req;
	(void)buf;
	(void)len;

	/* The actual address is only latched at the STATUS IN stage. */
	if ((req->bmRequestType != 0) || (req->wValue >= 128)) {
		return 0;
	}

	usbd_dev->current_address = req->wValue;

	/*
	 * Special workaround for STM32F10[57] that require the address
	 * to be set here. This is undocumented!
	 */
	if (usbd_dev->driver->set_address_before_status) {
		usbd_dev->driver->set_address(usbd_dev, req->wValue);
	}

	return 1;
}

static int usb_standard_set_configuration(usbd_device *usbd_dev,
					  struct usb_setup_data *req,
					  uint8_t **buf, uint16_t *len)
{
	unsigned i;
	int found_index = -1;
	const struct usb_config_descriptor *cfg;

	(void)req;
	(void)buf;
	(void)len;

	if (req->wValue > 0) {
		for (i = 0; i < usbd_dev->desc->bNumConfigurations; i++) {
			if (req->wValue
			    == usbd_dev->config[i].bConfigurationValue) {
				found_index = i;
				break;
			}
		}
		if (found_index < 0) {
			return USBD_REQ_NOTSUPP;
		}
	}

	usbd_dev->current_config = found_index + 1;

	if (usbd_dev->current_config > 0) {
		cfg = &usbd_dev->config[usbd_dev->current_config - 1];

		/* reset all alternate settings configuration */
		for (i = 0; i < cfg->bNumInterfaces; i++) {
			if (cfg->interface[i].cur_altsetting) {
				*cfg->interface[i].cur_altsetting = 0;
			}
		}
	}

	/* Reset all endpoints. */
	usbd_dev->driver->ep_reset(usbd_dev);

	if (usbd_dev->user_callback_set_config[0]) {
		/*
		 * Flush control callbacks. These will be reregistered
		 * by the user handler.
		 */
		for (i = 0; i < MAX_USER_CONTROL_CALLBACK; i++) {
			usbd_dev->user_control_callback[i].cb = NULL;
		}

		for (i = 0; i < MAX_USER_SET_CONFIG_CALLBACK; i++) {
			if (usbd_dev->user_callback_set_config[i]) {
				usbd_dev->user_callback_set_config[i](usbd_dev,
								req->wValue);
			}
		}
	}

	return 1;
}

static int usb_standard_get_configuration(usbd_device *usbd_dev,
					  struct usb_setup_data *req,
					  uint8_t **buf, uint16_t *len)
{
	(void)req;

	if (*len > 1) {
		*len = 1;
	}
	if (usbd_dev->current_config > 0) {
		const struct usb_config_descriptor *cfg =
			&usbd_dev->config[usbd_dev->current_config - 1];
		(*buf)[0] = cfg->bConfigurationValue;
	} else {
		(*buf)[0] = 0;
	}

	return 1;
}

static int usb_standard_set_interface(usbd_device *usbd_dev,
				      struct usb_setup_data *req,
				      uint8_t **buf, uint16_t *len)
{
	const struct usb_config_descriptor *cfx =
		&usbd_dev->config[usbd_dev->current_config - 1];
	const struct usb_interface *iface;

	(void)buf;

	if (req->wIndex >= cfx->bNumInterfaces) {
		return USBD_REQ_NOTSUPP;
	}

	iface = &cfx->interface[req->wIndex];

	if (req->wValue >= iface->num_altsetting) {
		return USBD_REQ_NOTSUPP;
	}

	if (iface->cur_altsetting) {
		*iface->cur_altsetting = req->wValue;
	} else if (req->wValue > 0) {
		return USBD_REQ_NOTSUPP;
	}

	if (usbd_dev->user_callback_set_altsetting) {
			usbd_dev->user_callback_set_altsetting(usbd_dev,
							       req->wIndex,
							       req->wValue);
	}

	*len = 0;

	return USBD_REQ_HANDLED;
}

static int usb_standard_get_interface(usbd_device *usbd_dev,
				      struct usb_setup_data *req,
				      uint8_t **buf, uint16_t *len)
{
	uint8_t *cur_altsetting;
	const struct usb_config_descriptor *cfx =
		&usbd_dev->config[usbd_dev->current_config - 1];

	if (req->wIndex >= cfx->bNumInterfaces) {
		return USBD_REQ_NOTSUPP;
	}

	*len = 1;
	cur_altsetting = cfx->interface[req->wIndex].cur_altsetting;
	(*buf)[0] = (cur_altsetting) ? *cur_altsetting : 0;

	return USBD_REQ_HANDLED;
}

static int usb_standard_device_get_status(usbd_device *usbd_dev,
					  struct usb_setup_data *req,
					  uint8_t **buf, uint16_t *len)
{
	(void)usbd_dev;
	(void)req;

	/* bit 0: self powered */
	/* bit 1: remote wakeup */
	if (*len > 2) {
		*len = 2;
	}
	(*buf)[0] = 0;
	(*buf)[1] = 0;

	return 1;
}

static int usb_standard_interface_get_status(usbd_device *usbd_dev,
					     struct usb_setup_data *req,
					     uint8_t **buf, uint16_t *len)
{
	(void)usbd_dev;
	(void)req;
	/* not defined */

	if (*len > 2) {
		*len = 2;
	}
	(*buf)[0] = 0;
	(*buf)[1] = 0;

	return 1;
}

static int usb_standard_endpoint_get_status(usbd_device *usbd_dev,
					    struct usb_setup_data *req,
					    uint8_t **buf, uint16_t *len)
{
	(void)req;

	if (*len > 2) {
		*len = 2;
	}
	(*buf)[0] = usbd_ep_stall_get(usbd_dev, req->wIndex) ? 1 : 0;
	(*buf)[1] = 0;

	return 1;
}

static int usb_standard_endpoint_stall(usbd_device *usbd_dev,
				       struct usb_setup_data *req,
				       uint8_t **buf, uint16_t *len)
{
	(void)buf;
	(void)len;

	usbd_ep_stall_set(usbd_dev, req->wIndex, 1);

	return 1;
}

static int usb_standard_endpoint_unstall(usbd_device *usbd_dev,
					 struct usb_setup_data *req,
					 uint8_t **buf, uint16_t *len)
{
	(void)buf;
	(void)len;

	usbd_ep_stall_set(usbd_dev, req->wIndex, 0);

	return 1;
}

/* Do not appear to belong to the API, so are omitted from docs */
/**@}*/

int _usbd_standard_request_device(usbd_device *usbd_dev,
				  struct usb_setup_data *req, uint8_t **buf,
				  uint16_t *len)
{
	int (*command)(usbd_device *usbd_dev, struct usb_setup_data *req,
		       uint8_t **buf, uint16_t *len) = NULL;

	switch (req->bRequest) {
	case USB_REQ_CLEAR_FEATURE:
	case USB_REQ_SET_FEATURE:
		if (req->wValue == USB_FEAT_DEVICE_REMOTE_WAKEUP) {
			/* Device wakeup code goes here. */
		}

		if (req->wValue == USB_FEAT_TEST_MODE) {
			/* Test mode code goes here. */
		}

		break;
	case USB_REQ_SET_ADDRESS:
		/*
		 * SET ADDRESS is an exception.
		 * It is only processed at STATUS stage.
		 */
		command = usb_standard_set_address;
		break;
	case USB_REQ_SET_CONFIGURATION:
		command = usb_standard_set_configuration;
		break;
	case USB_REQ_GET_CONFIGURATION:
		command = usb_standard_get_configuration;
		break;
	case USB_REQ_GET_DESCRIPTOR:
		command = usb_standard_get_descriptor;
		break;
	case USB_REQ_GET_STATUS:
		/*
		 * GET_STATUS always responds with zero reply.
		 * The application may override this behaviour.
		 */
		command = usb_standard_device_get_status;
		break;
	case USB_REQ_SET_DESCRIPTOR:
		/* SET_DESCRIPTOR is optional and not implemented. */
		break;
	}

	if (!command) {
		return 0;
	}

	return command(usbd_dev, req, buf, len);
}

int _usbd_standard_request_interface(usbd_device *usbd_dev,
				     struct usb_setup_data *req, uint8_t **buf,
				     uint16_t *len)
{
	int (*command)(usbd_device *usbd_dev, struct usb_setup_data *req,
		       uint8_t **buf, uint16_t *len) = NULL;

	switch (req->bRequest) {
	case USB_REQ_CLEAR_FEATURE:
	case USB_REQ_SET_FEATURE:
		/* not defined */
		break;
	case USB_REQ_GET_INTERFACE:
		command = usb_standard_get_interface;
		break;
	case USB_REQ_SET_INTERFACE:
		command = usb_standard_set_interface;
		break;
	case USB_REQ_GET_STATUS:
		command = usb_standard_interface_get_status;
		break;
	}

	if (!command) {
		return 0;
	}

	return command(usbd_dev, req, buf, len);
}

int _usbd_standard_request_endpoint(usbd_device *usbd_dev,
				    struct usb_setup_data *req, uint8_t **buf,
				    uint16_t *len)
{
	int (*command) (usbd_device *usbd_dev, struct usb_setup_data *req,
			uint8_t **buf, uint16_t *len) = NULL;

	switch (req->bRequest) {
	case USB_REQ_CLEAR_FEATURE:
		if (req->wValue == USB_FEAT_ENDPOINT_HALT) {
			command = usb_standard_endpoint_unstall;
		}
		break;
	case USB_REQ_SET_FEATURE:
		if (req->wValue == USB_FEAT_ENDPOINT_HALT) {
			command = usb_standard_endpoint_stall;
		}
		break;
	case USB_REQ_GET_STATUS:
		command = usb_standard_endpoint_get_status;
		break;
	case USB_REQ_SET_SYNCH_FRAME:
		/* FIXME: SYNCH_FRAME is not implemented. */
		/*
		 * SYNCH_FRAME is used for synchronization of isochronous
		 * endpoints which are not yet implemented.
		 */
		break;
	}

	if (!command) {
		return 0;
	}

	return command(usbd_dev, req, buf, len);
}

int _usbd_standard_request(usbd_device *usbd_dev, struct usb_setup_data *req,
			   uint8_t **buf, uint16_t *len)
{
	/* FIXME: Have class/vendor requests as well. */
	if ((req->bmRequestType & USB_REQ_TYPE_TYPE) != USB_REQ_TYPE_STANDARD) {
		return 0;
	}

	switch (req->bmRequestType & USB_REQ_TYPE_RECIPIENT) {
	case USB_REQ_TYPE_DEVICE:
		return _usbd_standard_request_device(usbd_dev, req, buf, len);
	case USB_REQ_TYPE_INTERFACE:
		return _usbd_standard_request_interface(usbd_dev, req,
							buf, len);
	case USB_REQ_TYPE_ENDPOINT:
		return _usbd_standard_request_endpoint(usbd_dev, req, buf, len);
	default:
		return 0;
	}
}

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


static usbd_device *st_usbfs_v1_usbd_init(void);

const struct _usbd_driver st_usbfs_v1_usb_driver = {
	.init = st_usbfs_v1_usbd_init,
	.set_address = st_usbfs_set_address,
	.ep_setup = st_usbfs_ep_setup,
	.ep_reset = st_usbfs_endpoints_reset,
	.ep_stall_set = st_usbfs_ep_stall_set,
	.ep_stall_get = st_usbfs_ep_stall_get,
	.ep_nak_set = st_usbfs_ep_nak_set,
	.ep_write_packet = st_usbfs_ep_write_packet,
	.ep_read_packet = st_usbfs_ep_read_packet,
	.poll = st_usbfs_poll,
};

/** Initialize the USB device controller hardware of the STM32. */
static usbd_device *st_usbfs_v1_usbd_init(void)
{
	rcc_periph_clock_enable(RCC_USB);
	SET_REG(USB_CNTR_REG, 0);
	SET_REG(USB_BTABLE_REG, 0);
	SET_REG(USB_ISTR_REG, 0);

	/* Enable RESET, SUSPEND, RESUME and CTR interrupts. */
	SET_REG(USB_CNTR_REG, USB_CNTR_RESETM | USB_CNTR_CTRM |
		USB_CNTR_SUSPM | USB_CNTR_WKUPM);
	return &st_usbfs_dev;
}

void st_usbfs_copy_to_pm(volatile void *vPM, const void *buf, uint16_t len)
{
	const uint16_t *lbuf = buf;
	volatile uint32_t *PM = vPM;
	for (len = (len + 1) >> 1; len; len--) {
		*PM++ = *lbuf++;
	}
}

/**
 * Copy a data buffer from packet memory.
 *
 * @param buf Source pointer to data buffer.
 * @param vPM Destination pointer into packet memory.
 * @param len Number of bytes to copy.
 */
void st_usbfs_copy_from_pm(void *buf, const volatile void *vPM, uint16_t len)
{
	uint16_t *lbuf = buf;
	const volatile uint16_t *PM = vPM;
	uint8_t odd = len & 1;

	for (len >>= 1; len; PM += 2, lbuf++, len--) {
		*lbuf = *PM;
	}

	if (odd) {
		*(uint8_t *) lbuf = *(uint8_t *) PM;
	}
}
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 * Copyright (C) 2015 Robin Kreis <r.kreis@uni-bremen.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


/* TODO - can't these be inside the impls, not globals from the core? */
uint8_t st_usbfs_force_nak[8];
struct _usbd_device st_usbfs_dev;

void st_usbfs_set_address(usbd_device *dev, uint8_t addr)
{
	(void)dev;
	/* Set device address and enable. */
	SET_REG(USB_DADDR_REG, (addr & USB_DADDR_ADDR) | USB_DADDR_EF);
}

/**
 * Set the receive buffer size for a given USB endpoint.
 *
 * @param ep Index of endpoint to configure.
 * @param size Size in bytes of the RX buffer.
 */
void st_usbfs_set_ep_rx_bufsize(usbd_device *dev, uint8_t ep, uint32_t size)
{
	(void)dev;
	if (size > 62) {
		if (size & 0x1f) {
			size -= 32;
		}
		USB_SET_EP_RX_COUNT(ep, (size << 5) | 0x8000);
	} else {
		if (size & 1) {
			size++;
		}
		USB_SET_EP_RX_COUNT(ep, size << 10);
	}
}

void st_usbfs_ep_setup(usbd_device *dev, uint8_t addr, uint8_t type,
		uint16_t max_size,
		void (*callback) (usbd_device *usbd_dev,
		uint8_t ep))
{
	/* Translate USB standard type codes to STM32. */
	const uint16_t typelookup[] = {
		[USB_ENDPOINT_ATTR_CONTROL] = USB_EP_TYPE_CONTROL,
		[USB_ENDPOINT_ATTR_ISOCHRONOUS] = USB_EP_TYPE_ISO,
		[USB_ENDPOINT_ATTR_BULK] = USB_EP_TYPE_BULK,
		[USB_ENDPOINT_ATTR_INTERRUPT] = USB_EP_TYPE_INTERRUPT,
	};
	uint8_t dir = addr & 0x80;
	addr &= 0x7f;

	/* Assign address. */
	USB_SET_EP_ADDR(addr, addr);
	USB_SET_EP_TYPE(addr, typelookup[type]);

	if (dir || (addr == 0)) {
		USB_SET_EP_TX_ADDR(addr, dev->pm_top);
		if (callback) {
			dev->user_callback_ctr[addr][USB_TRANSACTION_IN] =
			    (void *)callback;
		}
		USB_CLR_EP_TX_DTOG(addr);
		USB_SET_EP_TX_STAT(addr, USB_EP_TX_STAT_NAK);
		dev->pm_top += max_size;
	}

	if (!dir) {
		USB_SET_EP_RX_ADDR(addr, dev->pm_top);
		st_usbfs_set_ep_rx_bufsize(dev, addr, max_size);
		if (callback) {
			dev->user_callback_ctr[addr][USB_TRANSACTION_OUT] =
			    (void *)callback;
		}
		USB_CLR_EP_RX_DTOG(addr);
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
		dev->pm_top += max_size;
	}
}

void st_usbfs_endpoints_reset(usbd_device *dev)
{
	int i;

	/* Reset all endpoints. */
	for (i = 1; i < 8; i++) {
		USB_SET_EP_TX_STAT(i, USB_EP_TX_STAT_DISABLED);
		USB_SET_EP_RX_STAT(i, USB_EP_RX_STAT_DISABLED);
	}
	dev->pm_top = USBD_PM_TOP + (2 * dev->desc->bMaxPacketSize0);
}

void st_usbfs_ep_stall_set(usbd_device *dev, uint8_t addr,
				   uint8_t stall)
{
	(void)dev;
	if (addr == 0) {
		USB_SET_EP_TX_STAT(addr, stall ? USB_EP_TX_STAT_STALL :
				   USB_EP_TX_STAT_NAK);
	}

	if (addr & 0x80) {
		addr &= 0x7F;

		USB_SET_EP_TX_STAT(addr, stall ? USB_EP_TX_STAT_STALL :
				   USB_EP_TX_STAT_NAK);

		/* Reset to DATA0 if clearing stall condition. */
		if (!stall) {
			USB_CLR_EP_TX_DTOG(addr);
		}
	} else {
		/* Reset to DATA0 if clearing stall condition. */
		if (!stall) {
			USB_CLR_EP_RX_DTOG(addr);
		}

		USB_SET_EP_RX_STAT(addr, stall ? USB_EP_RX_STAT_STALL :
				   USB_EP_RX_STAT_VALID);
	}
}

uint8_t st_usbfs_ep_stall_get(usbd_device *dev, uint8_t addr)
{
	(void)dev;
	if (addr & 0x80) {
		if ((*USB_EP_REG(addr & 0x7F) & USB_EP_TX_STAT) ==
		    USB_EP_TX_STAT_STALL) {
			return 1;
		}
	} else {
		if ((*USB_EP_REG(addr) & USB_EP_RX_STAT) ==
		    USB_EP_RX_STAT_STALL) {
			return 1;
		}
	}
	return 0;
}

void st_usbfs_ep_nak_set(usbd_device *dev, uint8_t addr, uint8_t nak)
{
	(void)dev;
	/* It does not make sense to force NAK on IN endpoints. */
	if (addr & 0x80) {
		return;
	}

	st_usbfs_force_nak[addr] = nak;

	if (nak) {
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_NAK);
	} else {
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
	}
}

uint16_t st_usbfs_ep_write_packet(usbd_device *dev, uint8_t addr,
				     const void *buf, uint16_t len)
{
	(void)dev;
	addr &= 0x7F;

	if ((*USB_EP_REG(addr) & USB_EP_TX_STAT) == USB_EP_TX_STAT_VALID) {
		return 0;
	}

	st_usbfs_copy_to_pm(USB_GET_EP_TX_BUFF(addr), buf, len);
	USB_SET_EP_TX_COUNT(addr, len);
	USB_SET_EP_TX_STAT(addr, USB_EP_TX_STAT_VALID);

	return len;
}

uint16_t st_usbfs_ep_read_packet(usbd_device *dev, uint8_t addr,
					 void *buf, uint16_t len)
{
	(void)dev;
	if ((*USB_EP_REG(addr) & USB_EP_RX_STAT) == USB_EP_RX_STAT_VALID) {
		return 0;
	}

	len = MIN(USB_GET_EP_RX_COUNT(addr) & 0x3ff, len);
	st_usbfs_copy_from_pm(buf, USB_GET_EP_RX_BUFF(addr), len);
	USB_CLR_EP_RX_CTR(addr);

	if (!st_usbfs_force_nak[addr]) {
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
	}

	return len;
}

void st_usbfs_poll(usbd_device *dev)
{
	uint16_t istr = *USB_ISTR_REG;

	if (istr & USB_ISTR_RESET) {
		USB_CLR_ISTR_RESET();
		dev->pm_top = USBD_PM_TOP;
		_usbd_reset(dev);
		return;
	}

	if (istr & USB_ISTR_CTR) {
		uint8_t ep = istr & USB_ISTR_EP_ID;
		bool out = (istr & USB_ISTR_DIR) ? true : false;
		uint8_t type;
		uint16_t epreg = *USB_EP_REG(ep);

		/* If DIR is set in USB_ISTR, we must check if CTR_TX is set in
		 * the EP reg and if control_state.state is LAST_DATA_IN we
		 * must deal with the IN transaction first. */
		if ((ep == 0x00) && out && (epreg & USB_EP_TX_CTR) &&
			dev->control_state.state == LAST_DATA_IN) {
			type = USB_TRANSACTION_IN;
		} else if (out) { /* OUT or SETUP transaction */
			type = (epreg & USB_EP_SETUP) ? USB_TRANSACTION_SETUP : USB_TRANSACTION_OUT;
		} else { /* IN transaction */
			type = USB_TRANSACTION_IN;
			USB_CLR_EP_TX_CTR(ep);
		}

		if (dev->user_callback_ctr[ep][type]) {
			dev->user_callback_ctr[ep][type] (dev, ep);
		} else {
			USB_CLR_EP_RX_CTR(ep);
		}
	}

	if (istr & USB_ISTR_SUSP) {
		USB_CLR_ISTR_SUSP();
		if (dev->user_callback_suspend) {
			dev->user_callback_suspend();
		}
	}

	if (istr & USB_ISTR_WKUP) {
		USB_CLR_ISTR_WKUP();
		if (dev->user_callback_resume) {
			dev->user_callback_resume();
		}
	}

	if (istr & USB_ISTR_SOF) {
		USB_CLR_ISTR_SOF();
		if (dev->user_callback_sof) {
			dev->user_callback_sof();
		}
	}

	if (dev->user_callback_sof) {
		*USB_CNTR_REG |= USB_CNTR_SOFM;
	} else {
		*USB_CNTR_REG &= ~USB_CNTR_SOFM;
	}
}
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Frantisek Burian <bufran@seznam.cz>
 * .. file is merged from many other copyrighted files of stm32 family
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
/**@{*/


/*---------------------------------------------------------------------------*/
/** @brief RCC Enable Peripheral Clocks.
 *
 * Enable the clock on particular peripherals. There are three registers
 * involved, each one controlling the enabling of clocks associated with the
 * AHB, APB1 and APB2 respectively. Several peripherals could be enabled
 * simultaneously <em>only if they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Clock Enable Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 *
 * @param[in] en Unsigned int32. Logical OR of all enables to be set
 * @li If register is RCC_AHBER, from @ref rcc_ahbenr_en
 * @li If register is RCC_APB1ENR, from @ref rcc_apb1enr_en
 * @li If register is RCC_APB2ENR, from @ref rcc_apb2enr_en
 */

void rcc_peripheral_enable_clock(volatile uint32_t *reg, uint32_t en)
{
	*reg |= en;
}

/*---------------------------------------------------------------------------*/
/** @brief RCC Disable Peripheral Clocks.
 *
 * Enable the clock on particular peripherals. There are three registers
 * involved, each one controlling the enabling of clocks associated with
 * the AHB, APB1 and APB2 respectively. Several peripherals could be disabled
 * simultaneously <em>only if they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Clock Enable Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 * @param[in] en Unsigned int32. Logical OR of all enables to be used for
 * disabling.
 * @li If register is RCC_AHBER, from @ref rcc_ahbenr_en
 * @li If register is RCC_APB1ENR, from @ref rcc_apb1enr_en
 * @li If register is RCC_APB2ENR, from @ref rcc_apb2enr_en
 */
void rcc_peripheral_disable_clock(volatile uint32_t *reg, uint32_t en)
{
	*reg &= ~en;
}

/*---------------------------------------------------------------------------*/
/** @brief RCC Reset Peripherals.
 *
 * Reset particular peripherals. There are three registers involved, each one
 * controlling reset of peripherals associated with the AHB, APB1 and APB2
 * respectively. Several peripherals could be reset simultaneously <em>only if
 * they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Reset Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 * @param[in] reset Unsigned int32. Logical OR of all resets.
 * @li If register is RCC_AHBRSTR, from @ref rcc_ahbrstr_rst
 * @li If register is RCC_APB1RSTR, from @ref rcc_apb1rstr_rst
 * @li If register is RCC_APB2RSTR, from @ref rcc_apb2rstr_rst
 */
void rcc_peripheral_reset(volatile uint32_t *reg, uint32_t reset)
{
	*reg |= reset;
}

/*---------------------------------------------------------------------------*/
/** @brief RCC Remove Reset on Peripherals.
 *
 * Remove the reset on particular peripherals. There are three registers
 * involved, each one controlling reset of peripherals associated with the AHB,
 * APB1 and APB2 respectively. Several peripherals could have the reset removed
 * simultaneously <em>only if they are controlled by the same register</em>.
 *
 * @param[in] *reg Unsigned int32. Pointer to a Reset Register
 *			 (either RCC_AHBENR, RCC_APB1ENR or RCC_APB2ENR)
 * @param[in] clear_reset Unsigned int32. Logical OR of all resets to be
 * removed:
 * @li If register is RCC_AHBRSTR, from @ref rcc_ahbrstr_rst
 * @li If register is RCC_APB1RSTR, from @ref rcc_apb1rstr_rst
 * @li If register is RCC_APB2RSTR, from @ref rcc_apb2rstr_rst
 */
void rcc_peripheral_clear_reset(volatile uint32_t *reg, uint32_t clear_reset)
{
	*reg &= ~clear_reset;
}

#define _RCC_REG(i)		MMIO32(RCC_BASE + ((i) >> 5))
#define _RCC_BIT(i)		(1 << ((i) & 0x1f))

/*---------------------------------------------------------------------------*/
/** @brief Enable Peripheral Clock in running mode.
 *
 * Enable the clock on particular peripheral.
 *
 * @param[in] clken rcc_periph_clken Peripheral RCC
 *
 * For available constants, see #rcc_periph_clken (RCC_UART1 for example)
 */

void rcc_periph_clock_enable(enum rcc_periph_clken clken)
{
	_RCC_REG(clken) |= _RCC_BIT(clken);
}

/*---------------------------------------------------------------------------*/
/** @brief Disable Peripheral Clock in running mode.
 * Disable the clock on particular peripheral.
 *
 * @param[in] clken rcc_periph_clken Peripheral RCC
 *
 * For available constants, see #rcc_periph_clken (RCC_UART1 for example)
 */

void rcc_periph_clock_disable(enum rcc_periph_clken clken)
{
	_RCC_REG(clken) &= ~_RCC_BIT(clken);
}

/*---------------------------------------------------------------------------*/
/** @brief Reset Peripheral, pulsed
 *
 * Reset particular peripheral, and restore to working state.
 *
 * @param[in] rst rcc_periph_rst Peripheral reset
 *
 * For available constants, see #rcc_periph_rst (RST_UART1 for example)
 */

void rcc_periph_reset_pulse(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) |= _RCC_BIT(rst);
	_RCC_REG(rst) &= ~_RCC_BIT(rst);
}

/*---------------------------------------------------------------------------*/
/** @brief Reset Peripheral, hold
 *
 * Reset particular peripheral, and hold in reset state.
 *
 * @param[in] rst rcc_periph_rst Peripheral reset
 *
 * For available constants, see #rcc_periph_rst (RST_UART1 for example)
 */

void rcc_periph_reset_hold(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) |= _RCC_BIT(rst);
}

/*---------------------------------------------------------------------------*/
/** @brief Reset Peripheral, release
 *
 * Restore peripheral from reset state to working state.
 *
 * @param[in] rst rcc_periph_rst Peripheral reset
 *
 * For available constants, see #rcc_periph_rst (RST_UART1 for example)
 */

void rcc_periph_reset_release(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) &= ~_RCC_BIT(rst);
}

/** @brief Select the source of Microcontroller Clock Output
 *
 * Exact sources available depend on your target.  On devices with multiple
 * MCO pins, this function controls MCO1
 *
 * @parame[in] mcosrc the unshifted source bits
 */

void rcc_set_mco(uint32_t mcosrc)
{
	RCC_CFGR = (RCC_CFGR & ~(RCC_CFGR_MCO_MASK << RCC_CFGR_MCO_SHIFT)) |
			(mcosrc << RCC_CFGR_MCO_SHIFT);
}

/**@}*/

#undef _RCC_REG
#undef _RCC_BIT
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Karl Palsson <karlp@ŧweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


uint16_t desig_get_flash_size(void)
{
	return DESIG_FLASH_SIZE;
}

void desig_get_unique_id(uint32_t *result)
{
	*result++ = DESIG_UNIQUE_ID2;
	*result++ = DESIG_UNIQUE_ID1;
	*result = DESIG_UNIQUE_ID0;
}

void desig_get_unique_id_as_string(char *string,
				   unsigned int string_len)
{
	int i, len;
	uint32_t dev_id_buf[3];
	uint8_t *device_id = (uint8_t*)dev_id_buf;
	const char chars[] = "0123456789ABCDEF";

	desig_get_unique_id(dev_id_buf);

	/* Each byte produces two characters */
	len = (2 * sizeof(dev_id_buf) < string_len) ?
		2 * sizeof(dev_id_buf) : string_len - 1;

	for (i = 0; i < len; i += 2) {
		string[i]     = chars[(device_id[i / 2] >> 4) & 0x0F];
		string[i + 1] = chars[(device_id[i / 2] >> 0) & 0x0F];
	}

	string[len] = '\0';
}

