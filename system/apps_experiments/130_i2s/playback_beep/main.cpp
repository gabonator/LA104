#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
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

uint16_t wave[1024];

__attribute__((__section__(".entry")))
int main(void)
{
   for (int i=0; i<1024; i++)
     wave[i] = 0x8000+sin(i*M_PI*2/1024)*0x7fff;

    APP::Init("I2S Test");

    Gpio::SetState(Gpio::BASEB, Gpio::P1, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput | Gpio::StateInputFloating);

    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);

    //RCC->APB1ENR |= RCC_APB1Periph_SPI2;
    //RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

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
    uint16_t phase = 0;
    int clk = 0, add = 5, dir = 1;
    while (!BIOS::KEY::GetKey())
    {
      if (clk++ == 1000)
      {
         clk = 0;
         add += dir;
         if (add > 50)
           dir = -1;
         if (add < 2)
           dir = 1;
      }
      phase += add;

      // left channel
      SPI2->DR = wave[phase&1023]>>4;
      while (!(SPI2->SR & SPI_I2S_FLAG_TXE));

      // right channel
      SPI2->DR = 0;
      while (!(SPI2->SR & SPI_I2S_FLAG_TXE));
    }

    I2S_Cmd(SPI2, DISABLE);
    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateInput | Gpio::StateInputFloating);

    return 0;
}
