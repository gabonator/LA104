#ifdef __cplusplus
 extern "C" {
#endif

#define CDC_MAX_USB_PACKET_SIZE 64

typedef void (*cdc_receive_callback)(uint8_t* buf, int len);
struct usbd_device;
extern usbd_device* usb_setup(void);
extern void usbd_poll(usbd_device *usbd_dev);
extern void cdc_set_receive_callback(cdc_receive_callback callback);
extern bool cdc_transmit(uint8_t* buffer, int len);
extern bool cdc_transmitReady();

#ifdef __cplusplus
 }
#endif
