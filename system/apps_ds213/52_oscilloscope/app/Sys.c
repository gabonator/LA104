/**********************(C) COPYRIGHT 2018 e-Design Co.,Ltd.*********************
  DS213_APP Sys.c                                                Author : bure
*******************************************************************************/
#include "Sys.h"
#include "DS213Bios.h"

void SysInt(void);

//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
//
//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
uc8 RANGE_SW[][10]= {{ 1,    1,    1,    1,    1,    0,    0,    0,    0,    0,}, 
                     { 0,    0,    1,    1,    0,    0,    0,    0,    1,    0,}, 
                     { 0,    1,    1,    1,    0,    0,    1,    1,    1,    0,}, 
                     { 1,    1,    1,    0,    0,    1,    1,    0,    0,    0,}};
//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+

uc16 WF100[][100] = {{0x7FF, 0x87F, 0x8FF, 0x97E, 0x9FC, 0xA77, 0xAF0, 0xB66, 
                      0xBD9, 0xC48, 0xCB2, 0xD18, 0xD78, 0xDD3, 0xE29, 0xE77, 
                      0xEC0, 0xF01, 0xF3C, 0xF6F, 0xF9A, 0xFBE, 0xFDA, 0xFEE, 
                      0xFFA, 0xFFE, 0xFFA, 0xFEE, 0xFDA, 0xFBE, 0xF9A, 0xF6F, 
                      0xF3C, 0xF01, 0xEC0, 0xE77, 0xE29, 0xDD3, 0xD78, 0xD18, 
                      0xCB2, 0xC48, 0xBD9, 0xB66, 0xAF0, 0xA77, 0x9FC, 0x97E, 
                      0x8FF, 0x87F, 0x7FE, 0x77E, 0x6FE, 0x67F, 0x601, 0x586, 
                      0x50D, 0x496, 0x424, 0x3B5, 0x34B, 0x2E5, 0x285, 0x22A, 
                      0x1D4, 0x186, 0x13D, 0x0FC, 0x0C1, 0x08E, 0x063, 0x03F, 
                      0x023, 0x00F, 0x003, 0x000, 0x003, 0x00F, 0x023, 0x03F, 
                      0x063, 0x08E, 0x0C1, 0x0FC, 0x13D, 0x186, 0x1D5, 0x22A,
                      0x285, 0x2E5, 0x34B, 0x3B5, 0x424, 0x497, 0x50D, 0x586,
                      0x601, 0x67F, 0x6FE, 0x77E,}, // Sinusoidal Data
                     {0x000, 0x051, 0x0A3, 0x0F5, 0x147, 0x199, 0x1EB, 0x23D,
                      0x28F, 0x2E1, 0x333, 0x384, 0x3D6, 0x428, 0x47A, 0x4CC,
                      0x51E, 0x570, 0x5C2, 0x614, 0x666, 0x6B7, 0x709, 0x75B,
                      0x7AD, 0x7FF, 0x851, 0x8A3, 0x8F5, 0x947, 0x999, 0x9EA,
                      0xA3C, 0xA8E, 0xAE0, 0xB32, 0xB84, 0xBD6, 0xC28, 0xC7A,
                      0xCCC, 0xD1D, 0xD6F, 0xDC1, 0xE13, 0xE65, 0xEB7, 0xF09,
                      0xF5B, 0xFAD, 0xFFF, 0xFAD, 0xF5B, 0xF09, 0xEB7, 0xE65,
                      0xE13, 0xDC1, 0xD6F, 0xD1D, 0xCCC, 0xC7A, 0xC28, 0xBD6,
                      0xB84, 0xB32, 0xAE0, 0xA8E, 0xA3C, 0x9EA, 0x999, 0x947,
                      0x8F5, 0x8A3, 0x851, 0x7FF, 0x7AD, 0x75B, 0x709, 0x6B7,
                      0x666, 0x614, 0x5C2, 0x570, 0x51E, 0x4CC, 0x47A, 0x428,
                      0x3D6, 0x384, 0x333, 0x2E1, 0x28F, 0x23D, 0x1EB, 0x199,
                      0x147, 0x0F5, 0x0A3, 0x051,}, // Triangular Data
                     {0x000, 0x028, 0x051, 0x07A, 0x0A3, 0x0CC, 0x0F5, 0x11E,
                      0x147, 0x170, 0x199, 0x1C2, 0x1EB, 0x214, 0x23D, 0x266,
                      0x28F, 0x2B8, 0x2E1, 0x30A, 0x333, 0x35B, 0x384, 0x3AD,
                      0x3D6, 0x3FF, 0x428, 0x451, 0x47A, 0x4A3, 0x4CC, 0x4F5,
                      0x51E, 0x547, 0x570, 0x599, 0x5C2, 0x5EB, 0x614, 0x63D,
                      0x666, 0x68E, 0x6B7, 0x6E0, 0x709, 0x732, 0x75B, 0x784,
                      0x7AD, 0x7D6, 0x7FF, 0x828, 0x851, 0x87A, 0x8A3, 0x8CC,
                      0x8F5, 0x91E, 0x947, 0x970, 0x999, 0x9C1, 0x9EA, 0xA13,
                      0xA3C, 0xA65, 0xA8E, 0xAB7, 0xAE0, 0xB09, 0xB32, 0xB5B,
                      0xB84, 0xBAD, 0xBD6, 0xBFF, 0xC28, 0xC51, 0xC7A, 0xCA3,
                      0xCCC, 0xCF4, 0xD1D, 0xD46, 0xD6F, 0xD98, 0xDC1, 0xDEA,
                      0xE13, 0xE3C, 0xE65, 0xE8E, 0xEB7, 0xEE0, 0xF09, 0xF32,
                      0xF5B, 0xF84, 0xFAD, 0xFD6,}, // Sawtooth Data 
                     };

//gabo
//u8   DiskBuf[MAX_SEC_SIZE+4+32+28]; // USB 
u8   Flashing, PwrDownEn = 1;
vu16 Dly_mS, Delay_Cnt, Sec_Cnt, BeepCnt, PD_Cnt = 300;// = LED_PWM_MAX;
u16  Fps, FpsCnt, nKeySt;
u16  WfBuf[100];                    // 
u16  KeyAct = 0, Vsum = 3600*8;

/*******************************************************************************
  USB 
*******************************************************************************/
void DiskReset()
{
    /*gabo
  Hw.pDevInit(USB_SCI);
  Delay_mS(500);
  Hw.pDevInit(USB_DEV);
  USB_Init();
     */
}
/*******************************************************************************
  Description : 
*******************************************************************************/
void USB_DiskLink(u8 NewState)
{
    /*gabo
  if(NewState == DISABLE){             
    while(PowerOff() != USB_SUCCESS) {}; // USB Device Power Off!
    Hw.pDevInit(USB_SCI);
    *Hw.pOut_SCD = 0;
    *Hw.pOut_SCP = 0;
  } else {
    Hw.pDevInit(USB_DEV);
    USB_Init();                          // USB Device Power On Init
  }
     */
}
//int __GetEncoder(); //gabo
/*******************************************************************************
 ϵͳ
*******************************************************************************/
void SysInt(void)
{
  static u16 LastEnc, LastSt, Cnt_20mS, CursorCnt, Cnt_mS, LED_Dir, LED_Pwm;
  if (Dly_mS != 0) Dly_mS--;
  Cnt_mS++;
  if(Cnt_mS%2 == 0){                                     // 
/*gabo
    u16 EncdInp = (*Hw.pEa_St << 12)|(*Hw.pEb_St << 13)|
                  (*Hw.pEc_St << 14)|(*Hw.pEd_St << 15); // 
    u16 EncActP = EncdInp & ~LastEnc;                    // 
    u16 EncActN = ~EncdInp & LastEnc;                    // 
    LastEnc = EncdInp;
    if(EncActN & ENC1a) KeyAct = (EncdInp & ENC1b) ? ENCD_1p : ENCD_1n;
    if(EncActN & ENC2a) KeyAct = (EncdInp & ENC2b) ? ENCD_2n : ENCD_2p;
    if(EncActP & ENC1b) KeyAct = (EncdInp & ENC1a) ? ENCD_1p : ENCD_1n;
    if(EncActP & ENC2b) KeyAct = (EncdInp & ENC2a) ? ENCD_2n : ENCD_2p;
*/  
      KeyAct = 0; //__GetEncoder();

    if(BeepCnt > 1) BeepCnt -= 2;
    else            *Hw.pBuz_Dev &= ~ENABLE;             // 
    if(Cnt_mS % 20 == 0){                                // 20mS 
      if(Cnt_mS > 1000) Cnt_mS = 0;
      nKeySt  = (*Hw.pK1_St << 0)|(*Hw.pK2_St << 1)|
                (*Hw.pK3_St << 2)|(*Hw.pK4_St << 3);     // 
      //KeyAct |= (~nKeySt & LastSt) << 4;                 // ʶ
      //KeyAct |= (nKeySt & ~LastSt) << 8;                 // ʶ
      //LastSt  = nKeySt;
      if(Cnt_20mS < 50) Cnt_20mS++;
      else {                                             // 1S 
        Cnt_20mS = 0;
        Sec_Cnt++;
        if(PD_Cnt > 0) PD_Cnt--;
        Fps = FpsCnt;                                    // ֡
        FpsCnt = 0;
      }
      if((Pop[SPDT].Val != 0) && (PD_Cnt == 0)){         // 
        if(LED_Dir == 0){
          if(LED_Pwm < LED_PWM_MAX-5) LED_Pwm += 5;        // 
          else                        LED_Dir  = 1;
        } else {
          if(LED_Pwm > LED_PWM_MIN+5) LED_Pwm -= 5;        // 
          else                        LED_Dir  = 0;
        }
        *Hw.pPwm_LED = LED_Pwm;
      }
      if(CursorCnt++ >= 12){                             // 12*20mS = 240mS 
        CursorCnt = 0;
        Flashing = (Flashing) ? 0 : 1;
        Vsum = Vsum-Vsum/8+*Hw.pAdc_Vbty*375/256;        // 8 
        if(Vsum > 3800*8)      Menu[BTY].Val = 4;
        else if(Vsum > 3600*8) Menu[BTY].Val = 3; 
        else if(Vsum > 3400*8) Menu[BTY].Val = 2; 
        else if(Vsum > 3200*8) Menu[BTY].Val = 1; 
//      else if(Vsum > 3000*8) Menu[BTY].Val = 0; 
        else *Hw.pOut_PEn = 0;                           // 
        Menu[BTY].Flg |= UPDT;                           // ˢ
      }
    }
  }
}
/*******************************************************************************

*******************************************************************************/
void DispStr(u16 x, u16 y, u16 Color, u8 Mode, u8 *Str)
{ 
  LCD_WrBlock(x, y, 399, y+13);
  Hw.pDispStr8x14(Color, BLK, Mode, (u32)&Str[0]);
}
/*******************************************************************************
 Font_8x14
*******************************************************************************/
u16 Font_8x14(u8 Code, u16 Row)
{
  return Hw.pFont_8x14[((Code-0x22)*8)+Row];
}
/*******************************************************************************
 
*******************************************************************************/
void LCD_WrBlock(u16 x1, u16 y1, u16 x2, u16 y2)
{
  Hw.pLCD_W_Block(x1, y1, x2, y2);
}

void LCD_RdBlock(u16 x1, u16 y1, u16 x2, u16 y2)
{
  Hw.pLCD_R_Block(x1, y1, x2, y2);
}
/*******************************************************************************
 
*******************************************************************************/
void WrPxl(u16 Color)
{
  *Hw.pLcd_Data_W = Color;
}
/*******************************************************************************
 LCD_GetPixl
*******************************************************************************/
u16 RdPxl(void)
{
  return *Hw.pLcd_Data_R;
}
/*******************************************************************************
 
*******************************************************************************/
void PxlPosi(u16 x, u16 y)
{
  Hw.pLCD_W_Block(x, y, Hw.MaxLcdCol, Hw.MaxLcdRow);
}
/*******************************************************************************
 GetDev_SN
*******************************************************************************/
u32 GetDev_SN(void)
{
  return Hw.ProductSN;
}
/*******************************************************************************

*******************************************************************************/
void SmplStart(void)
{ 
  if(Menu[RUN].Val == HOLD) return;          // 
  FPGA_DataWr(A_C_CH, SMPL_MODE, SEPARATE);  // 
  FPGA_DataWr(B_D_CH, SMPL_MODE, SEPARATE);
  SetPreSmplDpth(30*Menu[T_0].Val);          // Ԥ
  SetBase(Menu[TIM].Val);                    // 
  FPGA_ByteWr(A_C_CH, TRIG_VOLT, Vt[CH_A]);  // 
  FPGA_ByteWr(B_D_CH, TRIG_VOLT, Vt[CH_B]);
  SetTriggTyp(Menu[TRG].Val, Menu[V_T].Src); // 
  
  Trigg = 0, ScrnF = 0;                      // 
  *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;        // FPGA 
}
/*******************************************************************************
  
*******************************************************************************/
void TrackClr(void)
{
  Mem32Set((u32*)Track, 0xFFFFFFFF, X_SIZE);
}
/*******************************************************************************
 
*******************************************************************************/
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
/*******************************************************************************
 
*******************************************************************************/
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
/*******************************************************************************
 
*******************************************************************************/
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
/*******************************************************************************
  bit7~0: Byte
*******************************************************************************/
u8 FPGA_ByteRd(u8 Dev, u8 Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  vu8 Info = SpiX->DR;                     // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  u8 Byte = (SpiX->DR);                    // 
  *nSS = 1;
  return Byte;
}
/*******************************************************************************
  bit15~0: Data
*******************************************************************************/
u16 FPGA_DataRd(u8 Dev, u8 Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  vu8 Info = SpiX->DR;                     // 
  while(!(SpiX->SR & SPI_I2S_FLAG_TXE)){}
  SpiX->DR = 0;                            // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  u16 Data = (SpiX->DR) & 0xFF;            // 
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
u32 FPGA_WordRd(u8 Dev, u8 Cmd)
{
  SPI_TypeDef* SpiX = (SPI_TypeDef*)Hw.Fpga_Spi;
  vu8* nSS = (Dev == 0) ? Hw.pOut_nSS0 : Hw.pOut_nSS1;
  *nSS = 0;
  SpiX->DR = Cmd;                          // 
  while(!(SpiX->SR & SPI_I2S_FLAG_RXNE)){}
  vu8 Info = SpiX->DR;                     // 
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
/*******************************************************************************
  
*******************************************************************************/
void SetRangeA(u8 Range)
{ 
  *Hw.pOut_A_Level  = RANGE_SW[0][Range];
  *Hw.pOut_A_Slct0  = RANGE_SW[1][Range];
  *Hw.pOut_A_Slct1  = RANGE_SW[2][Range];
  *Hw.pOut_A_Slct2  = RANGE_SW[3][Range];
  *Hw.pOut_A_Slct_B = 0;
}
/*******************************************************************************
  
*******************************************************************************/
void SetRangeB(u8 Range)
{
  *Hw.pOut_B_Level  = RANGE_SW[0][Range];
  *Hw.pOut_B_Slct0  = RANGE_SW[1][Range];
  *Hw.pOut_B_Slct1  = RANGE_SW[2][Range];
  *Hw.pOut_B_Slct2  = RANGE_SW[3][Range];
  *Hw.pOut_B_Slct_A = 0;
}
/*******************************************************************************
  
*******************************************************************************/
void SetBase(u8 Base)
{
  FPGA_DataWr(A_C_CH, SMPL_PSCL, BASE_PSC[Base]);
  FPGA_DataWr(B_D_CH, SMPL_PSCL, BASE_PSC[Base]);
  FPGA_DataWr(A_C_CH, SMPL_TDIV, BASE_ARR[Base]);
  FPGA_DataWr(B_D_CH, SMPL_TDIV, BASE_ARR[Base]);
}
/*******************************************************************************
  ͨ
*******************************************************************************/
void SetOffsetA(u8 Range, u8 Offset)
{
  *Hw.pCh_A_Posn = Diff[CH_A][Range]+CALIBRAT(Slope[CH_A], Offset);
}
void SetOffsetB(u8 Range, u8 Offset)
{
  *Hw.pCh_B_Posn = Diff[CH_B][Range]+CALIBRAT(Slope[CH_B], Offset);
}
/*******************************************************************************
  Ԥ
*******************************************************************************/
void SetPreSmplDpth(u16 PsDpth)
{
  PsDpth = (PsDpth+1)*BASE_KP1[Menu[TIM].Val]/1024; // 
  FPGA_DataWr(A_C_CH, SMPL_PSMP, PsDpth);
  FPGA_DataWr(B_D_CH, SMPL_PSMP, PsDpth);
}
/*******************************************************************************
  
*******************************************************************************/
void SetDgtlFrq(u16 Nfrq)
{
  Hw.pDevInit(SO_DGTL);
  *Hw.pFout_TIM_PSC = DGTL_PSC[Nfrq];
  *Hw.pFout_TIM_ARR = DGTL_ARR[Nfrq];
  *Hw.pFout_TIM_CCR = (DGTL_ARR[Nfrq]+1)/2;
}
/*******************************************************************************
  
*******************************************************************************/
void SetAnlgFrq(u16 Nfrq)
{
  Hw.pDevInit(SO_ANLG);
  Hw.pFout_DMA(DISABLE);
  *Hw.pFout_DMA_PSC = ANLG_PSC[Nfrq];
  *Hw.pFout_DMA_ARR = ANLG_ARR[Nfrq];
  u32 n = (Nfrq == 10) ? 50 : 100;
  *Hw.pFout_DMA_CNT = n;
  for(u32 i = 0; i < n; i++){
    u32 k = (n == 50) ? 2*i : i;
    WfBuf[i] = 205+WF100[Menu[OUT].Val][k]*900/1000;
  }
  *Hw.pFout_DMA_CMA = (u32)&WfBuf;
  Hw.pFout_DMA(ENABLE);
}
/*******************************************************************************
  
*******************************************************************************/
void SetTriggTyp(u8 Type, u8 Src)
{
  u8 Temp0, Temp1;
  if(Type < 8) switch(Src){
    case CH_A:
      Temp0 = Type+TRIG_AiDN;
      Temp1 = TRIG_NONE;
    break;
    case CH_B:
      Temp1 = Type+TRIG_AiDN;
      Temp0 = TRIG_NONE;
    break;
    case CH_C:
      Temp0 = Type+TRIG_DiDN;
      Temp1 = TRIG_NONE;
    break;
    case CH_D:
      Temp1 = Type+TRIG_DiDN;
      Temp0 = TRIG_NONE;
    break;
  } else {
    Temp1 = TRIG_ANY;
    Temp0 = TRIG_ANY;
  }
  FPGA_ByteWr(A_C_CH, TRIG_KIND, Temp0);
  FPGA_ByteWr(B_D_CH, TRIG_KIND, Temp1);
}
/*******************************************************************************
  
*******************************************************************************/
void Beep_mS(u16 mS)
{
  BeepCnt = mS;
  *Hw.pBuz_Dev |= ENABLE;
}
/*******************************************************************************
  
*******************************************************************************/
void Delay_mS(vu32 mS)
{
  Dly_mS = mS;
  while(Dly_mS) {};
}
/*******************************************************************************
  USB 
*******************************************************************************/
void USB_DevInit(void)
{
    /*gabo
  GPIO_InitTypeDef  GPIO_Struct;
  NVIC_InitTypeDef  NVIC_Struct;

  RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
  GPIO_Struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
  GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_Struct);
  RCC->APB1ENR |= RCC_APB1Periph_USB;
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_Struct.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;
  NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Struct.NVIC_IRQChannelSubPriority        = 0;
  NVIC_Struct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_Struct);
     */
}

/******************************** END OF FILE *********************************/
