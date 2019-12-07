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
//#include "webusb.h"

#define bool uint8_t 
#define true 1
#define false 0

/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef USB_CONF_H_INCLUDED
#define USB_CONF_H_INCLUDED


#define USB_VID                 0x1209
#define USB_PID                 0xdb42
#define USB_SERIAL_NUM_LENGTH   24
#define USB_CONTROL_BUF_SIZE    256  //  Previously 1024
#define MAX_USB_PACKET_SIZE     64   //  Previously 32

#define USB21_INTERFACE                       //  Enable USB 2.1 with WebUSB and BOS support.
#define SERIAL_USB_INTERFACE              //  Enable only serial USB interface.  No DFU and storage.

//  Index of each USB interface.  Must be consecutive and must sync with interfaces[].
#ifdef ALL_USB_INTERFACES
#define INTF_DFU                0
#define INTF_MSC                1
#define INTF_COMM               2
#define INTF_DATA               3
#endif  //  ALL_USB_INTERFACES

#ifdef STORAGE_AND_SERIAL_USB_INTERFACE
#define INTF_MSC                0
#define INTF_COMM               1  //  COMM must be immediately before DATA because of Associated Interface Descriptor.
#define INTF_DATA               2
#endif  //  STORAGE_AND_SERIAL_USB_INTERFACE

#ifdef SERIAL_USB_INTERFACE
#define INTF_COMM               0  //  COMM must be immediately before DATA because of Associated Interface Descriptor.
#define INTF_DATA               1
#endif  //  SERIAL_USB_INTERFACE

//  USB Endpoints.
#define MSC_OUT                 0x01
#define DATA_OUT                0x03

#define MSC_IN                  0x82
#define DATA_IN                 0x84
#define COMM_IN                 0x85

#ifdef NOTUSED
#define DATA_IN                 0x82
#define COMM_IN                 0x83
#define MSC_IN                  0x84

#define DATA_OUT                0x01
#define MSC_OUT                 0x02
#endif  //  NOTUSED

extern void usb_set_serial_number(const char* serial);
extern usbd_device* usb_setup(void);
extern void msc_setup(usbd_device* usbd_dev0);
extern uint16_t send_msc_packet(const void *buf, int len);
extern int aggregate_register_config_callback(
    usbd_device *usbd_dev,
	usbd_set_config_callback callback);
extern int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback);


////

#define debug_println(x...) 
#define debug_flush() 
#define debug_print(x...)
#define debug_print_unsigned(x...)
#define debug_printhex(x...)
#ifndef NULL
#define NULL 0
#endif

void dbg(char*);

#endif
/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef WEBUSB_H_INCLUDED
#define WEBUSB_H_INCLUDED


// Arbitrary
#define WEBUSB_VENDOR_CODE 0x22  //  Don't use 0x21, reserved for WinUSB.

extern const struct webusb_platform_descriptor webusb_platform_capability_descriptor;
extern const struct webusb_platform_descriptor webusb_platform_capability_descriptor_no_landing_page;
extern const struct microsoft_platform_descriptor microsoft_platform_capability_descriptor;

extern void webusb_setup(usbd_device* usbd_dev, const char* https_url);

#endif
/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef WEBUSB_DEFS_H_INCLUDED
#define WEBUSB_DEFS_H_INCLUDED


#define WEBUSB_REQ_GET_URL             0x02

#define WEBUSB_DT_DESCRIPTOR_SET_HEADER 0
#define WEBUSB_DT_CONFIGURATION_SUBSET_HEADER 1
#define WEBUSB_DT_FUNCTION_SUBSET_HEADER 2
#define WEBUSB_DT_URL 3

#define WEBUSB_URL_SCHEME_HTTP 0
#define WEBUSB_URL_SCHEME_HTTPS 1

struct webusb_platform_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint8_t bReserved;
	uint8_t platformCapabilityUUID[16];
	uint16_t bcdVersion;
	uint8_t bVendorCode;
	uint8_t iLandingPage;
} __attribute__((packed));

// from https://wicg.github.io/webusb/#webusb-platform-capability-descriptor
// see also this (for endianness explanation)
// https://github.com/WICG/webusb/issues/115#issuecomment-352206549
#define WEBUSB_UUID { 0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47,0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65 }
#define WEBUSB_PLATFORM_DESCRIPTOR_SIZE sizeof(struct webusb_platform_descriptor)

struct webusb_url_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bScheme;
	char URL[];
} __attribute__((packed));

#define WEBUSB_DT_URL_DESCRIPTOR_SIZE 3

#endif
#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

typedef void (*cdc_receive_callback)(uint8_t* buf, int len);

extern void cdc_setup(usbd_device* usbd_dev);
extern void cdc_set_receive_callback(cdc_receive_callback callback);
void cdc_transmit(uint8_t* buffer, int len);
#endif  //  CDC_H_INCLUDED
//  CDC code from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c
/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


const usbd_driver* target_usb_init(void) 
{
    return &st_usbfs_v1_usb_driver;
}

static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue
);

static const char* origin_url = "visualbluepill.github.io";

static char serial_number[USB_SERIAL_NUM_LENGTH+1];

static const char *usb_strings[] = {
    "Devanarchy",              //  USB Manufacturer
    "DAPBoot DFU Bootloader",  //  USB Product
    serial_number,             //  Serial number
    //"Blue Pill DFU",         //  DFU
    "DAPBoot DFU",             //  DFU
    "Blue Pill MSC",           //  MSC
    "Blue Pill Serial Port",   //  Serial Port
    "Blue Pill COMM",          //  COMM
    "Blue Pill DATA",          //  DATA
};

#define MSC_VENDOR_ID "BluePill"  //  Max 8 chars
#define MSC_PRODUCT_ID "UF2 Bootloader"  //  Max 16 chars
#define MSC_PRODUCT_REVISION_LEVEL "2.1"  //  Max 4 chars
#define USB_CLASS_MISCELLANEOUS 0xef  //  Copy from microbit.

enum usb_strings_index {  //  Index of USB strings.  Must sync with above, starts from 1.
    USB_STRINGS_MANUFACTURER = 1,
    USB_STRINGS_PRODUCT,
    USB_STRINGS_SERIAL_NUMBER,
    USB_STRINGS_DFU,
    USB_STRINGS_MSC,
    USB_STRINGS_SERIAL_PORT,
    USB_STRINGS_COMM,
    USB_STRINGS_DATA,
};

//  USB Device
static const struct usb_device_descriptor dev = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0210,  //  USB Version 2.1.  Need to handle special requests e.g. BOS.
	.bDeviceClass = USB_CLASS_CDC,  //  Set the class to CDC if it's only serial.  Serial interface will not start on Windows when class = 0.
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = MAX_USB_PACKET_SIZE,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = 0x0220,  //  Device Release number 2.2
    .iManufacturer = USB_STRINGS_MANUFACTURER,
    .iProduct = USB_STRINGS_PRODUCT,
    .iSerialNumber = USB_STRINGS_SERIAL_NUMBER,
    .bNumConfigurations = 1,
};

//  CDC Endpoints

/*
 * This notification endpoint isn't implemented. According to CDC spec it's
 * optional, but its absence causes a NULL pointer dereference in the
 * Linux cdc_acm driver. (Gareth McMullin <gareth@blacksphere.co.nz>)
 */
static const struct usb_endpoint_descriptor comm_endp[] = {
	{
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = COMM_IN,
		.bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
		.wMaxPacketSize = COMM_PACKET_SIZE,  //  Smaller than others
		.bInterval = 255,
	}
};

static const struct usb_endpoint_descriptor data_endp[] = {
	{
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = DATA_OUT,
		.bmAttributes = USB_ENDPOINT_ATTR_BULK,
		.wMaxPacketSize = MAX_USB_PACKET_SIZE,
		.bInterval = 1,
	}, {
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = DATA_IN,
		.bmAttributes = USB_ENDPOINT_ATTR_BULK,
		.wMaxPacketSize = MAX_USB_PACKET_SIZE,
		.bInterval = 1,
	}
};

static const struct {
	struct usb_cdc_header_descriptor header;
	struct usb_cdc_call_management_descriptor call_mgmt;
	struct usb_cdc_acm_descriptor acm;
	struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) cdcacm_functional_descriptors = {
	.header = {
		.bFunctionLength = sizeof(struct usb_cdc_header_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_HEADER,
		.bcdCDC = 0x0110,
	},
	.call_mgmt = {
		.bFunctionLength =
			sizeof(struct usb_cdc_call_management_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
		.bmCapabilities = 0,
		.bDataInterface = INTF_DATA,  //  DATA Interface
	},
	.acm = {
		.bFunctionLength = sizeof(struct usb_cdc_acm_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_ACM,
		.bmCapabilities = 0,
	},
	.cdc_union = {
		.bFunctionLength = sizeof(struct usb_cdc_union_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_UNION,
		.bControlInterface = INTF_COMM,       //  COMM Interface
		.bSubordinateInterface0 = INTF_DATA,  //  DATA Interface
	 }
};

//  CDC Interfaces
static const struct usb_iface_assoc_descriptor cdc_iface_assoc = {  //  Copied from microbit.  Mandatory for composite device.
	.bLength = USB_DT_INTERFACE_ASSOCIATION_SIZE,
	.bDescriptorType = USB_DT_INTERFACE_ASSOCIATION,
	.bFirstInterface = INTF_COMM,  //  First associated interface (INTF_COMM and INTF_DATA)
	.bInterfaceCount = 2,          //  Total number of associated interfaces (INTF_COMM and INTF_DATA), ID must be consecutive.
	.bFunctionClass = USB_CLASS_CDC,
	.bFunctionSubClass = USB_CDC_SUBCLASS_ACM,
	.bFunctionProtocol = USB_CDC_PROTOCOL_AT,
	.iFunction = USB_STRINGS_SERIAL_PORT  //  Name of Serial Port
};

static const struct usb_interface_descriptor comm_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = INTF_COMM,
    .bAlternateSetting = 0,
    .bNumEndpoints = 1,
    .bInterfaceClass = USB_CLASS_CDC,
    .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
    .bInterfaceProtocol = USB_CDC_PROTOCOL_AT,
    .iInterface = USB_STRINGS_COMM,  //  Name of COMM
    .endpoint = comm_endp,           //  COMM Endpoint
    .extra = &cdcacm_functional_descriptors,
    .extralen = sizeof(cdcacm_functional_descriptors)
};

static const struct usb_interface_descriptor data_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = INTF_DATA,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = USB_CLASS_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USB_STRINGS_DATA,  //  Name of DATA
    .endpoint = data_endp,           //  DATA Endpoints
};

//  All USB Interfaces
static const struct usb_interface interfaces[] = {
    {
        .num_altsetting = 1,
        .altsetting = &comm_iface,  //  Index must sync with INTF_COMM.
    }, 
    {
        .num_altsetting = 1,
        .altsetting = &data_iface,  //  Index must sync with INTF_DATA.
    },
};

//  USB Config
static const struct usb_config_descriptor config = {
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = sizeof(interfaces) / sizeof(struct usb_interface),
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80,  //  Bus-powered, i.e. it draws power from USB bus.
    .bMaxPower = 0xfa,     //  500 mA. Copied from microbit.
    .interface = interfaces,
};

//  BOS Capabilities for WebUSB and Microsoft Platform
static const struct usb_device_capability_descriptor* capabilities[] = {
	(const struct usb_device_capability_descriptor*) 
        &webusb_platform_capability_descriptor,
};

//  BOS Descriptor for WebUSB and Microsoft Platform
static const struct usb_bos_descriptor bos_descriptor = {
	.bLength = USB_DT_BOS_SIZE,
	.bDescriptorType = USB_DT_BOS,
	.bNumDeviceCaps = sizeof(capabilities) / sizeof(capabilities[0]),
	.capabilities = capabilities
};

/* Buffer to be used for control requests. */
static uint8_t usbd_control_buffer[USB_CONTROL_BUF_SIZE] __attribute__ ((aligned (2)));
usbd_device* usbd_dev = NULL;

usbd_device* usb_setup(void) {

    char serial[USB_SERIAL_NUM_LENGTH+1];
    serial[0] = '\0';
    desig_get_unique_id_as_string(serial, USB_SERIAL_NUM_LENGTH);
    usb_set_serial_number(serial);


    int num_strings = sizeof(usb_strings) / sizeof(const char*);
    // debug_print("usb_setup num_strings "); debug_print_int(num_strings); debug_println(""); // debug_flush(); ////
    const usbd_driver* driver = target_usb_init();

    usbd_dev = usbd_init(driver, &dev, &config, 

  &bos_descriptor, // gabo

        usb_strings, num_strings,
        usbd_control_buffer, sizeof(usbd_control_buffer));

    //  The following USB setup functions will call aggregate_register_callback() to register callbacks.
    cdc_setup(usbd_dev);
    //  Define USB 2.1 BOS interface used by WebUSB.
	webusb_setup(usbd_dev, origin_url);

    //  Set the aggregate callback.    
	int status = usbd_register_set_config_callback(usbd_dev, set_aggregate_callback);
    if (status < 0) { debug_println("*** usb_setup failed"); debug_flush(); }

    //  For WinUSB: Windows probes the compatible ID before setting the configuration, so also register the callback now.
    set_aggregate_callback(usbd_dev, (uint16_t) -1);
    return usbd_dev;
}


struct control_callback_struct {
    uint8_t type;
    uint8_t type_mask;
    usbd_control_callback cb;
};

#define MAX_CONTROL_CALLBACK 10
static struct control_callback_struct control_callback[MAX_CONTROL_CALLBACK];
static usbd_set_config_callback config_callback[MAX_CONTROL_CALLBACK];

int aggregate_register_config_callback(
    usbd_device *usbd_dev,
	usbd_set_config_callback callback) {
    //  Register the USB config callback.  We do this to overcome the 4 callback limit.
	int i;
	for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
		if (config_callback[i]) {
            if (config_callback[i] == callback) { return 0; }  //  Skip duplicate.
			continue;
		}
		config_callback[i] = callback;
		return 0;
	}
    debug_println("*** ERROR: Too many config callbacks"); debug_flush();
	return -1;
}

int aggregate_register_callback(
    usbd_device *usbd_dev, 
    uint8_t type,
    uint8_t type_mask,
    usbd_control_callback callback) {
    // Register application callback function for handling USB control requests.  We aggregate here so we can handle more than 4 callbacks.
    // debug_println("aggregate_register_callback"); ////
	int i;
	for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
		if (control_callback[i].cb) { 
            //  If already exists, skip.
            if (control_callback[i].type == type &&
                control_callback[i].type_mask == type_mask &&
                control_callback[i].cb == callback) { 
                    //  debug_println("callback exists"); ////
                    return 0;
                }
            continue;  //  Continue checking.
        }
		control_callback[i].type = type;
		control_callback[i].type_mask = type_mask;
		control_callback[i].cb = callback;
		return 0;
	}
    debug_println("*** ERROR: Too many control callbacks"); debug_flush();
	return -1;
}

static uint8_t usb_descriptor_type(uint16_t wValue) {
	return wValue >> 8;
}

static uint8_t usb_descriptor_index(uint16_t wValue) {
	return wValue & 0xFF;
}

uint16_t device_address = (uint16_t) -1;

static int aggregate_callback(
    usbd_device *usbd_dev,
	struct usb_setup_data *req, 
    uint8_t **buf, 
    uint16_t *len,
	usbd_control_complete_callback *complete) {
    //  This callback is called whenever a USB request is received.  We route to the right driver callbacks.
	int i, result = 0;
    //  Call the callbacks registered by the drivers.
    for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
        if (control_callback[i].cb == NULL) { break; }
        if ((req->bmRequestType & control_callback[i].type_mask) == control_callback[i].type) {
            result = control_callback[i].cb(
                usbd_dev, 
                req,
                buf,
                len,
                complete);
            if (result == USBD_REQ_HANDLED ||
                result == USBD_REQ_NOTSUPP) {
                return result;
            }
        }
    }
    if (!(req->bmRequestType == 0x80 && req->bRequest == 0x06)) {
        //  Dump the packet if not GET_DESCRIPTOR.
//	    dump_usb_request(">> ", req); debug_flush(); ////
    } 
	return USBD_REQ_NEXT_CALLBACK;
}

static void set_aggregate_callback(
  usbd_device *usbd_dev,
  uint16_t wValue) {
    //  This callback is called when the device is updated.  We set our control callback.
    if (wValue != (uint16_t) -1) {  //  If this is an actual callback, not a call by usb_setup()...
        //  Call the config functions before setting our callback.
        debug_println("set_aggregate_callback"); ////
        int i;
        for (i = 0; i < MAX_CONTROL_CALLBACK; i++) {
            if (!config_callback[i]) { break; }
            (config_callback[i])(usbd_dev, wValue);
        }
    }
    //  Set our callback.
	int status;
        status = usbd_register_control_callback(
		usbd_dev,
        0,  //  Register for all notifications.
        0,
		aggregate_callback);
	if (status < 0) { debug_println("*** ERROR: set_aggregate_callback failed"); debug_flush(); }  
}

void usb_set_serial_number(const char* serial) {
    serial_number[0] = '\0';
    if (serial) {
        strncpy(serial_number, serial, USB_SERIAL_NUM_LENGTH);
        serial_number[USB_SERIAL_NUM_LENGTH] = '\0';
    }
}
/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

//  Test WebUSB in Chrome console:
//  navigator.usb.requestDevice({ filters: [] }).then(console.log)
//  navigator.usb.getDevices().then(console.log)


#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_DEVICE)
#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)

#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })

//  WebUSB Descriptor with landing page.
const struct webusb_platform_descriptor webusb_platform_capability_descriptor = {
	.bLength = WEBUSB_PLATFORM_DESCRIPTOR_SIZE,
	.bDescriptorType = USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType = USB_DC_PLATFORM,
	.bReserved = 0,
	.platformCapabilityUUID = WEBUSB_UUID,
	.bcdVersion = 0x0100,
	.bVendorCode = WEBUSB_VENDOR_CODE,
	.iLandingPage = 1
};

//  WebUSB Descriptor without landing page.
const struct webusb_platform_descriptor webusb_platform_capability_descriptor_no_landing_page = {
	.bLength = WEBUSB_PLATFORM_DESCRIPTOR_SIZE,
	.bDescriptorType = USB_DT_DEVICE_CAPABILITY,
	.bDevCapabilityType = USB_DC_PLATFORM,
	.bReserved = 0,
	.platformCapabilityUUID = WEBUSB_UUID,
	.bcdVersion = 0x0100,
	.bVendorCode = WEBUSB_VENDOR_CODE,
	.iLandingPage = 0
};

static const char* webusb_https_url;

static int webusb_control_vendor_request(usbd_device *usbd_dev,
									 struct usb_setup_data *req,
									 uint8_t **buf, uint16_t *len,
									 usbd_control_complete_callback* complete) {
	//  Handle a request for the WebUSB URL:
	//  >> typ c0, req 22, val 0001, idx 0002, len 00ff
	(void)complete;
	(void)usbd_dev;
	//  For WebUSB, only request types C0 and C1 are allowed.
	if (req->bmRequestType != 0xc0 && req->bmRequestType != 0xc1) { return USBD_REQ_NEXT_CALLBACK; }
	if (req->bRequest != WEBUSB_VENDOR_CODE) { return USBD_REQ_NEXT_CALLBACK; }
	int status = USBD_REQ_NOTSUPP;
	switch (req->wIndex) {
		case WEBUSB_REQ_GET_URL: {
			struct webusb_url_descriptor* url = (struct webusb_url_descriptor*)(*buf);
			uint16_t index = req->wValue;
			if (index == 0) {
    			debug_print("*** webusb notsupp index "); debug_print_unsigned(index); debug_println(""); debug_flush(); ////
				return USBD_REQ_NOTSUPP;
			}
			if (index == 1) {
//				dump_usb_request("weburl", req); debug_flush(); ////
				size_t url_len = strlen(webusb_https_url);
				url->bLength = WEBUSB_DT_URL_DESCRIPTOR_SIZE + url_len;
				url->bDescriptorType = WEBUSB_DT_URL;
				url->bScheme = WEBUSB_URL_SCHEME_HTTPS;
				memcpy(&url->URL, webusb_https_url, url_len);
				*len = MIN(*len, url->bLength);
				status = USBD_REQ_HANDLED;
			} else {
				// TODO: stall instead?
    			debug_print("*** webusb notsupp index "); debug_print_unsigned(index); debug_println(""); debug_flush(); ////
				status = USBD_REQ_NOTSUPP;
			}
			break;
		}
		default: {
    		debug_print("*** webusb notsupp wIndex "); debug_print_unsigned(req->wIndex); debug_println(""); debug_flush(); ////
			status = USBD_REQ_NOTSUPP;
			break;
		}
	}

	return status;
}

static void webusb_set_config(usbd_device* usbd_dev, uint16_t wValue) {
    //  debug_println("webusb_set_config"); // debug_flush(); ////
	(void)wValue;
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		webusb_control_vendor_request);
	if (status < 0) { debug_println("*** webusb_set_config failed"); debug_flush(); }
}

void webusb_setup(usbd_device* usbd_dev, const char* https_url) {
    // debug_println("webusb_setup"); // debug_flush(); ////
	webusb_https_url = https_url;

	//  Register the callback now because WebUSB requests come earlier.
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		webusb_control_vendor_request);
	if (status < 0) { debug_println("*** webusb_setup failed"); debug_flush(); }

    //  Re-register the callback in case the USB restarts.
	status = aggregate_register_config_callback(usbd_dev, webusb_set_config);
	if (status < 0) { debug_println("*** webusb_setup failed"); debug_flush(); }
}

//  CDC code from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c

//#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
//#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)
#define USB_CDC_REQ_GET_LINE_CODING		0x21

//  Line config to be returned.
static const struct usb_cdc_line_coding line_coding = {
	.dwDTERate = 115200,
	.bCharFormat = USB_CDC_1_STOP_BITS,
	.bParityType = USB_CDC_NO_PARITY,
	.bDataBits = 0x08
};

cdc_receive_callback receive_callback = NULL;
usbd_device* transmit_device = NULL;

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
	//  dump_usb_request("*** cdc", req); ////
	switch (req->bRequest) {
		case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {
			/* From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
			* This Linux cdc_acm driver requires this to be implemented
			* even though it's optional in the CDC spec, and we don't
			* advertise it in the ACM functional descriptor. */
#ifdef NOTUSED
			char local_buf[10];
			struct usb_cdc_notification *notif = (void *)local_buf;

			/* We echo signals back to host as notification. */
			notif->bmRequestType = 0xA1;
			notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
			notif->wValue = 0;
			notif->wIndex = 0;
			notif->wLength = 2;
			local_buf[8] = req->wValue & 3;
			local_buf[9] = 0;
			// usbd_ep_write_packet(0x83, buf, 10);
#endif  //  NOTUSED
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_GET_LINE_CODING: {
			//  Windows requires this request, not Mac or Linux.
			//  From https://github.com/PX4/Bootloader/blob/master/stm32/cdcacm.c
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
				debug_print("*** cdcacm_control notsupp line_coding "); debug_print_unsigned(sizeof(struct usb_cdc_line_coding)); 
				debug_print(", len "); debug_print_unsigned(*len);
				debug_println(""); debug_flush(); ////
				return USBD_REQ_NOTSUPP;
			}
			*buf = (uint8_t *) &line_coding;
			*len = sizeof(struct usb_cdc_line_coding);
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_SET_LINE_CODING: {
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
				debug_print("*** cdcacm_control notsupp line_coding "); debug_print_unsigned(sizeof(struct usb_cdc_line_coding)); 
				debug_print(", len "); debug_print_unsigned(*len);
				debug_println(""); debug_flush(); ////
				return USBD_REQ_NOTSUPP;
			}
			return USBD_REQ_HANDLED;
		}
	}
	//  dump_usb_request("*** cdc next", req); ////
	return USBD_REQ_NEXT_CALLBACK;  //  Previously USBD_REQ_NOTSUPP
}

//  TODO: TX Up to MAX_USB_PACKET_SIZE
//  usbd_ep_write_packet(usbd_dev, DATA_IN, txbuf, txlen)

static char cdcbuf[MAX_USB_PACKET_SIZE + 1];   // rx buffer

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
    cdcbuf[pos] = 0;

//	usbd_ep_write_packet(usbd_dev, DATA_IN, cdcbuf, pos); ////  Echo the packet.
	
//    debug_print("["); debug_println(cdcbuf); debug_print("]"); // debug_flush(); ////
//dbg(cdcbuf);
  if (receive_callback)
    receive_callback((uint8_t*)cdcbuf, pos);
}

static void
cdcacm_comm_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
//	debug_println("comm"); debug_flush();
}

/*
 * USB Configuration:
 */
static void
cdcacm_set_config(
  usbd_device *usbd_dev,
  uint16_t wValue __attribute__((unused))
) {
	//  From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
    //  debug_println("*** cdcacm_set_config"); ////
	usbd_ep_setup(usbd_dev, DATA_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, DATA_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, NULL);
	usbd_ep_setup(usbd_dev, COMM_IN, USB_ENDPOINT_ATTR_INTERRUPT, COMM_PACKET_SIZE, cdcacm_comm_cb);
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		cdcacm_control_request);

	if (status < 0) { debug_println("*** cdcacm_set_config failed"); debug_flush(); }
}

void cdc_setup(usbd_device* usbd_dev) {
    //  debug_println("*** cdc_setup"); ////
	int status = aggregate_register_config_callback(usbd_dev, cdcacm_set_config);
	if (status < 0) { debug_println("*** cdc_setup failed"); debug_flush(); }
  transmit_device = usbd_dev;
}

void cdc_set_receive_callback(cdc_receive_callback callback)
{
  receive_callback = callback;
}

void cdc_transmit(uint8_t* buffer, int len)
{
  usbd_ep_write_packet(transmit_device, DATA_IN, buffer, len);
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

