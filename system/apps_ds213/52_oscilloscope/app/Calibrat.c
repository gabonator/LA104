/******************** (C) COPYRIGHT 2018 e-Design Co.,Ltd. *********************
  DS213_APP Calibrat.c                                           Author : bure
*******************************************************************************/
#include "Sys.h"
#include "DS213Bios.h"
#include "Calibrat.h"

/*******************************************************************************
  校正垂直位移零位偏差与斜率偏差
*******************************************************************************/
void Correction(void)
{
  u16 BakA[10], BakB[10];
  for(u32 i = 0; i < ITEM_END; i++) CtrlUpdate(i);          // 全部硬件初始化设置
  *Hw.pOut_A_Coupl = couplingAC, *Hw.pOut_B_Coupl = couplingAC;             // 模拟输入为交流耦合
  SetBase(15);                                              // 设置采样速率 10uS/Div
  SetTriggTyp(TRIG_ANY, Menu[V_T].Src);                     // 设任意触发
  Hw.pDevInit(SO_ANLG);
  *Hw.pFout_mV_DAC = 0;                                     // 锁定模拟输出值 = 0
  Hw.pFout_DMA(DISABLE);
  SetRangeA(9), SetRangeB(9), 
  Delay_mS(1000);                                           // 等待输入耦合稳定
  u32 AbsA, AbsB;
  for(s32 Range = 9; Range >= 0; Range--){
    SetOffsetA(9, 20), SetOffsetB(9, 20);
    SetRangeA(Range), SetRangeB(Range);
    BakA[Range] = Diff[CH_A][Range]; 
    BakB[Range] = Diff[CH_B][Range];                        // 备份偏移校正系数
    
//u8 Str[10]; 
//RowPosi(0*8, Range*18+36);
//u8To2DecStr(Str, Range); AddStr(TXT2C, CHAR, Str); AddStr(TXT2C, CHAR, ": ");

    Delay_mS((Range == 4) ? 500 : 200);                     // 等高低压切换稳定    
    for(u32 n = 20; n >= 10; n--){
      SetOffsetA(Range, n), SetOffsetB(Range, n);           // 设置低端偏移
      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;                   // 开始采样
      Delay_mS(50);                                         // 等待偏移滤波稳定    
      s32 DiffA = n-FPGA_ByteRd(A_C_CH, VAVG);
      s32 DiffB = n-FPGA_ByteRd(B_D_CH, VAVG);              // 低端偏移误差
      
//s8To2DecStr(Str, DiffA); AddStr(TXT2C, CHAR, Str); AddStr(TXT2C, CHAR, " ");
      
      AbsA = abs(DiffA), AbsB = abs(DiffB);
      if(AbsA == 0 && AbsB == 0) break;
      Diff[CH_A][Range] += DiffA; 
      Diff[CH_B][Range] += DiffB;                           // 修正低端偏移
    }
    if(AbsA > 2) Diff[CH_A][Range] = BakA[Range];           // 校准失败恢复原值
    if(AbsB > 2) Diff[CH_B][Range] = BakB[Range];
    AddStr(TXT2C, CHAR, ".");
  }
  SetOffsetA(9, 190), SetOffsetB(9, 190);                   // 设置高端偏移
  SetRangeA(9), SetRangeB(9), Delay_mS(1000);               // 等低压切换高压稳定
  BakA[0] = Slope[CH_A], BakB[0] = Slope[CH_B];             // 备份斜率校正系数
  
//RowPosi(3*8, 18);
//u8 Str[10]; 

  for(u32 n = 0; n < 4; n++){
    s32 Asum = 0, Bsum = 0;
    s32 OffsA, OffsB;
    AddStr(TXT2C, CHAR, (u8*)".");
    for(u32 Range = 9; Range >= 6; Range--){
      SetRangeA(Range), SetRangeB(Range);                   // 设置当前档位
      SetOffsetA(Range, 190), SetOffsetB(Range, 190);       // 设置高端偏移
      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;                   // 开始采样
      Delay_mS((Range == 9) ? 500 : 100);                   // 等待换挡稳定    
      OffsA = FPGA_ByteRd(A_C_CH, VAVG);
      OffsB = FPGA_ByteRd(B_D_CH, VAVG);                    // 读出高端偏移误差
      Asum += OffsA, Bsum += OffsB;
    }
    Slope[CH_A] = (Slope[CH_A]*190)*4/Asum;                 // 修正斜率误差系数
    Slope[CH_B] = (Slope[CH_B]*190)*4/Bsum;
    SetOffsetA(6, 190), SetOffsetB(6, 190);                 // 修正高端偏移
    *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;                     // 开始采样
    Delay_mS(100);                                          // 等待偏移滤波稳定    
    OffsA = FPGA_ByteRd(A_C_CH, VAVG)-190;
    OffsB = FPGA_ByteRd(B_D_CH, VAVG)-190;                  // 读出高端偏移误差
    
//s16To5DecStr(Str, OffsA); AddStr(TXT1C, CHAR, Str); AddStr(TXT1C, CHAR, " ");

    AbsA = abs(OffsA), AbsB = abs(OffsB);
    if(AbsA == 0 && AbsB == 0) break;
  } 
  if(AbsA > 2) Slope[CH_A] = BakA[0];                       // 校准失败恢复原值
  if(AbsB > 2) Slope[CH_B] = BakB[0];
  for(u32 i = 0; i < ITEM_END; i++) Menu[i].Flg |= UPDT;

//while(nKeySt & K4){};
  
} 

/*********************************  END OF FILE  ******************************/
