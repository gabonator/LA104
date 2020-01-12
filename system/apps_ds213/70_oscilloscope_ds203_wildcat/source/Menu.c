/*
 File Name : Menu.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include <string.h>
#include "Interrupt.h"
#include "Function.h"
#include "Process.h"
#include "Menu.h"
#include "Draw.h"
#include "BIOS.h"
#include "Calibrat.h"
#include "File.h"

u16 Result_FPS;
u8 Cnt_InCharge;
u8 Cnt_Charged;
u8 Cnt_Batt;
u8 FlagInCharge;
u16 NSamples;
u8 MinTresh=1;				//adjust this as the least significant amount to be displayed in V meters
u8 BTwink=0; 
u8  ConfigFileNumber=0;
s16 Ga_Max;
s16 Gb_Max;
s16 Ga_Min;
s16 Gb_Min;
s16 GHa_Max;
s16 GHb_Max;
s16 GHa_Min;
s16 GHb_Min;
s8  ShiftDigits;
u8  AutoFFT;
s8  GenFreqShift=0;
u8  GenAdjustMode=0;
u16 CurrentARR=0;
uc8 TIM2Speed[10]= {1,2,1,2,4,10,10,20,50,100};
u8  SweepMod=1;
u8  SweepStep=1;
u8  SweepIndex=2;
u8  Sweep=0;
s8  PrevShift=0;
u8  Det=0;
u8  GenTrigColorFlag;
u8  EnablePWM=1;
u8  PWAdjustMode=0;
u32 LastFreqReadout=0;
u8  EnableMeterCalc;
u8  T1Start=0;
u8  GenBaudAdjSpeed=0;    //button 5 short press       
u32 GenBaudRate=9600;     //                          left toggle  Adj=0
u8  GenUartMode=3;        //word size/parity          left toggle  Adj=1
u8  GenUartCont=0;        //1=continuous re-transmit  left toggle  Adj=2
u8  GenUartAdj=0;         //adj item  button 6 short press
u16 GenBaudIndex=7500;    //control for baud rate, default= 9600 Baud (72E6/7500)
u8  GenUartStopBits=0;    //0=1 stop bit, 1=2
u8  FastDimAdjust=0;
u8  FastDim=0;
u8  AutoSaveBuf=0;
u8  AutoSaveSelect=0;
u8  HboldAdjust=0;
u8  DETflag=0;
u8  OffsetSelect=0;
u8  ListOverride=0;
char EditChar[8][2]={{32,0},{32,0},{32,0},{32,0},{32,0},{32,0},{32,0},{32,0}};
u8  CharIndex=0;
char  CharValue=0x20;
u8  AdjBeepLevel=0;
u8 MinBypass=0;
u8 AutoSetFlag=0;
u8 AutoSequence=0;
u8 OldCurDefTime=0;
u16 OldPosi=0;
u8 MeterStatus=0;
u8 OldMode;
u8 Neg=0;
u8 DownConvertMode=0;

const char GEN_UART_MODE_STR[6][4]={"7Pe","7Po","8N ","8Pe","8Po","9N "};

u32  TimeScaleCorrection(u32 Tmp, u8 Polarity, u16 Kp);
void CalculateTimeMeters(u32 Tmp, u32 m, u32 n, u16 Kp);
u32  Period(u8 Ch,u32 k);
u32  ClockAdjust(u32 Value);
u8   MeterChannelLogic(u8 i,u8 channel);
void UpdatePWMDuty(u8 service);
u8   ChDetLogic(u8 ch,u8 val);
u8   DetStatusLogic(u8 logic);
void SyncSweep(u8 track);
u8   ListLogic(void);
u16  KpCompensation(void);
void ProcessCursorDisplayStr(char* TmpStr);
void AutoSet(void);

uc16 X100TbaseScale[7]={33333,16667,6667,3333,1667,667,333};

const char T_UNIT[12] ={'n','S', 0 ,'u','S', 0 ,'m','S', 0 ,'S',' ', 0 };

const char D_UNIT[15] ={'u','S', 0 ,'u','S', 0 ,'m','S', 0 ,'S',' ', 0 ,'S',' ', 0 };     //special for delta T, will not work right with "n" as 1st ch  	
const char M_UNIT[15] ={'u','S', 0 ,'u','S', 0 ,'M',' ', 0 ,'M',' ', 0 ,'M',' ', 0 };     

const char DD_UNIT[15] ={'n','S', 0 ,'u','S', 0 ,'m','S', 0 ,' ','S', 0 ,' ','S', 0 };    //for T cursor display and CSV time field  
const char MM_UNIT[15] ={'u','S', 0 ,'u','S', 0 ,'m',' ', 0 ,'m', 0 , 0 ,'m', 0 , 0 };     
const char H_UNIT[15]  ={'u','S', 0 ,'u','S', 0 ,'m', 0 , 0 ,'h', 0 , 0 ,'h', 0 , 0 };       

const char V_UNIT[12]  ={'m','V', 0 ,'m','V', 0 ,'V',' ', 0 ,'V',' ', 0 };                //for delta V display
const char VV_UNIT[12] ={'m','V', 0 ,'m','V', 0 ,'V', 0 , 0 ,'V', 0 , 0 };                //for V cursor and trigger level displays
 
      char N_UNIT[12]   ={ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };           	
const char S_UNIT[12]   ={'/','S','e','c', 0 ,'/','S','e','c', 0 , 0 , 0 };
const char P_UNIT[12]   ={'%',' ', 0 ,'%',' ', 0 ,'%',' ', 0 , 0 , 0 , 0 };
const char F_UNIT[12]   ={'H','z', 0 ,'K','@', 0 ,'M','@', 0 ,'G','@', 0 };         	  //Used at high sweep rates so display works right
const char F_UNITSUB[12]={'m','@', 0 ,'H','z', 0 ,'K','@', 0 ,'M','@', 0 };               //added to keep original FREQ display working right at low sweep rates 

const char STATESTR[3][10] = {"!RUN!", "HOLD", "HOLD"};                  	          // Running state str
//uc16 S_Inv[3]       = {(SCRN<<8)+TEXT2, (SCRN<<8)+X_POSI, (SCRN<<8)+NOTE1};             // Running state Color
  uc16 S_Inv[3]       = {(SCRN<<8)+TRACK4, (SCRN<<8)+X_POSI, (SCRN<<8)+NOTE1};            // Running state Color

const char BATT_STR[5][10] = {"~``'", "~``}", "~`;}", "~;;}", "{;;}"};                    // Battery Status Str
/*
uc16 B_COLOR[5]     = {(NOTE1<<8)+SCRN, (SIDE <<8)+SCRN,
                       (TEXT2<<8)+SCRN, (TEXT2<<8)+SCRN,
                       (TEXT2<<8)+SCRN}; */                                               // Battery Status Color
uc16 B_COLOR[5]     = {(NOTE1<<8)+SCRN, (SIDE <<8)+SCRN,
                       (TRACK4<<8)+SCRN, (TRACK4<<8)+SCRN,
                       (TRACK4<<8)+SCRN};                                                 // Battery Status Color


 char CH_A_STR[5][10]  = {" !OFF! ", "!CH(A)!", "!CH A !", "x10(A)", "x10 A "};
 char CH_B_STR[5][10]  = {" !OFF! ", "!CH(B)!", "!CH B !", "x10(B)", "x10 B "};

const char XY_CH_A_STR[5][10]  = {" !OFF! ", "CH!A=!X", "CH!A=!X", "X10A=X", "X10A=X"};
const char XY_CH_B_STR[5][10]  = {" !OFF! ", "CH!B=!Y", "CH!B=!Y", "X10B=Y", "X10B=Y"};

const char CH_C_STR[2][10]  = {" !OFF! ", "!CH(C)!"};
const char CH_D_STR[16][10] = {	" !OFF! ", "!CH(D)!", "!(A+B)!", "!(A-B)!",  
							"!(C&D)!", "!(C|D)!", "!REC_A!",  "!REC_B!",
							"!REC_C!","!REC_D!"," !MAP! "," SPEC ","hFFT_B","sFFT_B","hFFT_A","sFFT_A"};                      // Track source Str

const char NO_RANGE[1]      = ""; //" -- ";
const char NO_DEF[1]        = ""; //"--";                       

const char  YCOUPLE[7][10]  = {"DC", "AC", "DT","TL","RS","i2","SP"};     // Track Couple Str                    
const char  YCOUPLE_B[5][10]  = {"DC", "AC", "DT","i2","SP"};             // Track Couple Str                    
char  Vertical[15][10];                                                   // Track Range Str
const char  YPOSISTR[5]    = {"YPOS"};                                    // Track Position Str 
uc16 Y_INV[5]       = {(SCRN<<8)+TR_1, (SCRN<<8)+TR_2,

                       (SCRN<<8)+TR_3, (SCRN<<8)+TR_4,
                       (SCRN<<8)+VERNIE};                          // Track Color 1
uc16 Y_COLOR[5]     = {(TR_1<<8)+SCRN, (TR_2<<8)+SCRN,
                       (TR_3<<8)+SCRN, (TR_4<<8)+SCRN,
                       (VERNIE<<8)+SCRN};                          // Track Color 2

const char MODESTR[7][10]  = {"TrOFF","!AUTO!", "NORML","NorCL","PERST","SINGL"," X_Y "};        // Sync Mode Str

char BaseStr[30][10];                                            // Time Base b Str
const char  XPOSISTR[5]    = {"XPOS"};
uc16 XCOLOR[2]      = {(SCRN<<8)+X_POSI, (X_POSI<<8)+SCRN};        // Time Base Color

const char FO_TYPE[9][10]  = {"!SINUS!",  "TRIANG",  "! SAW !",  "SQUARE", "! PWM !","!NOISE!","! OFF !","!ARBTR!"," UART "}; // Output Kind Str  "Kd=  %"

const char FO_STR[23][10]  = {"! 1Hz !",  "! 2Hz !",  "! 5Hz !",
                        " 10Hz ",  " 20Hz ",  " 50Hz ",   "!100Hz!",
                       "!200Hz!", "!500Hz!", " 1KHz ",   " 2KHz ",
                       " 5KHz ",  "!10KHz!", "!20KHz!",  "!50KHz!",
                       "100KHz",  "200KHz",  "500KHz",   " 1MHz ",
                       " 2MHz ",  " 4MHz ",  " 6MHz ",   " 8MHz "}; // Output Frequency Str

const char SubStr[13][10]= {"100mS","200mS","500mS","! 1S !","! 2S !","! 5S !"," 10S "," 15S "," 30S ","! 1M !","! 2M !","! 5M !"," 10M "};

//uc16 O_COLOR[2]     = {(SCRN<<8)+TEXT3, (TEXT3<<8)+SCRN};         // Output Frequency Color BLUE
   u16 O_COLOR[2]     = {(SCRN<<8)+NOTE1, (NOTE1<<8)+SCRN};         // Output Frequency Color  RED

const char  TRIGSTR[5][10] = {"TR A ", "TR B ", "TR C ", "TR D ", " A&B "};      // Trigger source Str

const char  TR_TYPE[9][10] = {":!\\!", ":!^!", "<Vt", ">Vt",     // Trigger Type Str
                       "L<[",   "L>[",  "H<[", "H>[", "GEN" };    // Seems clearer to state, for example: "low pulse less in width
                                                                 // than delta T1-T2" as "L<Delta" rather than "<TL"
const char  VT_STR[4]  =  "LEV";                                 
const char  DELAY_STR[2] = "T";				   // Trigg holdoff indicator	

const char  FFT_GAIN_STR[10][6]={ {" LOG "},{" AUTO"},{" 0db "},{" +6db"},{"+12db"},
                                      {"+18db"},{"+24db"},{"+30db"},{"+36db"},{"+42db"} };

uc8 TrunkFactor[8]={8,6,5,5,6,5,5,5};

const char  VERNIE1[5]     = {"!V1!"};          // V1 Vernie Str
const char  VERNIE2[5]     = {"!V2!"};          // V2 Vernie Str
const char  VERNIE3[5]     = {"!T1!"};          // T1 Vernie Str
const char  VERNIE4[5]     = {"!T2!"};          // T2 Vernie Str
uc16 V_INV[1]       = {(SCRN<<8)+VERNIE};
uc16 V_COLOR[1]     = {(VERNIE<<8)+SCRN};                         // Y Vernie Color
uc16 T_INV[1]       = {(SCRN<<8)+VERNIE};
uc16 T_COLOR[1]        = {(VERNIE<<8)+SCRN};                         // X Vernie Color
const char  F_FUNC[2][11]  = {"Save  File", "Load  File"};                 // File Function Str

const char  F_EXT[8][10]   = {".BMP ", ".DAT ", ".BUF ",".CSV ", ".CFG ", ".ARB ", ".UAR ", " ROM "};

uc16 F_INV[1]       = {(SCRN<<8)+TEXT1};                          //  File Color
const char  DELTA_V[2][10] = {"[V:", "[V:"};
const char  DELTA_T[2][4]  = {"[T:","Frq"};
const char  METER[12][5]   = {"Vbt", "FPS", "Vdc", "RMS", "Frq",  "Max", 
                       "Min", "Vpp", "Per", "Dut", "!TH!", "!TL!"};

u8   Detail[14];
char NumStr[12];
char VNumStr[12];
char TNumStr[12];

char CursorDisplayStr[12];
char CursorDisplayStrM[12];
char CursorDisplayStrH[12];

u8   Current = 0, Update = 1;
const char BL_Str[5]="B.L", Vol_Str[5]="Vol";

char  x10[8][6]   = {"0.5V", " 1V ", " 2V ", " 5V ", "!10V!", "!20V!", "!50V!", "100V"};

menu Title[13][4]=   
{
  //     Item_String,   m_Color,   Limit, Mark, xPos, yPos, Value, Flag 
  {//============================ Title Track1 Group ===========================  <0>
    {(char*)CH_A_STR,(u16*)Y_INV,     5-1, CIRC,   35,  228,     1, UPDAT}, //  Track source   2
    {(char*)YCOUPLE, (u16*)Y_COLOR,   7-1, CIRC,   35,  216,     0, UPDAT}, //  Track Couple        
    {(char*)Vertical,(u16*)Y_COLOR,   9-1,    0,   51,  216,     5, UPDAT}, //  Track Range 
    {(char*)YPOSISTR,(u16*)Y_INV,     200,  FIX,    0,    0,   150, UPDAT}, //  Adj. Track Position 160
  },
  {//============================ Title Track2 Group ===========================  <1>
    {(char*)CH_B_STR,(u16*)Y_INV+1,   5-1, CIRC,   86,  228,     1, UPDAT}, //  Track source   2
    {(char*)YCOUPLE_B,(u16*)Y_COLOR+1,5-1, CIRC,   86,  216,     1, UPDAT}, //  Track Couple        
    {(char*)Vertical,(u16*)Y_COLOR+1, 9-1,    0,  102,  216,     1, UPDAT}, //  Track Range
    {(char*)YPOSISTR,(u16*)Y_INV+1,   200,  FIX,    0,    0,   100, UPDAT}, //  Adj. Track Position
  },
  {//============================ Title Track3 Group ===========================  <2>
    {(char*)CH_C_STR,(u16*)Y_INV+2,   2-1, CIRC,  137,  228,     1, UPDAT}, //  Track source
    {(char*)NO_DEF , (u16*)Y_COLOR+2, 1-1, CIRC,  137,  216,     0, HID}, //  Track Couple        
    {(char*)NO_RANGE,(u16*)Y_COLOR+2, 1-1,    0,  153,  216,     0, HID}, //  Track Range
    {(char*)YPOSISTR,(u16*)Y_INV+2, 200-1,  FIX,    0,    0,    60, UPDAT}, //  Adj. Track Position
  },
  {//============================ Title Track4 Group ===========================  <3>
    {(char*)CH_D_STR,(u16*)Y_INV+3,  16-1, CIRC,  188,  228,     1, UPDAT}, //  Track source
    {(char*)NO_DEF , (u16*)Y_COLOR+4, 1-1,  FIX,  233,    0,     0, UPDAT}, //  Used to store SpecMode and display FFT MAG        
    {(char*)NO_RANGE,(u16*)T_INV,    10-1,  FIX,  217,    0,     0, UPDAT}, //  FFT Gain Adjust     
    {(char*)YPOSISTR,(u16*)Y_INV+3, 200-1,  FIX,    0,    0,    20, UPDAT}, //  Adj. Track Position
  },
  {//================== Running State & Battery Status Group ===================  <4>
    {(char*)STATESTR,(u16*)S_Inv,     3-1,    0,    0,  228,     0, UPDAT}, //  Running state
    {(char*)BATT_STR,(u16*)B_COLOR,   5-1,    0,    0,  216,     0, UPDAT}, //  Battery status
    {(char*)BATT_STR,(u16*)B_COLOR,   5-1,  NOT,    0,    0,     0,   HID}, 
    {(char*)BATT_STR,(u16*)B_COLOR,   5-1,  NOT,    0,    0,     0,   HID},
  },
  {//======================= Title Output Signal Group =========================  <5>
    {(char*)FO_TYPE, (u16*)O_COLOR,   9-1, CIRC,  239,  228,     3, UPDAT}, //  Output Wave Kind    282 228       
    {(char*)FO_STR,  (u16*)O_COLOR+1,23-1,    0,  239,  216,    13, UPDAT}, //  Output Frequency    282 216      
    {(char*)NumStr,  (u16*)O_COLOR, 30000, NUM3,  212,  216, 15000, UPDAT}, //  Duty value      100
    {(char*)NumStr,  (u16*)O_COLOR,   100, NUM3,  196,  216,    50, UPDAT}, //  Attenuazione    251,202         
  },
  {//========================= Title Time Base Group ===========================  <6>
    {(char*)MODESTR, (u16*)XCOLOR,    7-1, CIRC,  290,  228,     0, UPDAT}, //  Sync Mode    5 numero modi  239  228
    {(char*)BaseStr, (u16*)XCOLOR+1, 22-1,    0,  290,  216,    17, UPDAT}, //  Time Base Range             239 216	WAS 27
    {(char*)XPOSISTR,(u16*)XCOLOR,   3703,  FIX,  366,    0,     0, UPDAT}, //  Adj. X position  3795
    {(char*)NO_RANGE,(u16*)T_INV,     253,  FIX,  159,    0,    32, UPDAT}, //  Adj. buffer size for X_Y mode      View window rule  3795
  },           
  {//=====================;===== Title Trigger Group ====;======================  <7>
    {(char*)TRIGSTR, (u16*)Y_INV,     5-1, CIRC,  333,  228,     1, UPDAT}, //  Trigger source         
    {(char*)TR_TYPE, (u16*)Y_INV,     9-1, CIRC,  373,  228,     0, UPDAT}, //  Trigger Kine           was 8-1      
    {(char*)VT_STR,  (u16*)Y_COLOR,   200,  FIX,  333,  216,     0, UPDAT}, //  Adj. Trigger threshold     
    {(char*)DELAY_STR,(u16*)Y_COLOR, 3400,  FIX,  92,    0,      0, UPDAT}, //  Trigger holdoff value     
  },
  {//============================ Title Y Vernie Group =========================  <8>
    {(char*)VERNIE1, (u16*)V_INV,   201-1,  FIX,   35,    0,   180, UPDAT}, //  V1 Vernie         
    {(char*)VERNIE2, (u16*)V_INV,   201-1,  FIX,   62,    0,    40, UPDAT}, //  V2 Vernie     
    {(char*)DELTA_V, (u16*)Y_INV,     2-1, CIRC,  314,  197,     0, UPDAT}, //  Measure Track    
    {(char*)NumStr,  (u16*)Y_COLOR,   240, NUM3,  342,  197,    70, UPDAT}, //  Delta V value    
  },
  {//============================= Title File Group ============================  <9>
    {(char*)F_FUNC,  (u16*)F_INV,     2-1, CIRC,  126,    0,     0,     0}, //  File function    
    {(char*)NumStr,  (u16*)F_INV,   256-1, NUM3,  206,    0,     0,     1}, //  File number     
    {(char*)F_EXT,   (u16*)F_INV,     8-1, CIRC,  230,    0,     0,     0}, //  Ext. name     
    {(char*)NO_RANGE,(u16*)F_INV,    15-1,  FIX,  166,    0,     0,   HID}, //  Dir readout position  
  },
  {//============================ Title X Vernie Group =========================  <10>
    {(char*)VERNIE3, (u16*)T_INV,     395,  FIX,  312,    0,    80, UPDAT}, //  T1 Vernie     300     
    {(char*)VERNIE4, (u16*)T_INV,     395,  FIX,  339,    0,   280, UPDAT}, //  T2 Vernie     300   
    {(char*)DELTA_T, (u16*)T_INV,     2-1, CIRC,  314,  182,     0, UPDAT}, //  Delta T Str     
    {(char*)NumStr,  (u16*)T_COLOR,   400, NUM3,  342,  182,    80, UPDAT}, //  Delta T value 300   
  },
  {//=========================== Title BackLight Group =========================  <11>
    {(char*)BL_Str,  (u16*)V_INV,     1-1,  FIX,  314,  167,     0,   HID}, //  BackLight    
    {(char*)NumStr,  (u16*)V_COLOR,  10-1, NUM2,  342,  167,     5,   HID}, //  Class     
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},     
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},  
  },
  {//============================ Title Volume Group ===========================  <12>
    {(char*)Vol_Str, (u16*)V_INV,     1-1,  FIX,  314,  152,     0,   HID}, //  Volume        
    {(char*)NumStr,  (u16*)V_COLOR,  10-1, NUM2,  342,  152,     5,   HID}, //  Class     was 11-1, causing display error
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},    
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},    
  },
};

      //.str
meter Meter[9]  = //.track   .item                         .flag
{ {(char*)METER,      4,    VBT,     314,    342,   137,  UPDAT}, //  Meter #0
  {(char*)METER,      4,    FPS,     314,    342,   122,  UPDAT}, //  Meter #1
  {(char*)METER, TRACK4,    FRQ,     314,    342,   107,  UPDAT}, //  Meter #2
  {(char*)METER, TRACK3,    CIR,     314,    342,    92,  UPDAT}, //  Meter #3
  {(char*)METER, TRACK3,    DUT,     314,    342,    77,  UPDAT}, //  Meter #4
  {(char*)METER, TRACK2,    TH,      314,    342,    62,  UPDAT}, //  Meter #5
  {(char*)METER, TRACK2,    TL,      314,    342,    47,  UPDAT}, //  Meter #6
  {(char*)METER, TRACK1,    VPP,     314,    342,    32,  UPDAT}, //  Meter #7
  {(char*)METER, TRACK1,    VDC,     314,    342,    17,  UPDAT}, //  Meter #8
};

void Display_Meter(void)                  // refresh measurements display
{
  u16 color;
  u8  i;
  u8  MeterLimit=9;

  if(FlagMeter==2){
    MeterLimit=1;
    if(UpdateMeter!=4)MeterLimit=0;
  }

  for(i=0; i<MeterLimit;i++){        
    if(Meter[i].Flag & UPDAT){            //-----the name of the measurements show the need to refresh

      Meter[i].Flag &= (!UPDAT & !BLINK);       // Clr Update & Blink flag

      if(((Options&4)==4)&&((Meter[i].Item==MIN)||(Meter[i].Item==MAX)))
        color=Y_INV[4];else color=Y_INV[Meter[i].Track];

        Print_Str
          (Meter[i].XPOS1, Meter[i].YPOS,        // coordinates to be displayed
           color,                // need to display color (belongs channel)
           PRN,                                  // print (no flash)
           Meter[i].Str +(Meter[i].Item * 5));   // need to display the project name

    } 
  }
  if((Current >= METER_0)&&(Current <(MeterLimit+METER_0))){  
    if(Blink){                            //----- the name of the display required flashing measurements
      i = Current - METER_0;
      Blink = 0;                               // Clr Blink Ctrl flag

      if(((Options&4)==4)&&((Meter[i].Item==MIN)||(Meter[i].Item==MAX)))
        color=Y_INV[4];else color=Y_INV[Meter[i].Track];

       Print_Str(
          Meter[i].XPOS1, Meter[i].YPOS,         // flashing coordinates
          color,                 // flashing colors (belongs channel)
          Twink,                                 // flashing
          Meter[i].Str +(Meter[i].Item *5));     // flashing the project name

    }
  }
}


void Display_Value(u8 i)
{
  s32 Tmp = 0;
  s32 Tmp2= 0;
  u32 UTmp=0;
  u16 Kp;
  u32 k, n, m;
  u8  j;

  Kp=KpCompensation();
  
  k = _T_Range; m = 1;  n = 1;
  if(k < 9)  m = Power(10, (11-k)/3); 
  else       n = Power(10, (k- 9)/3); 
  k = X_Attr[(k%3)+9].SCALE;
  ShiftDigits=0;

if(DETflag)goto CalculateDETvalues;
if(FFTflag)goto CalculateFFTvalues;
if((DownConvertInitiate)||(DownConvertMode))goto CalculateDownConvertValues;

  switch (Meter[i].Item){  
  case VBT://--------------- calculation and display of battery voltage ---------------
    Int2Str(NumStr, __Get(V_BATTERY)*1000, V_UNIT, 3, SIGN,0);
    break;
  case FPS://--------------- calculates and displays the frame count ---------------
    Int2Str(NumStr, Result_FPS, S_UNIT, 3, STD,0);
    for(j=0;j<11;j++)NumStr[j]=NumStr[j+1]; 
    break;  
  case VPP:
    if(Meter[i].Track == TRACK1){
		if(MeterChannelLogic(i,TRACK1)){
			Tmp=Ga_Max+Ka1[_A_Range]-_1_posi;				
	                Tmp=Ka2[_A_Range]*Tmp;
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	// Flip bias down when calculating negative values	
			Tmp2=Ga_Min+Ka1[_A_Range]-_1_posi;	// otherwise neg values end up 1 LSB short after adding +1/2 LSB (+512/1024)
		        Tmp2=Ka2[_A_Range]*Tmp2; 
                        if (Tmp2>0)Tmp2+=512; else Tmp2-=512;	 	        
			Tmp=(Tmp/1024)-(Tmp2/1024);
			if(Tmp <= MinTresh) Tmp = 0;						
			Tmp *= Y_Attr[_A_Range].SCALE;
			if(_1_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
		if(MeterChannelLogic(i,TRACK2)){
			Tmp=Gb_Max+Kb1[_B_Range]-_2_posi;
	                Tmp=Kb2[_B_Range]*Tmp; 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	    	
			Tmp2=Gb_Min+Kb1[_B_Range]-_2_posi;
	                Tmp2=Kb2[_B_Range]*Tmp2; 
                        if (Tmp2>0)Tmp2+=512; else Tmp2-=512;	 	        
			Tmp=(Tmp/1024)-(Tmp2/1024);
			if(Tmp <= MinTresh) Tmp = 0;						
			Tmp *= Y_Attr[_B_Range].SCALE;
			if(_2_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN,0);
    break;
  case VDC:
    if(Meter[i].Track == TRACK1){
		if(MeterChannelLogic(i,TRACK1)){
			Tmp=((VxAvg[0]/NSamples)+Ka1[_A_Range]-_1_posi);
	                Tmp=Ka2[_A_Range]*Tmp;   // Add signal level correction factor based on signal zero level rather than bottom of screen, prevents offsets from corrupting value 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	        	
                        Tmp/=1024;
			if((Tmp >= -MinTresh)&&(Tmp <= MinTresh)) Tmp = 0;
			Tmp *= Y_Attr[_A_Range].SCALE;
			if(_1_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
		if(MeterChannelLogic(i,TRACK2)){
			Tmp=((VxAvg[1]/NSamples)+Kb1[_B_Range]-_2_posi);
	                Tmp=Kb2[_B_Range]*Tmp; 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	        	
                        Tmp/=1024;
			if((Tmp >= -MinTresh)&&(Tmp <= MinTresh)) Tmp = 0;
			Tmp *= Y_Attr[_B_Range].SCALE;
			if(_2_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN,0);
    break;
  case RMS:
    if(Meter[i].Track == TRACK1){
		if(MeterChannelLogic(i,TRACK1)){
			Tmp = (Ka2[_A_Range]*Int_sqrt(VxSsq[0]/NSamples)+ 512)/1024;     // adding Ka1 here causes error in calc, already factored in in Process...
			if(Tmp <= MinTresh) Tmp = 0;										
			Tmp *= Y_Attr[_A_Range].SCALE;
			if(_1_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
		if(MeterChannelLogic(i,TRACK2)){
			Tmp = (Kb2[_B_Range]*Int_sqrt(VxSsq[1]/NSamples)+ 512)/1024; 			
			if(Tmp <= MinTresh) Tmp = 0;										
			Tmp *= Y_Attr[_B_Range].SCALE;
			if(_2_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN,0); //unsign
    break;
 case MAX:
    if(Meter[i].Track == TRACK1){
		if(MeterChannelLogic(i,TRACK1)){
			Tmp=GHa_Max+Ka1[_A_Range]-_1_posi;
	                Tmp=Ka2[_A_Range]*Tmp; 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	        	
                        Tmp/=1024;
			if((Tmp >= -MinTresh)&&(Tmp <= MinTresh)) Tmp = 0;
			Tmp *= Y_Attr[_A_Range].SCALE;
			if(_1_source>2)Tmp=Tmp*10;
		}else Tmp=0;
	}
    if(Meter[i].Track == TRACK2){
		if(MeterChannelLogic(i,TRACK2)){
			Tmp=GHb_Max+Kb1[_B_Range]-_2_posi;
         	        Tmp=Kb2[_B_Range]*Tmp; 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	        	
                        Tmp/=1024;
			if((Tmp >= -MinTresh)&&(Tmp <= MinTresh)) Tmp = 0;
			Tmp *= Y_Attr[_B_Range].SCALE;
			if(_2_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
	
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN,0);
    break;
  case MIN:
    if(Meter[i].Track == TRACK1){
		if(MeterChannelLogic(i,TRACK1)){
			Tmp=GHa_Min+Ka1[_A_Range]-_1_posi;
	            	Tmp=Ka2[_A_Range]*Tmp; 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	        	
                        Tmp/=1024;
			if((Tmp >= -MinTresh)&&(Tmp <= MinTresh)) Tmp = 0;
			Tmp *= Y_Attr[_A_Range].SCALE;
			if(_1_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
		if(MeterChannelLogic(i,TRACK2)){
			Tmp=GHb_Min+Kb1[_B_Range]-_2_posi;
	            	Tmp=Kb2[_B_Range]*Tmp; 
                        if (Tmp>0)Tmp+=512; else Tmp-=512;	 	        	
                        Tmp/=1024;
			if((Tmp >= -MinTresh)&&(Tmp <= MinTresh)) Tmp = 0;
			Tmp *= Y_Attr[_B_Range].SCALE;
			if(_2_source>2)Tmp=Tmp*10;
		}else Tmp=0;
    }

    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN,0);
    break;

  case FRQ:
    for(j=0;j<4;j++)if(MeterChannelLogic(i,j))UTmp=Sum[j];

CalculateDownConvertValues:
    if(DownConvertMode){
      if(_4_source>13)UTmp=Sum[0];
      else if((_4_source==12)||(_4_source==13))UTmp=Sum[1];
    } 

CalculateDETvalues:
    if(DETflag){
      if(Title[TRIGG][SOURCE].Value==0)UTmp=Sum[0];
        else if(Title[TRIGG][SOURCE].Value==1)UTmp=Sum[1];
          else {LastFreqReadout=0;return;}
    }
    UTmp=UpScale(UTmp,1);

CalculateFFTvalues:
    if(FFTflag)UTmp=500000000;
    UTmp=ClockAdjust(UTmp);

    if((DownConvertMode)&&(FFTflag==0)&&(DownConvertInitiate==0)){
      LfreqPeriod=UTmp; 
      FFTdownConvert(3);
      return;
    }

    if(n < 10){							

      UTmp=(1000*((UTmp+(k/2))/k))/m;

      if((DownConvertInitiate==1)&&(FFTflag==0)){
        FreqIN=UTmp/1000;
        FreqScaleShift=ShiftDigits;
        return;
      }

      if(DETflag){
        UTmp=(UTmp/100)*99;                                                      //for detector mode, -1% compensation for shifting freq in sweep mode
        LastFreqReadout=UTmp/1000;
      }

      if(FFTflag){
        if(ChartLogic()){
          UTmp=(((((72000000/(TbaseSub[SubIndex-1].Arr+1))*10000) /(TbaseSub[SubIndex-1].S_OS_Size))*100)
            /(TbaseSub[SubIndex-1].Psc+1))/2;    
          ShiftDigits=3;  
        }
        NFreq=UTmp;for(j=0;j<12;j++)N_UNIT[j]=F_UNITSUB[j];
      } else {
        if(ChartLogic()){UTmp=0;ShiftDigits=0;}                                    //don't calculate time meters when using chart
        Int2Str(NumStr,UTmp, F_UNITSUB , 4, UNSIGN, ShiftDigits);
      }
    }
    else{       
      if(FFTflag){NFreq=UTmp;for(j=0;j<12;j++)N_UNIT[j]=F_UNIT[j];}
      if(Kp==1024){
        UTmp=n*((UTmp+(k/2))/k);

        if(DownConvertInitiate==1){
          FreqIN=UTmp;
          FreqScaleShift=ShiftDigits;
          return;
        }

        if(DETflag){
          UTmp=(UTmp/100)*99;                                              //for detector mode, -1% compensation for shifting freq in sweep mode
            LastFreqReadout=UTmp;
        }

        if(FFTflag)NFreq=UTmp;else Int2Str(NumStr,UTmp, F_UNIT, 4, UNSIGN, ShiftDigits);
      } 
      else{                          //for interpolated ranges, adjust to prevent truncation errors in scaling to be further multiplied
        UTmp=TimeScaleCorrection(UTmp,0,Kp);
        if(Kp==24576){
            UTmp=((Kp*((((n/10)*((UTmp+(k/2))/k))+512)/1024))+5)/10;  	  //Kp here X100 * n/10=  /10
        }else{
            UTmp=((Kp*(((n*((UTmp+(k/2))/k))+512)/1024))+5)/10;	 	  //Kp X10 so need to /10
        }

        if(DownConvertInitiate==1){
          FreqIN=UTmp;
          FreqScaleShift=ShiftDigits;
          return;
        }

        if(UTmp>0x7FFFFFFF){						  //scale down if beyond s32 range
          Tmp=(UTmp/10);
          ShiftDigits--;
        }else Tmp=UTmp;

        if(FFTflag)NFreq=Tmp;else Int2Str(NumStr,Tmp, F_UNIT, 4, UNSIGN, ShiftDigits);
      }
    } 

    if(DETflag){
      j=ShiftDigits;
      while(j>0){LastFreqReadout/=10;j--;}                                  //cancel up-scaling for detector mode frequency readouts
    }

if(DETflag)return;
if(FFTflag)return;

    break;  
  case CIR:
    for(j=0;j<4;j++)if(MeterChannelLogic(i,j))UTmp=Period(j,k);
    CalculateTimeMeters(UTmp,m,n,Kp);
    break;
  case DUT:
if(EnablePaS){							//only update these once per second
    for(j=0;j<4;j++)if(MeterChannelLogic(i,j))Tmp=((10000*PxS[j])/TxS[j])*10;
    if(ChartLogic()){Tmp=0;ShiftDigits=0;}
    Int2Str(NumStr, Tmp, P_UNIT, 4, UNSIGN,0);
}
    break;
  case TL:
if(EnablePaS){
    for(j=0;j<4;j++)if(MeterChannelLogic(i,j))UTmp=(UpScale(k*(TxS[j]-PxS[j]),1)+(TxN[j]/2))/TxN[j];
    CalculateTimeMeters(UTmp,m,n,Kp);
}
    break;
  case TH:
if(EnablePaS){
    for(j=0;j<4;j++)if(MeterChannelLogic(i,j))UTmp=(UpScale(k*PxS[j],1)+(TxN[j]/2))/TxN[j];
    CalculateTimeMeters(UTmp,m,n,Kp);
}
    break;

  }  //end switch

  if (FlagMeter==1){
    Print_Str(
    Meter[i].XPOS2, Meter[i].YPOS,
    Y_COLOR[Meter[i].Track], 
    PRN,
    NumStr);                              // display the measured values
  }else if (FlagMeter==2){		  // display measured values and meter title in big meters
    DisplayBigMeter(i);
  }  
  
}

void Display_Title(void)			
{
  u16  i, j, TmpColor;//,BaseRatio;
  u8 l;
  u8 z;
  char *p;
  char Zero[2];
  Zero[1]=0;
//  u8 ShiftLeft=0;
  CH_A_STR[2][5]=129; CH_A_STR[4][5]=129;
  CH_B_STR[2][5]=129; CH_B_STR[4][5]=129;

if(((Sweep)||(_Kind==5))&&(_Kind<6)){
  O_COLOR[0]=(SCRN<<8)+VERNIE;  
  O_COLOR[1]=(VERNIE<<8)+SCRN;
}else{
  O_COLOR[0]=(SCRN<<8)+NOTE1;  
  O_COLOR[1]=(NOTE1<<8)+SCRN;
}

      //if(TimedDeltaView>0)ShiftLeft=2; else ShiftLeft=0;	//move it away from right edge to clear border 

      if (_Mode==X_Y){					//replace menu title for chA and chB to show X_Y mode
         Title[0][0].Str=(char*)XY_CH_A_STR;
         Title[1][0].Str=(char*)XY_CH_B_STR;
      }else{						//show regular title
         Title[0][0].Str=(char*)CH_A_STR;
         Title[1][0].Str=(char*)CH_B_STR;
      }

      if ((Current == OUTPUT)&&((_Det==DUTYPWM) || (_Det==OUTATT))){ z=Twink; }else { z=INV;}

      if  (Title[OUTPUT][KIND].Value == PWM)                                 
	{
        u8ToDec3(Title[OUTPUT][DUTYPWM].Str,(Title[OUTPUT][DUTYPWM].Value+150)/300,0);
        Print_Str(172,  216,(Title[OUTPUT][DUTYPWM].Color[0]), z, "Dut% ");            //225,202[Title[OUTPUT][PWM].Value]
        Print_Str(212, 216,O_COLOR[0],z,Title[OUTPUT][DUTYPWM].Str);
      }  
      else
      {
        Print_Str(172,  216, (Title[OUTPUT][DUTYPWM].Color[0]), z, "Vo ");//230,202
        Int2Str(NumStr, (Title[OUTPUT][OUTATT].Value*28000), V_UNIT, 2, UNSIGN,0);     //26000 
        Print_Str(196,216,O_COLOR[0],z,NumStr);
      }

        if (CalFlag==0){
          Print_Str(137,216,0x0405,PRN,"U");
        }else{
          Print_Str(137,216,0x0405,PRN,"C");
        }
        
        if (CurDefTime==0){
          Print_Str(153,216,0x0405,PRN," ");
        }else{
          Print_Str(153,216,0x0405,PRN,"X");
        }

        if(Options&4){
          Print_Str(145,216,0x0405,PRN,"H");
        }else{
          Print_Str(145,216,0x0405,PRN," ");
        }

        p=(char*)Zero;
        if(ListOverride){ 
          *p=0x23;                                       //box, denotes list being used with notification at top left instead
        }else{ 
          *p='0'+ConfigFileNumber;  
        }

        Print_Str(161,216,0x0405,PRN,Zero);

  if(NotificationTimer==0){                            
    FFTlevel(0);
    DisplaySamples();
  }

  for(i = TRACK1; i <= VOLUME; ++i){

    for(j = 0; j < 4; ++j){


      if(Title[i][j].Flag & UPDAT){   // need to refresh the Item



        Title[i][j].Flag &= ~UPDAT;   // Clr Update flag 


        if((i == BATTERY)||(i == TRIGG)){


          if(Title[i][j].MARK & FIX){                  // ---- Under fix mode

            if (!((i==TRIGG)&&(j==3)&&((NotificationTimer>0)||(Current==FILE)))){	// "T" delay 
              Print_Str( 								//notifications are displayed
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[Title[i][0].Value],    // Color variable 
                PRN, 
                Title[i][j].Str                          // String fixed
              );                    
            }

          }else if((i == TRIGG)&&(j==1)){               //trig kind, to enable gen kind color shift

              if (GenTrigColorFlag)TmpColor=0x0509;
                else TmpColor=Title[i][j].Color[Title[i][0].Value];

	      Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                TmpColor,    // Color variable 
                PRN, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 



          }else if(!(Title[i][j].MARK & NOT)){        // ---- Under other mode
	      Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[Title[i][0].Value],    // Color variable 
                PRN, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 
          }

        } else {                                       //not battery or trigg

          if(Title[i][j].MARK & FIX){                  // ---- Under fix mode 


          if (!((i==9)&&(j==3))){ //enabling shows filename DIR LIST OR EDIT BOX, SHOW REGULAR MENU HERE (default files) WHEN NOT SELECTED
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                    // Color fixed 
                PRN, 
                Title[i][j].Str                          // String fixed
		    	    // battery and volume meter titles - not flashing
               );
          }



          } else if(Title[i][j].MARK & NUM3){          // ---- Under NUM3 mode

            if(i == V_VERNIE){
              CalculateVvernier(0);
              Print_Str(
                (Title[i][j].XPOS), Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],      // Color fixed  
                PRN, 
                VNumStr                                // String for numerical
              );
            }

            if(i == T_VERNIE){				
              CalculateTvernier(0);                                             //delta T service
              Print_Str(
                (Title[i][j].XPOS), Title[i][j].YPOS,                 //shift left when calling with no meters to clear right edge
                Title[i][j].Color[0],                                           // Color fixed  
                PRN, 
                TNumStr                                                          // String for numerical
              );
            }

            if(i==FILE){                                                //WITHIN MARK & NUM3 
               u8ToDec3(Title[FILE][1].Str, Title[FILE][1].Value,0);
               Print_Str(
                Title[FILE][1].XPOS, Title[FILE][1].YPOS,
                Title[FILE][1].Color[0],                  // Color fixed  
                PRN, 
                Title[FILE][1].Str                        // String for numerical
              );
            }

          } else if(Title[i][j].MARK == NUM2){ 
                NumStr[0]=' ';
            if(i == BK_LIGHT){                        // backlight percentage
              if(Title[i][j].Value == 9){
                Int2Str(NumStr, 100, P_UNIT, 3, STD,0);
                } else Int2Str(NumStr, 10*(Title[i][j].Value+1), P_UNIT, 2, STD,0);
            } else {                                  // volume percentage
              if(Title[i][j].Value == 9){							//was 10, changed so volume ind works properly
                Int2Str(NumStr, 100, P_UNIT, 3, STD,0);
              } else Int2Str(NumStr, 10*(Title[i][j].Value+1), P_UNIT, 2, STD,0);
            }
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                  // Color fixed  
              PRN, 
              NumStr                        // String for numerical
			  //Battery and volume values
            );
          } else if(Title[i][j].MARK != NOT){                                               //LEAVES CIRC
          
            if(i == V_VERNIE){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],         // Color variable 
                PRN, 
                Title[i][j].Str +(Title[i][j].Value * 10) // String variable
				//Delta V meter title
              ); 
            } else if(i == T_VERNIE){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                    // Color fixed 
                PRN, 
                Title[i][j].Str + (Title[i][j].Value * 4) // String variable
              ); 
            }else{
				if (i<=1 && j==2 && Title[i][0].Value>2){
				
  				  Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                             // Color fixed 
					PRN, 
					x10[Title[i][j].Value]// String variable
					//ChA or ChB x10 value - not flasing
				  );


                                }else if((i==6)&&(j==1)&&(ChartLogic())){                   //for chart speed timebases
				  Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					0x0405,                                           // Color fixed 
					PRN, 
					(char*)SubStr[SubIndex-1]);                       // String variable

                                }else if((i==9)&&(j==0)){                                 //for file function display
				  Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
                                        Title[i][j].Color[0],                    // Color fixed 
					PRN, 
                                        Title[i][j].Str +(Title[i][j].Value * 11) // String variable
					);                       // String variable

				} else {
				  Print_Str(                                       //Upper menu titles and values - not flashing  
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                      // Color fixed 
					PRN, 
					Title[i][j].Str + (Title[i][j].Value *10) 
                                  );
                                }

            }

          } //end of else if(Title[i][j].MARK != NOT)

        }//end of else(if not battery or trigg) end of all other titles



      } else if((Current == i)&&(Detail[i] == j)&&(Blink)){           //end of if flag and update,  current cursor position Item



        Blink = 0;

        if((i == BATTERY)||(i == TRIGG)){

          if((Title[i][j].MARK & FIX)){      // ---- Under fix mode

            if (j==3) {						//show delayed trigger "T" on view area<<SHOULD ADD I=TRIGG?
              if ((FrameMode==2)&&((Options&1)>0)&&(NotificationTimer==0)) {
                Print_Str(Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[Title[i][0].Value],    // Color variable 
                Twink, 
                Title[i][j].Str);                          // String fixed
              }
            }else{
              Print_Str( 
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[Title[i][0].Value],    // Color variable 
                Twink, 
                Title[i][j].Str                          // String fixed
              ); 
            } 

          }else if((i == TRIGG)&&(j==1)){  	             //trig kind, to enable gen kind color shift

              if (GenTrigColorFlag)TmpColor=0x0509;
                else TmpColor=Title[i][j].Color[Title[i][0].Value];
	      Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                TmpColor,    // Color variable 
                Twink, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 

          } else {                                // ---- Under other mode
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[Title[i][0].Value],    // Color variable 
              Twink, 
              Title[i][j].Str +(Title[i][j].Value * 10)// String variable
            ); 
          }


        } else {                                         //other than battery or trigg


          if(Title[i][j].MARK & FIX){                    // ---- Under fix mode    SELECTED FILENAME EDIT BOX WILL END UP HERE

            if((Current==9)&&(_Det==3)&&(i==9)&&(j==3)&&(Title[FILE][0].Value==0)){              //EDIT BOX
              for(l=0;l<8;l++){              
                if(l==CharIndex){
                  Print_Str(
                    Title[i][j].XPOS+(l*8), Title[i][j].YPOS,
                    Title[i][j].Color[0],                        // Color fixed 
                    Twink, 
                    EditChar[l]                                  // String fixed < HILIGHTED CHAR 
                  );
                }else{
                  Print_Str(
                    Title[i][j].XPOS+(l*8), Title[i][j].YPOS,
                    Title[i][j].Color[0],                        // Color fixed 
                    PRN, 
                    EditChar[l]                                  // String fixed < INDEXED 
                  );
                } 
     
              }

            }else if(!((Current==9)&&(_Det==3)&&(i==9)&&(j==3)&&(Title[FILE][0].Value==1))){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                    // Color fixed 
                Twink, 
                Title[i][j].Str                          // String fixed
              );
            }


          }else if(Title[i][j].MARK != NUM3){
            if(i == V_VERNIE)
            Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],        // Color variable
                Twink, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
				// Delta V meter title - flashing
            );else if(i == T_VERNIE){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                    // Color fixed 
                Twink, 
                Title[i][j].Str + (Title[i][j].Value * 4) // String variable
              ); 
            }else{

                        if (i<=1 && j==2 && Title[i][0].Value>2){
				Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,      
					Title[i][j].Color[0],                               // Color fixed 
					Twink,  
					x10[Title[i][j].Value]// String variable
					//ChA or ChB x10 value - flashing
				); 


                          }else if((i==6)&&(j==1)&&(ChartLogic())){                           //for chart speed timebases
				  Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					0x0405,                                             // Color fixed 
					Twink, 
					(char*)SubStr[SubIndex-1]);                         // String variable


                          }else if((i==9)&&(j==0)){                                        //for file function display
                            Print_Str(
 			     Title[i][j].XPOS, Title[i][j].YPOS,
                             Title[i][j].Color[0],                    // Color fixed 
			     Twink, 
                             Title[i][j].Str +(Title[i][j].Value * 11) // String variable
		             );                       // String variable


		         }else
				Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                    // Color fixed 
					Twink, 
					Title[i][j].Str +(Title[i][j].Value * 10)// String variable
					//Upper menu titles and values - flashing
				); 
			}
                   }
            if((i == FILE)&&(Title[i][j].MARK & NUM3)){                            
                u8ToDec3(Title[i][1].Str, Title[i][1].Value,0);
                Print_Str(
                  Title[i][1].XPOS, Title[i][1].YPOS,
                  Title[i][1].Color[0],                  // Color fixed  
                  Twink, 
                  Title[i][1].Str                        // String for numerical
                );

            }


        }       //end of all other titles other than battery or trigg

      }		//end of current position flashing
    }		//for j

  }		//for i

}		//function

/*******************************************************************************
 Load_Attr:   load the hardware properties
*******************************************************************************/
void Load_Attr(void)
{
  u16 i;
  
  for(i=0; i<G_Attr[0].Yp_Max+1; ++i) 
    strcpy(&Vertical[i][0], Y_Attr[i].STR);

  Title[TRACK1][RANGE].Limit = G_Attr[0].Yp_Max;
  Title[TRACK2][RANGE].Limit = G_Attr[0].Yp_Max;
  Title[TRACK3][RANGE].Limit = 0;
  
  for(i=0; i<G_Attr[0].Xp_Max+6; ++i) 
    strcpy(&BaseStr[i][0], X_Attr[i].STR);
  Title[T_BASE][BASE].Limit = G_Attr[0].Xp_Max;
  
}
/*******************************************************************************
 Update_Battery:  refresh the battery indicator
*******************************************************************************/
void Update_Battery(void)
{
  u16 Vb, Level;
  
  Vb = __Get(V_BATTERY);
  if (Cnt_InCharge>0) Cnt_InCharge--;
  if (Cnt_Charged>0) Cnt_Charged--;
  Cnt_Batt++;
  if (Cnt_Batt>4) Cnt_Batt=1;
  
  if (__Get(CHARGE)==1){
    Cnt_Charged=4;
     if (Cnt_InCharge==0)FlagInCharge=1;
 }
    else
  {  
    Cnt_InCharge=4;  
    if (Cnt_Charged==0)FlagInCharge=0;
   
  }
  
  if (FlagInCharge==1){        //batteria in carica
    Level = Cnt_Batt ;
  }
  else
  {
    Level =0;	//3000,3500,3900,4100 (or)    3200,3600,3900,4100 (mod)     3400,3600,3800,3990 >Batt seemed to die off very fast after going from 1/2 to 1/4...
                //3400,3625,3775,3975 > effort to even transition times a bit, seemed to spend most of it's time at mid bar 
    //if(Vb > 3000 ) Level +=1;
    if(Vb > 3400 ) Level +=1;
    //if(Vb > 3500 ) Level +=1;

  //if(Vb > 3600 ) Level +=1;
  //if(Vb > 3800 ) Level +=1;
  //if(Vb > 3990 ) Level +=1; 
    if(Vb > 3625 ) Level +=1;
    if(Vb > 3775 ) Level +=1;
    if(Vb > 3975 ) Level +=1; 

     
  }  
  if (Level>4) Level=4;
    Print_Str(
    Title[4][1].XPOS, Title[4][1].YPOS,
    Title[4][1].Color[Level], 
    PRN,
    (Title[4][1].Str + Level*10) 
  );    
}


void DisplayBigMeter(u8 MeterNum){

  u16  color;
  u8   j;
  u8   BigMeterYpos1=0;
  u8   BigMeterYpos2=0;
  char BigMeterStr1[7];			//values
  char BigMeterStr2[4];			//suffix
  char BigMeterStr3[5];			//items
  BigMeterStr1[5]=0;			//termination nulls
  BigMeterStr2[2]=0;
  BigMeterStr3[3]=0;
  char RefStr[10]=" DCmV?k!S";		//reference matching characters	

  switch (MeterNum){			//meter positions
    case 1:  
      BigMeterYpos1=154;
      BigMeterYpos2=129;
    break;
    case 2:
      BigMeterYpos1=97;
      BigMeterYpos2=72;  
    break;               //32
    case 3:
      BigMeterYpos1=40;  //25
      BigMeterYpos2=15;
  }

  switch (Meter[MeterNum].Item){       //format string 1, values
    case VBT:
    case VDC...RMS:
    case MAX...VPP:
      if((NumStr[5]==RefStr[3])||(NumStr[5]==RefStr[4])){	//look for "m" or "V" for all volt meters	
        for (j=0;j<5;j++)BigMeterStr1[j]=NumStr[j];		
      }else{							//if "KV" or ending of number, strip leading zero and replace with sign
        BigMeterStr1[0]=NumStr[0];				//puts "-" or "+" sign before number, instead of leading "0" in 0.0
        for (j=1;j<5;j++)BigMeterStr1[j]=NumStr[j+1];           //ie:   +.124  rather than 0.124 for some ranges so all of number fits
      }
      break;
    case FPS:
      BigMeterStr1[5]=0; 
    case FRQ:
    case CIR...TL:
      for (j=0;j<5;j++)BigMeterStr1[j]=NumStr[j];

  }

  switch (Meter[MeterNum].Item){       //format string 2, suffix
    case VBT: 
    case VDC...RMS:
    case MAX...VPP:
      if(NumStr[5]==RefStr[3]){					//"m", so copy 2 ("mV")
        for (j=0;j<2;j++)BigMeterStr2[j]=NumStr[j+5];
      }else if((NumStr[5]==RefStr[4])||(NumStr[6]==RefStr[4])){ //"V"
        BigMeterStr2[0]=RefStr[7];				//"!"
        BigMeterStr2[1]=RefStr[4];
        BigMeterStr2[2]=RefStr[7];
        BigMeterStr2[3]=0;
      }else{
        BigMeterStr2[0]=RefStr[6];				//"kV" character
        BigMeterStr2[1]=RefStr[4];
      }
      break;
    case FRQ:
        for (j=0;j<2;j++)BigMeterStr2[j]=NumStr[j+5];		//HZ, etc
        break; 
    case CIR:
    case TH:
    case TL:
      if(NumStr[5]==RefStr[8]){					//"S"
        BigMeterStr2[0]=RefStr[7];				//"!"
        BigMeterStr2[1]=RefStr[8];
        BigMeterStr2[2]=RefStr[7];
        BigMeterStr2[3]=0;
      }else for (j=0;j<2;j++)BigMeterStr2[j]=NumStr[j+5];	//"uS" etc
      break; 
   case DUT:
        BigMeterStr2[0]=RefStr[7];				//"!"
        BigMeterStr2[1]=NumStr[5];
        BigMeterStr2[2]=RefStr[7];
        BigMeterStr2[3]=0;
      break;
   case FPS:
      for(j=0;j<2;j++)BigMeterStr2[j]=RefStr[0];
 }

 switch (Meter[MeterNum].Item){       //format string3, items
    case VBT...DUT:
      for (j=0;j<3;j++)BigMeterStr3[j]=METER[Meter[MeterNum].Item][j];
      break; 
    case TH...TL:
      for (j=0;j<4;j++)BigMeterStr3[j]=METER[Meter[MeterNum].Item][j];	//"!TH!,!TL!"
      BigMeterStr3[4]=0;						//termination null moved to 5th place to allow for the 2 half spaces
  }

  if((Meter[MeterNum].Item<9)||(EnablePaS)){                            //only update TH,TL,%duty values once per second
    Print_Str2(			     //display values	                //these generate much "bobbling" @3/sec due to uncompensated
    314,LCD_X2,BigMeterYpos1,	     //x and y positions		//quantization errors from PxS starting and stopping at every wave		  
    Y_COLOR[Meter[MeterNum].Track], 					//also extends averaging numbers if available (at the faster TB's)
    PRN,
    BigMeterStr1,
    3,3);			    //leading, trailing blanks  	

    Print_Str2(			    //display suffix	
    313,347,BigMeterYpos2,				  
    Y_COLOR[Meter[MeterNum].Track], 
    PRN,			      			    	
    BigMeterStr2,
    2,0);
  }

    if(((Meter[MeterNum].Item==MIN)||(Meter[MeterNum].Item==MAX))&&((Options&4)==4))
      color=Y_COLOR[4]; else color=Y_COLOR[Meter[MeterNum].Track];     			//display items in white for hold mode

    Print_Str2(			    	
    346,LCD_X2,BigMeterYpos2,				  
    color, 
    BTwink,			      			    	
    BigMeterStr3,
    12,1);	 			    //>10 leading blanks=leading line + (-10) leading blanks				

    __LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  		// restore the full-size window
 
}

u32 TimeScaleCorrection(u32 Tmp, u8 Polarity, u16 Kp){
                                    //Compensates for truncation of "k" scaling values in interpolated ranges
                                    //Prevents these errors to be further multiplied by IP scaling factors                          
                                    //Uses modified (X10, X100) Kp IP scaling factors as reference  
        switch (Kp){
          case 24576:						
          case 8192:						        // 1/.1 uS/div, k returns 10ppm off
            if(Polarity==0)Tmp-=(Tmp/100000); else Tmp+=(Tmp/100000);   // k here equals 33333 in which error is neg as opposed to other
            break;  							// k values
          case 6144:                                     	        // .5uS/div, k returns 20ppm off
            if(Polarity==0)Tmp+=(Tmp/50000); else Tmp-=(Tmp/50000); 
            break;  
          case 9830:					                //2uS/div, k returns 50ppm off (eg: (6667-6666.6666666)/6667)*1e6 = 50)     
          case 4915:						        //.2uS/div, k returns 50ppm off, + kp returns 41ppm off = 91ppm off) 
            if(Polarity==0)Tmp+=(Tmp/10989); else Tmp-=(Tmp/10989); 
        }
        return Tmp;
}


u32 UpScale(u32 Tmp, u16 limit){

    if(Tmp>4290){			//up scale,increases resolution with low number of waves counted when dividing with integer math
      while(Tmp<429000000){             //specially in large buffer mode. Makes sure the maximum number of digits of precision can
        Tmp*=10;                        //be considered under all conditions by making #'s as high as possible with 
        ShiftDigits++;                  //u32 variable minus some headroom for following adjustments.
      }					//ShiftDigits compensates for this in Int2Str
    }else ShiftDigits=0;

    while(limit>1){					//scale back down if limit is called for
      if(ShiftDigits>0)ShiftDigits--; else break;
      Tmp/=10;
      limit/=10;
    }
    return Tmp;
}

void CalculateTimeMeters(u32 Tmp, u32 m, u32 n, u16 Kp){
s32 STmp;

    if(ChartLogic()){Tmp=0;ShiftDigits=0;}              //do not enable time meters while in chart mode 
    if(n<10){
      while((m>1)&&(ShiftDigits>0)){			//remove any up scaling in the lower ranges
        m/=10;
        ShiftDigits--;
      }    
      if(Tmp <= 0x7FFFFFFF/m) Tmp*=m;
      else                    Tmp = 0x80000000;
    }else{
      Tmp=UpScale(Tmp,1);				//up scale after dividing by TxN, so significant digits are not lost
      if(Kp==1024){
        Tmp=((Tmp+(n/2))/n);
      }else{
        Tmp=TimeScaleCorrection(Tmp,1,Kp);		//add Kp IP scaled correction
        Tmp=((Tmp+(n/2))/n);				//sets range, with k factor selecting ranges within each decade
        Tmp=UpScale(Tmp,10);				//up scale after dividing by n, so significant digits are not lost, restrict by X10 to leave room for IP scaling
        if(Kp==24576){					//IP scaling                               
          Tmp=(102400*((Tmp+(Kp/2))/Kp));		//special case .1uS/div with Kp X 100
        }else Tmp=(10240*((Tmp+(Kp/2))/Kp));            //rest of IP ranges, Kp X 10
      } 
    }

        if(Tmp>0x7FFFFFFF){				//convert to signed var for Int2Str				 
          STmp=(Tmp/10);
          ShiftDigits--;
        }else STmp=Tmp;

        if(ChartLogic()){STmp=0;ShiftDigits=0;}        //don't calculate time meters when using chart

      Int2Str(NumStr, STmp, T_UNIT, 4, UNSIGN, ShiftDigits);


}

u32 Period(u8 Ch,u32 k){ 
u32 Tmp;
      Tmp=UpScale(Sum[Ch],1);
      Tmp=ClockAdjust(Tmp);
      ShiftDigits=-ShiftDigits;				//upscaled denominator so works backwards, invert shift    
      return(UpScale(((k*100000)/(Tmp/100000)),1)/10);    //scales by e10 (100000*100000) for best resolution in all cases,
}							// div/10 to bring down to original freq scaling of 1e9

u32 ClockAdjust(u32 Value){
    if((PPM_Comp>0)&&(PPM_Comp<1000)){			//do not use if out of range, indicating a read error
      return Value+=(Value/(1000000/PPM_Comp));
    }else if((PPM_Comp<0)&&(PPM_Comp>-1000)){
      return Value-=(Value/(1000000/-PPM_Comp));	//change PPM_Comp to + so works correctly with unsigned var	
    }else return Value;				 
}


u8 MeterChannelLogic(u8 i,u8 channel){
   switch (channel){
     case 0:
       return((Meter[i].Track == TRACK1)&&((_1_source != HIDE)||(_4_source>13)));
     case 1:
       return((Meter[i].Track == TRACK2)&&((_2_source != HIDE)||(_4_source==12)||(_4_source==13)));
     case 2:
       return((Meter[i].Track == TRACK3)&&(_3_source == CH_C));
     case 3:
       return((Meter[i].Track == TRACK4)&&((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)));
   }
   return 0;
    
}

void FFTlevel(u8 service){
char FFTorderSTR[5];

  if((Current==TRACK4)&&(_Det==1)){
    if(DownConvertMode){
      if(FFTt1Mode){
        Print_Str(112,0,0x0405,PRN,"T1 MODE MAG= X ");
        u8ToDec3(FFTorderSTR,DownConvert,1);
        Print_Str(234,0,0x0405,PRN,(char*)FFTorderSTR);
      }else{
        Print_Str(128,0,0x0405,PRN,"AUTO  MAG= X ");
        u8ToDec3(FFTorderSTR,DownConvert,1);
        Print_Str(234,0,0x0405,PRN,(char*)FFTorderSTR);
      } 
    }else{
      if(FFTt1Mode){
        Print_Str(112+(8*4),0,0x0405,Twink,"T1 MODE MAG= 0");
      }else{
        Print_Str(112+(8*5),0,0x0405,Twink, "AUTO  MAG= 0");
      }
    }
  }

  if((Current==TRACK4)&&(_Det==2)){
      if(_4_source>10){
        AutoFFT=Title[TRACK4][2].Value;
        if(AutoFFT>1)FFTGain=AutoFFT-2;else{if(AutoFFT==0)FFTGain=8;}
        if(service)UpdateBackground=3;
        else {
          Print_Str(144,0,0x0405,PRN,"FFT Gain=");
          Print_Str(218,0,0x0405,PRN,(char*)FFT_GAIN_STR[AutoFFT]);
        }
      }else _Det=0;
  }   
}

void TIM_2IRQControl(void){                    
 if(((Sweep)&&(_Kind<6))||(_Kind==5)){                                  //sweeps or bursts/sec= (Tim2Factor*SweepStep)/SweepMod
  //    SweepStep    =  1 1 1 1 1 1  2  2  2  2
  //    TIM2Speed[]  = {1,2,1,2,4,10,10,20,50,100};                     //lookup table for tim2 speed divider  (timer period=1000/x)
  //    SweepMod     =  5 5 1 1 1 1  1  1  1  1
   TIM2->DIER  =  0x0001;                                               //enable TIM2_INT  
   if(SweepIndex>9)SweepIndex=2;                                        //make sure we don't go out of bounds with improper config file read 
   Tim2Factor=TIM2Speed[SweepIndex];                                    //index sets timer speed
   if(_Kind==5){  
     TIM2->ARR=(1000/Tim2Factor)-4;                                       //timer call, xx microseconds per IRQ call,
   }else{
     TIM2->ARR=(1000/Tim2Factor)-1;                                       //timer call, xx microseconds per IRQ call,
   }
   if(SweepIndex<2)SweepMod=5; else SweepMod=1;                         //modulus factor to slow down sweep function
   if(SweepIndex>5)SweepStep=2; else SweepStep=1;                       //=1 step all 200, =2 step every other (100)
 }else{
   TIM2->DIER  =  0x0000;                                               //disable TIM2_INT  
   WaveGen();                                                           //in case stopped on space
   TIM4->CCER=0x0001;                                                   //turns PWM timer back on    (?)				                            
   OutputAdjust();                                                      //reset to saved freq
 }
}

void OutputAdjust(void){
s32 tmp;
u16 arr;
u16 PrevARR;
u16 i;

   if(_Kind==PWM)arr=D_Tab[_Frqn].ARR;
     else if(_Kind==7)arr=ARBT_ARR;
       else arr=A_Tab[_Frqn];
   if(arr>200) tmp=((arr*(300+GenFreqShift))+((300-GenFreqShift)/2))/(300-GenFreqShift);          //WITH ROUNDING
     else tmp=arr+GenFreqShift;
   if(tmp>0xFFFF)tmp=0xFFFF; 

   if(_Kind == PWM){
     if(tmp<9){
       if(Sweep==1){
         GenFreqShift=-100;
       }else{
         tmp=8;
         GenFreqShift=tmp-arr;
       }
     }								

     PrevARR=TIM4->ARR;  
     __Set(DIGTAL_ARR,(u16)tmp);

     if((Sweep==1)||(PWAdjustMode==0)){                                                             //sweep or reg PWM mode
       i=(TIM4->ARR*(30000-Title[OUTPUT][DUTYPWM].Value))/30000;
       if(i==0)i++;
       if(i>(TIM4->ARR))i=(TIM4->ARR);        //was -1 on both
       TIM4->CCR1=i;
     }else if(GenAdjustMode){    
       if ( TIM4->CCR1<(PrevARR/2) ){                                                                 //preserve negative pulses (duty%>50)
         if(TIM4->CCR1>=(tmp/2))TIM4->CCR1=(tmp/2);
       }else{                                                                                       //positive pulses (duty%<50)
         TIM4->CCR1= tmp-(PrevARR-TIM4->CCR1);
         if(TIM4->CCR1<=(tmp/2))TIM4->CCR1=(tmp/2);
       }
       UpdatePWMDuty(1);
     }

     if((Sweep==1)&&(EnablePWM)){
       TIM4->CR1 = 0x0081;           // SQR_OUT = Enable
       GPIOB->CRL &= 0xF0FFFFFF;  
       GPIOB->CRL |= 0x0B000000;     // PORT_SQR = Enable (puts PWM port(PB6) in output, push-pull)
       EnablePWM=0;  
     }
 
   }else if((_Kind>4)&&(_Kind<7)){                // DAC noise or off mode
     return;
   }else{
     if(tmp<40){
       if((Sweep==1)&&(_Kind!=7)){                //if sweep mode, exclude arbitrary mode
         GenFreqShift=-100;                       //end sweep
       }else{                                     //if manual mode
         tmp=39;                                  //limit to sampling frequency of 1.8Mhz
         GenFreqShift=tmp-arr;
       } 
     }						
     __Set(ANALOG_ARR,tmp);
   }
   CurrentARR=tmp;
 } 

void DisplaySamples(void){
char TempStr[10];
u32 UTmp=0;
u8 scale,i=0,j;
u16 offset;
char PPM_Str[5];

  if(AutoSetFlag){
    if(AutoSetFlag==2){
      Print_Str(95+(4*8),0,0x0405,PRN,   "AUTOSET: NO SIGNAL");
    }else{
      Print_Str(95+(4*8),0,0x0405,PRN,   "     AUTOSET      ");          
    }
  }

  if((OffsetSelect==1)&&(Current>1)){ 
    u8ToDec2(TempStr,ADCoffset);                                                                  
    Print_Str(132+(8*2),0,0x0405,PRN,"ADCoffset= ");
    Print_Str(131+(8*13),0,0x0405,Twink,"");
    Print_Str(132+(8*13),0,0x0405,PRN,TempStr);
  }

  if((OffsetSelect==2)&&(Current>1)){ 
    s16ToDec3(PPM_Str, PPM_Comp);
    Print_Str(127,0,0x0405,PRN,"Clock PPM Adj=");
    Print_Str(127+(8*14),0,0x0405,Twink,"");
    Print_Str(127+(8*14),0,0x0405,PRN,PPM_Str);
  }

  if((Current==T_BASE)&&(_Det==BASE)&&(ChartLogic())){
    if(AutoSaveSelect==1){
      if(AutoSaveBuf==0){
        Print_Str(95+(2*8),0,0x0405,PRN, "Chart auto save   OFF ");
      }else if(AutoSaveBuf==1){
        Print_Str(95+(2*8),0,0x0405,PRN, "Chart auto save BUF ON");
      }else if(AutoSaveBuf==2){
        Print_Str(95+(2*8),0,0x0405,PRN, "Chart auto save CSV ON");
      } 
    } 
    if(AutoSaveSelect==2){
      if(ChartMode==0){
        Print_Str(95+(2*8),0,0x0405,PRN, "CHART MODE=  AVERAGE  ");
      }else if(ChartMode==1){
        Print_Str(95+(2*8),0,0x0405,PRN, "CHART MODE= OVERSAMPLE");
      } 
    } 
  }

  if((Current==VOLUME)&&(AdjBeepLevel)){
    u8ToDec2(TempStr,9-LoBeepLevel);
    Print_Str(92+(5*8),0,0x0405,PRN,"Fast rep vol= ");  
    Print_Str(92+(19*8),0,0x0405,Twink,"");
    Print_Str(93+(19*8),0,0x0405,PRN,TempStr);
  }

  if(Current==BK_LIGHT){
    if(FastDimAdjust){                 
      u8ToDec2(TempStr,FastDim);
      Print_Str(92+(5*8),0,0x0405,PRN,"Vert Dim Adj= ");         
    }else if (HboldAdjust){
      u8ToDec2(TempStr,3-Hbold);
      Print_Str(92+(5*8),0,0x0405,PRN,"Hor Bold Adj= ");
    }else goto ExitIfBlock;
    Print_Str(92+(19*8),0,0x0405,Twink,"");
    Print_Str(93+(19*8),0,0x0405,PRN,TempStr);
  }
ExitIfBlock:


  if((Current==5)&&(_Det==1)&&(_Kind==8)){                                         //uart gen mode
    Print_Str(92,0,0x0405,PRN,"BAUD ");
    if(GenBaudAdjSpeed)Print_Str(92+(5*8),0,0x0405,PRN,"FAST=");
      else Print_Str(92+(5*8),0,0x0405,PRN,"FINE=");
    u32ToDec7(TempStr,GenBaudRate);                                                //GenBaudRate= 1200 to 4500000
    for(j=0;j<7;j++)if(TempStr[j]==48)TempStr[j]=32;else break;                    //strip off leading 0's
    Print_Str(93+(10*8),0,0x0405,PRN,TempStr);
    Print_Str(93+(17*8),0,0x0405,PRN," ");

    Print_Str(94+(18*8),0,0x0405,PRN,(char*)GEN_UART_MODE_STR[GenUartMode-1]);       
    Print_Str(94+(21*8),0,0x0405,PRN," ");

    if(GenUartStopBits==0)Print_Str(95+(22*8),0,0x0405,PRN,"1S");                 
      else Print_Str(95+(22*8),0,0x0405,PRN,"2S");
    Print_Str(95+(24*8),0,0x0405,PRN," ");

    if(GenUartCont)Print_Str(96+(25*8),0,0x0405,PRN,"C");                 
      else Print_Str(96+(25*8),0,0x0405,PRN,"1");

    if(GenUartAdj==0)i=Twink;else i=PRN;                                           //=blink on adjust positions
    Print_Str(92+(10*8),0,0x0405,i,"");
    if(GenUartAdj==1)i=Twink;else i=PRN;                                       
    Print_Str(93+(18*8),0,0x0405,i,"");
    if(GenUartAdj==2)i=Twink;else i=PRN;                                       
    Print_Str(94+(22*8),0,0x0405,i,"");
    if(GenUartAdj==3)i=Twink;else i=PRN;                                       
    Print_Str(95+(25*8),0,0x0405,i,"");
  }

  if((SpiLogic())&&((Current==TRACK2)&&(_Det==1))){  
    if(SpiChartFlag)Print_Str(117,0,0x0405,PRN,"SPI DECODE CHART  ON");                 
      else Print_Str(117,0,0x0405,PRN,"SPI DECODE CHART OFF");                 
  }
  if((SpiLogic())&&((Current==TRACK1)&&(_Det==1))){  

    Print_Str(100,0,0x0405,PRN,"Pol/Pha/Ord=");
    if(SpiNumBits>0){
      i=SpiNumBits;
      Print_Str(102+(8*15),0,0x0405,PRN," Bits=");
    }else if(SpiNumBits<0){
      i=-SpiNumBits;
      Print_Str(102+(8*15),0,0x0405,PRN," )!<");            
      Print_Str(107+(8*21),0,0x0405,PRN,">!(");
    }else{
      Print_Str(102+(8*15),0,0x0405,PRN,"   AUTO  ");
    }

    u8ToDec3(TempStr,i,0);   
    for(j=0;j<7;j++)if(TempStr[j]==48)TempStr[j]=32;else break;            //strip off leading 0's
    if(SpiNumBits>0){
      Print_Str(103+(8*21),0,0x0405,PRN,TempStr);                            //bits
    }else if(SpiNumBits<0){
      Print_Str(107+(8*18),0,0x0405,PRN,TempStr);                            //bits
    }
    if(SpiMode&1)TempStr[0]=49;else TempStr[0]=48;
    if(SpiMode&2)TempStr[1]=49;else TempStr[1]=48;
    TempStr[2]=0;
    Print_Str(101+(8*12),0,0x0405,PRN,TempStr);                            //mode

    if(SpiBitOrder)TempStr[0]=60;else TempStr[0]=62;TempStr[1]=0;
    Print_Str(102+(8*14),0,0x0405,PRN,TempStr);                            //bit order

    if(SpiAdj==1)i=Twink;else i=PRN;                                       //adjust mode 
    Print_Str(100+(8*12),0,0x0405,i,"");
    if(SpiAdj==2)i=Twink;else i=PRN;                                       //adjust bit order
    Print_Str(101+(8*14),0,0x0405,i,"");
    if(SpiAdj==3)i=Twink;else i=PRN;                                       //adjust number of bits per word
    if(SpiNumBits>0){
      Print_Str(102+(8*21),0,0x0405,i,"");
    }else if(SpiNumBits<0){
      Print_Str(106+(8*18),0,0x0405,i,"");
    }else{
      Print_Str(101+(8*18),0,0x0405,i,"");
    }
  }

  if((UartLogic())&&(((Current==TRACK1)&&(_Det==1))||(Current==T_VERNIE)||((Current==T_BASE)&&(_Det==1)))){  

    if(DataSize>9){                                              //datasize=5 to 9 (+10 if parity)
      i=DataSize%10;
      Parity=1;
    }else {Parity=0;i=DataSize;}
    FrameSize=i+2+Parity;
    if(_T_base<15){                                              //<10uS/div      
      if(_T2>(_T1+2))UTmp=((1000000000/(_T2-_T1))*FrameSize)/X_Attr[_T_base].SCALE;else UTmp=0;
    }else{
      if(_T2>(_T1+2))UTmp=(((1000000000/(_T2-_T1))*FrameSize)/X100TbaseScale[_T_base-15])*100;else UTmp=0;
    }

    if(UTmp>9999999)UTmp=9999999;
    u32ToDec7(TempStr,UTmp);				
    for(j=0;j<7;j++)if(TempStr[j]==48)TempStr[j]=32;else break;  //strip off leading 0's

    if(T1Start==0){
        if(ValidFrame)Print_Str(93,0,0x0405,PRN,"T2-1=");
          else Print_Str(93,0,0x0605,PRN,"T2-1=");
      }else{
        if(ValidFrame)Print_Str(93,0,0x0305,PRN,"!T1+2!");
          else Print_Str(93,0,0x0F05,PRN,"!T1+2!");
      }

    Print_Str(133,0,0x0405,PRN,"BAUD=");
    Print_Str(133+(8*5),0,0x0405,PRN,TempStr);
    TempStr[0]=i+48;TempStr[1]=0;                               //convert to char, add terminating nul 
    Print_Str(133+(8*12),0,0x0405,PRN," DATA=");
    if(((Current==TRACK1)&&(_Det==1))&&(Title[TRACK1][1].Value>2)&&(DataAdj))i=Twink;else i=PRN;
    Print_Str(134+(8*18),0,0x0405,i,"");
    Print_Str(135+(8*18),0,0x0405,PRN,TempStr);

    if(Parity)Print_Str(135+(8*19),0,0x0405,PRN," P");
      else Print_Str(135+(8*19),0,0x0405,PRN," N");
  }

  if((Current==6)&&(_Det==3)){
    if(_Mode==X_Y){
      u8ToDec3(TempStr,BufferSize,1);				//u8ToDec3 also doubles as a u16to Dec4 with option 1
      Print_Str(160,0,0x0405,PRN,"SAMPLES=");
      Print_Str(160+(8*8),0,0x0405,PRN,TempStr);
    } 
    if(_Mode==NORHLD){
      u8ToDec3(TempStr,PerstFrameNumber,0);				
      Print_Str(160,0,0x0405,PRN,"FRAMES=");
      if(PerstFrameNumber){
        Print_Str(160+(7*8),0,0x0405,PRN,TempStr);
      }else{
        Print_Str(160+(7*8),0,0x0405,PRN,"CONT");
      }    
    }
  }

  if((Current==5)&&(_Det==2)&&(PWAdjustMode)&&(_Kind==4)){      //pulse mode

    if (TIM4->CCR1> (TIM4->ARR/2)){
      if(_Frqn<3) UTmp=((u32)((TIM4->PSC+1)*((TIM4->ARR+1)-TIM4->CCR1)*100)/72)*10;
        else UTmp=(u32)((TIM4->PSC+1)*((TIM4->ARR+1)-TIM4->CCR1)*1000)/72;
      Print_Str(114,0,0x0405,PRN,"+PULSE LENGTH=");
    }else{
      if(_Frqn<3)UTmp=((u32)((TIM4->PSC+1)*TIM4->CCR1*100)/72)*10;
        else UTmp=(u32)((TIM4->PSC+1)*TIM4->CCR1*1000)/72;
      Print_Str(114,0,0x0405,PRN,"-PULSE LENGTH=");
    } 
    if(UTmp<1000)scale=4;else scale=0;
    Int2Str(TempStr,(s32)UTmp,T_UNIT,4,UNSIGN,scale);
    if(UTmp<1000){TempStr[0]=32;TempStr[1]=32;}
    if((UTmp<100)&&(UTmp>0))TempStr[2]=32;
    Print_Str(116+(8*14),0,0x0405,PRN,TempStr);
    Print_Str(115+(8*14),0,0x0405,Twink,"");                                                					
  }

  if((Current==5)&&(_Det==1)&&(GenAdjustMode)&&((_Kind<5)||(_Kind==7))){

   if(_Kind==7)goto ArbtBypass;                                             //do not engage sweep/burst functions with arbitrary wave mode
   if(Sweep==1){                                                            //adjust sweep period 

     if(_Kind==PWM){
       if(_Frqn<18)i=10;else i=TrunkFactor[_Frqn-18];
     }else{
       if(_Frqn<9)i=10;else i=TrunkFactor[_Frqn-9];
     }
     UTmp=((TIM2Speed[SweepIndex]*SweepStep*5)/SweepMod);
     scale=1;
     if(UTmp<200)UTmp=((100*i)/UTmp);
       else {UTmp=((1000*i)/UTmp);scale=10;}

     u8ToDec3(TempStr,UTmp,1);				                    //works as u16 to dec 4 with option 1
     for(i=0;i<4;i++)if(TempStr[i]==48)TempStr[i]=32;else break;            //strip off leading 0's
     if(scale==10){for(i=0;i<2;i++)TempStr[i]=TempStr[i+1];TempStr[2]=46;}
     Print_Str(118,0,0x0405,PRN,"SWEEP PERIOD =");                      
     Print_Str(120+(8*14),0,0x0405,PRN,TempStr);
     Print_Str(120+(8*18),0,0x0405,PRN,"mS");
     Print_Str(119+(8*14),0,0x0405,Twink,"");                                      					

   }else if((Sweep==2)||(Sweep==3)){                                        //adjust tone burst period

//=================================DISPLAY BOTH RATE IN X/SEC AND PERIOD IN MS==================================
     Print_Str(92,0,0x0405,PRN,"BURST RATE=");
     Print_Str(94+(8*14),0,0x0405,PRN,       "/S PER=");

     UTmp=((TIM2Speed[SweepIndex]*SweepStep*5)/SweepMod);                    //burst rate
     if(UTmp>999)Print_Str(94+(8*11),0,0x0405,PRN,"!1K!");else{
       u8ToDec3(TempStr,UTmp,0);				            
       for(i=0;i<3;i++)if(TempStr[i]==48)TempStr[i]=32;else break;          
       Print_Str(94+(8*11),0,0x0405,PRN,TempStr);
     }
     UTmp=1000000/UTmp;                                 
     offset=UTmp/100;                                                        //start offset
     if(SweepIndex<6)offset/=2;                                              //compensate for 50/s - 100/s breakpoint (CHECK IF OK ALL THE WAY DOWN)
     if(SweepIndex<2)offset=4000;                                            //4mS timing for cnt ms reset at 19 > 15 for mod 5=0
     UTmp=((UTmp*BurstLimit)+100)/200;
     UTmp+=offset;
     if(((SweepIndex<2)&&(_Frqn>5))||(_Frqn>13))
       UTmp+=((FineAdjust*3*FineAdjustFactor[_Frqn-6])/16);                  //fine adjust offset for high frequencies

     if(UTmp>=1000){                                                        
       i=0;
       while(UTmp>=1000){UTmp/=10;i++;}                                      //reduce to 3 decimals of precision, keep track of how much we divided
       u8ToDec3(TempStr,UTmp,0);				             //convert to string, function modified to work with u16 as well              
       if(i==1){                                                             //for 1.0mS to 9.9mS
         TempStr[2]=TempStr[1];                                              //make space and add period
         TempStr[1]=46;
       }
       if(i==2){
         TempStr[2]=TempStr[1];                                              //for 10mS to 99mS, shift right one decimal digit.
         TempStr[1]=TempStr[0];
         TempStr[0]=32;
       }
       Print_Str(97+(8*24),0,0x0405,PRN,"mS");                               //For 100mS and up (i=3) use as is
     }else{
       u8ToDec3(TempStr,UTmp,0);				                    
       Print_Str(97+(8*24),0,0x0405,PRN,"uS");
     }
     for(i=0;i<3;i++)if(TempStr[i]==48)TempStr[i]=32;else break;             //strip off any leading zeros      
     Print_Str(96+(8*21),0,0x0405,PRN,TempStr);

     if(Sweep==2){
       Print_Str(181,0,0x0405,Twink,"");                                    					
       Print_Str(263,0,0x0505,PRN,"");
     }else{
       Print_Str(263,0,0x0405,Twink,"");
       Print_Str(181,0,0x0505,PRN,"");                                     					
     } 
//==============================================================================================================
   }else{                                                                     //sweep=4 and 0, adj freq display for burst (4) and continuous (0) 
ArbtBypass:
     scale=3;
     if(_Kind==PWM){
       if(_Frqn>8){
         UTmp=720000000/((D_Tab[_Frqn].PSC+1)*(CurrentARR+1));                //scaled up x10
         scale=1; 
       }else{
         UTmp=720000000/(((D_Tab[_Frqn].PSC+1)*(CurrentARR+1))/100);          //scaled up x1000
       }
     }else if(_Kind==7){
         UTmp=((TIM7->PSC+1)*(CurrentARR+1)*ArbtSampleNumber);
          if(UTmp>720000){
           UTmp=720000000/(UTmp/100);                                         //scaled up x1000
         }else{
           UTmp=720000000/UTmp;                                               //scaled up x10
           scale=1;
         }
     }else{
       if(_Frqn>13){
         UTmp=((10000000*ScaleIndex[_Frqn])/(CurrentARR+1))*10;
       }else{
         UTmp=1000000000/(((CurrentARR+1)*10)/ScaleIndex[_Frqn]);            //scaled up x1000
       }
       if(_Frqn==0)UTmp/=2;                                                  //uses 36 Mhz clock
     }   
     if(UTmp<1000)scale=0;
     Int2Str(TempStr,UTmp, F_UNIT , 4, UNSIGN,scale);

     if((Sweep==4)&&(_Kind!=7)){
       Print_Str(120,0,0x0405,PRN,"BURST  FREQ= ");
     }else{
       Print_Str(120,0,0x0405,PRN,"FREQ OUTPUT= ");
     }
     Print_Str(120+(8*13),0,0x0405,PRN,TempStr);
     Print_Str(119,0,0x0405,Twink,"");                                       //puts vert blinking bar ahead of text					
   } 
  }

}


void GenTrigColor(void){
static u8 PrevGenTrigColorFlag;

  if((Sweep)&&(_Tr_kind==8)&&(_Kind<5)){
    if(Sweep==1){
      if(!((((s8)(_T_base-SweepIndex))<8)&&(((s8)(_T_base-_Frqn))<2)&&(((s8)(_Frqn-SweepIndex))>1)))GenTrigColorFlag=1; 
        else GenTrigColorFlag=0;
    }else{
      if(!((((s8)(_T_base-SweepIndex))<6)&&(((s8)(_T_base-_Frqn))<2)&&(((s8)(_Frqn-SweepIndex))>1)))GenTrigColorFlag=1; 
        else GenTrigColorFlag=0;
    }
  }else GenTrigColorFlag=0;

  if(GenTrigColorFlag!=PrevGenTrigColorFlag)Title[TRIGG][1].Flag=UPDAT;
  PrevGenTrigColorFlag=GenTrigColorFlag;
}

void 	UpdatePWMDuty(u8 service){
 if(service) Title[OUTPUT][DUTYPWM].Value=(((TIM4->ARR)-TIM4->CCR1)*30000)/(TIM4->ARR);
     else  Title[OUTPUT][DUTYPWM].Value=(((D_Tab[_Frqn].ARR)-TIM4->CCR1)*30000)/(D_Tab[_Frqn].ARR);
 if(Title[OUTPUT][DUTYPWM].Value<1)Title[OUTPUT][DUTYPWM].Value=1;
 if(Title[OUTPUT][DUTYPWM].Value>29999)Title[OUTPUT][DUTYPWM].Value=29999;
}

void   AlignTbaseSweep(u8 dir){   //call BEFORE engaging or changing coupling (0,1,4,5) and AFTER changing either sweep rate or frqn (2)
static s8 OldTbase=-1;

  if((Sweep==1)&&(_Kind<4)){ 

      if(dir<2){
        if((ChDetLogic(Current,dir))&&(!DetStatusLogic(0))){OldTbase=_T_base; SyncSweep(1);}  //if about to engage det mode save previous TB
          else if((ChDetLogic(Current,2))&&(!DetStatusLogic(1))){                             //if leaving with the only ch on with det mode                                                      //if in det mode and leaving restore previous TB
            if(OldTbase>-1){
              _T_base=OldTbase;
              Update_Base();
              Title[T_BASE][BASE].Flag |= UPDAT; 
            }
          }                                        
      }else if(dir==2){
        if(DetStatusLogic(0)){                                                        //if either ch is on in det mode
          if((Current==OUTPUT)&&(_Det==1)&&(GenAdjustMode==0))                        //freq range shift
            SyncSweep(1); else SyncSweep(2);                                          //non coupling shift call
          OldTbase=-1;                                                                //assume changing sweep parameters means using function
        }                                                                             //  in such case, stay in same TB when leaving    
      }else if(dir==3){                                                               //for FrameMode changes
        if(DetStatusLogic(0))SyncSweep(1);
      }else if(dir<6){                                                                //chs turned on with source selector
        if((((Det&1)&&(Current==0))||((Det&2)&&(Current==1)))&&(!DetStatusLogic(1))){ //ch called has det on and both chs are not on
          if(dir==4){ 
            OldTbase=_T_base; SyncSweep(1);
          }else{
            if(OldTbase>-1){
              _T_base=OldTbase;
              Update_Base();
              Title[T_BASE][BASE].Flag |= UPDAT; 
            }
          }
        }
      }

  }                                                                                   //return to previous TB assuming selection was error 
}

void SyncSweep(u8 track){
     Delayms(20);
     if(track==1){
       if(_Frqn<17){                                                                  //allow 16 > -5 =11 as will be reduced to 9 below  
         if(_Frqn>4)SweepIndex=_Frqn-5;
           else SweepIndex=0; 
         if(_Frqn>9)SweepIndex-=2;
       }else SweepIndex=9; 
     } 
     _T_base=SweepIndex+3;                                                                   //use best timebase to fit sweep
     if(_Frqn>9)_T_base++;                                                                   //higher frequencies have shorter sweeps
     if(FlagFrameMode==0){
       if(track==1){if(SweepIndex>1)SweepIndex-=2;else SweepIndex=0;}
       if(track==2)_T_base+=2;
     }
     Update_Base();
     TIM_2IRQControl();
     Title[T_BASE][BASE].Flag |= UPDAT; 
     Delayms(20);
}

u8 DetStatusLogic(u8 logic){          //test if at least 1 channel is on (0) or both (1) with detector mode
  if(logic==0)return(((Title[TRACK1][COUPLE].Value==2)&&(_1_source))||((Title[TRACK2][COUPLE].Value==2)&&(_2_source)));
  if(logic==1)return(((Title[TRACK1][COUPLE].Value==2)&&(_1_source))&&((Title[TRACK2][COUPLE].Value==2)&&(_2_source)));
  return 0;
}

u8 ChDetLogic(u8 ch,u8 val){          //tests if called channel is about to be changed to detector mode (0,1) or already on (2)
  if(ch==0)return((Title[ch][COUPLE].Value==val)&&(_1_source));
  if(ch==1)return((Title[ch][COUPLE].Value==val)&&(_2_source));
  return 0;
}

u8 ChartLogic(void){
  return  ((_Mode==SCAN)&&(SubIndex>0)&&(_T_base==0));
}

u8 ListLogic(void){
  return ((_4_source==10)||(_4_source==11));     
}

u8 EditListActive(u8 mode){
  return((Current==FILE)&&(_Det==DIR)&&(Title[9][0].Value==mode)); //mode 0=edit, 1=file list
}

void UpdateEditBox(u8 mode){
                                                                   //     left toggle      right toggle >      right toggle <  
   Print_Str(                                                      // modes  0=PRN,        1= PRN >> INV,      3=  INV << PRN   
     Title[FILE][DIR].XPOS+(CharIndex*8), Title[FILE][DIR].YPOS,
     Title[FILE][DIR].Color[0],                                    // Color fixed 
     mode&1,                                                       // bit 0 sets INV for highliting < > with right toggle
     EditChar[CharIndex]                                           // String fixed < HILIGHTED CHAR 
   );

   if((mode==1)&&(CharIndex>0)){
     Print_Str(                                                      
       Title[FILE][DIR].XPOS+((CharIndex-1)*8), Title[FILE][DIR].YPOS,
       Title[FILE][DIR].Color[0],                                    
       PRN, 
       EditChar[CharIndex-1]                       
     );
   }    

   if((mode==3)&&(CharIndex<7)){
     Print_Str(         
       Title[FILE][DIR].XPOS+((CharIndex+1)*8), Title[FILE][DIR].YPOS,
       Title[FILE][DIR].Color[0],     
       PRN, 
       EditChar[CharIndex+1]        
     );
   } 
}

u16 KpCompensation(void){
  switch (_Kp1){		        //add 1 or 2 digits of resolution to interpolation factors
  case 983:
    return 9830;
  case 819:
    return 8192;
  case 614:
    return 6144;
  case 492:
    return 4915;			//actual correct value here is 49152, compensated for in TimeScaleCorrection
  case 246:
    return 24576;
  default:
    return 1024;
  }
}

void CalculateTvernier(u8 mode){                                 //MODES: 0=delta T, 1=cursor display, 2=CSV file sample position 
  s32  k=0, Min=0, Hr=0, n, m;
  s8 shift=0,i=0;
  u16 BaseRatio=TbaseSub[SubIndex-1].Ratio;
  s16 LocalStartOffset=150;
  u8 DminBypass=0;
  s16 End=0,IpComp=0,Xposition=_X_posi.Value;

              Neg=0;
              m = 1;  n = 1;
              k = _T_Range;                                      //structure index to time bases

              if(mode==0){
                if(k < 9)  m = Power(10, (11-k)/3);              //in chart mode, all time bases are k = 0, m = 1000
                else       n = Power(10, (k- 9)/3);
                k = X_Attr[(k%3)+9].SCALE *(_T2 - _T1);
                if(_T_Range<3)m=1;
                if((_T_Range%3)==0)k+=((k+25000)/49999);       
              }else{                 
                MinBypass=0;                                     //mode 0= delta T service, 1= Xpos + T1
                if(k < 6)  shift= -((8-k)/3);                    //need 10x more precision for 5 digit time display, so use TB's 6-8 scale      
                else       shift= (k-6)/3;

                if(CursorDisplaySelect&1)LocalStartOffset=0;        
                if(mode==2){
                  if(OSBufferLogic())End=(_X_posi.Value-StartOffset);else End=0;   
                  if((CursorDisplaySelect&1)==0){
                    if(_T_Range>16)LocalStartOffset=150-(discard+2);
                    else LocalStartOffset=150-(discard+1);
                  }
                  Xposition=CSVposition;
                }else{
                  if(CursorDisplaySelect<2)End=_T1;
                    else if(CursorDisplaySelect<4)End=_T2;
                }

                if(mode==1){
                  switch(_Kp1){                                    //Kp1 interpolation truncation correction factors        
                    case 246:                                                                          
                      k=((((416667*Xposition)+50000)/100000)-LocalStartOffset)+End;
                      if(Xposition<1500){
                        k*=333333;        
                      }else{                                       //scale back so large IP factor doesn't overload at large xpos values 
                        k*=33333;        
                        k+=((k+50000)/99999);                      //10ppm compensation + rounding for 33333 scaling
                        shift=4;
                      } 
                      break;
                    case 1024:
                      k=X_Attr[(k%3)+6].SCALE*((Xposition-LocalStartOffset)+End);                   //non IP ranges
                      break;
                    case 983:                                      //2uS/div
                    case 492:                                      //.2uS/div
                      IpComp=4710;                                 //46ppm compensation for 2 IP ranges (41 IP correction + 5 truncation)
                    default:                                       //rest of IP ranges
                      k=X_Attr[(k%3)+6].SCALE*(((((((102400000+IpComp)/KpCompensation())*Xposition)+5000)/10000)-LocalStartOffset)+End);
                  }
                }else if(mode==2){

                  switch(_Kp1){                                   
                    case 246:                                      //.1uS/div                                                                          
                      if(Xposition<1100){
                        k=1388888;        
                      }else{                                       
                        k=138888;
                        shift=4;
                      } 
                      break;
                    case 492:                                      //.2uS/div
                      k=138888;
                      break;
                    case 614:                                      //.5uS/div
                      k=277778; 
                      break;
                    case 819:                                      //1uS/div
                      k=416666; 
                      break;
                    case 983:                                      //2uS/div
                      k=69447;
                      break;
                    case 1024:
                      k=X_Attr[(k%3)+6].SCALE;                     //non IP ranges
                  }
                  k*=(Xposition-(LocalStartOffset-End));
                } 

              }

              if(ChartLogic()){
                k+=((k+500000)/999999);                                 //1ppm compensation + rounding for 333333 scaling
                if(SubIndex>4){
                  if(mode>0){
                    if(k>(599999999/BaseRatio)){                        //greater than 60 seconds 
                      Min=k/60;                                         //convert to minutes                         
                      k=k%(600000000/BaseRatio);
                      MinBypass=1;                                  
                    }
                    if(Min>(599999999/BaseRatio)){      
                      Hr=Min/60;
                      Min=(Min%(600000000/BaseRatio));  
                      k=(k%(600000000/BaseRatio));
                      MinBypass=2;  
                    } 
                    Min*=BaseRatio;
                    if(Min<9900000)Min=0; 
                    Hr*=BaseRatio;
                  }else if(k>((119999999/BaseRatio)-33333)){             //greater than 119 seconds
                    k/=60;                                               //convert to minutes                         
                    DminBypass=1;
                  }  
                  k*=BaseRatio;
                }else if(SubIndex<4){
                  k/=BaseRatio;  
                }
              }

              if((mode>0)&&(k<0)){k=-k;Neg=1;}
              if(k>0){
                if(k <=  0x7FFFFFFF/m)  k = (m * k)/n;      
                else                    k = 0x80000000;     
              }else                     k = 0x80000000;

            if(mode>0){                                                 //Cursor time since start display  
              if((MinBypass)&&(k==0x80000000))k=0;
              if(MinBypass==1){                 
                Int2Str(CursorDisplayStrM, Min, MM_UNIT, 4, UNSIGN,shift);
                ProcessCursorDisplayStr(CursorDisplayStrM);
                Int2Str(CursorDisplayStr, k, DD_UNIT, 5, UNSIGN,shift);
                if(CursorDisplayStr[6]=='n'){CursorDisplayStr[6]=' ';CursorDisplayStr[7]='S';}
              }else if(MinBypass==2){
                Int2Str(CursorDisplayStrH, Hr, H_UNIT, 4, UNSIGN,shift-3);
                ProcessCursorDisplayStr(CursorDisplayStrH);
                Int2Str(CursorDisplayStrM, Min, MM_UNIT, 4, UNSIGN,shift);
                ProcessCursorDisplayStr(CursorDisplayStrM);
                Int2Str(CursorDisplayStr, k, DD_UNIT, 5, UNSIGN,shift);
                if(CursorDisplayStr[6]=='n'){CursorDisplayStr[6]=' ';CursorDisplayStr[7]='S';}
              }else{
                Int2Str(CursorDisplayStr, k, DD_UNIT, 5, UNSIGN,shift);
                if(k==0x80000000){
                  for(i=0;i<8;i++)CursorDisplayStr[i]=' '; 
                  if((Xposition==(LocalStartOffset-End))||(XposRef==(LocalStartOffset-End))){
                    CursorDisplayStr[2]='0';
                    CursorDisplayStr[5]=0;
                  }
                }else if(Neg){
                  for(i=9;i>0;i--)CursorDisplayStr[i]=CursorDisplayStr[i-1]; 
                  CursorDisplayStr[0]='-';
                }
              }

            }else{             
              if(Title[T_VERNIE][2].Value==0){                                  //DELTA T DISPLAY       
                if(DminBypass){
                  Int2Str(TNumStr, k, M_UNIT, 3, SIGN,0);
                }else{
                  Int2Str(TNumStr, k, D_UNIT, 3, SIGN,0);
                  if((_T_Range==3)&&(TNumStr[2]==' ')){
                    TNumStr[2]='.';
                    TNumStr[5]='S';
                    TNumStr[6]=' ';
                  }
                }
              }else{                                                            //FREQUENCY DISPLAY
                n=0;               
                while(k>100000){k/=10;n++;}
                k=((1000000000+(k/2))/k);
                if(MinBypass)k = 0x80000000;                                    //do not display frequency of less than 0.0083 Hz
                if(_T_Range<3)Int2Str(TNumStr, k, F_UNITSUB, 4, UNSIGN,n);
                  else Int2Str(TNumStr, k, F_UNIT, 4, UNSIGN,n);
              }
            }


}

void ProcessCursorDisplayStr(char* TmpStr){
                if(TmpStr[1]=='.'){
                  TmpStr[1]=TmpStr[0];
                  TmpStr[0]=' ';
                }
                TmpStr[2]=TmpStr[5];
                if(TmpStr[2]=='u')TmpStr[2]='m';
                TmpStr[3]=' ';
                TmpStr[4]=0;
}

void CalculateVvernier(u8 mode){
s32 k=0;

            if(mode>0)MinBypass=0;
            if(mode==0){                                                         //delta V calculations
              k=(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale;				
              if  (Title[_Meas_V_Track][SOURCE].Value>2)k*=10; 
              Int2Str(VNumStr,k, V_UNIT, 3, SIGN,0);
              return;
            }else if(mode==1){                                                   //V1 and V2 cursor display calculations
              if(_Det==0)k=(_V1_Vernie-Title[_Meas_V_Track][POSI].Value)*_Meas_V_Scale;				
                else k=(_V2_Vernie-Title[_Meas_V_Track][POSI].Value)*_Meas_V_Scale;
              if  (Title[_Meas_V_Track][SOURCE].Value>2)k*=10; 
            }else{                                                               //trigger display calculations
              if(_Tr_source==0)k=(_Vt1-_1_posi)*Y_Attr[Title[0][2].Value].SCALE;
                else if((_Tr_source==2)||(_Tr_source==3))k=700*Y_Attr[0].SCALE;  //default of approx 1.40V trig for V2.6 FPGA digital inputs
                  else k=(_Vt2-_2_posi)*Y_Attr[Title[1][2].Value].SCALE;
              if(((_Tr_source<2)||(_Tr_source==4))&&(Title[_Tr_source][SOURCE].Value>2))k*=10;
            }
            Int2Str(CursorDisplayStr,k, VV_UNIT, 3, SIGN,0);
}

void AutoSet(void){
u8 static AmaxLimit=0,Asignal=0,Bsignal=0,BmaxLimit=0,Abase=0,Bbase=0,AdjustFlag=0,RecurseLimit=75,AutoTrigStatus=0;
s16 static AminLimit=0,BminLimit=0,AtLevel=100,BtLevel=100;
u8 i;

   if(AutoSetFlag==1){                                              //initiate
     save_parameter();
     Title[7][1].Value = 3;                                         // >Vt trig type
     OldCurDefTime=CurDefTime;
     if (FlagFrameMode==0){     
       FlagFrameMode=1;
       if (_Mode!=SCAN)OldPosi=GetXpos(XposRef);
     }
     if(FlagMeter){MeterStatus=FlagMeter;FlagMeter=0;DisableMeter();}
     else MeterStatus=0;
     OSBuffer=1;  OSAvg=0;                                          //use 8x oversampling, minimizes aliasing
     InitiateOSBuffers();
     OsBufferLogicFlag=OSBufferLogic();
     ShowFFT=0;
     App_init(0);
     AutoSetFlag=2; AutoSequence=0;
     CurDefTime=1;                                                  //narrow measuring window to clear signal settling from Ypos changes
     Title[0][SOURCE].Value = 1; Title[1][SOURCE].Value = 1;        
     Title[2][SOURCE].Value = 0; Title[3][SOURCE].Value = 0;        
     if(Title[0][1].Value > 1)Title[0][1].Value = 0;                //restrict coupling mode to AC or DC             
     if(Title[1][1].Value > 1)Title[1][1].Value = 0;
     Title[7][0].Value = 0;                         
     _Mode=SCAN;  _T_base=11;                                       //use scan/auto modes to ensure waveform aquisition
     _A_Range=0;  _B_Range=0;
     Asignal=0;   Bsignal=0;
     AdjustFlag=0;
     _1_posi=100; _2_posi=100;                                       
     AutoTrigStatus=TrgAuto;                                        
     if(TrgAuto==0)TrgAuto=2;                                       //use auto trig
     Update_Base();
     Update_Range();
     Update_Trig(1);
     Process();				
     StartUart=0;  UartFileSize=0;
     Update_View_Area(); 
     XposRef=120;_X_posi.Value=GetXpos(XposRef);                   //expands single window buffer
     AutoSetTimer=5; 
    return;
   }

   if((FrameEndFlag)&&(AutoSetTimer==0)&&(AutoSetFlag==2)){        
     if((a_Max>110)||(a_Min<90))Asignal=1;                         //treshold at +/- 20mV to reject noise and baseline DC offset
     if((b_Max>110)||(b_Min<90))Bsignal=1;
     if((Asignal==0)&&(Bsignal==0))return;                         //if no signal, loop back from here   
     else AutoSetFlag=3;
     _T_base=21;
     Update_Base();
     return;
   }

   if(AutoSetFlag==3){                                             //set up according to which channels were found with signals 
     AutoSetFlag=4;
     RecurseLimit=75;
     if((Asignal)&&(Bsignal)){
       Title[0][SOURCE].Value = 1; Title[1][SOURCE].Value = 1;        
       Title[7][0].Value = 0;                                 
       _1_posi=50;_2_posi=150;                                       
       AmaxLimit=100;
       AminLimit=5;
       BmaxLimit=195;
       BminLimit=100;
       RecurseLimit=38;
     }else if(Asignal){
       Title[0][SOURCE].Value = 1; Title[1][SOURCE].Value = 0;        
       Title[7][0].Value = 0;                                      //trigger source
       _1_posi=100;                                       
       AmaxLimit=190;
       AminLimit=10;
     }else if(Bsignal){
       Title[0][SOURCE].Value = 0; Title[1][SOURCE].Value = 1;        
       Title[7][0].Value = 1;
       _2_posi=100;                                       
       BmaxLimit=190;
       BminLimit=10;
     }
     Update_Trig(1);
     AutoSetTimer=10;
     return;
   }

   Update_Trig(1);

   if((__Get(FIFO_START)==0)&&(_T_base<11)&&(AutoSetTimer==0)     //DC only signal detected  
   &&(FrameEndFlag==0)){

     if(AutoSetFlag<50){                                          //attempt to trigger on any AC riding on DC with auto trigger
       AutoSetTimer=5;                                           
       AutoSetFlag++;                                            
       return;       
     }
     Abase=7;                                                     //if can't trigger on AC component set timebase to 5mS/div: useful  
     Bbase=7;                                                     //to display powerline ripple, etc, and exit
     goto Untriggered;

   }else if((FrameEndFlag)&&(AutoSetTimer==0)){                   //if triggered (W/Tb<11) or mode=SCAN (scan is always triggered)                       
     AutoSetTimer=5;                                              //allows enough time for signal to settle after changing Ypos and V/div ranges

     if(AutoSequence==0){                                       

       if(!((Asignal)&&(Bsignal))){                               //adjust Y positions to nearest screen major division
         if((((Asignal)&&(a_Min!=a_Max))||((Bsignal)&&(b_Min!=b_Max)))||(_T_base<15)){   
           _1_posi=100+(_1_posi-(((((a_Min+a_Max)/2)+12)/25)*25));                       
           _2_posi=100+(_2_posi-(((((b_Min+b_Max)/2)+12)/25)*25));                       
           if(_1_posi>175)_1_posi=175;else if(_1_posi<25)_1_posi=25;          
           if(_2_posi>175)_2_posi=175;else if(_2_posi<25)_2_posi=25;          
         }
       }

       if((Asignal)&&(_A_Range<7)&&((a_Max>AmaxLimit)||(a_Min<AminLimit))){     //adjust V ranges
          _A_Range++;           
          Title[0][2].Flag |= UPDAT;
       }else if(AdjustFlag==1)AdjustFlag=2;

       if((Bsignal)&&(_B_Range<7)&&((b_Max>BmaxLimit)||(b_Min<BminLimit))){ 
          _B_Range++;           
          Title[1][2].Flag |= UPDAT;
       }else if(AdjustFlag==2)AdjustFlag=3;

       Update_Range();
                                                                  //Scan through timebases starting with fastest       
       //if(((TaN<3)||(Asignal==0))&&((TbN<3)||(Bsignal==0))&&(_T_base>4)){   //changes timebase to slower if less than 3 waves in "window"        
       if(((TaN<2)||(Asignal==0))&&((TbN<2)||(Bsignal==0))&&(_T_base>4)){     //changes timebase to slower if less than 2 waves in "window"      
         _T_base--;                                               //make sure full extent of + and - peaks are included.
         if(_T_base<11)_Mode=AUTO;
         Update_Base();                                       
         Title[6][1].Flag |= UPDAT;
       }else{                                                     //with V range set, move out of block.                                 
         if(AdjustFlag==3){    
           AutoSequence++;
           return;
         }
                                                                  //readjust v ranges with proper timebase by increasing gain + re-testing  
         if(  (((TaN>0)&&(Asignal))||((TbN>0)&&(Bsignal)) || (_T_base==4))         
         &&(AdjustFlag==0)  ){                                     
           if(((a_Max-_1_posi)<RecurseLimit)&&((_1_posi-a_Min)<RecurseLimit)){
             if(_A_Range>0)_A_Range--;
           }
           if(((b_Max-_2_posi)<RecurseLimit)&&((_2_posi-b_Min)<RecurseLimit)){     
             if(_B_Range>0)_B_Range--;
           }
           Update_Range();
           AdjustFlag=1;
           return;
         }
       }

     }

     if((Asignal)&&(AutoSequence==1)){                               //adjust timebase using A channel
       AtLevel=_Vt1;                                                 //aquire chA trig level
       Title[7][0].Value = 0;                                        //trigger source
       AutoSequence=2;
       if((_T_base%3)==1){
         if(TaN>3)Abase=_T_base+1;else Abase=_T_base;                //5-2 shift
       }else{
         if(TaN>2)Abase=_T_base+1;else Abase=_T_base;                //2-1 shift
       } 
       if(Abase>21)Abase=21; 
       if(Bsignal){
         Title[7][0].Value = 1;                                      //if B channel also active loop around to aquire trigger level
         Update_Trig(1);
         return;
       }  
     }else if((Asignal==0)&&(AutoSequence==1)){
       AutoSequence=2;
     }

     if((Bsignal)&&(AutoSequence==2)){
       BtLevel=_Vt2;                                                 //aquire chB trig level
       AutoSequence=3;
       if((_T_base%3)==1){                                           //adjust timebase using B channel
         if(TbN>3)Bbase=_T_base+1;else Bbase=_T_base;              
       }else{
         if(TbN>2)Bbase=_T_base+1;else Bbase=_T_base;  
       }
       if(Bbase>21)Bbase=21; 
     }else if((Bsignal==0)&&(AutoSequence==2)){
       AutoSequence=3;
       Bbase=Abase;
     }

       if(AutoSequence==3){                                           //exit out of function   
Untriggered:
         CurDefTime=OldCurDefTime;                                    //return "borrowed" Cursor defined meter function setting
         AutoSequence=0;
         Title[2][SOURCE].Value=TempPar[9];                           //restore digital channels 
         Title[3][SOURCE].Value=TempPar[13];                          //and chD special functions
         Title[6][0].Value=TempPar[17];                               //restore timebase mode
         if(_Mode==SCAN)_Mode=AUTO;                                   //only allow triggered modes to persist
         OSBuffer=TempPar[51];                                        //restore buffer mode  
         FlagFrameMode=TempPar[52];  
         OSAvg=TempPar[53];  
         _Vt1=AtLevel;                                                //set aquired trigger levels
         _Vt2=BtLevel;

         if((Asignal)&&(Bsignal)){
           Title[0][SOURCE].Value = 1; Title[1][SOURCE].Value = 1;    //engage A&B trig mode if this was on at start and both ch's have signal
           if(TempPar[56]==4){
             Title[7][0].Value=4;
             if(OldMode==SCAN)OldMode=AUTO;                           //avoid untriggered mode after leaving function
             _Mode=NORH;
             Update_Trig(0);
           }else{
             Title[7][0].Value=0;
             Update_Trig(1);                                            
           }    
           _1_posi=50;_2_posi=150;                                       
           _T_base=(Abase+Bbase)/2;
           UpdateMeter=4;                                             //set meter page to A&B
         }else{                                                
           if(Asignal){
             Title[0][SOURCE].Value = 1;
             Title[1][SOURCE].Value = 0;
             Title[7][0].Value = 0;  
             _T_base=Abase;
             UpdateMeter=0;                                           //meter page = A
           } 
           if(Bsignal){
             Title[1][SOURCE].Value = 1;
             Title[0][SOURCE].Value = 0;
             Title[7][0].Value = 1;  
             _T_base=Bbase;
             UpdateMeter=1;                                           //meter page = B                                           
           }
           Update_Trig(1);                                            
         }                           
         AutoSetFlag=0;                                               //disable function 
         TempPar[0]=0;                                                //discard saved parameters
         Current=6;Detail[6]=1;                                       //forces menu to timebase adjust position
         for(i=0;i<4;i++)Title[i][0].Flag |= UPDAT;
         Title[6][0].Flag |= UPDAT;
         Title[7][0].Flag |= UPDAT;
         UpdateMeterPage(); 
         TrgAuto=AutoTrigStatus;
         Update_Base();
         Update_Range();
         if(MeterStatus){FlagMeter=MeterStatus;EnableMeter();}
         ShowFFT=0;
         App_init(0);                                                 //updates settings without clearing screen
         XposRef=120;                                                 //sets Xpos to 1 major division from left edge of screen
         _X_posi.Value=GetXpos(XposRef);
       }

   }

}


/******************************** END OF FILE *********************************/

