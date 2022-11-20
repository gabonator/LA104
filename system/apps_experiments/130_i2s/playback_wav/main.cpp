#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "gpio.h"
#include <math.h>

/*
  PB12: WS                I2S2_WS   P4     LRCK             LRC
  PB13: SCK   SPI2_SCK    I2S2_CK   P3     BICK             BCLK
  PB14:       SPI2_MISO             P2
  PB15: SD    SPI2_MOSI   I2S2_SD   P1     SDTI data?       DIK

  PC6 I2S2_MCK
*/

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
uint16_t wave[1024];
CBufferedReader mReader;

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

    RCC->APB1ENR |= RCC_APB1Periph_SPI2;
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

    I2S_InitTypeDef initStruct;
    I2S_StructInit(&initStruct);

    initStruct.I2S_Mode = I2S_Mode_MasterTx;
    initStruct.I2S_Standard = I2S_Standard_Phillips;
    initStruct.I2S_DataFormat = I2S_DataFormat_32b;
    initStruct.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    initStruct.I2S_AudioFreq = I2S_AudioFreq_44k; // 8, 11, 16, 22, 32, 44, 48, 96, 192
    initStruct.I2S_CPOL = I2S_CPOL_Low;

    I2S_Init(SPI2, &initStruct);
    I2S_Cmd(SPI2, ENABLE);
    uint16_t data16;
    uint8_t data8;
    while (!BIOS::KEY::GetKey() && availBytes > 0)
    {
      availBytes -= bitsPerSample*channels/8;
      if (bitsPerSample == 16)
      {
        mReader >> data16;
        // left channel
        SPI2->DR = data16;
        while (!(SPI2->SR & SPI_I2S_FLAG_TXE));

        if (channels == 2)
          mReader >> data16;
        // right channel
        SPI2->DR = data16;
        while (!(SPI2->SR & SPI_I2S_FLAG_TXE));
      } else {
        mReader >> data8;
        // left channel
        SPI2->DR = data8<<8;
        while (!(SPI2->SR & SPI_I2S_FLAG_TXE));

        if (channels == 2)
          mReader >> data8;
        // right channel
        SPI2->DR = data8<<8;
        while (!(SPI2->SR & SPI_I2S_FLAG_TXE));
      }
    }
    mReader.Close();
    I2S_Cmd(SPI2, DISABLE);
    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateInput | Gpio::StateInputFloating);

    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
