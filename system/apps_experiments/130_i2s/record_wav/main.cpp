/*
  SPH0645
  SEL     - N/C
  LRCL    - P4
  DOUT    - P1
  BCLK    - P3
  GND     - GND
  3V      - 3V
*/

#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "gpio.h"

uint16_t sectorA[BIOS::FAT::SharedBufferSize/2];
uint16_t sectorB[BIOS::FAT::SharedBufferSize/2];
uint16_t* active = sectorA;
volatile uint16_t* ready = nullptr;
int sectorOffset = 0;
const int N = 1024+1024;
uint16_t buffer[N+4];
int pcmOffset = -1;
volatile int overflow = 0;
volatile bool flag1 = false;
volatile bool flag2 = false;
volatile int ofsChange = 0;
uint16_t header[8];
uint16_t header0[8];
volatile int32_t wmin = 0;
volatile int32_t wmax = 0;
uint16_t indicator[200];

// dma table: https://deepbluembedded.com/stm32-dma-tutorial-using-direct-memory-access-dma-in-stm32/

#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

int32_t dc = -8192; //-16384;

void Process(uint16_t* pbuf)
{
  // [aaaaaaaa aaaaaaaa][bb000000 00000000]
  //  aaaaaaaa aaaaaaaa  00000000 00000000  aa << 16
  //  aaaaaaaa aaaaaaaa  bb000000 00000000  (aa << 16) | b
  //  ssssssss ssssssaa  aaaaaaaa aaaaaabb  ((aa << 16) | b)>>14

  // 18 bits
  int n = N/2/4;
  int xmin = 0, xmax = 0, xdc = 0;
//  xmin = xmax = (((pbuf[0]<<16)|pbuf[1])>>14)-dc;
  while (n--)
  {
    int32_t vv1 = (pbuf[0]<<16)|pbuf[1];
    vv1 >>= 14;
    pbuf += 4;
    vv1 -= dc;
    if (vv1 > 32767)
      vv1 = 32767;
    if (vv1 < -32767)
      vv1 = -32767;
    if (xmin==0 && xmax==0)
    {
      xmin = xmax = vv1;
    } else {
      if (vv1 < xmin)
        xmin = vv1;
      if (vv1 > xmax)
        xmax = vv1;
    }
    if (vv1 > 0)
      xdc++;
    if (vv1 < 0)
      xdc--;
    active[sectorOffset++] = vv1;
  }
  wmin = xmin;
  wmax = xmax;
  if (xdc > 0)
    dc++;
  if (xdc < 0)
    dc--;
}

void __irq__ DMA1_Channel4_IRQHandler()
{

    if (DMA1->ISR & DMA_ISR_TEIF4)
    {
      _ASSERT(0);
    }
    else if (DMA1->ISR & DMA_ISR_HTIF4)
    { 
/*
      header[0] = buffer[0];
      header[1] = buffer[1];
      header[2] = buffer[2];
      header[3] = buffer[3];
      header[4] = buffer[4];
      header[5] = buffer[5];
      header[6] = buffer[6];
      header[7] = buffer[7];
 */
      int newOfs = 0;
      if (buffer[0] == 0 && buffer[3] == 0)
        newOfs = 1;
      else if (buffer[0] == 0 && buffer[1] == 0)
        newOfs = 2;
      else
        newOfs = 0;
      if (pcmOffset != newOfs)
      {
        pcmOffset = newOfs;
        ofsChange++;
      }

      Process(buffer+pcmOffset);
      if (sectorOffset == BIOS::FAT::SharedBufferSize/2)
      {
        if (ready)
          overflow++;

        ready = active;
        active = active == sectorA ? sectorB : sectorA;
        sectorOffset = 0;
      }
      DMA1->IFCR = DMA_IFCR_CHTIF4;
      _ASSERT(!(DMA1->ISR & DMA_ISR_TCIF4));
    }
    else if (DMA1->ISR & DMA_ISR_TCIF4)
    { 
      if (pcmOffset == -1)
      {
        DMA1->IFCR = DMA_IFCR_CTCIF4;
        _ASSERT(!(DMA1->ISR & DMA_ISR_HTIF4));
        return;
      }

      buffer[N] = buffer[0];
      buffer[N+1] = buffer[1];
      buffer[N+2] = buffer[2]; 
      Process(buffer+N/2+pcmOffset);
      if (sectorOffset == BIOS::FAT::SharedBufferSize/2)
      {
        if (ready)
          overflow++;

        ready = active;
        active = active == sectorA ? sectorB : sectorA;
        sectorOffset = 0;
      }
      DMA1->IFCR = DMA_IFCR_CTCIF4;
      _ASSERT(!(DMA1->ISR & DMA_ISR_HTIF4));
    }
}


/*
  PB12: WS                I2S2_WS   P4     LRCK             LRC
  PB13: SCK   SPI2_SCK    I2S2_CK   P3     BICK             BCLK
  PB14:       SPI2_MISO             P2
  PB15: SD    SPI2_MOSI   I2S2_SD   P1     SDTI data?       DIK

  PC6 I2S2_MCK
*/


__attribute__((__section__(".entry")))
int main(void)
{
    const int sampleRate = I2S_AudioFreq_32k;
    APP::Init("I2S Recording @ 32kHz");

    Gpio::SetState(Gpio::BASEB, Gpio::P1, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput | Gpio::StateInputFloating);

    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    RCC->APB1ENR |= RCC_APB1Periph_SPI2;
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO;

    I2S_InitTypeDef initStruct;
    I2S_StructInit(&initStruct);

    initStruct.I2S_Mode = I2S_Mode_MasterRx;
    initStruct.I2S_Standard = I2S_Standard_Phillips;
    initStruct.I2S_DataFormat = I2S_DataFormat_24b;
    initStruct.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    initStruct.I2S_AudioFreq = sampleRate; //I2S_AudioFreq_44k; // 8, 11, 16, 22, 32, 44, 48, 96, 192
    initStruct.I2S_CPOL = I2S_CPOL_Low;

    BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel4_IRQ, DMA1_Channel4_IRQHandler);

    I2S_Init(SPI2, &initStruct);
    I2S_Cmd(SPI2, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
        DMA_DeInit(DMA1_Channel4);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uintptr_t)&SPI2->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr = (uintptr_t)buffer;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize = N; //COUNT(buffer);
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);

        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TE, ENABLE);
        DMA_ITConfig(DMA1_Channel4, DMA_IT_HT, ENABLE);
        
        /* Disable SPI3 DMA TX request */
        SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
        DMA_Cmd (DMA1_Channel4, DISABLE);

    // Clear any pending interrupts for ch 4
    DMA1->IFCR = 0x0000F000;
    
    // Enable ch 5 interrupt
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 0); // Highest priority

    BIOS::FAT::Open((char*)"record.wav", BIOS::FAT::IoWrite);

    struct WaveFormat 
    {
      uint16_t wFormatTag{1}; // PCM
      uint16_t nChannels{1};
      uint32_t nSamplesPerSec{sampleRate};
      uint32_t nAvgBytesPerSec{sampleRate*2};
      uint16_t nBlockAlign{2};
      uint16_t wBitsPerSample{16};
      uint16_t cbSize{0};
    } WaveFormat;

    memset(sectorA, 0, sizeof(sectorA));
    CStream((uint8_t*)sectorA, sizeof(sectorA))
      << CStream("RIFF", 4)
      << (uint32_t)-1   // 4+4+sizeof(WaveFormat)+4+size
      << CStream("WAVE", 4)
        << CStream("fmt ", 4)
        << (uint32_t)sizeof(WaveFormat)
        << CStream(&WaveFormat, sizeof(WaveFormat))
        << CStream("data", 4)
        << (uint32_t)-1;

    // save header, pad with zeroes to align to sector size
    BIOS::FAT::Write((uint8_t*)sectorA);

    DMA_Cmd (DMA1_Channel4, ENABLE);
    BIOS::LCD::BufferBegin(CRect(0, 14, BIOS::LCD::Width, 14+128));
    int totalBytes = 0;
    int inhibit = 20;
    while (!BIOS::KEY::GetKey())
    {
      // f8f09 8000 0000 0000
      if (ready)
      {
        uint8_t* save = (uint8_t*)ready;
        ready = nullptr;
        if (inhibit > 0)
          inhibit--;
        else {
          BIOS::FAT::Write(save);
          totalBytes += BIOS::FAT::SharedBufferSize;
        }
      } else {
        EVERY(30)
        {
          int cmin = (wmin+32768)*128/65536;
          int cmax = (wmax+32768)*128/65536;
          if (cmin > 0) cmin--;
          if (cmax < 127) cmax++;
          wmin = 0;
          wmax = 0;
          uint16_t base = ((totalBytes>>16) & 1) ? RGB565(2020b0) : RGB565(2050b0);
          if (inhibit)
            base = RGB565(404080);
          if (overflow)
          {
            overflow = 0;
            base = RGB565(d00000);
          }
          if (ofsChange)
          {
            ofsChange = 0;
            base = RGB565(d000d0);
          }
          for (int i=0; i<128; i++)
            indicator[i] = base;
          for (int i=0; i<128; i+=16)
            indicator[i] = RGB565(202090);
          for (int i=cmin; i<64; i++)
            indicator[i] = RGB565(808000);
          for (int i=64; i<cmax; i++)
            indicator[i] = RGB565(808000);
          for (int i=cmin; i<cmax; i++)
            indicator[i] = RGB565(e0e000);
          BIOS::LCD::BufferWrite(indicator, 128);
        }
      }
    }
    BIOS::LCD::BufferEnd();
    DMA_Cmd (DMA1_Channel4, DISABLE);

    BIOS::FAT::Close(totalBytes);

    I2S_Cmd(SPI2, DISABLE);
    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateInput | Gpio::StateInputFloating);

    // warning: where does it point?
    //BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, pOldSysTick);
    BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel5_IRQ, [](){});

    CONSOLE::Print("\n\nDone. Press any key to exit.");
    while (!BIOS::KEY::GetKey());

    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
