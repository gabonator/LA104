#pragma once

typedef struct
{
  vu16 CR1;
  u16  RESERVED0;
  vu16 CR2;
  u16  RESERVED1;
  vu16 SMCR;
  u16  RESERVED2;
  vu16 DIER;
  u16  RESERVED3;
  vu16 SR;
  u16  RESERVED4;
  vu16 EGR;
  u16  RESERVED5;
  vu16 CCMR1;
  u16  RESERVED6;
  vu16 CCMR2;
  u16  RESERVED7;
  vu16 CCER;
  u16  RESERVED8;
  vu16 CNT;
  u16  RESERVED9;
  vu16 PSC;
  u16  RESERVED10;
  vu16 ARR;
  u16  RESERVED11;
  vu16 RCR;
  u16  RESERVED12;
  vu16 CCR1;
  u16  RESERVED13;
  vu16 CCR2;
  u16  RESERVED14;
  vu16 CCR3;
  u16  RESERVED15;
  vu16 CCR4;
  u16  RESERVED16;
  vu16 BDTR;
  u16  RESERVED17;
  vu16 DCR;
  u16  RESERVED18;
  vu16 DMAR;
  u16  RESERVED19;
} TIM_TypeDef;

typedef struct
{
  vu32 ISR;
  vu32 IFCR;
} DMA_TypeDef;

#define PERIPH_BASE           ((u32)0x40000000)

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define DMA2_BASE             (AHBPERIPH_BASE + 0x0400)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)

#define TIM7                ((TIM_TypeDef *) TIM7_BASE)


#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)

typedef struct
{
  vu32 CCR;
  vu32 CNDTR;
  vu32 CPAR;
  vu32 CMAR;
} DMA_Channel_TypeDef;

#define DMA2_Channel4_BASE    (AHBPERIPH_BASE + 0x0444)
#define DMA2_Channel4       ((DMA_Channel_TypeDef *) DMA2_Channel4_BASE)

typedef struct
{
  vu32 CRL;
  vu32 CRH;
  vu32 IDR;
  vu32 ODR;
  vu32 BSRR;
  vu32 BRR;
  vu32 LCKR;
} GPIO_TypeDef;

typedef struct
{
  vu32 CR;
  vu32 SWTRIGR;
  vu32 DHR12R1;
  vu32 DHR12L1;
  vu32 DHR8R1;
  vu32 DHR12R2;
  vu32 DHR12L2;
  vu32 DHR8R2;
  vu32 DHR12RD;
  vu32 DHR12LD;
  vu32 DHR8RD;
  vu32 DOR1;
  vu32 DOR2;
} DAC_TypeDef;

#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)
#define DAC                 ((DAC_TypeDef *) DAC_BASE)

typedef struct
{
  vu32 CR;
  vu32 CFGR;
  vu32 CIR;
  vu32 APB2RSTR;
  vu32 APB1RSTR;
  vu32 AHBENR;
  vu32 APB2ENR;
  vu32 APB1ENR;
  vu32 BDCR;
  vu32 CSR;
} RCC_TypeDef;
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
  #define RCC                 ((RCC_TypeDef *) RCC_BASE)

typedef struct
{
  vu32 EVCR;
  vu32 MAPR;
  vu32 EXTICR[4];
} AFIO_TypeDef;

#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000)
  #define AFIO                ((AFIO_TypeDef *) AFIO_BASE)

#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)

#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)

typedef struct
{
  vu16 SR;
  u16  RESERVED0;
  vu16 DR;
  u16  RESERVED1;
  vu16 BRR;
  u16  RESERVED2;
  vu16 CR1;
  u16  RESERVED3;
  vu16 CR2;
  u16  RESERVED4;
  vu16 CR3;
  u16  RESERVED5;
  vu16 GTPR;
  u16  RESERVED6;
} USART_TypeDef;
  #define USART1              ((USART_TypeDef *) USART1_BASE)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800)

#define DMA1_Channel4_BASE    (AHBPERIPH_BASE + 0x0044)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)
#define  DMA_CCR1_EN                         ((u16)0x0001)            /* Channel enable*/
