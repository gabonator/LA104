namespace SPI
{
#if !defined(EMULATED) && defined(LA104)
  void __Bios(uint32_t a, uint32_t b)
  {
  	typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
  	static Tfunc ___Bios = (Tfunc)(0x08000100|1);

  	___Bios(a, b);
  }

  typedef struct
  {
    volatile uint16_t CR1;
    uint16_t  RESERVED0;
    volatile uint16_t CR2;
    uint16_t  RESERVED1;
    volatile uint16_t SR;
    uint16_t  RESERVED2;
    volatile uint16_t DR;
    uint16_t  RESERVED3;
    volatile uint16_t CRCPR;
    uint16_t  RESERVED4;
    volatile uint16_t RXCRCR;
    uint16_t  RESERVED5;
    volatile uint16_t TXCRCR;
    uint16_t  RESERVED6;
    volatile uint16_t I2SCFGR;
    uint16_t  RESERVED7;
    volatile uint16_t I2SPR;
    uint16_t  RESERVED8;
  }  SPI_TypeDef;

  #define PERIPH_BASE           ((uint32_t)0x40000000)
  #define APB1PERIPH_BASE       PERIPH_BASE
  #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
  #define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
  #define SPI2                ((SPI_TypeDef *) SPI2_BASE)

  SPI_TypeDef* SPIx = SPI2;

  void begin(int baudrate)
  {
  	enum
  	{
  		SYSINFO, NIVCPTR, SYSTICK, AF_RMAP, PWRCTRL, BUZZDEV, KEYnDEV, DELAYuS,
  		DISPDEV, BLOCK_X, BLOCK_Y, PIXEL_X, PIXEL_Y, WrPIXEL, RdPIXEL, FONTPTR,
  		FPGADEV, IN_PORT, USBDEV, U_DISK, SPI_DEV, FLSHDEV, EXT_INP, EXT_OUT,
  		EXT_PWM, EXT_INF, EXT_SPI, EXT_UART, EXT_I2C, EXT_RXD, EXT_TXD // 27
  	};

  	#define INIT 0xFFFF0000
  	
  	__Bios(EXT_SPI, INIT);

  	#define SPI_BaudRatePrescaler_16           ((u16)0x0018)

  	int clockDivider = SPI_BaudRatePrescaler_16;
  	const uint32_t SPI_CR1_BR = 0x38;
  	uint16_t cr1 = SPIx->CR1 & ~(SPI_CR1_BR);
  	SPIx->CR1 = cr1 | (clockDivider & SPI_CR1_BR);
  }

  void write(uint8_t d)
  {
  	#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

  	SPIx->DR = d;
  	while( !(SPIx->SR & SPI_I2S_FLAG_TXE) );
  }

  void write(uint8_t* data, int length)
  {
  	while(length--)
  		write(*data++);
  }
#else
  void begin(int baudrate)
  {
  }
  void write(uint8_t)
  {
  }
  void write(uint8_t*, int)
  {
  }
#endif
}