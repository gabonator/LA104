#define LCD_PORT GPIOE->ODR

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

typedef struct
{
   uint32_t CRL;
   uint32_t CRH;
   uint32_t IDR;
   uint32_t ODR;
   uint32_t BSRR;
   uint32_t BRR;
   uint32_t LCKR;
} GPIO_TypeDef;

/*
GPIOD
#define D_C_PIN_NUM    8
#define nWR_PIN_NUM    9
#define nRD_PIN_NUM    10
*/
void Set_Pixel(uint_fast16_t Color)
{
  GPIOE->ODR = (u16)Color;
  GPIOD->BRR = GPIO_Pin_9;
  GPIOD->BSRR = GPIO_Pin_9;
}

void _delay()
{
  asm("nop");
}

void Lcd_Control(int d)
{
  GPIOD->BRR = GPIO_Pin_8;
  _delay();
  GPIOE->ODR = (u16)d;
  GPIOD->BRR = GPIO_Pin_9;
  _delay();
  GPIOD->BSRR = GPIO_Pin_9;
  _delay();
  GPIOD->BSRR = GPIO_Pin_8;
}

void Lcd_Data(int d)
{
  _delay();
  GPIOE->ODR = (u16)d;
  GPIOD->BRR = GPIO_Pin_9;
  _delay();
  GPIOD->BSRR = GPIO_Pin_9;
  _delay();
}

int Lcd_Read()
{
  // set port to read

  GPIOD->BRR = GPIO_Pin_10;
  _delay();
  int res = GPIOE->IDR;
  GPIOD->BSRR = GPIO_Pin_10;
  _delay();

  return res;
}

uint32_t LCD_RD_Type(void) 
{
  Lcd_Control(0x04);  // 0x04 returns all zeroes ???
  _delay();
  GPIOE->CRL = GPIOE->CRH = 0x44444444;
  _delay();
  uint32_t lcdType;
  Lcd_Read();
  lcdType  = Lcd_Read(); lcdType <<= 8;
  lcdType |= Lcd_Read(); lcdType <<= 8;
  lcdType |= Lcd_Read(); lcdType <<= 8;
  lcdType |= Lcd_Read();
  _delay();
  GPIOE->CRL = GPIOE->CRH = 0x33333333;
  _delay();

  return lcdType;
}


void SetScrollArea(int areaFixedBegin, int areaFixedEnd)
{
  int areaScroll = 0x140 - areaFixedBegin - areaFixedEnd;

  Lcd_Control(0x33);
  Lcd_Data(areaFixedBegin >> 8);
  Lcd_Data(areaFixedBegin & 255);
  Lcd_Data(areaScroll >> 8);
  Lcd_Data(areaScroll & 255);
  Lcd_Data(areaFixedEnd >> 8);
  Lcd_Data(areaFixedEnd & 255);
}

void SetScrollPosition(int i)
{
    Lcd_Control(0x37);
    Lcd_Data(i >> 8);
    Lcd_Data(i & 255);
}
