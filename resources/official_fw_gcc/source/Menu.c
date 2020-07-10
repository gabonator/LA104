#include "Menu.h"
#include "MyDefine.h"
#include <stdint.h>
#include <string.h>

u8   SmallWin[SW_WIDTH];
u8   gBatFlag = 1;
u8   gTriCond_X, gTriMask_X;
u16  gT0Posi, gT1Posi, gT2Posi, gZCPosi, gXPosi;

u8   ItemIndexTmp = 0;
u16  CurBuf[YHIGH];
u16  TimeCurBuf[4][YHIGH];
u32  xPosition = 0;
u64  gLast_XPosi = 0;

uc16 TitlePosi[3] = { 0, 22, 292 };
uc16 LeftTitleCol[6] = { CYN, YEL, PUR, GRN, ORN, BLU };
uc16 CursorCol[4] = { WHT, WHT, GRN, ORN };
uc16 CondMaskCol[6] = { CYN_, CYN, YEL, PUR, GRN, CYN_ };

uc8  TimeUnitStr[][6] = { "nS", "uS", "mS" };
uc8  VoltageStr[][6] = { "mV ", "V  ", "kV " };
uc8  ChannelStr[][4] = { "CH1", "CH2", "CH3", "CH4" };
uc8  ITEM_STStr[][6] = { "  No ", " Yes " };
uc8  Switch_Str[][7] = { "  OFF ", "  ON  " };

uc8  InputTypeStr[][5] = { "User", "SPI ", "I2C ", "UART", "USB ", "CAN ", "DMX ", "SWD " };
uc8  TimeStr[][3] = { "TB", "T1", "T2", " X", "T0", "ZC" };
uc8  OutputTypeStr[][5] = { " PWM", " SPI", " I2C", "UART", };
uc8  DateFormatStr[][7] = { "Ascii ", " DEC  ", " HEX  ", " BIN  ", };
uc8  StopBitsStr[][7] = { "  0.0 ", "  1.0 ", "  1.5 ", "  2.0 " };
uc8  ParityTypeStr[][7] = { " None ", "  Odd ", " Even " };
uc8  BaudRateStr[][7] = { " 4800 ", " 9600 ", " 14400", " 19200", " 38400", " 56000",
    " 57600", "115200", };
uc8  SourceStr[][7] = { " CH_1 ", " CH_2 ", " CH_3 ", " CH_4 ", };

uc8  Freq_Str[][7] = { " 10Hz ", " 20Hz ", " 50Hz ", " 100Hz",
    " 200Hz", " 500Hz", " 1KHz ", " 2KHz ",
    " 5KHz ", " 10KHz", " 20KHz", " 50KHz",
    "100KHz", "200KHz", "500KHz", " 1MHz " };

//uc8 OUT_PWM_Str[][14]= {"P1: PWM1_CH1 ", "P2: PWM1_CH2 ", "P3: PWM2_CH1 ", "P4: PWM2_CH2 ",};
uc8 OUT_PWM_Str[][14] = { "P1: PWM1_CH1 ", "P2: PWM1_CH2 ", "P3:          ", "P4:          ", };
uc8 OUT_SPI_Str[][14] = { "P1: SPI_MOSI ", "P2: SPI_MISO ", "P3: SPI_SCK  ", "P4: SPI_CS   ", };
uc8 OUT_I2C_Str[][14] = { "P1: I2C_SDA  ", "P2: I2C_SCL  ", "P3:          ", "P4:          ", };
uc8 OUT_UART_Str[][14] = { "P1: UART_TX  ", "P2:          ", "P3:          ", "P4:          ", };
//==============+======+======+======+======+======+======+======+======+======+
//  
//--------------+------+------+------+------+------+------+------+------+------+
uc8  BT_S[][5] = { ";<=", ";<=", ";&@", ";*@", ">`@", "\"#$", "\"#$" }; // 
uc16 V_BT[] = {  3250,  3300,  3400,  3700,  3900,  4200,  4200 };  // 
uc16 BT_C[] = {  RED,   YEL,   WHT,   WHT,   WHT,   CYN_,  CYN_ /*GRN*/ };  // 
//==============+======+======+======+======+======+======+======+======+======+
uc8  TimeBaseStr[][8] =
{
/*
    " 1.0nS ", " 2.0nS ", " 5.0nS ", "  10nS ",
    "  20nS ", "  50nS ", " 0.1uS ", " 0.2uS ",
    " 0.5uS ", " 1.0uS ", " 2.0uS ", " 5.0uS ",
    "  10uS ", "  20uS ", "  50uS ", " 0.1mS ",
    " 0.2mS ", " 0.5mS ", " 1.0mS ", " 2.0mS " 
*/
    " 1.0nS ", " 2.0nS ", " 5.0nS ", "  10nS ",
    "  20nS ", "  10nS ", "  20nS ", "  50nS ",
    " 0.1uS ", " 0.2uS ", " 0.5uS ", " 1.0uS ",
    " 2.0uS ", " 5.0uS ", "  10uS ", "  20uS ",
    "  50uS ", " 0.1mS ", " 0.2mS ", " 0.5mS "
};
uc8  TB_Str[][8] =
{
    "1.0nS ", "2.0nS ", "5.0nS ", "10nS  ",
    "20nS  ", "10nS  ", "20nS  ", "50nS  ",
    "0.1uS ", "0.2uS ", "0.5uS ", "1.0uS ",
    "2.0uS ", "5.0uS ", "10uS  ", "20uS  ",
    "50uS  ", "0.1mS ", "0.2mS ", "0.5mS "
};
uc32  gTB_Scale[20] =
{
    1,         2,           5,           1,
    2,         1,           2,           5,
    10,        20,          50,          100,
    200,       500,         1000,        2000,
    5000,      10000,       20000,       50000,
};

uc8  TriTypeStr[][8] = {  "  None ", " NE_CM ", " EQ_CM ", "   Any ",
    " LNE_W ", " LEQ_W ", " GNE_W ", " GEQ_W ", };
uc8  TriCondStr[][4] = {  "Non", " ^`", " ==", "Any", " ^`", " ==", " ^`", " ==", };
uc8  ProtocolStr[][6] =
{
    "CH1   ", "CH2   ", "CH3   ", "CH4   ",
    "MOSI  ", "MISO  ", "SCK   ", "CS    ",
    "SDA   ", "SCL   ", "      ", "      ",
    "UART  ", "      ", "      ", "      ",
    "D+    ", "D-    ", "      ", "      ",
    "CAN   ", "      ", "      ", "      ",
    "DMX512", "      ", "      ", "      ",
    "SWCLK ", "SWDIO ", "      ", "      ",
};

uc8  PercentStr[][10] =
{
    "  0%     ", " 10%     ", " 20%     ", " 30%     ", " 40%     ",
    " 50%     ", " 60%     ", " 70%     ", " 80%     ", " 90%     ", " 100%    "
};
uc8  InputStr[][13] = {
//  
    "Input:    ",
};
uc8  User_SetStr[][16] = {
    "               ", "               ",
};
uc8  SPI_SetStr[][16] = {
    "CS Enable:     ", "Clock Polarity:", "Clock Phase:   ",
    "Bit Order:     ", "Word Size:     ", "Data Format:   ",
};
uc8  I2C_SetStr[][16] = {
    "R/W bit Set 0: ", "Data Format:   ",
};
uc8  UART_SetStr[][16] = {
    "Baud Rate:     ", "Data Bits:     ", "Stop Bits:     ",
    "Parity Type:   ", "Bit Order:     ", "Invert Signal: ",
    "Data Format:   ",
};

uc8  TimeSetStr[][13] = {
    "TimeBase: ", "T1.Posi:  ", "T2.Posi:  ", "X .Posi:  ", "T0.Posi:  ", "ZC.Posi:  ",
};
uc8  TriggerStr[][13] = {
    "TriType:  ", "TriCond:  ", "TriMask:  ", "TriWidth: ", "LogLevel: ", /*"Threshold:",*/
};
uc8  MeasureStr[][13] = {
    "Source1:  ", "Source2:  ", "Source3:  ",
};
uc8  OutputStr[][13] = {
    "Output:   ",
};
uc8  PWM_SetStr[][16] = {
    "PWM1 Enable:   ", "PWM1 Freq:     ", "PWM1 CH1 Duty: ", "PWM1 CH2 Duty: ",
    "PWM2 Enable:   ", "PWM2 Freq:     ", "PWM2 CH1 Duty: ", "PWM2 CH2 Duty: ",
};

uc8  FileSysStr[][13] = {
    "SaveBmp:  ", "SaveCsv:  ", "SaveSvg:  ", "SaveBuf:  ",
};
uc8  SettingStr[][13] = {
    "Save:     ", "Restore:  ", "Volume:   ", "BKlight:  ", "Standby:  ", "PowerOff: ",
};


uc8 gItemMaxNum[MENU_NUM] = {
//  ʵ
    0, 5 - 2, 4, 1, 0, 1, 5,
};
uc8 ItemNum[MENU_NUM] = {

    2, 6 - 2, 5, 2, 5, 2, 6,
};

u8 gItemIndexNum[MENU_NUM] = {
    0, 0, 0, 0, 0, 0, 0,
};

uc8 gItemIndexNumBackup[MENU_NUM] = {
    0, 0, 0, 0, 0, 0, 0,
};

uc8 gInSubMaxNum[][ITEM_NUM] = {
    { 0, 5, 1, 6 },
};

u8 gInSubIndexNum[][ITEM_NUM] = {
    { 0, 0, 0, 0 },
};

uc8 gInSubIndexNumBackup[][ITEM_NUM] = {
    { 0, 0, 0, 0 },
};

uc8 gOutSubMaxNum[][ITEM_NUM] = {
//  { 7, 0, 0, 0 },
    { 3, 0, 0, 0 },
};

u8 gOutSubIndexNum[][ITEM_NUM] = {
    { 0, 0, 0, 0 },
};

uc8 gOutSubIndexNumBackup[][ITEM_NUM] = {
    { 0, 0, 0, 0 },
};


u8 gInputSubParam[][SUB_NUM] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 8, 0, 0, 0 },
    { 0, 2, 0, 0, 0, 0, 0, 0 },
    { 7, 8, 1, 0, 0, 0, 0, 0 },
};

uc8 gInputSubParamBackup[][SUB_NUM] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 8, 0, 0, 0 },
    { 0, 2, 0, 0, 0, 0, 0, 0 },
    { 7, 8, 1, 0, 0, 0, 0, 0 },
};

uc8 gInputSubParamMax[][SUB_NUM] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 8, 2, 0, 0 },
    { 1, 2, 1, 1, 1, 1, 1, 1 },
    { 7, 8, 1, 0, 1, 0, 2, 1 },
};

uc8 gInputSubParamMin[][SUB_NUM] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 8, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 8, 1, 0, 0, 0, 0, 0 },
};

u8 gOutSubParam[][SUB_NUM] = {
    { 0, 10, 5, 5, 0, 10, 5, 5 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};

uc8 gOutSubParamBackup[][SUB_NUM] = {
    { 0, 10, 5, 5, 0, 10, 5, 5 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};

uc8 gOutSubParamMax[][SUB_NUM] = {
    { 1, 15, 9, 9, 1, 15, 9, 9 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};

uc8 gOutSubParamMin[][SUB_NUM] = {
    { 0, 6, 1, 1, 0, 6, 1, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};

u16  gItemParam[PARAM_NUM] = {
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    8, 50, 230, 0,  TRI_POSI, 300,  0,  0,  0,  0,
    2,  0x0001,  0x0701,  1,  30,  0,  0,  0,  0,  0,
    0,  1,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  5,  5,  5, 30,  0,  0,  0,  0,
};

uc16  gItemParamBackup[PARAM_NUM] = {
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    8, 50, 230, 0,  TRI_POSI, 300,  0,  0,  0,  0,
    2,  0x0001,  0x0701,  1,  30,  0,  0,  0,  0,  0,
    0,  1,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  5,  5,  5, 30,  0,  0,  0,  0,
};

uc16 gItemParamMax[] = {
    3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    19, 275, 275, 0, 0,  0,  0,  0,  0,  0,
    7,  4,  4,  55, 30, 0,  0,  0,  0,  0,
    3,  3,  0,  0,  0,  0,  0,  0,  0,  0,
    3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    0,  0, 10, 10, 30, 60,  0,  0,  0,  0,
};

uc16  gItemParamMin[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    5,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  1,  1,  1, 12,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
};
uc16  gItemParamStep[] = {
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  5,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

/*******************************************************************************
 * FunctionName : ShowAllMenu
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowAllMenu(void)
{
//  
    DrawRectangle(0, 220, 320, 20, DAR);  //
    DrawRectangle(0,   1, 320,  3, DAR);  //
    DrawRectangle(0,  56,  38, 13, GRAY); //

    ShowSmplStatus(STOP);
    ShowBattery();
    ShowTopTitle();
    ShowLeftTitle();
    DrawTriggerIco();
    ShowTimeBaseStr();
    ShowScaleRuleTime(RULE_X, RULE_Y, SHOW_WIDTH, gItemParam[TIMEBASE]);
    ShowMeasureStr();
    ShowTimeMarkValue();
    ShowMeasureValue();
    SmallWinsInit();
}

/*******************************************************************************
 * FunctionName : ShowSmplStatus
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowSmplStatus(u8 Status)
{
    DrawRectangle(TitlePosi[0],  TITLE_Y,    6 * 3 - 1, 2, GRAY);       //0 * 6
    DrawRectangle(TitlePosi[0],  TITLE_Y + 12, 6 * 3 - 1, 2, GRAY);     //0 * 6
    DrawTriangle(TitlePosi[0] + 17, TITLE_Y,    14, GRAY, DIR_RIGHT);   //3 * 6-1
    Set_Color(GRAY, WHT);
    Show_Str6x8(TitlePosi[0],   TITLE_Y + 2,  PRN,  BOLD, " ");         //0 * 6
    if(Status == 0)
    {
        Set_Color(GRAY, WHT);
        Show_Str6x8(TitlePosi[0] + 6, TITLE_Y + 2,  SYMB, BOLD, "|");   //
    }
    else
    {
        Set_Color(GRAY, GRN);
        Show_Str6x8(TitlePosi[0] + 6, TITLE_Y + 2,  SYMB, BOLD, "{");   //
    }
    DrawRectangle(TitlePosi[0] + 12, TITLE_Y, 5, 14, GRAY);             //2 * 6
}

/*******************************************************************************
 * FunctionName : ShowBattery
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowBattery(void)
{
    u8 i = 0;
    u8 Str[5];
    u8 BatStatus = 0;

    if(gBatFlag == 1)
    {
        gBatFlag = 0;
        if(gUSB_Vol)
        {
            if(gBatVol > 4200) BatStatus = 6;
            else BatStatus = 5;
        }
        else
        {
            for(i = 5; i > 0; i--)
            {
                if(gBatVol > V_BT[i])
                {
                    BatStatus = i;
                    break;
                }
            }
        }
        DrawTriangle(TitlePosi[2],    TITLE_Y, 14, GRAY, DIR_LEFT);         //49 * 6 - 2
        DrawRectangle(TitlePosi[2] + 7, TITLE_Y,    3 * 6, 2, GRAY);        //50 * 6 - 1
        DrawRectangle(TitlePosi[2] + 7, TITLE_Y + 12, 3 * 6, 2, GRAY);      //50 * 6 - 1
        DrawRectangle(TitlePosi[2] + 25, TITLE_Y, 2, 14, GRAY);             //53 * 6 - 1
        Set_Color(GRAY, BT_C[BatStatus]);
        memcpy(Str, &BT_S[BatStatus], 4);
        Show_Str6x8(TitlePosi[2] + 7, TITLE_Y + 2,  SYMB, BOLD, Str);       //50 * 6 - 1
    }
}

/*******************************************************************************
 * FunctionName : ShowTopTitle
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowTopTitle(void)
{
    u8  i = 0;
    u8  ptr[10] = { 0 };
    u8  index = 0;
    u8  position = 0;
    u16 menu_col = 0;
    for(i = 0; i < 5; i++)
    {
        if(Menu.index == i)
        {
            menu_col = CYN_;
            Set_Color(CYN_, BLK);
        }
        else if((Menu.index > TITLE_CNT) && (i == TITLE_CNT))
        {
            menu_col = CYN_;
            Set_Color(CYN_, BLK);
        }
        else
        {
            menu_col = GRAY;
            Set_Color(GRAY, WHT);
        }
        DrawTriangle(TitlePosi[1] + (i * 54), TITLE_Y, 14, menu_col, DIR_LEFT);             // 3 * 6 + 4
        DrawRectangle(TitlePosi[1] + 7 + (i * 54), TITLE_Y,      7 * 6, 2, menu_col);       // 4 * 6 + 5
        DrawRectangle(TitlePosi[1] + 7 + (i * 54), TITLE_Y + 12, 7 * 6, 2, menu_col);       // 4 * 6 + 5
        if((Menu.index >= TITLE_CNT) && (i == TITLE_CNT) && (Menu.index != TITLE_CNT + 2)) DrawTriCol(TitlePosi[1] + 49 + (i * 54), TITLE_Y, 14, menu_col, YEL, DIR_RIGHT);
        else DrawTriangle(TitlePosi[1] + 49 + (i * 54), TITLE_Y, 14, menu_col, DIR_RIGHT);  // 11 * 6 + 5

        if(Menu.index > TITLE_CNT)
        {
            index = i + Menu.index - TITLE_CNT;
            position = i;
        }
        else
        {
            index = i;
            position = i;
        }
        switch(index)
        {
        case 0:
            memcpy(&ptr[0], "In:", 3);
            memcpy(&ptr[3], InputTypeStr[gItemParam[INTYPE]], 4);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5
            break;
        case 1:
//          memcpy(&ptr[0], "TimeSet", 7);
//          Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5
            memcpy(&ptr[0], "Time:", 5);
            memcpy(&ptr[5], TimeStr[gItemIndexNum[TIME_SET]], 2);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr);
            break;
        case 2:
//          memcpy(&ptr[0], "Trigger", 7);
//          Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5

            memcpy(&ptr[0], "Tri:", 4);
            memcpy(&ptr[4], TriCondStr[gItemParam[TRITYPE]], 3);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5

            break;
        case 3:
            memcpy(&ptr[0], "Measure", 7);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5
            break;
        case 4:
            memcpy(&ptr[0], "Out", 3);
            memcpy(&ptr[3], OutputTypeStr[gItemParam[OUTTYPE]], 4);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5
            break;
        case 5:
            memcpy(&ptr[0], "FileSys", 7);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5
            break;
        case 6:
            memcpy(&ptr[0], "Setting", 7);
            Show_Str6x8(TitlePosi[1] + 7 + (position * 54), TITLE_Y + 2,  PRN,  BOLD, ptr); //4 * 6 + 5
            break;
        }
    }
}

/*******************************************************************************
 * FunctionName : ShowLeftTitle
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowLeftTitle(void)
{
    u8  i = 0;
    u8  Str[10];

//  
    for(i = 0; i < 6; i++)
    {
        DrawRectangle(0, MENU_Y + 25 * (5 - i), 38, 24, GRAY);
        sprintf((char *)Str, "%1d", i + 1);
        Disp_Str7x9(0, MENU_Y + 25 * (5 - i) + 13, BLK, LeftTitleCol[i], PRN, THIN, Str);
    }

//  
    for(i = 0; i < 4; i++)
    {
        Set_Color(GRAY, WHT);
        memset(&Str[0], 0, 10);
        memcpy(&Str[0], ProtocolStr[gItemParam[INTYPE] * 4] + i * 6, 6);
        Show_Str6x8(1, MENU_Y + 25 * (5 - i) + 1,  PRN,  BOLD, Str);
    }
    Set_Color(GRAY, WHT);
    Show_Str6x8(1, MENU_Y + 25 * 1 + 1,   PRN,  BOLD, "DATA");
    Show_Str6x8(1, MENU_Y + 25 * 0 + 1,   PRN,  BOLD, "DATA");
    DrawTriggerIco();
}

/*******************************************************************************
 * FunctionName : DrawTriggerIco
 * Description  : 
*******************************************************************************/
void DrawTriggerIco(void)
{
    u8 i = 0;
    for(i = 0; i < 4; i++)
    {
        Set_Color(GRAY, BLK);
        if(((gItemParam[TRIMASK] >> 8) >> i) & 0x01) //
        {
//          Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "MN");   //
            Show_Str6x8(3 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "\\MN"); //
        }
        else
        {
            switch(gItemParam[TRITYPE])
            {
            case  TRI_Any:
                Show_Str6x8(3 * 6, MENU_Y + 25 * (5 - i) + 13,   PRN,  BOLD, "Any");     //
                break;
            case  TRI_DiNE:
            case  TRI_DiEQ:
                Show_Str6x8(3 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, (u8 *)((gItemParam[TRITYPE] > TRI_DiNE) ? "}" : "~")); //
                if(((gItemParam[TRICOND] >> 8) >> i) & 0x01)
                {
                    Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "IJ"); //
                }
                else
                {
                    Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "KL"); //
                }
                break;
            case  TRI_DfLE:
            case  TRI_DtLE:

                Show_Str6x8(3 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, (u8 *)((gItemParam[TRITYPE] > TRI_DfLE) ? "}" : "~")); //
                if(((gItemParam[TRICOND] >> 8) >> i) & 0x01)
                {
                    Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "AB"); //
                }
                else
                {
                    Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "CD"); //
                }
                break;
            case  TRI_DfGT:
            case  TRI_DtGT:

                Show_Str6x8(3 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, (u8 *)((gItemParam[TRITYPE] > TRI_DfGT) ? "}" : "~")); //
                if(((gItemParam[TRICOND] >> 8) >> i) & 0x01)
                {
                    Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "EF"); //
                }
                else
                {
                    Show_Str6x8(4 * 6, MENU_Y + 25 * (5 - i) + 13,   SYMB,  BOLD, "GH"); //
                }
                break;
            }
        }
    }
}
/*******************************************************************************
 * FunctionName : ShowPopItem
 * Description  : 
 * Param        : u8 MenuIndex 
*******************************************************************************/
void ShowPopItem(u8 MenuIndex)
{
    switch(MenuIndex)
    {
    case IN_TYPE:
//      ShowPopWindow(40, 216, 6 * 18, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        ShowPopWindow(40, 216, 6 * 19, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    case TIME_SET:
        if(Menu.flag == 1)
        {
            memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);        // 
            FPGA_LCD_RW(WFRM_SAVE, XWDTH, gLCD_Buf);

            FPGA_ReadWrite(T0_POSN, 2, 300);                     // 
            FPGA_ReadWrite(T1_POSN, 2, 300);
            FPGA_ReadWrite(T2_POSN, 2, 300);
            FPGA_ReadWrite(ZC_POSN, 2, 300);
            Set_Block(X0, Y0, XWDTH + X0 - 1, YHIGH + Y0 - 1);
            __Bios(DISPDEV, FPGA_MOD);                           // 
            FPGA_ReadWrite(WFRM_DRAW, 0, 0);                     // ˢ

            Delay_mS(10);

            __Bios(DISPDEV, MCU_DRV);
            Show_AnalyzeData(gItemParam[INTYPE]);
        }
        ShowPopWindow(50, 216, 6 * 21, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    case TRIGGER:
//      ShowPopWindow(90, 216, 6 * 21, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        ShowPopWindow(90, 216, 6 * 23, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    case MEASURE:
        ShowPopWindow(150, 216, 6 * 20, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    case OUT_TYPE:
//      ShowPopWindow(200, 216, 6 * 18, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        ShowPopWindow(200, 216, 6 * 20, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    case FILE_CTRL:
//      ShowPopWindow(200, 216, 6 * 18, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        ShowPopWindow(200, 216, 6 * 20, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    case SETTING:
//      ShowPopWindow(180, 216, 6 * 21, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        ShowPopWindow(180, 216, 6 * 23, 6 + 14 * ItemNum[MenuIndex], CYN_, DAR, MenuIndex);
        break;
    }
}

/*******************************************************************************
 * FunctionName : ShowPopWindow
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Width 
 * Param        : u16 Height 
 * Param        : u16 WinCol 
 * Param        : u16 BlkCol 
 * Param        : u8  MenuIndex 
*******************************************************************************/
void ShowPopWindow(u16 x0, u16 y0, u16 Width, u16 Height, u16 WinCol, u16 BlkCol, u8 MenuIndex)
{
    u8 i = 0;
    u8  Str[90], TempStr[10];
    u8  *CharPtr;

    memset(Str, 0, 90);
    memset(TempStr, 0, 10);
    y0 = y0 - Height - 4;    // 4Ϊ

//  
    if(Menu.flag)
    {
        DrawRectangle(x0 + 1, y0 + 1, Width - 2, Height - 2, BlkCol);
        DrawRectangle(x0,   y0, 1, Height, WinCol);
        DrawRectangle(x0 + Width - 1, y0, 1, Height, WinCol);
        DrawRectangle(x0, y0, Width, 1, WinCol);
        DrawRectangle(x0, y0 + Height - 1, Width, 5, WinCol);
        if(MenuIndex == TIME_SET)
        {
            RW_Row_Color(gItemParam[T0POSI] + X0, Y0, YHIGH, TimeCurBuf[BUF_T0], RD);
            RW_Row_Color(gItemParam[T1POSI] + X0, Y0, YHIGH, TimeCurBuf[BUF_T1], RD);
            RW_Row_Color(gItemParam[T2POSI] + X0, Y0, YHIGH, TimeCurBuf[BUF_T2], RD);
            RW_Row_Color(gItemParam[ZCPOSI] + X0, Y0, YHIGH, TimeCurBuf[BUF_ZC], RD);
            DrawDottedLine(gItemParam[T0POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T0]);
            DrawDottedLine(gItemParam[T1POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T1]);
            DrawDottedLine(gItemParam[T2POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T2]);
            DrawDottedLine(gItemParam[ZCPOSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_ZC]);
        }
//      Menu.flag = 0;
    }

//  
    switch(MenuIndex)
    {
    case IN_TYPE:
        CharPtr = (u8 *)InputStr;
        memcpy(&Str[LEN * 0], "       ", 7);
        memcpy(&Str[LEN * 0 + 2], &InputTypeStr[gItemParam[INTYPE]], 4);

        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 8, 7);
        if(gItemParam[INTYPE] == IN_USER)
        {
            Disp_Str6x8(x0 + 6 * 17 + 2, y0 + 4 + 14 * 1, GRAY, DAR, SYMB, BOLD, "m");
        }
        else
        {
            Disp_Str6x8(x0 + 6 * 17 + 2, y0 + 4 + 14 * 1, YEL, DAR, SYMB, BOLD, "m");
        }
        ShowLeftTitle();
        break;
    case TIME_SET:

//      if (ItemIndexTmp != gItemIndexNum[Menu.index])   //
        {
            ItemIndexTmp = gItemIndexNum[Menu.index];
//          if(gItemIndexNum[Menu.index] != 0) memcpy(CurBuf, &TimeCurBuf[gItemIndexNum[Menu.index] - 1], YHIGH * 2);
            if(gItemIndexNum[Menu.index] == TB_T1) memcpy(CurBuf, &TimeCurBuf[BUF_T1], YHIGH * 2);
            if(gItemIndexNum[Menu.index] == TB_T2) memcpy(CurBuf, &TimeCurBuf[BUF_T2], YHIGH * 2);
        }
        switch(gItemIndexNum[Menu.index])
        {
        case TB_TB:
//          DrawScaleRule(40, RULE_Y - 11, 280, gItemParam[TIMEBASE] + 1);
            ShowScaleRuleTime(RULE_X, RULE_Y, SHOW_WIDTH, gItemParam[TIMEBASE]);
            break;
        case TB_T0:
            if(gT0Posi != gItemParam[T0POSI])
            {
                RW_Row_Color(gT0Posi + X0, Y0, YHIGH, CurBuf, WR);                  //д
                RW_Row_Color(gItemParam[T0POSI] + X0, Y0, YHIGH, CurBuf, RD);       //
                memcpy(&TimeCurBuf[BUF_T0], CurBuf, YHIGH * 2);
//              BackupPreCursorPosition(gItemIndexNum[Menu.index] - 1); //ѡ
                BackupPreCursorPosition(gItemIndexNum[Menu.index]);
                ClrCursor(gT0Posi + X0, Y0, BLK);
                DrawDottedLine(gItemParam[T1POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T1]);
                DrawDottedLine(gItemParam[T2POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T2]);
                DrawDottedLine(gItemParam[ZCPOSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_ZC]);
                DrawDottedLine(gItemParam[T0POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T0]);
            }
            else
            {
                DrawDottedLine(gItemParam[T0POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T0]);
            }
            break;
        case TB_T1:
            if(gT1Posi != gItemParam[T1POSI])
            {
                RW_Row_Color(gT1Posi + X0, Y0, YHIGH, CurBuf, WR);
                RW_Row_Color(gItemParam[T1POSI] + X0, Y0, YHIGH, CurBuf, RD);
                memcpy(&TimeCurBuf[BUF_T1], CurBuf, YHIGH * 2);
                BackupPreCursorPosition(gItemIndexNum[Menu.index]);
                ClrCursor(gT1Posi + X0, Y0, BLK);
                DrawDottedLine(gItemParam[T0POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T0]);
                DrawDottedLine(gItemParam[T2POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T2]);
                DrawDottedLine(gItemParam[ZCPOSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_ZC]);
                DrawDottedLine(gItemParam[T1POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T1]);
            }
            else
            {
                DrawDottedLine(gItemParam[T1POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T1]);
            }
            break;
        case TB_T2:
            if(gT2Posi != gItemParam[T2POSI])
            {
                RW_Row_Color(gT2Posi + X0, Y0, YHIGH, CurBuf, WR);
                RW_Row_Color(gItemParam[T2POSI] + X0, Y0, YHIGH, CurBuf, RD);
                memcpy(&TimeCurBuf[BUF_T2], CurBuf, YHIGH * 2);
                BackupPreCursorPosition(gItemIndexNum[Menu.index]);
                ClrCursor(gT2Posi + X0, Y0, BLK);
                DrawDottedLine(gItemParam[T1POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T1]);
                DrawDottedLine(gItemParam[T0POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T0]);
                DrawDottedLine(gItemParam[ZCPOSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_ZC]);
                DrawDottedLine(gItemParam[T2POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T2]);
            }
            else
            {
                DrawDottedLine(gItemParam[T2POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T2]);
            }
            break;
        case TB_ZC:
            if(gZCPosi != gItemParam[ZCPOSI])
            {
                RW_Row_Color(gZCPosi + X0, Y0, YHIGH, CurBuf, WR);
                RW_Row_Color(gItemParam[ZCPOSI] + X0, Y0, YHIGH, CurBuf, RD);
                memcpy(&TimeCurBuf[BUF_ZC], CurBuf, YHIGH * 2);
                BackupPreCursorPosition(gItemIndexNum[Menu.index]);

                ClrCursor(gZCPosi + X0, Y0, BLK);
                DrawDottedLine(gItemParam[T0POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T0]);
                DrawDottedLine(gItemParam[T1POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T1]);
                DrawDottedLine(gItemParam[T2POSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_T2]);
                DrawDottedLine(gItemParam[ZCPOSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_ZC]);
            }
            else
            {
                DrawDottedLine(gItemParam[ZCPOSI] + X0, Y0, 1, YHIGH, CursorCol[BUF_ZC]);
            }
            break;
        }

        Draw_ScaleRule();

        gT0Posi = gItemParam[T0POSI];
        gT1Posi = gItemParam[T1POSI];
        gT2Posi = gItemParam[T2POSI];
        gZCPosi = gItemParam[ZCPOSI];

        CharPtr = (u8 *)TimeSetStr;

        memcpy(&Str[LEN * 0], &TimeBaseStr[gItemParam[TIMEBASE]], 7);

        sprintf((char *)TempStr, "%5d", gItemParam[T1POSI]);
        strcat((char *)TempStr, "  ");
        memcpy(&Str[LEN * 1], TempStr, 7);

        sprintf((char *)TempStr, "%5d", gItemParam[T2POSI]);
        strcat((char *)TempStr, "  ");
        memcpy(&Str[LEN * 2], TempStr, 7);

        u32ToStr_Unit(TempStr, gLastX_Time * 10, (u8 *)TimeUnitStr, 3);
        strcat((char *)TempStr, "  ");
        memcpy(&Str[LEN * 3], TempStr, 7);

        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 11, 7);

        break;
    case TRIGGER:
        CharPtr = (u8 *)TriggerStr;

        memcpy(&Str[LEN * 0], TriTypeStr[gItemParam[TRITYPE]], 7);

        memcpy(&Str[LEN * 1], "  ", 2);
        for(i = 0; i < 4; i++)
        {
            if((gItemParam[TRICOND] >> (i + 8)) & 0x01) memcpy(&Str[LEN * 1 + 2 + i], "{", 1); // 
            else                           memcpy(&Str[LEN * 1 + 2 + i], "}", 1); // 
        }
        memcpy(&Str[LEN * 1 + 6], " ", 1);
        memcpy(&Str[LEN * 2], "  ", 2);
        for(i = 0; i < 4; i++)
        {
            if((gItemParam[TRIMASK] >> (i + 8)) & 0x01) memcpy(&Str[LEN * 2 + 2 + i], "x", 1);
            else                           memcpy(&Str[LEN * 2 + 2 + i], "$", 1);
        }
        memcpy(&Str[LEN * 2 + 6], " ", 1);

        memset(TempStr, 0, 10);
        u32ToStr_Unit(TempStr, gItemParam[TRIWIDTH] * gTB_Scale[gItemParam[TIMEBASE]] * 10, (u8 *)TimeUnitStr, 3);
        strcat((char *)TempStr, "  ");
        memcpy(&Str[LEN * 3], TempStr, 7);

        memset(TempStr, 0, 10);
        ValuedivTen2Str(TempStr, gItemParam[THRESHOLD]);
        memcpy(&Str[LEN * 4], "  ", 2);
        memcpy(&Str[LEN * 4 + 2], TempStr, 3);
        memcpy(&Str[LEN * 4 + 5], "V ", 2);

        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 11, 7);

        if(gItemIndexNum[TRIGGER] == 1) //TriCond
        {
            DrawCursor(171 + 6 * gTriMask_X, MASK_CUR_H, DAR);      //
            DrawCursor(171 + 6 * gTriCond_X, COND_CUR_H, DAR);      //
            gTriCond_X = (u8)gItemParam[TRICOND];
            DrawCursor(171 + 6 * gTriCond_X, COND_CUR_H, (u16)CondMaskCol[(u8)gTriCond_X]);     //
        }
        else if(gItemIndexNum[TRIGGER] == 2) //TriMask
        {
            DrawCursor(171 + 6 * gTriCond_X, COND_CUR_H, DAR);      //
            DrawCursor(171 + 6 * gTriMask_X, MASK_CUR_H, DAR);      //
            gTriMask_X = (u8)gItemParam[TRIMASK];
            DrawCursor(171 + 6 * gTriMask_X, MASK_CUR_H, (u16)CondMaskCol[(u8)gTriMask_X]);     //
        }
        else
        {
            DrawCursor(171 + 6 * gTriCond_X, COND_CUR_H, DAR);      //
            DrawCursor(171 + 6 * gTriMask_X, MASK_CUR_H, DAR);      //
        }

        DrawTriggerIco();

        Disp_Str6x8(x0 + 6 * 20 + 2, y0 + 4 + 14 * 2, CYN_, DAR, SYMB, BOLD, "ok");
        Disp_Str6x8(x0 + 6 * 20 + 2, y0 + 4 + 14 * 3, CYN_, DAR, SYMB, BOLD, "ok");
        break;
    case MEASURE:
        CharPtr = (u8 *)MeasureStr;

        memcpy(&Str[LEN * 0], &SourceStr[gItemParam[SOURCE1]], 7);
        memcpy(&Str[LEN * 1], &SourceStr[gItemParam[SOURCE2]], 7);
        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 11, 6);
        break;
    case OUT_TYPE:
        CharPtr = (u8 *)OutputStr;

        memcpy(&Str[LEN * 0], "       ", 7);
        memcpy(&Str[LEN * 0 + 1], &OutputTypeStr[gItemParam[OUTTYPE]], 4);

        if(gItemParam[OUTTYPE] == OUT_PWM)
        {
            Disp_Str6x8(x0 + 6 * 17 + 2, y0 + 4 + 14 * 4, YEL, DAR, SYMB, BOLD, "mn");
        }
        else
        {
            Disp_Str6x8(x0 + 6 * 17 + 2, y0 + 4 + 14 * 4, CYN_, DAR, SYMB, BOLD, "ok");
        }
        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 8, 7);
        Show_PIO_Name(x0, y0, gItemParam[OUTTYPE]);
        ShowLeftTitle();
        break;
    case FILE_CTRL:
        CharPtr = (u8 *)FileSysStr;

        sprintf((char *)TempStr, "%3d", gItemParam[SAVEBMP]);
        strcat((char *)TempStr, " ");
        memcpy(&Str[LEN * 0], TempStr, 4);


        sprintf((char *)TempStr, "%3d", gItemParam[SAVECSV]);
        strcat((char *)TempStr, " ");
        memcpy(&Str[LEN * 1], TempStr, 4);


        sprintf((char *)TempStr, "%3d", gItemParam[SAVESVG]);
        strcat((char *)TempStr, " ");
        memcpy(&Str[LEN * 2], TempStr, 4);


        sprintf((char *)TempStr, "%3d", gItemParam[SAVEBUF]);
        strcat((char *)TempStr, " ");
        memcpy(&Str[LEN * 3], TempStr, 4);


        Disp_Str6x8(x0 + 6 * 17 + 2, y0 + 4 + 14 * 0, CYN_, DAR, SYMB, BOLD, "ok");
        Disp_Str6x8(x0 + 6 * 17 + 2, y0 + 4 + 14 * 1, CYN_, DAR, SYMB, BOLD, "ok");

        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 11, 4);
        break;
    case SETTING:
        CharPtr = (u8 *)SettingStr;

        memcpy(&Str[LEN * 0], " Param  ", 7);
        memcpy(&Str[LEN * 1], " Param  ", 7);

        sprintf((char *)TempStr, "%4d", gItemParam[VOLUME] * 10);
        strcat((char *)TempStr, "%  ");
        memcpy(&Str[LEN * 2], TempStr, 7);


        sprintf((char *)TempStr, "%4d", gItemParam[BKLIGHT] * 10);
        strcat((char *)TempStr, "%  ");
        memcpy(&Str[LEN * 3], TempStr, 7);


        sprintf((char *)TempStr, "%3d", gItemParam[STANDBY]);
        strcat((char *)TempStr, "min ");
        memcpy(&Str[LEN * 4], TempStr, 7);


        sprintf((char *)TempStr, "%3d", gItemParam[POWEROFF]);
        strcat((char *)TempStr, "min ");
        memcpy(&Str[LEN * 5], TempStr, 7);

        Disp_Str6x8(x0 + 6 * 20 + 2, y0 + 4 + 14 * 4, CYN_, DAR, SYMB, BOLD, "ok");
        Disp_Str6x8(x0 + 6 * 20 + 2, y0 + 4 + 14 * 5, CYN_, DAR, SYMB, BOLD, "ok");

        ShowPopStr(x0, y0, gItemMaxNum[Menu.index], gItemIndexNum[Menu.index], CharPtr, Str, 11, 7);
        break;
    }

}
/*******************************************************************************
 * FunctionName : ShowSubItem
 * Description  : 
 * Param        : u8 NowMenu 
 * Param        : u8 NowItme 
*******************************************************************************/
void ShowSubItem(u8 NowMenu, u8 NowItme)
{
    u8  param_str[90], TempStr[10];
    u8  *char_str;

    memset(param_str, 0, 90);
    memset(TempStr, 0, 10);

    switch(NowMenu)
    {
    case IN_TYPE:
        switch(NowItme)
        {
        case INTYPE:
            switch(gItemParam[INTYPE])
            {
            case IN_USER:
                char_str = (u8 *)User_SetStr;
                ShowSubPop(60, 195, 6 * 24, 6 + 14 * (INPUT_SUB_MAX_NUM + 1), YEL, DAR, char_str, param_str, SIP_PARAM_STR_LEN);
                break;

            case IN_SPI:
                char_str = (u8 *)SPI_SetStr;

                memcpy(&param_str[LEN * 0], (gInputSubParam[IN_SPI][0] ? "  1  " : "  0  "), SIP_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 1], (gInputSubParam[IN_SPI][1] ? "  1  " : "  0  "), SIP_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 2], (gInputSubParam[IN_SPI][2] ? "  1  " : "  0  "), SIP_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 3], (gInputSubParam[IN_SPI][3] ? " MSB " : " LSB "), SIP_PARAM_STR_LEN);
                sprintf((char *)TempStr, "%3d", gInputSubParam[IN_SPI][4]);
                strcat((char *)TempStr, "  ");
                memcpy(&param_str[LEN * 4], TempStr, SIP_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 5], &DateFormatStr[gInputSubParam[IN_SPI][5]], SIP_PARAM_STR_LEN);

                ShowSubPop(60, 195, 6 * 24, 6 + 14 * (INPUT_SUB_MAX_NUM + 1), YEL, DAR, char_str, param_str, SIP_PARAM_STR_LEN);
                break;
            case IN_I2C:
                char_str = (u8 *)I2C_SetStr;
                memcpy(&param_str[LEN * 0], &ITEM_STStr[gInputSubParam[IN_I2C][0]], I2C_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 1], &DateFormatStr[gInputSubParam[IN_I2C][1]], I2C_PARAM_STR_LEN);

                ShowSubPop(60, 195, 6 * 24, 6 + 14 * (INPUT_SUB_MAX_NUM + 1), YEL, DAR, char_str, param_str, I2C_PARAM_STR_LEN);
                break;
            case IN_UART:
                char_str = (u8 *)UART_SetStr;

                memcpy(&param_str[LEN * 0], &BaudRateStr[gInputSubParam[IN_UART][0]], UART_PARAM_STR_LEN);
                sprintf((char *)TempStr, "%4d", gInputSubParam[IN_UART][1]);
                strcat((char *)TempStr, "  ");
                memcpy(&param_str[LEN * 1], TempStr, UART_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 2], &StopBitsStr[gInputSubParam[IN_UART][2]], UART_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 3], &ParityTypeStr[gInputSubParam[IN_UART][3]], UART_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 4], (gInputSubParam[IN_UART][4] ? "  MSB " : "  LSB "), UART_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 5], (gInputSubParam[IN_UART][5] ? "  Yes " : "  No  "), UART_PARAM_STR_LEN);
                memcpy(&param_str[LEN * 6], &DateFormatStr[gInputSubParam[IN_UART][6]], UART_PARAM_STR_LEN);

                ShowSubPop(60, 195, 6 * 25, 6 + 14 * (INPUT_SUB_MAX_NUM + 1), YEL, DAR, char_str, param_str, UART_PARAM_STR_LEN);
                break;
            }
            break;
        }
        break;
    case TIME_SET:
        switch(NowItme + 10 * TIME_SET) // ÿһ
        {
        case TIMEBASE:
            break;
        }
        break;
    case TRIGGER:
        break;
    case MEASURE:
        break;
    case OUT_TYPE:
        switch(gItemParam[OUTTYPE])
        {
        case OUT_PWM:
            char_str = (u8 *)PWM_SetStr;
            memcpy(&param_str[LEN * 0], (gOutSubParam[OUT_PWM][0] ? "  ON  " : "  OFF "), PWM_PARAM_STR_LEN);
            memcpy(&param_str[LEN * 1], &Freq_Str[gOutSubParam[OUT_PWM][1]], PWM_PARAM_STR_LEN);
            sprintf((char *)TempStr, "%4d", gOutSubParam[OUT_PWM][2] * 10);
            strcat((char *)TempStr, "% ");
            memcpy(&param_str[LEN * 2], TempStr, 6);
            sprintf((char *)TempStr, "%4d", gOutSubParam[OUT_PWM][3] * 10);
            strcat((char *)TempStr, "% ");
            memcpy(&param_str[LEN * 3], TempStr, 6);

            memcpy(&param_str[LEN * 4], (gOutSubParam[OUT_PWM][4] ? "  ON  " : "  OFF "), PWM_PARAM_STR_LEN);
            memcpy(&param_str[LEN * 5], &Freq_Str[gOutSubParam[OUT_PWM][5]], PWM_PARAM_STR_LEN);
            sprintf((char *)TempStr, "%4d", gOutSubParam[OUT_PWM][6] * 10);
            strcat((char *)TempStr, "% ");
            memcpy(&param_str[LEN * 6], TempStr, 6);
            sprintf((char *)TempStr, "%4d", gOutSubParam[OUT_PWM][7] * 10);
            strcat((char *)TempStr, "% ");
            memcpy(&param_str[LEN * 7], TempStr, 6);

            ShowSubPop(160, 195, 6 * 25, 6 + 14 * (OUT_SUB_MAX_NUM + 1), YEL, DAR, char_str, param_str, PWM_PARAM_STR_LEN);

            break;
        default:
            break;
        }
        break;
    case FILE_CTRL:
        break;
    case SETTING:
        break;
    default:
        break;
    }

}

/*******************************************************************************
 * FunctionName : ShowSubPop
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Width 
 * Param        : u16 Height 
 * Param        : u16 WinCol 
 * Param        : u16 BlkCol 
*******************************************************************************/
void ShowSubPop(u16 x0, u16 y0, u16 Width, u16 Height, u16 WinCol, u16 BlkCol, u8 *CharStr, u8 *ParamStr, u8 ParamStrWidth)
{
    y0 = y0 - Height - 4; // 4Ϊ

    if(Item.flag)
    {
        DrawRectangle(x0 + 1, y0 + 1, Width - 2, Height - 2, BlkCol);
        DrawRectangle(x0,   y0, 1, Height, WinCol);
        DrawRectangle(x0 + Width - 1, y0, 1, Height, WinCol);
        DrawRectangle(x0, y0, Width, 1, WinCol);
        DrawRectangle(x0, y0 + Height - 1, Width, 5, WinCol);
    }
    switch(Menu.index)
    {
    case IN_TYPE:
        if(gItemParam[INTYPE] != IN_USER)
        {
            ShowSubPopStr(x0, y0, INPUT_SUB_MAX_NUM, INPUT_SUB_INDEX_NUM, CharStr, ParamStr, 16, 9, 16, ParamStrWidth);
        }
        break;
    case OUT_TYPE:
        if(gItemParam[OUTTYPE] == OUT_PWM)
        {
            ShowOutSubPopStr(x0, y0, OUT_SUB_MAX_NUM, OUT_SUB_INDEX_NUM, CharStr, ParamStr, 16, 9, 16, ParamStrWidth);
        }
        break;
    default:
        break;
    }
}
/*******************************************************************************
 * FunctionName : ShowSubPopStr
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u8  ItemNum    
 * Param        : u8  ItemIndex  
 * Param        : u8* CharStr    
 * Param        : u8* ParamStr   
 * Param        : u8  CharLen    
 * Param        : u8  ParamLen   
 * Param        : u8  StrStarNum 
 * Param        : u8  StrWidth   
*******************************************************************************/
void ShowSubPopStr(u16 x0, u16 y0, u8 ItemNum, u8 ItemIndex, u8 *CharStr, u8 *ParamStr, u8 CharLen, u8 ParamLen, u8 StrStarNum, u8 StrWidth)
{
    u8  i = 0;
    u16 x = 0, y = 0;

    for(i = 0; i <= ItemNum; i++)
    {
        if(ItemNum == 0)
        {
            x = x0 + 6;
            y = y0 + 5 + 14 * 1;       // 
        }
        else
        {
            x = x0 + 6;
            y = y0 + 5 + 14 * (ItemNum  - i);
        }
        if(i == ItemIndex)
        {
            Set_Color(YEL, BLK);
            Show_Str6x8(x, y, PRN, BOLD, CharStr);
            if(INPUT_SUB_PARAM == INPUT_SUB_PARAM_MIN)
            {
                Set_Color(YEL, DAR);
            }
            else
            {
                Set_Color(YEL, BLK);
            }
            Show_Str6x8(x + 6 * (StrStarNum - 1), y, PRN, BOLD, "<");
            Set_Color(YEL, BLK);
            x = x0 + 6 + StrStarNum * 6;
            Show_Str6x8(x, y, PRN, BOLD, ParamStr);
            x = x0 + 6 + (StrStarNum + StrWidth - 1) * 6;
            if(INPUT_SUB_PARAM == INPUT_SUB_PARAM_MAX)
            {
                Set_Color(YEL, DAR);
            }
            else
            {
                Set_Color(YEL, BLK);
            }
            Show_Str6x8(x + 6, y, PRN, BOLD, ">");
        }
        else
        {
            Set_Color(DAR, WHT);
            Show_Str6x8(x, y, PRN, BOLD, CharStr);
            if(gInputSubParam[gItemParam[INTYPE]][0 + i] == gInputSubParamMin[gItemParam[INTYPE]][0 + i])
            {
                Set_Color(DAR, GRAY);
            }
            else
            {
                Set_Color(DAR, WHT);
            }
            Show_Str6x8(x + 6 * (StrStarNum - 1), y, PRN, BOLD, "<");
            Set_Color(DAR, WHT);
            x = x0 + 6 + StrStarNum * 6;
            Show_Str6x8(x, y, PRN, BOLD, ParamStr);
            x = x0 + 6 + (StrStarNum + StrWidth - 1) * 6;
            if(gInputSubParam[gItemParam[INTYPE]][0 + i] == gInputSubParamMax[gItemParam[INTYPE]][0 + i])
            {
                Set_Color(DAR, GRAY);
            }
            else
            {
                Set_Color(DAR, WHT);
            }
            Show_Str6x8(x + 6, y, PRN, BOLD, ">");
        }

        CharStr  += CharLen;    //  SettingStr
        ParamStr += ParamLen;   //  LEN = 9
    }
}
/*******************************************************************************
 * FunctionName : ShowOutSubPopStr
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u8 ItemNum 
 * Param        : u8 ItemIndex 
 * Param        : u8* CharStr 
 * Param        : u8* ParamStr 
 * Param        : u8 CharLen 
 * Param        : u8 ParamLen 
 * Param        : u8 StrStarNum 
 * Param        : u8 StrWidth 
*******************************************************************************/
void ShowOutSubPopStr(u16 x0, u16 y0, u8 ItemNum, u8 ItemIndex, u8 *CharStr, u8 *ParamStr, u8 CharLen, u8 ParamLen, u8 StrStarNum, u8 StrWidth)
{
    u8  i = 0;
    u16 x = 0, y = 0;

    for(i = 0; i <= ItemNum; i++)
    {
        if(ItemNum == 0)
        {
            x = x0 + 6;
            y = y0 + 5 + 14 * 1;       // 
        }
        else
        {
            x = x0 + 6;
            y = y0 + 5 + 14 * (ItemNum  - i);
        }
        if(i == ItemIndex)
        {
            Set_Color(YEL, BLK);
            Show_Str6x8(x, y, PRN, BOLD, CharStr);
            if(OUT_SUB_PARAM == OUT_SUB_PARAM_MIN) Set_Color(YEL, DAR);
            else                                  Set_Color(YEL, BLK);
            Show_Str6x8(x + 6 * (StrStarNum - 1), y, PRN, BOLD, "<");
            Set_Color(YEL, BLK);
            x = x0 + 6 + StrStarNum * 6;
            Show_Str6x8(x, y, PRN, BOLD, ParamStr);
            x = x0 + 6 + (StrStarNum + StrWidth - 1) * 6;
            if(OUT_SUB_PARAM == OUT_SUB_PARAM_MAX) Set_Color(YEL, DAR);
            else                                  Set_Color(YEL, BLK);
            Show_Str6x8(x + 6, y, PRN, BOLD, ">");
        }
        else
        {
            Set_Color(DAR, WHT);
            Show_Str6x8(x, y, PRN, BOLD, CharStr);
            if(gOutSubParam[gItemParam[OUTTYPE]][0 + i] == gOutSubParamMin[gItemParam[OUTTYPE]][0 + i])
            {
                Set_Color(DAR, GRAY);
            }
            else
            {
                Set_Color(DAR, WHT);
            }
            Show_Str6x8(x + 6 * (StrStarNum - 1), y, PRN, BOLD, "<");
            Set_Color(DAR, WHT);
            x = x0 + 6 + StrStarNum * 6;
            Show_Str6x8(x, y, PRN, BOLD, ParamStr);
            x = x0 + 6 + (StrStarNum + StrWidth - 1) * 6;
            if(gOutSubParam[gItemParam[OUTTYPE]][0 + i] == gOutSubParamMax[gItemParam[OUTTYPE]][0 + i])
            {
                Set_Color(DAR, GRAY);
            }
            else
            {
                Set_Color(DAR, WHT);
            }
            Show_Str6x8(x + 6, y, PRN, BOLD, ">");
        }

        CharStr  += CharLen;    //  SettingStr
        ParamStr += ParamLen;   //  LEN = 9
    }
}
/*******************************************************************************
 * FunctionName : ShowPopStr
 * Description  : 10
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u8  ItemNum 
 * Param        : u8  ItemIndex 
 * Param        : u8* CharStr 
 * Param        : u8* ParamStr 
 * Param        : u8  StrStarNum   
*******************************************************************************/
void ShowPopStr(u16 x0, u16 y0, u8 ItemNum, u8 ItemIndex, u8 *CharStr, u8 *ParamStr, u8 StrStarNum, u8 StrWidth)
{
    u8  i = 0;
    u16 x = 0, y = 0;

    for(i = 0; i <= ItemNum; i++)
    {
        if(ItemNum == 0)
        {
            if(Menu.index == OUT_TYPE)
            {
                x = x0 + 6;
                y = y0 + 5 + 14 * 4;       // 
            }
            else
            {
                x = x0 + 6;
                y = y0 + 5 + 14 * 1;       // 
            }
        }
        else
        {
            x = x0 + 6;
            y = y0 + 5 + 14 * (ItemNum  - i);
        }
        if(i == ItemIndex)
        {
            Set_Color(CYN_, BLK);
            Show_Str6x8(x, y, PRN, BOLD, CharStr);
            if(gItemParam[Menu.index * 10 + i] == gItemParamMin[Menu.index * 10 + i])
            {
                Set_Color(CYN_, DAR);
            }
            else
            {
                Set_Color(CYN_, BLK);
            }
            Show_Str6x8(x + 6 * (StrStarNum - 1), y, PRN, BOLD, "<");
            Set_Color(CYN_, BLK);
            x = x0 + 6 + StrStarNum * 6;
            Show_Str6x8(x, y, PRN, BOLD, ParamStr);
            x = x0 + 6 + (StrStarNum + StrWidth - 1) * 6;
            if(gItemParam[Menu.index * 10 + i] == gItemParamMax[Menu.index * 10 + i])
            {
                Set_Color(CYN_, DAR);
            }
            else
            {
                Set_Color(CYN_, BLK);
            }
            Show_Str6x8(x + 6, y, PRN, BOLD, ">");
        }
        else
        {
            Set_Color(DAR, WHT);
            Show_Str6x8(x, y, PRN, BOLD, CharStr);
            if(gItemParam[Menu.index * 10 + i] == gItemParamMin[Menu.index * 10 + i])
            {
                Set_Color(DAR, GRAY);
            }
            else
            {
                Set_Color(DAR, WHT);
            }
            Show_Str6x8(x + 6 * (StrStarNum - 1), y, PRN, BOLD, "<");
            Set_Color(DAR, WHT);
            x = x0 + 6 + StrStarNum * 6;
            Show_Str6x8(x, y, PRN, BOLD, ParamStr);
            x = x0 + 6 + (StrStarNum + StrWidth - 1) * 6;
            if(gItemParam[Menu.index * 10 + i] == gItemParamMax[Menu.index * 10 + i])
            {
                Set_Color(DAR, GRAY);
            }
            else
            {
                Set_Color(DAR, WHT);
            }
            Show_Str6x8(x + 6, y, PRN, BOLD, ">");
        }

        CharStr  += 13;  //  SettingStr
        ParamStr += 9;   //  LEN = 9
    }
}

/*******************************************************************************
 * FunctionName : DrawDottedLine
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Width 
 * Param        : u16 Height 
 * Param        : u16 Col 
 * Return       : u8   
*******************************************************************************/
u8 DrawDottedLine(u16 x0, u16 y0, u16 Width, u16 Height, u16 Col)
{
    u16 x, y, i, j;
    x = x0;
    y = y0;
//  
    if((x0 + Width) > 320 || (y0 + Height) > 240)
    {
        return 0;
    }

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 2 * i + 1; j++)
        {
            if((x - i + j) >= 40)
            {
                Set_Posi(x - i + j, 216 + i); Set_Pixel(Col);
                Set_Posi(x - i + j, 73 - 4 - i); Set_Pixel(Col);
            }
        }
    }

    for(i = 0; i < Width; i++)
    {
        // Set_Posi(x++, y);
        for(j = 0; j < Height; j++)
        {
            Set_Posi(x, y + j);
            if(j % 4 == 0)
            {
                if((x > 50) &&  (x < 50 + 6 * 21))
                {
                    if((y + j) > (120 + 14 * 2))
                    {}
                    else Set_Pixel(Col);
                }
                else Set_Pixel(Col);
            }
        }
        x++;
    }
    return 1;
}

/*******************************************************************************
 * FunctionName : ClrCursor
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Col 
*******************************************************************************/
void ClrCursor(u16 x0, u16 y0, u16 Col)
{
    u16 x, i, j;
    x = x0;

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 2 * i + 1; j++)
        {
            if((x - i + j) >= 40)
            {
                Set_Posi(x - i + j, 216 + i); Set_Pixel(Col);
                Set_Posi(x - i + j, 69 - i); Set_Pixel(Col);
            }
        }
    }


}

/*******************************************************************************
 * FunctionName : Draw_ScaleRule
 * Description  : 
*******************************************************************************/
void Draw_ScaleRule(void)
{
    u16 i = 0;

    for(i = 0; i < SHOW_WIDTH; i++)
    {
        if((i % 50) == 0)
        {
            Set_Posi(i + 40, 66); Set_Pixel(WHT); Set_Pixel(WHT); Set_Pixel(WHT); Set_Pixel(WHT); Set_Pixel(WHT);
        }
        else if((i % 25) == 0) // 40
        {
            Set_Posi(i + 40, 66); Set_Pixel(WHT); Set_Pixel(WHT); Set_Pixel(WHT);
        }
        else if((i % 5) == 0)
        {
            Set_Posi(i + 40, 66); Set_Pixel(WHT); Set_Pixel(WHT);
        }
    }
}

/*******************************************************************************
 * FunctionName : DrawCursor
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Col 
*******************************************************************************/
void DrawCursor(u16 x0, u16 y0, u16 Col)
{
    u16 x, y, i, j;
    x = x0;
    y = y0;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 2 * i + 1; j++)
        {

            Set_Posi(x - i + j, y + i); Set_Pixel(Col);
        }
    }
}

/*******************************************************************************
 * FunctionName : DrawRectangle
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Width  
 * Param        : u16 Height 
 * Param        : u16 Col 
 * Return       : u8  0
*******************************************************************************/
u8 DrawRectangle(u16 x0, u16 y0, u16 Width, u16 Height, u16 Col)
{
    u16 x, y, i, j;
    x = x0;
    y = y0;
//  
    if((x0 + Width) > 320 || (y0 + Height) > 240)
    {
        return 0;
    }
    for(i = 0; i < Width; i++)
    {
        Set_Posi(x++, y);
        for(j = 0; j < Height; j++) Set_Pixel(Col);
    }
    return 1;
}

/*******************************************************************************
 * FunctionName : DrawRecLine
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Width 
 * Param        : u16 Height 
 * Param        : u16 Col 
 * Return       : u8   
*******************************************************************************/
u8 DrawRecLine(u16 x0, u16 y0, u16 Width, u16 Height, u16 Col)
{
    u16 x, y, i;
    x = x0;
    y = y0;
//  
    if((x0 + Width) > (LCD_ROW - 1) || (y0 + Height) > (LCD_COL - 1))
    {
        return 0;
    }

    Set_Posi(x, y);
    for(i = 0; i < Height; i++) Set_Pixel(Col);
    Set_Posi(x + Width - 1, y);
    for(i = 0; i < Height; i++) Set_Pixel(Col);
    for(i = 0; i < Width; i++)
    {
        Set_Posi(x + i, y);
        Set_Pixel(Col);
    }
    for(i = 0; i < Width; i++)
    {
        Set_Posi(x + i, y + Height - 1);
        Set_Pixel(Col);
    }

    return 1;
}

/*******************************************************************************
 * FunctionName : DrawTriangle
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Height 
 * Param        : u16 Col 
 * Param        : u8  Direction 
*******************************************************************************/
void DrawTriangle(u16 x0, u16 y0, u16 Height, u16 Col, u8 Direction)
{
    u16 x, y, i, j;
    x = x0;
    y = y0;

    if(Direction == DIR_LEFT)
    {
        for(i = 0; i < Height;)
        {
            Set_Posi(x++, y);
            for(j = 0; j < i + 2; j++) Set_Pixel(Col);
            i += 2;
        }
    }
    else
    {
        for(i = Height; i > 0;)
        {
            Set_Posi(x++, y + Height - i);
            for(j = 0; j < i; j++) Set_Pixel(Col);
            i -= 2;
        }
    }
}

/*******************************************************************************
 * FunctionName : DrawTriangle
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Height 
 * Param        : u16 Col 
 * Param        : u8  Direction 
*******************************************************************************/
void DrawTriCol(u16 x0, u16 y0, u16 Height, u16 Col, u16 TriCol, u8 Direction)
{
    u16 x, y, i, j;
    x = x0;
    y = y0;

    if(Direction == DIR_LEFT)
    {
        for(i = 0; i < Height;)
        {
            Set_Posi(x++, y);
            for(j = 0; j < i + 2; j++) Set_Pixel(Col);
            i += 2;
        }
    }
    else
    {
        for(i = Height; i > 0;)
        {
            Set_Posi(x++, y + Height - i);
            for(j = 0; j < i; j++)
            {

                if(x < x0 + 4) Set_Pixel(Col);
                else        Set_Pixel(TriCol);
            }
            i -= 2;
        }
    }
}

/*******************************************************************************
 * FunctionName : ShowScaleRuleTime
 * Description  : ˮƽ
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Length 
 * Param        : u8  Multiple 
*******************************************************************************/
void ShowScaleRuleTime(u16 x0, u16 y0, u16 Length, u8 Multiple)
{
    u8  Str[7] = { 0 };
    u8  Len = 0;
    u16 x, y, i;
    u32 Scale = 0;
    u64 StartTime = 0;

    StartTime = gLastX_Time;

    x = x0;
    y = y0;
    Multiple = Multiple + 1;                       // 

//  ƫ
    if(StartTime > 0)
    {

        //  
        switch(Multiple % 3)
        {
        case 0:
            Scale = 10 *  Exp(Multiple / 3 - 1);
            break;
        case 1:
            Scale = 2 *  Exp(Multiple / 3);
            break;
        case 2:
            Scale = 5 *  Exp(Multiple / 3);
            break;
        }

        Set_Posi(x, y);
        for(i = 0; i <= 10; i++)
        {
            if(i % 2 == 0)
            {
                Set_Posi(x, y);
                Set_Color(BLK, GRAY);

                u32ToStr_Unit(Str, i * Scale / 2 + StartTime * 10, (u8 *)TimeUnitStr, 3);
                Len = strlen((char *)Str);
                strncat((char *)Str, (char *)"       ", (7 - Len));

                if(i == 0)
                {
                    Show_Str6x8(x - 2, y + 1,  PRN,  THIN, Str);
                }
                else
                {
                    Show_Str6x8(x + 16 + (i - 1) * 25, y + 1,  PRN,  THIN, Str);
                }
            }
        }
    }
    else
    {
        DrawScaleRule(RULE_X, RULE_Y, SHOW_WIDTH, gItemParam[TIMEBASE] + 1);
    }

    ShowWindowPrecent();

}

/*******************************************************************************
 * FunctionName : DrawScaleRule
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u16 Length 
 * Param        : u8  Multiple 
*******************************************************************************/
void DrawScaleRule(u16 x0, u16 y0, u16 Length, u8 Multiple)
{
    u8  Str[7] = { 0 };
    u8  Len = 0;
    u16 x, y, i;
    u32 Scale = 0;

    x = x0;
    y = y0;

//  
    switch(Multiple % 3)
    {
    case 0:
        Scale = 10 *  Exp(Multiple / 3 - 1);
        break;
    case 1:
        Scale = 2 *  Exp(Multiple / 3);
        break;
    case 2:
        Scale = 5 *  Exp(Multiple / 3);
        break;
    }

    Set_Posi(x, y);
    for(i = 0; i <= 10; i++)
    {
        if(i % 2 == 0)
        {
            Set_Posi(x, y);
            Set_Color(BLK, GRAY);

            u32ToStr_Unit(Str, i * Scale / 2, (u8 *)TimeUnitStr, 3);

            Len = strlen((char *)Str);
            strncat((char *)Str, (char *)"       ", (7 - Len));

            if(i == 0)
            {
                Show_Str6x8(x - 2, y + 1,  PRN,  THIN, Str);
            }
            else
            {
                Show_Str6x8(x + 16 + (i - 1) * 25, y + 1,  PRN,  THIN, Str);

            }

        }
    }

}
/*******************************************************************************
 * FunctionName : ShowWindowPrecent
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowWindowPrecent(void)
{
#define WIDTH  10

    u16 Color = 0;
    u64 xPosi = 0;

    if(gTransTimeSum == 0)
    {
        Color = GRAY;
        xPosi = 0;
    }
    else
    {
        xPosi = gLastX_Time * (LCD_ROW - WIDTH) / gTransTimeSum;
        Color = CYN_;
    }

    DrawRectangle(gLast_XPosi, 0, WIDTH, 1, BLK);
    DrawRectangle(gLast_XPosi, 4, WIDTH, 1, BLK);
    DrawRectangle(gLast_XPosi, 1, WIDTH, 3, DAR);
    DrawRectangle(xPosi,       0, WIDTH, 5, Color);
    gLast_XPosi = xPosi;

}

/*******************************************************************************
 * FunctionName : ShowTimeBaseStr
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowTimeBaseStr(void)
{
    u8  Str[10] = { 0 };

    memcpy(Str, TB_Str[gItemParam[TIMEBASE]], 6);
    Set_Color(GRAY, WHT);
    Show_Str6x8(6 * 0,  RULE_Y + 2,  PRN,   BOLD, Str);
}

/*******************************************************************************
 * FunctionName : BackupPreCursorPosition
 * Description  : 
 * Param        : u8 IndexCur 
*******************************************************************************/
void BackupPreCursorPosition(u8 IndexCur)
{
    u8 Index = 0;
    u8 BufCnt = 0;
    switch(IndexCur)
    {
    case TB_T1:
        Index = T1POSI;
        BufCnt = BUF_T1;
        break;
    case TB_T2:
        Index = T2POSI;
        BufCnt = BUF_T2;
        break;
    case TB_T0:
        Index = T0POSI;
        BufCnt = BUF_T0;
        break;
    case TB_ZC:
        Index = ZCPOSI;
        BufCnt = BUF_ZC;
        break;
    }

    if(gItemParam[Index] == gItemParam[T0POSI])
    {
        if(IndexCur != TB_T0)
        {
            memcpy(&TimeCurBuf[BufCnt], &TimeCurBuf[BUF_T0], YHIGH * 2);
        }
    }
    else if(gItemParam[Index] == gItemParam[T1POSI])
    {
        if(IndexCur != TB_T1)
        {
            memcpy(&TimeCurBuf[BufCnt], &TimeCurBuf[BUF_T1], YHIGH * 2);
        }
    }
    else if(gItemParam[Index] == gItemParam[T2POSI])
    {
        if(IndexCur != TB_T2)
        {
            memcpy(&TimeCurBuf[BufCnt], &TimeCurBuf[BUF_T2], YHIGH * 2);
        }
    }
    else if(gItemParam[Index] == gItemParam[ZCPOSI])
    {
        if(IndexCur != TB_ZC)
        {
            memcpy(&TimeCurBuf[BufCnt], &TimeCurBuf[BUF_ZC], YHIGH * 2);
        }
    }

}

/*******************************************************************************
 * FunctionName : ShowMeasureStr
 * Description  : 
*******************************************************************************/
void ShowMeasureStr(void)
{
    u8 i = 0;

//  
    for(i = 0; i < 3; i++)
    {
        DrawRectangle(0, MEASURE_Y + 12 * i, 38, 11, GRAY);
    }
    Set_Color(GRAY, WHT);
    Show_Str6x8(1,  MEASURE_Y + 24, PRN,  BOLD, "Measur");
    Set_Color(GRAY, WHT);
//  
    ShowMeasureSourceStr();

    DrawRectangle(40, TEST_Y + 48, 280, 1, GRAY);
    DrawRectangle(40, TEST_Y + 36, 280, 1, GRAY);
    DrawRectangle(40, TEST_Y + 24, 280, 1, GRAY);
    DrawRectangle(40, TEST_Y + 12, 280, 1, GRAY);
//  DrawRectangle(40, TEST_Y +  0, 280, 1, GRAY);
    DrawRectangle(40 - 1,              MEASURE_Y - 1, 1, 37, GRAY);
    DrawRectangle(40 + 6 * 9 * 1 - 3,  MEASURE_Y - 1, 1, 36, GRAY);
    DrawRectangle(40 + 6 * 9 * 2 - 3,  MEASURE_Y - 1, 1, 36, GRAY);
    DrawRectangle(40 + 6 * 9 * 3 - 3,  MEASURE_Y - 1, 1, 36, GRAY);
    DrawRectangle(40 + 6 * 9 * 4 - 3,  MEASURE_Y - 1, 1, 36, GRAY);
    DrawRectangle(319,                 MEASURE_Y - 1, 1, 37, GRAY);

//  
    Set_Color(BLK, WHT);
    Show_Str6x8(40 + 6 * 9 * 0,  TEST_Y + 37,  PRN,  THIN, "P_PWSum");
    Show_Str6x8(40 + 6 * 9 * 1,  TEST_Y + 37,  PRN,  THIN, "N-PWSum");
    Show_Str6x8(40 + 6 * 9 * 2,  TEST_Y + 37,  PRN,  THIN, "UpNum  ");
    Show_Str6x8(40 + 6 * 9 * 3,  TEST_Y + 37,  PRN,  THIN, "DownNum");
//  Show_Str6x8(40 + 6 * 7 * 4,  TEST_Y + 37,  PRN,  BOLD, "Freq");

    Show_Str6x8(46 + 6 * 7 * 5,  TEST_Y + 37,  PRN,  BOLD, "T1:------");
    Show_Str6x8(46 + 6 * 7 * 5,  TEST_Y + 25,  PRN,  BOLD, "T2:------");
    Show_Str6x8(46 + 6 * 7 * 5,  TEST_Y + 13, SYMB,  BOLD, "]");
    Show_Str6x8(52 + 6 * 7 * 5,  TEST_Y + 13,  PRN,  BOLD, "T:------");
}

/*******************************************************************************
 * FunctionName : ShowTimeMarkValue
 * Description  : 
*******************************************************************************/
void ShowTimeMarkValue(void)
{
    u8 Str[10] = { 0 };
    u8 Length = 0;
    u32 TimeMark1 = 0;
    u32 TimeMark2 = 0;
    u32 TimeMark = 0;
    //T1_Time=StartTime+T1Cnt/5*gTB_Scale[gItemParam[TIMEBASE]]

    TimeMark1 = gLastX_Time + gItemParam[T1POSI] * gTB_Scale[gItemParam[TIMEBASE]] / TIME_TO_POINT;
    u32ToStr_Unit(Str, TimeMark1 * 10, (u8 *)TimeUnitStr, 3);
    Length = strlen((char *)Str);
    strncat((char *)Str, (char *)"      ", (6 - Length));
    Set_Color(BLK, WHT);
//  Show_Str6x8(256 + 18,  TEST_Y + 37, PRN, THIN, "      ");
    Show_Str6x8(256 + 18,  TEST_Y + 37, PRN, THIN, Str);

    TimeMark2 = gLastX_Time + gItemParam[T2POSI] * gTB_Scale[gItemParam[TIMEBASE]] / TIME_TO_POINT;
    u32ToStr_Unit(Str, TimeMark2 * 10, (u8 *)TimeUnitStr, 3);
    Length = strlen((char *)Str);
    strncat((char *)Str, (char *)"      ", (6 - Length));
    Set_Color(BLK, WHT);
//  Show_Str6x8(256 + 18,  TEST_Y + 25, PRN, THIN, "      ");
    Show_Str6x8(256 + 18,  TEST_Y + 25, PRN, THIN, Str);

    if(gItemParam[T2POSI] >= gItemParam[T1POSI])
    {
        TimeMark = gItemParam[T2POSI] - gItemParam[T1POSI];
    }
    else
    {
        TimeMark = gItemParam[T1POSI] - gItemParam[T2POSI];
    }
    TimeMark = TimeMark * gTB_Scale[gItemParam[TIMEBASE]] / TIME_TO_POINT;

    u32ToStr_Unit(Str, TimeMark * 10, (u8 *)TimeUnitStr, 3);
    Length = strlen((char *)Str);
    strncat((char *)Str, (char *)"      ", (6 - Length));
    Set_Color(BLK, WHT);
//  Show_Str6x8(256 + 18,  TEST_Y + 13, PRN, THIN, "      ");
    Show_Str6x8(256 + 18,  TEST_Y + 13, PRN, THIN, Str);
}

/*******************************************************************************
 * FunctionName : ShowMeasureSourceStr
 * Description  : 
 * Param        : void 
*******************************************************************************/
void ShowMeasureSourceStr(void)
{
    u8 temp_str[10];

    Set_Color(GRAY, WHT);
//  
    memcpy(temp_str, &ChannelStr[gItemParam[SOURCE1]], 4);
    Show_Str6x8(5,  MEASURE_Y + 12, PRN,  THIN, temp_str);
    memcpy(temp_str, &ChannelStr[gItemParam[SOURCE2]], 4);
    Show_Str6x8(5,  MEASURE_Y +  0, PRN,  THIN, temp_str);
}

/*******************************************************************************
 * FunctionName : ProcessSmallWins
 * Description  : 
*******************************************************************************/
void ProcessSmallWins(void)
{
    u16 i = 0, j = 0, Cnt = 0;
    u64 AvgTime = 0;
    u64 SumTime = 0;

    j = TRI_START_NUM + 1;
    Cnt = TRI_START_NUM + 1;
    AvgTime = gTransTimeSum / SW_WIDTH + ((gTransTimeSum % SW_WIDTH) ? 1 : 0);

    for(i = 1; i <= SW_WIDTH; /*i++*/)
    {
        if(SumTime >= i * AvgTime)
        {
            if(Cnt == j)
            {
                SmallWin[i - 1] = 0;
            }
            else
            {
                SmallWin[i - 1] = 1;
                if((TransStatusInfo(j) & FPGA_EMPTY) == FPGA_EMPTY) SmallWin[i - 1] = 0;
                Cnt = j;
            }
            i++;
        }
        else
        {
            SumTime += TransformTime(j);
            j++;
        }
    }
    if(gTransTimeSum) ShowSmallWins();
    else SmallWinsInit();
}


/*******************************************************************************
 * FunctionName : ShowSmallWins
 * Description  : 
*******************************************************************************/
void ShowSmallWins(void)
{
    u16 i = 0, j = 0;

    DrawRectangle(0, 8, SW_WIDTH, 8, BLK);

    for(i = 0; i < SW_WIDTH; i++)
    {
        if(SmallWin[i] == 0)
        {
            Set_Posi(i, 12);
            Set_Pixel(SW_COL);
        }
        else if(SmallWin[i] == 1)
        {
            Set_Posi(i, 8);
            for(j = 0; j < 8; j++)
            {
                Set_Pixel(SW_COL);
            }
        }
    }
}


/*******************************************************************************
 * FunctionName : SmallWinsInit
 * Description  : 
*******************************************************************************/
void SmallWinsInit(void)
{
    DrawRectangle(0, 8, SW_WIDTH, 8, BLK);
}

/*******************************************************************************
 * FunctionName : Show_PIO_Name
 * Description  : 
 * Param        : u16 x 
 * Param        : u16 y 
 * Param        : u8 Type 
*******************************************************************************/
void Show_PIO_Name(u16 x, u16 y, u8 Type)
{
#define PIO_NUM  4
    u8 i = 0;

    switch(Type)
    {
    case OUT_PWM:
        for(i = 0; i < PIO_NUM; i++)
        {
            Disp_Str6x8(x + 6, y + 4 + 14 * (PIO_NUM - 1 - i), GRAY, DAR, PRN, BOLD, (u8 *)&OUT_PWM_Str[i]);
        }
        break;
    case OUT_SPI:
        for(i = 0; i < PIO_NUM; i++)
        {
            Disp_Str6x8(x + 6, y + 4 + 14 * (PIO_NUM - 1 - i), GRAY, DAR, PRN, BOLD, (u8 *)&OUT_SPI_Str[i]);
        }
        break;
    case OUT_I2C:
        for(i = 0; i < PIO_NUM; i++)
        {
            Disp_Str6x8(x + 6, y + 4 + 14 * (PIO_NUM - 1 - i), GRAY, DAR, PRN, BOLD, (u8 *)&OUT_I2C_Str[i]);
        }
        break;
    case OUT_UART:
        for(i = 0; i < PIO_NUM; i++)
        {
            Disp_Str6x8(x + 6, y + 4 + 14 * (PIO_NUM - 1 - i), GRAY, DAR, PRN, BOLD, (u8 *)&OUT_UART_Str[i]);
        }
        break;
    default:
        break;
    }

}
