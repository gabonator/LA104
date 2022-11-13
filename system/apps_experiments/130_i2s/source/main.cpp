#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "gpio.h"

/*
  PB12: WS                I2S2_WS   P4     LRCK
  PB13: SCK   SPI2_SCK    I2S2_CK   P3     BICK
  PB14:       SPI2_MISO             P2
  PB15: SD    SPI2_MOSI   I2S2_SD   P1     SDTI data?

  PC6 I2S2_MCK
*/

__attribute__((__section__(".entry")))
int main(void)
{
    APP::Init("I2S Test");

    Gpio::SetState(Gpio::BASEB, Gpio::P1, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput | Gpio::StateInputFloating);

    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateOutput10Mhz | Gpio::StateOutputFunctionPushPull);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);

RCC->APB1ENR |= RCC_APB1Periph_SPI2;
RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

    I2S_InitTypeDef initStruct;
    I2S_StructInit(&initStruct);

    initStruct.I2S_Mode = I2S_Mode_MasterRx;
    initStruct.I2S_Standard = I2S_Standard_Phillips;
    initStruct.I2S_DataFormat = I2S_DataFormat_24b;
    initStruct.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    initStruct.I2S_AudioFreq = I2S_AudioFreq_Default; //I2S_AudioFreq_48k;
    initStruct.I2S_CPOL = I2S_CPOL_Low;

    I2S_Init(SPI2, &initStruct);
    I2S_Cmd(SPI2, ENABLE);
    int data = 0;
    uint8_t c = 0;
    while (!BIOS::KEY::GetKey())
    {
//      for (int i=0; i<1000; i++)
//      {
/*
        for (volatile int j=0; j<1000; j++)
          if (SPI2->SR & SPI_I2S_FLAG_TXE)
            break; 
        SPI2->DR = 0;
//        while (!(SPI2->SR & SPI_I2S_FLAG_RXNE));
        for (volatile int j=0; j<1000; j++)
          if (SPI2->SR & SPI_I2S_FLAG_RXNE)
            break; 
*/
//        SPI2->DR = 0;
//        data = SPI2->DR;
//      }

        for (volatile int j=0; j<1000; j++)
          if (SPI2->SR & SPI_I2S_FLAG_RXNE)
            break; 
        data = SPI2->DR;
      if (data)
      BIOS::LCD::Printf(0, 32, RGB565(ffffff), RGB565(000000), "%02x %02x %d   ", c++, SPI3->SR & 0xff, data);
    }

    I2S_Cmd(SPI2, DISABLE);
    Gpio::SetState(Gpio::BASEB, Gpio::P1MOSI, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P2MISO, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P3SCK2, Gpio::StateInput | Gpio::StateInputFloating);
    Gpio::SetState(Gpio::BASEB, Gpio::P4CS, Gpio::StateInput | Gpio::StateInputFloating);

    return 0;
}
