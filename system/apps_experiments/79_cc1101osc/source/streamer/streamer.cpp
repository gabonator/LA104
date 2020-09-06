// based on https://github.com/PetteriAimonen/dso-quad-logic.git

#include "streamer.h"
#include <library.h>
#include "stm32f10x.h"
#include "ringbuf.h"
#include "dma.h"

// Note: Uses optimize("O2") to work around this bug:
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=39585
#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

#define ADC_FIFO_SIZE 1024 // TODO: velky buffer, bity maju periodu 20 cyklov!
// 128 -> 64 half buffer => 1ms
// 512 -> 256 half buffer => 10-11ms
//#define ADC_FIFO_SIZE 512
#define ADC_FIFO_HALFSIZE (ADC_FIFO_SIZE/2)


// 0x40000000 + 0x20000 + 0x0044 DMA1_Channel4
/*

0x40020048 <- tuto je timer


0x40000000 + 0x10000 + 0x2C00 TIM1
0x40012c00+9*4
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



40020044
typedef struct
{
  __IO uint32_t CCR;   /// TOTO
  __IO uint32_t CNDTR; 
  __IO uint32_t CPAR;
  __IO uint32_t CMAR;
} DMA_Channel_TypeDef;

*/

BIOS::OS::TInterruptHandler oldDMA1_Channel4_IRQ = nullptr;

volatile uint16_t adc_fifo[ADC_FIFO_SIZE];
volatile int streamerOverrun = 0;

bool streamerBufferLogic = 0;
int streamerBufferCounter = 0;
// 49.5 kHz !!
int streamerBufferMaxCounter = 20000; // 1 second @ 50 kHz
const int streamerPeriodUs = 20;
volatile int totalSamples = 0;
volatile int totalPulses = 0;

RingBufCPP<uint16_t, 512> streamerBuffer;

uint16_t streamerSecondary[64];
uint32_t streamerSecondaryIndex = 0;
uint32_t streamerTick = 0;
volatile int streamerFlag = 0;

#define GetLogic(idr) (((idr) >> 9) & 1) // B9

void secondaryPush(int c);

uint32_t streamerCCR()
{
  return *((volatile uint32_t*)0x40020048); //DMA1_Channel4->CCR;
}

uint32_t streamerCurrent()
{
  int ccr = (1024 - DMA1_Channel4->CCR)&511;

  return totalSamples + ccr; // consider half-sized buffers, probably & (ADC_FIFO_HALFSIZE-1)
}

void streamerProcess(const volatile uint16_t *data)
{
  const volatile uint16_t *end = data + ADC_FIFO_HALFSIZE;
  while (data < end)
  {   
    totalSamples++;
    int sample = GetLogic(*data++); 
    if (sample == streamerBufferLogic)
    {
      streamerBufferCounter++;
      // append [60000, 0] to buffer
      if (streamerBufferCounter >= streamerBufferMaxCounter)
      {
        secondaryPush(streamerBufferCounter);
        secondaryPush(0);

        if (streamerBuffer.capacity() > 2)
        {                                           
          streamerBuffer.push(streamerBufferCounter | streamerFlag);
          streamerBuffer.push(0 | streamerFlag);
        } else
        {
          streamerOverrun++;
        }
        streamerBufferCounter = 0;
      }
    } else
    {
      // append ticks to buffer and toggle logic
      secondaryPush(streamerBufferCounter);

      if (streamerBuffer.capacity() > 1)
        streamerBuffer.push(streamerBufferCounter | streamerFlag);
      else
        streamerOverrun++;

      streamerBufferCounter = 0;
      streamerBufferLogic = 1 - streamerBufferLogic;
      totalPulses++;
    }
  }
}

void __irq__ streamerIrqHandler()
{
    if (DMA1->ISR & DMA_ISR_TEIF4)
    {
        BIOS::DBG::Print("Error: DMA channel 4 transfer error!");
        while(1);
    }
    else if (DMA1->ISR & DMA_ISR_HTIF4)
    {
        streamerProcess(&adc_fifo[0]);
        DMA1->IFCR = DMA_IFCR_CHTIF4;
        if (DMA1->ISR & DMA_ISR_TCIF4)
        {
            BIOS::DBG::Print("Error: ADC fifo overflow in HTIF");
            while(1);
        }
    }
    else if (DMA1->ISR & DMA_ISR_TCIF4)
    {
        streamerProcess(&adc_fifo[ADC_FIFO_HALFSIZE]);
        DMA1->IFCR = DMA_IFCR_CTCIF4;
        if (DMA1->ISR & DMA_ISR_HTIF4)
        {
            BIOS::DBG::Print("Error: ADC fifo overflow in TCIF");
            while(1);
        }
    }
}

void streamerInit()
{
  oldDMA1_Channel4_IRQ = BIOS::OS::GetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel4_IRQ);
  BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel4_IRQ, streamerIrqHandler);
  __HAL_RCC_DMA1_CLK_ENABLE();
}

void streamerDeinit()
{
  BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel4_IRQ, oldDMA1_Channel4_IRQ);
}

void streamerStart()
{
    // TIMER1 channel 4 -> DMA1 channel 2
    TIM1->CR1 = 0;
    TIM1->CR2 = 0;
    TIM1->CNT = 0;
    TIM1->SR = 0;

    // freq = 72MHz / 12 / psc
    // psc = 72e6 / 12 / freq
    // psc = 120 -> 50kHz

// 1200 -> 85 samples per second!?

    // freq = 72MHz / 24 / psc
    // psc = 72e6 / 24 / freq
    // psc = 60 -> 40kHz (1ms as 39 pulses)
    // psc = 48 -> 50kHz (1ms as 61 pulses)
    // psc = 53          (1ms as 51.3 pulses)
    // psc = 54          (1ms as 48.5 pulses), 40 pulses per 1us

    int psc = 54;
    TIM1->PSC = psc - 1;
    TIM1->ARR = psc/2 - 1;
    TIM1->CCMR1 = 0x0000; // CC2 time base
    TIM1->CCMR2 = 0x0000; // CC4 time base
    TIM1->DIER = TIM_DIER_CC4DE;
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR4 = 0;

    DMA1_Channel4->CCR = 0;
    DMA1_Channel4->CNDTR = sizeof(adc_fifo) / 2;
    DMA1_Channel4->CPAR = (uint32_t)&GPIOB->IDR;
    DMA1_Channel4->CMAR = (uint32_t)adc_fifo;
    
    DMA1_Channel4->CCR = DMA_PRIORITY_VERY_HIGH | DMA_PDATAALIGN_HALFWORD | DMA_MDATAALIGN_HALFWORD |
      DMA_PINC_DISABLE | DMA_MINC_ENABLE | DMA_CIRCULAR | 0xf;

    // Clear any pending interrupts for ch 4
    DMA1->IFCR = 0x0000F000;
    
    // Enable ch 4 interrupt
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 0); // Highest priority
    
    // Now, lets go!
    TIM1->CR1 |= TIM_CR1_CEN;
}

void streamerPause()
{
  TIM1->CR1 = 0;
  NVIC_DisableIRQ(DMA1_Channel4_IRQn);
}

void streamerResume()
{
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  TIM1->CR1 |= TIM_CR1_CEN;
}

void streamerStop()
{
  TIM1->CR1 = 0;
  NVIC_DisableIRQ(DMA1_Channel4_IRQn);
}

bool streaming = false;
void streamerBegin()
{
  streamerBufferLogic = 0;
  streamerInit();
  streamerStart();
  streaming = true;
}

void streamerEnd()
{
  streamerStop();
  streamerDeinit();
  streaming = false;
}

bool isStreaming()
{
  return streaming;
}

void streamerSetPrescaler(int psc)
{
    TIM1->PSC = psc - 1;
    TIM1->ARR = psc/2 - 1;
}
