  #include "DS213HwDriver.h"

  namespace LOW
  {

HwDrvDef  Hw;

#define __IO

typedef struct
{
  volatile __IO uint16_t CR1;
  volatile uint16_t  RESERVED0;
  volatile __IO uint16_t CR2;
  volatile uint16_t  RESERVED1;
  volatile __IO uint16_t SR;
  volatile uint16_t  RESERVED2;
  volatile __IO uint16_t DR;
  volatile uint16_t  RESERVED3;
  volatile __IO uint16_t CRCPR;
  volatile uint16_t  RESERVED4;
  volatile __IO uint16_t RXCRCR;
  volatile uint16_t  RESERVED5;
  volatile __IO uint16_t TXCRCR;
  volatile uint16_t  RESERVED6;
  volatile __IO uint16_t I2SCFGR;
  volatile uint16_t  RESERVED7;
  volatile __IO uint16_t I2SPR;
  volatile uint16_t  RESERVED8;  
} SPI_TypeDef;

#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)
#define I2S_FLAG_CHSIDE                 ((uint16_t)0x0004)
#define I2S_FLAG_UDR                    ((uint16_t)0x0008)
#define SPI_FLAG_CRCERR                 ((uint16_t)0x0010)
#define SPI_FLAG_MODF                   ((uint16_t)0x0020)
#define SPI_I2S_FLAG_OVR                ((uint16_t)0x0040)
#define SPI_I2S_FLAG_BSY                ((uint16_t)0x0080)

uint8_t  FPGA_ByteWr(uint8_t Dev, uint8_t Cmd, uint8_t Byte)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  volatile uint8_t* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = (uint8_t)Cmd;                      // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd = SpiX->DR;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = Byte;                         // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd |= (SpiX->DR)<< 16;                  // 
  *nSS = 1;
  return Cmd;
}

    uint32_t SetParam(int attribute, int value)
    {
        static bool init = true;
        if (init)
        {
            pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
            __BiosInit((uint32_t)&Hw);
            init = false;
        }
// spi = 0x040003800, ss=0x0422181b0
        BIOS::DBG::Print("atr[%d]=%d;", attribute, value);
        return FPGA_ByteWr(0, attribute, value);
    }
  }

