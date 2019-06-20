/********************* (C) COPYRIGHT 2017 e-Design Co.,Ltd. ********************/
#define PIN_I(IDR, n)   *(u32*)((IDR & 0xF0000000)+(1<<25)+((IDR & 0xFFFFFF)<<5)+(n<<2))
#define PIN_O(ODR, n)   *(u32*)((ODR & 0xF0000000)+(1<<25)+((ODR & 0xFFFFFF)<<5)+(n<<2))

//---------------------------- BackLight 相关宏定义 ----------------------------

#define BKL_IO_RCC     RCC_APB2Periph_GPIOA
#define BKL_PORT       GPIOA
#define BKL_PIN        GPIO_Pin_1

#define BKL_TIM_RCC    RCC_APB1Periph_TIM5
#define BKL_TIM        TIM5
#define BKL_PWM(Val)   BKL_TIM->CCR2 = (u8)Val;

#define BKL_PWM_CFG() {RCC_APB1PeriphClockCmd(BKL_TIM_RCC, ENABLE);            \
                       BKL_TIM->CCER  = 0x0030;                                \
                       BKL_TIM->CCMR1 = 0x7800;                                \
                       BKL_TIM->PSC   = 2000-1;                                \
                       BKL_TIM->ARR   = 100-1;                                 \
                       BKL_PWM(0);                                             \
                       BKL_TIM->CR1   = 0x0081;} // Frqn = 360Hz, Duty = 0~100%

//--------------------------- LCD 显示模块相关宏定义 ---------------------------

#define LCD_SLCT_RCC   RCC_APB2Periph_GPIOA
#define LCD_SLCT_PORT  GPIOA
#define LCD_nRST_PORT  LCD_SLCT_PORT
#define LCD_nCS_PORT   LCD_SLCT_PORT
#define nRST_PIN       GPIO_Pin_3
#define nCS_PIN        GPIO_Pin_2

#define LCD_CTRL_RCC   RCC_APB2Periph_GPIOD
#define LCD_CTRL_PORT  GPIOD
#define LCD_D_C_PORT   LCD_CTRL_PORT
#define LCD_nWR_PORT   LCD_CTRL_PORT
#define LCD_nRD_PORT   LCD_CTRL_PORT
#define D_C_PIN        GPIO_Pin_8
#define nWR_PIN        GPIO_Pin_9
#define nRD_PIN        GPIO_Pin_10

#define LCD_DB_RCC     RCC_APB2Periph_GPIOE
#define LCD_DB_PORT    GPIOE
#define LCD_DB_PINS    GPIO_Pin_All

#define LCD_nRST_ODR  (GPIOA_BASE+0x0C)
#define LCD_nCS_ODR   (GPIOA_BASE+0x0C)
#define LCD_D_C_ODR   (GPIOD_BASE+0x0C)
#define LCD_nWR_ODR   (GPIOD_BASE+0x0C)
#define LCD_nRD_ODR   (GPIOD_BASE+0x0C)
#define LCD_DATA_ODR  (GPIOE_BASE+0x0C)
#define LCD_DATA_IDR  (GPIOE_BASE+0x08)

#define nRST_PIN_NUM   3
#define nCS_PIN_NUM    2
#define D_C_PIN_NUM    8
#define nWR_PIN_NUM    9
#define nRD_PIN_NUM    10

#define LCD_nRST       PIN_O(LCD_nRST_ODR, nRST_PIN_NUM)
#define LCD_nCS        PIN_O(LCD_nCS_ODR,   nCS_PIN_NUM)
#define LCD_D_C        PIN_O(LCD_D_C_ODR,   D_C_PIN_NUM)
#define LCD_nWR        PIN_O(LCD_nWR_ODR,   nWR_PIN_NUM)
#define LCD_nRD        PIN_O(LCD_nRD_ODR,   nRD_PIN_NUM)

#define LCD_IO_RCC     LCD_SLCT_RCC | LCD_CTRL_RCC | LCD_DB_RCC | BKL_IO_RCC
#define LCD_DISP_DIR   0xC0

#define LCD_PORT_IN()  LCD_DB_PORT->CRL = LCD_DB_PORT->CRH = 0x44444444
#define LCD_PORT_OUT() LCD_DB_PORT->CRL = LCD_DB_PORT->CRH = 0x33333333

#define MCU_WRITE()   {GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;              \
                       GPIO_Struct.GPIO_Mode  = GPIO_Mode_Out_PP;              \
                       GPIO_Struct.GPIO_Pin   = LCD_DB_PINS;                   \
                       GPIO_Init(LCD_DB_PORT, &GPIO_Struct);                   \
                       LCD_CTRL_PORT->BSRR    = nWR_PIN | D_C_PIN | nRD_PIN;   \
                       GPIO_Struct.GPIO_Pin   = nWR_PIN | D_C_PIN | nRD_PIN;   \
                       GPIO_Init(LCD_CTRL_PORT, &GPIO_Struct);}

#define MCU_READ()    {GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;              \
                       GPIO_Struct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;         \
                       GPIO_Struct.GPIO_Pin   = LCD_DB_PINS;                   \
                       GPIO_Init(LCD_DB_PORT, &GPIO_Struct);                   \
                       GPIO_Struct.GPIO_Mode  = GPIO_Mode_Out_PP;              \
                       LCD_CTRL_PORT->BSRR    = nWR_PIN | D_C_PIN | nRD_PIN;   \
                       GPIO_Struct.GPIO_Pin   = nWR_PIN | D_C_PIN | nRD_PIN;   \
                       GPIO_Init(LCD_CTRL_PORT, &GPIO_Struct);}

#define FPGA_WRITE()  {GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;              \
                       GPIO_Struct.GPIO_Mode  = GPIO_Mode_IPU;                 \
                       GPIO_Struct.GPIO_Pin   = LCD_DB_PINS;                   \
                       GPIO_Init(LCD_DB_PORT, &GPIO_Struct);                   \
                       GPIO_Struct.GPIO_Pin   = nWR_PIN | D_C_PIN | nRD_PIN;   \
                       GPIO_Init(LCD_CTRL_PORT, &GPIO_Struct);}

#define LCD_DEV_INIT(){RCC_APB2PeriphClockCmd(LCD_IO_RCC, ENABLE);             \
                       MCU_WRITE();                                            \
                       LCD_SLCT_PORT->BSRR    = nRST_PIN;                      \
                       LCD_SLCT_PORT->BRR     = nCS_PIN;                       \
                       GPIO_Struct.GPIO_Pin   = nRST_PIN | nCS_PIN;            \
                       GPIO_Init(LCD_SLCT_PORT, &GPIO_Struct);                 \
                       GPIO_Struct.GPIO_Mode  = GPIO_Mode_AF_PP;               \
                       GPIO_Struct.GPIO_Pin   = BKL_PIN;                       \
                       GPIO_Init(BKL_PORT, &GPIO_Struct);                      \
                       BKL_PWM_CFG();                                          \
                       LCD_Init(LCD_DISP_DIR);}

//----------------------------- USB DISK 相关宏定义 ----------------------------

/******************************** END OF FILE *********************************/
