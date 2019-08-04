/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP Menu.c                                               Author : bure
*******************************************************************************/
#include "Sys.h"
#include "Menu.h"

uc8 MENU_ITEM[][11] = {"Save Param", "Save Dat  ", "Save Buf  ", "Save Bmp  ",
                       "Save Csv  ", "Load Dat  ", "Load Buf  ", "BackLight ", 
                       "Buzzer    ", "Standby   ", "Calibrat  " };

uc8  CMD_SEL[][9]   = {"SaveParam", "SaveDat00", "SaveBuf00", "SaveBmp00", 
                       "SaveCsv00", "LoadDat00", "LoadBuf00", "BkLight00", 
                       "BuzzVol00", "SleepDsbl", "Calibrate",};
                    
uc8  LOADSEL[][9]   = {"LoadDat", "LoadBuf"};
uc8  BKLIGHT[][9]   = {"BkLight"};
uc8  BUZZVOL[][9]   = {"BuzzVol"};

uc8 TK[] = {1, 5, 2, 1, 5, 2, 1, 5, 2, 1, 5, 2, 1, 5, 2, 1, 5, 2, 1, 5, 2, 1};
uc8 TM[] = {9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2};
uc8 TN[] = {0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7};

//=================+=========+=========+=========+=========+=========+=========+
uc8  T_BASE[][8] = { " 1S  ",  "500mS",  "200mS",  "100mS",  "50mS ",  "20mS ",
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
//-----------------+---------+---------+---------+---------+---------+---------+
uc16 BASE_KP1[]  = {    1024,     1024,     1024,     1024,     1024,     1024, 
                        1024,     1024,     1024,     1024,     1024,     1024,
                        1024,     1022,     1018,     1004,     1004,      975,
                         853,      853,      341,      341,                   };
//=================+=========+=========+=========+=========+=========+=========+
uc8  FRQSTR[][8] = {" 10Hz ", " 20Hz ", " 50Hz ", " 100Hz", " 200Hz", " 500Hz",
                    " 1KHz ", " 2KHz ", " 5KHz ", " 10KHz", " 20KHz", " 50KHz",
                    "0.1MHz", "0.2MHz", "0.5MHz", " 1MHz ", " 2MHz ", " 4MHz ",
                    " 6MHz ", " 8MHz "};  // Output Frequency
//-----------------+---------+---------+---------+---------+---------+---------+
uc8  DGTL_PSC[]  = {     179,      179,      179,      179,      179,      179,
                         179,      179,       17,       17,       17,        0,
                           0,        0,        0,        0,        0,        0,
                           0,        0};
//-----------------+---------+---------+---------+---------+---------+---------+
uc16 DGTL_ARR[]  = {   39999,    19999,     7999,     3999,     1999,      799,
                         399,      199,      799,      399,      199,     1439,
                         719,      359,      143,       71,       35,       17,
                          11,        8};
//-----------------+---------+---------+---------+---------+---------+---------+
uc8  ANLG_PSC[]  = {    18-1,     18-1,     18-1,     18-1,     18-1,     18-1,
                        18-1,     18-1,     18-1,     18-1,     18-1,};
//-----------------+---------+---------+---------+---------+---------+---------+
uc16 ANLG_ARR[]  = {  4000-1,   2000-1,    800-1,    400-1,    200-1,     80-1,
                        40-1,     20-1,      8-1,      4-1,     4-1,};
//=================+=========+=========+=========+=========+=========+=========+

//=======+======+======+======+======+======+======+======+======+======+======+
uc8 Y_STR[][6] = 
//-------+------+------+------+------+------+------+------+------+------+------+
         {"10mV","20mV","50mV","0.1V","0.2V","0.5V"," 1V "," 2V "," 5V ","!10V!"};
//-------+------+------+------+------+------+------+------+------+------+------+
uc32 Y_SCALE[] = 
//-------+------+------+------+------+------+------+------+------+------+------+
         {   400,   800,  2000,  4000,  8000, 20000, 40000, 80000,200000,400000};
//=======+======+======+======+======+======+======+======+======+======+======+

uc8  CURR_ST[][8] = {"HOLD", "!RUN!"};                        // Current Running State
uc8  BTRY_ST[][8] = {"~``'", "~``}", "~`;}", "~;;}", "{;;}"}; // Battery Status
uc8  COUPLER[][8] = {" -- ", " DC ", " AC "}; 
uc8  V_RANGE[][8] = {"10mV","20mV","50mV","0.1V","0.2V",
                     "0.5V"," 1V "," 2V "," 5V ","10V ",};
uc8  TRACK_3[][8] = {"! -- !", "CH(C)", "(A+B)", "(A-B)",
                     "(C&D)",  "(C|D)", "INV A", "INV B"};    // Track 3 source
uc8  TRACK_4[][8] = {"! -- !", "CH(D)", "REC_A", "REC_B",
                     "REC_C", "REC_D"};                       // Track 4 source
uc8  SYNCTYP[][8] = {"!AUTO!", "!NORM!", "SINGL", "!SLOW!"};  // Sync Mode
uc8  OUT_TYP[][8] = {"Sin\"\"\"", "Tri###", "Saw$$$", "Sqr,,,"}; // Signal Output Type
uc8  TRIGTYP[][8] = {"! \\ !", "! ^ !", "!<Vt!", "!>Vt!", 
                     "!<TL!", "!>TL!",  "!<TH!", "!>TH!"};    // Trigger Type Str
uc8  TXMETER[][8] = {"TwH :", "TwL :", "Cycl:", "Frqn:", 
                     "Duty:", " [T :"};
uc8  VXMETER[][8] = {"Vmax:", "Vmin:", "Vp-p:", "Vrms:", 
                     "Vavg:", " [V :", "Vtrg:", "Vbat:"}; 
uc8  XNP_MAX[4]   = {3, 21, 55, 123};                         // XNP Limits
uc8  CALIBRA[5]   = "CAL";                                    // Offset Calibration
uc8  Xn_POSN[5]   = "Xpos";                                   // X VIEW WINDOW POS
uc8  Yn_POSN[5]   = "!Yp!";                                   // Track n position
uc8  VTP_STR[5]   = "Vtrg";                                   // Trigg Voltage Str
uc8  V1P_STR[5]   = "!V1!";                                   // V1 Vernie Str
uc8  V2P_STR[5]   = "!V2!";                                   // V2 Vernie Str
uc8  T0P_STR[5]   = "!T0!";                                   // Presampling Depth
uc8  T1P_STR[5]   = "!T1!";                                   // T1 Vernie Str
uc8  T2P_STR[5]   = "!T2!";                                   // T2 Vernie Str
uc8  V_UNIT[][5]  = {"uV", "mV", "V ", "--"};
uc8  T_UNIT[][5]  = {"!nS!", "!uS!", "!mS!", " S ", "---"};
uc8  F_UNIT[][5]  = {"!Hz!", "KHz",  "MHz", "---"};
uc8  D_UNIT[][5]  = {" % ", " % "," % ","---"};
uc8  FSTR[][8]    = {"SaveWav", "LoadWav", "SaveBuf", "SaveCsv"};
uc8  VOLUME[]     =  "Volume";
uc8  BKLGHT[]     =  "LcdBkl";
uc8  STANDBY[]    =  "PwrDnT ";
uc8  TIP1[][11]   = {" Press K3 ", "          ","          "};
uc8  TIP2[][11]   = {"to Confirm", "Completed ","  Error   "};

u8   PopUpdt = 0;

//......................... 以下为可以保存与读出的工作参数 .....................

//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
// 偏移误差校正    | 10mv| 20mv| 50mv| 0.1v| 0.2v| 0.5v|  1v |  2v |  5v | 10v |
//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
u16 Diff[][10] =  {{ 436,  407,  390,  386,  383,  411,  395,  387,  384,  386},
                   { 421,  406,  396,  392,  391,  419,  404,  396,  393,  394}};
//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
// 增益误差校正    | 10mv| 20mv| 50mv| 0.1v| 0.2v| 0.5v|  1v |  2v |  5v | 10v |
//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
u16 Gain[][10] =  {{1143, 1126, 1092, 1088, 1041, 1126, 1109, 1104, 1098, 1056},
                   {1143, 1126, 1109, 1088, 1041, 1126, 1109, 1109, 1136, 1040}};
//=================+=====+=====+=====+=====+=====+=====+=====+=====+=====+=====+
u16 Slope[2]   =  { 2157, 2157,}; // 斜率校正系数

u16 Vt[4] = {160, 80, 51, 21};
u16 Yn[4] = {150, 75, 50, 25};
u16 Item  = VM1;                  // Limit 0 ~ VM1
u16 Line  = 0;                    // Limit 0 ~ 10  弹出窗内当前命令行

uimenu Pop[]   = {//........ Val, Src, Flag 为可以保存与读出的工作参数
//==============+====+====+====+====+=====+======+===================+
//|     pStr    | x0 | y0 | >= | <= | Val |  Src |       Flag        |
//==============+====+====+====+====+=====+======+===================+
  {&FSTR[0][0],    5, 127,   0,  999,   0, CNUM1,      FNUM+LOOP+UPDT},// [SWAV]
  {&FSTR[1][0],    5, 112,   0,  999,   0, CNUM1,      FNUM+LOOP+UPDT},// [LWAV]
  {&FSTR[2][0],    5,  97,   0,  999,   0, CNUM1,      FNUM+LOOP+UPDT},// [SBUF]
  {&FSTR[3][0],    5,  82,   0,  999,   0, CNUM1,      FNUM+LOOP+UPDT},// [SCSV]
  {&VOLUME[0],     5,  67,   0,  100,  50, CNUM3,      PCNT+    +UPDT},// [SVOL]
  {&BKLGHT[0],     5,  53,  10,  100,  50, CNUM3,      PCNT+    +UPDT},// [SBKL]
  {&STANDBY[0],    5,  37,   0,   60,   2, CNUM3,      TCNT+    +UPDT},// [SPDT]
  {&TIP1[0][0],    5,  19,   0,    0,   0, CNUM2,      PCNT+    +UPDT},
  {&TIP2[0][0],    5,   4,   0,    0,   0, CNUM2,      PCNT+    +UPDT},
//==============+====+====+====+====+=====+======+===================+
};

uimenu Menu[] = {//......... Val, Src, Flag 为可以保存与读出的工作参数 
//==============+====+====+====+====+=====+======+===================+
//|     pStr    | x0 | y0 | >= | <= | Val |  Src |       Flag        |
//==============+====+====+====+====+=====+======+===================+
  {(u8*)COUPLER,  37, 228,   0,   2,    2, TRCK1, SLCT+LOOP+INVR+UPDT}, // [CPA]: Channel A Coupler
  {(u8*)V_RANGE,  37, 215,   0,   9,    6, TRCK1, SLCT+          UPDT}, // [RNA]: Channel A Range
  {(u8*)COUPLER,  73, 228,   0,   2,    2, TRCK2, SLCT+LOOP+INVR+UPDT}, // [CPB]: Channel B Coupler
  {(u8*)V_RANGE,  73, 215,   0,   9,    6, TRCK2, SLCT+          UPDT}, // [RNB]: Channel B Range
//--------------+----+----+----+----+-----+------+-------------------+
  {(u8*)TRACK_3, 109, 228,   0,   7,    1, TRCK3, SLCT+LOOP+INVR+UPDT}, // [T3S]: Track 3 Source
  {(u8*)TRACK_4, 153, 228,   0,   5,    1, TRCK4, SLCT+LOOP+INVR+UPDT}, // [T4S]: Track 4 source
  {(u8*)Xn_POSN, 197, 228,   0, 123,    0, XATTR,           INVR+UPDT}, // [XNP]: View Window ptr 0~136 Grid 
  {(u8*)VTP_STR, 233, 228,   5, 195, VOID, TRCK2,      LOOP+INVR+UPDT}, // [V_T]: CH A~B Trigger threshold
  {(u8*)TRIGTYP, 269, 228,   0,   7,    0, TRCK2, SLCT+LOOP+INVR+UPDT}, // [TRG]: CH A~D Trigger Type
//--------------+----+----+----+----+-----+------+-------------------+
  {(u8*)SYNCTYP, 305, 228,   0,   3,    0, XATTR, SLCT+LOOP+INVR+UPDT}, // [SYN]: Sync Mode
  {(u8*)T_BASE,  305, 215,   0,  21,   15, XATTR, SLCT+          UPDT}, // [TIM]: Time Base Range
  {(u8*)OUT_TYP, 349, 228,   0,   3,    0, FOUTC, SLCT+LOOP+INVR+UPDT}, // [OUT]: Output Wave Type 
  {(u8*)FRQSTR,  349, 215,   0,  19,    9, FOUTC, SLCT+          UPDT}, // [FRQ]: Output Frequency
//--------------+----+----+----+----+-----+------+-------------------+
  {(u8*)Yn_POSN, 376, 156,   5, 195, VOID, TRCK2,           INVR+UPDT}, // [YNP]: CH A~D Y Position
  {(u8*)V1P_STR, 376, 140,   2, 198,  175, TXT1C,                UPDT}, // [V_1]: V1 Cursor
  {(u8*)V2P_STR, 376, 124,   2, 198,  150, TXT1C,                UPDT}, // [V_2]: V2 Cursor
  {(u8*)T0P_STR, 376, 108,   1, 132,    6, TRCK2,           INVR+UPDT}, // [T_0]: Presampling ptr 0~136 Grid 
  {(u8*)T1P_STR, 376,  92,   2, 358,   90, TXT1C,                UPDT}, // [T_1]: T1 Cursor
  {(u8*)T2P_STR, 376,  76,   2, 358,  120, TXT1C,                UPDT}, // [T_2]: T2 Cursor
  {(u8*)CALIBRA, 376,  60, 350, 480,  400, TRCK1,           INVR+UPDT}, // [CAL]: CH A~B Offset Calibration 
//--------------+----+----+----+----+-----+------+-------------------+
  {(u8*)TXMETER, 300,   0,   0,   5,    0, TRCK2, SLCT+LOOP+INVR+UPDT}, // [TM2]: Measure F & T
  {(u8*)TXMETER, 200,   0,   0,   5,    3, TRCK3, SLCT+LOOP+INVR+UPDT}, // [TM1]: Measure F & T
  {(u8*)VXMETER, 100,   0,   0,   7,    0, TRCK2, SLCT+LOOP+INVR+UPDT}, // [VM2]: Measure V
  {(u8*)VXMETER,   0,   0,   0,   7,    0, TRCK1, SLCT+LOOP+INVR+UPDT}, // [VM1]: Measure V
//--------------+----+----+----+----+-----+------+-------------------+
  {(u8*)CURR_ST,   1, 228,   0,   1,    1, N_2_W, SLCT+LOOP+INVR+UPDT}, // [RUN]: Running state
  {(u8*)BTRY_ST,   1, 215,   0,   4,    4, N_2_W, SLCT+          UPDT}, // [BTY]: Battery status
//==============+====+====+====+====+=====+======+===================+
};
/*******************************************************************************
  更新主界面菜单显示
*******************************************************************************/
void UpdtMainMenu(void)
{ 
  I32STR_RES Num;
  static u8 Last;
  u32 Ts = Menu[TIM].Val;                              // Curr T_base Scale
  u64 Fx = Power(10, TN[Ts]);
  u64 Tx = Power(10, TM[Ts]);
  for(u32 i = 0; i < ITEM_END; i++){
    u32 Flag = Menu[i].Flg;
    if(i == Item && Last != Flashing){
      Flag = (Flag^Flashing) | FLSH;                   // Curr Item Flashing
      Menu[VM1].Flg |= UPDT;                           // Update Vn Display
      Menu[TM1].Flg |= UPDT;                           // Update FTn Display
      Menu[VM2].Flg |= UPDT;                           // Update Vn Display
      Menu[TM2].Flg |= UPDT;                           // Update FTn Display
      Last = Flashing;
    }
    if(Flag & (UPDT | FLSH)){
      if(Flag & UPDT){
        CtrlUpdate(i);                                 // 更新硬件设置
        if(i == V_T || i == T_0 || i == XNP) Litimg(); // 更新缓冲区缩略图
      }
      UpdateTag();                                     // 更新信号标签
      Menu[i].Flg &= ~(UPDT | FLSH);                   // 清除更新标志
      RowPosi(Menu[i].X0, Menu[i].Y0);
      u32 Indx = Menu[i].Val;
      u32 Cnum = Menu[i].Src;
      if(Cnum == N_2_W) Cnum = Indx ? NORMO : WARNG;   // 自动转换警示色
      u32 ChAB = Cnum & 1, Dgtl = Cnum & 2;
      u32 Slct = (Flag & SLCT) ? 0x08 : 0x00;
      u32 Mode = (Flag & INVR) ? INVR : CHAR;
      u32 Vpp  = (ChAB) ? MaxB-MinB : MaxA-MinA;
      uc8* Ptr = 0;
      u8* pStr = (u8*)Menu[i].STR;
      u32 Void = 0;
      if(i == VM1 || i == VM2){
        s32 My  = Y_SCALE[Menu[ChAB ? RNB : RNA].Val];
        switch(Indx){
          case MAX: 
            My *= (ChAB ? MaxB : MaxA)-Yn[ChAB];
          break;
          case MIN: 
            My *= (ChAB ? MinB : MinA)-Yn[ChAB];
          break;
          case VPP: 
            My = (Vpp < 5) ? 0 : My*Vpp;               // 小于 5 点则置 0
          break;
          case RMS: 
            My *= ChAB ? RmsB : RmsA;
            if(My == 0) Void = 1;
          break;
          case AVG:
            My  *= FPGA_ByteRd(ChAB, VAVG)-Yn[ChAB];
          break;
          case D_V: 
            My *= Menu[V_1].Val-Menu[V_2].Val;
          break;
          case VTH:
            My *= Vt[ChAB]-Yn[ChAB];
          break;
          case B_V:
            Cnum = TXT1C;
            My  = Vsum/8*1000;
          break;
        }
        PrintStr(Cnum, Mode, pStr+Indx*Slct);          // 显示标提字符串
        if(Void == 0){
          Int32String_sign(&Num, My);                  // 带符号三位有效数字符串
          AddStr(Cnum, Mode, Num.str);                 // 显示测量数值
          Ptr = V_UNIT[Num.decPos];                    // 选择电压测量量纲
          AddStr(Cnum, Mode, (u8*)Ptr);                // 显示测量量纲
        } else AddStr(Cnum, Mode, " ----- "); 
      } else 
//==============================================================================        
        if(i == TM1 || i == TM2){
        u64 Mx  = (Vpp < 5 && !Dgtl) ? 0 : (Indx == FQRN) ? Fx : Tx;
        u32 TwL = MeasT[Menu[i].Src][PTWL];
        u32 TwH = MeasT[Menu[i].Src][PTWH];
        switch(Indx){
          case DUTY:
            Mx  = (!TwH || !TwL) ? 0 : (100*TwH/(TwH+TwL));
            Ptr = D_UNIT[0];                           // 选择显示百分比量纲
          break;
          case FQRN: 
            Mx  = (!TwH || !TwL) ? 0 : Mx*30/TK[Ts]/(TwH+TwL);
            Ptr = F_UNIT[0];                           // 选择显示频率量纲
            break;
          case CYCL: 
            Mx  = (!TwH || !TwL) ? 0 : Mx*TK[Ts]*(TwH+TwL)/30;
            Ptr = T_UNIT[0];                           // 选择显示时间量纲
          break;
          case PTWH:
            Mx  = Mx*TK[Ts]*TwH/30;
            Ptr = T_UNIT[0];                           // 选择显示时间量纲
          break;
          case PTWL:
            Mx  = Mx*TK[Ts]*TwL/30;
            Ptr = T_UNIT[0];                           // 选择显示时间量纲
          break;
          case D_T: 
            Mx  = Tx*TK[Ts]*(Menu[T_2].Val-Menu[T_1].Val)/30;
            Cnum = TXT1C;
            Ptr = T_UNIT[0];                           // 选择显示时间量纲
          break;
        }
        PrintStr(Cnum, Mode, pStr+Indx*Slct);          // 显示标提字符串
        if(Mx != 0){
          Int32String(&Num, Mx, 3);                    // 转无符号三位有效数字  
          Ptr += Num.decPos*5;                         // 选择时间测量量纲
          AddStr(Cnum, Mode, Num.str);                 // 显示测量数值
          AddStr(Cnum, Mode, (u8*)Ptr);                // 显示测量量纲
        } else AddStr(Cnum, Mode, " ----- "); 
      } 
//==============================================================================        
      else PrintStr(Cnum, Mode, pStr+Indx*Slct);     // 显示标提字符串
    }
  }
}
/*******************************************************************************
 
*******************************************************************************/
void PopValInc(u16 Line, u16 Unit)
{
  s32 Temp = Pop[Line].Val+Unit;
  if(Temp <= Pop[Line].Max)     Pop[Line].Val = Temp;
  else if(LOOP & Pop[Line].Flg) Pop[Line].Val = Pop[Line].Min;
}
/*******************************************************************************
 
*******************************************************************************/
void PopValDec(u16 Line, u16 Unit)
{
  s32 Temp = Pop[Line].Val-Unit;
  if(Temp >= Pop[Line].Min)     Pop[Line].Val = Temp;
  else if(LOOP & Pop[Line].Flg) Pop[Line].Val = Pop[Line].Max;
}
/*******************************************************************************
 
*******************************************************************************/
u16 IndxInc(u16 Val, u16 Unit, u16 Indx)
{
  s32 Temp = Val+Unit;
  return (Temp <= Menu[Indx].Max) ? Temp : 
         (LOOP &  Menu[Indx].Flg) ? Menu[Indx].Min : Val;
}
/*******************************************************************************
 
*******************************************************************************/
u16 IndxDec(u16 Val, u16 Unit, u16 Indx)
{
  s32 Temp = Val-Unit;
  return (Temp >= Menu[Indx].Min) ? Temp : 
         (LOOP &  Menu[Indx].Flg) ? Menu[Indx].Max : Val;
}

/******************************** END OF FILE *********************************/
