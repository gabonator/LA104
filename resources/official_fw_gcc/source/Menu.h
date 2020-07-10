#ifndef __MENU_H
#define __MENU_H

#include "STM32F10x.h"

#define   DIR_LEFT        0
#define   DIR_RIGHT       1
#define   I_INDEX         (Menu.index*10+gItemIndexNum[Menu.index])

#define   STOP            0
#define   RUN             1

#define   COND_CUR_H      194
#define   MASK_CUR_H      180

#define   TITLE_Y         223
#define   TEST_Y          7
#define   MEASURE_Y       20
#define   MENU_Y          70
#define   SCALE_Y         0
#define   TITLE_CNT       4     //0-4¼´ÊÇÎª5
#define   LEN             9

#define   RULE_X          41
#define   RULE_Y          55
#define   SW_WIDTH        320

#define   SIP_PARAM_STR_LEN   5
#define   I2C_PARAM_STR_LEN   5
#define   UART_PARAM_STR_LEN  6
#define   PWM_PARAM_STR_LEN   6

#define   MENU_NUM            7
#define   ITEM_NUM            4
#define   SUB_NUM             8
#define   PARAM_NUM           70

#define   INPUT_SUB_PARAM       (gInputSubParam[gItemParam[INTYPE]][INPUT_SUB_INDEX_NUM])
#define   INPUT_SUB_PARAM_MAX   (gInputSubParamMax[gItemParam[INTYPE]][INPUT_SUB_INDEX_NUM])
#define   INPUT_SUB_PARAM_MIN   (gInputSubParamMin[gItemParam[INTYPE]][INPUT_SUB_INDEX_NUM])
#define   INPUT_SUB_MAX_NUM     (gInSubMaxNum[gItemIndexNum[Menu.index]][gItemParam[INTYPE]])
#define   INPUT_SUB_INDEX_NUM   (gInSubIndexNum[gItemIndexNum[Menu.index]][gItemParam[INTYPE]])

#define   OUT_SUB_PARAM         (gOutSubParam[gItemParam[OUTTYPE]][OUT_SUB_INDEX_NUM])
#define   OUT_SUB_PARAM_MAX     (gOutSubParamMax[gItemParam[OUTTYPE]][OUT_SUB_INDEX_NUM])
#define   OUT_SUB_PARAM_MIN     (gOutSubParamMin[gItemParam[OUTTYPE]][OUT_SUB_INDEX_NUM])
#define   OUT_SUB_MAX_NUM       (gOutSubMaxNum[gItemIndexNum[Menu.index]][gItemParam[OUTTYPE]])
#define   OUT_SUB_INDEX_NUM     (gOutSubIndexNum[gItemIndexNum[Menu.index]][gItemParam[OUTTYPE]])

enum
{
    IN_TYPE, TIME_SET, TRIGGER, MEASURE, OUT_TYPE, FILE_CTRL, SETTING
};

enum
{
    INTYPE = 0,
    TIMEBASE = 10, T1POSI,  T2POSI,  XPOSI,    T0POSI,   ZCPOSI,
    TRITYPE = 20,  TRICOND, TRIMASK, TRIWIDTH, THRESHOLD,
    SOURCE1 = 30,  SOURCE2, SOURCE3,
    OUTTYPE = 40,
    SAVEBMP = 50,  SAVECSV, SAVESVG, SAVEBUF,   SAVEDAT, LOADDAT,  LOADBUF,
    SAVEPAM = 60,  RSTPAM,  VOLUME,  BKLIGHT,   STANDBY, POWEROFF,
};
enum
{
    TB_TB,  TB_T1,  TB_T2,  TB_X, TB_T0,  TB_ZC,
};
enum
{
    BUF_T1,  BUF_T2, BUF_T0, BUF_ZC,
};
enum
{
    TRI_None,  TRI_DiNE,  TRI_DiEQ,  TRI_Any,
    TRI_DfLE,  TRI_DtLE,  TRI_DfGT,  TRI_DtGT, };
enum
{
    IN_USER, IN_SPI, IN_I2C, IN_UART,
};
enum
{
    SPI_CS_POLAR, SPI_CLK_POLAR, SPI_CLK_PHASE, SPI_BIT_ORDER, SPI_WORD_SIZE, SPI_DATA_FORMAT
};
enum
{
    I2C_ADD_FORMAT, I2C_DATA_FORMAT
}; 
enum
{
    UART_BAUD_RATE, UART_DATA_BITS,   UART_STOP_BITS,   UART_PARITY_TYPE,
    UART_BIT_ORDER, UART_INVERT_SIGN, UART_DATA_FORMAT
};
enum
{
    OUT_PWM, OUT_SPI, OUT_I2C, OUT_UART,
};
enum
{
    PWM1_EN, PWM1_FREQ, PWM1_DUT1, PWM1_DUT2, PWM2_EN, PWM2_FREQ, PWM2_DUT1, PWM2_DUT2
};
enum
{
    BMP, CSV,
};
enum
{
    SAVEPARAM, RSTPARAM,
};


extern uc8 gItemMaxNum[], TimeUnitStr[][6], gInputSubParamBackup[][SUB_NUM], gItemIndexNumBackup[];
extern uc8 gInSubMaxNum[][ITEM_NUM], gInputSubParamMax[][SUB_NUM], gInputSubParamMin[][SUB_NUM], Run_ICO[];
extern u8  gItemIndexNum[];
extern u8  gInputSubParam[][SUB_NUM];
extern u8  gTriCond_X, gTriMask_X,gBatFlag;
extern uc16 gItemParamMax[];
extern uc16 gItemParamMin[];
extern uc16 gItemParamStep[], gItemParamBackup[];
extern u16  gItemParam[];
extern u16  gT0Posi, gT1Posi, gT2Posi, gZCPosi, gXPosi;
extern uc32 gTB_Scale[];
extern u8  gOutSubParam[][SUB_NUM],gInSubIndexNum[][ITEM_NUM],gOutSubIndexNum[][ITEM_NUM];
extern uc8 gInSubIndexNumBackup[][ITEM_NUM],gOutSubIndexNumBackup[][ITEM_NUM];
extern uc8 gOutSubParamMax[][SUB_NUM],gInSubMaxNum[][ITEM_NUM],gOutSubMaxNum[][ITEM_NUM];
extern uc8 gOutSubParamBackup[][SUB_NUM],gOutSubParamMin[][SUB_NUM];

void BackupPreCursorPosition(u8 IndexCur);
void ShowAllMenu(void);
void DrawScaleRule(u16 x0, u16 y0, u16 Length, u8 multiple);
void DrawTriangle(u16 x0, u16 y0, u16 Height, u16 Col, u8 Direction);
void DrawTriCol(u16 x0, u16 y0, u16 Height, u16 Col, u16 TriCol, u8 Direction);
void ShowSmplStatus(u8 Status);
void ShowBattery(void);
void ShowTopTitle(void);
void ShowLeftTitle(void);
void ShowPopItem(u8 MenuNum);
void ShowPopWindow(u16 x0, u16 y0, u16 Width, u16 Height, u16 WinCol, u16 BlkCol, u8 MenuIndex);
void ShowSubPop(u16 x0, u16 y0, u16 Width, u16 Height, u16 WinCol, u16 BlkCol, u8 *CharStr, u8 *ParamStr, u8 ParamStrWidth);
void ShowOutSubPopStr(u16 x0, u16 y0, u8 ItemNum, u8 ItemIndex, u8 *CharStr, u8 *ParamStr, u8 CharLen, u8 ParamLen, u8 StrStarNum, u8 StrWidth);
void ShowPopStr(u16 x0, u16 y0, u8 ItemNum, u8 ItemIndex, u8 *CharStr, u8 *ParamStr, u8 StrStarNum, u8 StrWidth);
void ClrCursor(u16 x0, u16 y0, u16 Col);
void ShowScaleRuleTime(u16 x0, u16 y0, u16 Length, u8 Multiple);
void ShowWindowPrecent(void);
void ShowTimeBaseStr(void);
void DrawCursor(u16 x0, u16 y0, u16 Col);
void ShowMeasureStr(void);
void ShowTimeMarkValue(void);
void ShowSubItem(u8 NowMenu, u8 NowItme);
void ShowSubPopStr(u16 x0, u16 y0, u8 ItemNum, u8 ItemIndex, u8 *CharStr, u8 *ParamStr, u8 CharLen, u8 ParamLen, u8 StrStarNum, u8 StrWidth);
void ShowMeasureSourceStr(void);
void DrawTriggerIco(void);
void ProcessSmallWins(void);
void ShowSmallWins(void);
void SmallWinsInit(void);
void Show_PIO_Name(u16 x, u16 y, u8 Type);
void Draw_ScaleRule(void);

u8   DrawDottedLine(u16 x0, u16 y0, u16 Width, u16 Height, u16 Col);
u8   DrawRectangle(u16 x0, u16 y0, u16 Width, u16 Height, u16 Col);
u8   DrawRecLine(u16 x0, u16 y0, u16 Width, u16 Height, u16 Col);

#endif
/*********************************  END OF FILE  ******************************/

