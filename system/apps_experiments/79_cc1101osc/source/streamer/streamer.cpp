// based on https://github.com/PetteriAimonen/dso-quad-logic.git

#include "streamer.h"
#include <library.h>
#include "stm32f10x.h"
#include "ringbuf.h"
#include "dma.h"

// Note: Uses optimize("O2") to work around this bug:
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=39585
#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

#define ADC_FIFO_SIZE 1024
#define ADC_FIFO_HALFSIZE (ADC_FIFO_SIZE/2)

BIOS::OS::TInterruptHandler oldDMA1_Channel4_IRQ = nullptr;

volatile uint16_t adc_fifo[ADC_FIFO_SIZE];
volatile int streamerOverrun = 0;

bool streamerBufferLogic = 0;
int streamerBufferCounter = 0;
int streamerBufferMaxCounter = 20000; // 1 second @ 50 kHz
const int streamerPeriodUs = 40;
int totalSamples = 0;
RingBufCPP<uint16_t, 256> streamerBuffer;

#define GetLogic(idr) (((idr) >> 9) & 1) // B9

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
        if (streamerBuffer.isFull())
          streamerOverrun++;
        else
          streamerBuffer.push(streamerBufferCounter);

        streamerBufferCounter = 0;

        if (streamerBuffer.isFull())
          streamerOverrun++;
        else
          streamerBuffer.push(0);
      }
    } else
    {
      // append ticks to buffer and toggle logic
      if (streamerBuffer.isFull())
        streamerOverrun++;
      else
        streamerBuffer.push(streamerBufferCounter);
      streamerBufferCounter = 0;
      streamerBufferLogic = 1 - streamerBufferLogic;
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
