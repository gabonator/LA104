#ifndef __CTRL_H
#define __CTRL_H

#include "STM32F10x.h"

#define  PWM1              0
#define  PWM2              1
#define  PWM_NUM1          0
#define  PWM_NUM2          1

#define  PWROFF_TIME_UNIT  60
#define  LCD_BUF_WIDTH     281
#define  SHOW_WIDTH        280

#define  TRI_PRE           7
#define  TRI_START_NUM     10
#define  PRE_OFFSET        10
#define  TIME_TO_POINT     5
#define  TRI_POSI          2

extern u8  gXposiAdd, gXposiDec,  gRunFlag;
extern u16 gStartCnt, gZC_Posi;
extern u64 gNowX_Time;
extern u64 gLastX_Time;
extern u64 gTransTimeSum;

void TriConfig(void);
void Process(void);
void ShowWaveToLCD(void);
void X_PositionUpdate(void);
void TimeBaseScale(void);
void HW_Ctrl(u8 MenuNum, u8 ItemNum);
void ResetPowerOffTime(void);
void StandbyAndPowerOff(void);
void ShowMeasureValue(void);
void ShortcutBMP(void);
void PIO_SendData(u8 Type);
void PIO_Init(u8 Type);
void PIO_PWM_Ctrl(u8 Channel, u8 Status);
void PIO_PWM_SetDuty(u8 Channel, u8 Num, u16 Duty);
void PIO_PWM_SetFreq(u8 Channel, u16 Psc, u16 Arr);
void PIO_AllDisable(void);
void DefaultTir(u8 Type);
void KeyQuickAct(void);
void ShutdownTest(void);
u8   TransStatusInfo(u16 Num);
u8   ZC_Scale(void);
u8   TransStatusData(u16 Num);
u8   AddAnalyzeStatus(u8 Status, u16 Num);
u16  GetBatteryVol(void);
u16  Get_USB_Vol(void);
u32  TransformTime(u16 Num);
u64  UpdateXposiTime(void);
u64  CalculateTimeSum(u16 Cnt);



#endif

