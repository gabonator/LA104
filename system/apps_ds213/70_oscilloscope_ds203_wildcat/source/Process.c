/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
 File Name : Process.c       
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "Process.h"
#include "Draw.h"
#include "Menu.h"
#include "BIOS.h"
#include "File.h"
#include "Calibrat.h"
#include "platform.h"


// FFT ////////////////////////////////////////////////////////////////////
#warning Gabo: FFT disabled
#if 0
s32 fr[FFTSize];		
s32 fi[FFTSize];
#endif

u32 NFreq;
char NFreqStr[12];
int imax;			
short PeakFreq;
char BaseFreqStr[12];
char PeakFreqStr[12];
char FreqDivStr[12];
char FreqT1Str[12];
////////////////////////////////////////////////////////////////////////////

u32 Sum[4]={0,0,0,0};		//averaging accumulators
u16 NAvg[4]={0,0,0,0};
u16 VNAvg[2]={0,0};
s32 VxAvg[2]={0,0};
s32 VxSsq[2]={0,0};
u32 PxS[4]={0,0,0,0};
u32 TxS[4]={0,0,0,0};
u32 TxN[4]={0,0,0,0};

u16 TaS, TbS, TcS, TdS;            // cycles accumulated
u16 PaS, PbS, PcS, PdS;            // pulse width of the cumulative
u16 TaN, TbN, TcN, TdN;            // Cycle Count
s16  a_Mid_H, a_Mid_L;
s16  b_Mid_H, b_Mid_L;

s32 a_Avg, b_Avg, a_Ssq, b_Ssq;              // use signed integers, allows values at bottom of screen to be read
s16  a_Max, b_Max, a_Min, b_Min;              // 0 levels can get pushed up by calibration, bringing bottom of screen below 0
s16  aT_Max, bT_Max, aT_Min, bT_Min;         // for auto trigger, kept separate from meter data for curdeftime

s16 Posi_412,Posi_41_2, Posi_41, Posi_42, Posi_4_2, Posi_4F1, Posi_4F2, Posi_4F3, Posi_4F4;
s16 c_Max=0, d_Max=0, A_Posi, B_Posi;
u8  BailoutFlag=0;
u16 JumpCnt,CountUnread;
u8  FrameMode;
u8  HoldOnNext =0;
u16 bag_max_buf = 4096;
u8  freerun=0;					//flags for auto trig mode
u8  exitflag=0;
u8  entryflag=0;
u8  ADCoffset=54; //shifts ADC/FIFO operating area away from non-linear first xx steps into previously unused linear 200-255 step area
u16 TempKp1=1024;
u8  CalFlag;
u8  CurDefTime=0;			
u8  Options;				//options flag
u8  HoldResetFlag=0;		
s16 Xtend;				//buffer size control
u8  discard=0;				//pre-signal trace blanking
s16 TrigSourceEnable=1;			//for alternate triggering, end of cycling, will go to chA
u32 ReverseBitMask;			
u32 BitMask;
u16 AltHoldoff=0;
s8  OldShift=0;
u8  ClearMeterAreaFlag=0;
u8  ScalingOffset=40;			//centers Ka3 Y-position scaling factor on a step other than 0
                                        //25= same as calibration gain correction (Ka2) zero
                                        //40= same as y-position alignment offset (Ka1) zero calibration point  
u16 QStart[2]={0,0};			//quantization error start/stop reference positions	
u16 QEnd[2]={0,0};
u8  LastA_Mid=0;			//trigger level points for time meter quantization error calculations
u8  LastB_Mid=0;
u8  FirstA_Mid=0;
u8  FirstB_Mid=0;
u8  SumResetFlag=0;			//resets meter summing function
u32 UTmp;
u8  EnablePaS=1;			//controls update speed of PaS and PbS derived calcs (TH, TL, %duty) in large meters
u8  ClearHoldFlag=0;
u8  SlowModeSkip=0;
u8  ClearLeadingEdge=0;			//WAS 0	
u8  FFTflag=0;
u8  Normal=100;
u8  FrameEndFlag;
u8  FFTGain=0;
u16 BufferSize=512;
u8  EnableFFT;
u8  Filter=40;
u8  InitiateNoise;
s32 Waste;
s8  PrevSweepIndex=-1;
u8  ResetFlag=0;
u16 ARBT_ARR;
u8  M_Factor=1;
u8  WaitForTrigFlag=0;
s16 Ch1TLevel=100;
s16 Ch2TLevel=100;
u8  SerialStatus;
u8  StartUart=0;
u32 USART1_DR=((u32)(USART1_BASE + 0x04));
u8  InitiateUartGen=0;
u8  OsTransferCompleteFlag;
u8  OsFFTData[2][700];
u8  AutoTrigIni=0;
u8  RMSdata[2][700];
u8  InvertA=0;
u8  InvertB=0;
u8  ScrollFlag=2;
u8  ErrorFlag=0;
u8  ResetEnableFFT=0;
u8  FastMode=1;
u8  ChartMode=0;
u8  OsChartFlag=0;
u8  ToggleName=0;
u8  DisableCursorTimer=0;
u16 DownConvert=0;
u32 FreqIN;
u8  DownConvertInitiate=0;
u32 FFTScale=0;
u8  DCbase;
u8  DCframe;
u8  DCmode;
u8  DCmeter;
u16 DCt1=0;
u8  FreqScaleShift;
u8  DownConvertShiftEnable=0;
u32 LfreqPeriod;
u16 PrecisionLevel=0;
u8  FFTt1Mode=0;

void Beeper(u16 ms);					//alarms
void AlternateChannel(void);				//for A&B mode
void SetOffset(u8 channel,u8 range, s16 Ypos);		//sets Ka3,Kb3 zero level reference		
void BatLevelCompensation(void);
s8   InterpolateS8(u8 Ch, s8 L8,s8 H8);			//for battery level comp
u16  InterpolateU16(u8 Ch, u16 L16,u16 H16);
s16  QParam(u8 Ch, u16 Position,u8 service);		//for Qerror
s32  QError(u8 Ch, u16 Start, u16 End, u32 Utmp);	//quantization error compensation
void Average(u8 Ch);
void RunAvg(u8 Ch);
s32  VRunAvg(u8 Ch, s32 Value, s32 Sum);
void cleardatabuf(u8 service);
s32  Log10Process(u16 i);
u8   TriggerModeLogic(void);
void SetIRQ2Priority(void);
void CalculateArbtTimer(void);
void UpdateTLevels(void);
void UpdateCursorMeter(void);
void CursorDisplaySelectLogic(void);

s16 WaveValue(u16 j);
u8  TriggerType=0;
u8  SubIndex=0;
s16 WindowPosition;

//u8  StartOffset=16;           //this displayed trigger point ~2 samples to left of trigger vernier on factory FPGA, ~4 on custom FPGA
u8  StartOffset=14;             //NOTE: 2 sample delay buffer now used for data aligns with trigger's 2 sample delay so need default of 16(?) 

u8  OsBufferLogicFlag=0;
u8  OSAvg=0;
u8 PrevSource=0xFF,PrevKind=0xFF;
u8  InvTrig[8]={1,0,3,2,6,7,4,5};
u8  HardwareVersion;


const SubTbase TbaseSub[13]={
   //  .Psc   .Arr  .S_OS_Size  .Ratio             
      {1-1,   2400-1,  100,   10},         //.1S
      {1-1,   4800-1,  100 ,   5},         //.2S
      {1-1,   8000-1,  150 ,   2},         //.5S  
      {1-1,   16000-1, 150 ,   1},         //1S
      {1-1,   32000-1, 150 ,   2},         //2S
      {1-1,   60000-1, 200 ,   5},         //5S  
      {10-1,  12000-1, 200 ,  10},         //10S 
      {10-1,  12000-1, 300 ,  15},         //15S 
      {10-1,  24000-1, 300 ,  30},         //30S   
      {10-1,  36000-1, 400 ,  60},         //60S 
      {100-1, 7200-1,  400 , 120},         //2M 
      {100-1, 12000-1, 600 , 300},         //5M 
      {100-1, 24000-1, 600 , 600}};        //10M 





const OSTbase TbaseOS[17]={
   //  .Psc     .Arr    
      {200-1,1500-1},          //1S
      {100-1,1500-1},          //500MS
      {40-1,1500-1},           //200MS
      {40-1,750-1},            //100MS  
      {40-1,375-1},            //50MS 
      {16-1,375-1},            //20MS
      {8-1,375-1},            //10MS 
      {4-1,375-1},             //5mS
      {4-1,150-1},             //2MS
      {2-1,150-1},             //1MS  
      {1-1,150-1},             //500uS
      {1-1,60-1},              //200uS
      {1-1,30-1},              //100uS  
      {1-1,15-1},              //50uS 
      {1-1,6-1},               //20uS
      {1-1,3-1},               //10uS
      {1-1,2-1}};              //5uS 


uc16 Wait[22]=        {1000, 500, 200, 100, 65, 30, 20, 10, 5, 3,   			
                       2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    			
                        2,      2};

uc16 shortwait[22]=        {600, 250, 100, 75, 35, 15, 10, 10, 5, 3,   	//for auto trig mode, set time to hold before going auto		
                        2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    			
                        2,      2 };


uc16 TrigDelayLoop[10]= {2,4,10,20,40,100,200,400,1000,2000};		//for trig holdoff, sets how many time loops before allowing rest of program to function

uc16 AltDelay[21]= {1000, 500, 300, 150, 75, 30, 15, 8, 4, 2};          //for alt trig mode, sets time to wait for trigger in other ch if untriggered

uc16 TimeBase[22]= {50000,50000,20000,10000,5000,2000,1000,500,200,100,50,20,10,5,2,1,1,1,1,1,1,1};

Y_attr *Y_Attr; 
X_attr *X_Attr; 
G_attr *G_Attr; 
T_attr *T_Attr;

//u32 DataBuf[4096];			//could save 4K of RAM here, using u16 + u8 struct
#warning Gabo:
u32 DataBuf[2048];

u8  TrackBuff  [397 * 4];         // curve track: i +0, i +1, i +2, i +3, respectively, placed one on the 4th track data

uc8 Log10Mant[100]={ 0, 0, 3, 5, 6, 7, 8, 8, 9, 10, 0, 4, 8,11,15,18,20,23,26,28,30,32,34,36,38,   //0-9: scale x10, 10-99: scale x100
                    40,41,43,45,46,48,49,51,52,53,54,56,57,58,59,60,61,62,63,64,65,66,67,68,69,
                    70,71,72,72,73,74,75,76,76,77,78,79,79,80,81,81,82,83,83,84,85,85,86,86,87,
                    88,88,89,89,90,90,91,91,92,92,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100};

uc8 Window[512]=       //Hann window
         {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,
          2,  2,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,  7,  7,  8,  8,  8,
          9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 17, 17, 18, 18, 19, 
         20, 20, 21, 21, 22, 23, 23, 24, 25, 25, 26, 27, 28, 28, 29, 30, 30, 31, 32, 33,
         33, 34, 35, 36, 37, 37, 38, 39, 40, 41, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49,
         50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 61, 61, 62, 63, 64, 65, 66, 67,
         68, 69, 70, 71, 72, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 84, 85, 
         86, 87, 88, 89, 90, 91, 92, 93, 93, 94, 95, 96, 97, 98, 99,100,101,101,102,103,
        104,105,106,106,107,108,109,110,111,111,112,113,114,115,115,116,117,118,118,119,
        120,121,121,122,123,124,124,125,126,126,127,128,128,129,130,130,131,131,132,133,
        133,134,134,135,135,136,137,137,138,138,139,139,140,140,140,141,141,142,142,143,
        143,143,144,144,144,145,145,145,146,146,146,147,147,147,147,148,148,148,148,148,
        149,149,149,149,149,149,149,150,150,150,150,150,150,150,150,150,150,150,150,150,
        150,150,150,150,150,149,149,149,149,149,149,149,148,148,148,148,148,147,147,147,
        147,146,146,146,145,145,145,144,144,144,143,143,143,142,142,141,141,140,140,140,
        139,139,138,138,137,137,136,135,135,134,134,133,133,132,131,131,130,130,129,128,
        128,127,126,126,125,124,124,123,122,121,121,120,119,118,118,117,116,115,115,114,
        113,112,111,111,110,109,108,107,106,106,105,104,103,102,101,101,100, 99, 98, 97,
         96, 95, 94, 93, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 84, 83, 82, 81, 80, 79,
         78, 77, 76, 75, 74, 73, 72, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 61,
         60, 59, 58, 57, 56, 55, 54, 53, 53, 52, 51, 50, 49, 48, 47, 46, 46, 45, 44, 43,
         42, 41, 41, 40, 39, 38, 37, 37, 36, 35, 34, 33, 33, 32, 31, 30, 30, 29, 28, 28,
         27, 26, 25, 25, 24, 23, 23, 22, 21, 21, 20, 20, 19, 18, 18, 17, 17, 16, 15, 15,
         14, 14, 13, 13, 12, 12, 11, 11, 10, 10,  9,  9,  8,  8,  8,  7,  7,  6,  6,  6,
          5,  5,  5,  4,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,
          1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}; 

 s8  Ka1[10], Kb1[10]; 
 u16 Ka2[10], Kb2[10]; 
 s8  Ka3[10], Kb3[10]; 
 s8  HKa1[10], HKb1[10]; 
 u16 HKa2[10], HKb2[10]; 
 s8  HKa3[10], HKb3[10]; 
 s8  LKa1[10], LKb1[10]; 
 u16 LKa2[10], LKb2[10]; 
 s8  LKa3[10], LKb3[10]; 

u16 HiBatLevel[2]={0,0};		//high battery correction reference level
u16 LoBatLevel[2]={0,0};               //low battery correction reference level
u16 PrevBatLevel=0;		
u16 VDiff[2]={0,0};

D_tab D_Tab[23] ={  // pulse waveform output driver table, based on the 72MHz frequency
//    PSC     ARR       
  { 2400-1,30000-1}, //1  0 
  { 1200-1,30000-1}, //2  1 
  { 480-1, 30000-1}, //5  2 
  { 240-1, 30000-1}, //10  3
  { 120-1, 30000-1},  //20  4
  { 48-1,  30000-1},  //50  5
  { 24-1,  30000-1},  //100  6
  { 12-1,  30000-1},  //200   7
  { 5-1,   28800-1},  //500   8
  { 2-1,   36000-1},  //1K    9
  { 1-1,   36000-1},  //2K    10
  { 1-1,   14400-1},   //5K   11 
  { 1-1,    7200-1},   //10K   12
  { 1-1,    3600-1},   //20K    13
  { 1-1,    1440-1},    //50K   14  
  { 1-1,     720-1},    //100K  15  
  { 1-1,     360-1},    //200K  16  
  { 1-1,     144-1},    //500K  17  
  { 1-1,      72-1},
  { 1-1,      36-1},
  { 1-1,      18-1},
  { 1-1,      12-1},
  { 1-1,       9-1}};   //      22

                                                     //previous versions used 1.8Mhz sampling rate for 25Khz range (@ 72samples/period) 

uc32 A_Freq[18]={1,2,5,10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000};

uc8 ScaleIndex[17]={1,1,1,1,1,1,1,1,1,1,1,2,4,10,20,40,80};   //for max DAC sampling rate of 1.8Mhz(DAC is only rated to 1Mhz but seems OK @ 1.8Mhz)
uc16 A_Tab[17]={
    50000-1,                //1Hz uses 36Mhz clock, rest 72Mhz                 	
    50000-1,
    20000-1,
    10000-1,
     5000-1,                //all except noted @ 720 samples/period      (=<1.44Mhz sampling rate)	
     2000-1,
     1000-1,
      500-1,
      200-1,                //500Hz 
      100-1,                //1Khz 
       50-1,                //2Khz 
       40-1,                //5Khz  @ 360 samples/period   (1.8Mhz sampling rate)
       40-1,                //10Khz @ 180 samples/period   (1.8Mhz sampling rate)
       50-1,                //20Khz @ 72 samples/period	   (1.44Mhz sampling rate)
       40-1,                //50Khz @ 36 samples/period    (1.8Mhz sampling rate)    14
       40-1,                //100Khz@ 18 samples/period    (1.8Mhz sampling rate)
       40-1};               //200Khz@  9 samples/period    (1.8Mhz sampling rate)


s16   ATT_DATA[720];    //could go up to 2880 to allow long arb digital streams

sc16 SIN_QUAD[181]={    2048 , 2066 , 2084 , 2102 , 2119 , 2137 , 2155 , 2173 , 2191 , 2209 , 2226 ,
			2244 , 2262 , 2280 , 2297 , 2315 , 2333 , 2351 , 2368 , 2386 , 2403 , 2421 ,
			2439 , 2456 , 2474 , 2491 , 2508 , 2526 , 2543 , 2561 , 2578 , 2595 , 2612 ,
			2629 , 2646 , 2664 , 2681 , 2698 , 2714 , 2731 , 2748 , 2765 , 2782 , 2798 ,
			2815 , 2831 , 2848 , 2864 , 2881 , 2897 , 2913 , 2929 , 2945 , 2961 , 2977 ,
 			2993 , 3009 , 3025 , 3040 , 3056 , 3072 , 3087 , 3102 , 3118 , 3133 , 3148 ,
 			3163 , 3178 , 3193 , 3207 , 3222 , 3237 , 3251 , 3266 , 3280 , 3294 , 3308 ,
 			3322 , 3336 , 3350 , 3364 , 3377 , 3391 , 3404 , 3418 , 3431 , 3444 , 3457 ,
 			3470 , 3483 , 3495 , 3508 , 3520 , 3533 , 3545 , 3557 , 3569 , 3581 , 3593 ,
 			3605 , 3616 , 3628 , 3639 , 3650 , 3661 , 3672 , 3683 , 3693 , 3704 , 3714 ,
 			3725 , 3735 , 3745 , 3755 , 3765 , 3774 , 3784 , 3793 , 3803 , 3812 , 3821 ,	
			3830 , 3838 , 3847 , 3855 , 3864 , 3872 , 3880 , 3888 , 3896 , 3903 , 3911 ,
			3918 , 3925 , 3932 , 3939 , 3946 , 3953 , 3959 , 3965 , 3972 , 3978 , 3983 ,
 			3989 , 3995 , 4000 , 4006 , 4011 , 4016 , 4021 , 4025 , 4030 , 4034 , 4038 ,
 			4043 , 4046 , 4050 , 4054 , 4057 , 4061 , 4064 , 4067 , 4070 , 4073 , 4075 ,
 			4077 , 4080 , 4082 , 4084 , 4086 , 4087 , 4089 , 4090 , 4091 , 4092 , 4093 ,
			4094 , 4094 , 4095 , 4095 , 4095  };

sc16 Sine100K[18]=                                                                                                                   //         
  {0x9C0,0xC59,0xE68,0xFAB,          
   0xFFF,0xF70,0xDF5,0xBBD,0x90E,    
   0x63E,0x3A5,0x196,0x053,
   0x000,0x08E,0x209,0x441,0x6F0};         

sc16 Sine200K[9]=                    
  {0xC59,0xFAB,                      
   0xF70,0xBBD,                      
   0x63E,0x196,
   0x000,0x209,0x6F0};                     

sc16 Triangle100K[18] =              
  {0x8E2,0xAA9,0xC70,0xE37,          
   0xFFE,0xE37,0xC70,0xAA9,0x8E2,    
   0x71B,0x554,0x38D,0x1C6,
   0x000,0x1C6,0x38D,0x554,0x71B};         

sc16 Triangle200K[9] =               
  {0xAA9,0xE37,                      
   0xE37,0xAA9,                      
   0x71B,0x38D,
   0x000,0x38D,0x71B};                     

sc16 DIGI_DATA[2] =  // Square wave data                                                                                             //         
  {2047,-2048};    

void BackGround_Reset(u8 clear)
{ 
  u8 i, j;
  u8 TitleLimit=13;
  if ((FlagMeter==2)&&(UpdateMeter!=4))TitleLimit=11;
  if(clear)__Clear_Screen(BLACK);
  Delayms(100); 
  for(i=0; i<TitleLimit; i++) for(j=0; j<4; j++) Title[i][j].Flag |= UPDAT;

  for(i=0; i<9; i++)                     Meter[i].Flag    |= UPDAT;
  if(Current != FILE) {
    Title[FILE][0].Flag &= !UPDAT;
    Title[FILE][1].Flag &= !UPDAT;
    Title[FILE][3].Flag &= !UPDAT;
  }  
   Update = 1;                  // return back the jumper settings
   ClearMeterAreaFlag=1;
   InitXY=1;
}

/*******************************************************************************
 App_init: Displays the window waveform data initialization
*******************************************************************************/
void App_init(u8 ClearScreen)
{ 
  __Set(ADC_CTRL, EN );       
  __Set(ADC_MODE, SEPARATE);
  __Set(STANDBY, DN);          // exit the power saving state
  Delayms(20); 
  __Set(FIFO_CLR, W_PTR); 
  BackGround_Reset(ClearScreen);
  PD_Cnt = 600;
}

/*******************************************************************************
 Update_Range: 
*******************************************************************************/
void Update_Range(void) 
{
u8 Amode=Title[TRACK1][COUPLE].Value;
u8 Bmode=Title[TRACK2][COUPLE].Value;
if (Amode==2){Amode--;Det|=1;}else Det&=2;              //use AC for coupling in detector mode
if (Bmode==2){Bmode--;Det|=2;}else Det&=1;
if (Amode>2)Amode=0;                                    //DC coupling for serial decode
if (Bmode>2)Bmode=0;                                   

  __Set(CH_A_COUPLE, Amode);
  __Set(CH_A_RANGE,  Title[TRACK1][RANGE].Value);
  SetOffset(0,_A_Range,_1_posi);
  __Set(CH_B_COUPLE, Bmode);
  __Set(CH_B_RANGE,  Title[TRACK2][RANGE].Value);                            
  SetOffset(1,_B_Range,_2_posi);
  if(SerialStatus)UpdateTLevels();
  if((Sweep)||(Det)){
    Update_Output();
    if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
  }

}
/*******************************************************************************
 Update_Base: 
*******************************************************************************/
void Update_Base(void) 
{
  u16 i,Tmp;
  
  if(DownConvertMode)return;
  __Set(ADC_CTRL, EN);
  if(ChartLogic()){

    __Set(T_BASE_PSC, TbaseSub[SubIndex-1].Psc);
    __Set(T_BASE_ARR, TbaseSub[SubIndex-1].Arr);
    __Set_Param(8,0x96);                         
    __Set_Param(9,0x00);                                         //normal 150 sample pre-sampling depth
  }else{
    i = Title[T_BASE][BASE].Value;     

    if(OSBufferLogic()){                             
        if(_T_base<16) Tmp=(1208-(_X_posi.Value*8))+(StartOffset*8);
          else Tmp=(908-(_X_posi.Value*6))+(StartOffset*6);
        __Set_Param(8,(Tmp&0xFF));                                 //stabilize window position and shift trigger point to match xpos rather
        __Set_Param(9,((Tmp>>8)&0xFF));                            //than shifting window to keep buffer size at 512 (X8= 4096)  
        __Set(T_BASE_PSC,TbaseOS[i].Psc);
        __Set(T_BASE_ARR,TbaseOS[i].Arr);                                         
    }else{
      __Set_Param(8,0x96);                         
      __Set_Param(9,0x00);                                         //normal 150 sample pre-sampling depth
      __Set(T_BASE_PSC, X_Attr[i].PSC);
      __Set(T_BASE_ARR, X_Attr[i].ARR);
    } 

    SubIndex=0;                                                    //reset extended time base if moving out of SCAN mode                                   
  } 
  Wait_Cnt = Wait[_T_base];
  if(_Mode!=SCAN){
    if(_Status == RUN) __Set(FIFO_CLR, W_PTR);                     // FIFO write pointer reset
    if((_T_base>16)||(OSBufferLogic())){if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);}
    JumpCnt=0;
  }
}

void FFTdownConvert(u8 service){

static u32 Divisor,WindowShiftFactor,AdjDivisor,Base,Fsamp=0,RefFreq,AdjBase;
static u16 FFTorderADJ;
static s8  i;
s8 h;
u32 Adj,Tmp=0,Tmp2,j;
u16 AdjustedARR=0;
u32 PreScale=0;

     if(((FreqIN==0)&&(FFTt1Mode==0))||(DownConvertMode==0))return;
     if(service==4){
       if(FreqIN>5000000)DownConvertShiftEnable=60;
       else if(FreqIN>1500000)DownConvertShiftEnable=30;
       else DownConvertShiftEnable=15;
       service=0;
     }         
     if(service==3)goto ReadFreqMeter;                                            //meter reads
     else if((service)||(DownConvertShiftEnable==2))goto ShiftWindow;             //window shifts       

     while(1){

       if(DownConvertInitiate){
         if(FFTt1Mode){                                                           //"T1" mode 
           Tmp=(NFreq/FFTBins)*_T1;
           if(_T_base<12)Tmp/=1000;
           _T1=128;
           DownConvert=1;                                                         //reset to base magnification 
         }else{
           Tmp=FreqIN;
           for(i=0;i<(FreqScaleShift);i++)Tmp/=10;                                //establish proper frequency from prior scaling
         }
         if(Tmp<215){
          DownConvertMode=0; 
          DownConvertRestore();
          return; 
         }
       }else Tmp=RefFreq;
       RefFreq=Tmp;

       WindowShiftFactor=((90000000/Tmp)+11)/22;
       if(WindowShiftFactor==0)WindowShiftFactor=1;   
       Adj=DownConvert*125;                                                       //shift halfway up sub-order 
       Adj-=(Adj%100);                                                            //remove offset to obtain sub-order base
       FFTorderADJ=Adj/100;                                                       //store unscaled base order
       Adj+=25;                                                                   //restore offset
       Divisor=((7200*Adj)/RefFreq)*100;                                          //calculate division ratio
       Divisor+=(((((7200*Adj)%RefFreq)*100)+(RefFreq/2))/RefFreq);               //add and round last 2 significant digits of precision

       if(Divisor>429495){                                                        //limits slowest screen refresh to approx 2 to 3 seconds
         switch(DownConvert){                                                     //and reasonably accurate <1Hz calculations
           case   5:
           case  50:
           case 500:
             DownConvert/=5;
             DownConvert*=2;
             break;
           default:
             if(DownConvert>1)DownConvert/=2;else return;
         }
       }else break;
     }

ShiftWindow:
     if(service==1)Divisor+=WindowShiftFactor;
     else if(service==2)Divisor-=WindowShiftFactor;

     if(Divisor<1200)_T_base=12;else _T_base=7;                                   //allows proper selection of buffer aquisition modes
     __Set_Param(8,0x96);                         
     __Set_Param(9,0x00);                                                         //normal 150 sample pre-sampling depth

    if(Divisor<61356){                                      //setup prescaling
      PreScale=1;
      AdjustedARR=Divisor;
    }else{
      PreScale=(Divisor/61356)+1;                         //try to find integral prescale
      while((Divisor%PreScale)!=0){                      
        PreScale++;                                       
      }
      if(PreScale<61356){                                   //if integral ratios found
        AdjustedARR=(Divisor/PreScale);
      }else{                                                //if not keep prescale small and use rounded ARR
        i=1;
        for(j=122712;j<429496;j+=61356){
          i++;
          if(Divisor<j){
            PreScale=i;
            AdjustedARR=((Divisor+(i/2))/i);
            break;
          }
        }
      }
    }
    __Set(T_BASE_PSC,PreScale-1); 
    __Set(T_BASE_ARR,AdjustedARR-1);
    __Set(FIFO_CLR, W_PTR);                                                   
    JumpCnt=0;

    AdjDivisor=PreScale*AdjustedARR;                                                  
    if(((72000000/AdjDivisor)*FFTorderADJ)>1000000){                                                                           
      Fsamp=((72000000/AdjDivisor)*100);                                                      //calculate downconverted sampling base 
      Tmp=((((72000000%AdjDivisor)*10000)/AdjDivisor)*FFTorderADJ)/100;                       //x10000 correction factor
      Base=(Fsamp*FFTorderADJ)+Tmp;                                                           //actual frequency at base
      i=-1;
    }else{
      Fsamp=((72000000/AdjDivisor)*1000);   
      Tmp=((((72000000%AdjDivisor)*10000)/AdjDivisor)*FFTorderADJ)/10;                   
      Base=(Fsamp*FFTorderADJ)+Tmp;                                                
      i=0;
    }
    AdjBase=ClockAdjust(Base);                                                                //adjust with CPU clock frequency correction

ReadFreqMeter:
  if(service==3){                                                                             //offset from sub-order fold
    if(i==-1)Tmp2=(((LfreqPeriod/(Power(10,(ShiftDigits+1))))+(AdjDivisor/2))/AdjDivisor)*72;
    else Tmp2=(((LfreqPeriod+(AdjDivisor/2))/AdjDivisor)*72)/(Power(10,(ShiftDigits)));             

    if(AdjBase<1000000){Tmp=10;h=i+1;}else{Tmp=1;h=i;}
    Int2Str(BaseFreqStr,(AdjBase*Tmp), F_UNITSUB , 7, UNSIGN,h);
    Int2Str(PeakFreqStr,((AdjBase+Tmp2)*Tmp), F_UNITSUB , 7, UNSIGN,h);
    Int2Str(NFreqStr,((AdjBase+(Fsamp>>1))*Tmp), F_UNITSUB , 7, UNSIGN,h);
    if(Fsamp>10000000)j=((AdjBase+((((Fsamp>>1)+128)/FFTBins)*_T1))*Tmp);
      else j=(( AdjBase+(((Fsamp*_T1)+256)/512))*Tmp);
    Int2Str(FreqT1Str,j,F_UNITSUB, 7, UNSIGN,h);

    if((Fsamp*15)>286331000)Tmp=(((Fsamp+(FFTBins/2))/FFTBins)*15);
    else Tmp=((Fsamp*15)+(FFTBins/2))/FFTBins;     
    if(Tmp<1000){Tmp*=10000;j=4;}
    else if(Tmp<10000){Tmp*=1000;j=3;}
    else if(Tmp<100000){Tmp*=100;j=2;}
    else if(Tmp<1000000){Tmp*=10;j=1;}else j=0;
    Int2Str(FreqDivStr,Tmp,F_UNITSUB, 7, UNSIGN,j+i);

    if(i==-1)Tmp=RefFreq*100;else Tmp=RefFreq*1000;

    if((imax>20)&&(imax<215)&&(Base<Tmp)&&(Tmp<(Base+(Fsamp>>1))))Tmp=(Base+Tmp2);
    if((Base>Tmp)||(Tmp>(Base+(Fsamp>>1))))
      {for(j=0;j<10;j++)PeakFreqStr[j]='-';PeakFreqStr[10]=0;}

    if(FFTt1Mode==0){                                                     
      if(i==-1)RefFreq=Tmp/100;else RefFreq=Tmp/1000;
    }

  }

  if(FFTt1Mode){                                                                        
    if(((DownConvert>PrecisionLevel)&&(service==0))
    ||((DownConvert>=PrecisionLevel)&&((service==1)||(service==2)))){          
      if(service==0)PrecisionLevel=DownConvert;
      if((service==1)||(service==2))_T1=128;                                   //reset T1 to middle of screen if shifting window  
      if(i==-1)RefFreq=(Base+(((Fsamp>>1)/FFTBins)*_T1))/100;                                  
      else RefFreq=(Base+(((Fsamp>>1)/FFTBins)*_T1))/1000;
    }
  }

}



/*******************************************************************************
 Update_Output: 
*******************************************************************************/
void Update_Output(void) {
u16 i,BaudDiv;

  if((StartUart)&&(_Kind==8)){                      //transmit uart data
    DMA1_Channel4->CCR    &=~DMA_CCR1_EN;           //disable usart1 dma access
    USART1->SR            &=0xFFBF;                 //clear bit 6 (transmission complete bit, TC)
    DMA1_Channel4->CNDTR   =UartFileSize;           //number of bytes to load                         
    DMA1_Channel4->CCR    |=0x00000001;             //enable                                                          
    StartUart=0;
    return;
  }else if(_Kind==8){        
    if(InitiateUartGen==0)InitiateUartGen=1;else goto AdjUartGen;
  }else InitiateUartGen=0;

  if(_Frqn==0)TIM7->PSC=1; else TIM7->PSC=0;        //set DAC clock to 36Mhz for 1Hz to avoid ARR overload, 72Mhz for rest
  SetIRQ2Priority();
  TIM_2IRQControl();
  if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
  M_Factor=1;

  TIM4->CR1   =  0x0080;            // SQR_OUT = Disable. Stops counter 0x80=auto reload preload enable
  TIM4->DIER  =  0x0000;            // disable TIM4_INT  
  TIM4->CR2   =  0x0000;            // disable update event as output

  AFIO->MAPR    =0x04000000;        //reset remaped PB6 alt function                                          
  USART1->CR1   =0x0000;            //disable usart1   
  RCC->APB2ENR &=0xFFFFBFFF;        //disable usart1 clock
  DMA1_Channel4->CCR &=~DMA_CCR1_EN;//disable usart1 dma access

  if(ResetFlag){
    TIM4->EGR=0x0001;               // generate update event for full reset 
    TIM2->EGR=0x0001;                 
    ResetFlag--;
  }
  DAC->CR     =  0x00000000;        // reset both channels   

  if(_Kind == PWM){                        //(4)

    if(Sweep==1){                               //if in sweep mode, initialize analog as well as it uses this along with PWM
      Title[OUTPUT][DUTYPWM].Value=15000;
      DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      for(i=0;i<720;i++)ATT_DATA[i]=2048;
      ResetDMA2_Ch4Params(72);
      __Set(ANALOG_ARR, 40-1);
    }
    __Set(DIGTAL_PSC, D_Tab[_Frqn].PSC);
    __Set(DIGTAL_ARR, D_Tab[_Frqn].ARR);

    i=((D_Tab[_Frqn].ARR+1)*(30000-Title[OUTPUT][DUTYPWM].Value))/30000;
    if(i==0)i++;
    if(i>(D_Tab[_Frqn].ARR+1))i=(D_Tab[_Frqn].ARR+1);

    if((Sweep==1)||(PWAdjustMode==0)){
      __Set(DIGTAL_CCR,i);
    }else{
        GPIOB->CRL &= 0xF0FFFFFF;  
        GPIOB->CRL |= 0x0B000000;
        TIM4->CR1 = 0x0081;
      }
     TIM4->EGR=0x0001;                 //reset

  }else if(_Kind==5){                  // random noise mode

    if(_Frqn>18)_Frqn=18;              // DAC refresh limit, 1Mhz 
    InitiateNoise=1;
    if(PrevSweepIndex<0)PrevSweepIndex=SweepIndex;
    DMA2_Channel4->CCR &=0xFFFFFFFE;   // release DMA channel from DAC 
    DMA1_Channel4->CCR    &= ~DMA_CCR1_EN;          //make sure ch is off (same as ccrX)
    GPIOB->CRL  = 0x34BBB438;          // puts PWM port(TIM4/PB6) in input, floating mode + configures other bits  
    GPIOA->CRL  = 0x110011BB;          // puts DAC2 port (PA5) in input, analog mode

    if(_Frqn<8)i=_Frqn+6;              // 100x averaging filter @ <500hz
      else if(_Frqn<14)i=_Frqn+3;      // 10x filter for 500hz - 20Khz
        else i=_Frqn;                  // above 20Khz no filtering
    TIM4->PSC   = D_Tab[i].PSC;        // oversampling setup                 
    TIM4->ARR   = D_Tab[i].ARR;      

    if(_Frqn<16){
      TIM4->DIER|=0x0001;              // enable TIM4_INT, engages seeding @ <200Khz and filters for frequencies <=20Khz
    }else{
      TIM4->DIER=0x0000;               // seeding by TIM2_INT for freq limits > 100Khz 
      SweepIndex=_Frqn-13;                     
    }
    TIM_2IRQControl();

    TIM4->CR2   = 0x0020;              // use update event as output
    TIM4->CR1   = 0x0085;              // turn on DAC2 clock, preload arr, update only on overflow 

    if(_Frqn<14){
      DAC->CR       = 0x0B6C002C;      // set up both DACs to trigger from TIM4 update event: DAC2=generator > filter > DAC1=output
      DAC->CR      |= 0x00010001;      // enable both channels   
    }else{                             // above 20Khz
      DAC->CR       = 0x00000B6C;      // set up DAC1 for generator and output
      DAC->CR      |= 0x00000001;      // enable DAC1   
    }
    return;

  }else if(_Kind==6){                  // generator turned off    DAC, PWM and TIM4_INT already shut down at top of function
                                       // shut down additional resources + set output low
    DMA2_Channel4->CCR&= 0xFFFFFFFE;   // release DMA channel from DAC 
    TIM7->CR1          = 0x0084;       // shut off DAC timer
    GPIOA->CRL        |= 0x00040000;   // disconnect DAC if connected
    GPIOB->CRL        &= 0xF0FFFFFF;   // reset bit 6 control
    GPIOB->CRL        |= 0x03000000;   // enable bit 6 as output
    GPIOB->ODR        &= 0x0000FFBF;   // reset bit 6 to set gen output low
    if(PrevSweepIndex<0)PrevSweepIndex=SweepIndex;
    return; 

  }else if(_Kind==7){                  //arbitrary waveforms

     while((ArbtSampleNumber*A_Freq[_Frqn])>1800000)_Frqn--;          //limit frequency range so sampling rate is <1.8Mhz
     Title[OUTPUT][1].Flag|= UPDAT;
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
     ResetDMA2_Ch4Params(ArbtSampleNumber);
     CalculateArbtTimer();

  }else if(_Kind==8){                 //UART OUTPUT

     GPIOA->CRL  &=0xFFF0FFFF;        //reset PA4 control
     GPIOA->CRL  |=0x00080000;        //PA4 (DAC1 port) set to input pull up/down mode 
     GPIOA->ODR  |=0x00000010;        //pull up port so idle line stays high through external connection with PB6

     TIM4->PSC = 2400-1;              //Generate update event for full reset, sets disabled TIM4 output low.    
     __Set(DIGTAL_ARR, 3000-1);       //Remapped alternate function appears to be logically or'd with default
     TIM4->CCR1 = 3000-1;             //alternate function, so default needs to be at 0 level or it will shut
     TIM4->EGR=0x0001;                //remapped off. Could not find this defined in STM32 programming reference...     

     AFIO->MAPR   =0x04000004;        //remap PB6 alt function to USART1 TX
     RCC->APB2ENR |=0x00004000;       //enable usart1 clock 
     GPIOB->CRL  &=0xF0FFFFFF;        //reset PB6 control
     GPIOB->CRL  |=0x09000000;        //PB6 set to alt function, B=50Mhz 9=10Mhz push-pull output                            

AdjUartGen:
     switch(GenUartMode){
       case 1:
         USART1->CR1  =0x2408;            //uart1 enable, 7Pe 
         break;
       case 2:
         USART1->CR1  =0x2608;            //uart1 enable, 7Po 
         break;
       case 3:
         USART1->CR1  =0x2008;            //uart1 enable, 8N 
         break;
       case 4:
         USART1->CR1  =0x3408;            //uart1 enable, 8Pe 
         break;
       case 5:
         USART1->CR1  =0x3608;            //uart1 enable, 8Po
         break;
       case 6:
         USART1->CR1  =0x3008;            //uart1 enable, 9N 
     }

     if(GenUartStopBits==0) 
       USART1->CR2  =0x0000;                    //1 stop bit, clock disabled
         else USART1->CR2  =0x2000;             //2 stop bits
     USART1->CR3  =0x0088;                      //enable DMA for transmit, half duplex (mode allows remap of PB6 only)

     if(GenBaudIndex<8485){                     //below 8485 divider =  index for baud rates 8485 to 4.5M
       BaudDiv=GenBaudIndex;
       GenBaudRate=72000000/GenBaudIndex; 
     }else{                                     //above 8485 baud rate 1:1 ratio with index, baud rate calculated for all integers down to 1099
       GenBaudRate=((15871-GenBaudIndex)+1099);
       BaudDiv=72000000/GenBaudRate;
     }     
     USART1->BRR  =BaudDiv;            
     USART1->SR  &=0xFFBF;                           //clear bit 6 (transmission complete bit, TC)

     DMA1_Channel4->CCR    &=~DMA_CCR1_EN;           //disable usart1 dma access
     DMA1_Channel4->CPAR    =(u32)USART1_DR;         //usart data register address 
     DMA1_Channel4->CMAR    =(u32)SecBuff;           //file data                                
     DMA1_Channel4->CNDTR   =0;                      //only initialize                         
     DMA1_Channel4->CCR     =0x00003000;             //priority 

     if(GenUartCont){
       DMA1_Channel4->CCR  |=0x000000B0;             //setup   B > circular mode
     }else{
       DMA1_Channel4->CCR  |=0x00000090;             //setup   9 > single mode        
     } 
     DMA1_Channel4->CCR    |=0x00000001;             //enable                                                          
                                          
    return;                           

  }else{

     if(_Frqn>16)_Frqn=16;
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;                          
     WaveGen();
     ResetDMA2_Ch4Params(720/ScaleIndex[_Frqn]);
     __Set(ANALOG_ARR, A_Tab[_Frqn]);
  }

  if(PrevSweepIndex>=0){SweepIndex=PrevSweepIndex;PrevSweepIndex=-1;}
  if(GenAdjustMode==0){GenFreqShift=0;PrevShift=0;}

  OutputAdjust();

}


void ResetDMA2_Ch4Params(u32 Count){          //This is an attempt to fix a *very* occasional loss of this DMA function while quickly
  DMA2_Channel4->CPAR=DHR12R1_DAC;            //shifting some gen menu functions by holding down repeat keys, requiring reboot. All relevant parms now reset 
  __Set(ANALOG_PTR, (u32)ATT_DATA);           //DMA ch needs to be disabled before calling
  __Set(ANALOG_CNT, Count);
  DMA2_Channel4->CCR =0x00003000;             //priority
  DMA2_Channel4->CCR|=0x000005B0;             //setup
  DMA2_Channel4->CCR|=0x00000002;             //turn transfer complete int on
  DMA2_Channel4->CCR|=0x00000001;             //enable
  DMA2->IFCR|=0x00002000;                     //clear transfer complete int flag
  TIM7->DIER=0x0100;                          //enable DMA call
  TIM7->CR2 =0x0000;                          //system (V1.50-1.64, others?) sets bit 3 here for DMA call on update event, no such selection on basic timers
}                                             //TIM 7 does not have capture compare mode, so DMA call is always on update event


void CalculateArbtTimer(void){
u32 X;
u16 Y;

  X=72000000/(ArbtSampleNumber*A_Freq[_Frqn]);
  if(X<10000){
    TIM7->PSC=0;
    ARBT_ARR=X-1;    
  }else{
    Y=X/10000;
    while((X%Y)!=0)Y++;          //  find an integral divider
    if(Y>(X/Y)){                 //  if divider larger than divided, use as ARR so freq shift function can work correctly
      TIM7->PSC=(X/Y)-1;             
      ARBT_ARR=(Y-1);             
    }else{
      TIM7->PSC=(Y-1);    
      ARBT_ARR=(X/Y)-1;          
    } 
  }
  __Set(ANALOG_ARR,ARBT_ARR);
}

/*******************************************************************************
 Update_Trig: 
*******************************************************************************/
void Update_Trig(u8 service) 
{ 
u16 a;
u16 b;
u32 UTmp;

  AutoTrigIni=1;
  TriggerType=_Tr_kind;

  a=((TempKp1*_T1)+512)/1024;           //compensate for interpolated ranges
  b=((TempKp1*_T2)+512)/1024;
  if(OSBufferLogic()){if(_T_base<16) {a*=8;b*=8;}else{a*=6;b*=6;}}                                                                   
                                      
   if(_Tr_kind==8){
     if((Sweep)&&(_Kind<5)){
       UTmp=((1000/((TIM2Speed[SweepIndex]*SweepStep*5)/SweepMod))*750)/TimeBase[_T_base];    //calculate optimum period to trigger on
       if(UTmp>200)UTmp=200;
       if(OSBufferLogic()){if(_T_base<16)UTmp*=8;else UTmp*=6;}                                                                   
       __Set(T_THRESHOLD,UTmp);          //bits 0-15

       if(service){       
         if(Title[TRIGG][SOURCE].Value == TRACK1){
           if(_Vt1>_1_posi)TriggerType=5;else TriggerType=7;  
         }
         if(Title[TRIGG][SOURCE].Value == TRACK2){ 
           if(_Vt2>_2_posi)TriggerType=5;else TriggerType=7;  
         } 
       }else{
         if(TrigSourceEnable == TRACK1){                                  
           if(_Vt1>_1_posi)TriggerType=5;else TriggerType=7;  
         }
         if(TrigSourceEnable == TRACK2){ 
           if(_Vt2>_2_posi)TriggerType=5;else TriggerType=7;  
         } 
       }

       if((_Kind==PWM)&&((Sweep>1)||(Det==0)))TriggerType=5;                     

     }else{
       TriggerType=3;                                                                //trigger on positive transition of continuous wave
    }

   }else{
     if(_T1 > _T2)  __Set(T_THRESHOLD,a-b);    
     else           __Set(T_THRESHOLD,b-a); 	 
   }
                                                                                                 
   if((InvertA)&&((Title[TRIGG][SOURCE].Value==0)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==0)))){
     a=((((-(_Vt1+Ka1[_A_Range]-_1_posi))*1024)/Ka2[_A_Range])&0xFF)+ADCoffset+_1_posi;        
   }else{
     a=((((_Vt1-Ka1[_A_Range]-_1_posi)*1024)/Ka2[_A_Range])&0xFF)+ADCoffset+_1_posi;    
   }

   if((InvertB)&&((Title[TRIGG][SOURCE].Value==1)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==1)))){
     b=((((-(_Vt2+Kb1[_B_Range]-_2_posi))*1024)/Kb2[_B_Range])&0xFF)+ADCoffset+_2_posi;   
   }else{
     b=((((_Vt2-Kb1[_B_Range]-_2_posi)*1024)/Kb2[_B_Range])&0xFF)+ADCoffset+_2_posi;   
   }

   UpdateTLevels();

 if(TriggerType>7)TriggerType=3;               
                                                          
 if((_T_base>18)||(OSBufferLogic())){             //use time based triggering for OS and fastest timebases rather than edge or level trig
   if((TriggerType==0)||(TriggerType==2)){        //more reliable for random, occasional triggers or noisy OS waveforms
     TriggerType=7;                               //triggers if time exceeds threshold 
     __Set(T_THRESHOLD,0);                        //setting time to exceed at 0 turns these into an edge or level trigger
   }
   if((TriggerType==1)||(TriggerType==3)){
     TriggerType=5;
     __Set(T_THRESHOLD,0);                        //setting time to exceed at 0 turns these into an edge or level trigger
   }
 }

 if(((Title[TRIGG][SOURCE].Value==0)&&(InvertA))||((Title[TRIGG][SOURCE].Value==1)&&(InvertB))||
   ((Title[TRIGG][SOURCE].Value==4)&&(((TrigSourceEnable==0)&&(InvertA))||((TrigSourceEnable==1)&&(InvertB)))))
     TriggerType=InvTrig[TriggerType];

 if(service){                                     //in regular trigger modes

  if(  (((PrevSource!=_Tr_source)||(PrevKind!=TriggerType)))||(!OSBufferLogic())||(service==2))//does not reset trig mode in OS mode except when changing
    __Set(TRIGG_MODE,(_Tr_source<< 3)+TriggerType);
  PrevSource=_Tr_source; PrevKind=TriggerType;

  if(Title[TRIGG][SOURCE].Value == TRACK1){ 
  __Set(V_THRESHOLD,a); 
  }
  if(Title[TRIGG][SOURCE].Value == TRACK2){ 
  __Set(V_THRESHOLD,b); 
  }
  if(OSBufferLogic()){
      if(_T_base<16) UTmp=(1208-(_X_posi.Value*8))+(StartOffset*8);
        else UTmp=(908-(_X_posi.Value*6))+(StartOffset*6);
    __Set_Param(8,(UTmp&0xFF));                               //stabilize window position and shift trigger point to match xpos rather
    __Set_Param(9,((UTmp>>8)&0xFF));                          //than shifting window to keep buffer size at 512 (X8= 4096)  
  }
 }else{                                                       //in A&B trigger mode  
  __Set(TRIGG_MODE,  (TrigSourceEnable<< 3)+TriggerType);     //TRIGG_MODE = 
  if(TrigSourceEnable == TRACK1){ 
  __Set(V_THRESHOLD,a); 
  }
  if(TrigSourceEnable == TRACK2){ 
  __Set(V_THRESHOLD,b); 
  }
 }

  if((Current==7)&&(_Det==2)&&(_Mode!=X_Y)&&(_4_source!=SPEC_A)
  &&(_4_source!=SPEC_B)&&(ListOverride)){
    ConfNameTimer=110;
    ToggleName=1;
    Update_Mark();
  }
}

void UpdateTLevels(void){                                                  
  Ch1TLevel=(((_Vt1-Ka1[_A_Range]-_1_posi)*1024)/Ka2[_A_Range])+_1_posi;
  Ch2TLevel=(((_Vt2-Kb1[_B_Range]-_2_posi)*1024)/Kb2[_B_Range])+_2_posi;
}

/*******************************************************************************
 Process: Calculate processing buffer data
*******************************************************************************/
void Process(void)
{ 
  s16 i, j = 0, k, V[8] = {0},a,b, n = 0,OsA=0,OsB=0;
  s16 IValueA, IValueB, IVT1, IVT2;
  s32 Tmp=0;
  u8  Ch[4], C_D=0;
  s8  Sa = 2, Sb = 2, Sc = 2, Sd = 2;     // time status
  u16 Ta, Tb, Tc, Td;                     // pulse width count
  u16 Pa, Pb, Pc, Pd;			  // holds + side transition of wave for PaS
  u8  jj;
  s16 h;
  s16 start,Tstart;                       //for cursor defined meters
  u16 finish,Tstop;
  s8  Shift=0;				  //jitter compensation value
  u8  TriggerPosition=150;
  u8  DTriggerPosition=150;
  u8  TimeCalcHyst=2;			  //time based calculations trigger point hysteresis value
  s16  AWavCenter;                          
  s16  BWavCenter;
  s32  X=0,Y=0;
  u16  LeftSkirt=0;

  Waste=DataBuf[4095]&0x0000000F;

  if(UartLogic()||i2cLogic()||SpiLogic())SerialStatus=1;else SerialStatus=0;
  Ta = Tb = Tc = Td = 0;
  Pa = Pb = Pc = Pd = 0;

  TaS = 0; TbS = 0; TcS = 0; TdS = 0; 
  PaS = 0; PbS = 0; PcS = 0; PdS = 0; 
  TaN = 0; 
  TbN = 0; TcN = 0; TdN = 0; 

  Posi_412 = _4_posi - _1_posi - _2_posi;
  Posi_41_2 = _4_posi - _1_posi + _2_posi;
  Posi_41  = _4_posi - _1_posi;
  Posi_42  = _4_posi - _2_posi;
  Posi_4_2 = _4_posi + _2_posi;
  Posi_4F1 = _4_posi - FileBuff[ 397];
  Posi_4F2 = _4_posi - FileBuff[ 797];
  Posi_4F3 = _4_posi - FileBuff[ 1197];
  Posi_4F4 = _4_posi - FileBuff[1597];

  A_Posi   = _1_posi-Ka1[_A_Range];    
  B_Posi   = _2_posi-Kb1[_B_Range];    
  
  a_Max=-0x7FFF;a_Min=0x7FFF;          //in case max is negative or min is positive (as with DC voltages)	
  b_Max=-0x7FFF;b_Min=0x7FFF;          //originally would initiate @i=0 but with init @i=4 could transfer in slow mode before init

  aT_Max = A_Posi; bT_Max = B_Posi;    // Used for calculating auto trigger
  aT_Min = A_Posi; bT_Min = B_Posi;             

  if((_3_posi + 20)>= Y_BASE+Y_SIZE)  c_Max = Y_BASE+Y_SIZE-1;
  else                                c_Max = _3_posi + 20;
  if((_4_posi + 20)>= Y_BASE+Y_SIZE)  d_Max = Y_BASE+Y_SIZE-1;
  else                                d_Max = _4_posi + 20;

  if((ChartLogic())&&(FrameMode==0)&&(ScrollFlag>0)){  
    if((JumpCnt>(380+_X_posi.Value))){_X_posi.Value=(JumpCnt-380);if((Current!=FILE)&&(NotificationTimer==0))
      Update_View_Area();ScrollFlag=1;}         
    if((ScrollFlag==1)&&(JumpCnt<=380)){_X_posi.Value=0;if(Current!=FILE)Update_View_Area();}         
    XposRef=GetXposRef(_X_posi.Value);
  }

  if(OsBufferLogicFlag){                                               
    Xtend=119;                                                          //provides 512 samples for FFT 
  }else{
     if ((((_Mode==NORH)||(_Mode==NORHLD)||(_Mode==NORC))&&(_T_base < 10)&&(Options&1))||(_4_source>9)){
       Xtend=150;							//stabilize buffer size in holdoff mode to cover pre-trigger section
     }else{								//optimize size for best aquisition
       Xtend=_X_posi.Value;
       if (FlagMeter>0) Xtend-=86;
     } 
     if(_4_source>9)Xtend+=56; else if(Det)Xtend+=20;                   //cover right end of FFT buffer or averaging filter in detector mode
  }

      if(TriggerModeLogic())TriggerPosition=149;else TriggerPosition=150;		//time based triggers occur 1 sample earlier

      switch (_T_base){									//digital ch trig positions for interpolated ranges
        case 17:
	  DTriggerPosition=149;
          break;
        case 18:
	  DTriggerPosition=148;
          break;
        case 19:                                                 
          if(TriggerModeLogic())DTriggerPosition=147;else DTriggerPosition=148;		
          break;
        case 20:
          if(TriggerModeLogic())DTriggerPosition=146;else DTriggerPosition=147;		
          break;
        case 21:
          if(TriggerModeLogic())DTriggerPosition=145;else DTriggerPosition=146;		
      }

  if(_Status==RUN)TempKp1=_Kp1;	                   // eliminates improper change of wave timebase by Kp1 while in hold mode at faster timebases
  if(OsBufferLogicFlag){
    k=StartOffset;
    WindowPosition=0;
  }else{
    k =((1024 - TempKp1)*TriggerPosition + 512)/1024 + _X_posi.Value;  // window position in the calculation of the interpolation of the correction value
    WindowPosition=_X_posi.Value;                            
    Tmp=1792/Int_sqrt(TempKp1);
    if(WindowPosition>Tmp)WindowPosition-=Tmp; else     // shift window back to center FFT over display 
      WindowPosition=0;
    if(WindowPosition>3582)WindowPosition=3582;         // FFT window is 512, need to keep from going past last samples
  }

    if(AutoSetFlag){                                    //autoset uses cursor defined function to filter out transients from shifting ranges
      if(AutoSequence==0){
        //Tstart=300;
        Tstart=200;
        Tstop=get_bag_max_buf()-45;
      }else{
        Tstart=160;
        Tstop=get_bag_max_buf()-20;
      }
    }else{
      Tstart=Title[T_VERNIE][0].Value;
      Tstop=Title[T_VERNIE][1].Value;
    }

    start=(((((TempKp1*1000)/1024)*Tstart)+500)/1000)+k-((1024/TempKp1)/4);	//calculate start/stop positions for
    if((_T_base==20)&&(Title[7][0].Value<2))start--; 				//cursor defined meters
    if(_T_base==19)start--; 
    if((_T_base==18)&&(Title[7][1].Value>3))start--; 
    finish=(((((TempKp1*1000)/1024)*Tstop)+500)/1000)+k;    

    if ((Title[TRIGG][SOURCE].Value>1)&&(Title[TRIGG][SOURCE].Value<4)){start+=5;finish+=5;}      // adjust to align with shifted wave when triggering from dig ch

    if(_Mode==X_Y){if(Title[6][3].Value<16)Title[6][3].Value=16;BufferSize=Title[6][3].Value*16;}
    bag_max_buf = get_bag_max_buf();

    if(AutoSetFlag==0){
      if(Title[T_VERNIE][T1].Value==0)start=0;			// lock out time cursors from restricting measurements if
      if(FlagMeter){                                              // at extreme right or left in cursor defined meter mode
        if(Title[T_VERNIE][T2].Value==300)finish=bag_max_buf;
      }else{
        if(Title[T_VERNIE][T2].Value==386)finish=bag_max_buf;
      }
    }

    if (CurDefTime==0){			
      NSamples=(bag_max_buf-4);			//count starts @ i=4 to eliminate noise on some ranges			
      a_Avg=NSamples/2;
      b_Avg=NSamples/2;
    }else{
       if (finish>start){
         NSamples=(finish-start);
         if (NSamples>1){
	    NSamples--;
            a_Avg=NSamples/2;
            b_Avg=NSamples/2;
         }else{
          a_Avg=A_Posi;
          b_Avg=B_Posi;
         }
       }else{
        NSamples=1;
        a_Avg=A_Posi;
        b_Avg=B_Posi;
       }
    }  
    a_Ssq=b_Ssq=NSamples/2;
    if((_4_source==15)||(_4_source==13))Normal=100; else Normal=80;                //normalize FFT levels

    if ((exitflag==1)&&(JumpCnt!=0)) exitflag=0;      // leave exitflag on at beginning of frame so if complete frame is done, allow freerun 

  for(i=0; i <bag_max_buf; i++){

    if((((_T_base>10)|| (  (_T_base>9)&&    ( (FrameMode>0)||((FrameMode==0)&&(_Mode==NORHLD)) )    
      &&(Current==FILE)&&(_Det==DIR)&&(Title[9][0].Value==1)&& (!ListLogic())   )  
      ||((_T_base>9)&&(FrameMode==0)&& (((_Kind==5)&&(_Frqn>11))||(ShowFFT==1)))) || (freerun==1)   
      || (  (_T_base>9)&&( (Tim2Factor>20)||(((FrameMode==0)&& (SerialStatus))||(SpiLogic())) )  )  
      || (   ( (_Mode==X_Y)||((SpiLogic())&&(FrameMode==0)&&(SpiChartFlag)&&((_3_source)
      || (SpiNumBits==7)||(SpiNumBits==8)) &&(ArrayIndex>47))) && (_T_base>8) ))  &&  (_Status == RUN)  ){

      Tmp=Update;               //compiler optimizations: need to do something significant with Tmp or it dismisses the while loops
      if((FrameMode==0)&&(_T_base==10)&&(((_Kind==5)&&(_Frqn>11))||(ShowFFT==1)||(Tim2Factor>20))){
        while(1){if(Tmp++>(Waste+150))break;}            //Proper timing for 500uS timebase in full buffer mode under various conditions
        Waste=Tmp;
        if((_Mode==NORHLD)&&((_Kind==5)&&(_Frqn>15)))while(1){if(Tmp++>100)break;}
      }
      Tmp=0; 
      if((FrameMode==0)&&(_T_base==11)&&(Sweep)&&(SweepIndex<6))while(1){if(Tmp++>25)break;}         //timing for 200uS timebase
      Waste=Tmp;                                      

      if((FrameMode>0)&&(_T_base==10)&&(Current==FILE)&&(_Det==DIR)&&(Title[9][0].Value==1)&& (!ListLogic()) ){
        Tmp=Update; 
        if(_Mode==NORHLD){
          while(1){if(Tmp++>2600)break;}
        }else{
          while(1){if(Tmp++>900)break;}          
        }
        Waste=Tmp;
      } 
  
      if( (_Mode==NORHLD)&&(FrameMode==0)&&(_T_base==10)&&(Current==FILE)&&(_Det==DIR)&&(Title[9][0].Value==1)&& (!ListLogic()) ){
        Tmp=Update; 
        while(1){if(Tmp++>100)break;}
        Waste=Tmp;
      } 


      if(Title[TRIGG][SOURCE].Value==4) TransferFIFO(i,1); else TransferFIFO(i,0);
      FastMode=1;   

    }else if(         (__Get(FIFO_EMPTY)==0)                   &&(i == JumpCnt)&&(_Status == RUN)){
      if(Title[TRIGG][SOURCE].Value==4) TransferFIFO(i,1); else TransferFIFO(i,0);
      if(      ((!OsBufferLogicFlag)&&(!ChartLogic()))          ||(OsTransferCompleteFlag)){
        OsTransferCompleteFlag=0;
        JumpCnt++;
      }
      FastMode=0;
    }

      V[A]=(DataBuf[i]&0xFF)-ADCoffset;
      if(V[A]>=0)Ch[A]=V[A];else Ch[A]=0;
      if(V[A]<-15)V[A]=-15;

      V[B]=((DataBuf[i]>>8)&0xFF)-ADCoffset;
      if(V[B]>=0)Ch[B]=V[B];else Ch[B]=0;
      if(V[B]<-15)V[B]=-15;

      IValueA=V[A];      
      IValueB=V[B];

      if(OsBufferLogicFlag||OsChartFlag){
        if((OSAvg)&&(!OsChartFlag)){
          V[A]=OsFFTData[0][i]-ADCoffset;
          OsA=V[A];
          V[B]=OsFFTData[1][i]-ADCoffset;
          OsB=V[B];
        }else{         
          OsA=(((DataBuf[i]>>16)&0xFF)-ADCoffset);   //accumulated minimum, maximum is already in V[x}
          OsB=(((DataBuf[i]>>24)&0xFF)-ADCoffset);   
        } 

        if((RMSdata[0][i]-ADCoffset)>=0)Ch[A]=(RMSdata[0][i]-ADCoffset);else Ch[A]=0;  //2 lines take the place of commented above
        if((RMSdata[1][i]-ADCoffset)>=0)Ch[B]=(RMSdata[1][i]-ADCoffset);else Ch[B]=0;

        if(OsA<-15)OsA=-15;
        if(OsB<-15)OsB=-15;
      }

      if(InvertA){
        V[A]-=A_Posi;
        V[A]=-V[A];
        V[A]+=A_Posi;
        if(OsBufferLogicFlag||OsChartFlag){
          if((OSAvg)&&(!OsChartFlag)){
            OsA=V[A];  
          }else{
            OsA-=A_Posi;                                  
            OsA=-OsA;
            OsA+=A_Posi;
            Tmp=OsA;
            OsA=V[A];
            V[A]=Tmp;
          }

          Ch[A]-=A_Posi;
          Ch[A]=-Ch[A];                                 //<< would be ok here << could use this for both averaging and OS modes...(?)
          Ch[A]+=A_Posi;                                //takes the place of commented lines above 

          if(OsA<-15)OsA=-15;
        }else{
          if(V[A]>=0)Ch[A]=V[A];else Ch[A]=0;
        } 
        if(V[A]<-15)V[A]=-15;
      }

      if(InvertB){
        V[B]-=B_Posi;
        V[B]=-V[B];
        V[B]+=B_Posi;
        if(OsBufferLogicFlag||OsChartFlag){
          if((OSAvg)&&(!OsChartFlag)){
            OsB=V[B];
          }else{
            OsB-=B_Posi;
            OsB=-OsB;
            OsB+=B_Posi;
            Tmp=OsB;
            OsB=V[B];
            V[B]=Tmp;
          }

          Ch[B]-=B_Posi;                                 //as for ch A
          Ch[B]=-Ch[B];
          Ch[B]+=B_Posi;

          if(OsB<-15)OsB=-15;
        }else{
          if(V[B]>=0)Ch[B]=V[B];else Ch[B]=0;
        } 
        if(V[B]<-15)V[B]=-15;
      }

      if(OsA>255)OsA=255;
      if(OsB>255)OsB=255;
      if(V[A]>255)V[A]=255;
      if(V[B]>255)V[B]=255;

        if(OsBufferLogicFlag){
	  if(i == 4) {								// used for auto trigger and time measurements in OS modes
  	        aT_Max = Ch[A];	        // statistics channel A maximum
		bT_Max = Ch[B];	        // statistics channel B maximum
		aT_Min = aT_Max;	// statistics channel A minimum
		bT_Min = bT_Max;	// statistics channel B minimum
	  }else{         				  // not the first values
  		if(Ch[A] > aT_Max)  aT_Max = Ch[A];         // statistics channel A maximum
		if(Ch[B] > bT_Max)  bT_Max = Ch[B];         // statistics channel B maximum
                if(Ch[A] < aT_Min)  aT_Min = Ch[A];         // statistics channel A minimum
                if(Ch[B] < bT_Min)  bT_Min = Ch[B];         // statistics channel B minimum
	  }
        }else{
	  if(i == 4) {								// used for auto trigger
  	        aT_Max = V[A];	        // statistics channel A maximum
		bT_Max = V[B];	        // statistics channel B maximum
		aT_Min = aT_Max;	// statistics channel A minimum
		bT_Min = bT_Max;	// statistics channel B minimum
	  }else{         				  // not the first values
  		if(V[A] > aT_Max)  aT_Max = V[A];         // statistics channel A maximum
		if(V[B] > bT_Max)  bT_Max = V[B];         // statistics channel B maximum
                if(V[A] < aT_Min)  aT_Min = V[A];         // statistics channel A minimum
                if(V[B] < bT_Min)  bT_Min = V[B];         // statistics channel B minimum
	  }
        } 

		// FFT ///////////////////////////////////////  

#warning Gabo: FFT disabled
#if 0
           if((_4_source>10)&&(i>=WindowPosition)&&(i<(FFTSize+WindowPosition))&&(_Mode!=X_Y)
           &&       ( (EnableFFT==0)||(_T_base>7)||(_Mode==SCAN)||(_State.Value==2) )        ){

		if (((_4_source==SPEC_A)&&(_2_source))||(_4_source==13)||(_4_source==12)){
                  if(OsBufferLogicFlag){
                    Tmp=OsFFTData[1][i-WindowPosition]-ADCoffset;
                    if(Tmp<-15)Tmp=-15;
                    fr[i-WindowPosition]=(((Tmp-_2_posi)*Window[i-WindowPosition])/Normal)<<(2+FFTGain); 
                  }else{
                    fr[i-WindowPosition]=(((V[B]-_2_posi)*Window[i-WindowPosition])/Normal)<<(2+FFTGain);  			     
                  }
                }     

                if (((_4_source==SPEC_A)&&(_1_source))||(_4_source>13) ){
                  if(OsBufferLogicFlag){
                    Tmp=OsFFTData[0][i-WindowPosition]-ADCoffset;
                    if(Tmp<-15)Tmp=-15;
                    fr[i-WindowPosition]=(((Tmp-_1_posi)*Window[i-WindowPosition])/Normal)<<(2+FFTGain); 
                  }else{
                    fr[i-WindowPosition]=(((V[A]-_1_posi)*Window[i-WindowPosition])/Normal)<<(2+FFTGain);
                  }
                }

            }
#endif
		////////////////////////////////////// FFT ///

      if(!OsBufferLogicFlag){if (i>5) {C_D = DataBuf[i-6] >>16;}else C_D=0;}	  // align digital chs with analogs

//if(FlagMeter>0){	//could be used to speed up display refresh rate when meters are turned off?
      if (((CurDefTime==0)&&(i>3)&&(i<bag_max_buf))||((i>start)&&(i<finish))){		//for voltage measurements, only for analog channels

        if(OsBufferLogicFlag){

            if(OSAvg){
              Tmp=OsFFTData[0][i]-ADCoffset-A_Posi;
              a_Ssq +=(Tmp * Tmp);                              
              Tmp=OsFFTData[1][i]-ADCoffset-B_Posi;
              b_Ssq +=(Tmp * Tmp);  
            }else{  
              Tmp=RMSdata[0][i]-ADCoffset-A_Posi;
              a_Ssq +=(Tmp * Tmp);                              
              Tmp=RMSdata[1][i]-ADCoffset-B_Posi;
              b_Ssq +=(Tmp * Tmp);  
            }


          Tmp=OsFFTData[0][i]-ADCoffset;
          if(InvertA){
            Tmp-=A_Posi;
            Tmp=-Tmp;
            Tmp+=A_Posi;
          } 
          if(Tmp<-15)Tmp=-15;
          a_Avg +=Tmp;
          Tmp=OsFFTData[1][i]-ADCoffset;
          if(InvertB){
            Tmp-=B_Posi;
            Tmp=-Tmp;
            Tmp+=B_Posi;
          } 
          if(Tmp<-15)Tmp=-15;
          b_Avg += Tmp;                                     

        }else{               
          a_Avg += V[A];                                    // use signed vars, otherwise, values at bottom of screen, pushed up by Kx1, now below zero, fail to register
          Tmp = V[A]- A_Posi;
          a_Ssq +=(Tmp * Tmp);                              // statistical sum of squares of the A channel

          b_Avg += V[B];                                    // cumulative average channel B, DC
          Tmp = V[B]- B_Posi;
          b_Ssq +=(Tmp * Tmp);                              // statistical sum of squares of the B channel
        }

	  if((i == 4)||((CurDefTime>0)&&(i==(start+1)))) {		  // read first values - max = min = values
	        a_Max = V[A];	// statistics channel A maximum
		b_Max = V[B];	// statistics channel B maximum
		a_Min = a_Max;	// statistics channel A minimum
		b_Min = b_Max;	// statistics channel B minimum
	  }else{			                     // not the first values
  		if(V[A] > a_Max)  a_Max = V[A];              // statistics channel A maximum
		if(V[B] > b_Max)  b_Max = V[B];              // statistics channel B maximum
                if(OsBufferLogicFlag){
  		  if(OsA  < a_Min)  a_Min = OsA;             // statistics channel A minimum
  		  if(OsB  < b_Min)  b_Min = OsB;             // statistics channel A minimum
                }else{ 
  		  if(V[A] < a_Min)  a_Min = V[A];            // statistics channel A minimum
		  if(V[B] < b_Min)  b_Min = V[B];            // statistics channel B minimum
                }
	  }
      }

    if((CurDefTime==0)||((i>start-1)&&(i<(finish+1)))){        //for time related measurements for both dig and analog channels

      if((i>3)&&(i<(bag_max_buf-3))){ 

       if((CurDefTime==0)||(i<finish)){

        if(Ch[A] > a_Mid_H){
          if (Sa<2){					//don't initiate if value is above trigger point when starting out
            if(Sa == 0){
		  TaS = i;                    					
                  LastA_Mid=a_Mid_H;			//save relevant time meter trigger level for quantization compensation
              if(Ta==0){				//use Ta to initiate
                  Ta=i;					//first + crossing point saved 
                  QStart[0]=i;				//start position saved for quantization calcs 
                  FirstA_Mid=a_Mid_H;
		  }else{				//after initialized, count + crossings, add + wave sampling count to PaS
		    TaN++;                              //increment TaN only after initial crossing point saved
		    PaS += Pa;				//only add positive part if whole wave is considered	
              }
		}
            Sa = 1;  
          } 
        } else { 
          if(Ch[A] < a_Mid_L){
            if(Sa == 2){
		  Sa=0; 				//initiate only after going below trigger point so partial wave does not get counted
		  PaS=0;				//initiate PaS
            }
	      if(Sa == 1){
                  Sa = 0;
		  Pa = i-TaS;				//save for PaS
            }
          }
        }

       if(Ch[B] > b_Mid_H){
          if (Sb<2){					
            if(Sb == 0){
		  TbS = i;                    					
                  LastB_Mid=b_Mid_H;			//save relevant time meter trigger level for quantization compensation
              if(Tb==0){				
                  Tb=i;					 
                  QStart[1]=i;				//start position saved for quantization calcs 
                  FirstB_Mid=b_Mid_H;
		  }else{
		    TbN++;                         
		    PbS += Pb;					
              }
		}
            Sb = 1;  
          } 
        } else { 
          if(Ch[B] < b_Mid_L){
            if(Sb == 2){
		  Sb=0; 					
		  PbS=0;					
            }
	      if(Sb == 1){
              Sb = 0;
		  Pb = i-TbS;				
            }
          }
        }
       }  

    if((i>10)&&((CurDefTime==0)||(i>start))){                        //eliminate noise at start of wave causing errors in dig channels

       if(C_D & 1){
          if (Sc<2){					
            if(Sc == 0){
		  TcS = i;                    					
              if(Tc==0){				
                Tc=i;					 
		  }else{
		    TcN++;                         
		    PcS += Pc;					
              }
		}
            Sc = 1;  
          } 
        } else { 
            if(Sc == 2){
		  Sc=0; 					
		  PcS=0;					
            }
	      if(Sc == 1){
              Sc = 0;
		  Pc = i-TcS;				
            }
	  } 

       if(C_D & 2){
          if (Sd<2){					
            if(Sd == 0){
		  TdS = i;                    					
              if(Td==0){				
                Td=i;					 
		  }else{
		    TdN++;                         
		    PdS += Pd;					
              }
		}
            Sd = 1;  
          } 
        } else { 
            if(Sd == 2){
		  Sd=0; 					
		  PdS=0;					
            }
	      if(Sd == 1){
              Sd = 0;
		  Pd = i-TdS;				
            }
        }

      }	//if i>8
    } //if i>3
  }  //if curdeftime
//}    //if flagmeter>0
   
      if((i==TriggerPosition)&&(_T_base > 16)&&(_Mode!=SCAN)&&(__Get(FIFO_START)!=0)){                          //analog channels
        if(InvertA)IVT1=((-(_Vt1-_1_posi))+_1_posi);else IVT1=_Vt1;
        if(InvertB)IVT2=((-(_Vt2-_2_posi))+_2_posi);else IVT2=_Vt2;
        if((Title[7][1].Value==1)||(Title[7][1].Value==3)||(Title[7][1].Value==4)||(Title[7][1].Value==5)){	//positive transitions
          if ((Title[7][0].Value==0)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==0))){										//select trigger channel
            b=(Ka1[_A_Range]+A_Posi+(Ka2[_A_Range]*(IValueA-A_Posi)+ 512)/1024)-IVT1;
            a=IVT1-(Ka1[_A_Range]+A_Posi+(Ka2[_A_Range]*(((DataBuf[TriggerPosition-1]& 0xFF)-ADCoffset)-A_Posi)+ 512)/1024);        
          }else if((Title[7][0].Value==1)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==1))){
            b=(Kb1[_B_Range]+B_Posi+(Kb2[_B_Range]*(IValueB-B_Posi)+ 512)/1024)-IVT2;
	    a=IVT2-(Kb1[_B_Range]+B_Posi+(Kb2[_B_Range]*((((DataBuf[TriggerPosition-1]>>8)&0xFF)-ADCoffset)-B_Posi)+ 512)/1024);        
          }else goto bypasscalculate;
        }else{													//negative transitions
          if ((Title[7][0].Value==0)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==0))){										
            b=IVT1-(Ka1[_A_Range]+A_Posi+(Ka2[_A_Range]*(IValueA-A_Posi)+ 512)/1024);
	    a=(Ka1[_A_Range]+A_Posi+(Ka2[_A_Range]*(((DataBuf[TriggerPosition-1]& 0xFF)-ADCoffset)-A_Posi)+ 512)/1024)-IVT1;        
          }else if((Title[7][0].Value==1)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==1))){
            b=IVT2-(Kb1[_B_Range]+B_Posi+(Kb2[_B_Range]*(IValueB-B_Posi)+ 512)/1024);
	    a=(Kb1[_B_Range]+B_Posi+(Kb2[_B_Range]*((((DataBuf[TriggerPosition-1]>>8)&0xFF)-ADCoffset)-B_Posi)+ 512)/1024)-IVT2;        
          }else goto bypasscalculate; 
        }
        Shift=-((((b*102400)/((a+b)*TempKp1))+50)/100);		//calculate jitter from analog channels
      }

bypasscalculate:								   //digital channels		

     if ((i==155)&&(_T_base > 16)&&(_Mode!=SCAN)&&(__Get(FIFO_START)!=0)){	   //calculate jitter position if trig from dig channels
        if((Title[7][1].Value==1)||(Title[7][1].Value==3)||(Title[7][1].Value==4)||(Title[7][1].Value==5)){	//positive transitions
          if (Title[7][0].Value==2){
            for (jj=DTriggerPosition;jj<155;jj++){					
	      if (((DataBuf[jj]>>16)&1)==1){Shift=((jj-DTriggerPosition)*1024)/TempKp1;break;}       
            }		
          }else if(Title[7][0].Value==3){
            for (jj=DTriggerPosition;jj<155;jj++){
	      if (((DataBuf[jj]>>16)&2)==2){Shift=((jj-DTriggerPosition)*1024)/TempKp1;break;}
            }		
          }
        }else{										//negative transitions
          if (Title[7][0].Value==2){										
            for (jj=DTriggerPosition;jj<155;jj++){
	      if (((DataBuf[jj]>>16)&1)==0){Shift=((jj-DTriggerPosition)*1024)/TempKp1;break;}
            }		
          }else if(Title[7][0].Value==3){
            for (jj=DTriggerPosition;jj<155;jj++){
	      if (((DataBuf[jj]>>16)&2)==0){Shift=((jj-DTriggerPosition)*1024)/TempKp1;break;}
            }		
          } 
        }
      }  

      if(i >= k){                          // pointer to reach the specified window position
					   // Use V[x] loaded above rather than unsigned Ch[x], prevents clipping.

	  if (CalFlag>0) {

            Tmp=Ka2[_A_Range]*(V[A]-A_Posi)  ;              // Factor in gain correction for signal (Ka2) from signal zero point
            if (Tmp>0)Tmp+=512; else Tmp-=512;	            // Flip bias if negative, allows proper compensation of negative values
            V[A]  = Ka1[_A_Range]+A_Posi+(Tmp/1024);      
            Tmp=Kb2[_B_Range]*(V[B]-B_Posi);
            if (Tmp>0)Tmp+=512; else Tmp-=512;	   
            V[B]  = Kb1[_B_Range]+B_Posi+(Tmp/1024);     

            if(OsBufferLogicFlag||OsChartFlag){                                // adjust oversampling data
              Tmp=Ka2[_A_Range]*(OsA-A_Posi)  ;             // Factor in gain correction for signal (Ka2) from signal zero point
              if (Tmp>0)Tmp+=512; else Tmp-=512;	    // Flip bias if negative, allows proper compensation of negative values
              OsA  = Ka1[_A_Range]+A_Posi+(Tmp/1024);      
              Tmp=Kb2[_B_Range]*(OsB-B_Posi);
              if (Tmp>0)Tmp+=512; else Tmp-=512;	   
              OsB  = Kb1[_B_Range]+B_Posi+(Tmp/1024);     
            }

	  }else{											    
            V[A]  = Ka1[_A_Range]+V[A];                     // gain correction switched off
            V[B]  = Kb1[_B_Range]+V[B];
            if(OsBufferLogicFlag||OsChartFlag){
              OsA+=Ka1[_A_Range];     
              OsB+=Kb1[_B_Range];     
            }
          }
          if(OsA<0)OsA=0;             
          if(OsB<0)OsB=0;             

      if((Title[TRACK1][COUPLE].Value==2)&&(_1_source)){                                 //filter=40
        if(i>=(Filter+28)){
          X=0;
          for(h=(i-Filter);h<i;h++){           
            Tmp=(DataBuf[h] & 0xFF )-ADCoffset;                 //retrieve each of Filter number of samples           
            if(Tmp<0)Tmp=0;                                  
            Tmp=Tmp+Ka1[_A_Range]-_1_posi;
            Tmp=(Ka2[_A_Range]*Tmp)/1024;
            if(Tmp<0)Tmp=-Tmp;                                  //detect
            if(_Kind==4){
              Tmp=((Tmp*225)+50)/100;                             
            }else{  
              Tmp=((Tmp*280)+50)/100;                             
            }
            X+=(Tmp*Tmp);                                       //sum squares
          }
          Y=(Int_sqrt(X/Filter));
          if(Y>198)Y=198;
          Y=(Y*(100+((198-Y)/2)))/100; 
          V[A]=Y;
        }else V[A]=0;
      }

      if((Title[TRACK2][COUPLE].Value==2)&&(_2_source)){
        if(i>=(Filter+28)){
          X=0;
          for(h=(i-Filter);h<i;h++){           
            Tmp=((DataBuf[h] >> 8) & 0xFF)-ADCoffset;                            
            if(Tmp<0)Tmp=0;                                  
            Tmp=Tmp+Kb1[_B_Range]-_2_posi;
            Tmp=(Kb2[_B_Range]*Tmp)/1024;
            if(Tmp<0)Tmp=-Tmp;              
            if(_Kind==4){
              Tmp=((Tmp*225)+50)/100;                             
            }else{  
              Tmp=((Tmp*280)+50)/100;                             
            }
            X+=(Tmp*Tmp);                               
          }
          Y=(Int_sqrt(X/Filter));                                        
          if(Y>198)Y=198;
          Y=(Y*(100+((198-Y)/2)))/100; 
          V[B]=Y;
        }else V[B]=0;
      }

        while(j > 0 ){
          if ((Options&2)&&(_T_base<17)){               // prevent interpolate routine from interfering with non-interpolated time bases
            if(OsBufferLogicFlag||OsChartFlag){
              Send_Data( V[A],V[B],OsA,OsB,n++);         // send oversampling data over digital channels 		
            }else{
              Send_Data( V[A_],V[B_],C_D,0,n++);	  		
            } 
          }else{
            Send_Data( V[A_]+((V[A]-V[A_])*(1024 - j))/1024, // the current CH_A point interpolation
                       V[B_]+((V[B]-V[B_])*(1024 - j))/1024, // the current CH_B point interpolation
                       C_D,0,                                // current point digital channel values
                       n++);
          }
          j -= TempKp1;
          if(n >= X_SIZE){ k = 8192;  break;}    	
        }  

        j += 1024;                              
        V[A_] = V[A];  V[B_] = V[B];            
      }

    }						// end of for loop

    if ((exitflag==1)&&(JumpCnt<(393+Xtend))) exitflag=0;      //if exitflag was on at start, full frame finished allows freerun     

    if ((FrameMode!=0) && (_T_base>11) && (_Mode==SCAN)) __Set(FIFO_CLR, W_PTR);

        if(Options&4){ 	//alarms
          if(_Mode!=X_Y){

                      if(Title[V_VERNIE][V1].Value<200){	//lock out if at extreme top
			h=a_Max-A_Posi;
	                h = (((Ka2[_A_Range]*h)+512)/1024);
                        h=h+_1_posi;	 	        	
                        if((h>Title[V_VERNIE][V1].Value)&&(Title[0][0].Value!=0)&&(Title[V_VERNIE][2].Value==0)) Beeper(500);

			h=b_Max-B_Posi;
	                h = (((Kb2[_B_Range]*h)+512)/1024);
                        h=h+_2_posi;	 	        	
                        if((h>Title[V_VERNIE][V1].Value)&&(Title[1][0].Value!=0)&&(Title[V_VERNIE][2].Value==1))Beeper(500);
                      }

                      if(Title[V_VERNIE][V2].Value>0){	//lock out if at extreme bottom
			h=a_Min-A_Posi;
	                h = (((Ka2[_A_Range]*h)+512)/1024);
                        h=h+_1_posi;	 	        	
                        if((h<Title[V_VERNIE][V2].Value)&&(Title[0][0].Value!=0)&&(Title[V_VERNIE][2].Value==0))Beeper(500);

			h=b_Min-B_Posi;
	                h = (((Kb2[_B_Range]*h)+512)/1024);
                        h=h+_2_posi;	 	        	
                        if((h<Title[V_VERNIE][V2].Value)&&(Title[1][0].Value!=0)&&(Title[V_VERNIE][2].Value==1))Beeper(500);
                      }
          }

        }

  	AWavCenter=(aT_Max + aT_Min)/2;
        BWavCenter=(bT_Max + bT_Min)/2;

	if((CurDefTime>0)&&(AutoSetFlag==0)){  	                     //allow V cursors to adjust time meters trigger level
  	  h=AWavCenter;
          Tmp=h; 
          h=(((Ka2[_A_Range]*(h-A_Posi))+512)/1024)+_1_posi;
          if(Title[V_VERNIE][V1].Value<h){
            AWavCenter-=(((1024*(h-Title[V_VERNIE][V1].Value))+((512*Ka2[_A_Range])/1024))/Ka2[_A_Range]);
            if(Title[V_VERNIE][V2].Value>Title[V_VERNIE][V1].Value){
              Tmp+=(((1024*(Title[V_VERNIE][V2].Value-h))+((512*Ka2[_A_Range])/1024))/Ka2[_A_Range]);
              AWavCenter=Tmp; 
            }
          }else if(Title[V_VERNIE][V2].Value>h){
            AWavCenter+=(((1024*(Title[V_VERNIE][V2].Value-h))+((512*Ka2[_A_Range])/1024))/Ka2[_A_Range]);
          } 
  	  h=BWavCenter;
          Tmp=h; 
          h=(((Kb2[_B_Range]*(h-B_Posi))+512)/1024)+_2_posi;
          if(Title[V_VERNIE][V1].Value<h){
            BWavCenter-=(((1024*(h-Title[V_VERNIE][V1].Value))+((512*Kb2[_B_Range])/1024))/Kb2[_B_Range]);
            if(Title[V_VERNIE][V2].Value>Title[V_VERNIE][V1].Value){
              Tmp+=(((1024*(Title[V_VERNIE][V2].Value-h))+((512*Kb2[_B_Range])/1024))/Kb2[_B_Range]);
              BWavCenter=Tmp;
            }
          }else if(Title[V_VERNIE][V2].Value>h){
            BWavCenter+=(((1024*(Title[V_VERNIE][V2].Value-h))+((512*Kb2[_B_Range])/1024))/Kb2[_B_Range]);
          }
        }
        a_Mid_H = TimeCalcHyst +AWavCenter;
        a_Mid_L = a_Mid_H - (2*TimeCalcHyst);
        b_Mid_H = TimeCalcHyst +BWavCenter;
        b_Mid_L = b_Mid_H - (2*TimeCalcHyst);

        QEnd[0]=TaS;					//ending sampling positions for quantization error correction 
        QEnd[1]=TbS;

       TaS -= Ta; TbS -= Tb; TcS -= Tc; TdS -= Td;

  if((_Status == RUN)&&(Options&4)&&(ClearHoldFlag>0)){       //clear hold if in fast mode (after first loop)
    if((_T_base > 10)||((_T_base < 11)&&((JumpCnt==get_bag_max_buf())||(freerun==1)))){ //if in slow mode, wait for frame to be done     
     if(SlowModeSkip==0){    
      if(Title[TRIGG][SOURCE].Value<4){   						//if not in ab mode
        ClearMinMax(ClearHoldFlag);
        ClearHold(ClearHoldFlag);
        ClearHoldFlag=0;
      }else{										//in ab mode
         if((TrigSourceEnable==0)&&(ClearHoldFlag&1)){
           ClearMinMax(1);
           ClearHold(1);
           ClearHoldFlag--;								//removes bit 1 (tested as set)
         }
         if((TrigSourceEnable==1)&&(ClearHoldFlag&2)){
           ClearMinMax(2);
           ClearHold(2);
           ClearHoldFlag&=0xFD;								//removes bit 2
         }
      }
     }else if(SlowModeSkip>0) SlowModeSkip--;
    } 
  }    

    if(JumpCnt==get_bag_max_buf())ClearLeadingEdge=0;             //  WAS 0    if turned on by clear screen, reset at end of frame

    if((SumResetFlag==1)&&((JumpCnt==get_bag_max_buf())||(freerun==1)||(_Status!= RUN)||((DownConvertMode)&&(FFTt1Mode)))){   //in slow mode wait for all necessary loops 
      EnablePaS=1;									    //      	to complete frame
      if((DownConvertMode)&&(DownConvertInitiate==0))Display_Value(0);
      ResetSum();				//sync sum resets between meter reads and completed frames 
      SumResetFlag=0;
    }

      if (WaitForTrigFlag){                     //meter reset flag for normal modes when not triggered so display retains values while waiting  
        EnablePaS=1;				//while allowing consequent trigger to display values properly					    
        ResetSum();
        WaitForTrigFlag=0;				
      }

        if(a_Max>Ga_Max)Ga_Max=a_Max;		//update peak from all frames into Gx_Max,Min
        if(b_Max>Gb_Max)Gb_Max=b_Max;
        if(a_Min<Ga_Min)Ga_Min=a_Min;
        if(b_Min<Gb_Min)Gb_Min=b_Min;

        if((Options&4)==4){ 			//if in hold mode, transfer to GHx_xxx since Gx gets reset every read or frame interval
          if(a_Max>GHa_Max)GHa_Max=a_Max;else if(HoldResetFlag&1)GHa_Max=a_Max;		//if max is negative, establish start point
          if(b_Max>GHb_Max)GHb_Max=b_Max;else if(HoldResetFlag&2)GHb_Max=b_Max;
          if(a_Min<GHa_Min)GHa_Min=a_Min;else if(HoldResetFlag&1)GHa_Min=a_Min;		//if min is positive, start
          if(b_Min<GHb_Min)GHb_Min=b_Min;else if(HoldResetFlag&2)GHb_Min=b_Min;
          HoldResetFlag=0;
        }else{					//if not in hold mode
          GHa_Max=Ga_Max;
          GHb_Max=Gb_Max;
          GHa_Min=Ga_Min;
          GHb_Min=Gb_Min;
        }


    if((_1_source != HIDE)||(_4_source>13)||((Title[TRIGG][SOURCE].Value==0)&&(EnableMeterCalc))){
           UTmp= (1000000000/TaS)*TaN;			//Calculate frequency using data from last frame
           UTmp-=QError(0,QStart[0],QEnd[0],UTmp);	//Adjust for any possible quantization error compensation
	   Average(0);					//Include all previous data transfers and average
    }
    if((_2_source != HIDE)||(_4_source==12)||(_4_source==13)||((Title[TRIGG][SOURCE].Value==1)&&(EnableMeterCalc))){          
           UTmp= (1000000000/TbS)*TbN;		
           UTmp-=QError(1,QStart[1],QEnd[1],UTmp);	
	   Average(1);
    } 	
    if(_3_source == CH_C){         
           UTmp= (1000000000/TcS)*TcN;		
	   Average(2);
    } 	
    if((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)){
           UTmp = (1000000000/TdS)*TdN;
	   Average(3);
    } 	

   if ((Title[TRIGG][SOURCE].Value>1)&&(Title[TRIGG][SOURCE].Value<4)){	  //if triggering from digital chs, alignment with analog chs shifts waveforms to the right,
     for(h=0; h<=(X_SIZE-5); h++){                     			  //shift back to original position    
      for(i=0;i<4;i++){
        TrackBuff[h*4+i] = TrackBuff[(h+5)*4+i];
      } 
     }
   }

if (_T_base > 16){					//jitter compensation for interpolated modes

  if(Title[TRIGG][SOURCE].Value==4){  			//if in A&B trig mode   

    if(TrigSourceEnable==0){

     if (Shift<0){
      for(h=X_SIZE;h>=-Shift; h--){                   
        TrackBuff[h*4] = TrackBuff[(h+Shift)*4];
      }
     }
     if (OldShift<0){
      for(h=X_SIZE;h>=-OldShift; h--){                   
        TrackBuff[h*4+1] = TrackBuff[(h+OldShift)*4+1];
      }
     }

    }else if(TrigSourceEnable==1){

     if (Shift<0){
      for(h=X_SIZE;h>=-Shift; h--){                   
        TrackBuff[h*4+1] = TrackBuff[(h+Shift)*4+1];
      }
     }
     if (OldShift<0){
      for(h=X_SIZE;h>=-OldShift; h--){                   
        TrackBuff[h*4] = TrackBuff[(h+OldShift)*4];
      }
     } 

    }
    
    Tmp=Shift;
    if(OldShift<Shift)Shift=OldShift;			//for blanking, use the largest neg value of the two
    OldShift=Tmp;

  }else{	//source not AltAB

    if (Shift>0){
      for(h=0; h<=(X_SIZE-Shift); h++){                   
        for(i=0;i<4;i++){
          TrackBuff[h*4+i] = TrackBuff[(h+Shift)*4+i];
        } 
      }
    }else if (Shift<0){
      for(h=X_SIZE;h>=-Shift; h--){                   
        for(i=0;i<4;i++){
          TrackBuff[h*4+i] = TrackBuff[(h+Shift)*4+i];
        } 
      }
    } 

  }   					//else... 

}  					//if Tbase...  


 if(!(OsBufferLogicFlag)){

   if (_T_base < 17){			//pre signal blanking						
     if (_T_base > 10){
       discard=4;
     }else discard=1;
     if((_T_base==10)&&(SerialStatus))discard=4;
   }else if (Shift<0){
     discard=-Shift;
   }else discard=0;
   if (_Mode==SGL) discard=4;
   if(OsChartFlag)discard=0;
							
   if (Shift>0)Rdiscard=Shift;else Rdiscard=0;		    //Shift is >0 when in highest time bases triggered from digital channel 	
   if (_T_base>16)Rdiscard+=7;else Rdiscard=0;              //and is compensated for interpolation

   for(jj=0; jj<discard; jj++){                             // Discard first pixels, fill with next valid sample
    TrackBuff[jj*4] = TrackBuff[discard*4];		    // conditional in draw.c blanks these 	
    TrackBuff[jj*4+1] = TrackBuff[discard*4+1];             // this way it doesn't misalign wave trace with trigger vernier...
   }
   if ((Title[TRIGG][SOURCE].Value<2)||(Title[TRIGG][SOURCE].Value==4))Tmp=6; else Tmp=0;      //add 6 to blank C and D if not shifting screen to the left 5 samples ie:triggering from analog channels
   if(OsChartFlag)Tmp=0; 

   for(jj=0; jj<(discard+Tmp); jj++){                       // Discard first pixels, fill with next valid sample
    TrackBuff[jj*4+2] = TrackBuff[(discard+Tmp)*4+2];          
   }

   if ((Title[TRIGG][SOURCE].Value<2)||(Title[TRIGG][SOURCE].Value==4)){				
     if ((_4_source==A_add_B)||(_4_source==A_sub_B))Tmp=0;      
   }

   for(jj=0; jj<(discard+Tmp); jj++){                           	
    TrackBuff[jj*4+3] = TrackBuff[(discard+Tmp)*4+3];
   }


    for(h=X_SIZE; h>(X_SIZE-Rdiscard); h--){                  //post signal blanking 
      for(i=0;i<4;i++){
        TrackBuff[h*4+i] = TrackBuff[(X_SIZE-Rdiscard)*4+i];         
      } 
    }

 }else discard=0;


    if(Det){                                                  //center filter output on wave
      for(h=0; h<(X_SIZE-(Filter/2)); h++){                   
        if((Det&1)&&(_1_source)){
          TrackBuff[h*4] = TrackBuff[(h+(Filter/2))*4];
        }
        if((Det&2)&&(_2_source)){
          TrackBuff[h*4+1] = TrackBuff[(h+(Filter/2))*4+1];
        }
      }
      for(h=(X_SIZE-(Filter/2)); h<X_SIZE; h++){                   
        if((Det&1)&&(_1_source))TrackBuff[h*4] = TrackBuff[(h-1)*4];
        if((Det&2)&&(_2_source))TrackBuff[h*4+1] = TrackBuff[(h-1)*4+1];
      }
    } 

    //if((_T_base<8)&&(OsBufferLogicFlag)&&(FPGAsubVer)&&(FPGAosFlag)){     //On the premise other modes can fall through at least once
      if(_T_base<8){                                                        
        if ((JumpCnt>(392+Xtend))||((_State.Value==2)&&(DownConvertMode==0))){  //to carry enableFFT set at end of DrawWindow with FrameEndFlag
          EnableFFT=1;                                                      //"Short circuits" loop in case of fast fall through with 1 frame
          ResetEnableFFT=1;                                                 //Disables set at end of DrawWindow
        }
      }


	// FFT /////////////////////////////
#warning Gabo: FFT disabled
#if 0
        u8 treshold=6;
        if(AutoFFT==0)treshold=75;		//log mode, uses high gain
	if((_4_source>10)&&(_Mode!=X_Y)&&   ((EnableFFT==1)||(_T_base>7)||(__Get(FIFO_START)==0)||(_Mode==SCAN))   ){
           EnableFFT=2;
	   for(i=0; i<FFTSize;i++)	fi[i] = 0;
	   fix_fft(fr, fi, FFTSize);

           fr[1]=0; 
           fr[0]=0; fi[0]=0;                            					            
	   for (i=0; i < (FFTSize/2); i++)
	     {
		X= fr[i];      /* Real */		
		Y= fi[i];      /* Imag */    
                fi[ i ] = Int_sqrt((X*X)+(Y*Y));	//load in fi[x] instead of fr[x], so result can persist after loading input in fr[x]
                if(fi[i]<0)fi[i]=0;        

                if((_4_source==15)||(_4_source==13)){   //wide bandwidth, sums peak with adjacent bins, so need to leave space before next peak
                  if(i>3){         			//was 5    3 now works with  i>5 on leftskirt conditional and fr[1] cleared 
                    if((fi[i-2]>=fi[i-3])&&(fi[i-2]>fi[i-1])&&(fi[i-2]>treshold)){	        //detect peak (i= -2 =peak), threshold
                      fi[i-2]=Int_sqrt((fi[i-2]*fi[i-2])+(fi[i-3]*fi[i-3])+(fi[i-1]*fi[i-1]));  //sum with neighbors
                      fi[i-3]=2+(fi[i-2]>>2);					      		//suppress left skirt (was >>3 for hamming W)                      
                      LeftSkirt=i-3;						      		//save it's position	 
                    }
                    if(((i-5)==LeftSkirt)&&(i>5)){fi[i-3]=fi[i-5];LeftSkirt=0;}  //suppress right skirt of summed peak if next bin skipped
                  }  

                  if(i>7){
                    fi[i-7]=Log10Process(i-7);          //Processed bins in summing mode
                  } 

                }else{
                    fi[i]=Log10Process(i);               //In Hann only mode
                }
 	     }

             if((_4_source==15)||(_4_source==13)){
                for(i=249;i<256;i++){
                    fi[i]=Log10Process(i);                    //Rest of bins in summing mode
                }
             }
          } 
	//////////////////////////// FFT ///
#endif
      if((_T_base<11)&&(JumpCnt>=get_bag_max_buf())){                           
          if(UpdateBackground==3)UpdateBackground=0;                            
            else if(UpdateBackground>0){
              UpdateBackground++;
              if(_T_base<5)UpdateBackground=3;
            }     
      }
      if(_T_base>10)FrameEndFlag=1; 

}


void Send_Data(s16 Va, s16 Vb, u8 C_D, u8 osB, u16 n)  // output display data
{
  s32 Tmp = 0, i;
  u8 Start;
  if((Title[TRIGG][SOURCE].Value>1)&&(Title[TRIGG][SOURCE].Value<4))Start=5;else Start=0;

  i = n*4;

  if(Va >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK1] = Y_BASE+Y_SIZE-1;
  else if(Va <= Y_BASE+1)  TrackBuff[i + TRACK1] = Y_BASE+1;
  else                     TrackBuff[i + TRACK1] = Va;                
  if(Vb >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK2] = Y_BASE+Y_SIZE-1;
  else if(Vb <= Y_BASE+1)  TrackBuff[i + TRACK2] = Y_BASE+1;
  else                     TrackBuff[i + TRACK2] = Vb;
  

 if(OsBufferLogicFlag||OsChartFlag){

  if(C_D>= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK3] = Y_BASE+Y_SIZE-1;
  else if(C_D<= Y_BASE+1)  TrackBuff[i + TRACK3] = Y_BASE+1;
  else                     TrackBuff[i + TRACK3] = C_D;
  if(osB>= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK4] = Y_BASE+Y_SIZE-1;
  else if(osB<= Y_BASE+1)  TrackBuff[i + TRACK4] = Y_BASE+1;
  else                     TrackBuff[i + TRACK4] = osB;

 }else{

  if(C_D & 1)  TrackBuff[i + TRACK3] = c_Max;
  else         TrackBuff[i + TRACK3] = _3_posi;
  
  switch (_4_source){                       
  case A_add_B:
    Tmp = Posi_412 + Va + Vb;
    break;
  case A_sub_B:
    Tmp = Posi_41_2 + Va - Vb;	
    break;
  case C_and_D:
    if((~C_D)& 3) Tmp = _4_posi; 
    else          Tmp = d_Max;
    break;  
  case C_or_D:
    if(C_D & 3)   Tmp = d_Max; 
    else          Tmp = _4_posi;
    break;  

 if(n>=Start){
  case REC_1:
    Tmp = Posi_4F1 + FileBuff[n-Start];  
    break;
  case REC_2:
    Tmp = Posi_4F2 + FileBuff[n+400-Start];  
    break;
  case REC_3:
    Tmp = Posi_4F3 + FileBuff[n+800-Start];  
    break;
  case REC_4:
    Tmp = Posi_4F4 +  FileBuff[n+1200-Start];  
    break;
 }

  case (10)...(15):		//fft functions	
    break;
  default:
    if(C_D & 2)  Tmp = d_Max;
    else         Tmp = _4_posi;
  }
  if(Tmp >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK4] = Y_BASE+Y_SIZE-1;
  else if(Tmp <= Y_BASE+1)  TrackBuff[i + TRACK4] = Y_BASE+1;
  else                      TrackBuff[i + TRACK4] = Tmp;

 }

}

/*******************************************************************************
 Synchro: scan synchronization, waveform display by setting the mode
*******************************************************************************/
void Synchro(void)  // scan synchronization: AUTO, NORM, SGL, NONE, SCAN modes
{ 
u32 waste=0;
freerun=0;
   if((Update)&&(_Mode==NORHLD))UpdateCursorMeter();

   if(HoldOnNext==1) {
    _State.Value = HOLD;                                    	       // for single mode
    _State.Flag |= UPDAT;
    HoldOnNext=0;
    return;
   }

if (BailoutFlag>0) {BailoutFlag=0; goto BailOut;} 				//if in trigger delay

if (FrameMode>0)								//single frame buffer
 {
  switch (_Mode)
  {  
     case X_Y:
     case AUTO:
      if(!OsBufferLogicFlag)__Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2)){
        if (entryflag>0){
          JumpCnt=0;
          entryflag=0;
        }
        Process();
        Wait_Cnt=shortwait[_T_base];                                              
        exitflag=1;                                                          
      }else if ((Wait_Cnt==1)&&(_T_base < 9)){
        if (_T_base <6)__Set(FIFO_CLR, W_PTR);                              
      }else if(Wait_Cnt ==0){
        if ((_T_base > 8)||((OsBufferLogicFlag)&&(_T_base > 7))){
          freerun=1;
          entryflag=0;
        }else{
          freerun=2;						  
          entryflag=1;                                                 
        }
        if (JumpCnt > (392+Xtend))JumpCnt = 0;                     
        Process();
      }
      break;
    case NORH:                      
    case NORHLD:
      if(Title[TRIGG][SOURCE].Value !=4){
        if(!OsBufferLogicFlag)__Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);
      }  
      if((__Get(FIFO_START)!=0)||(_State.Value==2)){                   //if device triggered or in pause mode
        Process();                                 
        Wait_Cnt=Wait[_T_base];
      }else{
        if(Wait_Cnt==0){
          Wait_Cnt = 1;
        }
      }
      break;
    case NORC:					
      if(!OsBufferLogicFlag)__Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2)){
          Process();
          Wait_Cnt=Wait[_T_base];
        } else if(Wait_Cnt==0)						//was ==1
         {
          ResetSum();	                      
          if (_T_base < 7) cleardatabuf(2);
          ClearTrackBuff(0);
          Wait_Cnt=Wait[_T_base];						//added
      }
      break;
   case SGL:					
      if(!OsBufferLogicFlag)__Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2))Process();         
      break;
    case SCAN:								
      if (_T_base>11){ 
        __Set(TRIGG_MODE, UNCONDITION);            	// works best at high sweep rates
      }else{                                          // works better at low rates
       __Set(TRIGG_MODE,3);  		                  // same as track1 (0) <<3 + 3 (>vt) (do not use digital chs...) 
       __Set(V_THRESHOLD,255);                        // level then set out of range. Prevents triggering(vt>255), with fifo "splice" shifted left 150pix, gives smooth display at low speeds
      }
      if (_T_base>7){ 
        freerun=1;
        Process();                                  	
      }else{ 								//tbase<8
         freerun=2;
         if ((FlagMeter==0) && (JumpCnt> 389)) JumpCnt=0;
         if ((FlagMeter>0) && (JumpCnt> 304)) JumpCnt=0;
         Process();
      } 
   } //switch
 }
 else										//regular large buffer mode
 {
 switch (_Mode)
   { 
   case X_Y:
   case AUTO:
       __Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2)){
        Process();                                 
        Wait_Cnt = Wait[_T_base];
      } else if(Wait_Cnt==0) {
          if(_Mode==X_Y){
            if(JumpCnt>=BufferSize)JumpCnt = 0;
          }else{if(JumpCnt >= 4095)  JumpCnt = 0;} 
          Process();   
      } break;
    case NORH:
    case NORHLD:
      if(Title[TRIGG][SOURCE].Value !=4) __Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2)){
        Process();                                 
        Wait_Cnt = Wait[_T_base];
      } else if(Wait_Cnt==0) {
       Wait_Cnt = Wait[_T_base];
      } break;
     case NORC:
      __Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2)){
        Process();                                 
        Wait_Cnt = Wait[_T_base];
      } else if(Wait_Cnt==0) {
       ResetSum();	                      
       ClearTrackBuff(0);		      // clear screen
        Wait_Cnt = Wait[_T_base];
      } break;
   case SGL:
      if(!OsBufferLogicFlag)__Set(TRIGG_MODE,(_Tr_source <<3)+TriggerType);  
      if((__Get(FIFO_START)!=0)||(_State.Value==2))Process();         
      break;
    case SCAN:								
      if (_T_base>11){ 
        __Set(TRIGG_MODE, UNCONDITION);               	
      }else{
       __Set(TRIGG_MODE,3);  
       __Set(V_THRESHOLD,255); 
      }
      if (_T_base>7){ 
        freerun=1;
        Process();                                  	
      }else{ 								//tbase<8
         freerun=2;
         if(ChartLogic()){
           if(JumpCnt >= 4095){
             JumpCnt = 0; 
             if(AutoSaveBuf==1){
               if(ErrorFlag==0)ErrorFlag=Save_Buf(Title[9][1].Value++);
             }else if(AutoSaveBuf==2){
               if(ErrorFlag==0)ErrorFlag=Save_Csv(Title[9][1].Value++);
             }
             if(ErrorFlag){ErrorFlag=0;AutoSaveBuf=0;}             
           } 
         }else{
           if ((FlagMeter==0) && (JumpCnt> 389)) JumpCnt=0;
           if ((FlagMeter>0) && (JumpCnt> 304)) JumpCnt=0;
         } 
         Process();
      } 
   }  										

 }   							 // end else (framemode=0)

  if (_Mode==SCAN) Wait_Cnt = 1;	     	

  if(_Mode==X_Y){
    if(JumpCnt>=BufferSize){
      JumpCnt = 0;
      __Set(FIFO_CLR, W_PTR);
      FrameEndFlag=1;
    }
  }

  if ((FrameMode > 0)&&(_Mode!=SGL)){                    // in single frame buffer mode
    if (_Mode==SCAN){
      if ((FlagMeter==0) && (JumpCnt> 389)) JumpCnt=0;
      if ((FlagMeter>0) && (JumpCnt> 304)) JumpCnt=0;
    } 

    if (JumpCnt>(392+Xtend)){

      if((__Get(FIFO_START)!=0)&&(freerun!=2)){         // resetting FIFO after start flag and frame completed provides proper triggering in single frame buffer mode
        FrameEndFlag=1;
        JumpCnt=0;
        if ((Options&1)&&(_T_base < 10)&&(!OsBufferLogicFlag)&&(_Mode!=AUTO)&&(_Mode!=X_Y)&&(_Mode!=SGL)&&(_Mode!=SCAN)){	//trigger hold mode, works only with time bases where process is slower than data transfers
          CountUnread=0;
BailOut:
          if (Title[7][3].Value>0){				       //add additional adjustable delay just before resetting write pointer

            DelayLoopBreak=50;				//restrict max time in loop to 1 sec to prevent possible lockup with empty buffer
            while (CountUnread<Title[7][3].Value){

              if (__Get(FIFO_EMPTY)==0){		//if FIFO has data to read
                waste =__Read_FIFO();			//FIFO reads provide relative timing for delay
                CountUnread++;
              }else{					//while loop can lockup with empty buffer          
                if(Update>1)break;			//when initiating, drop out to reset FIFO
                if(DelayLoopBreak==0)break;      	//interrupt based breakout after 1 sec if necessary           
              }
              if ((CountUnread>0)&&((CountUnread%(TrigDelayLoop[_T_base]))==0)){    //Allow program to operate on a time base related interval while counting delay
                BailoutFlag=1;							    	
                break;
              }
            } 

          }  //if Title... (trig hold value > 0)
        }    //if options... (set trigger hold mode)

        if (BailoutFlag==0){

	    if (Title[TRIGG][SOURCE].Value==4){AlternateChannel();AltHoldoff=AltDelay[_T_base];}
            __Set(FIFO_CLR, W_PTR);			

        }

        waste++;			//compiler needs to do something with this or generates error... (used to read FIFO)
      }          //if get fifo start
    }            //if jumpcnt>
  } 		 //if framemode>0

  if((_Status == RUN)&&((_T_base > 10)||((_T_base>9)&&(Tim2Factor>20)))&&(_T_base < 14)&&(_Mode != SGL)){  	    				
    if ((__Get(FIFO_START)!=0)&&(FrameMode >0))	{				    //prevents "double looping" in single window buffer mode
        if ((Title[TRIGG][SOURCE].Value==4)&&(_Status == RUN)) AlternateChannel();  //causing problems in 50, 100, and 200uS/div ranges
        __Set(FIFO_CLR, W_PTR);	 					  
    }          								  		
  }

  if ((_Status == RUN)&&(__Get(FIFO_FULL)!=0)){                         // FIFO is full 
    if(_Mode != SGL) {
      if (Title[TRIGG][SOURCE].Value==4){
        AlternateChannel();
        if(_T_base < 10)AltHoldoff=AltDelay[_T_base];
      } 
      __Set(FIFO_CLR, W_PTR);   //if mode is not single                                         // FIFO write pointer reset

    }else if(_T_base < 11){	                                        //in SGL mode, added reset after moving to slow mode
      __Set(FIFO_CLR, W_PTR);                                           //also added reset JumpCnt on hold/run reset 
    }

    if(_Mode!=X_Y){
      if (FrameMode == 0)FrameEndFlag=1;                                
      if(!ChartLogic())JumpCnt=0;                                       //FIFO full jumpcnt reset irrelevant in scan/chart mode
    } 

    if(_Mode == SGL){
      HoldOnNext=1;
    }
  }

  if(AutoSetFlag)AutoSet();

  if(((FrameEndFlag)||((DownConvertMode)&&(FFTt1Mode)))&&(DownConvertShiftEnable)){
    FFTdownConvert(0);
    if(DownConvertShiftEnable==3)DownConvertShiftEnable=1;
    else if (DownConvertShiftEnable==2)DownConvertShiftEnable=0;
    else DownConvertShiftEnable--;
  }

  if ((_Mode != AUTO)||(_T_base > 8)||(exitflag==0)||(entryflag==0)) Draw_Window(); //Executing time consuming screen update AFTER resetting FIFO rather than before
                                             					    //issue of slow/random pulses not triggering
  if ((_Status == RUN)&&(__Get(FIFO_START)==0)&&(Title[TRIGG][SOURCE].Value==4)){   //alternate channels after delay if selected ch not triggered	
     if(_T_base > 9){
         cleardatabuf(TrigSourceEnable);
         AlternateChannel();
         __Set(FIFO_CLR, W_PTR);
     }else{
       if(AltHoldoff==0){
         AltHoldoff=AltDelay[_T_base];
         cleardatabuf(TrigSourceEnable);
         AlternateChannel();
         __Set(FIFO_CLR, W_PTR);
       }
       if(AltHoldoff>0)AltHoldoff--;
     }
  }

}

u16 get_bag_max_buf(void) {
u16 out = 4096;
  if(((_Mode==SCAN)&&(!ChartLogic()))||((ChartLogic())&&(FrameMode>0))){					
    if (FlagMeter==0)out=390; else out=305;
    if (_4_source>9)out=512;              //for FFT
    if (_T_base>9) out=390+150;	          //moves trigger splice out of view to left edge of screen at fast time bases for cleaner display
    if (AutoSetFlag)out=670;
  }else if (((FrameMode>0)&&(_Mode!=SGL))||(OsBufferLogicFlag)){
    out =  (393+Xtend);		          //modify buffer size if meters are on or off or XPOS shifted
    if(OsBufferLogicFlag)FlagFrameMode=1;
  }else if(_Mode==X_Y)out=BufferSize;
  if(out>4096)out=4096;
  return out;
}	


void Beeper(u16 ms){
  if(Beep_mS<20) __Set(BEEP_VOLUME, 5*(Title[VOLUME][CLASS].Value-1)); //don't call beeper if currently beeping, seems to shut it off...
  Beep_mS = ms;
}

void AlternateChannel(void){
      	      if(TrigSourceEnable==1)TrigSourceEnable=0; else TrigSourceEnable=1;		     //cycle to next channel
	
	      if(TrigSourceEnable==0){								     //set up ch if enabled	
                if(Title[0][0].Value==0){							     //otherwise skip to next ch	
                  TrigSourceEnable=1;
                }else{
                  ReverseBitMask=0xFFFFFF00;
                  BitMask=0xFF;
                }    
              }

	      if(TrigSourceEnable==1){
                if(Title[1][0].Value==0){
                  TrigSourceEnable=0;
                }else{
                  ReverseBitMask= 0xFFFF00FF;
                  BitMask=0xFF00;	
                }    
              }

              Update_Trig(0);			
}


void cleardatabuf(u8 service)   //Clears/replaces n samples in DataBuf
{
						   
 u32 sample=(B_Posi+ADCoffset);		
 u16 i;

  sample <<= 8;

  if(((OsBufferLogicFlag)&&(!OSAvg))||(OsChartFlag)){
    sample|=((u32)(A_Posi+ADCoffset)<<16);
    sample|=((u32)(B_Posi+ADCoffset)<<24);
  }

  if(service==0){                                            //clear ch A
    sample=(A_Posi+ADCoffset); 		
    for (i=0; i<bag_max_buf; i++){
      DataBuf[i]&=0xFFFFFF00;      
      DataBuf[i]|=sample;
    }
  }

  if(service==1){                                            //clear ch B
    for (i=0; i<bag_max_buf; i++){
      DataBuf[i]&=0xFFFF00FF;      
      DataBuf[i]|=sample;
    }
  }

  if(service==2){                                            //clear ch A & B 
    sample|=(A_Posi+ADCoffset);			
    for (i=0; i<bag_max_buf; i++) DataBuf[i]=sample;      
  } 

  if(service==3){                                            //clear entire buffer
    sample|=(A_Posi+ADCoffset);			
    for (i=0; i<4096; i++) DataBuf[i]=sample;      
  } 

}

void TransferFIFO(s16 i,u8 service){
  static   s16 TmpA=0,TmpB=0;
  static   u8  Counter=0;
  register u32 swap;
  static   u32 loA=0x000000FF,loB=0x000000FF;
  static   u8  hiA=0;
  static   u16 hiB=0;
  static   u16 ChartSampleCounter=0;
  u32 Transfer=0;
  u8  OSFactor=8;

	    if((service)&&(!ChartLogic())){                                      
              DataBuf[i] &= ReverseBitMask;                                     //preserve previous data on alternate channel
              DataBuf[i] |= (__Read_FIFO() & BitMask);                          //only load triggering channel if in alternate mode
	    } else {

              if((OsBufferLogicFlag)&&(CalibrateMode==0)){

                  if(_T_base<16)OSFactor=8;else OSFactor=6;
                  OsTransferCompleteFlag=0;
                  if(i==0){TmpA=0;TmpB=0;Counter=0;}

                  while(Counter++<OSFactor){
                    if(_T_base>7){
                      if(_T_base==8){Waste=0; while(__Get(FIFO_EMPTY)){if(Waste++>5000)break;}}
                      Transfer=__Read_FIFO();
                    }else{
                      if(__Get(FIFO_EMPTY)==0)Transfer=__Read_FIFO();else {Counter--;return;}
                    } 

                    swap=0x300;                                             //this is only used in older versions
                    swap &= Transfer;
                    if ((swap==0x100)||(swap==0x200))Transfer^=0x300; 

                    swap=Transfer&0xFF;
                    if(swap>hiA)hiA=swap;
                    if(swap<loA)loA=swap;                  
                    TmpA+=swap;
                    RMSdata[0][i]=swap;

                    swap=(Transfer>>8)&0xFF;
                    if(swap>hiB)hiB=swap;
                    if(swap<loB)loB=swap;                  
                    TmpB+=swap; 
                    RMSdata[1][i]=swap;

                  }                                                          //while

                DataBuf[i]=hiA+(hiB<<8)+(loA<<16)+(loB<<24);
                OsFFTData[0][i]=(TmpA+(OSFactor/2))/OSFactor;
                OsFFTData[1][i]=(TmpB+(OSFactor/2))/OSFactor;
                Counter=0;
                OsTransferCompleteFlag=1;
                hiA=0; hiB=0;
                loA=0x000000FF; loB=0x000000FF;
                TmpA=0; TmpB=0;

              }else{
                                                                                       //(ChartLogic()&&ChartMode)
                if((ChartLogic())&&(CalibrateMode==0)){

                  while(__Get(FIFO_EMPTY)==0){ 
                    Transfer=__Read_FIFO();

                    swap=0x300;                                                        //this is only used in older versions
                    swap &= Transfer;
                    if ((swap==0x100)||(swap==0x200))Transfer^=0x300; 

                    if(ChartMode==0){                                                  //Averaging mode
                      loA+=Transfer & 0xFF;                                            
                      loB+=(Transfer & 0xFF00)>>8;                                     
                    }else{
                      swap=(Transfer&0xFF00)>>8;                                       //peak display
                      Transfer&=0xFF;                                       
                      if(Transfer<loA)loA=Transfer;
                      if(swap<loB)loB=swap;
                      if(Transfer>hiA)hiA=Transfer;
                      if(swap>hiB)hiB=swap;                                            
                    } 
                    ChartSampleCounter++;

                    if(ChartSampleCounter>=TbaseSub[SubIndex-1].S_OS_Size){
                      if(ChartMode==0){                                                //Averaging mode
                        loA/=TbaseSub[SubIndex-1].S_OS_Size;                           
                        loB/=TbaseSub[SubIndex-1].S_OS_Size;                           
                        DataBuf[i]=(loB<<8)+loA;                                       
                        DataBuf[i]|=Transfer&0x00030000;                               //add digital channels sampled at end of period                                                          
                        loA=0;                                                         
                        loB=0;
                      }else{                                                           //peak display 
                        DataBuf[i]=hiA+(hiB<<8)+(loA<<16)+(loB<<24);                 
                        hiA=0; hiB=0;                                                
                        loA=0x000000FF; loB=0x000000FF;                              
                      } 
                      ChartSampleCounter=0;  
                      OsTransferCompleteFlag=1;
                      break;
                    }else OsTransferCompleteFlag=0;                     
                  }  

                }else{
                  DataBuf[i] = __Read_FIFO();                                        //normal mode
                }//if (chartmode && calibratemode==0), else (regular mode and calibrate mode)

              }//if  (OS buffer mode && calibratemode==0), else

            }//if (service && !chartlogic),else   (service= A&B mode)

            if((CalibrateMode)||(((service==0)||(TrigSourceEnable==1))&&(!ChartLogic())&&
              ((!OsBufferLogicFlag)||(Title[T_BASE][BASE].Value>16)))){                        //only swap bits if loading chB 
              swap=0x300;
	      swap &= DataBuf[i];
	      if ((swap==0x100)||(swap==0x200))DataBuf[i]^=0x300; //swap 2 least significant bits of chB, fixes error in FPGA programming (V2.61)
            }
}

void SetOffset(u8 channel,u8 range, s16 Ypos) {
s32 temp;

      if(channel==0)temp=Ka3[range];else temp=Kb3[range];
      temp=(1024+temp)*(Ypos-ScalingOffset);			               //set Ka3 zero axis, allows scaling optimisation
      if(temp>0)temp+=512; else temp-=512;
      temp=(temp/1024)+ADCoffset+ScalingOffset;
      if (temp<0)temp=0;						       //should not go anywhere close to 0 but just to make sure
      if(channel==0)__Set(CH_A_OFFSET, temp); else __Set(CH_B_OFFSET, temp);
}

void BatLevelCompensation(void){
u8 i;								//make sure both sets are available or don't use

 if((LoBatLevel[0]>2800)&&(LoBatLevel[0]<5200)&&(HiBatLevel[0]>2800)&&(HiBatLevel[0]<5200)){  	         		
  for(i=0;i<10;i++){
    Ka1[i]=InterpolateS8(0,LKa1[i],HKa1[i]);
    Ka2[i]=InterpolateU16(0,LKa2[i],HKa2[i]);
    Ka3[i]=InterpolateS8(0,LKa3[i],HKa3[i]);
  }
 }

 if((LoBatLevel[1]>2800)&&(LoBatLevel[1]<5200)&&(HiBatLevel[1]>2800)&&(HiBatLevel[1]<5200)){	         		
  for(i=0;i<10;i++){
    Kb1[i]=InterpolateS8(1,LKb1[i],HKb1[i]);
    Kb2[i]=InterpolateU16(1,LKb2[i],HKb2[i]);
    Kb3[i]=InterpolateS8(1,LKb3[i],HKb3[i]);
  }
 } 
}


s8 InterpolateS8(u8 Ch, s8 L8,s8 H8){
s32 Tmp;
  Tmp=((__Get(V_BATTERY)-LoBatLevel[Ch])*(H8-L8));
  if(Tmp>0)Tmp+=(VDiff[Ch]/2); else Tmp-=(VDiff[Ch]/2);
  return (Tmp/VDiff[Ch])+L8;
}

u16 InterpolateU16(u8 Ch, u16 L16,u16 H16){
s32 Tmp;
  Tmp=((__Get(V_BATTERY)-LoBatLevel[Ch])*(H16-L16));
  if(Tmp>0)Tmp+=(VDiff[Ch]/2); else Tmp-=(VDiff[Ch]/2);
  return (Tmp/VDiff[Ch])+L16;
}

s32 QError(u8 Ch, u16 Start, u16 End, u32 Utmp){	
s32 b,ab,tmp=0;
		//quantization error compensation; improves accuracy of frequency/period meters on analog channels

     b=(QParam(Ch,Start,1)*(QParam(Ch,End,0)+QParam(Ch,End,1)))-(QParam(Ch,End,1)*(QParam(Ch,Start,0)+QParam(Ch,Start,1)));
     ab=(QParam(Ch,Start,0)+QParam(Ch,Start,1))*(QParam(Ch,End,0)+QParam(Ch,End,1));
     tmp=Utmp/ab;
     tmp=tmp*b;
     if(Ch==0)tmp=tmp/TaS; else tmp=tmp/TbS;
     return tmp;

}

s16 QParam(u8 Ch, u16 Position,u8 service){	
u8 a[2],b[2];

          if(OsBufferLogicFlag){
            b[Ch]=RMSdata[Ch][Position]-ADCoffset;
            a[Ch]=RMSdata[Ch][Position-1]-ADCoffset;
          }else{
            if(Ch==0){
              b[0]=(DataBuf[Position]& 0xFF)-ADCoffset;
              a[0]=(DataBuf[Position-1]& 0xFF)-ADCoffset;
            }else{
              b[1]=((DataBuf[Position]>>8)&0xFF)-ADCoffset;
              a[1]=((DataBuf[Position-1]>>8)&0xFF)-ADCoffset;
            } 
          }
          
          if((InvertA)&&(Ch==0)){
            b[0]-=A_Posi;
            b[0]=-b[0];
            b[0]+=A_Posi;

            a[0]-=A_Posi;
            a[0]=-a[0];
            a[0]+=A_Posi;
          }
          if((InvertB)&&(Ch>0)){
            b[1]-=B_Posi;
            b[1]=-b[1];
            b[1]+=B_Posi;

            a[1]-=B_Posi;
            a[1]=-a[1];
            a[1]+=B_Posi;
          }

          if(service){								//"b"								
            if(Ch==0){ 
              return b[0]-LastA_Mid;                                 
            }else{
              return b[1]-LastB_Mid;                                   
            }
          }else{								//"a"
            if(Ch==0){ 
	      return FirstA_Mid-a[0];                               
            }else{   
	      return FirstB_Mid-a[1];                        
            }
          }

}


void Average(u8 Ch){

              //sum averaging of all FIFO transfers; improves resolution/accuracy of all meters at faster timebases (>~5mS/div)

           if((_T_base > 10)||((JumpCnt==get_bag_max_buf())&&(_T_base>6)&&(FrameMode>0))){
             RunAvg(Ch);				//time/freq parameters
             if(Ch==0){					//voltage values
               VxAvg[0]=VRunAvg(0,a_Avg,VxAvg[0]);
               VNAvg[0]--;				//reset VN counters to do SSQ
               VxSsq[0]=VRunAvg(0,a_Ssq,VxSsq[0]);
             }else if(Ch==1){
               VxAvg[1]=VRunAvg(1,b_Avg,VxAvg[1]);
               VNAvg[1]--;
               VxSsq[1]=VRunAvg(1,b_Ssq,VxSsq[1]);
             }

             switch(Ch){				//summing for TH, TL, %duty
             case 0:
               PxS[Ch]+=PaS;             
               TxS[Ch]+=TaS;             
               TxN[Ch]+=TaN;             
               break;
             case 1:
               PxS[Ch]+=PbS;             
               TxS[Ch]+=TbS;             
               TxN[Ch]+=TbN;             
               break;
             case 2:
               PxS[Ch]+=PcS;             
               TxS[Ch]+=TcS;             
               TxN[Ch]+=TcN;             
               break;
             case 3:
               PxS[Ch]+=PdS;             
               TxS[Ch]+=TdS;             
               TxN[Ch]+=TdN;             
             }
           }else{
             Sum[Ch]=UTmp; 				//if frame rates slower than meter reads no summing possible

             switch(Ch){
             case 0:
               VxAvg[Ch]=a_Avg;
               VxSsq[Ch]=a_Ssq;
               PxS[Ch]=PaS;             
               TxS[Ch]=TaS;             
               TxN[Ch]=TaN;             
               break;
             case 1:
               VxAvg[Ch]=b_Avg;
               VxSsq[Ch]=b_Ssq;
               PxS[Ch]=PbS;             
               TxS[Ch]=TbS;             
               TxN[Ch]=TbN;             
               break;
             case 2:
               PxS[Ch]=PcS;             
               TxS[Ch]=TcS;             
               TxN[Ch]=TcN;             
               break;
             case 3:
               PxS[Ch]=PdS;             
               TxS[Ch]=TdS;             
               TxN[Ch]=TdN;             
             }
           }
}

s32 VRunAvg(u8 Ch, s32 Value, s32 Sum){ 		//running average of voltage values
       if(Value>Sum){
         Sum+=((Value-Sum)/++VNAvg[Ch]);
       }else if(Sum>Value){
         Sum-=((Sum-Value)/++VNAvg[Ch]);
       }else VNAvg[Ch]++;
       return Sum; 
}

void RunAvg(u8 Ch){
   if(UTmp>Sum[Ch]){					//moving sum average for frequency/period
     Sum[Ch]+=((UTmp-Sum[Ch])/++NAvg[Ch]);
   }else if(Sum[Ch]>UTmp){				
     Sum[Ch]-=((Sum[Ch]-UTmp)/++NAvg[Ch]);
   }else NAvg[Ch]++;         
}


void ResetSum(void){
u8 Ch;

        if(DownConvertMode){  
          if(DownConvertInitiate==1){
            PrecisionLevel=0;
            Display_Value(0);
            FlagFrameMode=1;
            Update_View_Area();
            DownConvertInitiate=2; 
          }else if(DownConvertInitiate==2){
            FFTdownConvert(0);
            DownConvertInitiate=0;
            if((FFTt1Mode==0)&&(DownConvert>1))FFTdownConvert(4);
          }
        } 

        for(Ch=0;Ch<4;Ch++){ 
          Sum[Ch]=0;
          NAvg[Ch]=0;
        }

        if(EnablePaS){				//only reset after read, 1/second for TH,TL,%duty in large meters
          for(Ch=0;Ch<4;Ch++){ 
            PxS[Ch]=0;             
            TxS[Ch]=0;             
            TxN[Ch]=0;             
          }

        }
        VxAvg[0]=A_Posi*NSamples;
        VxAvg[1]=B_Posi*NSamples;
        for(Ch=0;Ch<2;Ch++){ 
          VxSsq[Ch]=NSamples/2;
          VNAvg[Ch]=0; 
          ClearMinMax(Ch+1);
        }

}

void ClearMinMax(u8 Channels){
  if(Channels&1){Ga_Max=(-0x7FFF);Ga_Min=0x7FFF;}       
  if(Channels&2){Gb_Max=(-0x7FFF);Gb_Min=0x7FFF;}
  if((Options&4)==0){					//if hold mode is not set, reset GHx as this is used to transfer max and min values
    ClearHold(3);
  }       
}

void ClearHold(u8 Channels){
  HoldResetFlag=Channels;
  if(Channels&1){GHa_Max=A_Posi;GHa_Min=A_Posi;}       
  if(Channels&2){GHb_Max=B_Posi;GHb_Min=B_Posi;}       
}

void ClearTrackBuff(u8 service){
u16 i;
  if(service)ClearLeadingEdge=1; else ClearLeadingEdge=0;             

  if(service==2){
    for(i=0;i<397;i++){                             
      TrackBuff[(i*4)]   = A_Posi;		     	
      TrackBuff[(i*4)+1] = B_Posi;             
      TrackBuff[(i*4)+2] = _3_posi;		     	
      TrackBuff[(i*4)+3] = _4_posi;             
    }
  }else{
    for(i=0; i<X_SIZE*4; i++)TrackBuff[i]=0;                   
  }

}

s32 Log10Process(u16 i){
#warning Gabo: FFT disabled
#if 0
u8 Charact=0;           
s32 n=fi[i];
          if(AutoFFT==0){				//log scaling
            if(n==10)n=5;else{				//special case, would return 0 otherwise because of break in dual scale mant[]
              while (n>99){ 
                n/=10;
                Charact++;
              }
              n=((50*((Charact*100)+Log10Mant[n]))+50)/100; 
            }
          } 
          if(n>198)n=198;   	                        
          if(AutoFFT>0)n=(n*(100+((198-n)/2)))/100;    //2:1 compression, used for regular scaling
          return n;
#endif
return 0;
}

void WaveGen(void){
u16 i;
u16 j=0;
u16 YShift=0;

  if(_Kind==SINE)YShift=4095;

  if(_Kind==SAW){
    for(i=0;i<(720/ScaleIndex[_Frqn]);i++)                                                      
      ATT_DATA[i]=(((Title[OUTPUT][OUTATT].Value*WaveValue(j+=ScaleIndex[_Frqn]))/100)+2048);
  }else if(_Kind==DIGI){
    for(i=0;i<(720/ScaleIndex[_Frqn]);i++) 
      ATT_DATA[i]=(((Title[OUTPUT][OUTATT].Value*DIGI_DATA[(i/(360/ScaleIndex[_Frqn]))])/100)+2048);
  }else if(_Kind==7){
    return;
  }else{
    if(_Frqn<15){ 
      for(i=0;i<(720/ScaleIndex[_Frqn]);i++){                                                      
        if(i<(180/ScaleIndex[_Frqn]))ATT_DATA[i]=(((Title[OUTPUT][OUTATT].Value*WaveValue(j+=ScaleIndex[_Frqn]))/100)+2048);
          else if(i<(360/ScaleIndex[_Frqn]))ATT_DATA[i]=(((Title[OUTPUT][OUTATT].Value*WaveValue(j-=ScaleIndex[_Frqn]))/100)+2048);
            else if(i<(540/ScaleIndex[_Frqn]))ATT_DATA[i]=YShift-(((Title[OUTPUT][OUTATT].Value*(WaveValue(j+=ScaleIndex[_Frqn]))/100)+2048));
              else ATT_DATA[i]=YShift-(((Title[OUTPUT][OUTATT].Value*(WaveValue(j-=ScaleIndex[_Frqn]))/100)+2048));
      }

    }else if(_Frqn==15){

      if(_Kind==SINE){
        for(i=0;i<18;i++){
          ATT_DATA[i]=((Title[OUTPUT][OUTATT].Value*(Sine100K[i]-2048))/100)+2048;
        } 
      }
      if(_Kind==TRIANG){
        for(i=0;i<18;i++){
          ATT_DATA[i]=((Title[OUTPUT][OUTATT].Value*(Triangle100K[i]-2048))/100)+2048;
        } 
      }
    }else{
      if(_Kind==SINE){
        for(i=0;i<9;i++){
          ATT_DATA[i]=((Title[OUTPUT][OUTATT].Value*(Sine200K[i]-2048))/100)+2048;
        } 
      }
      if(_Kind==TRIANG){
        for(i=0;i<9;i++){
          ATT_DATA[i]=((Title[OUTPUT][OUTATT].Value*(Triangle200K[i]-2048))/100)+2048;
        } 
      }

    }
  }
}


s16 WaveValue(u16 j){
    if(_Kind==SINE) return (SIN_QUAD[j]-2048);
      else if(_Kind==TRIANG)return (((11372*j)+500)/1000);
        else if(_Kind==SAW)return (((5686*j)+500)/1000)-2048;     
          else return 0;
}


void InitiateCalData(void){
u8 i;

  for(i=0;i<10;i++){

    Ka1[i] = Kb1[i] = Ka3[i] = Kb3[i] = 0;
    Ka2[i] = Kb2[i] = 1024;

    HKa1[i] = HKb1[i] = HKa3[i] = HKb3[i] = 0;
    HKa2[i] = HKb2[i] = 1024;

    LKa1[i] = LKb1[i] = LKa3[i] = LKb3[i] = 0;
    LKa2[i] = LKb2[i] = 1024;
  }

}

void InitiateOSBuffers(void){
u16 i;
                for (i=0; i<bag_max_buf; i++){                                 
                  DataBuf[i]&=0x0000FFFF;                       
                  DataBuf[i]|=((DataBuf[i]&0x000000FF)<<16);      
                  DataBuf[i]|=((DataBuf[i]&0x0000FF00)<<16);
                  if((i>=WindowPosition)&&(i<(FFTSize+WindowPosition))){
                    OsFFTData[0][i-WindowPosition]=DataBuf[i]&0xFF;
                    OsFFTData[1][i-WindowPosition]=(DataBuf[i]>>8)&0xFF;
                  }      
                }
                for(i=0;i<397;i++){                             
                  TrackBuff[(i*4)+2] = TrackBuff[(i*4)];		     	
                  TrackBuff[(i*4)+3] = TrackBuff[(i*4)+1];             
                }

}

u16 RandomGen(void){      //random number gen to dynamically seed noise generator
static u32 Tmp=1746698375;
  Tmp=((u32)1664525*Tmp)+(u32)1013904223;
  Tmp>>=20;
  return Tmp&0x00000FFF;
}

u8 TriggerModeLogic(void){  
  return(((Title[7][1].Value>3)&&(Title[7][1].Value<8))||(_T_base>18)||((Sweep>0)&&(_Kind<5)&&(Title[7][1].Value==8)));
}
	
u8 OSBufferLogic(void){
  return((_T_base<17)&&(OSBuffer)&&(!(((Title[0][1].Value==2)&&(_1_source))||((Title[1][1].Value==2)&&(_2_source))))
          &&(_Mode!=X_Y)&&(!ChartLogic())&&(_Mode!=SCAN)&&(Title[TRIGG][SOURCE].Value!=4)
            &&(!UartLogic())&&(!i2cLogic())&&(!SpiLogic()));
}

void ClearMeters(void){
            if(((_Mode==NORH)||(_Mode==NORHLD))&&(__Get(FIFO_START)==0)){
              EnablePaS=1;
              ResetSum();
            }             
}

u16 GetXposRef(u16 XPos){
  if(_Kp1==246){
    return((((102400000+(KpCompensation()/2))/KpCompensation())*XPos)+500)/1000;
  }else if(_Kp1==1024){
    return XPos;
  }else{
    return((((102400000+(KpCompensation()/2))/KpCompensation())*XPos)+5000)/10000;
  }

}

u16 GetXpos(u16 XposRef){
  if(_Kp1==246){
    return(((((KpCompensation()*1000)+512)/1024)*XposRef)+50000)/100000;  //align interpolated time bases with others
  }else if(_Kp1==1024){
    return XposRef;
  }else{
    return(((((KpCompensation()*10000)+512)/1024)*XposRef)+50000)/100000;  
  }
}

void UpdateCursorMeter(void){
        CursorDisplaySelectLogic();
        if(CursorDisplaySelect<4)CalculateTvernier(1);                       //Xpos/Tvernier display
            else if(CursorDisplaySelect<8)CalculateVvernier(1);              //Vvernier display
              else {CalculateVvernier(2);return;}                            //Trig vernier display 
        CalculateTvernier(0);
        CalculateVvernier(0); 
}

void CursorDisplaySelectLogic(void){                                     //called from update
EnableCursorDisplay=1;
CursorDisplayTimer=150;

  if(((DisableCursorDisplay&1)==0)&&((Current==T_VERNIE)||((Current==T_BASE)&&(_Det==2)))){             
    if(Detail[T_VERNIE]==0)CursorDisplaySelect=0;
      else if(Detail[T_VERNIE]>0) CursorDisplaySelect=2;
      if(_Mode==SCAN)CursorDisplaySelect++;
  }else if(((DisableCursorDisplay&2)==0)&&(Current==V_VERNIE)){
    if(_Det==0)CursorDisplaySelect=4;
      else CursorDisplaySelect=6;
    if(_Meas_V_Track)CursorDisplaySelect++;          
  }else if(((DisableCursorDisplay&2)==0)&&(Current==TRIGG)&&(_Det==2)&&(TrgAuto==0)&&(_Mode!=SCAN)){
    CursorDisplaySelect=8;
  }else{
    EnableCursorDisplay=0;
    CursorDisplayTimer=0;
  }
  if(DisableCursorTimer){CursorDisplayTimer=0;DisableCursorTimer--;}

}

void DownConvertRestore(void){
                      DownConvertInitiate=0;
                      _T_base=DCbase;
                      Title[6][0].Value=DCmode;
                      Update_Base();
                      FlagFrameMode=DCframe;                                            
                      FlagMeter=DCmeter;
                      _T1=DCt1;
                      if(FlagMeter)EnableMeter();
                      Title[6][0].Flag |= UPDAT;	
                      Title[6][1].Flag |= UPDAT;	
                      Update_View_Area();
}
/******************************** END OF FILE *********************************/



