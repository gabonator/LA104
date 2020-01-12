/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Draw.h  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#ifndef __DRAW_H
#define __DRAW_H

#include "system.h"

typedef struct 
{
  s16  Value;
  u8   Flag;       // HID=0x04, UPDAT=0x02,	< does not appear to be used 
} trigg; 

//------------------- 
#define TR_1       0
#define TR_2       1
#define TR_3       2
#define TR_4       3
#define VERNIE     4
#define SCRN       5
#define GRID       9
#define NOTE2      7
#define NOTE3      8
#define X_POSI     6
#define TEXT1      10
#define TEXT2      11
#define TEXT3      12
#define BLOCK      13
#define SIDE       14
#define NOTE1      15


#define SINE       0
#define TRIANG     1
#define SAW        2
#define DIGI       3
#define PWM        4

#define PRN        0   // No inverse Display
#define INV        1   // Inverse Display


#define WHT        0xFFFF
#define CYAN       0xFFE0
#define CYAN_      0xBDE0
#define YEL        0x07FF
#define YEL_       0x05F7
#define PURPL      0xF81F
#define PURPL_     0xB817
#define GRN        0x07E0
#define GRN_       0x05E0
//#define GRAY       0x7BEF    //original, dark gray
//#define GRAY       0x9DEF    //greenish gray 
//#define GRAY       0xBDF7      //bright gray   
#define GRAY       0xA534      //medium gray
#define BLUE       0xF800
#define ORANGE     0x051F
#define RED        0x001F
#define BLACK      0x0000


#define LCD_X1      0
#define LCD_X2      399
#define LCD_Y1      0
#define LCD_Y2      239
#define X_SIZE      392   //386  300  400 
#define X_BASE      0
#define Y_SIZE      200
#define Y_BASE      0
#define MIN_X       11
#define MIN_Y       13
#define TRACK_OFFSET  2     //2         
//#define MAX_X       (X_SIZE + MIN_X)	now variable, at most, with meters off=386 + min_x or 397 
#define MAX_Y       (Y_SIZE + MIN_Y)


#define CHAR_WIDTH  8
#define CHAR_HEIGHT 11

extern u16 XposRef;
extern u8 UpdateScale;
extern u16 SpecRow;
extern u8 Raw;
extern u8 PerstFrameNumber;
extern u8 XYper;
extern u8 InitXY;
extern u8 UpdateWindow;
extern u8 SpecMode;
extern u8 OffsetX;
extern u8 OffsetY;
extern u16 MAX_X;
extern u8 FlagFrameMode;
extern u8 FlagMeter;
extern u8 UpdateMeter;
extern u8 TrgAuto;
extern u8 UpdateBackground;
extern u16 Color[16];
extern u16 Count_FPS;
extern trigg V_Trigg[4];
extern s16 DetFrqn[12];
extern u32 BaudRate;
extern u8  FrameSize;
extern u8  Parity;
extern u8  DataSize;
extern u8  DataAdj;
extern u8  ValidFrame;
extern u8  SpiMode;
extern u8  SpiBitOrder;
extern s16 SpiNumBits;
extern u8  SpiAdj;
extern u8  SpiChartFlag;
extern u8  ArrayIndex;
extern u8  OSBuffer;
extern u8  DigChLockout;
extern u8  Hbold;
extern u8  CursorDisplaySelect;
extern u8  DisableCursorDisplay;
extern u8  EnableCursorDisplay;

u16  Get_TAB_8x11(u8 Code, u8 Row);
void Print_Str(u16 x0, u16 y0, u16 Color, u8 Mode, char *s);
void Print_Str2(u16 x0,u16 x1,u16 y0, u16 Type, u8 Mode, char *s,u8 LeadingBlanks,u8 TrailingBlanks);
void Draw_Row(u16 Row);
void Draw_Window(void);						
void LoadBaseBuffers(void);

void Draw_Mark(u16 m, u16 n);						
void Update_Mark(void);						
void Update_View_Area(void);
void Clear_Meter_Area(void);
void ClearFFTbuffer(void);
void ClearScreenArea(u16 x1,u16 x2,u8 y1,u8 y2);
u8   UartLogic(void);
u8   i2cLogic(void);
u8   SpiLogic(void);
u16  IndexPosition(s16 Position,u8 service);
void PrintDir(u16 Row, u8 buffer,u8 Mode);
void UpdateMarkLogic(void);

#endif

/********************************* END OF FILE ********************************/
