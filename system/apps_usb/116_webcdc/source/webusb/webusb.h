#ifdef __cplusplus
extern "C" {
#endif

#define USB_CDC_REQ_SET_LINE_CODING		0x20
#define USB_CDC_REQ_GET_LINE_CODING		0x21
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE	0x22

struct usb_cdc_line_coding {
	uint32_t dwDTERate;
	uint8_t bCharFormat;
	uint8_t bParityType;
	uint8_t bDataBits;
} __attribute__((packed));

enum usb_cdc_line_coding_bCharFormat {
	USB_CDC_1_STOP_BITS			= 0,
	USB_CDC_1_5_STOP_BITS			= 1,
	USB_CDC_2_STOP_BITS			= 2,
};

enum usb_cdc_line_coding_bParityType {
	USB_CDC_NO_PARITY			= 0,
	USB_CDC_ODD_PARITY			= 1,
	USB_CDC_EVEN_PARITY			= 2,
	USB_CDC_MARK_PARITY			= 3,
	USB_CDC_SPACE_PARITY			= 4,
};

#define CDC_MAX_USB_PACKET_SIZE 64

typedef void (*cdc_receive_callback)(uint8_t* buf, int len);
struct usbd_device;
extern usbd_device* usb_setup(void);
extern void usbd_poll(usbd_device *usbd_dev);
extern void cdc_set_receive_callback(cdc_receive_callback callback);
extern bool cdc_transmit(uint8_t* buffer, int len);
extern bool cdc_transmitReady();
extern void cdc_request(int req, int value);

#ifdef __cplusplus
}
#endif
