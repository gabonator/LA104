	//******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
// File Name : Draw.c       
// Version   : DS203_APP Ver 2.3x                                  Author : bure
//*******************************************************************************/
#include <string.h>
#include "Interrupt.h"
#include "Process.h"
#include "Draw.h"
#include "Menu.h"
#include "BIOS.h"
#include "Function.h"
#include "File.h"
#include "platform.h"

void LoadBuffer(u16 Row, u8 buffer,u8 *y,u8 *Dot_Hide,u8 service);	
void LoadFFTbuffer(u16 Row,u8 buffer);
void Print_Str_Row(u16 Row, u16* LCD_Buffer, u16 x0, s16 y0, char *s);
void DisplayFFtValues(u16 Row,u8 buffer);
void DisplayDbScale(u16 Row, u8 buffer, u8 service);
void Base1(u8 buffer);
void LoadBaseBuffers(void);       //load all 3 buffers
void TraceShadow(u8 start,u8 limit,u8 buffer);
void VernierMark(u8 buffer,u16 color);
void LoadXYBuffer(u16 Row);
void ClearFFTbuffer(void);
s32 ScaleXposi(void);
void NFreqParse(u8 index,u8 suffix);
u16 ColorIndexGen(u8 level);
void DrawPixel(u16 x,u8 y, u16 color);
void DrawBase(u16 x,u16 color);
void DisplayDetFrqn(u16 Row, u8 buffer);
void Uart();
void PrintUart(u16 Row, u8 buffer, u8 service);
void i2c(void);
void LoadChars(u8 service);
void InitiateCharArrays(void);
void Spi(void);
void SpiChart(u16 Row,u8 buffer);
u8   BitReadPosition(u16 i);
u8   DigChLockout=0;
void PrintText(u16 x,u8 y,char *s);
void PrintConfigName(void);
void DisplayCursorValues(u16 Row, u8 buffer);
u8   DisableCursorDisplay=0;
u8   EnableCursorDisplay=0;
void DisplayDeltaValues(u16 Row, u8 buffer);

char Nfreq[9];
char Nsuffix[3];
u16 MAX_X;
uc16 RULE_BASE[8] ={0x020,0x040,0x080,0x040,0x020,0x010,0x008,0x010};
u8 OffsetX;
u8 OffsetY;
s16 FactorX;
s16 FactorY;
s16 FactorY13;
u16 SpecRow = 25;
u8 SpecLine = 0;
s32 Tmp2;
s16 skip;
u8  UpdateBackground;
u8  Rdiscard=0;				//post-signal trace blanking
char DbScaleStr[3];
//uc16 COLOR_INDEX=0x050F;		//red fft values display
u16 COLOR_INDEX=0x0503;		        //green fft values display             
                                        //0x0500 = cyan w black bg
                                        //0x0501 = yellow w black bg
u16 Count_FPS = 0;
u8 SpecMode=0;
u8 InitXY=1;
u8 XYper=0;
u8 FrameCount;
u8 ClearPerst;
u8 PerstFrameNumber=0;
u8 Raw=0;
u8 UpdateScale;
s16 DetFrqn[12];
uc8 DetFrqAdj[10]= {110,108,106,104,104,104,104,104,104,104};
u16 RowMem, RowMemPeak;
u16 XYLimit=234;
u8  ListBottom;

u32  BaudRate;
u8   FrameSize=10;
u8   DataSize=8;
u8   Parity=0;
char AscChar[2][3][17];
char AscCharB[2][3][17];
char HexChar[6][49];
char HexCharB[6][49];
u8   ArrayIndex;
u16  BitDuration;
u8   Mask=1;
u8   DataAdj=0;
u8   ValidFrame;
typedef struct{
  u8  Data;
  u16 Index;
  u16 End;
  u8  DataB;
  u8  BitsPerWord;
}ByteInfo;
ByteInfo ByteArray[96];
u8 SpiMode=0;
u8 SpiBitOrder=0;
s16 SpiNumBits=8;
u8 SpiAdj=0;
u8 WordEndFlag=0;
u8 SpiChartFlag=0;
u16 ClockPeriod=0xFFFF;
s16 ChartIndex=0x7FFF;
u16 TcurDelta=30;
u8  OSBuffer=0;
u8  Hbold=0;
u16 Limit=299;
u8  SkipFirstRow=0;
u8  CursorDisplaySelect=0;

const char CursorTypeStr[5][4]={{'T','1',':',0},{'T','2',':',0},{'V','1',':',0},{'V','2',':',0},{'T','r',':',0}};

uc8  Mark_TAB_1[7] ={0x00,0x00,0x42,0xFE,0x02,0x00,0x00};      // Mark 1
uc8  Mark_TAB_2[7] ={0x00,0x46,0x8A,0x92,0x92,0x62,0x00};      // Mark 2
uc8  Mark_TAB_3[7] ={0x00,0x44,0x82,0x92,0x92,0x6C,0x00};      // Mark 3
uc8  Mark_TAB_4[7] ={0x00,0x18,0x28,0x4A,0xFE,0x0A,0x00};      // Mark 4
uc8  Mark_TAB_T[7] ={0x00,0x40,0x40,0x7E,0x40,0x40,0x00};      // Mark T

uc8  Mark_TAB[5][7] ={{0x00,0x00,0x42,0xFE,0x02,0x00,0x00},    // Mark 1
                      {0x00,0x46,0x8A,0x92,0x92,0x62,0x00},    // Mark 2
                      {0x00,0x44,0x82,0x92,0x92,0x6C,0x00},    // Mark 3
                      {0x00,0x18,0x28,0x4A,0xFE,0x0A,0x00},    // Mark 4
                      {0x00,0x40,0x40,0x7E,0x40,0x40,0x00}};   // Mark T

  uc16 FrameCountRef[22]= {18000,9000,3600,1800,900,362,182,92,     49,   29,   15,   12,    12,12,12,12,12,12,12,12,12,12};   
//                           1   500   200  100  50  20  10  5       2     1    500   200			      time base	

//uc16 Char_TAB_8x11[744] = {
//uc16 Char_TAB_8x11[752] = {                      // used when adding extra character 			 	
//uc16 Char_TAB_8x11[747] = {                      // used when adding 3 pix wide period 			 	
  uc16 Char_TAB_8x11[768] = {                      // used when adding 3 pix period and extra char
  0x000,0x000,0x000,0x780,0x040,0x020,0x020,0x020, // "the upper left corner
//  0x020,0x020,0x040,0x780,0x000,0x000,0x000,0x000, //  # the upper right corner
  0x1FC,0x1FC,0x1FC,0x1FC,0x1FC,0x1FC,0x1FC,0x000, // # box
//  0x000,0x000,0x000,0x00F,0x010,0x020,0x020,0x020, // $ lower left corner
//  0x070,0x0F8,0x1FC,0x1FC,0x1FC,0x0F8,0x070,0x000, // $ dot
  0x006,0x00C,0x018,0x030,0x060,0x0C0,0x180,0x000, // $ backslash 
  0x30C,0x18C,0x0C0,0x060,0x030,0x318,0x30C,0x000, // % 
  0x000,0x180,0x260,0x21C,0x26A,0x284,0x140,0x000, // & 
  0x202,0x202,0x202,0x202,0x202,0x202,0x3FE,0x000, // ' battery Last empty
  0x000,0x000,0x0F8,0x1FC,0x306,0x202,0x000,0x000, // (
  0x000,0x000,0x202,0x306,0x1FC,0x0F8,0x000,0x000, // )
  0x000,0x18C,0x0D8,0x070,0x070,0x0D8,0x18C,0x000, // * x
  0x000,0x020,0x020,0x0F8,0x0F8,0x020,0x020,0x000, // + 
  0x020,0x020,0x010,0x00F,0x000,0x000,0x000,0x000, // , the lower right corner
  0x000,0x020,0x020,0x020,0x020,0x020,0x020,0x000, // -
  0x000,0x000,0x300,0x300,0x000,0x000,0x000,0x000, // .
  0x180,0x0C0,0x060,0x030,0x018,0x00C,0x006,0x000, // / 
  0x1FC,0x3FE,0x242,0x222,0x212,0x3FE,0x1FC,0x000, // 0
  0x000,0x208,0x20C,0x3FE,0x3FE,0x200,0x200,0x000, // 1
  0x304,0x386,0x2C2,0x262,0x232,0x31E,0x30C,0x000, // 2
  0x104,0x306,0x222,0x222,0x222,0x3FE,0x1DC,0x000, // 3
  0x060,0x070,0x058,0x24C,0x3FE,0x3FE,0x240,0x000, // 4
  0x11E,0x21E,0x212,0x212,0x212,0x3F2,0x1E2,0x000, // 5
  0x1F8,0x3FC,0x226,0x222,0x222,0x3E0,0x1C0,0x000, // 6
  0x006,0x006,0x3C2,0x3E2,0x032,0x01E,0x00E,0x000, // 7
  0x1DC,0x3FE,0x222,0x222,0x222,0x3FE,0x1DC,0x000, // 8
  0x01C,0x23E,0x222,0x222,0x322,0x1FE,0x0FC,0x000, // 9
  0x000,0x000,0x000,0x198,0x198,0x000,0x000,0x000, // : 
  0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA, // ; battery body
  0x000,0x020,0x070,0x0D8,0x18C,0x306,0x202,0x000, // <
  0x090,0x090,0x090,0x090,0x090,0x090,0x000,0x000, // =  
  0x000,0x202,0x306,0x18C,0x0D8,0x070,0x020,0x000, // >
  //0x018,0x01C,0x004,0x344,0x364,0x03C,0x018,0x000, // ?
  //0x07F,0x008,0x0D4,0x322,0x441,0x300,0x0C0,0x000, // ? Kv
  0x070,0x0F8,0x1FC,0x1FC,0x1FC,0x0F8,0x070,0x000, // ? dot
  //0x1F8,0x104,0x272,0x272,0x2F2,0x284,0x078,0x000, // @ 
  0x3FE,0x020,0x3FE,0x000,0x320,0x2A0,0x260,0x000, // @		Hz
  0x3F0,0x3F8,0x04C,0x046,0x04C,0x3F8,0x3F0,0x000, // A
  0x202,0x3FE,0x3FE,0x222,0x222,0x3FE,0x1DC,0x000, // B
  0x0F8,0x1FC,0x306,0x202,0x202,0x306,0x18C,0x000, // C
  0x202,0x3FE,0x3FE,0x202,0x306,0x1FC,0x0F8,0x000, // D
  0x202,0x3FE,0x3FE,0x222,0x272,0x306,0x38E,0x000, // E
  0x202,0x3FE,0x3FE,0x222,0x072,0x006,0x00E,0x000, // F
  0x0F8,0x1FC,0x306,0x222,0x222,0x1E6,0x3EC,0x000, // G
  0x3FE,0x3FE,0x020,0x020,0x020,0x3FE,0x3FE,0x000, // H
  0x000,0x000,0x202,0x3FE,0x3FE,0x202,0x000,0x000, // I
  0x1C0,0x3C0,0x200,0x202,0x3FE,0x1FE,0x002,0x000, // J
  0x202,0x3FE,0x3FE,0x030,0x0F8,0x3CE,0x306,0x000, // K
  0x202,0x3FE,0x3FE,0x202,0x200,0x200,0x300,0x000, // L
  0x3FE,0x3FE,0x01C,0x038,0x01C,0x3FE,0x3FE,0x000, // M
  0x3FE,0x3FE,0x01C,0x038,0x070,0x3FE,0x3FE,0x000, // N
  0x0F8,0x1FC,0x306,0x202,0x306,0x1FC,0x0F8,0x000, // O
  0x202,0x3FE,0x3FE,0x222,0x022,0x03E,0x01C,0x000, // P
  0x1FC,0x3FE,0x202,0x282,0x302,0x3FE,0x1FC,0x000, // Q
  0x202,0x3FE,0x3FE,0x022,0x062,0x3FE,0x39C,0x000, // R
  0x10C,0x31E,0x232,0x222,0x262,0x3CE,0x18C,0x000, // S
  0x000,0x00E,0x206,0x3FE,0x3FE,0x206,0x00E,0x000, // T
  0x1FE,0x3FE,0x200,0x200,0x200,0x3FE,0x1FE,0x000, // U
  0x07E,0x0FE,0x180,0x300,0x180,0x0FE,0x07E,0x000, // V
  0x0FE,0x3FE,0x380,0x1E0,0x380,0x3FE,0x0FE,0x000, // W
  0x306,0x3DE,0x0F8,0x020,0x0F8,0x3DE,0x306,0x000, // X
  0x000,0x01E,0x23E,0x3E0,0x3E0,0x23E,0x01E,0x000, // Y
  0x38E,0x3C6,0x262,0x232,0x31E,0x38E,0x000,0x000, // Z
  0x200,0x300,0x2C0,0x220,0x218,0x204,0x3FE,0x000, // [ Triangle       
  0x000,0x022,0x042,0x1FE,0x3FE,0x240,0x220,0x000, // \ falling edge   
  0x020,0x010,0x008,0x006,0x008,0x010,0x020,0x020, // ] Pointer   
  0x000,0x220,0x210,0x3FC,0x3FE,0x012,0x022,0x000, // ^ rising edge
  0x000,0x200,0x200,0x200,0x200,0x200,0x200,0x000, // _ under the dash
  0x202,0x202,0x202,0x202,0x202,0x202,0x202,0x202, // ` battery itself empty
  0x1C0,0x3E8,0x228,0x228,0x1F8,0x3F0,0x200,0x000, // a         
  0x202,0x3FE,0x1FE,0x220,0x220,0x3E0,0x1C0,0x000, // b 
  0x1E0,0x3F0,0x210,0x210,0x210,0x330,0x120,0x000, // c 
  0x000,0x1C0,0x3E0,0x220,0x222,0x3FE,0x3FE,0x000, // d 
  0x1E0,0x3F0,0x250,0x250,0x250,0x370,0x160,0x000, // e 
  0x000,0x220,0x3FC,0x3FE,0x222,0x002,0x004,0x000, // f
  0x130,0x378,0x248,0x248,0x3F8,0x1F8,0x000,0x000, // g
  0x202,0x3FE,0x3FE,0x020,0x010,0x3F0,0x3E0,0x000, // h
  0x000,0x000,0x200,0x3F6,0x3F6,0x200,0x000,0x000, // i 
  0x000,0x100,0x300,0x210,0x3F6,0x1F6,0x000,0x000, // j
  0x202,0x3FE,0x3FE,0x062,0x0F0,0x398,0x308,0x000, // k
  0x000,0x000,0x202,0x3FE,0x3FE,0x200,0x000,0x000, // l
  0x3F8,0x3F8,0x018,0x3F0,0x018,0x3F8,0x3F0,0x000, // m 
  0x008,0x3F8,0x3F0,0x008,0x008,0x3F8,0x3F0,0x000, // n 
  0x000,0x1F0,0x3F8,0x208,0x208,0x3F8,0x1F0,0x000, // o
  0x208,0x3F8,0x3F0,0x248,0x048,0x078,0x030,0x000, // p 
  0x030,0x078,0x048,0x248,0x3F8,0x3F8,0x200,0x000, // q
  0x208,0x3F8,0x3F0,0x218,0x008,0x018,0x030,0x000, // r 
  0x000,0x110,0x338,0x268,0x248,0x3D8,0x190,0x000, // s 
  0x010,0x010,0x1F8,0x3FC,0x210,0x310,0x100,0x000, // t
  0x1F8,0x3F8,0x200,0x200,0x1F8,0x3F8,0x200,0x000, // u 
  0x000,0x0F8,0x1F8,0x300,0x300,0x1F8,0x0F8,0x000, // v
  0x0F8,0x3F8,0x300,0x180,0x300,0x3F8,0x0F8,0x000, // w
  0x208,0x318,0x1B0,0x0E0,0x1B0,0x318,0x208,0x000, // x
  0x038,0x278,0x240,0x240,0x240,0x1F8,0x0F8,0x000, // y
  0x318,0x388,0x2C8,0x268,0x238,0x318,0x000,0x000, // z 
  0x0F8,0x088,0x38E,0x022,0x2FA,0x2FA,0x2FA,0x2FA, // { battery head
  0x000,0x000,0x000,0x3FE,0x3FE,0x000,0x000,0x000, // |
  0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x202,0x3FE,0x000, // } battery last
  0x0F8,0x088,0x38E,0x202,0x202,0x202,0x202,0x202, // ~ battery empty head 
  //0x300,0x300,0x000};                              // 3 pix wide proportional period (#127) 
  0x300,0x300,0x000,0x000,0x000,0x000,0x000,0x000, // 3 pix wide proportional period (#127) 
  0x03C,0X00A,0x03C,0x000,0x3E0,0x220,0x1C0,0x000, //"ad" i2c address word id  (#128)                              
  0x000,0x100,0x200,0x7FC,0X200,0X100,0X000,0X000};// down arrow (#129) 
 
 //0x000,0x004,0x002,0x1FF,0X002,0X004,0X000,0X000};// up arrow (#129) 

char HexLookup[3];

sc8 DbScale[29]=
{-40,-37,-35,-33,-32,-31,-30,-29,-28,-27,-27,-26,-25,-25,-24,-24,-23,-23,-22,-22,
-21,-21,-21,-20,-20,-20,-20,-19,-19};
 
u16 Color[16] = { CYAN,   // #0  TRACK1 
                  YEL,    // #1  TRACK2
                  PURPL,  // #2  TRACK3 
                  GRN,    // #3  TRACK4 
                  WHT,    // #4  VERNIE   
                  BLACK,  // #5  SCRN
                  ORANGE, // #6  X_POSI 
                //BLUE,   // #7  TRIGG 
                  PURPL,  // #7  TRIGG 
                  CYAN,   // #8  VERNIE  
                  GRAY,   // #9  GRID 
                  WHT,    // #10 TEXT 
                //GRN,    // #11 TEXT2
             //0x47E8,    // #11 TEXT2 (bright green with 1/4 blue and red
               0x87F0,    // #11 TEXT2 (bright green with 1/2 blue and red
                  BLUE,   // #12 TEXT3
                //GRAY,   // #13 BLOCK
                0x7BEF,   // #13 DARK GRAY 
                  YEL_,    // #14 SIDE
                  RED };  // #15 NOTE 

trigg V_Trigg[4] = {// Value,  Flag:( HID=0x04, UPDAT=0x02 )
                     {   175,  UPDAT + HID },
                     {   116,  UPDAT       },
                     {    75,  UPDAT + HID },
                     {    35,  UPDAT + HID },
                   };

u16 LCD_Buffer[2][240];
u16 Base4Buffer[201];
u16 Base3Buffer[201];
u16 Base2Buffer[201];

/*******************************************************************************
 Get_TAB_8x11
*******************************************************************************/
u16 Get_TAB_8x11(u8 Code, u8 Row)
{
  return Char_TAB_8x11[((Code-0x22)*8)+Row];
}
/*******************************************************************************
 Print_Str: 
*******************************************************************************/
void Print_Str(u16 x0, u16 y0, u16 Type, u8 Mode, char *s)
{ 
  signed short i, j, b; 
  __LCD_Set_Block(x0, LCD_X2, y0, y0+10);

  if(SkipFirstRow==0){
    for (j=0; j<11;++j){ 
      if(Mode == 0) __LCD_SetPixl(Color[Type & 0x0F]);   //Normal replace Display
      else          __LCD_SetPixl(Color[Type >> 0x8]);   //Inverse replace Display
    }
    x0++;                            // a string to add a blank row
  }

  while (*s!=0) {
    for(i=0;i<8;++i){
      if((*s==0x20)||(*s==0x21)) b = 0x0000;
      else                       b = Get_TAB_8x11(*s, i);
      if((*s==0x21)&&(i==4)) break;
      for(j=0;j<11;++j){
        if((b << j)& 0x400) {
          if(Mode == 0) __LCD_SetPixl(Color[Type >> 0x8]);            //0=PRN, 1=INV, Twink, i=0-3
          else          __LCD_SetPixl(Color[Type & 0x0F]);
        } else {
          if(Mode == 0) __LCD_SetPixl(Color[Type & 0x0F]);
          else          __LCD_SetPixl(Color[Type >> 0x8]);
        }
      }
    }
    if(*s==0x21) x0 +=4;           // display position horizontally +4 ("!" character - shift by 1/2 a character)
    else  x0 += 8;                 // display position horizontally +8
    ++s;                           // string pointer +1
  }
  __LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  // restore the full-size window
}

void PrintConfigName(void){
char Str[2]={0,0};
u8 i;
u8 j=0;

  if((_4_source!=SPEC_A)&&(_4_source!=SPEC_B)&&(ListOverride)){
    for(i=204;i>126;i-=11){
      Str[0]=ConfigFileName[j++];                                        //print config name   
      PrintText(0,i,Str);
    }
  }
  __LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  // restore the full-size window
}


void PrintText(u16 x, u8 y,char *s){
u8 i;
u16 j,b;

  __LCD_Set_Block(x,x+6,y,y+10);

  while (*s!=0) {
    for(i=0;i<7;++i){
      if((*s==32)||(*s==95)) b = 0x0000;
      else                   b = Get_TAB_8x11(*s, i);

      for(j=0;j<11;++j){
        if((b << j)& 0x400) {
          __LCD_SetPixl(WHT);     
        } else {
          __LCD_SetPixl(BLACK);
        }
      }

    }
    x += 8;                        // display position horizontally +8
    ++s;                           // string pointer +1
  }

}


/*******************************************************************************
Print_Str2: 
*******************************************************************************/
void Print_Str2(u16 x0,u16 x1,u16 y0, u16 Type, u8 Mode, char *s,u8 LeadingBlanks,u8 TrailingBlanks)
{
u8 LeadingLine=0;

	signed short i, j, b; 
	u8 xm, ym;
	__LCD_Set_Block(x0, x1, y0, y0+21);

      if(LeadingBlanks>10){		//special case to add 1 leading line
        LeadingBlanks-=10; 
        LeadingLine=1;
      } 

      for (i=0;i<LeadingBlanks;i++){
	for (j=0;j<22;j++){ 
	  __LCD_SetPixl(BLACK);   //Normal replace Display
	}
	x0++;                            // a string to add a blank row
      }

    if(LeadingLine){
      for (j=0; j<44;++j){ 
        if(Mode == 0) __LCD_SetPixl(Color[Type & 0x0F]);   //Normal replace Display
        else          __LCD_SetPixl(Color[Type >> 0x8]);   //Inverse replace Display
      }
      x0+=2;                            // a string to add a blank row
    }
	while (*s!=0) {
		for(i=0;i<8;++i){
			if((*s==0x20)||(*s==0x21)) b = 0x0000;
			else                       b = Get_TAB_8x11(*s, i);
				if((*s==0x21)&&(i==4)) break;
			for(xm=0;xm<2;++xm){
				for(j=0;j<11;++j){
					if((b << j)& 0x400) {
						if(Mode == 0) for(ym=0;ym<2;++ym) __LCD_SetPixl(Color[Type >> 0x8]);
						else          for(ym=0;ym<2;++ym) __LCD_SetPixl(Color[Type & 0x0F]);
					} else {
						if(Mode == 0) for(ym=0;ym<2;++ym) __LCD_SetPixl(Color[Type & 0x0F]);
						else          for(ym=0;ym<2;++ym) __LCD_SetPixl(Color[Type >> 0x8]);
					}
				}
			}
		}
		if(*s==0x21) x0 +=8;           // display position horizontally +4 ("!" character - shift by 1/2 a character)
		else  x0 += 16;                 // display position horizontally +8
		++s;                           // string pointer +1
	}

      for (i=0;i<TrailingBlanks;i++){
	for (j=0;j<22;j++){ 
          __LCD_SetPixl(BLACK);   //Normal replace Display
	}
	x0++;                            // a string to add a blank row
      }
      //__LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  		// restore the full-size window > Now done externally
}



/*******************************************d************************************
 Draw_Row : to ease the DMA conflict, in two buffers alternately
*******************************************************************************/
void Draw_Row(u16 Row)
{ 
  u8  i, y[8], Dot_Hide[8];       //,Zm=0,Zn=0; 
  s16 Tmp, m, n;
  
 if((_T_base<11)&&((ClearLeadingEdge)||(_Mode==NORHLD))){
   if(OsBufferLogicFlag){
     skip = MIN_X+JumpCnt-(StartOffset+1);  
   }else{
     skip = MIN_X+JumpCnt-_X_posi.Value;  
     if(OsChartFlag)skip--;
   }
 }else skip=0x7FFF; 

 if(ChartLogic())ChartIndex=(JumpCnt+11)-_X_posi.Value;else ChartIndex=0x7FFF;

if((Row > MIN_X)&&(Row <= MAX_X)){                  // waveform display data preprocessing
    m = (Row - MIN_X-1)* 4;
    n = (Row - MIN_X)  * 4;

    for(i = 0; i < 8; i += 2) {
      if( ((OsBufferLogicFlag)||(OsChartFlag))  &&(i>2))break;          // Digital channels cannot be displayed in OS mode          
      Dot_Hide[i] = 0;
      y[i]   = TrackBuff[m + i/2];                  // endpoint to extract
      y[i+1] = TrackBuff[n + i/2];

      if(y[i]>199)y[i]= 199;                        // bounds   
      else if(y[i]<1)y[i]=1;                               
      if(y[i+1]>199)y[i+1]=199;
      else if(y[i+1]<1)y[i+1]=1;
     
      if(y[i] == y[i+1]){
        if((y[i] == Y_BASE+1)||(y[i] == Y_SIZE-1)){                
          if(  (((!OsBufferLogicFlag)||(OSAvg))&&(!OsChartFlag))  ||(y[i]==TrackBuff[m + 2+(i/2)])||
            ((y[i]==1)&&(TrackBuff[m + 2+(i/2)]==0)))Dot_Hide[i] = 1;                //regular mode or not being drawn down by OS data 
        }else{                                                                       // else if((FPGAosFlag==0)||(!OsBufferLogicFlag)){
          if((y[i] >= Y_BASE+2)&&(Hbold<1))           y[i]   -= 1;                   // alt config, allows OS display bold on display 2 & 3
          if((y[i+1] <= Y_BASE+Y_SIZE-2)&&(Hbold<2))  y[i+1] += 1;                   // non OS modes: 1, 2 or 3 bold lines
          //if((y[i] >= Y_BASE+2)&&(Hbold<2))           y[i]   -= 1;                 // OS modes: bold only on 3                   
          //if((y[i+1] <= Y_BASE+Y_SIZE-2)&&(Hbold<1))  y[i+1] += 1;                    
          //if((y[i+1] <= Y_BASE+Y_SIZE-2)&&((Hbold<1)||((OsBufferLogicFlag)&&(OS_Bold))))y[i+1] += 1;//Hyst OS_Bold flag for dir os bold on 2                    
        }
      }

      if(y[i] > y[i+1]){                                           // order of
        Tmp = y[i+1]; y[i+1]= y[i]; y[i]= Tmp;                       
      } 
      if(  ( ((OsBufferLogicFlag)&&(!OSAvg))||(OsChartFlag) )  &&(Row>(MIN_X+1))){
        if(TrackBuff[m + 2+(i/2)]<TrackBuff[n + 2+(i/2)])y[i]=TrackBuff[m + 2+(i/2)]; 
          else y[i]=TrackBuff[n + 2+(i/2)];                                              
      }

    }
} 

  if((_Mode!=NORHLD)||((_State.Value==0)&&((Update)||(UpdateBackground==3)||(ClearPerst==2)))){

  __Row_DMA_Ready();
  __Point_SCR(Row, MIN_Y);
  
if(Row & 1){                                       // Odd row process
//----------------------- Fill the row base data -------------------------------
    __Row_Copy(Base2Buffer, LCD_Buffer[0]);
//------------------------- Draw the Curve data --------------------------------
  if((Row > MIN_X)&&(Row < MAX_X)){
    LoadBuffer(Row,1,y,Dot_Hide,1);
//------------------------- Draw the Trigg Vernie data -------------------------
  if (_Mode!=SCAN){ 

      for(i=0;i<4;i++){
        if((Title[TRIGG][SOURCE].Value == i)&&(Title[i][SOURCE].Value != HIDE)) 
          LCD_Buffer[1][V_Trigg[i].Value] |= Color[i];
      }

      if(Title[TRIGG][SOURCE].Value == 4){                    //display both trigger verniers in alt mode
        LCD_Buffer[1][V_Trigg[TRACK1].Value] |= Color[0];
        LCD_Buffer[1][V_Trigg[TRACK2].Value] |= Color[1];
      }

//------------------------- Draw the X Vernie data -----------------------------
      Tmp2=ScaleXposi();

     if(Tmp2 > MIN_X) {
        if((Row == Tmp2)&&((_X_posi.Flag & HID)== 0)){				
          for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[1][i] |= ORANGE;       //X vernier
        }
        if((Row == Tmp2-1)||(Row == Tmp2+1)){				      //mid portion for marks, aligned with borders, 2 outside pix	
          LCD_Buffer[1][Y_SIZE] = ORANGE;
          LCD_Buffer[1][Y_BASE] = ORANGE;
        }
        if(Row == Tmp2)VernierMark(1,ORANGE);
      }
  }//if mode !scan    

      Tmp = MIN_X + Title[T_VERNIE][T1].Value;
      if((Row == Tmp)&&((Title[T_VERNIE][T1].Flag & HID)== 0)){		      	
        for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[1][i] |= WHT;         //T1 vernier
      }
      if((Row == Tmp-1)||(Row == Tmp+1)){				      //mark mid portions minus mid	
        LCD_Buffer[1][Y_SIZE] = WHT;
        LCD_Buffer[1][Y_BASE] = WHT;
      }
      if(Row == Tmp)VernierMark(1,WHT);
      Tmp = MIN_X + Title[T_VERNIE][T2].Value;
      if((Row == Tmp)&&((Title[T_VERNIE][T2].Flag & HID)== 0)){
        for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[1][i] |= WHT;
      }
      if((Row == Tmp-1)||(Row == Tmp+1)){
        LCD_Buffer[1][Y_SIZE] = WHT;
        LCD_Buffer[1][Y_BASE] = WHT;
      }
      if(Row == Tmp)VernierMark(1,WHT);
    }else{	//if row>min x & < max x					//CHANGED 
//------------------------- Draw the Y Vernie data -----------------------------

      for(Tmp=-1;Tmp<2;Tmp++)LCD_Buffer[1][Title[V_VERNIE][V1].Value+Tmp] |= WHT;
      for(Tmp=-1;Tmp<2;Tmp++)LCD_Buffer[1][Title[V_VERNIE][V2].Value+Tmp] |= WHT;
      }    
      if((((Det&1)&&(_1_source))||((Det&2)&&(_2_source)))&&(ShowFFT==0))DisplayDbScale(Row,1,1);
      LoadFFTbuffer(Row,1);
      if(Sweep==1)DisplayDetFrqn(Row,1);else EnableMeterCalc=0;
      if(!((Current==OUTPUT)&&(_Det==1)&&(_Kind==8)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC)))){
        if(UartLogic())PrintUart(Row,1,0);
        if(i2cLogic())PrintUart(Row,1,1);
        if(SpiLogic())PrintUart(Row,1,2);
      }

      if((Row<192)&&(ShowFFT==0)&&(((CursorDisplayTimer)||(FlagMeter))&&(EnableCursorDisplay)))
          DisplayCursorValues(Row,1);      
      if((Row>311)&&(CursorDisplayTimer))
          DisplayDeltaValues(Row,1);

      if ((Current==9)&&(_Det==3))PrintDir(Row,1,0);
      __LCD_Copy(LCD_Buffer[1], Y_SIZE+1);               // Odd row Transitive

} else {                                           // Even row process

//----------------------- Fill the row base data -------------------------------

    if(Row+1 == MAX_X)Base1(1);
    else if(Row+1 == MIN_X);
    else if((Row+1 - MIN_X)%30 == 0)       __Row_Copy(Base4Buffer, LCD_Buffer[1]);
    else if((Row+1 - MIN_X)%6  == 0)     __Row_Copy(Base3Buffer, LCD_Buffer[1]);
    else                                 __Row_Copy(Base2Buffer, LCD_Buffer[1]);


//------------------------- Draw the Y Vernie data -----------------------------
    if((Row==MIN_X+1)||(Row==MAX_X-1)){
        LCD_Buffer[0][Title[V_VERNIE][V1].Value] |= WHT;
        LCD_Buffer[0][Title[V_VERNIE][V2].Value] |= WHT;
    }
      if((Title[V_VERNIE][V1].Flag & HID)== 0) 
        LCD_Buffer[0][Title[V_VERNIE][V1].Value] |= WHT;
      if((Title[V_VERNIE][V2].Flag & HID)== 0) 
        LCD_Buffer[0][Title[V_VERNIE][V2].Value] |= WHT;
//------------------------- Draw the Curve data --------------------------------
   if((Row > MIN_X)&&(Row < MAX_X))LoadBuffer(Row,0,y,Dot_Hide,1);
//------------------------- Draw the X Vernie data -----------------------------
   if (_Mode!=SCAN){
      Tmp2=ScaleXposi();

      if(Tmp2 > MIN_X) {
        if((Row == Tmp2)&&((_X_posi.Flag & HID)== 0)){
          for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[0][i] |= ORANGE;
        }
        if((Row == Tmp2-1)||(Row == Tmp2+1)){
          LCD_Buffer[0][Y_SIZE] = ORANGE;
          LCD_Buffer[0][Y_BASE] = ORANGE;
        }
        if(Row == Tmp2)VernierMark(0,ORANGE);
      }
   } 

      Tmp = MIN_X + Title[T_VERNIE][T1].Value;
      if((Row == Tmp)&&((Title[T_VERNIE][T1].Flag & HID)== 0)){
        for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[0][i] |= WHT;
      }
      if((Row == Tmp-1)||(Row == Tmp+1)){
        LCD_Buffer[0][Y_SIZE] = WHT;
        LCD_Buffer[0][Y_BASE] = WHT;
      }
      if(Row == Tmp)VernierMark(0,WHT);
      Tmp = MIN_X + Title[T_VERNIE][T2].Value;
      if((Row == Tmp)&&((Title[T_VERNIE][T2].Flag & HID)== 0)){
        for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[0][i] |= WHT;
      }
      if((Row == Tmp-1)||(Row == Tmp+1)){
        LCD_Buffer[0][Y_SIZE] = WHT;
        LCD_Buffer[0][Y_BASE] = WHT;
      }
      if(Row == Tmp)VernierMark(0,WHT);

     if((((Det&1)&&(_1_source))||((Det&2)&&(_2_source)))&&(ShowFFT==0))DisplayDbScale(Row,0,1);
     LoadFFTbuffer(Row,0);
     if(Sweep==1)DisplayDetFrqn(Row,0);else EnableMeterCalc=0;
     if(!((Current==OUTPUT)&&(_Det==1)&&(_Kind==8)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC)))){
       if(UartLogic())PrintUart(Row,0,0);
       if(i2cLogic())PrintUart(Row,0,1);
       if(SpiLogic())PrintUart(Row,0,2);
     }

     if((Row<192)&&(ShowFFT==0)&&(((CursorDisplayTimer)||(FlagMeter))&&(EnableCursorDisplay)))
         DisplayCursorValues(Row,0);
     if((Row>311)&&(CursorDisplayTimer))
          DisplayDeltaValues(Row,0);

     if ((Current==9)&&(_Det==3))PrintDir(Row,0,0);
     __LCD_Copy(LCD_Buffer[0], Y_SIZE+1);             // Even row Transitive

  }//even row process

 }else if((_State.Value==0)&&(Row > MIN_X)&&(Row < MAX_X)){	//persistence mode
    if((_T_base>10)||(Row!=skip)){
      if(ClearPerst!=1)LoadBuffer(Row,0,y,Dot_Hide,0);          //ClearPerst==1 denotes variable timer to display after loading X frames
      __Point_SCR(Row, MIN_Y);
    }
    LoadFFTbuffer(Row,0);
 }  
}

/***************u16 Color[16] = { CYAN,   // #0  TRACK1 
                  YEL,    // #1  TRACK2
                  PURPL,  // #2  TRACK3 
                  GRN,    // #3  TRACK4 
                  WHT,    // #4  VERNIE   
                  BLACK,  // #5  SCRN
                  ORANGE, // #6  X_POSI 
                  BLUE,   // #7  TRIGG 
                  CYAN,   // #8  VERNIE  
                  GRAY,   // #9  GRID 
                  WHT,    // #10 TEXT 
                  GRN,    // #11 TEXT2
                  BLUE,   // #12 TEXT3
                  GRAY,   // #13 BLOCK
                  YEL,    // #14 SIDE
                  RED };  // #15 NOTE 
****************************************************************
 Draw_Window :  
*******************************************************************************/
void Draw_Window(void)						
{ 
  s32 Tmp;
  u16 Row,i,Offset,SpecLimit;   
  s16 h=0;
  u16 TrackColor=0;
  u8  j,end=0;
	
  if(FlagMeter)Limit=213;else Limit=299;
  ListBottom=174+11;

  for(i=0;i<15;i++){if(Label[i][0]==0)break;else ListBottom-=11;}
  Base1(1);	
  __Row_DMA_Ready();
  __Row_Copy(Base2Buffer, LCD_Buffer[0]);
   
      //if(_4_source>13)COLOR_INDEX=0x0500; else COLOR_INDEX=0x0501;    //sets fft values display to source channel color (need to change uc)

	if(((ShowFFT==1)||(_4_source==SPEC_A))&&(_Mode!=X_Y)){
                if(_T_base==8){                         //special case to align with data flow and other timebases
                  Delayms(2);                           //>> Delayms delay may be undefined for small delays (<20mS)...
                  Count_FPS++;
                  Delayms(4);
                }      

                if(DownConvertMode==0){
                  FFTflag=1;		
                  Display_Value(FRQ);			//calculate NFreq using meter routine, sets N_UNIT and ShiftDigits for proper display
                  FFTflag=0;
                }
  
		PeakFreq = 2;                          //Called for as array index for LCD_Buffer @ -2. Less than 2 could put index out of bounds
	        imax = 0;
#warning Gabo: FFT disabled
#if 0               
		for (i=1; i < FFTSize/2; i++) {
			if (PeakFreq < fi[i]) {  
				PeakFreq= fi[i] ; 
				imax = i;
			}
		}
#endif
                if((AutoFFT==1)&&((EnableFFT==2)||(_T_base>7)||(freerun))){                      //auto fft is on if =1
                  if(FFTGain>7)FFTGain=7;			//auto gain limit
                  if((PeakFreq==198)&&(FFTGain>0))FFTGain--;
                    else{
                      if(PeakFreq<100){                                     
                        if(FFTGain<7)FFTGain++;
                        if((PeakFreq<78)&&(FFTGain<7))FFTGain++;                 			
                      }
                    }
                }
                if(EnableFFT==2)EnableFFT=0; 

                if(DownConvertMode==0){
                  Int2Str(NFreqStr, NFreq,N_UNIT, 4, UNSIGN,ShiftDigits);
                  Int2Str(FreqDivStr, ((NFreq / FFTBins) * 30),N_UNIT, 4, UNSIGN,ShiftDigits);

                  Tmp=(NFreq/FFTBins)*_T1;
                  if((ChartLogic())&&(Tmp<1000))Tmp=0x80000000;                
                  Int2Str(FreqT1Str, Tmp,N_UNIT, 4, UNSIGN,ShiftDigits);

                  Tmp=(NFreq/FFTBins)*imax;
                  if((ChartLogic())&&(Tmp<1000))Tmp=0x80000000;                
	          if (imax>0)Int2Str(PeakFreqStr, Tmp, N_UNIT, 4, UNSIGN,ShiftDigits);
                }

        }


 if ((_Mode!=X_Y)&&(((_4_source!=SPEC_A)&&(_4_source!=SPEC_B))||(ChartLogic()))){
   Count_FPS++;
   if((_Mode==NORHLD)&&(PerstFrameNumber>0)){
     if(ClearPerst==0){
       if(FrameCount>PerstFrameNumber){
         FrameCount=0;
         ClearPerst=1;
         //if(PerstFrameNumber>50)PersHoldTimer=50;else PersHoldTimer=PerstFrameNumber; //adds delay after x frames displayed
         PersHoldTimer=0;                                                               //no delay
       }else if(FrameEndFlag)FrameCount++;
     }
     if((ClearPerst==1)&&(PersHoldTimer==0))ClearPerst=2;               //each PersHoldTimer count = 20ms > make this manually adj? 
   }else ClearPerst=0;

   if(_4_source>11) NFreqParse(1,0);

   TcurDelta=(((_T2-_T1)*_Kp1)+512)/1024;

   if(!((Current==OUTPUT)&&(_Det==1)&&(_Kind==8)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC)))){
     if(UartLogic())Uart();
     if(i2cLogic())i2c();
     if(SpiLogic())Spi();
   }

  for(Row = MIN_X; Row <= MAX_X; ++Row){

   if(!(OsBufferLogicFlag||OsChartFlag)){

     if (Row<(discard + MIN_X)){			//blanks first few pixels of traces to eliminate noise
       Color[TR_1]=0x0000;				//process makes these into a hor line so "low brightness"
       Color[TR_2]=0x0000;				//in draw row does not turn them on
     }

     if (Row>(404-Rdiscard)){			//post-signal blanking for dig ch triggering jitter stabilization	 
       Color[TR_1]=0x0000;			
       Color[TR_2]=0x0000;			//404 aligns with x_size @ 392 and IP routine bailing out @ 392	
       Color[TR_3]=0x0000;
       Color[TR_4]=0x0000;
     }

     if ((Row>382)&&(Row<398)){                                       //post trace blanking of detector trace
        if((Det&1)&&_1_source)Color[TR_1]=0x0000;
        if((Det&2)&&_2_source)Color[TR_2]=0x0000;
     }

     if ((Title[TRIGG][SOURCE].Value<2)||(Title[TRIGG][SOURCE].Value==4))end=discard+5; else end=discard;  //was 4, changed to 5
   
     if (Row<(MIN_X+end)){			//blanks first pixels shifted in dig channels to align with analog chs
       Color[TR_3]=0x0000;
     }

     if ((_4_source==A_add_B)||(_4_source==A_sub_B))end=discard;  

     if (Row<(MIN_X+end)){			//blanks first pixels shifted in dig channels to align with analog chs
       Color[TR_4]=0x0000;
     }

   } //if !OsBufferLogicFlag

     Draw_Row(Row);     //Modo oscilloscopio
     if((Row<(discard+5+MIN_X))||(Row>(404-Rdiscard))) {		//was4, changed to 5
       Color[TR_1]=0xFFE0; 								//return to correct colors
       Color[TR_2]=0x07FF;
       Color[TR_3]=0xF81F;
       Color[TR_4]=0x07E0;
     }

     if ((Row>382)&&(Row<398)){                                       //post trace blanking of detector trace, return to correct colors
        if ((Det&1)&&_1_source)Color[TR_1]=0xFFE0;
        if((Det&2)&&_2_source)Color[TR_2]=0x07FF;
     }

  }  //end for loop
 }   //if mode !XY,MAP OR SPEC...

  if(ClearPerst==2)ClearPerst=0;

  if (_Mode==X_Y){
    PrintConfigName();
    Count_FPS++; 
    FactorX=((s16)OffsetX-100)+((-ADCoffset)+Ka1[_A_Range]);
    FactorY=((s16)OffsetY-100)+((-ADCoffset)+Kb1[_B_Range]);
    FactorY13=FactorY+13;							//for direct screen access, need offset 13
    if(EditListActive(1))XYLimit=310;else XYLimit=234;
    for(Row = MIN_X; Row <= MAX_X; ++Row) LoadXYBuffer(Row);
    __LCD_DMA_Ready();
    __Row_DMA_Ready();
    if((XYper)&&(EditListActive(1)))PrintDir(0,0,1);
    InitXY=0;
  } 											

if(((_4_source==SPEC_A)||(_4_source==SPEC_B))&&(!ChartLogic())){
  if((_Mode!=X_Y)&&(_State.Value==0)&&((FrameEndFlag==1)||((SpecFrameCount>FrameCountRef[_T_base])&&((_T_base>10)  ||(_Mode==SCAN)   ||
      ((FrameEndFlag==0)&&(__Get(FIFO_START)==0)))))){					//spectrograph/MAP modes

      if(SpecMode>0){        
        if(_1_source){h=(a_Max-a_Min)/(4/SpecMode);TrackColor=CYAN_;}		        //scale and remove offset using last frame
          else if(_2_source){h=(b_Max-b_Min)/(4/SpecMode);TrackColor=YEL_;}		//done vars (not accumulated)
        if(h<0)h=-h; 
      }
      if((FrameEndFlag==0)&&(_Mode!=AUTO)&&(_Mode!=SCAN)&&(__Get(FIFO_START)==0)){
        for (i=0; i<MAX_X; i++){TrackBuff[i*4]=0;TrackBuff[(i*4)+1]=0;}                 //clear track data if triggering lost
        ClearFFTbuffer();
        h=0; 										//clear envelope
      }

    if(_4_source==SPEC_B){ 				//MAP mode
        Count_FPS++;

        if (SpecLine<MIN_Y)SpecLine=MAX_Y;		

        if ((EditListActive(1))&&(SpecLine<(MAX_Y-15))){
          if(SpecLine>(ListBottom+MIN_Y))SpecLimit=Limit;else SpecLimit=X_SIZE;
        }else SpecLimit=X_SIZE;

	for (i=0; i<MAX_X+1; i++) {							//max_x=397 with meters off
          __Point_SCR(i,SpecLine);                    
          if((i>MIN_X)&&(i<X_SIZE)){
            if(i>SpecLimit)continue; 								
            if(_1_source)__LCD_SetPixl(ColorIndexGen(TrackBuff[(i-6)*4]));
              else if(_2_source)__LCD_SetPixl(ColorIndexGen(TrackBuff[((i-6)*4)+1]));
          }else if((i>3)&&(i<9)&&(SpecLine>(MIN_Y+1))){
             __LCD_SetPixl(0);
             DrawPixel(i,SpecLine-1,YEL_);
             if(_T_base>9)DrawPixel(i,SpecLine-2,YEL_);
          }else __LCD_SetPixl(0);
  	}

        if(FlagMeter==0)for(i=397;i<400;i++)DrawPixel(i,SpecLine,0);
        if((SpecLine==MIN_Y)||(SpecLine==MAX_Y))for(i=0;i<MAX_X;i++)DrawPixel(i,SpecLine,GRAY);
	SpecLine--;
    }

    if(_4_source==SPEC_A){				//now defines A/B spec and envelope display
#warning Gabo: FFT disabled
#if 0               
      Count_FPS++;

      if((SpecRow==25)||(UpdateScale)){                //>>>> need to reset specrow @ 25 every time mode is engaged

        ClearScreenArea(0,26,12,214);
        UpdateScale=0;
                
        Print_Str(0,193+MIN_Y,0x050E,INV,"sec");
        if(_T_base<2)Offset=128;else Offset=512;
        for(i=34;i<195;i+=34){					  //draw freq scale in spec mode
          for(j=20;j<25;j++)DrawPixel(j,i+MIN_Y,YEL_);
          Tmp=((NFreq+Offset)/255)*i;
          Int2Str(NFreqStr,Tmp,N_UNIT, 4, UNSIGN,ShiftDigits);    
          NFreqParse(0,1);
          if(Nfreq[2]==46){Nfreq[2]=0;j=4;}else j=0;              //remove trailing periods, shift over
          Print_Str(j,i+(MIN_Y+2),0x0508,INV,Nfreq);
          Print_Str(4,i+3,0x0508,INV,Nsuffix);
        }
      }
        if(SpecRow==25)SecondsTick=0;	
	
        if(AutoFFT==0){for(i=0;i<200;i++){if(fi[i]<51){fi[i]=(fi[i]*fi[i]*fi[i])/2500;}}}    // x^3/50^2 noise suppression in log mode 
        for (i=Y_BASE+1;i<200;i++) {				     
          if(SpecMode==0){
            LCD_Buffer[SpecRow & 1][i] = ColorIndexGen(fi[i-Y_BASE]);  				//spec display only
          }else if(SpecMode==1){
            if((i>=(150-h))&&(i<=(150+h)))							//spec + envelope display(1/2size)
              LCD_Buffer[SpecRow & 1][i] = TrackColor;							
                else LCD_Buffer[SpecRow & 1][i] = ColorIndexGen(fi[i-Y_BASE]);  
          }else{								
            if((i>=(100-h))&&(i<=(100+h)))LCD_Buffer[SpecRow & 1][i] = TrackColor;		//envelope display only full size
              else LCD_Buffer[SpecRow & 1][i] = BLACK;
          }
  	}

        if ((EditListActive(1))&&(SpecRow>Limit)){
          PrintDir(SpecRow,SpecRow&1,0);
        }

        if(SpecRow<MAX_X-1)for(i=209;i<213;i++)DrawPixel(SpecRow+2,i,YEL_);                      //moving cursor
        if ((_T_base>5)&&(SecondsTick==0)&&(SpecRow<MAX_X-1)){
          for(i=209;i<213;i++)DrawPixel(SpecRow-1,i,YEL_);                                       //seconds ticks
          SecondsTick=48;
        } 
	__Point_SCR(SpecRow, MIN_Y);
        __LCD_Copy(LCD_Buffer[SpecRow & 1], Y_SIZE+1);               	
	SpecRow++;
        if (SpecRow > MAX_X) SpecRow = 25;        
#endif
     }

    if(_Mode==AUTO)SpecFrameCount=3;else SpecFrameCount=0;					//reset interrupt based frame timing
  }									//if mode != xy
}									// is in spec a or b

  if((FrameEndFlag)&&(ResetEnableFFT==0))EnableFFT=1;
  ResetEnableFFT=0;
  FrameEndFlag=0;
  RowMemPeak = RowMem;
  
  __LCD_DMA_Ready();
  __Row_DMA_Ready();

  if ((Current==9)&&(_Det==3)&&((ListLogic())||(_Mode==NORHLD)))PrintDir(0,0,1);
}
/*******************************************************************************
 Draw_Mark :  Routine per un marcatore laterale
*******************************************************************************/
void Draw_Mark(u16 m, u16 n)						
{ 
  u16 i, j, Tmp;

  V_Trigg[2].Value=Title[2][POSI].Value + 10;			        // show ch3 and 4 trigger points at their actual values
  V_Trigg[3].Value=Title[3][POSI].Value + 10;
  
  if(m < 4)  { if((Title[m][POSI].Value > 200-3)||(Title[m][POSI].Value <3))return; }     //shut off at bottom too
  else       { if((V_Trigg[n].Value > 200-3)||(V_Trigg[n].Value <3))return; }

  for(i = 0; i <= 10; ++i){
    if(i < 7){
      if(m < 4){									    //Y position mark
        Tmp = Mark_TAB[m][i];
        __Point_SCR(i, Title[m][POSI].Value +(MIN_Y-4));
      } else {
        Tmp = Mark_TAB[4][i];
        __Point_SCR(i,     V_Trigg[n].Value +(MIN_Y-4));				    //Trigg vernier position mark
      }
      for(j = 0; j < 8; ++j){
        if(Tmp & 1) __LCD_SetPixl(BLACK);
        else        __LCD_SetPixl(Color[n]);
        Tmp >>= 1;
      }
      __LCD_SetPixl(Color[n]);
    } else {
      if(m < 4){
        __Point_SCR(i, Title[m][POSI].Value +(MIN_Y-10)+ i);
        for(j=Title[m][POSI].Value+(MIN_Y-10)+i;j<(Title[m][POSI].Value+(MIN_Y+11)-i);++j)  //Y position mark
          __LCD_SetPixl(Color[n]); 
      } else { 
        __Point_SCR(i,     V_Trigg[n].Value +(MIN_Y-10)+ i);   
        for(j=V_Trigg[n].Value+(MIN_Y-10)+i;j<(V_Trigg[n].Value+(MIN_Y+11)-i);++j)          //Trigg vernier position mark
          __LCD_SetPixl(Color[n]); 
      }
    }
  }
}
/*******************************************************************************
 Update_Mark :  Disegna Marcatori laterali
*******************************************************************************/
void Update_Mark(void)						
{ 
  s16 i, Tmp;
  u8  j=0;
  __Row_DMA_Ready();

  ClearScreenArea(0,11,MIN_Y-2,MAX_Y+3);

  if(ConfNameTimer>0)PrintConfigName();                                 //marks cover text

  for(i=0;i<4;i++){
    if(Title[i][SOURCE].Value!=HIDE)Draw_Mark(i,i);	 		 // Display Track mark
    if(_Tr_source==i)Draw_Mark(TRIGG,i);				 // Display Trigg mark
  }
  if(_Tr_source == 4) {
    Draw_Mark(TRIGG,  TRACK1); // Display Trigg1 mark
    Draw_Mark(TRIGG,  TRACK2); // Display Trigg2 mark
  }
  if(SpiLogic()){
    if(_1_source)Draw_Mark(TRIGG,  TRACK1);                              // Display Trigg marks in spi mode 
    if(_2_source)Draw_Mark(TRIGG,  TRACK2); 
  }
  if(i2cLogic()){
    if(_1_source)Draw_Mark(TRIGG,  TRACK1);                              // Display Trigg marks in i2c mode 
  }

  if(ConfNameTimer==0)PrintConfigName();                                 //text covers marks

  __Point_SCR(MAX_X+1, MIN_Y-2);								//up/down 1 more as above
  for(j = MIN_Y-2; j < MAX_Y+3; ++j) __LCD_SetPixl(BLACK); // Clear next to last row 
  __Point_SCR(MAX_X+2, MIN_Y-2);
  for(j = MIN_Y-2; j < MAX_Y+3; ++j) __LCD_SetPixl(BLACK); // Clear last row 
  for(i = MIN_X; i <= MAX_X+1; ++i)DrawBase(i,BLACK);

  __Point_SCR(MIN_X-1, Title[V_VERNIE][V1].Value +(MIN_Y - 2));		//draw "base" of V vernier marks
  for(i = 0; i < 5; ++i) __LCD_SetPixl(WHT);
  __Point_SCR(MIN_X-1, Title[V_VERNIE][V2].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(WHT);
  __Point_SCR(MAX_X+1, Title[V_VERNIE][V1].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(WHT);
  __Point_SCR(MAX_X+1, Title[V_VERNIE][V2].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(WHT);

  Tmp2=ScaleXposi();

  if(Tmp2 > MIN_X){							//X position trigger point vernier "base" marks
    for(i = Tmp2 - 2; i <= Tmp2 + 2; ++i)DrawBase(i,ORANGE);
  }
  Tmp = MIN_X + Title[T_VERNIE][T1].Value;				//T vernier base marks
  for(i = Tmp - 2; i <= Tmp + 2; ++i)DrawBase(i,WHT);
  Tmp = MIN_X + Title[T_VERNIE][T2].Value;
  for(i = Tmp - 2; i <= Tmp + 2; ++i)DrawBase(i,WHT);
}
/*******************************************************************************
 Update_View_Area: Disegna rettangolo con zig zag posizione buffer 
*******************************************************************************/
void Update_View_Area(void)
{  
  s16 i, j, k;
  u16 color;

  for(i=89;i<310;i++){
    __Point_SCR(i,  0);
    for(j=0; j<12; ++j) __LCD_SetPixl(ORANGE); 
    if(i==90)i+=217;
  }

    if(OSBuffer==0)color=ORANGE; else{
      if(OSAvg)color=GRN;else{
        color=RED;
      }
    }

    for(i=91; i<307; i+=8){
      for(j=0; j<8; ++j){
        __Point_SCR(i+j, 0);
        for(k=0; k<12; ++k){
          if((0x01 << k)& RULE_BASE[j])__LCD_SetPixl(color);
          else  __LCD_SetPixl(BLACK);
        }
      }
    }
    __Point_SCR(307,0);
    for (i=0;i<12;i++) __LCD_SetPixl(BLACK);
    
    if (((FlagFrameMode ==1)&&(_Mode!=SGL))||(OSBuffer)) {

      if(OSBuffer==0)color=WHT; else color=ORANGE;
      __Point_SCR(0  + 91 , 0);      // disegna rettangolo per buffer ridotto draw rectangle to buffer reduced
      for(j=0;j<9;++j) __LCD_SetPixl(color);
      __Point_SCR(15 + 91 + 200, 0);
      for(j=0;j<9;++j) __LCD_SetPixl(color);
      for(i=0;i<216;++i){
        DrawPixel(i + 91,0,color);
        DrawPixel(i + 91,9,color);
      }

      if ((Options&1)>0) {
        if (((_Mode==NORH)||(_Mode==NORHLD)||(_Mode==NORC))&&(_T_base < 10)&&(!OsBufferLogicFlag)){
          Print_Str(Title[7][3].XPOS, 0,Title[7][3].Color[Title[7][0].Value], PRN,"T");	//active in this mode, normal color
        }else{
          Print_Str(Title[7][3].XPOS, 0,0x0905, PRN,"T");						//greyed out, not active
        }
      }
    }   
    else
    {
      __Point_SCR(0  + 91 + (_X_posi.Value * 200)/3720, 0); //3796
      for(j=0;j<9;++j) __LCD_SetPixl(WHT);
      __Point_SCR(15 + 91 + (_X_posi.Value * 200)/3720, 0);
      for(j=0;j<9;++j) __LCD_SetPixl(WHT);
      for(i=0;i<16;++i){
        DrawPixel(i + 91 +(_X_posi.Value * 200)/3720, 0,WHT);
        DrawPixel(i + 91 +(_X_posi.Value * 200)/3720, 9,WHT);
      }
    } 
    _X_View.Flag &= !UPDAT;
    DisplaySamples();

}
/*******************************************************************************
Clear_Meter_Area  Pulisce l'area sotto ai meter per quando vengono rivisualizzati
*******************************************************************************/
void Clear_Meter_Area(void)
{  
    ClearScreenArea(312,400,11,216);
    _X_View.Flag &= !UPDAT;
}

void LoadXYBuffer(u16 Row){
u8 i,buffer;   
s16 TempY;  
register u16 RegTemp=BufferSize-1; 
u16 h;

 if(((Row>37)&&(Row<XYLimit))||InitXY){
    __Row_DMA_Ready();
    __Point_SCR(Row, MIN_Y);
    if(Row & 1)buffer=1;else buffer=0;  
  if((InitXY)||(XYper==0)){
    if((Row == 260)||(Row+1 == MAX_X))Base1(buffer);						//for(i=0;i<201;i++)LCD_Buffer[1][i]=GRAY;
    else if(Row == MIN_X)Base1(buffer);								//for(i=0;i<201;i++)LCD_Buffer[1][i]=GRAY;
    else if(((Row+1 - MIN_X)%25 == 0)&& (Row<260))__Row_Copy(Base4Buffer, LCD_Buffer[buffer]);  //%30
    else if(((Row+1 - MIN_X)%5  == 0)&& (Row<260))__Row_Copy(Base3Buffer, LCD_Buffer[buffer]); 	//%6
    else if (Row != 260)__Row_Copy(Base2Buffer, LCD_Buffer[buffer]);
    if(InitXY)goto DrawCross;
  }

  if(XYper){										//persistence mode
    for(h = 0; h <=RegTemp; ++h){
         if(((DataBuf[h]&0xFF)+FactorX)==Row){					 	// if value in signal X matches position
           TempY=((DataBuf[h]>>8)&0xFF)+FactorY13;
           if((TempY>14)&&(TempY<212)){
           DrawPixel(Row,TempY,GRN);
           }
         }
    }

  }else{										//regular mode
    for(h = 0; h <=RegTemp; ++h){
         if(((DataBuf[h]&0xFF)+FactorX)==Row){					 		       
           TempY=((DataBuf[h]>>8)&0xFF)+FactorY;
           if((TempY>1)&&(TempY<199)){
             LCD_Buffer[buffer][TempY] |= GRN;
           }  
         }
    }
    if((Current==9)&&(_Det==3))PrintDir(Row,buffer,0);
DrawCross:
    if ((Row<(OffsetX+5)) && (Row>(OffsetX-5)))LCD_Buffer[buffer][OffsetY] |=PURPL;   
    if ((Row==OffsetX))for(i=OffsetY-4;i<OffsetY+5;i++)LCD_Buffer[buffer][i] |=PURPL;
    __LCD_Copy(LCD_Buffer[buffer], Y_SIZE+1);               
  }

 }											//if row>37
}


void DisplayDetFrqn(u16 Row, u8 buffer){
u16 i,End;
u8  j;
s32 Tmp;
static u8 PrintFlag=1;
static u16 xpos;
static u8 ypos=1;
u16 ReadPos;
u16 ReadARR;
u32 SamplingPeriod;
u32 Tim2Period;
s16 Tim2Shifts;
static char Str1[12];

 if((ShowFFT==0)&&(CurDefTime)&&(_Kind<4)&&(Sweep==1)){  

  EnableMeterCalc=1;
  if(LastFreqReadout==0)return;
  if(FlagMeter)End=280;else End=340;

  for(i=(MIN_X+18);i<(MIN_X+End);i+=30){
    if((Row>=i)&&(Row<(i+33))){                                                                    
      if(Row==(MIN_X+18)){
        ReadPos=(_T1+_T2)/2;                                                                          
        if(A_Tab[_Frqn]<201)ReadARR=((100000*ScaleIndex[_Frqn])+(LastFreqReadout/2))/LastFreqReadout;    //use rounding for 500hz ranges and up 
          else ReadARR=(100000*ScaleIndex[_Frqn])/LastFreqReadout;                                       //use no rounding for lower frequency ranges <500hz 
        SamplingPeriod=(((X_Attr[_T_base].PSC+1)*(X_Attr[_T_base].ARR+1)*100)+36)/72;                    //X this by 100 for x10nS and full scale range
        Tim2Period=((TIM2->ARR+1)*SweepMod*100)/SweepStep;                                               //as above

        for(j=0;j<12;j++){
          Tmp=ReadPos-((j+1)*30);                                           //how far away (+or-)from readposition each display is in number of samples
          if(Tmp>0)Tim2Shifts=(s32)((SamplingPeriod*Tmp)+(Tim2Period/2))/(s32)Tim2Period;                   
            else Tim2Shifts=(s32)((SamplingPeriod*Tmp)-(Tim2Period/2))/(s32)Tim2Period;
         if(A_Tab[_Frqn]<201){                                              //500hz and up FREQUENCY DISPLAY TRACKING
//====================500 HZ AND UP COMPENSATION===================================================
            if(Tim2Shifts>0)Tim2Shifts=((Tim2Shifts*DetFrqAdj[SweepIndex])+50)/100;           //INCREASING factor DECREASES display freq values LEFT of sample                                    
              else Tim2Shifts=((Tim2Shifts*DetFrqAdj[SweepIndex])-50)/100;                    //INCREASING factor INCREASES display freq values RIGHT of sample 
//=================================================================================================
            DetFrqn[j]=(s16)ReadARR+Tim2Shifts;                                
//====================500 HZ AND UP BOUNDS LIMIT===================================================
            if((_Frqn==8)&&(DetFrqn[j]<100))DetFrqn[j]=0;                   //500HZ SPECIAL CASE RIGHT BOUNDS LIMIT    >flag out of bounds for disabling
              else if(DetFrqn[j]<39)DetFrqn[j]=0;                           //RIGHT BOUNDS LIMIT REST
                else if(DetFrqn[j]>(A_Tab[_Frqn]+105))DetFrqn[j]=0;         //LEFT BOUNDS LIMIT
//=================================================================================================
          }else{                                                            //200hz and down FREQUENCY DISPLAY TRACKING
//====================200 HZ AND DOWN COMPENSATION===================================================
            if(Tim2Shifts>0)Tim2Shifts=((Tim2Shifts*120)+50)/100;           //INCREASING factor DECREASES display freq values LEFT of sample                                      
              else Tim2Shifts=((Tim2Shifts*120)-50)/100;                    //INCREASING factor INCREASES display freq values RIGHT of sample                             
//=================================================================================================
            Tmp=((ReadARR-((ReadARR+(A_Tab[_Frqn]+1))/2))*600)/((ReadARR+A_Tab[_Frqn]+1));    //contains X TIM2shifts in relation to freq range default ARR
            Tmp+=Tim2Shifts;
            DetFrqn[j]=(((A_Tab[_Frqn]+1)*(300+Tmp))+((300-Tmp)/2))/(300-Tmp);                //shifts back to individual display positions, gives ARR 
//====================200 HZ AND DOWN BOUNDS LIMIT===================================================
            if((Tmp<-102)||(Tmp>105))DetFrqn[j]=0;                          //RIGHT AND LEFT BOUNDS LIMIT
//=================================================================================================
          }
        }
      }//if(Row==MIN_X
      if(Row==i){
        Tmp=DetFrqn[(i-MIN_X)/30];
        if(Tmp>0)PrintFlag=1;else PrintFlag=0;                                                //do not show out of bounds values       
        Tmp=(1000000000/Tmp)*ScaleIndex[_Frqn];                                               //calculate frequency at each division (scale=4)  
        Int2Str(Str1,Tmp, F_UNIT, 3, UNSIGN, 4);
        if(Str1[0]==46){for(j=0;j<3;j++)Str1[j]=Str1[j+1]; Str1[3]=0;}                        //remove leading periods, not needed without suffixes
        for(j=0;j<5;j++){if(Str1[j]==46)Str1[j]=127;}                                         //proportional 3pix period so displays are not too wide
        Str1[4]=0;
        if((i-(MIN_X+18))%60==0)ypos=1;else ypos=6;                                           
        xpos=i;                                 
      }

      if(PrintFlag){
        Print_Str_Row(Row, LCD_Buffer[buffer],xpos,ypos,Str1);         
        if((Row-MIN_X)%30==0){
          for(j=(ypos+11);j<(ypos+17);j++)LCD_Buffer[buffer][j]=GRN;
        } 
      }
    }//if((Row>=i 
  }//for
 }else EnableMeterCalc=0; //if showfft=0  

}

void InitiateCharArrays(void){
u16 i;
char *p,*q;

  p=AscChar[0][0];
  q=AscCharB[0][0];
  for(i=0;i<102;i++){*p++=0;*q++=0;}                                                    
  p=&HexChar[0][0];
  q=&HexCharB[0][0];
  for(i=0;i<294;i++){*p++=0;*q++=0;}
}

void Uart(void){
u16 Iterations=0;
u16 i;
u8  j;  
static u16 Start=90;
u8  End;
u8  Tmp;
u16  FrameSamples=TcurDelta;
u16  LoString=TcurDelta;
u16  HiString=TcurDelta;
register u8   DataByte;
register u8   PrevDataByte;

  if(_T1>=_T2)return;                                                                    
  BitDuration=(FrameSamples+(FrameSize/2))/FrameSize;
  End=FrameSize-(2+Parity);                                  

  if(T1Start==1){
    if(Current==T_VERNIE){
      Start=IndexPosition(_T1,1)+_X_posi.Value;                                                                                
      if(Start<1)Start=1;
    } 
  }else Start=90;

Restart:
  for(i=0;i<96;i++)ByteArray[i].Data=0;
  ArrayIndex=0;
  ValidFrame=1;

  for(i=Start;i<(bag_max_buf-FrameSamples);i++){                            //limit to end of buffer - 1 frame size
    DataByte=((DataBuf[i]&0xFF)-ADCoffset);
    PrevDataByte=((DataBuf[i-1]&0xFF)-ADCoffset);

    if(DataByte<Ch1TLevel)LoString++;else LoString=0;
    if(DataByte>Ch1TLevel)HiString++;else HiString=0;

    if(((Title[TRACK1][1].Value==3)&&  ( (PrevDataByte>Ch1TLevel)||(LoString<FrameSamples) )       &&(DataByte<Ch1TLevel))||
      ((Title[TRACK1][1].Value==4)&&  ( (PrevDataByte<Ch1TLevel)||(HiString<FrameSamples) )       &&(DataByte>Ch1TLevel))){              

      ByteArray[ArrayIndex].Index=i;                                       //record on screen position of each captured frame start bit 
      for(j=0;j<End;j++){
        Tmp=((DataBuf[i+((((j+1)*FrameSamples)+(FrameSize/2))/FrameSize)+(BitDuration/2)]&0xFF)-ADCoffset);
        if(Title[TRACK1][1].Value==3){
          if(Tmp>Ch1TLevel)ByteArray[ArrayIndex].Data|=Mask<<j;                                    //TTL mode
        }else if(Title[TRACK1][1].Value==4){if(Tmp<Ch1TLevel)ByteArray[ArrayIndex].Data|=Mask<<j;} //RS232 mode
      }
      ArrayIndex++;if(ArrayIndex>95)break;                                 //move to next byte

      i+=(FrameSamples-((BitDuration+1)/2));                               //move index to middle of stop bit

                     //with start bit recognized, look for stop bit, if not found set validframe to 0 to shift start position and restart scan
      if(((Title[TRACK1][1].Value==3)&&(((DataBuf[i]&0xFF)-ADCoffset)<Ch1TLevel)&&(((DataBuf[i+1]&0xFF)-ADCoffset)<Ch1TLevel))||
        ((Title[TRACK1][1].Value==4)&&(((DataBuf[i]&0xFF)-ADCoffset)>Ch1TLevel)&&(((DataBuf[i+1]&0xFF)-ADCoffset)>Ch1TLevel)))ValidFrame=0;
      if((ValidFrame==0)&&(T1Start==0)){
        if(Iterations<(FrameSamples*2)){     //limit tries to 2 frame lengths, if not sync'ed by then ValidFrame stays at 0
          Iterations++;
          Start++;
          goto Restart;
        }else break;
      }                             
      
    }//if
    //if(Key_Buffer)break;                     
  }//for

  LoadChars(0);
}


void LoadChars(u8 service){
u8 i,j; 
u8 Tmp,end;                   
u16 Counter=0;
char *p,*s;

 InitiateCharArrays();
 if(service==2)end=2;else end=1;
 for(j=0;j<end;j++){
  for(i=0;i<96;i++){
    if(i==ArrayIndex){
      AscChar[i/48][(i/16)%3][i%16]=0;
      AscCharB[i/48][(i/16)%3][i%16]=0;
      HexChar[i/16][(i%16)*3]=0;                                                
      HexCharB[i/16][(i%16)*3]=0;
      break;
    }                       
    if(j==0)Tmp=ByteArray[i].Data;else Tmp=ByteArray[i].DataB;
    switch(Tmp){                                                                //for correct ascii chars available in program
      case (45)...(58):
      case (65)...(90):
      case (97)...(122):
      case (60)...(62):
      case (37)...(38):
      case (40)...(41):
      case 32:
      case 43:
      case 124: 
        if(j==0)AscChar[i/48][(i/16)%3][i%16]=ByteArray[i].Data;
          else AscCharB[i/48][(i/16)%3][i%16]=ByteArray[i].DataB;
        break;
      default:
        if(j==0)AscChar[i/48][(i/16)%3][i%16]=63;
          else AscCharB[i/48][(i/16)%3][i%16]=63;
    }
    if (j==0){
      if(service==1){
        if(ByteArray[i].End & 0x2000)AscChar[i/48][(i/16)%3][i%16]=128;     // special "ad" char 
        if(i>0){
          if(ByteArray[i-1].End & 0x1000){                                  // if byte following a 10 bit address frame 
            AscChar[i/48][(i/16)%3][i%16]=128;                              // special "ad" char for second part of 10 bit address
          }
        }
      }
      if((service<2)||((SpiNumBits>0)&&(SpiNumBits<9))){
        p=&HexChar[i/16][(i%16)*3];                                        //copy 2 bytes + add space or null if at end
        Char2Hex(HexLookup,ByteArray[i].Data);
        s=(char*)HexLookup;
        *p++=*s++;*p++=*s++;
        if((i%16)==15)*p++=0;else*p++=0x20;
      }
    }
    if((j==1)&&((SpiNumBits>0)&&(SpiNumBits<9))){
      p=&HexCharB[i/16][(i%16)*3];                                        //copy 2 bytes + add space or null if at end
      Char2Hex(HexLookup,ByteArray[i].DataB);
      s=(char*)HexLookup;
      *p++=*s++;*p++=*s++;
      if((i%16)==15)*p++=0;else*p++=0x20;                                 //if end of array dimension, add null otherwise add space and go on 
    }

  }//for i
 }//for j

  if((service==2)&&((SpiNumBits<=0)||(SpiNumBits>8))){                                  //SPI chart with variable word length
    Counter=0;
    for(i=0;i<96;i++){
      if(i==ArrayIndex){
        HexChar[Counter/48][Counter%48]=0;                                          
        HexCharB[Counter/48][Counter%48]=0;
        return;
      }
      if((ByteArray[i].Index>0)&&(ByteArray[i].BitsPerWord>0)){                          //id start of word
        for(j=0;j<((ByteArray[i].BitsPerWord+7)/8);j++){
          if((j==0)&&((((ByteArray[i].BitsPerWord+7)/8)*2)>(48-(Counter%48))))           //move to next line if not enough room for full word
            while(Counter%48){Counter++;if(Counter>(5*48))break;}               

          Char2Hex(HexLookup,ByteArray[i+j].Data);
          HexChar[Counter/48][Counter%48]=HexLookup[0];              //high nibbles
          Char2Hex(HexLookup,ByteArray[i+j].DataB);
          HexCharB[Counter/48][Counter%48]=HexLookup[0];
          Counter++;              

          Char2Hex(HexLookup,ByteArray[i+j].Data);
          HexChar[Counter/48][Counter%48]=HexLookup[1];              //low nibbles
          Char2Hex(HexLookup,ByteArray[i+j].DataB);
          HexCharB[Counter/48][Counter%48]=HexLookup[1];
          Counter++;              

          if(Counter>=(6*48))return;
        }
        if(((Counter%48)<46)&&((Counter%48)>0)){                                         //no spaces needed at end or start of lines
          HexChar[Counter/48][Counter%48]=0x20;                                          //spaces between words
          HexCharB[Counter/48][Counter%48]=0x20;
          Counter++;              
        } 
      }
    }

  }

}

void SpiChart(u16 Row,u8 buffer){                                                         //displays words with varying lengths
u8 i,j,x1,x2,x3,x4;

    COLOR_INDEX=0x0504;
    if((A_Posi<B_Posi)||(_3_source==0)){x1=90;x2=63;x3=187;x4=160;}else     //if ChA is below ChB or if ChB is used as clock > ChA is below 
      {x3=90;x4=63;x1=187;x2=160;}                                          //if ChA is above and ChC used as clock, need to show ChB below

    for(i=0;i<2;i++){
      for(j=0;j<3;j++){
        if((SpiNumBits==7)||(SpiNumBits==8)){
          if(j==1)COLOR_INDEX=0x0501;else COLOR_INDEX=0x0504;
          Print_Str_Row(Row, LCD_Buffer[buffer],13+(j*(16*8)),x1-(i*11),AscChar[i][j]); 
          if(_3_source)Print_Str_Row(Row, LCD_Buffer[buffer],13+(j*(16*8)),x3-(i*11),AscCharB[i][j]); 
          Print_Str_Row(Row, LCD_Buffer[buffer],17,x2-((((i*3)+j)*11)+(i*5)),HexChar[(i*3)+j]);    
          if(_3_source)Print_Str_Row(Row, LCD_Buffer[buffer],17,x4-((((i*3)+j)*11)+(i*5)),HexCharB[(i*3)+j]);   
        }else{
          Print_Str_Row(Row, LCD_Buffer[buffer],13,x2-((((i*3)+j)*11)+(i*5)),HexChar[(i*3)+j]);    
          if(_3_source)Print_Str_Row(Row, LCD_Buffer[buffer],13,x4-((((i*3)+j)*11)+(i*5)),HexCharB[(i*3)+j]);   
        }
      }
    }
}

void PrintDir(u16 Row, u8 buffer,u8 Mode){
u8 i,j,h,y=174,LastY=174;
u16 x;
char Transfer[13];
     
  if(FlagMeter)x=214;else x=300;
  SkipFirstRow=1;                                                                         //aligns Print_Str with Print_Str_Row

  if(Title[9][0].Value==1){ 
    for(i=0;i<15;i++){

      for(j=0;j<13;j++)Transfer[j]=Label[i][j];
      for(j=0;j<8;j++){
        switch(Transfer[j]){                                                              //process unavailable chars to display dot
          case 39:              //'
          case 64:              //@
            Transfer[j]=63;     //dot
            break;
          default:
            if(((Transfer[j]>32)&&(Transfer[j]<37))||(Transfer[j]>124))Transfer[j]=63;
        } 
      }

      if(i==Title[9][3].Value){                                                           //if item position is selected (blink)
        if(Mode==2){
          COLOR_INDEX=0x0405;                                                             //selection hilighted but not blinking
        }else{
          if(Twink)COLOR_INDEX=0x0504;else COLOR_INDEX=0x0405;                            //selection blinking
        } 

      }else{
        COLOR_INDEX=0x0504;
        for(h=0;h<8;h++){
          if(SelectedFileName[h]!=Label[i][h])break;
          if(h==7)COLOR_INDEX=0x0405;                                                     //if file has been recently loaded or saved
        }
      }

      if(Label[i][0]!=0){
        if(Mode==0){
          Print_Str_Row(Row, LCD_Buffer[buffer],x,y,Transfer);                            //write to screen buffer   
        }else if((Mode==1)||(Mode==2)){ 
          Print_Str(x,y+MIN_Y,COLOR_INDEX,INV,Transfer);                                  //direct write to screen (mode 2= no blinking)
        }else{
          for(j=0;j<12;j++)Transfer[j]=' ';                                               //direct write cleared list to screen
          Print_Str(x,y+MIN_Y,0x0504,INV,Transfer);
        }
        LastY=y;
      } 
      y-=11;

    }

    if(DownConvertMode){
      j=LastY-5;
      if((Row>(x-7))&&(Row<x)){
        for(i=j;i<185;i++)LCD_Buffer[buffer][i]=0;                                   
      }
      if((Row>=x)&&(Row<(x+(8*12)))){
        for(i=j;i<LastY;i++)LCD_Buffer[buffer][i]=0;
      }
    }

  }
  SkipFirstRow=0;                                                                        //return Print_Str to normal for rest of program
  COLOR_INDEX=0x0503;
}



void PrintUart(u16 Row, u8 buffer,u8 service){                                          //service=0> print uart, =1> print i2c, =2> print spi
u8 i,j; 
char *p;
char HexStr[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};      //32 bytes max for 192 possible SPI bits
static u8 Read=0;
static u8 BytesPerWord=1;

if(SpiNumBits>0)BytesPerWord=((SpiNumBits+7)/8);
                                      
if((Row > MIN_X)&&(Row < MAX_X)){
  for(i=0;i<96;i++){
    if(i>=ArrayIndex)break;

    if(service==0){
      if((IndexPosition(Row,1)+_X_posi.Value)>=(ByteArray[i].Index)&&(IndexPosition(Row,1)+_X_posi.Value)<=(ByteArray[i].Index+(TcurDelta)+9)){

        COLOR_INDEX=0x0509;      //GRAY
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+MIN_X+1)-(_X_posi.Value),0),112,"<");               //bar at start of frame
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+(TcurDelta)+MIN_X)-(_X_posi.Value+((8*1024)/_Kp1)),0),112,">");     //bar at end of frame
        COLOR_INDEX=0x0504;      //WHITE
        HexStr[0]=AscChar[i/48][(i/16)%3][i%16];HexStr[1]=0;
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+((((TcurDelta)+1)/2)-((4*_Kp1)/1024))+MIN_X)-_X_posi.Value,0),112,HexStr);                          //ascii char
        COLOR_INDEX=0x050B;      //GREEN 
        Char2Hex(HexLookup,ByteArray[i].Data);               
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+((((TcurDelta)+1)/2)-((8*_Kp1)/1024))+MIN_X)-_X_posi.Value,0),101,(char*)HexLookup);    //hex value

      }
    }

    if(service==1){
      if(      (IndexPosition(Row,1)+_X_posi.Value)        >=(ByteArray[i].Index)&&                                   
         (IndexPosition(Row,1)+_X_posi.Value)<=((ByteArray[i].End & 0xFFF)+9)){

        if(ByteArray[i].End & 0x2000){                                                                                       //if address word
           if(ByteArray[i].End & 0x8000)Read=1;else Read=0;
        }

        if(Read){if(ByteArray[i].End & 0x4000)COLOR_INDEX=0x050F; else  COLOR_INDEX=0x050B;}
          else COLOR_INDEX=0x050D;
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+MIN_X+1)-(_X_posi.Value),0),112,"<");       //bar at start of frame

        if(Read)COLOR_INDEX=0x050D; else {if(ByteArray[i].End & 0x4000)COLOR_INDEX=0x050F; 
          else COLOR_INDEX=0x050B;}
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition(((ByteArray[i].End & 0xFFF)+MIN_X)
          -(_X_posi.Value+8),0),112,">");               

        if((ByteArray[i].End&0x2000)||((i>0)&&(ByteArray[i-1].End&0x1000)))COLOR_INDEX=0x0507;else COLOR_INDEX=0x0504;                                              //add frame
        HexStr[0]=AscChar[i/48][(i/16)%3][i%16];HexStr[1]=0;
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+(((((ByteArray[i].End & 0xFFF)
          -ByteArray[i].Index)/2)-4)+MIN_X)-_X_posi.Value),0),112,HexStr);                                                  //ascii char

        if((ByteArray[i].End&0x2000)||((i>0)&&(ByteArray[i-1].End&0x1000)))COLOR_INDEX=0x0507;else COLOR_INDEX=0x050B;                                              //add frame
        Char2Hex(HexLookup,ByteArray[i].Data);
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+(((((ByteArray[i].End & 0xFFF)             //hex value
          -ByteArray[i].Index)/2)-8)+MIN_X)-_X_posi.Value),0),101,(char*)HexLookup);                                                      

      }
    }

    if(service==2){
      if(SpiNumBits<=0){                                                //INCLUDED 0
        if(ByteArray[i].BitsPerWord>0){
          Read=ByteArray[i].BitsPerWord;
          BytesPerWord=(((Read-1)/8)+1);
        }
      }else Read=SpiNumBits;           

      if(((IndexPosition(Row,1)+_X_posi.Value)>=ByteArray[i].Index)&&((IndexPosition(Row,1)+_X_posi.Value)<=
        (ByteArray[i].End+12))){                         //was End+9

        COLOR_INDEX=0x0509;                              
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+MIN_X+1)-(_X_posi.Value+8),0)+8,_1_posi-15,"<");   //CH A "<"         
        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].End+MIN_X)
          -(_X_posi.Value+6),0),_1_posi-15,">");                                                                                   //CH A ">"         
        if((_2_source)&&(_3_source)){                     
          Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+MIN_X+1)-(_X_posi.Value+8),0)+8,_2_posi-15,"<"); //CH B "<"
          Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].End+MIN_X)
            -(_X_posi.Value+6),0),_2_posi-15,">");                                                                                 //CH B ">"     
        } 

        COLOR_INDEX=0x0504;      //WHITE    
        if((Read==7)||(Read==8)){
          HexStr[0]=AscChar[i/48][(i/16)%3][i%16];
        }else HexStr[0]=63;
        HexStr[1]=0;

        Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+((((ByteArray[i].End
          -ByteArray[i].Index)/2))+MIN_X)-_X_posi.Value),0)-3,_1_posi-15,HexStr);                                              //CH A ASCII

        if((_2_source)&&(_3_source)){
          if((Read==7)||(Read==8)){
            HexStr[0]=AscCharB[i/48][(i/16)%3][i%16];
          }else HexStr[0]=63;
          Print_Str_Row(Row, LCD_Buffer[buffer],IndexPosition(((ByteArray[i].Index+((((ByteArray[i].End
            -ByteArray[i].Index))/2))+MIN_X)-_X_posi.Value),0)-3,_2_posi-15,HexStr);                                            //CH B ASCII
        }                             

        COLOR_INDEX=0x050B;      //GREEN    

        p=HexStr;
        for(j=0;j<BytesPerWord;j++){
          Char2Hex(HexLookup,ByteArray[i+j].Data);
          *p++=HexLookup[0];
          Char2Hex(HexLookup,ByteArray[i+j].Data);
          *p++=HexLookup[1];
        }
        *p=0;
        Print_Str_Row(Row,LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+((((ByteArray[i].End                      
          -ByteArray[i].Index)/2))+MIN_X)-_X_posi.Value),0)-((8*BytesPerWord)-1),_1_posi-26,HexStr);                 //CH A HEX                        

        if((_2_source)&&(_3_source)){
          p=HexStr;
          for(j=0;j<BytesPerWord;j++){
            Char2Hex(HexLookup,ByteArray[i+j].DataB);
            *p++=HexLookup[0];
            Char2Hex(HexLookup,ByteArray[i+j].DataB);
            *p++=HexLookup[1];
          }
          *p=0;
          Print_Str_Row(Row,LCD_Buffer[buffer],IndexPosition((ByteArray[i].Index+((((ByteArray[i].End                 
            -ByteArray[i].Index)/2))+MIN_X)-_X_posi.Value),0)-((8*BytesPerWord)-1),_2_posi-26,HexStr);               //CH B HEX                           
        }
        i+=(BytesPerWord-1);
      }
    }

  }
  
  if((service==2)&&(SpiChartFlag))SpiChart(Row,buffer);

  if(service<2){
    for(i=0;i<2;i++){
      for(j=0;j<3;j++){
        if(j==1)COLOR_INDEX=0x0501;else COLOR_INDEX=0x0504;
        Print_Str_Row(Row, LCD_Buffer[buffer],13+(j*(16*8)),90-(i*11),AscChar[i][j]); 
        Print_Str_Row(Row, LCD_Buffer[buffer],17,63-((((i*3)+j)*11)+(i*5)),HexChar[(i*3)+j]);
      }
    }
  }

  if(service==1){
    COLOR_INDEX=0x0507;                                                                             //highlight addresses in purple
    for(i=0;i<96;i++){
      if((ByteArray[i].End & 0x2000)||((i>0)&&(ByteArray[i-1].End & 0x1000))){                      //add frame or 2nd byte of 10 bit address
        HexStr[0]=AscChar[i/48][(i/16)%3][i%16];HexStr[1]=0;
        Print_Str_Row(Row, LCD_Buffer[buffer],13+((i%48)*8),90-((i/48)*11),HexStr);
        HexStr[0]=HexChar[i/16][((i%16)*3)];
        HexStr[1]=HexChar[i/16][((i%16)*3)+1];
        HexStr[2]=0;
        Print_Str_Row(Row, LCD_Buffer[buffer],17+(((i%16)*3)*8),63-(((i/16)*11)+((i/48)*5)),HexStr);
      }
    }
  }

  COLOR_INDEX=0x0503;
 }//if row 
}

u16 IndexPosition(s16 Position,u8 service){
if(service)return(((Position-150)*_Kp1)/1024)+150;                  
  else return(((Position-150)*1024)/_Kp1)+150;
}

void Spi(void){
u16 i;
s16 TargetBit=0;
u8 BitNumber=8, BytesPerWord=1;
u8 Start=90;

  for(i=0;i<96;i++){ByteArray[i].Data=0;ByteArray[i].Index=0;
    ByteArray[i].End=0;ByteArray[i].DataB=0;ByteArray[i].BitsPerWord=0;}
  ArrayIndex=0;

  if(SpiNumBits>0){                                                 //normal mode with defined word length
    BytesPerWord=((SpiNumBits+7)/8); 
    if(SpiBitOrder)TargetBit=0;else TargetBit=(SpiNumBits-1);
  }else{                                                            //in "reset word after space" mode

    if(SpiNumBits==0){
      ClockPeriod=0xFFFF;                                           //initiate WordEndFlag function in auto mode
      Start=90;                                                     
    }else{                      
      if(SpiNumBits>-120)Start=(135+SpiNumBits);else Start=15;      //SpiNumBits has been limited to prevent possible out of bounds errors
    }    

    for(i=Start;i<(bag_max_buf-1);i++){                             //setup to find in between clock bursts spaces and word lengths                  
      if(BitReadPosition(i)){
        if(WordEndFlag){                                            //beginning of new word at this point       
          if(TargetBit>0){                                          //if start position of new word, not at beginning
            ByteArray[ArrayIndex].BitsPerWord=TargetBit;            //store previous word length in bits
            if((ArrayIndex+(((TargetBit-1)/8)+1))<96){
              ArrayIndex+=(((TargetBit-1)/8)+1);                    //advance ArrayIndex enough bytes to contain all bits from previous word
            }else break;
            ByteArray[ArrayIndex].Index=i;                          //new start position
            TargetBit=1;
          }else {ByteArray[ArrayIndex].Index=i;TargetBit=1;}        //first word position, TargetBit will not advance, so do it here 
          WordEndFlag=0;
        }else if(TargetBit<192)TargetBit++;                         //count word bits
      }
    }
    if(WordEndFlag)ByteArray[ArrayIndex].BitsPerWord=TargetBit;     //record word length if not done
    if(WordEndFlag==0)ByteArray[ArrayIndex].Index=0xFFFF;           //if WordEndFlag=0, means end of buffer at mid word, cancel
  }                                                                 

  ArrayIndex=0;                                                     //initiate ArrayIndex
  if(SpiNumBits==0)ClockPeriod=0xFFFF;                              //initiate auto mode
  for(i=Start;i<(bag_max_buf-1);i++){                               //scan for data

    if(BitReadPosition(i)){                                  

      if(SpiNumBits<=0){
        if(ByteArray[ArrayIndex].Index==i){                  //TargetBit is index, shifts through ALL bit positions to load ByteArray                     
          TargetBit=ByteArray[ArrayIndex].BitsPerWord;       //BitNumber = bits per word recorded from array. Here, varies from word to word
          BytesPerWord=(((TargetBit-1)/8)+1); 
          BitNumber=TargetBit; 
          if(SpiBitOrder)TargetBit=0;else TargetBit--;
        }else if(ByteArray[ArrayIndex].Index==0xFFFF){ByteArray[ArrayIndex].Index=0;break;}  //end of decoding flag
      }else BitNumber=SpiNumBits;                                                            //preset bit numbers

        if(((DataBuf[i]&0xFF)-ADCoffset)>Ch1TLevel){                                                //if ch A bit is set 
        ByteArray[ArrayIndex+((BytesPerWord-(TargetBit/8))-1)].Data|=1<<(TargetBit%8);                        
      }
        if((_3_source)&&(((((DataBuf[i]>>8)&0xFF)-ADCoffset))>Ch2TLevel)){                          //if ch C used as clock, read ch B bits
        ByteArray[ArrayIndex+((BytesPerWord-(TargetBit/8))-1)].DataB|=1<<(TargetBit%8);                        
      }

      if(SpiBitOrder){                                                                       //LSB FIRST "<"
        if(TargetBit==0){
          ByteArray[ArrayIndex].Index=i;                                                     //start of word
          if(_3_source)ByteArray[ArrayIndex].Index-=5;                                       //compensate for digital/analog ch offset
          if(SpiNumBits>0)ByteArray[ArrayIndex].BitsPerWord=SpiNumBits;
        }
        TargetBit++;
        if(TargetBit>=BitNumber){                                                            //advance bit position, shift to new word                                                           
          TargetBit=0;
          ByteArray[ArrayIndex].End=i;                                                       //end of word
          if(_3_source)ByteArray[ArrayIndex].End-=5;                  
          ArrayIndex+=BytesPerWord;
        }
      }else{                                                                                 //MSB FIRST ">"
        if(TargetBit==(BitNumber-1)){
          ByteArray[ArrayIndex].Index=i;                                                     //start of word
          if(_3_source)ByteArray[ArrayIndex].Index-=5;
          if(SpiNumBits>0)ByteArray[ArrayIndex].BitsPerWord=SpiNumBits;
        }
        if(TargetBit<=0){                                                                                                                         
          TargetBit=(BitNumber-1);
          ByteArray[ArrayIndex].End=i;                                                       //end of word
          if(_3_source)ByteArray[ArrayIndex].End-=5;              
          ArrayIndex+=BytesPerWord;
        }else TargetBit--; 
      }
      if(ArrayIndex>95){
        if(ArrayIndex==96)break;
        ArrayIndex-=BytesPerWord;
        ByteArray[ArrayIndex].Index=0;
        break;
      }               
    }
  }
  LoadChars(2);
}

u8 BitReadPosition(u16 i){                                        //flags a valid spi read position
static u16 SpiIdleSamples=0,PrevIPos=0;
register u8 PrevClock,Clock;                                    
u8 Read=0;
    if(_3_source){                                                //use ch C as clock if available
      Clock=((DataBuf[i-4]>>16)&1);
      PrevClock=((DataBuf[i-5]>>16)&1);
      switch(SpiMode){
        case 0:
        case 3:
          if((PrevClock==0)&&(Clock==1))Read=1;         
          break;        
        case 1:
        case 2:
          if((PrevClock==1)&&(Clock==0))Read=1;
      }
    }else{                                               //if ch C not available, use ch B as clock 
      Clock=((DataBuf[i+1]>>8)&0xFF)-ADCoffset;    
      PrevClock=((DataBuf[i]>>8)&0xFF)-ADCoffset;    
      switch(SpiMode){
        case 0:
        case 3:
          if((PrevClock<Ch2TLevel)&&(Clock>=Ch2TLevel))Read=1;         
          break;        
        case 1:
        case 2:
          if((PrevClock>=Ch2TLevel)&&(Clock<Ch2TLevel))Read=1;         
      }
    }

    if(SpiNumBits==0){
      if(ClockPeriod==0xFFFF)PrevIPos=0;                                     //initiate, ClockPeriod set at FFFF before engaging loops 
      if(Read){
        if((i-PrevIPos)<ClockPeriod)ClockPeriod=(i-PrevIPos);                //find shortest clock period to filter out longer spaces
        PrevIPos=i;
      } 
    }

    if((SpiNumBits<=0)||(SpiNumBits>8)){                                     //inter-clock bursts space detection               
      if(Read==0)SpiIdleSamples++;else SpiIdleSamples=0;
      if(SpiIdleSamples>4096)SpiIdleSamples=4096;
      if(SpiNumBits==0){
        if(SpiIdleSamples>(ClockPeriod*2))WordEndFlag=1;
      }else{ 
        if(SpiIdleSamples>(-SpiNumBits))WordEndFlag=1;     
      }
    }
    return Read;
}

void i2c(void){
u16 i;
u8 Frame=0;
u8 BitPosition=8;
u16 BitStore=0;
u16 Tmp=0;
register u8 PrevData,Data,PrevClock,Clock;                           

  for(i=0;i<96;i++){ByteArray[i].Data=0;ByteArray[i].Index=0;ByteArray[i].End=0;}
  ArrayIndex=0;

  for(i=90;i<(bag_max_buf-1);i++){                                   
    PrevData=((DataBuf[i-1]&0xFF)-ADCoffset);                        //ch a
    Data=((DataBuf[i]&0xFF)-ADCoffset);                              //ch a
    PrevClock=((DataBuf[i]>>8)&0xFF)-ADCoffset;                      //ch b           
    Clock=((DataBuf[i+1]>>8)&0xFF)-ADCoffset;                        //ch b  

    if((PrevData>=Ch1TLevel)&&(Data<Ch1TLevel)&&(Clock>Ch2TLevel))             //start/restart of frame        
      {Frame=1;BitPosition=8;BitStore=0;}             
    if((PrevData<Ch1TLevel)&&(Data>=Ch1TLevel)&&(Clock>Ch2TLevel))Frame=0;     //end of frame

    if(Frame){
      if(BitPosition==7){
        if((PrevClock>=Ch2TLevel)&&(Clock<Ch2TLevel)){                         //end of clock pulse
          Tmp=((i-ByteArray[ArrayIndex].Index)/2);                             //shift back by one half +clock pulse to center 
          ByteArray[ArrayIndex].Index-=Tmp;
        }
      }else if(BitPosition==0){
          if((PrevClock>=Ch2TLevel)&&(Clock<Ch2TLevel)){                       //end of clock pulse
            ByteArray[ArrayIndex].End=i-Tmp;                                   //end stored to account for clock stretching
          }
      }

      if((PrevClock<Ch2TLevel)&&(Clock>=Ch2TLevel)){                           //read position
        if(BitPosition==8){
          ByteArray[ArrayIndex].Index=i;
        }
        if(Data>Ch1TLevel)BitStore|=(1<<BitPosition);                          //store high bits   
        if(BitPosition)BitPosition--;                                
        else{                                                        //end of word after 9 bits
          BitPosition=8;
          if(Frame==1){                                              //address
            if((BitStore>>4)==0x1E){                                 //10 bit address id
              ByteArray[ArrayIndex].End|=0x1000;                     //set bit 12: 10bit address flag
              ByteArray[ArrayIndex].Data=(BitStore>>2)&3;            //display 2 MSB of 10 bit address         
            }else ByteArray[ArrayIndex].Data=(BitStore>>2);          //if 7 bit address
            if((BitStore>>1)&1)ByteArray[ArrayIndex].End|=0x8000;    //W/R bit 15 (0=W, 1=R)
            if(BitStore&1)ByteArray[ArrayIndex].End|=0x4000;         //Ack bit 14 (0=Ack, 1=NAck)
            ByteArray[ArrayIndex].End|=0x2000;                       //set bit 13 to id address word 
            Frame=2;                                                 //End bits 0-11 used to store ending position of word 
          }else{                                                     //Frame=2 > data frames
            ByteArray[ArrayIndex].Data=BitStore>>1;            
            if(BitStore&1)ByteArray[ArrayIndex].End|=0x4000;         //Ack bit 14 (0=Ack, 1=NAck)
          }
          BitStore=0;
          ArrayIndex++;if(ArrayIndex>95)break;                       //move to next byte
        }
      }
    }//if frame

  }//for scanning databuf
  LoadChars(1);
}

void DisplayDbScale(u16 Row, u8 buffer, u8 service){
u8 j=189;
s8 i,DB[8]={3,0,-2,-4,-7,-10,-14,-20};
s32 Tmp;
u8 Charact;
char DbString[4];

              if((AutoFFT==0)&&(service==0)){                                      //setup array to display log db scale
                for(i=-1;i<7;i++){
                  DB[i+1]=i*-10;
                }
              }

              if(Row>266){				
	        if((service==0)&&(DownConvertMode==0)){
                  Print_Str_Row(Row, LCD_Buffer[buffer], 267,189,  "db" );                        //print "db" on top line    
                }   
                for(i=0;i<8;i++){                                                                 //print db values in auto and manual modes
                  if(service==0){if(AutoFFT>0)Tmp=DB[i]-(FFTGain*6);else Tmp=DB[i];}
                    else Tmp=DB[i];                                                               //print db values for detector mode
                  s8ToDec2(DbString,Tmp);
                  if((Tmp>-10)&&(Tmp<10))Tmp=4; else Tmp=0;
                  if(FlagMeter==0)Tmp+=88;                                                          
		  Print_Str_Row(Row, LCD_Buffer[buffer], 284+Tmp,j,DbString); 
                  if(j==189)j-=19;else j-=25;   
                }
              }


            if(service==0){
              if(AutoFFT==0){                                                     //print v2 cursor db value
                Tmp=(-(((200-Title[V_VERNIE][V2].Value)*10)+12)/25)+10;
              }else{
                Tmp=Title[V_VERNIE][V2].Value;           //retrieve cursor position
                Tmp=(Tmp*100)/(100+((244-Tmp)/2));       //scale to get original value
                Tmp=((1300+(Tmp/2))/Tmp);                //convert to ratio, refered to 0db, upscaled by 10
                Charact=0;
                while(Tmp>99){                           //calculate log10
                  Tmp/=10;
                  Charact++;
                }
                Tmp=(2*((Charact*100)+Log10Mant[Tmp]))/10;         //convert to decibels, scaled back down 
                  if(Title[V_VERNIE][V2].Value>179){
                    Tmp=(Title[V_VERNIE][V2].Value-180)/5;
                  }else if(Title[V_VERNIE][V2].Value<29){
                    Tmp=DbScale[Title[V_VERNIE][V2].Value];
                  }else Tmp=-Tmp;   
                Tmp-=(FFTGain*6);                                  //add gain 
              }

              s8ToDec2(DbString,(s8)Tmp);
              if((Tmp>-10)&&(Tmp<10))Tmp=4; else Tmp=0;
              if(DownConvertMode){Tmp+=(6*8);COLOR_INDEX=0x0504;}
              Print_Str_Row(Row, LCD_Buffer[buffer],242+Tmp,189,DbString); 
              COLOR_INDEX=0x0503;
            }  

}

void DisplayFFtValues(u16 Row,u8 buffer){
u8 Yadj=0;
              if((_Mode!=NORHLD)&&((FlagMeter>0)||((DownConvertMode)&&(FFTt1Mode==0)))){
		if ( PeakFreq>5){

			if  ( RowMemPeak < 15)RowMemPeak = 15;
			if  ( RowMemPeak > 253)RowMemPeak = 253;
			if  (PeakFreq+14 > 178){
                          Print_Str_Row(Row, LCD_Buffer[buffer],  RowMemPeak+2,  178,  PeakFreqStr);
			} else {
                          Print_Str_Row(Row, LCD_Buffer[buffer],  RowMemPeak+2,  PeakFreq+14,  PeakFreqStr);
			}

		}
              }

                if(DownConvertMode)Yadj=(3*8);
		if(Row<54)Print_Str_Row(Row, LCD_Buffer[buffer], 21, 189,  "Div:" ); 			
		if(Row<(110+Yadj))Print_Str_Row(Row, LCD_Buffer[buffer], 53,189,  FreqDivStr);
		if((Row>(123+Yadj))&&(Row<(149+Yadj)))Print_Str_Row(Row, LCD_Buffer[buffer], 124+Yadj,189,  "T1:" ); 
		if((Row>(147+Yadj))&&(Row<(205+(Yadj*2))))Print_Str_Row(Row, LCD_Buffer[buffer], 148+Yadj,189, FreqT1Str);
                if(DownConvertMode)Yadj=(6*8);
		if((Row>(217+Yadj))&&(Row<(243+Yadj)))Print_Str_Row(Row, LCD_Buffer[buffer], 218+Yadj,189, "V2:");    

                if(Row>267){ 
                  if(DownConvertMode){
                    Print_Str_Row(Row, LCD_Buffer[buffer],274,61, "Start=");
                    Print_Str_Row(Row, LCD_Buffer[buffer],274,50, BaseFreqStr);
                    Print_Str_Row(Row, LCD_Buffer[buffer],274,21, "End=");
                    Print_Str_Row(Row, LCD_Buffer[buffer],274,10, NFreqStr);
                  }else{
  		    Print_Str_Row(Row, LCD_Buffer[buffer], 280, 1,  Nsuffix);
		    Print_Str_Row(Row, LCD_Buffer[buffer], 268,10,  Nfreq);
                  }
                }
}

void LoadBuffer(u16 Row, u8 buffer,u8 *y,u8 *Dot_Hide,u8 service){		
  u8  i=0,j=0,h=0,end; 
  u16 Col=0,Dim[4]={0x4200,0x0208,0x4008,0x0200};
 
  Dim[0]>>=FastDim;
  Dim[1]>>=FastDim;
 
 if((OsBufferLogicFlag)||(DigChLockout)||(OsChartFlag))end=3;
   else{if(ShowFFT)end=5;else end=7;}

   if( (_Mode==NORHLD)&&(Current==FILE)&&(Row>Limit)&&(_Det==3)&&(Title[9][0].Value==1) ){
     for(i=0;i<end;i++)if(y[i]>ListBottom){y[i]=ListBottom;Dot_Hide[i]=1;}                                         //limit to bottom of list
   }

 if(service){		//normal draw

   if((_T_base>7)||(Row!=skip) ||((_T_base>3)&&(_Mode!=SCAN))){
     for(j=0;j<end;j+=2){
       h=j/2;
       if((Dot_Hide[j] == 0)&&(Title[h][SOURCE].Value != HIDE)){                           
         Col=Color[h];
         if((y[j+1]-y[j])>5)Col-=Dim[h];
         for(i=y[j]; i<=y[j+1]; ++i) LCD_Buffer[buffer][i] |=Col;        
       }
     }
   }

   if((Row==ChartIndex)||(Row==(ChartIndex-1))){
     for(i=191;i<199;i++)LCD_Buffer[buffer][i] |= YEL_;        
   }

 }else{			//persistence mode 

      for(j=0;j<end;j+=2){
        h=j/2;
        if((Dot_Hide[j] == 0)&&(Title[h][SOURCE].Value != HIDE)){                           
          __Point_SCR(Row,MIN_Y+y[j]);
          Col=Color[h];
          if((Raw)&&(h<2)){
            __LCD_SetPixl(Col);
          }else{        
            if((y[j+1]-y[j])>5)Col-=Dim[h];
            for(i=y[j]; i<=y[j+1]; ++i) __LCD_SetPixl(Col);        
          }
        }
      }
 } 

}

void LoadFFTbuffer(u16 Row,u8 buffer){
#warning Gabo: FFT disabled
#if 0
  s16 fftx, val= 0;
  u8  i;  //,j; 

     if((Row > MIN_X)&&(Row < MAX_X)){
	// FFT ///////
	if (ShowFFT==1){

                if((ClearPerst)||(Update)||(_Mode!=NORHLD)||(UpdateBackground==3)){

                  if(DownConvertMode)COLOR_INDEX=0x0504;
                  DisplayFFtValues(Row,buffer);
                  COLOR_INDEX=0x0503;

                  if(!((_Mode==NORHLD)&&(AutoFFT==1)))	//AutoFFT=0 sets log, other values: 1=Auto on, 2-X  = AutoFFT OFF, =FFTgain+1        
                    {if(Row>241)DisplayDbScale(Row,buffer,0);}
                } 

		fftx = Row - MIN_X; // starts at 1
		if (fftx < (FFTSize/2)-1) {
                        val = fi[fftx];
			if (val >= 200) val = 199;
                        if(PeakFreq>198)PeakFreq=198;
                      
                      if(_Mode!=NORHLD){

                        if(fi[fftx+1]>173)TraceShadow(174,fi[fftx+1]+2,buffer);
                        if(fi[fftx-1]>173)TraceShadow(174,fi[fftx-1]+2,buffer);

                        for(i=Y_BASE+1;i<val; i++) LCD_Buffer[buffer][i] = GRN; if(val>173)LCD_Buffer[buffer][i]=BLACK;     
                        if((FlagMeter>0)||((DownConvertMode)&&(FFTt1Mode==0))){
  			  if ((fftx == imax - 4) || (fftx == imax + 4)) {
				for (i=PeakFreq - 1; i<PeakFreq+1; i++) {
					LCD_Buffer[buffer][i] = RED;
					RowMem=Row; 
				}
			  } else if ((fftx == imax - 3) || (fftx == imax + 3)) {
				LCD_Buffer[buffer][PeakFreq] = RED;
			  } else if ((fftx == imax - 5) || (fftx == imax + 5)) {
				for (i=PeakFreq - 2; i<PeakFreq+2; i++) { 
					LCD_Buffer[buffer][i] = RED;
				}
			  }
                        }

                      }else {

                        if( (Current==FILE)&&(Row>Limit)&&(_Det==3)&&(Title[9][0].Value==1) ){
                          if(val>ListBottom)val=ListBottom;
                        }

                        __Point_SCR(Row,MIN_Y);
                        for(i=Y_BASE+1;i<val; i++)__LCD_SetPixl(GRN);
                        __Point_SCR(Row, MIN_Y); 
                      }

		}  	//if fftx
	}      		//if show fft	
      }        		//if row
#endif
}



/*******************************************************************************
Print_Str_Row: 
*******************************************************************************/
void Print_Str_Row(u16 Row, u16* LCD_Buffer, u16 x0, s16 y0, char *s)	
{ 
	signed short i, y, b;  
        u16 w = x0;
        u8 Width;
        u8 YStart=0;
        if(y0<1)YStart=(-(y0))+1;                                                  //clip at bottom edge for moveable text
        
	while(*s != 0) {
                if(*s==127)Width=3;
                else if(*s==33)Width=4;
                else Width=CHAR_WIDTH;  
		if ((Row >= w) && (Row < (w + Width))) {
			i = Row - w;				 
                        if((*s==0x20)||(*s==33)) b = 0x0000; else b = Get_TAB_8x11(*s, i);	   //add space and half space characters	
			for (y = YStart; y < CHAR_HEIGHT; y++) {
				if((b << y)& 0x400) {
				  LCD_Buffer[y0 + y] = Color[COLOR_INDEX & 0x0F];   //Normal replace Display
				} else {
				  LCD_Buffer[y0 + y] = Color[COLOR_INDEX >> 0x8];   //Inverse replace Display
				}
			}
			break;
		}
		
		w += Width;
		s++;
	}
}

void VernierMark(u8 buffer,u16 color){
        LCD_Buffer[buffer][Y_SIZE]   = color;
        LCD_Buffer[buffer][Y_SIZE-1] = color;
        LCD_Buffer[buffer][Y_BASE+1] = color;
        LCD_Buffer[buffer][Y_BASE]   = color;
}

void LoadBaseBuffers(void){
u8 i;

    for(i=0;i<201;i++){
      Base2Buffer[i]=BLACK;
      Base3Buffer[i]=BLACK;
      Base4Buffer[i]=BLACK;
    }
    Base2Buffer[0]=GRAY; 
    Base2Buffer[200]=GRAY; 
    for(i=0;i<201;i+=25)Base3Buffer[i]=GRAY;
    for(i=0;i<201;i+=5)Base4Buffer[i]=GRAY;
    LoadNwave();
}


void TraceShadow(u8 start,u8 limit,u8 buffer){
u8 i;
  if(limit>199)limit=199;
  for(i=start;i<limit;i++)LCD_Buffer[buffer][i] = BLACK;			
}        


void Base1(u8 buffer){
u8 i;
  for(i=0;i<201;i++)LCD_Buffer[buffer][i]=GRAY;
}

void ClearFFTbuffer(void){
#warning Gabo: FFT disabled
#if 0
u16 i;
   for (i=0; i<256; i++)fi[i]=0;     						 
#endif
}

s32 ScaleXposi(void){
   if(_Kp1==246){
     return (MIN_X + 150)-(((102400*_X_posi.Value)+(KpCompensation()/2))/KpCompensation());
   }else if(_Kp1==1024){
     return (MIN_X + 150)-_X_posi.Value;
   }else{
     return (MIN_X + 150)-(((10240*_X_posi.Value)+(KpCompensation()/2))/KpCompensation());
   }   
}

void ClearScreenArea(u16 x1,u16 x2,u8 y1,u8 y2){
u16 i;
u8  j;
     for (i=x1;i<x2;i++){		      
       __Point_SCR(i,y1);
       for (j=y1;j<y2;j++){
          __LCD_SetPixl(BLACK);
       }                
     }  
}

void NFreqParse(u8 index,u8 suffix){
u8 j;
/*
     if(DownConvertMode){ 
       for(j=0;j<8;j++)Nfreq[j]=NFreqStr[j]; Nfreq[8]=0;            
       for(j=0;j<2;j++)Nsuffix[j]=NFreqStr[j+8]; Nsuffix[2]=0;      
     }else{
*/

     if(DownConvertMode==0){ 
       for(j=0;j<5;j++)Nfreq[j]=NFreqStr[j]; Nfreq[3+index+index]=0;            
       for(j=0;j<2;j++)Nsuffix[j]=NFreqStr[j+(4+index+suffix)]; Nsuffix[2]=0;      
     }

}

u16 ColorIndexGen(u8 level){                            //use instead of lookup table to save memory              
u16 color;
u8 r=0;  //5 bits  0-31
u8 g=0;  //6 bits  0-63
u8 b=0;  //5 bits  0-31

    if(level>198)level=198; 

    //RED
    if((level>100)&&(level<148)){			//ramp up 1/2 - 3/4, max 3/4 - 1
      r=((level-100)*2)/3;
    }else if(level>147)r=31;

    //GREEN
    if((level>50)&&(level<98)){				//ramp up 1/4 - 1/2, max 1/2 - 3/4, ramp down 3/4 - 1
      g=((level-50)*4)/3;
    }else if(level>150){
      g=((198-level)*4)/3;
    }else if(level>97) g=63;

    //BLUE
    if(level<48){					//ramp up 0 - 1/4, max 1/4 - 3/4, ramp down 3/4 - 1
      b=(level*2)/3;
    }else if(level>150){
      b=((198-level)*2)/3;
    }else b=31;

    color=r;
    color|=((u16)g<<5);
    color|=((u16)b<<11);
    return color; 
}

void DrawPixel(u16 x,u8 y, u16 color){
                  __Point_SCR(x,y); 
                  __LCD_SetPixl(color);
}

void DrawBase(u16 x,u16 color){
    DrawPixel(x,MIN_Y-1,color);
    DrawPixel(x,MAX_Y+1,color);
}

u8 UartLogic(void){
  return  ((Title[TRACK1][1].Value>2)&&(Title[TRACK1][1].Value<5)&&(_4_source<10)&&(_Mode!=X_Y)&&(!ChartLogic()));
}

u8 i2cLogic(void){
  return  ((Title[TRACK1][1].Value==5)&&(Title[TRACK2][1].Value==3)&&(_4_source<10)&&(_Mode!=X_Y)&&(!ChartLogic()));
}

u8 SpiLogic(void){
  return  ((Title[TRACK1][1].Value==6)&&(Title[TRACK2][1].Value==4)&&(_4_source<10)&&(_Mode!=X_Y)&&(!ChartLogic()));
}

void UpdateMarkLogic(void){
  if ((_Mode!=X_Y)&&(((_4_source!=SPEC_A)&&(_4_source!=SPEC_B))||(ChartLogic()))) Update_Mark();
}

void DisplayDeltaValues(u16 Row, u8 buffer){
char HalfSpace[2]={'!',0};
 if((FlagMeter==0)&&(DownConvertMode==0)&&((Current==T_VERNIE)||(Current==V_VERNIE))){
   COLOR_INDEX=0x0005+(u16)(_Meas_V_Track<<8);
   if(Row>314)Print_Str_Row(Row, LCD_Buffer[buffer],315,184,(char*)DELTA_V[0]);
   COLOR_INDEX=0x0500+_Meas_V_Track;
   if(Row>338){Print_Str_Row(Row, LCD_Buffer[buffer],339,184,HalfSpace);
               Print_Str_Row(Row, LCD_Buffer[buffer],343,184,VNumStr);}
   COLOR_INDEX=0x0405;
   if(Row>314)Print_Str_Row(Row, LCD_Buffer[buffer],315,169,(char*)DELTA_T[Title[10][2].Value]);
   COLOR_INDEX=0x0504;
   if(Row>338){Print_Str_Row(Row, LCD_Buffer[buffer],339,169,HalfSpace);
               Print_Str_Row(Row, LCD_Buffer[buffer],343,169,TNumStr);}
   COLOR_INDEX=0x0503;
 }
}

void DisplayCursorValues(u16 Row, u8 buffer){
   if(CursorDisplaySelect<4){
     COLOR_INDEX=0x0504;
   }else if(CursorDisplaySelect<8){
     COLOR_INDEX=0x0500+_Meas_V_Track;
   }else{
     COLOR_INDEX=0x0500+_Tr_source;
   }

   if(Row<42)Print_Str_Row(Row, LCD_Buffer[buffer],17,189,(char*)CursorTypeStr[CursorDisplaySelect/2]);
   if(Row>35){
     if(MinBypass==0){
       Print_Str_Row(Row, LCD_Buffer[buffer],17+(8*3),189,CursorDisplayStr);
     }else if(MinBypass==1){
       Print_Str_Row(Row, LCD_Buffer[buffer],17+(8*3),189,CursorDisplayStrM);
       Print_Str_Row(Row, LCD_Buffer[buffer],17+(8*7),189,CursorDisplayStr);      
     }else if(MinBypass==2){
       Print_Str_Row(Row, LCD_Buffer[buffer],17+(8*3),189,CursorDisplayStrH);
       Print_Str_Row(Row, LCD_Buffer[buffer],17+(8*7),189,CursorDisplayStrM);
       Print_Str_Row(Row, LCD_Buffer[buffer],17+(8*11),189,CursorDisplayStr);
     }
   }
   COLOR_INDEX=0x0503;
}


/******************************** END OF FILE *********************************/
















