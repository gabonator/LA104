/********************* (C) COPYRIGHT 2017 e-Design Co.,Ltd. ********************
  DS213_APP Fpga.h                                               Author : bure
*******************************************************************************/
#ifndef __FPGADRV_H
#define __FPGADRV_H


//=================== FPGA ===================

  #define A_C_CH         0
  #define B_D_CH         1

  //FPGA
  #define CHIP_RST       0x80


  //FPGA
  #define SMPL_Din       0x01
  #define PSMPL          0x02
  #define TRIGG          0x04
  #define FULL           0x08
  #define EMPTY          0x10
  #define TRIGG_ASK      0x20
  #define SCRNF          0x40

  //
  #define SMPL_MODE      0x40
  #define SMPL_PSCL      0x41
  #define SMPL_TDIV      0x42
  #define SMPL_PSMP      0x43
  #define SMPL_RPTR      0x45
  #define SMPL_RD        0x46
  #define SMPL_ST        0x47

  //
  #define SEPARATE       0x00
  #define INTERLACE      0x01

  //
  #define TRIG_KIND      0x20
  #define TRIG_VOLT      0x21
  #define TRIG_WDTH      0x22

  //
  #define VMIN           0x30
  #define VMAX           0x31
  #define VAVG           0x32
  #define ATWL           0x34
  #define ATWH           0x35
  #define DTWL           0x36
  #define DTWH           0x37

  //
  #define TRIG_NONE      0x00
  #define TRIG_ANY       0x01

  #define TRIG_AiDN      0x10
  #define TRIG_AiUP      0x11
  #define TRIG_AiLT      0x12
  #define TRIG_AiGT      0x13
  #define TRIG_AnLT      0x14
  #define TRIG_AnGT      0x15
  #define TRIG_ApLT      0x16
  #define TRIG_ApGT      0x17

  #define TRIG_DiDN      0x18
  #define TRIG_DiUP      0x19
  #define TRIG_DiLT      0x1A
  #define TRIG_DiGT      0x1B
  #define TRIG_DnLT      0x1C
  #define TRIG_DnGT      0x1D
  #define TRIG_DpLT      0x1E
  #define TRIG_DpGT      0x1F

#endif

extern HwDrvDef  Hw;

uint8_t  FPGA_DataWr(uint8_t Dev, uint8_t Cmd, uint16_t Data)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = (u8)Cmd;                      // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd = SpiX->DR;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = (u8)Data;                     // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd |= (SpiX->DR)<< 16;                  // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = Data >> 8;                    // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd |= (SpiX->DR)<< 16;                  // 
  *nSS = 1;
  return Cmd;
}
/*******************************************************************************
 
*******************************************************************************/
uint8_t  FPGA_ByteWr(uint8_t Dev, uint8_t Cmd, uint8_t Byte)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = (u8)Cmd;                      // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd = SpiX->DR;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = Byte;                         // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Cmd |= (SpiX->DR)<< 16;                  // 
  *nSS = 1;
  return Cmd;
}
/*******************************************************************************
 
*******************************************************************************/
uint8_t  FPGA_CtrlRW(uint8_t Dev, uint8_t Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  uint8_t Info = SpiX->DR;                      // 
  *nSS = 1;
  return Info;
}
/*******************************************************************************
  bit0: Dgtl Smpl Bit,  bit1~5: Info, bit8~15: Anlg Smpl Byte
*******************************************************************************/
uint16_t FPGA_SmplRd(uint8_t Dev)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = SMPL_RD;                      // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  uint16_t Data = (u8)(SpiX->DR) ;              // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Data |= (u8)(SpiX->DR) << 8;             // 
  *nSS = 1;
  return Data;
}
/*******************************************************************************
  bit7~0: Byte
*******************************************************************************/
uint8_t FPGA_ByteRd(uint8_t Dev, uint8_t Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  volatile uint8_t Info = SpiX->DR;                     // 
  (void)Info;
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  uint8_t Byte = (SpiX->DR);                    // 
  *nSS = 1;
  return Byte;
}
/*******************************************************************************
  bit15~0: Data
*******************************************************************************/
uint16_t FPGA_DataRd(uint8_t Dev, uint8_t Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  volatile uint8_t Info = SpiX->DR;                     // 
  (void)Info;
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  uint16_t Data = (SpiX->DR) & 0xFF;            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Data |= (u16)((SpiX->DR)<< 8);           // 
  *nSS = 1;
  return Data;
}
/*******************************************************************************
  bit31~0: Word
*******************************************************************************/
u32 FPGA_WordRd(uint8_t Dev, uint8_t Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  volatile uint8_t Info = SpiX->DR;                     // 
  (void)Info;
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  u32 Word = (u8)(SpiX->DR);               // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Word |= ((u8)(SpiX->DR)) << 8;           // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Word |= ((u8)(SpiX->DR)) << 16;          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Word |= ((u8)(SpiX->DR)) << 24;          // 
  *nSS = 1;
  return Word;
}
