
char  T_BASE[][8] = { " 1S  ",  "500mS",  "200mS",  "100mS",  "50mS ",  "20mS ",
                     "10mS ",  " 5mS ",  " 2mS ",  " 1mS ",  "500uS",  "200uS",
                     "100uS",  "50uS ",  "20uS ",  "10uS ",  " 5uS ",  " 2uS ",
                     " 1uS ",  "500nS",  "200nS",  "100nS",                   };
//-----------------+---------+---------+---------+---------+---------+---------+
uc16 BASE_PSC[]  = {  2000-1,   2000-1,    800-1,    400-1,    200-1,     80-1, 
                        40-1,     20-1,     10-1,      5-1,      2-1,      1-1,
                         1-1,      1-1,      1-1,      1-1,      1-1,      1-1,
                         1-1,      1-1,      1-1,      1-1,                   };
//-----------------+---------+---------+---------+---------+---------+---------+
uc16 BASE_ARR[]  = {  1667-1,    833-1,    833-1,    833-1,    833-1,    833-1, 
                       833-1,    833-1,    667-1,    667-1,    833-1,    667-1,
                       333-1,    167-1,     67-1,     34-1,     17-1,      7-1,
                         4-1,      2-1,      2-1,      1-1,                   };

  #define CH_A       0
  #define CH_B       1
  #define CH_C       2
  #define CH_D       3


u8  FPGA_ByteWr(u8 Dev, u8 Cmd, u8 Byte)
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

u8  FPGA_DataWr(u8 Dev, u8 Cmd, u16 Data)
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

u8  FPGA_CtrlRW(u8 Dev, u8 Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  u8 Info = SpiX->DR;                      // 
  *nSS = 1;
  return Info;
}
/*******************************************************************************
  bit0: Dgtl Smpl Bit,  bit1~5: Info, bit8~15: Anlg Smpl Byte
*******************************************************************************/
u16 FPGA_SmplRd(u8 Dev)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = SMPL_RD;                      // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  u16 Data = (u8)(SpiX->DR) ;              // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  Data |= (u8)(SpiX->DR) << 8;             // 
  *nSS = 1;
  return Data;
}


void SetPreSmplDpth(u16 PsDpth)
{
  FPGA_DataWr(A_C_CH, SMPL_PSMP, PsDpth);
  FPGA_DataWr(B_D_CH, SMPL_PSMP, PsDpth);
}

void SetBase(u8 Base)
{
  FPGA_DataWr(A_C_CH, SMPL_PSCL, BASE_PSC[Base]);
  FPGA_DataWr(B_D_CH, SMPL_PSCL, BASE_PSC[Base]);
  FPGA_DataWr(A_C_CH, SMPL_TDIV, BASE_ARR[Base]);
  FPGA_DataWr(B_D_CH, SMPL_TDIV, BASE_ARR[Base]);
}

uc8 RANGE_SW[][10]= {{ 1,    1,    1,    1,    1,    0,    0,    0,    0,    0,}, 
                     { 0,    0,    1,    1,    0,    0,    0,    0,    1,    0,}, 
                     { 0,    1,    1,    1,    0,    0,    1,    1,    1,    0,}, 
                     { 1,    1,    1,    0,    0,    1,    1,    0,    0,    0,}};

void SetRangeA(u8 Range)
{ 
  *Hw.pOut_A_Level  = RANGE_SW[0][Range];
  *Hw.pOut_A_Slct0  = RANGE_SW[1][Range];
  *Hw.pOut_A_Slct1  = RANGE_SW[2][Range];
  *Hw.pOut_A_Slct2  = RANGE_SW[3][Range];
  *Hw.pOut_A_Slct_B = 0;
}

void SmplStart(void)
{ 
  FPGA_DataWr(A_C_CH, SMPL_MODE, SEPARATE);  // 
  FPGA_DataWr(B_D_CH, SMPL_MODE, SEPARATE);
//  SetPreSmplDpth(20);          // Ԥ
//  SetBase(9);                    // 
//  FPGA_ByteWr(A_C_CH, TRIG_VOLT, 100);  // 
//  FPGA_ByteWr(B_D_CH, TRIG_VOLT, 100);

  FPGA_ByteWr(A_C_CH, TRIG_KIND, TRIG_ANY);
  FPGA_ByteWr(B_D_CH, TRIG_KIND, TRIG_ANY);
//  FPGA_ByteWr(A_C_CH, TRIG_KIND, TRIG_AiDN);
//  FPGA_ByteWr(B_D_CH, TRIG_KIND, TRIG_AiDN);
  
  *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;        // FPGA 
}
