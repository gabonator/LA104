/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Process.h  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#ifndef __Process_h
#define __Process_h

#include "system.h"
#include "Draw.h"

//====================================================
#define A                 0
#define B                 1
#define A_                2
#define B_                3

//================== Structure variables, macro definitions ==================
#define _1_source  Title[TRACK1][SOURCE].Value
#define _2_source  Title[TRACK2][SOURCE].Value
#define _3_source  Title[TRACK3][SOURCE].Value
#define _4_source  Title[TRACK4][SOURCE].Value
#define _1_posi    Title[TRACK1][POSI].Value
#define _2_posi    Title[TRACK2][POSI].Value
#define _3_posi    Title[TRACK3][POSI].Value
#define _4_posi    Title[TRACK4][POSI].Value
#define _C_adj     Title[TRACK3][RANGE].Value
#define _D_adj     Title[TRACK4][RANGE].Value
#define _A_Range   Title[TRACK1][RANGE].Value
#define _B_Range   Title[TRACK2][RANGE].Value

#define _Tr_source Title[TRIGG][SOURCE].Value
#define _Trigg     Title[TRIGG]
#define _Tr_kind   Title[TRIGG][TR_KIND].Value

#define _File      Title[FILE][0]

#define _X_posi    Title[T_BASE][XPOSI]
#define _X_View    Title[T_BASE][VIEW]
#define _T_base    Title[T_BASE][BASE].Value
#define _Mode      (Title[T_BASE][MODE].Value)

#define _A_Range   Title[TRACK1][RANGE].Value
#define _B_Range   Title[TRACK2][RANGE].Value

#define _Kp1       X_Attr[Title[T_BASE][BASE].Value].KP   // common sampling gear X interpolation coefficients
#define _Kp2       X_Attr[Title[T_BASE][BASE].Value+5].KP // X interpolation coefficients alternately sampling stalls
#define _INSERT    G_Attr[0].INSERT                       // Start using the interpolated stalls

#define _State     Title[RUNNING][STATE]
#define _Status    Title[RUNNING][STATE].Value

#define _Curr      Title[Current]
#define _Det       Detail[Current]

#define _Frqn      Title[OUTPUT][FRQN].Value
#define _Kind      Title[OUTPUT][KIND].Value
#define _Duty      Title[OUTPUT][DUTYPWM].Value

#define _T1        Title[T_VERNIE][T1].Value
#define _T2        Title[T_VERNIE][T2].Value

#define _Vt1       V_Trigg[TRACK1].Value
#define _Vt2       V_Trigg[TRACK2].Value

#define DHR12R1_DAC ((vu32)(0x40007400+0x08));

typedef struct  // 
{
  uc16 PSC;     // stall identification string
  uc16 ARR;     // frequency coefficient
} D_tab ;

typedef struct{
  u16 Psc;  
  u16 Arr;
  u16 S_OS_Size; 
  u16 Ratio;
}SubTbase;

typedef struct{
  u16 Psc;  
  u16 Arr; 
}OSTbase;

extern u8  discard;
extern s16 Xtend;
extern u8  BailoutFlag;
extern u8  CurDefTime;
extern u16 NSamples;
extern u16 CountUnread;
extern u8  TrackBuff [397 * 4];               // X_Size presently set @ 392, some routines go to MAX_X (386+MIN_X or 397 w/meters off)
extern s32 a_Avg, b_Avg, a_Ssq, b_Ssq;
extern s16  a_Max, b_Max, a_Min, b_Min;                // statistics of intermediate variables
extern s16  aT_Max, bT_Max, aT_Min, bT_Min;            // for auto trig when using cursor restricted meters
extern u8 Rdiscard;
extern u8 FrameEndFlag;
extern u16 Tcs, Tcnt;

extern u16 TaS, TbS, TcS, TdS;            // cycles accumulated
extern u32 Sum[4];
extern u16 NAvg[4];
extern u16 VNAvg[2];
extern s32 VxAvg[2];
extern s32 VxSsq[2];
extern u16 PaS, PbS, PcS, PdS;            // pulse width of the cumulative
extern u16 TaN, TbN, TcN, TdN;           // Cycle Count
extern u32 PxS[4];
extern u32 TxS[4];
extern u32 TxN[4];
extern u8 FrameMode;
extern u8 FFTflag;
extern u8 EnableFFT;
extern uc16 A_Tab[17];          
extern D_tab D_Tab[23];
extern const SubTbase TbaseSub[13];

extern u8 Options;
extern u16 TempKp1;
extern u8 ADCoffset;
extern u8 CalFlag;
extern u16 JumpCnt;
extern s8  Kab;              
extern s8  Ka1[10], Kb1[10]; 
extern u16 Ka2[10], Kb2[10]; 
extern s8  Ka3[10], Kb3[10]; 
extern s8  HKa1[10], HKb1[10]; 
extern u16 HKa2[10], HKb2[10]; 
extern s8  HKa3[10], HKb3[10]; 
extern s8  LKa1[10], LKb1[10]; 
extern u16 LKa2[10], LKb2[10]; 
extern s8  LKa3[10], LKb3[10]; 
#warning Gabo:
//extern u32 DataBuf[4096];	//
extern u32 DataBuf[2048];	//
extern u8 ScalingOffset;
extern char Vertical[15][10];
extern s16 TrigSourceEnable; 
extern u16 HiBatLevel[2];		
extern u16 LoBatLevel[2];               
extern u16 PrevBatLevel;		
extern u16 VDiff[2];			
extern  u8 SumResetFlag;
extern  u8 EnablePaS;
extern  u8 ClearHoldFlag;
extern  u8 SlowModeSkip;
extern s16 A_Posi;
extern s16 B_Posi;
extern u8  ClearLeadingEdge;
extern u16 bag_max_buf;
extern u8  FFTGain;
extern u8  ClearMeterAreaFlag;
extern u16 BufferSize;
extern uc8 Log10Mant[100];
extern u8  freerun;
extern uc8 ScaleIndex[17];
extern s16 ATT_DATA[720];
extern u8 Filter;
extern u16 NoiseBW;
extern u8  InitiateNoise;
extern u8  TriggerType;
extern s8  PrevSweepIndex;
extern u8  ResetFlag;
extern uc32 A_Freq[18];
extern s32 Waste;
extern u8  M_Factor;
extern u8  WaitForTrigFlag;
extern s16 Ch1TLevel;
extern s16 Ch2TLevel;
extern u8  SubIndex;
extern u8  StartUart;
extern u8  InitiateUartGen;
extern s16 WindowPosition;
extern u8  StartOffset;
extern u8  AutoTrigIni;
extern u8  OsBufferLogicFlag;
extern u8  OSAvg;
extern u8  PrevSource;
extern u8  PrevKind;
extern u8  OsFFTData[2][700];
extern u8  RMSdata[2][700];
extern u8  InvertA;
extern u8  InvertB;
extern u8  ScrollFlag;
extern u8  HardwareVersion;
extern u8  ResetEnableFFT;
extern u8  FastMode;
extern u8  ChartMode;
extern u8  OsChartFlag;
extern u16 ARBT_ARR;
extern u8  ToggleName;
extern u8  DisableCursorTimer;
extern u16  DownConvert;
extern u32 FreqIN;
extern u8  DownConvertInitiate;
extern u32 FFTScale; 
extern u8  DCbase;
extern u8  DCframe;
extern u8  DCmode;
extern u8  DCmeter;
extern u8  FreqScaleShift;
extern u8  DownConvertShiftEnable;
extern u32 LfreqPeriod;
extern u8  FFTt1Mode;
extern u16 DCt1;

void BackGround_Reset(u8 clear);
void App_init(u8 ClearScreen);
void View_init(void);
void Update_Range(void);
void Update_Base(void);
void Update_Output(void); 
void Update_Trig(u8 service); 
void Process(void);
void Synchro(void);
void Send_Data(s16 Va, s16 Vb, u8 C_D, u8 osB, u16 n);
u16 get_bag_max_buf();
void cleardatabuf(u8 service);
void Config_init(void);
void TransferFIFO(s16 i,u8 service);
void SetOffset(u8 channel,u8 range, s16 Ypos);
void BatLevelCompensation(void);
void ResetSum(void);
void ClearMinMax(u8 Channels);
void ClearHold(u8 Channels);
void ClearTrackBuff(u8 service);
void Beeper(u16 ms);
void WaveGen(void);
void InitiateCalData(void);
u16  RandomGen(void);
void SetIRQ2Priority(void);
void ResetDMA2_Ch4Params(u32 Count);
void ClearMeters(void);
u8 OSBufferLogic(void);
void InitiateOSBuffers(void);
u16 GetXposRef(u16 XPos);
u16 GetXpos(u16 XposRef);
void UpdateCursorMeter(void);
void CursorDisplaySelectLogic(void);
void FFTdownConvert(u8 service);
void DownConvertRestore(void);

// FFT ////////////////////////////////////////////////////////////////////

extern s32 fr[];
extern s32 fi[];

extern u8 ShowFFT;
extern u32 NFreq;
extern char NFreqStr[];
extern int imax;
extern short PeakFreq;
extern char BaseFreqStr[];
extern char PeakFreqStr[];
extern char FreqDivStr[];
extern char FreqT1Str[];

///////////////////////////////////////////////////////////// FFT ///////


#endif
/******************************** END OF FILE *********************************/
