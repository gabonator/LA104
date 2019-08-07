/******************** (C) COPYRIGHT 2018 e-Design Co.,Ltd. *********************
  DS213_APP Process.c                                            Author : bure
*******************************************************************************/
#include "Process.h"
#include "DS213Bios.h"
#include "Sys.h"


extern uint32_t counter;

u8   WaveFormExtract(u16 Xposi);
u8   EffectiveMeasur(u8 CHn, u8 MaxV, u8 MinV);
void TimeMeasur(uimenu *MeterT);

u8   Recod[4*X_SIZE+1];        // 
u8   Trigg = 0, ScrnF = 0;
u16  RmsA, RmsB, MaxA, MaxB, MinA, MinB;
u16  MeasT[4][6];

/*******************************************************************************
  
*******************************************************************************/
void TimeMeasur(uimenu *MeterT)
{
  u32 TrkN = MeterT->Src;
//  , Kind = MeterT->Val;
//  if(Kind > CYCL) return;
  u32 Vst = 0;
  u32 Vth = (TrkN == TRCK1) ? (MaxA+MinA)/2 : 
            (TrkN == TRCK2) ? (MaxB+MinB)/2 : Yn[TrkN]+10;
            
//  u32 Vst = 0, Vth = (TrkN <= TRCK2) ? Vt[TrkN] : Yn[TrkN]+10;
  u32 Lst = 0, T1H = 0, T2H = 0, T1L = 0, T2L = 0;
  u32 TcS = 0, TcN = 0, Bgn = 0, End = 0;
  for(u32 x = 20; x < X_SIZE; x++){
    u32 Vin = Track[TrkN+x*4];
    if(Vin > Vth+5) Vst = 1;
    if(Vin < Vth-5) Vst = 0;
    if(x > 20 && Lst != Vst){
      if(Vst == 0){
        if(T2H == 0 && T1H != 0) T2H = x;       // 
        if(T1L == 0)             T1L = x;       // 
      } else {
        if(T2L == 0 && T1L != 0) T2L = x;       // 
        if(T1H == 0)             T1H = x;       // 
      } 
      if(TcS == 0)    TcS = Lst*2+Vst, Bgn = x; // 
      else if(TcS == Lst*2+Vst) TcN++, End = x; // 
    }
    Lst = Vst;
  }
  u32 TwH = (T2H != 0 && T1H != 0) ? T2H-T1H : 0;
  u32 TwL = (T2L != 0 && T1L != 0) ? T2L-T1L : 0;
  MeasT[TrkN][PTWH] = TwH, MeasT[TrkN][PTWL] = TwL;
  MeasT[TrkN][CYCL] = (TcN != 0 && End != 0) ? (End-Bgn)/TcN : 0;
  
//  u32 T1P = 0, T2P = 0;
//  switch(Kind){
//    case PTWH: T1P = T1H, T2P = T2H; break;
//    case DUTY: T1P = T1H, T2P = T2H; break;
//    case PTWL: T1P = T1L, T2P = T2L; break;
//    case CYCL:
//    case FQRN: T1P = Bgn, T2P = End; break;
//  }
//  if(T1P) Menu[T_1].Flg |= INVR, Menu[T_1].Val = T1P; // T1 
//  if(T2P) Menu[T_2].Flg |= INVR, Menu[T_2].Val = T2P; // T2 
}
/*******************************************************************************
  
*******************************************************************************/
u8 EffectiveMeasur(u8 CHn, u8 MaxV, u8 MinV)
{
  if(MaxV-MinV < 20) return 0;                          // 
  u32 Mid = (MaxV+MinV)/2, Rms, Sqs = 0, Cnt = 0;
  u32 Lst, CnS = 0;
  for(u32 x = X_BASE; x < X_SIZE; x++){
    s32 Vin = Track[CHn+x*4]-Yn[CHn];
    Sqs = Sqs+Vin*Vin, Cnt++;                           // 
    u32 Vst = (Track[CHn+x*4] > Mid) ? 1 : 0;
    if(x > X_BASE && Lst != Vst){
      if(CnS == 0){                                     // 
        CnS = Lst*2+Vst;//, Bgn = x;
        Sqs = 0, Cnt = 0;
      } else if(CnS == Lst*2+Vst){                      // 
        Rms = Sqrt32(Sqs/Cnt); // End = x; 
      }
    } 
    Lst = Vst;
  }
//  u32 Bgn = 0, End = 0;
//  *pTnS = 0;
//  if(*pTnS == 0){                                       // 
//    Menu[T_2].Flg |= INVR, Menu[T_2].Val = X_SIZE-1;
//    if(Bgn) Menu[T_1].Flg |= INVR, Menu[T_1].Val = Bgn; // T1 
//    if(End) Menu[T_2].Flg |= INVR, Menu[T_2].Val = End; // T2 
//  }
//  if(Bgn && End) *pTnS = 1;                             // ռ
  return Rms;
}
/*******************************************************************************
  
*******************************************************************************/
void DispSync(void)
{
  uint32_t millis = counter;
  static uint32_t lastRefresh = 0;

  u32 ViewPosi = 30*Menu[XNP].Val*BASE_KP1[Menu[TIM].Val]/1024; // 


u8 str[16];
u32To8HexStr(str, ViewPosi);
  DispStr(0, 0, GRN, CHAR, (u8*)"         ");
  DispStr(0, 0, GRN, CHAR, (u8*)str);


  u32 Status = FPGA_CtrlRW(CH_A, SMPL_ST) | FPGA_CtrlRW(CH_B, SMPL_ST);
  if(Status & TRIGG) Trigg = 1;
  if(Status & SCRNF) ScrnF = 1;
  
  switch(Menu[SYN].Val){
    case AUTO:                                        // AUTO Mode 
      if(Trigg || ScrnF) WaveFormExtract(ViewPosi);   // 
    break;
    case NORM:                                        // NORM Mode 
        if(Trigg) WaveFormExtract(ViewPosi);          // 
    break;
    case SNGL:                                        // SNGL Mode 
      if(Trigg){
        WaveFormExtract(ViewPosi);                    // 
        if(ScrnF){
          Menu[RUN].Val  = HOLD;                      // ȡ
          Menu[RUN].Flg |= UPDT;                      // ˢ
        }
      }
    break;
    case SLOW:                                        // SLOW Mode 
      WaveFormExtract(0);                             // 
    break;
  }
  DisplayWaveForm();                                  // 
  if(ScrnF){
    MaxA = FPGA_ByteRd(A_C_CH, VMAX), MinA = FPGA_ByteRd(A_C_CH, VMIN);
    MaxB = FPGA_ByteRd(B_D_CH, VMAX), MinB = FPGA_ByteRd(B_D_CH, VMIN);
    if(Menu[VM1].Val == RMS || Menu[VM2].Val == RMS){
      RmsA = EffectiveMeasur(TRCK1, MaxA, MinA);      // 
      RmsB = EffectiveMeasur(TRCK2, MaxB, MinB);
    }
    TimeMeasur(&Menu[TM1]), TimeMeasur(&Menu[TM2]);   // 
    if(Menu[SYN].Val != SNGL) { lastRefresh = millis; SmplStart(); }            // 
    if(Menu[SYN].Val == SLOW) TrackClr();             // 
  }
  if (millis - lastRefresh > 500)
  {
  DispStr(0, 0, GRN, CHAR, (u8*)"#############");
    lastRefresh = millis;
    SmplStart();
  }
}
/*******************************************************************************
  
*******************************************************************************/
u8 WaveFormExtract(u16 Xposi)
{
  FPGA_DataWr(A_C_CH, SMPL_RPTR, Xposi);              // 
  FPGA_DataWr(B_D_CH, SMPL_RPTR, Xposi);
  
  u32 Posi1 = Yn[TRCK1], Posi2 = Yn[TRCK2];           // 
  u32 Posi3 = Yn[TRCK3], Posi4 = Yn[TRCK4];           // 
  s32 Intrp = BASE_KP1[Menu[TIM].Val];                // 
  s32 GainA = Gain[CH_A][Menu[RNA].Val];              // A ͨ
  s32 GainB = Gain[CH_B][Menu[RNB].Val];              // B ͨ
  s32 n = Menu[T3S].Val;                              // n = 2~7 
  s32 m = Menu[T4S].Val;                              // m = 2~5 
  u8* p = (u8*)Track;
  u8* q = (u8*)&Recod[m-2];
  u32 Fposi = Recod[4*359+m-2];
  u32 k, LastA, LastB, LastC, LastD;
  u32 x = 0;
  
  while(1){
    u32 SmplAC = FPGA_SmplRd(A_C_CH);
    u32 SmplBD = FPGA_SmplRd(B_D_CH);
    u32 Status = SmplAC | SmplBD;
    if(Status & TRIGG) Trigg = 1;
    if(Status & SCRNF) ScrnF = 1;
    if(Status & EMPTY) return Status;

    s16 DataA = CALIBRAT(GainA, (SmplAC >> 8)-Posi1); // A ͨ
    s32 AmplA = Posi1+DataA;                          // A ͨ
    s32 CurrA = (AmplA > Y_SIZE)? Y_SIZE :
                (AmplA < Y_BASE)? Y_BASE : AmplA;     // A ͨ

    s16 DataB = CALIBRAT(GainB, (SmplBD >> 8)-Posi2); // B ͨ
    s32 AmplB = Posi2+DataB;                          // B ͨ
    s32 CurrB = (AmplB > Y_SIZE)? Y_SIZE :
                (AmplB < Y_BASE)? Y_BASE : AmplB;     // B ͨ
    
    s16 DataD = (m > 1) ? *q-Fposi : (SmplBD & 1)*20; // D ͨ
    s32 AmplD = Posi4+DataD;                          // D ͨ
    u32 CurrD = (AmplD > Y_SIZE)? Y_SIZE :
                (AmplD < Y_BASE)? Y_BASE : AmplD;     // D ͨ
    
    s16 DataC = (SmplAC & 1)*20;                      // C ͨ
    switch (n){                                       // C ͨ
      case A_ADD_B: DataC = DataA + DataB; break;
      case A_SUB_B: DataC = DataA - DataB; break;
      case C_AND_D: DataC = DataC & DataD; break;
      case C_OR_D : DataC = DataC | DataD; break;
      case INV_A  : DataC = -DataA; break;
      case INV_B  : DataC = -DataB; break;
    }        
    s32 AmplC = Posi3+DataC;                          // C ͨ
    s32 CurrC = (AmplC > Y_SIZE)? Y_SIZE :
                (AmplC < Y_BASE)? Y_BASE : AmplC;     // C ͨ
   
    if(x == X_BASE){                                  // 
      *p++ = CurrA, LastA = CurrA; 
      *p++ = CurrB, LastB = CurrB; 
      *p++ = CurrC, LastC = CurrC; 
      *p++ = CurrD, LastD = CurrD;
      k = 0, x++;
    } else {
      s32 DiffA = CurrA-LastA, DiffB = CurrB-LastB;
      s32 DiffC = CurrC-LastC, DiffD = CurrD-LastD;
      u32 InstA, InstB, InstC, InstD;
      while(k <= 1024){                               // 
        if(m > 1) q +=4;           
        InstA = LastA+DiffA*k/1024, *p++ = InstA; 
        InstB = LastB+DiffB*k/1024, *p++ = InstB; 
        InstC = LastC+DiffC*k/1024, *p++ = InstC; 
        InstD = LastD+DiffD*k/1024, *p++ = InstD; 
        k += Intrp;
        if(++x == X_SIZE) return Status;//SCRNF;
      }
      k -= 1024; 
      LastA = CurrA, LastB = CurrB, LastC = CurrC, LastD = CurrD;
    }
  }
}
/*******************************************************************************
  Update_Range: 
*******************************************************************************/
void CtrlUpdate(u8 Item) 
{
  u16 Value = Menu[Item].Val;
  u32 Tw;
  switch(Item){
    case RUN:
      if(Menu[RUN].Val == SMPL){
        SmplStart();                             // 
        TrackClr();                              // 
      }
    break;
    case CPA: 
      *Hw.pOut_A_Coupl = Value & 1, YnHide[TRCK1] = !Value; // If "--" then Hide Trakc#1 
    break;
    case CPB: 
      *Hw.pOut_B_Coupl = Value & 1, YnHide[TRCK2] = !Value; // If "--" then Hide Trakc#2
    break;
    case RNA:
      SetRangeA(Value), SetOffsetA(Value, Yn[CH_A]);
    break;
    case RNB:
      SetRangeB(Value), SetOffsetB(Value, Yn[CH_B]);
    break;
    case T3S:
      YnHide[TRCK3] = !Value;                    // If "--" then Hide Trakc#3
    break;
    case T4S:
      YnHide[TRCK4] = !Value;                    // If "--" then Hide Trakc#4
    break;
    case YNP:
    case CAL:
      SetOffsetA(Menu[RNA].Val, Yn[CH_A]);
      SetOffsetB(Menu[RNB].Val, Yn[CH_B]);
    break;
    case T_0:
      SetPreSmplDpth(Value*30);                  // Ԥ
      break;
    case V_T:
      FPGA_ByteWr(A_C_CH, TRIG_VOLT, Vt[CH_A]);
      FPGA_ByteWr(B_D_CH, TRIG_VOLT, Vt[CH_B]);
    break;
    case T_1:
    case T_2:
      if(Menu[T_1].Val > Menu[T_2].Val) Tw = Menu[T_1].Val-Menu[T_2].Val;
      else                              Tw = Menu[T_2].Val-Menu[T_1].Val;
      FPGA_DataWr(A_C_CH, TRIG_WDTH, Tw);
      FPGA_DataWr(B_D_CH, TRIG_WDTH, Tw);
    break;
    case OUT:
    case FRQ:
      if(Menu[OUT].Val == DGTL) SetDgtlFrq(Menu[FRQ].Val);
      else                      SetAnlgFrq(Menu[FRQ].Val);
    break;
    case TIM:
      SetBase(Value); 
    break;
    case TRG:
      SetTriggTyp(Menu[TRG].Val, Menu[V_T].Src);
    break;
  }
  if(Menu[SYN].Val == SLOW) SetTriggTyp(TRIG_ANY, Menu[V_T].Src);
  else                      SetTriggTyp(Menu[TRG].Val, Menu[V_T].Src);
}
/******************************** END OF FILE *********************************/
