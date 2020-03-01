// based on https://github.com/PetteriAimonen/dso-quad-logic.git

#include "streamer.h"
#include <library.h>
#include "stm32f10x.h"
#include "ringbuf.h"

#define ADC_FIFO_SIZE 256
#define ADC_FIFO_HALFSIZE (ADC_FIFO_SIZE/2)


void crash_with_message(const char *message, void *caller)
{
  BIOS::DBG::Print("Error: %s", message);
  while (1);
}

// Note: Uses optimize("O2") to work around this bug:
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=39585
#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

// For some reason, the headers don't have these registers
#define FSMC_BCR1   (*((vu32 *)(0xA0000000+0x00)))
#define FSMC_BTR1   (*((vu32 *)(0xA0000000+0x04)))
#define FSMC_BCR2   (*((vu32 *)(0xA0000008+0x00)))
#define FSMC_BTR2   (*((vu32 *)(0xA0000008+0x04)))

// GPIOC->BSRR values to toggle GPIOC5
static const uint32_t hl_set[2] = {1 << (16 + 5), 1 << 5};

// FIFO for stuff that is coming from the DMA.

static uint32_t adc_fifo[ADC_FIFO_SIZE];

RingBufCPP<uint8_t, 2048+256> streamerBuffer;

volatile int streamerOverrun = 0;

void process_samples(const uint32_t *data)
{
  const uint32_t *end = data + ADC_FIFO_HALFSIZE;
  while (data < end)
  {   
    int sample = *data++ & 0xff;
    if (streamerBuffer.isFull())
      streamerOverrun++;
    else
      streamerBuffer.push(sample);
  }
}

void __irq__ DMA1_Channel4_IRQHandler()
{
    if (DMA1->ISR & DMA_ISR_TEIF4)
    {
        crash_with_message("Oh noes: DMA channel 4 transfer error!",
            __builtin_return_address(0)
        );
        while(1);
    }
    else if (DMA1->ISR & DMA_ISR_HTIF4)
    {
        process_samples(&adc_fifo[0]);
        DMA1->IFCR = DMA_IFCR_CHTIF4;
        if (DMA1->ISR & DMA_ISR_TCIF4)
        {
            crash_with_message("Oh noes: ADC fifo overflow in HTIF", __builtin_return_address(0));
            while(1);
        }
    }
    else if (DMA1->ISR & DMA_ISR_TCIF4)
    {
        process_samples(&adc_fifo[ADC_FIFO_HALFSIZE]);
        DMA1->IFCR = DMA_IFCR_CTCIF4;
        if (DMA1->ISR & DMA_ISR_HTIF4)
        {
            crash_with_message("Oh noes: ADC fifo overflow in TCIF", __builtin_return_address(0));
            while(1);
        }
    }
}

void streamerInit()
{
  BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel4_IRQ, DMA1_Channel4_IRQHandler);
}

void streamerDeinit()
{
  // reset DMA handler
  BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel4_IRQ, [](){});
}

void streamerStart()
{
    // Samplerate is 500kHz, two TMR1 cycles per sample -> PSC = 12 -1, ARR = 6 - 1
    // Channel 2: Trigger DMA Ch3 to write H_L bit
    // Channel 4: Trigger DMA Ch4 to read data to memory
    //
    // TMR cycle:    0  1  2  3  4  5  0  1  2  3  4  5 0
    // MCO output:  _|^^^^^^^^^^^^^^^^^|________________|^
    // H_L:         _|^^^^^^^^^^^^^^^^^|________________|^
    // DMA sample:         ^ read ch A&B     ^ read ch C&D
    TIM1->CR1 = 0; // Turn off TIM1 until we are ready
    TIM1->CR2 = 0;
    TIM1->CNT = 0;
    TIM1->SR = 0;
    TIM1->PSC = 12*10 - 1; // 50kHz
    TIM1->ARR = 6*10 - 1;
//    TIM1->PSC = 12*5 - 1; // 100kHz
//    TIM1->ARR = 6*5 - 1;
//    TIM1->PSC = 12 - 1; // 500kHz
//    TIM1->ARR = 6 - 1;
    TIM1->CCMR1 = 0x0000; // CC2 time base
    TIM1->CCMR2 = 0x0000; // CC4 time base
    TIM1->DIER = TIM_DIER_CC2DE | TIM_DIER_CC4DE;
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR4 = 2;
        
    // DMA1 channel 3: copy data from hl_set to GPIOC->BSRR
    // Priority: very high
    // MSIZE = PSIZE = 32 bits
    // MINC enabled, CIRC mode enabled
    // Direction: read from memory
    // No interrupts
    DMA1_Channel3->CCR = 0;
    DMA1_Channel3->CNDTR = 2;
    DMA1_Channel3->CPAR = (uint32_t)&GPIOC->BSRR;
    DMA1_Channel3->CMAR = (uint32_t)hl_set;
    DMA1_Channel3->CCR = 0x3AB1;
    GPIOC->BSRR = hl_set[1];
    
    // DMA1 channel 4: copy data from FPGA to adc_fifo.
    // Priority: very high
    // MSIZE = PSIZE = 16 bits
    // MINC enabled, CIRC mode enabled
    // Direction: read from peripheral
    // Half- and Full-transfer interrupts, plus error interrupt
    DMA1_Channel4->CCR = 0;
    DMA1_Channel4->CNDTR = sizeof(adc_fifo) / 2;
    DMA1_Channel4->CPAR = 0x64000000; // FPGA memory-mapped address
    DMA1_Channel4->CMAR = (uint32_t)adc_fifo;
    DMA1_Channel4->CCR = 0x35AF;
    
    // Reduce the wait states of the FPGA & LCD interface
    FSMC_BTR1 = 0x10100110;
    FSMC_BTR2 = 0x10100110;
    FSMC_BCR1 |= FSMC_BCR1_CBURSTRW;
    
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

void adcConfigure()
{
  BIOS::ADC::Enable(false);
  BIOS::ADC::Enable(true);
//  BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH1, BIOS::ADC::ECouple::DC, BIOS::ADC::EResolution::_50mV, 128);
//  BIOS::ADC::ConfigureInput( BIOS::ADC::EInput::CH2, BIOS::ADC::ECouple::DC, BIOS::ADC::EResolution::_50mV, 128);
  BIOS::ADC::ConfigureTimebase(500e-9f);
  BIOS::ADC::ConfigureTrigger(0, 128, BIOS::ADC::ETriggerType::None, BIOS::ADC::EInput::CH1);
//  BIOS::ADC::Restart();

  #define BiosBase(addr) (void*)(0x08004200+addr)
  u32 (*__Set_Param)(uint8_t, uint8_t) = (u32 (*)(uint8_t, uint8_t)) BiosBase(0x51);

  #define FPGA_SP_PERCNT_L     8 // Presample count, bits 0..7
  #define FPGA_SP_PERCNT_H     9 // Presample count, bits 8..11
  __Set_Param(FPGA_SP_PERCNT_L, 0); // TODO!!!!
  __Set_Param(FPGA_SP_PERCNT_H, 0);

  BIOS::ADC::Get();
  BIOS::ADC::Get();
  BIOS::SYS::DelayMs(500);
}

void adcReset()
{
  BIOS::ADC::Enable(false);
  BIOS::ADC::Enable(true);
  #define BiosBase(addr) (void*)(0x08004200+addr)
  u32 (*__Set_Param)(uint8_t, uint8_t) = (u32 (*)(uint8_t, uint8_t)) BiosBase(0x51);

  #define FPGA_SP_PERCNT_L     8 // Presample count, bits 0..7
  #define FPGA_SP_PERCNT_H     9 // Presample count, bits 8..11
  __Set_Param(FPGA_SP_PERCNT_L, 120); // TODO!!!!
  __Set_Param(FPGA_SP_PERCNT_H, 0);
}


bool streaming = false;
void streamerBegin()
{
  adcConfigure();
  streamerInit();
  streamerStart();
  streaming = true;
}

void streamerEnd()
{
  streamerStop();
  streamerDeinit();
  adcReset();
  streaming = false;
}

bool isStreaming()
{
  return streaming;
}

/*

    __Set(ADC_CTRL, EN);       
    __Set(ADC_MODE, SEPARATE);               

    __Set(CH_A_COUPLE, DC);
    __Set(CH_A_RANGE, ADC_500mV);
    
    __Set(CH_B_COUPLE, DC);
    __Set(CH_B_RANGE, ADC_500mV);
    
    __Set(TRIGG_MODE, UNCONDITION);
    __Set(T_BASE_PSC, 0);
    __Set(T_BASE_ARR, 1); // MCO as sysclock/2
    __Set(CH_A_OFFSET, 0);
    __Set(CH_B_OFFSET, 0);
    __Set_Param(FPGA_SP_PERCNT_L, 0);
    __Set_Param(FPGA_SP_PERCNT_H, 0);
    
    __Read_FIFO();
    __Read_FIFO();
    
    while (~__Get(KEY_STATUS) & ALL_KEYS);
    get_keys(ANY_KEY);
    delay_ms(500); // Wait for ADC to settle
*/