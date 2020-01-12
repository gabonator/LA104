#if 0




//static inline volatile uint32_t* __bb_addr(volatile void *address,
//                                         uint32_t bit,
//                                         uint32_t bb_base,
//                                         uint32_t bb_ref) {
//    return (volatile uint32_t*)(bb_base + ((uint32)address - bb_ref) * 32 +
//                              bit * 4);
//}
//
//static inline volatile uint32_t* bb_perip(volatile void *address, uint32_t bit) {
//    return __bb_addr(address, bit, BB_PERI_BASE, BB_PERI_REF);
//}
//
//static inline void bb_peri_set_bit(volatile void *address,
//                                   uint32_t bit,
//                                   uint8_t val) {
//    *bb_perip(address, bit) = val;
//}
//
//void dma_init(dma_dev *dev) {
//    rcc_clk_enable(dev->clk_id);
//}
//
//void dma_setup_transfer(dma_dev       *dev,
//                        dma_channel    channel,
//                        __IO void     *peripheral_address,
//                        dma_xfer_size  peripheral_size,
//                        __IO void     *memory_address,
//                        dma_xfer_size  memory_size,
//                        uint32         mode) {
//    dma_channel_reg_map *channel_regs = dma_channel_regs(dev, channel);
//
//    dma_disable(dev, channel);  /* can't write to CMAR/CPAR otherwise */
//    channel_regs->CCR = (memory_size << 10) | (peripheral_size << 8) | mode;
//    channel_regs->CMAR = (uint32)memory_address;
//    channel_regs->CPAR = (uint32)peripheral_address;
//}
//
//void dma_disable(dma_dev *dev, dma_channel channel) {
//    dma_channel_reg_map *chan_regs = dma_channel_regs(dev, channel);
//    bb_peri_set_bit(&chan_regs->CCR, DMA_CCR_EN_BIT, 0);
//}
//
//void dma_set_num_transfers(dma_dev *dev,
//                           dma_channel channel,
//                           uint16 num_transfers) {
//    dma_channel_reg_map *channel_regs;
//
//    ASSERT_FAULT(!dma_is_channel_enabled(dev, channel));
//
//    channel_regs = dma_channel_regs(dev, channel);
//    channel_regs->CNDTR = num_transfers;
//}
//
//static inline void dma_clear_isr_bits(dma_dev *dev, dma_tube tube) {
//    dma_reg_map *regs = dev->regs;
//    __IO uint32 *ifcr = tube > DMA_S3 ? &regs->HIFCR : &regs->LIFCR;
//    *ifcr = (0x3D << _dma_sr_fcr_shift(tube));
//}
//
//static inline void dma_clear_isr_bits(dma_dev *dev, dma_tube tube) {
//    dev->regs->IFCR = (1U << (4 * (tube - 1)));
//}
//
//void dma_enable(dma_dev *dev, dma_channel channel) {
//    dma_channel_reg_map *chan_regs = dma_channel_regs(dev, channel);
//    bb_peri_set_bit(&chan_regs->CCR, DMA_CCR_EN_BIT, 1);
//}
//
//void spi_tx_dma_enable(spi_dev *dev) {
//    bb_peri_set_bit(&dev->regs->CR2, SPI_CR2_TXDMAEN_BIT, 1);
//}
//


#define PERIPH_BASE           ((uint32_t)0x40000000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)

#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)

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

SPI_TypeDef* SPIx = SPI2;


CSPI SPI;

enum
{
	SYSINFO, NIVCPTR, SYSTICK, AF_RMAP, PWRCTRL, BUZZDEV, KEYnDEV, DELAYuS,
	DISPDEV, BLOCK_X, BLOCK_Y, PIXEL_X, PIXEL_Y, WrPIXEL, RdPIXEL, FONTPTR,
	FPGADEV, IN_PORT, USBDEV, U_DISK, SPI_DEV, FLSHDEV, EXT_INP, EXT_OUT,
	EXT_PWM, EXT_INF, EXT_SPI, EXT_UART, EXT_I2C, EXT_RXD, EXT_TXD // 27
};

#define INIT 0xFFFF0000

void __Bios(uint32_t a, uint32_t b)
{
	typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
	static Tfunc ___Bios = (Tfunc)(0x08000100|1);

	___Bios(a, b);
}
void setClock()
{
	/*
	#define PERIPH_BASE           ((uint32_t)0x40000000)
	#define APB1PERIPH_BASE       PERIPH_BASE
	#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)

	#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
	#define SPI2                ((SPI_TypeDef *) SPI2_BASE)

	typedef struct
	{
	  volatile uint16_t CR1;
	}  SPI_TypeDef;

	SPI_TypeDef* SPIx = SPI2;
*/
//	#define SPI_BaudRatePrescaler_32           ((u16)0x0020)
	#define SPI_BaudRatePrescaler_2            ((u16)0x0000)
	#define SPI_BaudRatePrescaler_4            ((u16)0x0008)
	#define SPI_BaudRatePrescaler_8            ((u16)0x0010)
	#define SPI_BaudRatePrescaler_16           ((u16)0x0018)
	#define SPI_BaudRatePrescaler_32           ((u16)0x0020)
	#define SPI_BaudRatePrescaler_64           ((u16)0x0028)
	#define SPI_BaudRatePrescaler_128          ((u16)0x0030)
	#define SPI_BaudRatePrescaler_256          ((u16)0x0038)


	int clockDivider = SPI_BaudRatePrescaler_16;
	const uint32_t SPI_CR1_BR = 0x38;
	uint16_t cr1 = SPIx->CR1 & ~(SPI_CR1_BR);
	SPIx->CR1 = cr1 | (clockDivider & SPI_CR1_BR);

}
void CSPI::begin()
{
	BIOS::DBG::Print("Init1");
	__Bios(EXT_SPI, INIT);
	BIOS::DBG::Print("Init2");
	BIOS::SYS::DelayMs(100);
	setClock();
	BIOS::SYS::DelayMs(100);
	BIOS::DBG::Print("InitOk");

}

void CSPI::end()
{
	
}

void CSPI::setClockDivider(int clockDivider)
{
}

void Send(uint8_t d)
{
//
//			// TODO: pragma pack???
//			typedef struct
//			{
//			  volatile uint16_t CR1;
//			  uint16_t  RESERVED0;
//			  volatile uint16_t CR2;
//			  uint16_t  RESERVED1;
//			  volatile uint16_t SR;
//			  uint16_t  RESERVED2;
//			  volatile uint16_t DR;
//			  uint16_t  RESERVED3;
//			  volatile uint16_t CRCPR;
//			  uint16_t  RESERVED4;
//			  volatile uint16_t RXCRCR;
//			  uint16_t  RESERVED5;
//			  volatile uint16_t TXCRCR;
//			  uint16_t  RESERVED6;
//			  volatile uint16_t I2SCFGR;
//			  uint16_t  RESERVED7;
//			  volatile uint16_t I2SPR;
//			  uint16_t  RESERVED8;
//			} /*__attribute__((packed))*/ SPI_TypeDef;
//
//			#define PERIPH_BASE           ((uint32_t)0x40000000)
//			#define APB1PERIPH_BASE       PERIPH_BASE
//			#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
//
//			#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
//			#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
//
//			SPI_TypeDef* SPIx = SPI2;
	
	#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

	#ifndef EMULATED
	SPIx->DR = d;
	while( !(SPIx->SR & SPI_I2S_FLAG_TXE) );
	#endif

}

void CSPI::dmaSendAsync(uint8_t* data, int length)
{
	
	BIOS::DBG::Print("Send %04x, ", SPIx->CR1);
	/*
			typedef struct
			{
				uint16_t  n;
				uint8_t  *pBuf;
			}  SPI_ParamDef;

			static SPI_ParamDef  Param;

	Param.n    = 2;//length;
			Param.pBuf = data;
			
			__Bios(EXT_SPI, (uint32_t)&Param);

	return;
	setClock();
*/
	while(length--)
		Send(*data++);
	return;
//
//	typedef enum dma_mode_flags {
//		DMA_MEM_2_MEM  = 1 << 14, /**< Memory to memory mode */
//		DMA_MINC_MODE  = 1 << 7,  /**< Auto-increment memory address */
//		DMA_PINC_MODE  = 1 << 6,  /**< Auto-increment peripheral address */
//		DMA_CIRC_MODE  = 1 << 5,  /**< Circular mode */
//		DMA_FROM_MEM   = 1 << 4,  /**< Read from memory to peripheral */
//		DMA_TRNS_ERR   = 1 << 3,  /**< Interrupt on transfer error */
//		DMA_HALF_TRNS  = 1 << 2,  /**< Interrupt on half-transfer */
//		DMA_TRNS_CMPLT = 1 << 1   /**< Interrupt on transfer completion */
//	} dma_mode_flags;
//
//	typedef enum dma_xfer_size {
//		DMA_SIZE_8BITS  = 0,        /**< 8-bit transfers */
//		DMA_SIZE_16BITS = 1,        /**< 16-bit transfers */
//		DMA_SIZE_32BITS = 2,        /**< 32-bit transfers */
//	} dma_xfer_size;
//
//    uint32 flags = DMA_MINC_MODE | DMA_FROM_MEM | DMA_TRNS_CMPLT;
//
//    dma_init(DMA1);
	
	
	///
	
	typedef struct
	{
	  volatile uint32_t ISR;
	  volatile uint32_t IFCR;
	} DMA_TypeDef;
	#define DMA1_BASE             (AHBPERIPH_BASE + 0x0000)

	#define DMA1                ((DMA_TypeDef *) DMA1_BASE)

	
	//////
	
	
	typedef struct
	{
	  volatile uint32_t CR;
	  volatile uint32_t CFGR;
	  volatile uint32_t CIR;
	  volatile uint32_t APB2RSTR;
	  volatile uint32_t APB1RSTR;
	  volatile uint32_t AHBENR;
	  volatile uint32_t APB2ENR;
	  volatile uint32_t APB1ENR;
	  volatile uint32_t BDCR;
	  volatile uint32_t CSR;
	} RCC_TypeDef;
	#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

	#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)

	#define RCC                 ((RCC_TypeDef *) RCC_BASE)

	#define RCC_AHBPeriph_DMA1               ((uint32_t)0x00000001)

    RCC->AHBENR |= RCC_AHBPeriph_DMA1;
	BIOS::DBG::Print("3, ");

//    // TX
//    dma_xfer_size dma_bit_size = DMA_SIZE_8BITS;
//
//    dma_setup_transfer(DMA1, DMA_CH5, &SPIx->DR, dma_bit_size,
//    (volatile void*)data, dma_bit_size, flags); // Transmit buffer DMA
//    dma_set_num_transfers(DMA1, DMA_CH5, length);
	
	typedef struct
	{
	  volatile uint32_t CCR;
	  volatile uint32_t CNDTR;
	  volatile uint32_t CPAR;
	  volatile uint32_t CMAR;
	} DMA_Channel_TypeDef;

	#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
	#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
	#define DMA1_Channel5_BASE    (AHBPERIPH_BASE + 0x0058)
	#define DMA1_Channel5       ((DMA_Channel_TypeDef *) DMA1_Channel5_BASE)

	
	#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010) // DMA_FROM_MEM
	#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
	#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
	#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
	#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080) // MINC
	#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
	#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
	#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
	#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
	#define DMA_Priority_Low                   ((uint32_t)0x00000000)
	#define DMA_M2M_Disable                    ((uint32_t)0x00000000)

	#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)

	DMA_Channel_TypeDef* DMAy_Channelx = (DMA_Channel_TypeDef*)DMA1_Channel5;
	BIOS::DBG::Print("2, ");

	uint32_t tmpreg = 0;
	tmpreg = DMAy_Channelx->CCR;
	tmpreg &= CCR_CLEAR_Mask;
	tmpreg |= DMA_DIR_PeripheralDST | DMA_Mode_Normal |
		DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable |
		DMA_PeripheralDataSize_Byte | DMA_MemoryDataSize_Byte |
		DMA_Priority_Low | DMA_M2M_Disable;
	DMAy_Channelx->CCR = tmpreg;
	DMAy_Channelx->CNDTR = length;
	DMAy_Channelx->CPAR = (uint32_t)&SPIx->DR;
	DMAy_Channelx->CMAR = (uint32_t)data;

	BIOS::DBG::Print("1, ");

//    dma_clear_isr_bits(DMA1, DMA_CH5);
    DMA1->IFCR = (1U << (4 * (5 - 1)));

//    dma_enable(DMA1, DMA_CH5);// enable transmit
	#define  DMA_CCR1_EN                         ((uint16_t)0x0001)            /*!< Channel enable*/

	#define SPI_I2S_DMAReq_Tx               ((uint16_t)0x0002)
	SPIx->CR2 |= SPI_I2S_DMAReq_Tx;
//    spi_tx_dma_enable(SPIx);
    DMAy_Channelx->CCR |= DMA_CCR1_EN;
	BIOS::DBG::Print("Ok, ");

}







#if 0
		/*
		void _Bios(uint32_t a, uint32_t b)
		{
	#ifndef EMULATED
			typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
			static Tfunc __Bios = (Tfunc)(0x08000100|1);
			__Bios(a, b);
	#endif
		}*/
	void CSPI::dmaSendAsync(uint8_t* data, int len)
	{
		BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::Output);

		enum
		{
			SYSINFO, NIVCPTR, SYSTICK, AF_RMAP, PWRCTRL, BUZZDEV, KEYnDEV, DELAYuS,
			DISPDEV, BLOCK_X, BLOCK_Y, PIXEL_X, PIXEL_Y, WrPIXEL, RdPIXEL, FONTPTR,
			FPGADEV, IN_PORT, USBDEV, U_DISK, SPI_DEV, FLSHDEV, EXT_INP, EXT_OUT,
			EXT_PWM, EXT_INF, EXT_SPI, EXT_UART, EXT_I2C, EXT_RXD, EXT_TXD
		};

		typedef struct
		{
			uint16_t  n;
			uint8_t  *pBuf;
		} __attribute__((packed)) SPI_ParamDef;

		static SPI_ParamDef  Param;

		Param.n    = len;
		Param.pBuf = data;
		
#ifndef EMULATED
		typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
		static Tfunc __Bios = (Tfunc)(0x08000100|1);

		__Bios(EXT_SPI, (uint32_t)&Param);
#endif
	}

	void configure()
	{
		// TODO: pragma pack???
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
		#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
		#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
		#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
		#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
		#define SPI3                ((SPI_TypeDef *) SPI3_BASE)

		SPI_TypeDef* SPIx = SPI2;
		typedef uint16_t u16;
		#define SPI_BaudRatePrescaler_2            ((u16)0x0000)
		#define SPI_BaudRatePrescaler_4            ((u16)0x0008)
		#define SPI_BaudRatePrescaler_8            ((u16)0x0010)
		#define SPI_BaudRatePrescaler_16           ((u16)0x0018)
		#define SPI_BaudRatePrescaler_32           ((u16)0x0020)
		#define SPI_BaudRatePrescaler_64           ((u16)0x0028)
		#define SPI_BaudRatePrescaler_128          ((u16)0x0030)
		#define SPI_BaudRatePrescaler_256          ((u16)0x0038)

		int clockDivider = SPI_BaudRatePrescaler_256;
		
		const uint32_t SPI_CR1_BR = 0x38;
#ifndef EMULATED
		uint16_t cr1 = SPIx->CR1 & ~(SPI_CR1_BR);
		SPIx->CR1 = cr1 | (clockDivider & SPI_CR1_BR);
		
		#define CR1_CLEAR_Mask       ((uint16_t)0x3040)
		#define SPI_Mode_Select      ((uint16_t)0xF7FF)

		#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
		#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0400)
		#define SPI_Direction_1Line_Rx          ((uint16_t)0x8000)
		#define SPI_Direction_1Line_Tx          ((uint16_t)0xC000)
		#define SPI_Mode_Master                 ((uint16_t)0x0104)
		#define SPI_Mode_Slave                  ((uint16_t)0x0000)
		#define SPI_DataSize_8b                 ((uint16_t)0x0000)
		#define SPI_CPOL_Low                    ((uint16_t)0x0000)
		#define SPI_CPOL_High                   ((uint16_t)0x0002)
		#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
		#define SPI_CPHA_2Edge                  ((uint16_t)0x0001)
		#define SPI_NSS_Hard                    ((uint16_t)0x0000)
		#define SPI_FirstBit_MSB                ((uint16_t)0x0000)

		uint16_t tmpreg = 0;

		tmpreg = SPIx->CR1;
		/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
		tmpreg &= CR1_CLEAR_Mask;
		/* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
		   master/salve mode, CPOL and CPHA */
		/* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
		/* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values */
		/* Set LSBFirst bit according to SPI_FirstBit value */
		/* Set BR bits according to SPI_BaudRatePrescaler value */
		/* Set CPOL bit according to SPI_CPOL value */
		/* Set CPHA bit according to SPI_CPHA value */
		tmpreg |= SPI_Direction_2Lines_FullDuplex | SPI_Mode_Master | SPI_DataSize_8b |
		SPI_CPOL_Low | SPI_CPHA_1Edge | SPI_NSS_Hard | SPI_FirstBit_MSB |
		SPI_BaudRatePrescaler_256;
		/*(uint16_t)((uint32_t)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_Mode |
						SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |
						SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_NSS |
						SPI_InitStruct->SPI_BaudRatePrescaler | SPI_InitStruct->SPI_FirstBit);*/
		/* Write to SPIx CR1 */
		SPIx->CR1 = tmpreg;

		/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
		SPIx->I2SCFGR &= SPI_Mode_Select;
		
// SPI Cmd - enable
		#define CR1_SPE_Set          ((uint16_t)0x0040)

		SPIx->CR1 |= CR1_SPE_Set;

#endif
		
	}
	
	void Send(uint8_t d)
	{
				// TODO: pragma pack???
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
				} /*__attribute__((packed))*/ SPI_TypeDef;
				
				#define PERIPH_BASE           ((uint32_t)0x40000000)
				#define APB1PERIPH_BASE       PERIPH_BASE
				#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)

				#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
				#define SPI2                ((SPI_TypeDef *) SPI2_BASE)

				SPI_TypeDef* SPIx = SPI2;
		
		#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

		#ifndef EMULATED
		SPIx->DR = d;
		//while( !(SPIx->SR & SPI_I2S_FLAG_TXE) );
		#endif

	}


#endif



#ifdef BITBANG

void CSPI::begin()
{/*
	enum
	{
		SYSINFO, NIVCPTR, SYSTICK, AF_RMAP, PWRCTRL, BUZZDEV, KEYnDEV, DELAYuS,
		DISPDEV, BLOCK_X, BLOCK_Y, PIXEL_X, PIXEL_Y, WrPIXEL, RdPIXEL, FONTPTR,
		FPGADEV, IN_PORT, USBDEV, U_DISK, SPI_DEV, FLSHDEV, EXT_INP, EXT_OUT,
		EXT_PWM, EXT_INF, EXT_SPI, EXT_UART, EXT_I2C, EXT_RXD, EXT_TXD // 27
	};

	#define INIT 0xFFFF0000
	typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
	static Tfunc __Bios = (Tfunc)(0x08000100|1);

	//__Bios(EXT_SPI, INIT);
  */
	BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::Output);
	BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P1, 1);
}

void CSPI::end()
{
	
}

void CSPI::setClockDivider(int clockDivider)
{
}

void CSPI::dmaSendAsync(uint8_t* data, int len)
{
	while (len--)
	{
		int d = *data++;
		int j = 8;
		while (j--)
		{
			BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P1, d&1);
			d >>= 1;
			__asm__("");
			__asm__("");
			__asm__("");
		}
	}
	BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P1, 1);
}






#endif
#endif
