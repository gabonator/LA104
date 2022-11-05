#define __IO volatile

typedef struct
{
  __IO uint16_t CR1;
  uint16_t  RESERVED0;
  __IO uint16_t CR2;
  uint16_t  RESERVED1;
  __IO uint16_t SR;
  uint16_t  RESERVED2;
  __IO uint16_t DR;
  uint16_t  RESERVED3;
  __IO uint16_t CRCPR;
  uint16_t  RESERVED4;
  __IO uint16_t RXCRCR;
  uint16_t  RESERVED5;
  __IO uint16_t TXCRCR;
  uint16_t  RESERVED6;
  __IO uint16_t I2SCFGR;
  uint16_t  RESERVED7;
  __IO uint16_t I2SPR;
  uint16_t  RESERVED8;  
} SPI_TypeDef;

typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFGR;
  __IO uint32_t CIR;
  __IO uint32_t APB2RSTR;
  __IO uint32_t APB1RSTR;
  __IO uint32_t AHBENR;
  __IO uint32_t APB2ENR;
  __IO uint32_t APB1ENR;
  __IO uint32_t BDCR;
  __IO uint32_t CSR;

} RCC_TypeDef;

//SPI_TypeDef* SPI2;
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
#define CR1_SPE_Set          ((uint16_t)0x0040)
#define CR1_SPE_Reset        ((uint16_t)0xFFBF)
#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)


#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)

#define PERIPH_BASE           ((uint32_t)0x40000000)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)

#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)

#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)

#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */

typedef struct
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;

namespace Gpio {
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)

enum {P1=10, P1MOSI = 15,
      P2=11, P2MISO = 14,
      P3=8,  P3SCK2 = 13,
      P4=9,  P4CS = 12,
      BASEB = GPIOB_BASE, 
      BASED = GPIOD_BASE,
      //PINMISO = P1,
      //PINSCK = P2,
      //PINMOSI = P3,
      //PINCS = P4
     };

enum {
    StateInput = 0,           // ..00
    StateOutput10Mhz = 1,     // ..01
    StateOutput2Mhz = 2,      // ..10
    StateOutput50Mhz = 3,     // ..11

    StateInputAnalog = 0<<2,
    StateInputFloating = 1<<2,
    StateInputPull = 2<<2,

    StateOutputPushPull = 0<<2,
    StateOutputOpenDrain = 1<<2,
    StateOutputFunctionPushPull = 2<<2,
    StateOutputFunctionOpenDrain = 3<<2,

};


void SetState(uint32_t nPortBase, int nPin, int nState)
{
   int nPinPos = nPin & 7;
   uint32_t dwMask = ~(0xf << (nPinPos*4));
   uint32_t dwBits = nState << (nPinPos*4);

  if (nPin < 8)
  {
    uint32_t* pCRL = (uint32_t*)nPortBase;
    *pCRL = (*pCRL & dwMask) | dwBits;
  } else
  {
    uint32_t* pCRH = (uint32_t*)(nPortBase + 4);
    *pCRH = (*pCRH & dwMask) | dwBits;
  }
}

void SetLevel(uint32_t nPortBase, int nPin, bool value)
{
    uint32_t* pODR = &((uint32_t*)nPortBase)[3];
  if (value)
  {
      *pODR |= 1<<nPin;
  } else
  {
      *pODR &= ~(1<<nPin);
  }
}

bool GetLevel(uint32_t nPortBase, int nPin)
{
  return (((uint32_t*)nPortBase)[2] >> nPin) & 1;
}

}

class CSpi // : public Platform::CGpio
{
public:
    void begin() const
    {
        Gpio::SetState(Gpio::BASEB, Gpio::P1, Gpio::StateInput | Gpio::StateInputFloating);
        Gpio::SetState(Gpio::BASEB, Gpio::P2, Gpio::StateInput | Gpio::StateInputFloating);
        Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput | Gpio::StateInputFloating);
        Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput | Gpio::StateInputFloating);

        Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
        Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
        Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
        Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);

        Gpio::SetLevel(Gpio::BASEB, Gpio::P1, false);
        Gpio::SetLevel(Gpio::BASEB, Gpio::P2, false);
        Gpio::SetLevel(Gpio::BASEB, Gpio::P3, false);
        Gpio::SetLevel(Gpio::BASEB, Gpio::P4, false);

        deselect();
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB1Periph_SPI2              ((uint32_t)0x00004000)
#define  RCC_CFGR_HPRE                       ((uint32_t)0x000000F0)        /*!< HPRE[3:0] bits (AHB prescaler) */

#define  RCC_CFGR_PPRE1                      ((uint32_t)0x00000700)        /*!< PRE1[2:0] bits (APB1 prescaler) */
#define  RCC_CFGR_PPRE2                      ((uint32_t)0x00003800)        /*!< PRE2[2:0] bits (APB2 prescaler) */

#define  RCC_CFGR_PPRE1                      ((uint32_t)0x00000700)        /*!< PRE1[2:0] bits (APB1 prescaler) */
#define  RCC_CFGR_PPRE1_0                    ((uint32_t)0x00000100)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE1_1                    ((uint32_t)0x00000200)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE1_2                    ((uint32_t)0x00000400)        /*!< Bit 2 */

#define  RCC_CFGR_PPRE1_DIV1                 ((uint32_t)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE1_DIV2                 ((uint32_t)0x00000400)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE1_DIV4                 ((uint32_t)0x00000500)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE1_DIV8                 ((uint32_t)0x00000600)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE1_DIV16                ((uint32_t)0x00000700)        /*!< HCLK divided by 16 */


//#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
RCC->APB1ENR |= RCC_APB1Periph_SPI2;
RCC->CFGR &= ~RCC_CFGR_HPRE;
RCC->CFGR &= ~RCC_CFGR_PPRE1;
RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
RCC->CFGR &= ~RCC_CFGR_PPRE2;

RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

        // configure spi        
        //SPI2->CR1 = 0;
        uint32_t tmpreg = SPI2->CR1;
        #define CR1_CLEAR_Mask       ((uint16_t)0x3040)

        tmpreg &= CR1_CLEAR_Mask;
  /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
     master/salve mode, CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
  /* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values */
  /* Set LSBFirst bit according to SPI_FirstBit value */
  /* Set BR bits according to SPI_BaudRatePrescaler value */
  /* Set CPOL bit according to SPI_CPOL value */
  /* Set CPHA bit according to SPI_CPHA value */
  uint32_t SPI_Direction = 0; //SPI_Direction_2Lines_FullDuplex
  uint32_t SPI_Mode = 0x0104; //SPI_Mode_Master;
  uint32_t SPI_DataSize = 0;
  uint32_t SPI_CPOL = 0; // low
  uint32_t SPI_CPHA = 0; // SPI_CPHA_1Edge
  uint32_t SPI_NSS = 0x200; // SPI_NSS_Hard, SPI_NSS_Soft
  uint32_t SPI_BaudRatePrescaler = 0; //0x0030; // SPI_BaudRatePrescaler_128
  uint32_t SPI_FirstBit = 0x00; // SPI_FirstBit_MSB;
        tmpreg |= (uint16_t)((uint32_t)SPI_Direction | SPI_Mode |
                  SPI_DataSize | SPI_CPOL |  
                  SPI_CPHA | SPI_NSS |  
                  SPI_BaudRatePrescaler | SPI_FirstBit);
  /* Write to SPIx CR1 */
  SPI2->CR1 = tmpreg;
  
        // enable spi 
        SPI2->CR1 |= CR1_SPE_Set;
        SPI2->CR2 = 0;
        SPI2->CRCPR = 7;

    }
    #define CR1_SPE_Reset        ((uint16_t)0xFFBF)

    void end() const
    {
        SPI2->CR1 &= CR1_SPE_Reset;
        
        Gpio::SetState(Gpio::BASEB, Gpio::P1, Gpio::StateInput);
        Gpio::SetState(Gpio::BASEB, Gpio::P2, Gpio::StateInput);
        Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput);
        Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput);

        Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput);
        Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput);
        Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateInput);
        Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateInput);
    }

    void transferBytes(uint8_t* bufWrite, uint8_t* bufRead, int len) const
    {
        if (!bufRead)
        {
          uint16_t d;
          while (len--)
          {
              d = (uint16_t)*bufWrite++;
              SPI2->DR = d;
              while (!(SPI2->SR & SPI_I2S_FLAG_RXNE));
              d = SPI2->DR;
          }
          return;
        }
        while (len--)
        {
            uint16_t snd = bufWrite ? *bufWrite++ : 0xff;
            SPI2->DR = snd;
	    while (!(SPI2->SR & SPI_I2S_FLAG_RXNE));
	    uint16_t rcv = SPI2->DR;

            if (bufRead)
                *bufRead++ = rcv;
        }
    }

    void select() const
    {
        Gpio::SetLevel(Gpio::BASEB, Gpio::P4CS, false);
    }

    void deselect() const
    {
        Gpio::SetLevel(Gpio::BASEB, Gpio::P4CS, true);
    }

    uint8_t transfer(uint8_t b) const
    {
        uint8_t dataIn;
        transferBytes(&b, &dataIn, 1);
        return dataIn;
    }
    
    uint16_t transfer16(uint16_t b) const
    {
        uint8_t dataOut[2] = {(uint8_t)(b >> 8), (uint8_t)b};
        uint8_t dataIn[2];
        transferBytes(dataOut, dataIn, 2);
        return dataIn[1] | (dataIn[0] << 8);
    }

    uint32_t transfer32(uint32_t b) const
    {
        uint8_t dataOut[4] = {(uint8_t)(b >> 24), (uint8_t)(b >> 16), (uint8_t)(b >> 8), (uint8_t)b};
        uint8_t dataIn[4];
        transferBytes(dataOut, dataIn, 4);
        return dataIn[3] | (dataIn[2] << 8) | (dataIn[1] << 16) | (dataIn[0] << 24);
    }
    
};
