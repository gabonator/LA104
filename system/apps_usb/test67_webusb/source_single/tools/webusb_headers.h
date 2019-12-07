#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define bool uint8_t 
#define true 1
#define false 0

struct usb_setup_data {
 uint8_t bmRequestType;
 uint8_t bRequest;
 uint16_t wValue;
 uint16_t wIndex;
 uint16_t wLength;
} __attribute__((packed));
struct usb_device_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint16_t bcdUSB;
 uint8_t bDeviceClass;
 uint8_t bDeviceSubClass;
 uint8_t bDeviceProtocol;
 uint8_t bMaxPacketSize0;
 uint16_t idVendor;
 uint16_t idProduct;
 uint16_t bcdDevice;
 uint8_t iManufacturer;
 uint8_t iProduct;
 uint8_t iSerialNumber;
 uint8_t bNumConfigurations;
} __attribute__((packed));






struct usb_device_qualifier_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint16_t bcdUSB;
 uint8_t bDeviceClass;
 uint8_t bDeviceSubClass;
 uint8_t bDeviceProtocol;
 uint8_t bMaxPacketSize0;
 uint8_t bNumConfigurations;
 uint8_t bReserved;
} __attribute__((packed));


struct usb_config_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint16_t wTotalLength;
 uint8_t bNumInterfaces;
 uint8_t bConfigurationValue;
 uint8_t iConfiguration;
 uint8_t bmAttributes;
 uint8_t bMaxPower;


 const struct usb_interface {
  uint8_t *cur_altsetting;
  uint8_t num_altsetting;
  const struct usb_iface_assoc_descriptor *iface_assoc;
  const struct usb_interface_descriptor *altsetting;
 } *interface;
} __attribute__((packed));
struct usb_interface_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint8_t bInterfaceNumber;
 uint8_t bAlternateSetting;
 uint8_t bNumEndpoints;
 uint8_t bInterfaceClass;
 uint8_t bInterfaceSubClass;
 uint8_t bInterfaceProtocol;
 uint8_t iInterface;


 const struct usb_endpoint_descriptor *endpoint;
 const void *extra;
 int extralen;
} __attribute__((packed));



struct usb_endpoint_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint8_t bEndpointAddress;
 uint8_t bmAttributes;
 uint16_t wMaxPacketSize;
 uint8_t bInterval;


 const void *extra;
 int extralen;
} __attribute__((packed));


struct usb_device_capability_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDevCapabilityType;
} __attribute__((packed));

struct usb_bos_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumDeviceCaps;

    const struct usb_device_capability_descriptor **capabilities;
} __attribute__((packed));
struct usb_string_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint16_t wData[];
} __attribute__((packed));


struct usb_iface_assoc_descriptor {
 uint8_t bLength;
 uint8_t bDescriptorType;
 uint8_t bFirstInterface;
 uint8_t bInterfaceCount;
 uint8_t bFunctionClass;
 uint8_t bFunctionSubClass;
 uint8_t bFunctionProtocol;
 uint8_t iFunction;
} __attribute__((packed));



enum usb_language_id {
 USB_LANGID_ENGLISH_US = 0x409,
};




enum usbd_request_return_codes {
 USBD_REQ_NOTSUPP = 0,
 USBD_REQ_HANDLED = 1,
 USBD_REQ_NEXT_CALLBACK = 2,
};

typedef struct _usbd_driver usbd_driver;
typedef struct _usbd_device usbd_device;

extern const usbd_driver st_usbfs_v1_usb_driver;
extern const usbd_driver stm32f107_usb_driver;
extern const usbd_driver stm32f207_usb_driver;
extern const usbd_driver st_usbfs_v2_usb_driver;


extern const usbd_driver efm32lg_usb_driver;
extern usbd_device * usbd_init(const usbd_driver *driver,
          const struct usb_device_descriptor *dev,
          const struct usb_config_descriptor *conf,
                   const struct usb_bos_descriptor *bos,
          const char **strings, int num_strings,
          uint8_t *control_buffer,
          uint16_t control_buffer_size);


extern void usbd_register_reset_callback(usbd_device *usbd_dev,
      void (*callback)(void));

extern void usbd_register_suspend_callback(usbd_device *usbd_dev,
        void (*callback)(void));

extern void usbd_register_resume_callback(usbd_device *usbd_dev,
       void (*callback)(void));

extern void usbd_register_sof_callback(usbd_device *usbd_dev,
           void (*callback)(void));

typedef void (*usbd_control_complete_callback)(usbd_device *usbd_dev,
  struct usb_setup_data *req);

typedef int (*usbd_control_callback)(usbd_device *usbd_dev,
  struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
  usbd_control_complete_callback *complete);

typedef void (*usbd_set_config_callback)(usbd_device *usbd_dev,
      uint16_t wValue);

typedef void (*usbd_set_altsetting_callback)(usbd_device *usbd_dev,
          uint16_t wIndex, uint16_t wValue);

typedef void (*usbd_endpoint_callback)(usbd_device *usbd_dev, uint8_t ep);
extern int usbd_register_control_callback(usbd_device *usbd_dev, uint8_t type,
       uint8_t type_mask,
       usbd_control_callback callback);





extern int usbd_register_set_config_callback(usbd_device *usbd_dev,
       usbd_set_config_callback callback);

extern void usbd_register_set_altsetting_callback(usbd_device *usbd_dev,
     usbd_set_altsetting_callback callback);


extern void usbd_register_extra_string(usbd_device *usbd_dev, int index, const char* string);


extern void usbd_poll(usbd_device *usbd_dev);


extern void usbd_disconnect(usbd_device *usbd_dev,
                                                  bool
                                                       disconnected);





extern void usbd_ep_setup(usbd_device *usbd_dev, uint8_t addr, uint8_t type,
  uint16_t max_size, usbd_endpoint_callback callback);






extern uint16_t usbd_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
    const void *buf, uint16_t len);






extern uint16_t usbd_ep_read_packet(usbd_device *usbd_dev, uint8_t addr,
          void *buf, uint16_t len);




extern void usbd_ep_stall_set(usbd_device *usbd_dev, uint8_t addr,
         uint8_t stall);





extern uint8_t usbd_ep_stall_get(usbd_device *usbd_dev, uint8_t addr);





extern void usbd_ep_nak_set(usbd_device *usbd_dev, uint8_t addr, uint8_t nak);


struct usb_cdc_header_descriptor {
 uint8_t bFunctionLength;
 uint8_t bDescriptorType;
 uint8_t bDescriptorSubtype;
 uint16_t bcdCDC;
} __attribute__((packed));


struct usb_cdc_union_descriptor {
 uint8_t bFunctionLength;
 uint8_t bDescriptorType;
 uint8_t bDescriptorSubtype;
 uint8_t bControlInterface;
 uint8_t bSubordinateInterface0;

} __attribute__((packed));
struct usb_cdc_call_management_descriptor {
 uint8_t bFunctionLength;
 uint8_t bDescriptorType;
 uint8_t bDescriptorSubtype;
 uint8_t bmCapabilities;
 uint8_t bDataInterface;
} __attribute__((packed));


struct usb_cdc_acm_descriptor {
 uint8_t bFunctionLength;
 uint8_t bDescriptorType;
 uint8_t bDescriptorSubtype;
 uint8_t bmCapabilities;
} __attribute__((packed));
struct usb_cdc_line_coding {
 uint32_t dwDTERate;
 uint8_t bCharFormat;
 uint8_t bParityType;
 uint8_t bDataBits;
} __attribute__((packed));

enum usb_cdc_line_coding_bCharFormat {
 USB_CDC_1_STOP_BITS = 0,
 USB_CDC_1_5_STOP_BITS = 1,
 USB_CDC_2_STOP_BITS = 2,
};

enum usb_cdc_line_coding_bParityType {
 USB_CDC_NO_PARITY = 0,
 USB_CDC_ODD_PARITY = 1,
 USB_CDC_EVEN_PARITY = 2,
 USB_CDC_MARK_PARITY = 3,
 USB_CDC_SPACE_PARITY = 4,
};







struct usb_cdc_notification {
 uint8_t bmRequestType;
 uint8_t bNotification;
 uint16_t wValue;
 uint16_t wIndex;
 uint16_t wLength;
} __attribute__((packed));












uint16_t desig_get_flash_size(void);






void desig_get_unique_id(uint32_t *result);







void desig_get_unique_id_as_string(char *string,
       unsigned int string_len);


extern uint32_t rcc_ahb_frequency;
extern uint32_t rcc_apb1_frequency;
extern uint32_t rcc_apb2_frequency;



enum rcc_osc {
 RCC_PLL, RCC_PLL2, RCC_PLL3, RCC_HSE, RCC_HSI, RCC_LSE, RCC_LSI
};







enum rcc_periph_clken {


 RCC_DMA1 = (((0x14) << 5) + (0)),
 RCC_DMA2 = (((0x14) << 5) + (1)),
 RCC_SRAM = (((0x14) << 5) + (2)),
 RCC_FLTF = (((0x14) << 5) + (4)),
 RCC_CRC = (((0x14) << 5) + (6)),
 RCC_FSMC = (((0x14) << 5) + (8)),
 RCC_SDIO = (((0x14) << 5) + (10)),
 RCC_OTGFS = (((0x14) << 5) + (12)),
 RCC_ETHMAC = (((0x14) << 5) + (14)),
 RCC_ETHMACTX = (((0x14) << 5) + (15)),
 RCC_ETHMACRX = (((0x14) << 5) + (16)),


 RCC_AFIO = (((0x18) << 5) + (0)),
 RCC_GPIOA = (((0x18) << 5) + (2)),
 RCC_GPIOB = (((0x18) << 5) + (3)),
 RCC_GPIOC = (((0x18) << 5) + (4)),
 RCC_GPIOD = (((0x18) << 5) + (5)),
 RCC_GPIOE = (((0x18) << 5) + (6)),
 RCC_GPIOF = (((0x18) << 5) + (7)),
 RCC_GPIOG = (((0x18) << 5) + (8)),
 RCC_ADC1 = (((0x18) << 5) + (9)),
 RCC_ADC2 = (((0x18) << 5) + (10)),
 RCC_TIM1 = (((0x18) << 5) + (11)),
 RCC_SPI1 = (((0x18) << 5) + (12)),
 RCC_TIM8 = (((0x18) << 5) + (13)),
 RCC_USART1 = (((0x18) << 5) + (14)),
 RCC_ADC3 = (((0x18) << 5) + (15)),
 RCC_TIM15 = (((0x18) << 5) + (16)),
 RCC_TIM16 = (((0x18) << 5) + (17)),
 RCC_TIM17 = (((0x18) << 5) + (18)),
 RCC_TIM9 = (((0x18) << 5) + (19)),
 RCC_TIM10 = (((0x18) << 5) + (20)),
 RCC_TIM11 = (((0x18) << 5) + (21)),


 RCC_TIM2 = (((0x1C) << 5) + (0)),
 RCC_TIM3 = (((0x1C) << 5) + (1)),
 RCC_TIM4 = (((0x1C) << 5) + (2)),
 RCC_TIM5 = (((0x1C) << 5) + (3)),
 RCC_TIM6 = (((0x1C) << 5) + (4)),
 RCC_TIM7 = (((0x1C) << 5) + (5)),
 RCC_TIM12 = (((0x1C) << 5) + (6)),
 RCC_TIM13 = (((0x1C) << 5) + (7)),
 RCC_TIM14 = (((0x1C) << 5) + (8)),
 RCC_WWDG = (((0x1C) << 5) + (11)),
 RCC_SPI2 = (((0x1C) << 5) + (14)),
 RCC_SPI3 = (((0x1C) << 5) + (15)),
 RCC_USART2 = (((0x1C) << 5) + (17)),
 RCC_USART3 = (((0x1C) << 5) + (18)),
 RCC_UART4 = (((0x1C) << 5) + (19)),
 RCC_UART5 = (((0x1C) << 5) + (20)),
 RCC_I2C1 = (((0x1C) << 5) + (21)),
 RCC_I2C2 = (((0x1C) << 5) + (22)),
 RCC_USB = (((0x1C) << 5) + (23)),
 RCC_CAN = (((0x1C) << 5) + (25)),
 RCC_CAN1 = (((0x1C) << 5) + (25)),
 RCC_CAN2 = (((0x1C) << 5) + (26)),
 RCC_BKP = (((0x1C) << 5) + (27)),
 RCC_PWR = (((0x1C) << 5) + (28)),
 RCC_DAC = (((0x1C) << 5) + (29)),
 RCC_CEC = (((0x1C) << 5) + (30)),
};

enum rcc_periph_rst {


 RST_OTGFS = (((0x28) << 5) + (12)),
 RST_ETHMAC = (((0x28) << 5) + (14)),


 RST_AFIO = (((0x0c) << 5) + (0)),
 RST_GPIOA = (((0x0c) << 5) + (2)),
 RST_GPIOB = (((0x0c) << 5) + (3)),
 RST_GPIOC = (((0x0c) << 5) + (4)),
 RST_GPIOD = (((0x0c) << 5) + (5)),
 RST_GPIOE = (((0x0c) << 5) + (6)),
 RST_GPIOF = (((0x0c) << 5) + (7)),
 RST_GPIOG = (((0x0c) << 5) + (8)),
 RST_ADC1 = (((0x0c) << 5) + (9)),
 RST_ADC2 = (((0x0c) << 5) + (10)),
 RST_TIM1 = (((0x0c) << 5) + (11)),
 RST_SPI1 = (((0x0c) << 5) + (12)),
 RST_TIM8 = (((0x0c) << 5) + (13)),
 RST_USART1 = (((0x0c) << 5) + (14)),
 RST_ADC3 = (((0x0c) << 5) + (15)),
 RST_TIM15 = (((0x0c) << 5) + (16)),
 RST_TIM16 = (((0x0c) << 5) + (17)),
 RST_TIM17 = (((0x0c) << 5) + (18)),
 RST_TIM9 = (((0x0c) << 5) + (19)),
 RST_TIM10 = (((0x0c) << 5) + (20)),
 RST_TIM11 = (((0x0c) << 5) + (21)),


 RST_TIM2 = (((0x10) << 5) + (0)),
 RST_TIM3 = (((0x10) << 5) + (1)),
 RST_TIM4 = (((0x10) << 5) + (2)),
 RST_TIM5 = (((0x10) << 5) + (3)),
 RST_TIM6 = (((0x10) << 5) + (4)),
 RST_TIM7 = (((0x10) << 5) + (5)),
 RST_TIM12 = (((0x10) << 5) + (6)),
 RST_TIM13 = (((0x10) << 5) + (7)),
 RST_TIM14 = (((0x10) << 5) + (8)),
 RST_WWDG = (((0x10) << 5) + (11)),
 RST_SPI2 = (((0x10) << 5) + (14)),
 RST_SPI3 = (((0x10) << 5) + (15)),
 RST_USART2 = (((0x10) << 5) + (17)),
 RST_USART3 = (((0x10) << 5) + (18)),
 RST_UART4 = (((0x10) << 5) + (19)),
 RST_UART5 = (((0x10) << 5) + (20)),
 RST_I2C1 = (((0x10) << 5) + (21)),
 RST_I2C2 = (((0x10) << 5) + (22)),
 RST_USB = (((0x10) << 5) + (23)),
 RST_CAN = (((0x10) << 5) + (24)),
 RST_CAN1 = (((0x10) << 5) + (24)),
 RST_CAN2 = (((0x10) << 5) + (25)),
 RST_BKP = (((0x10) << 5) + (27)),
 RST_PWR = (((0x10) << 5) + (28)),
 RST_DAC = (((0x10) << 5) + (29)),
 RST_CEC = (((0x10) << 5) + (30)),
};



void rcc_peripheral_enable_clock(volatile uint32_t *reg, uint32_t en);
void rcc_peripheral_disable_clock(volatile uint32_t *reg, uint32_t en);
void rcc_peripheral_reset(volatile uint32_t *reg, uint32_t reset);
void rcc_peripheral_clear_reset(volatile uint32_t *reg, uint32_t clear_reset);

void rcc_periph_clock_enable(enum rcc_periph_clken clken);
void rcc_periph_clock_disable(enum rcc_periph_clken clken);
void rcc_periph_reset_pulse(enum rcc_periph_rst rst);
void rcc_periph_reset_hold(enum rcc_periph_rst rst);
void rcc_periph_reset_release(enum rcc_periph_rst rst);

void rcc_set_mco(uint32_t mcosrc);





void rcc_osc_ready_int_clear(enum rcc_osc osc);
void rcc_osc_ready_int_enable(enum rcc_osc osc);
void rcc_osc_ready_int_disable(enum rcc_osc osc);
int rcc_osc_ready_int_flag(enum rcc_osc osc);
void rcc_css_int_clear(void);
int rcc_css_int_flag(void);
void rcc_wait_for_osc_ready(enum rcc_osc osc);
void rcc_osc_on(enum rcc_osc osc);
void rcc_osc_off(enum rcc_osc osc);
void rcc_css_enable(void);
void rcc_css_disable(void);
void rcc_osc_bypass_enable(enum rcc_osc osc);
void rcc_osc_bypass_disable(enum rcc_osc osc);
void rcc_set_sysclk_source(uint32_t clk);
void rcc_set_pll_multiplication_factor(uint32_t mul);
void rcc_set_pll2_multiplication_factor(uint32_t mul);
void rcc_set_pll3_multiplication_factor(uint32_t mul);
void rcc_set_pll_source(uint32_t pllsrc);
void rcc_set_pllxtpre(uint32_t pllxtpre);
uint32_t rcc_rtc_clock_enabled_flag(void);
void rcc_enable_rtc_clock(void);
void rcc_set_rtc_clock_source(enum rcc_osc clock_source);
void rcc_set_adcpre(uint32_t adcpre);
void rcc_set_ppre2(uint32_t ppre2);
void rcc_set_ppre1(uint32_t ppre1);
void rcc_set_hpre(uint32_t hpre);
void rcc_set_usbpre(uint32_t usbpre);
void rcc_set_prediv1(uint32_t prediv);
void rcc_set_prediv2(uint32_t prediv);
void rcc_set_prediv1_source(uint32_t rccsrc);
uint32_t rcc_system_clock_source(void);
void rcc_clock_setup_in_hsi_out_64mhz(void);
void rcc_clock_setup_in_hsi_out_48mhz(void);
void rcc_clock_setup_in_hsi_out_24mhz(void);
void rcc_clock_setup_in_hse_8mhz_out_24mhz(void);
void rcc_clock_setup_in_hse_8mhz_out_72mhz(void);
void rcc_clock_setup_in_hse_12mhz_out_72mhz(void);
void rcc_clock_setup_in_hse_16mhz_out_72mhz(void);
void rcc_clock_setup_in_hse_25mhz_out_72mhz(void);
void rcc_backupdomain_reset(void);



struct _usbd_device {
 const struct usb_device_descriptor *desc;
 const struct usb_config_descriptor *config;
    const struct usb_bos_descriptor *bos;
 const char **strings;
 int num_strings;

 uint8_t *ctrl_buf;
 uint16_t ctrl_buf_len;

 uint8_t current_address;
 uint8_t current_config;

 uint16_t pm_top;


 void (*user_callback_reset)(void);
 void (*user_callback_suspend)(void);
 void (*user_callback_resume)(void);
 void (*user_callback_sof)(void);

 struct usb_control_state {
  enum {
   IDLE, STALLED,
   DATA_IN, LAST_DATA_IN, STATUS_IN,
   DATA_OUT, LAST_DATA_OUT, STATUS_OUT,
  } state;
  struct usb_setup_data req __attribute__((aligned(4)));
  uint8_t *ctrl_buf;
  uint16_t ctrl_len;
  usbd_control_complete_callback complete;
  
 bool
      needs_zlp;
 } control_state;

 struct user_control_callback {
  usbd_control_callback cb;
  uint8_t type;
  uint8_t type_mask;
 } user_control_callback[4];

 usbd_endpoint_callback user_callback_ctr[8][3];


 usbd_set_config_callback user_callback_set_config[4];

 usbd_set_altsetting_callback user_callback_set_altsetting;

 const struct _usbd_driver *driver;


 int extra_string_idx;
 const char* extra_string;



 uint16_t fifo_mem_top;
 uint16_t fifo_mem_top_ep0;
 uint8_t force_nak[4];




 uint32_t doeptsiz[4];





 uint16_t rxbcnt;
};

enum _usbd_transaction {
 USB_TRANSACTION_IN,
 USB_TRANSACTION_OUT,
 USB_TRANSACTION_SETUP,
};




void _usbd_control_in(usbd_device *usbd_dev, uint8_t ea);
void _usbd_control_out(usbd_device *usbd_dev, uint8_t ea);
void _usbd_control_setup(usbd_device *usbd_dev, uint8_t ea);

int _usbd_standard_request_device(usbd_device *usbd_dev,
      struct usb_setup_data *req, uint8_t **buf,
      uint16_t *len);
int _usbd_standard_request_interface(usbd_device *usbd_dev,
         struct usb_setup_data *req, uint8_t **buf,
         uint16_t *len);
int _usbd_standard_request_endpoint(usbd_device *usbd_dev,
        struct usb_setup_data *req, uint8_t **buf,
        uint16_t *len);
int _usbd_standard_request(usbd_device *usbd_dev, struct usb_setup_data *req,
      uint8_t **buf, uint16_t *len);

void _usbd_reset(usbd_device *usbd_dev);


struct _usbd_driver {
 usbd_device *(*init)(void);
 void (*set_address)(usbd_device *usbd_dev, uint8_t addr);
 void (*ep_setup)(usbd_device *usbd_dev, uint8_t addr, uint8_t type,
    uint16_t max_size, usbd_endpoint_callback cb);
 void (*ep_reset)(usbd_device *usbd_dev);
 void (*ep_stall_set)(usbd_device *usbd_dev, uint8_t addr,
        uint8_t stall);
 void (*ep_nak_set)(usbd_device *usbd_dev, uint8_t addr, uint8_t nak);
 uint8_t (*ep_stall_get)(usbd_device *usbd_dev, uint8_t addr);
 uint16_t (*ep_write_packet)(usbd_device *usbd_dev, uint8_t addr,
        const void *buf, uint16_t len);
 uint16_t (*ep_read_packet)(usbd_device *usbd_dev, uint8_t addr,
       void *buf, uint16_t len);
 void (*poll)(usbd_device *usbd_dev);
 void (*disconnect)(usbd_device *usbd_dev,
                                          bool
                                               disconnected);
 uint32_t base_address;
 
bool
     set_address_before_status;
 uint16_t rx_fifo_size;
};
void st_usbfs_set_address(usbd_device *dev, uint8_t addr);
void st_usbfs_set_ep_rx_bufsize(usbd_device *dev, uint8_t ep, uint32_t size);

void st_usbfs_ep_setup(usbd_device *usbd_dev, uint8_t addr,
  uint8_t type, uint16_t max_size,
  void (*callback) (usbd_device *usbd_dev,
  uint8_t ep));

void st_usbfs_endpoints_reset(usbd_device *usbd_dev);
void st_usbfs_ep_stall_set(usbd_device *usbd_dev, uint8_t addr, uint8_t stall);
uint8_t st_usbfs_ep_stall_get(usbd_device *usbd_dev, uint8_t addr);
void st_usbfs_ep_nak_set(usbd_device *usbd_dev, uint8_t addr, uint8_t nak);
uint16_t st_usbfs_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
      const void *buf, uint16_t len);
uint16_t st_usbfs_ep_read_packet(usbd_device *usbd_dev, uint8_t addr,
     void *buf, uint16_t len);
void st_usbfs_poll(usbd_device *usbd_dev);
void st_usbfs_copy_from_pm(void *buf, const volatile void *vPM, uint16_t len);
void st_usbfs_copy_to_pm(volatile void *vPM, const void *buf, uint16_t len);

extern uint8_t st_usbfs_force_nak[8];
extern struct _usbd_device st_usbfs_dev;










/*
 * This file contains structure definitions for the USB control structures
 * defined in chapter 9 of the "Universal Serial Bus Specification Revision 2.0"
 * Available from the USB Implementers Forum - http://www.usb.org/
 */

/* USB Setup Data structure - Table 9-2 */
/* Class Definition */
#define USB_CLASS_VENDOR			0xFF

/* bmRequestType bit definitions */
/* bit 7 : direction */
#define USB_REQ_TYPE_DIRECTION			0x80
#define USB_REQ_TYPE_IN				0x80
/* bits 6..5 : type */
#define USB_REQ_TYPE_TYPE			0x60
#define USB_REQ_TYPE_STANDARD			0x00
#define USB_REQ_TYPE_CLASS			0x20
#define USB_REQ_TYPE_VENDOR			0x40
/* bits 4..0 : recipient */
#define USB_REQ_TYPE_RECIPIENT			0x1F
#define USB_REQ_TYPE_DEVICE			0x00
#define USB_REQ_TYPE_INTERFACE			0x01
#define USB_REQ_TYPE_ENDPOINT			0x02
#define USB_REQ_TYPE_OTHER			0x03

/* USB Standard Request Codes - Table 9-4 */
#define USB_REQ_GET_STATUS			0
#define USB_REQ_CLEAR_FEATURE			1
/* Reserved for future use: 2 */
#define USB_REQ_SET_FEATURE			3
/* Reserved for future use: 3 */
#define USB_REQ_SET_ADDRESS			5
#define USB_REQ_GET_DESCRIPTOR			6
#define USB_REQ_SET_DESCRIPTOR			7
#define USB_REQ_GET_CONFIGURATION		8
#define USB_REQ_SET_CONFIGURATION		9
#define USB_REQ_GET_INTERFACE			10
#define USB_REQ_SET_INTERFACE			11
#define USB_REQ_SET_SYNCH_FRAME			12

/* USB Descriptor Types - Table 9-5 */
#define USB_DT_DEVICE				1
#define USB_DT_CONFIGURATION			2
#define USB_DT_STRING				3
#define USB_DT_INTERFACE			4
#define USB_DT_ENDPOINT				5
#define USB_DT_DEVICE_QUALIFIER			6
#define USB_DT_OTHER_SPEED_CONFIGURATION	7
#define USB_DT_INTERFACE_POWER			8
/* From ECNs */
#define USB_DT_OTG				9
#define USB_DT_DEBUG				10
#define USB_DT_INTERFACE_ASSOCIATION		11
/* USB 3.1 Descriptor Types - Table 9-6 */
#define USB_DT_BOS							15
#define USB_DT_DEVICE_CAPABILITY			16
#define USB_DT_SUPERSPEED_USB_ENDPOINT_COMPANION 48
#define USB_DT_SUPERSPEEDPLUS_ISOCHRONOUS_ENDPOINT_COMPANION 49

/* USB Standard Feature Selectors - Table 9-6 */
#define USB_FEAT_ENDPOINT_HALT			0
#define USB_FEAT_DEVICE_REMOTE_WAKEUP		1
#define USB_FEAT_TEST_MODE			2

/* Information Returned by a GetStatus() Request to a Device - Figure 9-4 */
#define USB_DEV_STATUS_SELF_POWERED		0x01
#define USB_DEV_STATUS_REMOTE_WAKEUP		0x02

#define USB_DT_DEVICE_SIZE sizeof(struct usb_device_descriptor)

/* USB Device_Qualifier Descriptor - Table 9-9
 * Not used in this implementation.
 */
#define USB_DT_CONFIGURATION_SIZE		9

/* USB Configuration Descriptor bmAttributes bit definitions */
#define USB_CONFIG_ATTR_DEFAULT			0x80	/** always required (USB2.0 table 9-10) */
#define USB_CONFIG_ATTR_SELF_POWERED		0x40
#define USB_CONFIG_ATTR_REMOTE_WAKEUP		0x20

/* Other Speed Configuration is the same as Configuration Descriptor.
 *  - Table 9-11
 */

/* USB Standard Interface Descriptor - Table 9-12 */
#define USB_DT_INTERFACE_SIZE			9

/* USB Standard Endpoint Descriptor - Table 9-13 */
#define USB_DT_ENDPOINT_SIZE		7

#define USB_DT_BOS_SIZE 5

/* USB Device Capability Types - USB 3.1 Table 9-14 */
#define USB_DC_WIRELESS_USB 1
#define USB_DC_USB_2_0_EXTENSION 2
#define USB_DC_SUPERSPEED_USB 3
#define USB_DC_CONTAINER_ID 4
#define USB_DC_PLATFORM 5
#define USB_DC_POWER_DELIVERY_CAPABILITY 6
#define USB_DC_BATTERY_INFO_CAPABILITY 7
#define USB_DC_PD_CONSUMER_PORT_CAPABILITY 8
#define USB_DC_PD_PROVIDER_PORT_CAPABILITY 9
#define USB_DC_SUPERSPEED_PLUS 10
#define USB_DC_PRECISION_TIME_MEASUREMENT 11
#define USB_DC_WIRELESS_USB_EXT 12

/* USB bEndpointAddress helper macros */
#define USB_ENDPOINT_ADDR_OUT(x) (x)
#define USB_ENDPOINT_ADDR_IN(x) (0x80 | (x))

/* USB Endpoint Descriptor bmAttributes bit definitions - Table 9-13 */
/* bits 1..0 : transfer type */
#define USB_ENDPOINT_ATTR_CONTROL		0x00
#define USB_ENDPOINT_ATTR_ISOCHRONOUS		0x01
#define USB_ENDPOINT_ATTR_BULK			0x02
#define USB_ENDPOINT_ATTR_INTERRUPT		0x03
#define USB_ENDPOINT_ATTR_TYPE		0x03
/* bits 3..2 : Sync type (only if ISOCHRONOUS) */
#define USB_ENDPOINT_ATTR_NOSYNC		0x00
#define USB_ENDPOINT_ATTR_ASYNC			0x04
#define USB_ENDPOINT_ATTR_ADAPTIVE		0x08
#define USB_ENDPOINT_ATTR_SYNC			0x0C
#define USB_ENDPOINT_ATTR_SYNCTYPE		0x0C
/* bits 5..4 : usage type (only if ISOCHRONOUS) */
#define USB_ENDPOINT_ATTR_DATA			0x00
#define USB_ENDPOINT_ATTR_FEEDBACK		0x10
#define USB_ENDPOINT_ATTR_IMPLICIT_FEEDBACK_DATA 0x20
#define USB_ENDPOINT_ATTR_USAGETYPE		0x30

/* Table 9-15 specifies String Descriptor Zero.
 * Table 9-16 specified UNICODE String Descriptor.
 */
#define USB_DT_INTERFACE_ASSOCIATION_SIZE \
				sizeof(struct usb_iface_assoc_descriptor)


/*****************************************************************************/
/* Module definitions                                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Register definitions                                                      */
/*****************************************************************************/

/* --- USB general registers ----------------------------------------------- */

/* USB Control register */
#define USB_CNTR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x40))
/* USB Interrupt status register */
#define USB_ISTR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x44))
/* USB Frame number register */
#define USB_FNR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x48))
/* USB Device address register */
#define USB_DADDR_REG		(&MMIO32(USB_DEV_FS_BASE + 0x4C))
/* USB Buffer table address register */
#define USB_BTABLE_REG		(&MMIO32(USB_DEV_FS_BASE + 0x50))

/* USB EP register */
#define USB_EP_REG(EP)		(&MMIO32(USB_DEV_FS_BASE) + (EP))


/*****************************************************************************/
/* Register values                                                           */
/*****************************************************************************/

/* --- USB control register masks / bits ----------------------------------- */

/* Interrupt mask bits, set to 1 to enable interrupt generation */
#define USB_CNTR_CTRM		0x8000
#define USB_CNTR_PMAOVRM	0x4000
#define USB_CNTR_ERRM		0x2000
#define USB_CNTR_WKUPM		0x1000
#define USB_CNTR_SUSPM		0x0800
#define USB_CNTR_RESETM		0x0400
#define USB_CNTR_SOFM		0x0200
#define USB_CNTR_ESOFM		0x0100

/* Request/Force bits */
#define USB_CNTR_RESUME		0x0010 /* Resume request */
#define USB_CNTR_FSUSP		0x0008 /* Force suspend */
#define USB_CNTR_LP_MODE	0x0004 /* Low-power mode */
#define USB_CNTR_PWDN		0x0002 /* Power down */
#define USB_CNTR_FRES		0x0001 /* Force reset */

/* --- USB interrupt status register masks / bits -------------------------- */

#define USB_ISTR_CTR		0x8000 /* Correct Transfer */
#define USB_ISTR_PMAOVR		0x4000 /* Packet Memory Area Over / Underrun */
#define USB_ISTR_ERR		0x2000 /* Error */
#define USB_ISTR_WKUP		0x1000 /* Wake up */
#define USB_ISTR_SUSP		0x0800 /* Suspend mode request */
#define USB_ISTR_RESET		0x0400 /* USB RESET request */
#define USB_ISTR_SOF		0x0200 /* Start Of Frame */
#define USB_ISTR_ESOF		0x0100 /* Expected Start Of Frame */
#define USB_ISTR_DIR		0x0010 /* Direction of transaction */
#define USB_ISTR_EP_ID		0x000F /* Endpoint Identifier */

/* --- USB interrupt status register manipulators -------------------------- */

/* Note: CTR is read only! */
#define USB_CLR_ISTR_PMAOVR()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_PMAOVR)
#define USB_CLR_ISTR_ERR()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_ERR)
#define USB_CLR_ISTR_WKUP()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_WKUP)
#define USB_CLR_ISTR_SUSP()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_SUSP)
#define USB_CLR_ISTR_RESET()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_RESET)
#define USB_CLR_ISTR_SOF()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_SOF)
#define USB_CLR_ISTR_ESOF()	CLR_REG_BIT(USB_ISTR_REG, USB_ISTR_ESOF)

/* --- USB Frame Number Register bits -------------------------------------- */

#define USB_FNR_RXDP		(1 << 15)
#define USB_FNR_RXDM		(1 << 14)
#define USB_FNR_LCK		(1 << 13)

#define USB_FNR_LSOF_SHIFT	11
#define USB_FNR_LSOF		(3 << USB_FNR_LSOF_SHIFT)

#define USB_FNR_FN		(0x7FF << 0)

/* --- USB device address register masks / bits ---------------------------- */

#define USB_DADDR_EF		(1 << 7)
#define USB_DADDR_ADDR		0x007F

/* USB_BTABLE Values ------------------------------------------------------- */

#define USB_BTABLE_BTABLE	0xFFF8

/* --- USB device address register manipulators ---------------------------- */

/* --- USB endpoint register offsets --------------------------------------- */

#define USB_EP0			0
#define USB_EP1			1
#define USB_EP2			2
#define USB_EP3			3
#define USB_EP4			4
#define USB_EP5			5
#define USB_EP6			6
#define USB_EP7			7

/* --- USB endpoint register masks / bits ---------------------------------- */

/* Masks and toggle bits */
#define USB_EP_RX_CTR		0x8000 /* Correct transfer RX */
#define USB_EP_RX_DTOG		0x4000 /* Data toggle RX */
#define USB_EP_RX_STAT		0x3000 /* Endpoint status for RX */

#define USB_EP_SETUP		0x0800 /* Setup transaction completed */
#define USB_EP_TYPE		0x0600 /* Endpoint type */
#define USB_EP_KIND		0x0100 /* Endpoint kind.
				* When set and type=bulk    -> double buffer
				* When set and type=control -> status out
				*/

#define USB_EP_TX_CTR		0x0080 /* Correct transfer TX */
#define USB_EP_TX_DTOG		0x0040 /* Data toggle TX */
#define USB_EP_TX_STAT		0x0030 /* Endpoint status for TX */

#define USB_EP_ADDR		0x000F /* Endpoint Address */

/* Masking all toggle bits */
#define USB_EP_NTOGGLE_MSK	(USB_EP_RX_CTR | \
				 USB_EP_SETUP | \
				 USB_EP_TYPE | \
				 USB_EP_KIND | \
				 USB_EP_TX_CTR | \
				 USB_EP_ADDR)

/* All non toggle bits plus EP_RX toggle bits */
#define USB_EP_RX_STAT_TOG_MSK	(USB_EP_RX_STAT | USB_EP_NTOGGLE_MSK)
/* All non toggle bits plus EP_TX toggle bits */
#define USB_EP_TX_STAT_TOG_MSK	(USB_EP_TX_STAT | USB_EP_NTOGGLE_MSK)

/* Endpoint status bits for USB_EP_RX_STAT bit field */
#define USB_EP_RX_STAT_DISABLED	0x0000
#define USB_EP_RX_STAT_STALL	0x1000
#define USB_EP_RX_STAT_NAK	0x2000
#define USB_EP_RX_STAT_VALID	0x3000

/* Endpoint status bits for USB_EP_TX_STAT bit field */
#define USB_EP_TX_STAT_DISABLED	0x0000
#define USB_EP_TX_STAT_STALL	0x0010
#define USB_EP_TX_STAT_NAK	0x0020
#define USB_EP_TX_STAT_VALID	0x0030

/* Endpoint type bits for USB_EP_TYPE bit field */
#define USB_EP_TYPE_BULK	0x0000
#define USB_EP_TYPE_CONTROL	0x0200
#define USB_EP_TYPE_ISO		0x0400
#define USB_EP_TYPE_INTERRUPT	0x0600

/* --- USB endpoint register manipulators ---------------------------------- */

/*
 * Set USB endpoint tx/rx status.
 *
 * USB status field is changed using an awkward toggle mechanism, that
 * is why we use some helper macros for that.
 */
#define USB_SET_EP_RX_STAT(EP, STAT) \
	TOG_SET_REG_BIT_MSK_AND_SET(USB_EP_REG(EP), \
		USB_EP_RX_STAT_TOG_MSK, STAT, USB_EP_RX_CTR | USB_EP_TX_CTR)

#define USB_SET_EP_TX_STAT(EP, STAT) \
	TOG_SET_REG_BIT_MSK_AND_SET(USB_EP_REG(EP), \
		USB_EP_TX_STAT_TOG_MSK, STAT, USB_EP_RX_CTR | USB_EP_TX_CTR)

/*
 * Macros for clearing and setting USB endpoint register bits that do
 * not use the toggle mechanism.
 *
 * Because the register contains some bits that use the toggle
 * mechanism we need a helper macro here. Otherwise the code gets really messy.
 */
#define USB_CLR_EP_NTOGGLE_BIT_AND_SET(EP, BIT, EXTRA_BITS) \
	CLR_REG_BIT_MSK_AND_SET(USB_EP_REG(EP), \
		USB_EP_NTOGGLE_MSK, BIT, EXTRA_BITS)

#define USB_CLR_EP_RX_CTR(EP) \
	USB_CLR_EP_NTOGGLE_BIT_AND_SET(EP, USB_EP_RX_CTR, USB_EP_TX_CTR)

#define USB_CLR_EP_TX_CTR(EP) \
	USB_CLR_EP_NTOGGLE_BIT_AND_SET(EP, USB_EP_TX_CTR, USB_EP_RX_CTR)


#define USB_SET_EP_TYPE(EP, TYPE) \
	SET_REG(USB_EP_REG(EP), \
		(GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & \
		(~USB_EP_TYPE))) | TYPE)

#define USB_SET_EP_KIND(EP) \
	SET_REG(USB_EP_REG(EP), \
		(GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & \
		(~USB_EP_KIND))) | USB_EP_KIND)

#define USB_CLR_EP_KIND(EP) \
	SET_REG(USB_EP_REG(EP), \
		(GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & (~USB_EP_KIND))))

#define USB_SET_EP_STAT_OUT(EP)	USB_SET_EP_KIND(EP)
#define USB_CLR_EP_STAT_OUT(EP)	USB_CLR_EP_KIND(EP)

#define USB_SET_EP_ADDR(EP, ADDR) \
	SET_REG(USB_EP_REG(EP), \
		((GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK & \
		(~USB_EP_ADDR))) | ADDR))

/* Macros for clearing DTOG bits */
#define USB_CLR_EP_TX_DTOG(EP) \
	SET_REG(USB_EP_REG(EP), \
		GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK | USB_EP_TX_DTOG))

#define USB_CLR_EP_RX_DTOG(EP) \
	SET_REG(USB_EP_REG(EP), \
		GET_REG(USB_EP_REG(EP)) & \
		(USB_EP_NTOGGLE_MSK | USB_EP_RX_DTOG))


/* --- USB BTABLE registers ------------------------------------------------ */

#define USB_GET_BTABLE		GET_REG(USB_BTABLE_REG)

/* --- USB BTABLE manipulators --------------------------------------------- */

#define USB_GET_EP_TX_ADDR(EP)		GET_REG(USB_EP_TX_ADDR(EP))
#define USB_GET_EP_TX_COUNT(EP)		GET_REG(USB_EP_TX_COUNT(EP))
#define USB_GET_EP_RX_ADDR(EP)		GET_REG(USB_EP_RX_ADDR(EP))
#define USB_GET_EP_RX_COUNT(EP)		GET_REG(USB_EP_RX_COUNT(EP))
#define USB_SET_EP_TX_ADDR(EP, ADDR)	SET_REG(USB_EP_TX_ADDR(EP), ADDR)
#define USB_SET_EP_TX_COUNT(EP, COUNT)	SET_REG(USB_EP_TX_COUNT(EP), COUNT)
#define USB_SET_EP_RX_ADDR(EP, ADDR)	SET_REG(USB_EP_RX_ADDR(EP), ADDR)
#define USB_SET_EP_RX_COUNT(EP, COUNT)	SET_REG(USB_EP_RX_COUNT(EP), COUNT)



/**@}*/


/* --- STM32 specific peripheral definitions ------------------------------- */

/* Memory map for all buses */
#define FLASH_BASE			(0x08000000U)
#define PERIPH_BASE			(0x40000000U)
#define INFO_BASE			(0x1ffff000U)
#define PERIPH_BASE_APB1		(PERIPH_BASE + 0x00000)
#define PERIPH_BASE_APB2		(PERIPH_BASE + 0x10000)
#define PERIPH_BASE_AHB			(PERIPH_BASE + 0x18000)

/* Register boundary addresses */

/* APB1 */
#define TIM2_BASE			(PERIPH_BASE_APB1 + 0x0000)
#define TIM3_BASE			(PERIPH_BASE_APB1 + 0x0400)
#define TIM4_BASE			(PERIPH_BASE_APB1 + 0x0800)
#define TIM5_BASE			(PERIPH_BASE_APB1 + 0x0c00)
#define TIM6_BASE			(PERIPH_BASE_APB1 + 0x1000)
#define TIM7_BASE			(PERIPH_BASE_APB1 + 0x1400)
#define TIM12_BASE			(PERIPH_BASE_APB1 + 0x1800)
#define TIM13_BASE			(PERIPH_BASE_APB1 + 0x1c00)
#define TIM14_BASE			(PERIPH_BASE_APB1 + 0x2000)
/* PERIPH_BASE_APB1 + 0x2400 (0x4000 2400 - 0x4000 27FF): Reserved */
#define RTC_BASE			(PERIPH_BASE_APB1 + 0x2800)
#define WWDG_BASE			(PERIPH_BASE_APB1 + 0x2c00)
#define IWDG_BASE			(PERIPH_BASE_APB1 + 0x3000)
/* PERIPH_BASE_APB1 + 0x3400 (0x4000 3400 - 0x4000 37FF): Reserved */
#define SPI2_BASE			(PERIPH_BASE_APB1 + 0x3800)
#define SPI3_BASE			(PERIPH_BASE_APB1 + 0x3c00)
/* PERIPH_BASE_APB1 + 0x4000 (0x4000 4000 - 0x4000 3FFF): Reserved */
#define USART2_BASE			(PERIPH_BASE_APB1 + 0x4400)
#define USART3_BASE			(PERIPH_BASE_APB1 + 0x4800)
#define UART4_BASE			(PERIPH_BASE_APB1 + 0x4c00)
#define UART5_BASE			(PERIPH_BASE_APB1 + 0x5000)
#define I2C1_BASE			(PERIPH_BASE_APB1 + 0x5400)
#define I2C2_BASE			(PERIPH_BASE_APB1 + 0x5800)
#define USB_DEV_FS_BASE			(PERIPH_BASE_APB1 + 0x5c00)
#define USB_PMA_BASE			(PERIPH_BASE_APB1 + 0x6000)
#define USB_CAN_SRAM_BASE		(PERIPH_BASE_APB1 + 0x6000)
#define BX_CAN1_BASE			(PERIPH_BASE_APB1 + 0x6400)
#define BX_CAN2_BASE			(PERIPH_BASE_APB1 + 0x6800)
/* PERIPH_BASE_APB1 + 0x6800 (0x4000 6800 - 0x4000 6BFF): Reserved? Typo? */
#define BACKUP_REGS_BASE		(PERIPH_BASE_APB1 + 0x6c00)
#define POWER_CONTROL_BASE		(PERIPH_BASE_APB1 + 0x7000)
#define DAC_BASE			(PERIPH_BASE_APB1 + 0x7400)
#define CEC_BASE			(PERIPH_BASE_APB1 + 0x7800)
/* PERIPH_BASE_APB1 + 0x7c00 (0x4000 7c00 - 0x4000 FFFF): Reserved */

/* APB2 */
#define AFIO_BASE			(PERIPH_BASE_APB2 + 0x0000)
#define EXTI_BASE			(PERIPH_BASE_APB2 + 0x0400)
#define GPIO_PORT_A_BASE		(PERIPH_BASE_APB2 + 0x0800)
#define GPIO_PORT_B_BASE		(PERIPH_BASE_APB2 + 0x0c00)
#define GPIO_PORT_C_BASE		(PERIPH_BASE_APB2 + 0x1000)
#define GPIO_PORT_D_BASE		(PERIPH_BASE_APB2 + 0x1400)
#define GPIO_PORT_E_BASE		(PERIPH_BASE_APB2 + 0x1800)
#define GPIO_PORT_F_BASE		(PERIPH_BASE_APB2 + 0x1c00)
#define GPIO_PORT_G_BASE		(PERIPH_BASE_APB2 + 0x2000)
#define ADC1_BASE			(PERIPH_BASE_APB2 + 0x2400)
#define ADC2_BASE			(PERIPH_BASE_APB2 + 0x2800)
#define TIM1_BASE			(PERIPH_BASE_APB2 + 0x2c00)
#define SPI1_BASE			(PERIPH_BASE_APB2 + 0x3000)
#define TIM8_BASE			(PERIPH_BASE_APB2 + 0x3400)
#define USART1_BASE			(PERIPH_BASE_APB2 + 0x3800)
#define ADC3_BASE			(PERIPH_BASE_APB2 + 0x3c00)
#define TIM15_BASE			(PERIPH_BASE_APB2 + 0x4000)
#define TIM16_BASE			(PERIPH_BASE_APB2 + 0x4400)
#define TIM17_BASE			(PERIPH_BASE_APB2 + 0x4800)
#define TIM9_BASE			(PERIPH_BASE_APB2 + 0x4c00)
#define TIM10_BASE			(PERIPH_BASE_APB2 + 0x5000)
#define TIM11_BASE			(PERIPH_BASE_APB2 + 0x5400)
/* PERIPH_BASE_APB2 + 0x5800 (0x4001 5800 - 0x4001 7FFF): Reserved */

/* AHB */
#define SDIO_BASE			(PERIPH_BASE_AHB + 0x00000)
/* PERIPH_BASE_AHB + 0x0400 (0x4001 8400 - 0x4001 7FFF): Reserved */
#define DMA1_BASE			(PERIPH_BASE_AHB + 0x08000)
#define DMA2_BASE			(PERIPH_BASE_AHB + 0x08400)
/* PERIPH_BASE_AHB + 0x8800 (0x4002 0800 - 0x4002 0FFF): Reserved */
#define RCC_BASE			(PERIPH_BASE_AHB + 0x09000)
/* PERIPH_BASE_AHB + 0x9400 (0x4002 1400 - 0x4002 1FFF): Reserved */
#define FLASH_MEM_INTERFACE_BASE	(PERIPH_BASE_AHB + 0x0a000)
#define CRC_BASE			(PERIPH_BASE_AHB + 0x0b000)
/* PERIPH_BASE_AHB + 0xb400 (0x4002 3400 - 0x4002 7FFF): Reserved */
#define ETHERNET_BASE			(PERIPH_BASE_AHB + 0x10000)
/* PERIPH_BASE_AHB + 0x18000 (0x4003 0000 - 0x4FFF FFFF): Reserved */
#define USB_OTG_FS_BASE			(PERIPH_BASE_AHB + 0xffe8000)

/* PPIB */
#define DBGMCU_BASE			(PPBI_BASE + 0x00042000)

/* FSMC */
#define FSMC_BASE			(PERIPH_BASE +  0x60000000)

/* Device Electronic Signature */
#define DESIG_FLASH_SIZE_BASE		(INFO_BASE + 0x7e0)
#define DESIG_UNIQUE_ID_BASE		(INFO_BASE + 0x7e8)
/* Ignore the "reserved for future use" half of the first word */
#define DESIG_UNIQUE_ID0		MMIO32(DESIG_UNIQUE_ID_BASE)
#define DESIG_UNIQUE_ID1		MMIO32(DESIG_UNIQUE_ID_BASE + 4)
#define DESIG_UNIQUE_ID2		MMIO32(DESIG_UNIQUE_ID_BASE + 8)


#define MMIO8(addr)		(*(volatile uint8_t *)(addr))
#define MMIO16(addr)		(*(volatile uint16_t *)(addr))
#define MMIO32(addr)		(*(volatile uint32_t *)(addr))
#define MMIO64(addr)		(*(volatile uint64_t *)(addr))










/* Table 2: Communications Device Class Code */
#define USB_CLASS_CDC			0x02

/* Table 4: Class Subclass Code */
#define USB_CDC_SUBCLASS_DLCM		0x01
#define USB_CDC_SUBCLASS_ACM		0x02
/* ... */

/* Table 5 Communications Interface Class Control Protocol Codes */
#define USB_CDC_PROTOCOL_NONE		0x00
#define USB_CDC_PROTOCOL_AT		0x01
/* ... */

/* Table 6: Data Interface Class Code */
#define USB_CLASS_DATA			0x0A

/* Table 12: Type Values for the bDescriptorType Field */
#define CS_INTERFACE			0x24
#define CS_ENDPOINT			0x25

/* Table 13: bDescriptor SubType in Communications Class Functional
 * Descriptors */
#define USB_CDC_TYPE_HEADER		0x00
#define USB_CDC_TYPE_CALL_MANAGEMENT	0x01
#define USB_CDC_TYPE_ACM		0x02
/* ... */
#define USB_CDC_TYPE_UNION		0x06
/* ... */
#define USB_CDC_REQ_SET_LINE_CODING		0x20
/* ... */
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE	0x22
#define USB_CDC_NOTIFY_SERIAL_STATE		0x20


#define MAX_USER_CONTROL_CALLBACK	4
#define MAX_USER_SET_CONFIG_CALLBACK	4


/* Get register content. */
#define GET_REG(REG)		((uint16_t) *(REG))

/* Set register content. */
#define SET_REG(REG, VAL)	(*(REG) = (uint16_t)(VAL))

#define TOG_SET_REG_BIT_MSK_AND_SET(REG, MSK, BIT, EXTRA_BITS)		\
do {									\
	register uint16_t toggle_mask = GET_REG(REG) & (MSK);		\
	register uint16_t bit_selector;					\
	for (bit_selector = 1; bit_selector; bit_selector <<= 1) {	\
		if ((bit_selector & (BIT)) != 0) {			\
			toggle_mask ^= bit_selector;			\
		}							\
	}								\
	SET_REG((REG), toggle_mask | (EXTRA_BITS));			\
} while (0)

#define TOG_SET_REG_BIT_MSK(REG, MSK, BIT) \
	TOG_SET_REG_BIT_MSK_AND_SET((REG), (MSK), (BIT), 0)
#define CLR_REG_BIT(REG, BIT)	SET_REG((REG), (~(BIT)))




#define USB_EP_TX_ADDR(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 0) * 2))

#define USB_EP_TX_COUNT(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 2) * 2))

#define USB_EP_RX_ADDR(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 4) * 2))

#define USB_EP_RX_COUNT(EP) \
	((uint32_t *)(USB_PMA_BASE + (USB_GET_BTABLE + EP * 8 + 6) * 2))

/* --- USB BTABLE manipulators --------------------------------------------- */

#define USB_GET_EP_TX_BUFF(EP) \
	(USB_PMA_BASE + (uint8_t *)(USB_GET_EP_TX_ADDR(EP) * 2))

#define USB_GET_EP_RX_BUFF(EP) \
	(USB_PMA_BASE + (uint8_t *)(USB_GET_EP_RX_ADDR(EP) * 2))


#define CLR_REG_BIT_MSK_AND_SET(REG, MSK, BIT, EXTRA_BITS) \
	SET_REG((REG), (GET_REG((REG)) & (MSK) & (~(BIT))) | (EXTRA_BITS))

#define USBD_PM_TOP 0x40


#define RCC_CFGR_MCO_SHIFT			24
#define RCC_CFGR				MMIO32(RCC_BASE + 0x04)
#define RCC_CFGR_MCO_MASK			0xf
#define RCC_CFGR_MCO_NOCLK			0x0
#define RCC_CFGR_MCO_SYSCLK			0x4
#define RCC_CFGR_MCO_HSICLK			0x5
#define RCC_CFGR_MCO_HSECLK			0x6
#define RCC_CFGR_MCO_PLLCLK_DIV2		0x7
#define RCC_CFGR_MCO_PLL2CLK			0x8 /* (**) */
#define RCC_CFGR_MCO_PLL3CLK_DIV2		0x9 /* (**) */
#define RCC_CFGR_MCO_XT1			0xa /* (**) */
#define RCC_CFGR_MCO_PLL3			0xb /* (**) */

#define DESIG_FLASH_SIZE		MMIO16(DESIG_FLASH_SIZE_BASE + 0x00)

#undef bool
#undef true
#undef false
