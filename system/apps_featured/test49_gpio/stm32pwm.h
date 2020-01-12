#define __IO
typedef struct
{
	__IO uint16_t CR1;
	uint16_t  RESERVED0;
	__IO uint16_t CR2;
	uint16_t  RESERVED1;
	__IO uint16_t SMCR;
	uint16_t  RESERVED2;
	__IO uint16_t DIER;
	uint16_t  RESERVED3;
	__IO uint16_t SR;
	uint16_t  RESERVED4;
	__IO uint16_t EGR;
	uint16_t  RESERVED5;
	__IO uint16_t CCMR1;
	uint16_t  RESERVED6;
	__IO uint16_t CCMR2;
	uint16_t  RESERVED7;
	__IO uint16_t CCER;
	uint16_t  RESERVED8;
	__IO uint16_t CNT;
	uint16_t  RESERVED9;
	__IO uint16_t PSC;
	uint16_t  RESERVED10;
	__IO uint16_t ARR;
	uint16_t  RESERVED11;
	__IO uint16_t RCR;
	uint16_t  RESERVED12;
	__IO uint16_t CCR1;
	uint16_t  RESERVED13;
	__IO uint16_t CCR2;
	uint16_t  RESERVED14;
	__IO uint16_t CCR3;
	uint16_t  RESERVED15;
	__IO uint16_t CCR4;
	uint16_t  RESERVED16;
	__IO uint16_t BDTR;
	uint16_t  RESERVED17;
	__IO uint16_t DCR;
	uint16_t  RESERVED18;
	__IO uint16_t DMAR;
	uint16_t  RESERVED19;
} TIM_TypeDef;

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define APB1PERIPH_BASE       PERIPH_BASE
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)

