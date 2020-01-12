#include <stdint.h>

struct usbd_device;
extern usbd_device* usb_setup(void);
extern void usbd_poll(usbd_device *usbd_dev);
typedef void (*cdc_receive_callback)(uint8_t* buf, int len);
extern void cdc_set_receive_callback(cdc_receive_callback callback);
extern void cdc_transmit(uint8_t* buffer, int len);
