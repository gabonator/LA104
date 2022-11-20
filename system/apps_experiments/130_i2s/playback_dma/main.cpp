#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "gpio.h"
#include <math.h>

volatile bool flag1 = false;
volatile bool flag2 = false;


#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

void crash_with_message(const char *message, void *caller)
{
  BIOS::DBG::Print("Error: %s", message);
  while (1);
}


void __irq__ DMA1_Channel5_IRQHandler()
{
    if (DMA1->ISR & DMA_ISR_TEIF5)
    {
        crash_with_message("Oh noes: DMA channel 4 transfer error!",
            __builtin_return_address(0)
        );
        while(1);
    }
    else if (DMA1->ISR & DMA_ISR_HTIF5)
    {
//        process_samples(&adc_fifo[0]);
        flag1 = true;
        DMA1->IFCR = DMA_IFCR_CHTIF5;
        if (DMA1->ISR & DMA_ISR_TCIF5)
        {
            crash_with_message("Oh noes: ADC fifo overflow in HTIF", __builtin_return_address(0));
            while(1);
        }
    }
    else if (DMA1->ISR & DMA_ISR_TCIF5)
    {
//        process_samples(&adc_fifo[ADC_FIFO_HALFSIZE]);
        flag2 = true;
        DMA1->IFCR = DMA_IFCR_CTCIF5;
        if (DMA1->ISR & DMA_ISR_HTIF5)
        {
            crash_with_message("Oh noes: ADC fifo overflow in TCIF", __builtin_return_address(0));
            while(1);
        }
    }
}


/*
  PB12: WS                I2S2_WS   P4     LRCK             LRC
  PB13: SCK   SPI2_SCK    I2S2_CK   P3     BICK             BCLK
  PB14:       SPI2_MISO             P2
  PB15: SD    SPI2_MOSI   I2S2_SD   P1     SDTI data?       DIK

  PC6 I2S2_MCK
*/

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
//uint16_t wave[1024];
CBufferedReader mReader;

uint16_t txbuf[4096+512];

uint16_t rev(uint16_t v)
{
  return (v >> 8) | (v << 8);
}

uint32_t rev(uint32_t v)
{
  return ((v & 0xff) << 24) | (((v >> 8) & 0xff) << 16) | 
    (((v >> 16) & 0xff) << 8) | (((v >> 24) & 0xff) << 0);
}

bool CheckWavHeader(CBufferedReader& r, int& channels, int& sampleRate, int& bitsPerSample, int& availBytes)
{
  char tmp[5] = {0};
  uint32_t tmp32;
  uint16_t tmp16;

  r >> CStream(tmp, 4);
  if (strcmp(tmp, "RIFF") != 0)
    return false;
  // skip chunk size
  r >> tmp32;
  r >> CStream(tmp, 4);
  if (strcmp(tmp, "WAVE") != 0)
    return false;

  while (!r.Eof())
  {
    r >> CStream(tmp, 4);
    if (strcmp(tmp, "fmt ") == 0)
      break;
  }

  // sub chunk size
  r >> tmp32;

  int toSkip = 0;
  if (tmp32 == 16) { toSkip = 0; }
  else if (tmp32 == 18) { toSkip = 18 - 16; }
  else if (tmp32 == 40) { toSkip = 40 - 16; }
  else
    return false;

  // audio format
  r >> tmp16;
  if (tmp16 != 1)
    return false;

  // channels
  r >> tmp16;
  if (tmp16 != 1 && tmp16 != 2)
    return false;

  channels = tmp16;

  // sample rate
  r >> tmp32;
  sampleRate = tmp32;
  if (sampleRate < 1)
    return false;

  // byte rate
  r >> tmp32;
  // block align
  r >> tmp16;
  // bits per sample
  r >> tmp16;
  if (tmp16 != 8 && tmp16 != 16)
    return false;
  bitsPerSample = tmp16;
  r.Seek(r.GetFileOffset() + toSkip);

  while (!r.Eof())
  {
    r >> CStream(tmp, 4);
    if (strcmp(tmp, "data") == 0)
      break;
    r >> tmp32;
    r.Seek(r.GetFileOffset() + tmp32);
  }
  if (r.Eof())
    return false;
  r >> tmp32;
  availBytes = tmp32;
  return true;
} 

__attribute__((__section__(".entry")))
int main(void)
{
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    if (!mReader.Open((char*)"test.wav"))
        return 1;

    int channels = 7;
    int sampleRate = 7;
    int bitsPerSample = 7;
    int availBytes = 7;
    if (!CheckWavHeader(mReader, channels, sampleRate, bitsPerSample, availBytes))
    {
        BIOS::SYS::DelayMs(2000);
        return 2;
    }

    APP::Init("I2S Test");
    CONSOLE::Print("ch=%d, rate=%d, bps=%d, avail=%d\n", channels, sampleRate, bitsPerSample, availBytes);

    Gpio::SetState(Gpio::BASEB, Gpio::P1, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput | Gpio::StateInputFloating);

    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    RCC->APB1ENR |= RCC_APB1Periph_SPI2;
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO;

    I2S_InitTypeDef initStruct;
    I2S_StructInit(&initStruct);

    initStruct.I2S_Mode = I2S_Mode_MasterTx;
    initStruct.I2S_Standard = I2S_Standard_Phillips;
    initStruct.I2S_DataFormat = I2S_DataFormat_16b;
    initStruct.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    initStruct.I2S_AudioFreq = I2S_AudioFreq_44k; // 8, 11, 16, 22, 32, 44, 48, 96, 192
    initStruct.I2S_CPOL = I2S_CPOL_Low;

    BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel5_IRQ, DMA1_Channel5_IRQHandler);

    I2S_Init(SPI2, &initStruct);
    I2S_Cmd(SPI2, ENABLE);

    for (int i=0; i<COUNT(txbuf); i+=1)
      txbuf[i] = 0;

    DMA_InitTypeDef DMA_InitStructure;
#define SPI3_DR_Addr        ((u32)0x40003C0C)
_ASSERT(SPI3_DR_Addr == (uintptr_t)&SPI3->DR);

        DMA_DeInit(DMA1_Channel5);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uintptr_t)&SPI2->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr = (uintptr_t)txbuf;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize = COUNT(txbuf);
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);

        DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
        DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);
        DMA_ITConfig(DMA1_Channel5, DMA_IT_HT, ENABLE);
        
        /* Disable SPI3 DMA TX request */
        SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
        DMA_Cmd (DMA1_Channel5,DISABLE);

    // Clear any pending interrupts for ch 4
    DMA1->IFCR = 0x000F0000;
    
    // Enable ch 5 interrupt
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    NVIC_SetPriority(DMA1_Channel5_IRQn, 0); // Highest priority


//        DMA1_Channel5->CNDTR = COUNT(txbuf);
        DMA_Cmd (DMA1_Channel5,ENABLE);

    int16_t sample;
    while (!BIOS::KEY::GetKey() && !mReader.Eof())
    {
      if (flag1)
      {
        flag1 = false;
//        CONSOLE::Print("1");
        for (int i=0; i<COUNT(txbuf)/2; i+=2)
        {
          mReader >> sample;
          sample /= 4;
          txbuf[i] = sample;
          txbuf[i+1] = sample;
        }
      }
      if (flag2)
      {
        flag2 = false;
        for (int i=COUNT(txbuf)/2; i<COUNT(txbuf); i+=2)
        {
          mReader >> sample;
          sample /= 4;
          txbuf[i] = sample;
          txbuf[i+1] = sample;
        }
//        CONSOLE::Print("2");
      }
    }

    mReader.Close();
    DMA_Cmd (DMA1_Channel5, DISABLE);
    I2S_Cmd(SPI2, DISABLE);
    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateInput | Gpio::StateInputFloating);

    // warning: where does it point?
    BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel5_IRQ, [](){});

    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
