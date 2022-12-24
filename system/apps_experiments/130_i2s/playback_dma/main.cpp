/*
  MAX98357A
  LRC   P4
  BCLK  P3
  DIN   P1
  GAIN  GND
  SD
  GND   GND
  VIN   3V+


*/

#include <library.h>
#include <math.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../../os_host/source/framework/Wnd.cpp"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "gpio.h"

using namespace BIOS;
#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

CFileDialog mFile;
CFileFilterSuffix filter(".WAV");
volatile bool flag1 = false;
volatile bool flag2 = false;
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedReader mReader;
uint16_t txbuf[4096+512];

#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

void __irq__ DMA1_Channel5_IRQHandler()
{
    if (DMA1->ISR & DMA_ISR_TEIF5)
    {
        _ASSERT(0);
    }
    else if (DMA1->ISR & DMA_ISR_HTIF5)
    {
        flag1 = true;
        DMA1->IFCR = DMA_IFCR_CHTIF5;
        if (DMA1->ISR & DMA_ISR_TCIF5)
        {
            _ASSERT(0);
        }
    }
    else if (DMA1->ISR & DMA_ISR_TCIF5)
    {
        flag2 = true;
        DMA1->IFCR = DMA_IFCR_CTCIF5;
        if (DMA1->ISR & DMA_ISR_HTIF5)
        {
            _ASSERT(0);
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
  else if (tmp32 == 20) { toSkip = 20 - 16; }
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

bool PlaySound(char* name)
{
    APP::Init("I2S Wav Player");

    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    CONSOLE::Print("Opening %s...\n", name);
    if (!mReader.Open(name))
        return false;

    int channels = -1;
    int sampleRate = -1;
    int bitsPerSample = -1;
    int availBytes = -1;
    if (!CheckWavHeader(mReader, channels, sampleRate, bitsPerSample, availBytes))
    {
        CONSOLE::Print("Format error!\n");
        BIOS::SYS::DelayMs(2000);
        return false;
    }

    CONSOLE::Print("\nChannels: %d\nSample rate: %d\nBits per sample: %d\n", channels, sampleRate, bitsPerSample);

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
    initStruct.I2S_AudioFreq = sampleRate; // 8, 11, 16, 22, 32, 44, 48, 96, 192
    initStruct.I2S_CPOL = I2S_CPOL_Low;

    BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IDMA1_Channel5_IRQ, DMA1_Channel5_IRQHandler);

    I2S_Init(SPI2, &initStruct);
    I2S_Cmd(SPI2, ENABLE);

    for (int i=0; i<COUNT(txbuf); i+=1)
      txbuf[i] = 0;

    DMA_InitTypeDef DMA_InitStructure;

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
    DMA_Cmd (DMA1_Channel5,ENABLE);

    CONSOLE::Print("\n\n");
    int progress = 0;
    int fileSize = mReader.GetFileSize();

    int16_t sample;
    while (!BIOS::KEY::GetKey() && !mReader.Eof())
    {
      if (!flag1 && !flag2)
      {
        int newProgress = mReader.GetFileOffset()*39/fileSize;
        if (newProgress > progress)
        {
          CONSOLE::Print("\xdb");
          progress++;
        }
      }
      if (flag1)
      {
        flag1 = false;
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

    CONSOLE::Print("\nDone!\n");
    BIOS::SYS::DelayMs(2000);
    return true;
}

__attribute__((__section__(".entry")))
int main(void)
{
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
        APP::Init("I2S Wav Player");

        if (mFile.ModalShow(nullptr, "Load sound", &filter))
        {
            PlaySound(mFile.GetFilename());
        } else
        {
            break;
        }
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
