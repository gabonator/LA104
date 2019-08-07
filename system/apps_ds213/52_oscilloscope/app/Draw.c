/******************** (C) COPYRIGHT 2018 e-Design Co.,Ltd. *********************
  DS213_APP Draw.c                                               Author : bure
*******************************************************************************/
#include "Draw.h"
#include "Sys.h"

void PopPixle(u8 Hue);
void DrawTags(void);
void SetPopSide(u8 Hue);
void SetPopWindow(u16 x1, u16 y1, u16 x2, u16 y2);

uc8  TAG_IMG[][7] = {{0x00,0x3E,0x68,0xC8,0x68,0x3E,0x00,},    // Tag A
                     {0x00,0xFE,0x92,0x92,0x92,0x6C,0x00,},    // Tag B
                     {0x00,0x7C,0x82,0x82,0x82,0x44,0x00,},    // Tag C
                     {0x00,0xFE,0x82,0x82,0x44,0x38,0x00}};    // Tag D

uc8  BUF_IMG[]    =  {0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x10};
                     
uc16 CLK_IMG[]    =  {0x000,0x000,0x000,0x000,0x070,0x3FF,0x070,0x000,
                      0x000,0x000,0x000,0x000,0x002,0x004,0x008,0x070,
                      0x070,0x070,0x080,0x100,0x200,0x000,0x020,0x020,
                      0x020,0x020,0x070,0x070,0x070,0x020,0x020,0x020,
                      0x020,0x000,0x200,0x100,0x080,0x070,0x070,0x070,
                      0x008,0x004,0x002,0x000};

uc16 FONT_8x11[]  =  {0x008,0x004,0x008,0x020,0x080,0x100,0x080,0x020, // " 正弦波
                      0x010,0x004,0x010,0x040,0x100,0x400,0x100,0x040, // # 三角波
                      0x200,0x100,0x080,0x040,0x020,0x010,0x008,0x3FC, // $ 锯齿波
                      0x30C,0x18C,0x0C0,0x060,0x030,0x318,0x30C,0x000, // % 
                      0x000,0x180,0x260,0x21C,0x26A,0x284,0x140,0x000, // & 
                      0x202,0x202,0x202,0x202,0x202,0x202,0x3FE,0x000, // ' 电池尾空
                      0x000,0x000,0x0F8,0x1FC,0x306,0x202,0x000,0x000, // (
                      0x000,0x000,0x202,0x306,0x1FC,0x0F8,0x000,0x000, // )
                      0x000,0x18C,0x0D8,0x070,0x070,0x0D8,0x18C,0x000, // *  ×
                      0x000,0x020,0x020,0x0F8,0x0F8,0x020,0x020,0x000, // + 
                      0x004,0x004,0x004,0x3FC,0x200,0x200,0x200,0x3FC, // , 矩形波
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
                      0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA, // ; 电池身
                      0x000,0x020,0x070,0x0D8,0x18C,0x306,0x202,0x000, // <
                      0x090,0x090,0x090,0x090,0x090,0x090,0x000,0x000, // = 
                      0x000,0x202,0x306,0x18C,0x0D8,0x070,0x020,0x000, // >
                      0x018,0x01C,0x004,0x344,0x364,0x03C,0x018,0x000, // ? 
                      0x1F8,0x104,0x272,0x272,0x2F2,0x284,0x078,0x000, // @
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
                      0x200,0x300,0x2C0,0x220,0x218,0x204,0x3FE,0x000, // [ 三角       
                      0x000,0x022,0x042,0x1FE,0x3FE,0x240,0x220,0x000, // \下降沿   
                      0x020,0x010,0x008,0x006,0x008,0x010,0x020,0x020, // ] 指针    
                      0x000,0x220,0x210,0x3FC,0x3FE,0x012,0x022,0x000, // ^ 上升沿  
                      0x000,0x200,0x200,0x200,0x200,0x200,0x200,0x000, // _ 下横线  
                      0x202,0x202,0x202,0x202,0x202,0x202,0x202,0x202, // ` 电池身空
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
                      0x0F8,0x088,0x38E,0x222,0x2FA,0x2FA,0x2FA,0x2FA, // { 电池头
                      0x000,0x000,0x000,0x3FE,0x3FE,0x000,0x000,0x000, // | 
                      0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x202,0x3FE,0x000, // } 电池尾
                      0x0F8,0x088,0x38E,0x202,0x202,0x202,0x202,0x202}; // ~ 空电池头

uc16 Palette[] = {CYAN, YEL, PRPL, GRN, CYAN_, YEL_, PRPL_, GRN_, // 主屏显示调色板
                  BLK,  RED, WHT,  GRN, BLUE,  GRAY, ORNG,  DARK};
                 
uc16 HueTab[4] = {GRAY, CYAN, YEL, WHT}; // Pop Window 色调编号（color(0~1)bit, 透明 0/1（2~3)bit

u8   PopHide = 1;              // Pop Window 显示/消隐标志
u32  PopBuf[1500];             // Pop Window 显示缓冲区
u16  Nx0, Ny0;                 // Pop Window 内信息当前显示位置
u16  Bx1, Bx2, By1, By2;       // Pop Window 在主屏显示区中起始位置
u8   YnHide[4] = {0, 0, 0, 0}; // Track1~4迹显示/消隐标志
u8   Track[(X_SIZE+1)*4];      // 轨迹缓存: i+0,i+1,i+2,i+3, 为 1～4 号轨迹数据

/*******************************************************************************
  Setup Pop Window 
*******************************************************************************/
void SetPopWindow(u16 x1, u16 y1, u16 x2, u16 y2)
{
  Bx1 = x1, Bx2 = x2, By1 = y1, By2 = y2;
  for(u32 i = 0; i < 1500; i++) PopBuf[i] = 0; // Background: Gray Color
  SetPopSide(3);                               // Side Line: White Color
  PopHide = 0;                                 // Enbale Pop Window
}
/*******************************************************************************
  
*******************************************************************************/
void PopProcess(void)
{ 
  u8 r = 0;
  Nx0 = Pop[7].X0, Ny0 = Pop[7].Y0;
  PopSTR(Pop[7].Src, 0, (u8*)Pop[7].STR+11);     // 清除 " Press K3 "
  Nx0 = Pop[8].X0, Ny0 = Pop[8].Y0;
  if(Line >= SVOL) r = SaveParam();              // 保存当前设置参数
  if(Line == SWAV) r = SaveTrack(Pop[Line].Val);
  if(Line == LWAV) r = LoadTrack(Pop[Line].Val);
  if(Line == SBUF) r = SaveBuf(Pop[Line].Val);
  if(Line == SCSV) r = SaveCsv(Pop[Line].Val);
  u8 n = (r == OK) ? 11 : 22;
  PopSTR(Pop[8].Src, 0, (u8*)Pop[8].STR+n); // 显示 "Completed " 或 "  Error   "
  DisplayWaveForm(); 
  Delay_mS(1000);
  PopUpdt = 1;
}
/*******************************************************************************
  UpdtPopMenu 
*******************************************************************************/
void UpdtPopMenu(void)
{
  SetPopWindow(200, 40, 200+11*8, 40+8*18);
  u8 NumStr[6];
  for(u32 i = 0; i < 7; i++){
    Nx0 = Pop[i].X0, Ny0 = Pop[i].Y0;
    PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, (u8*)Pop[i].STR);
    if(Pop[i].Flg & FNUM){
      u16To5DecStr(NumStr, Pop[i].Val);
      PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, (u8*)&NumStr[2]);
    } else if(Pop[i].Flg & PCNT){
      u16To4DecStr(NumStr, Pop[i].Val);
      PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, (u8*)&NumStr[1]);
      PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, "%");
    } else if(Pop[i].Flg & TCNT){                       // 设置休眠计时器
      if(Pop[i].Val > 0){
        u16To5DecStr(NumStr, Pop[i].Val);
        PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, (u8*)&NumStr[3]);
        PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, "m");
        PD_Cnt = Pop[i].Val*60;                         // 重置休眠计时器
        PwrDownEn = 1;
      } else {
        PopSTR(Pop[i].Src, (i == Line) ? 1 : 0, "Off");
        PD_Cnt = 1000*60;                               // 重置休眠计时器
        PwrDownEn = 0;
      }
    }
    if(i == SBKL) *Hw.pPwm_Bkl = Pop[i].Val;
    if(i == SVOL) *Hw.pBuz_Vol = Pop[i].Val;
    if(i == SPDT){
      if(Pop[i].Val > 0) PD_Cnt = Pop[i].Val*60;      
      else               PwrDownEn = 0; 
    }
  }
  Nx0 = Pop[6+1].X0, Ny0 = Pop[6+1].Y0;
  PopSTR(Pop[6+1].Src, 0, (u8*)Pop[6+1].STR);
  Nx0 = Pop[7+1].X0, Ny0 = Pop[7+1].Y0;
  PopSTR(Pop[7+1].Src, 0, (u8*)Pop[7+1].STR);
}
/*******************************************************************************
  Pop Window 像素设置     HueNum: (0~3)     像素定位: Nx0, Ny0
*******************************************************************************/
void PopPixle(u8 HueNum)
{
  u16 i = Nx0*((By2-By1+15)/16)+Ny0/16, j = 2*(Ny0%16);
  
  PopBuf[i] &= ~(3 << j);
  PopBuf[i] |= HueNum << j;
  if(++Ny0 == (By2-By1)) Ny0 = 0, Nx0++;
}
/*******************************************************************************
  Pop Window 字符串打印    Hue: BackGrndNum bit2~3 + ForeGrndNum bit0~1
*******************************************************************************/
void PopSTR(u8 Hue, u8 Inv, u8 *s)
{
  u16 ForeGrnd = Inv ? (Hue &  3) : (Hue >> 2);
  u16 BackGrnd = Inv ? (Hue >> 2) : (Hue &  3);
  
  while(*s != 0){
    Nx0--;
    for(u32 i = 0; i < 12; i++) PopPixle(ForeGrnd); // 字符前增加一空白列
    Nx0++, Ny0 -= 12;
    for(u32 i = 0; i < 8; i++){
      u32 b = (*s <= 0x21) ? 0 : FONT_8x11[((*s-0x22)*8)+i];
      if((*s == 0x21) && (i == 4)) break;
      for(u32 j = 0; j < 12; ++j){
        PopPixle(((b << j) & 0x800) ? BackGrnd : ForeGrnd);
      }
      Nx0++, Ny0 -= 12;
    }
    s++;
  }
}
/*******************************************************************************
  Pop Window 边框打印     Hue: BackGrndNum bit2~3 + ForeGrndNum bit0~1
*******************************************************************************/
void SetPopSide(u8 Hue)
{
  u16 i = Bx2-Bx1-1, j = By2-By1-1;

  for(u32 x = 1; x < i; x++){
    Nx0 = x, Ny0 = 1;
    for(u32 y = 1; y < j; y++) PopPixle(Hue >> 2);
  }
  for(u32 x = 2; x < i; x++){
    Nx0 = x, Ny0 = 0, PopPixle(Hue);
    Nx0 = x, Ny0 = j, PopPixle(Hue);
  }
  Nx0 = 0, Ny0 = 2;
  for(u32 y = 2; y < j; y++) PopPixle(Hue);
  Nx0 = i, Ny0 = 2;
  for(u32 y = 2; y < j; y++) PopPixle(Hue);
  
  Nx0 = 1,   Ny0 = 1,   PopPixle(Hue);
  Nx0 = i-1, Ny0 = 1,   PopPixle(Hue);
  Nx0 = 1,   Ny0 = j-1, PopPixle(Hue);
  Nx0 = i-1, Ny0 = j-1, PopPixle(Hue);
}
/*******************************************************************************
 
*******************************************************************************/
void RowPosi(u16 x, u16 y)
{
  LCD_WrBlock(x, y, Hw.MaxLcdCol, y+11);
}
/*******************************************************************************
 
*******************************************************************************/
void PrintStr(u8 ColorNo, u8 Mode, u8 *Str)
{
  u16 ForeGrnd = Palette[(Mode) ? BLANK : ColorNo];
  u16 BackGrnd = Palette[(Mode) ? ColorNo : BLANK];
  for(u32 j = 0; j < 12; ++j) WrPxl(BackGrnd);
  while(*Str != 0){
    for(u32 i = 0; i < 8; i++){
      u16 b = (*Str <= 0x21) ? 0 : FONT_8x11[((*Str-0x22)*8)+i];
      if((*Str == 0x21) && (i == 4)) break;
      for(u32 j = 0x800; j > 0; j >>= 1) WrPxl((b & j) ? ForeGrnd : BackGrnd);
    }
    Str++;
  }
}
/*******************************************************************************
 
*******************************************************************************/
void AddStr(u8 ColorNo, u8 Mode, u8 *Str)
{
  u16 ForeGrnd = Palette[(Mode) ? BLANK : ColorNo];
  u16 BackGrnd = Palette[(Mode) ? ColorNo : BLANK];
  while(*Str != 0){
    for(u32 i = 0; i < 8; i++){
      u32 b = (*Str <= 0x21) ? 0 : FONT_8x11[((*Str-0x22)*8)+i];
      if((*Str == 0x21) && (i == 4)) break;
      for(u32 j = 0x800; j > 0; j >>= 1) WrPxl((b & j) ? ForeGrnd : BackGrnd);
    }
    Str++;
  }
}
/*******************************************************************************
 PrintClk: 进度指示
*******************************************************************************/
void PrintClk(u16 x, u16 y, u8 Phase)
{
  LCD_WrBlock(x, y, x+10, y+10);
  for(u32 i = 0; i < 11; ++i){
    u16 b = CLK_IMG[i+Phase*11];
    for(u32 j = 0; j < 11; ++j){
      WrPxl(Palette[((b >> j) & 1) ? BLANK : TXT1C]);
    }
  }
}
/*******************************************************************************

*******************************************************************************/
void DrawTags(void)
{
  u16 Buf0[256], Buf1[256];
  LCD_WrBlock(0, MIN_Y-1, 10, MIN_Y+Y_SIZE+1); // 设 Tag 显示窗
  for(u16 x = 0; x <= 10; x++){
    u16* p = (x & 1) ? Buf1 : Buf0; 
    u32* p32 = (u32*)p;
    for(u32 i = 0; i < (Y_SIZE+4)/2; i++) p32[i] = BLK;
    for(u32 i = 0; i < 4; i++){
      u32 Posi  = Yn[i]+1; 
      u32 Color = Palette[i];
      u8* pTab  = (u8*)&TAG_IMG[i][0];
      if(!YnHide[i] && (Item != YNP || Menu[YNP].Src != i || Flashing)){
        if(Posi >= 3 || Posi <= Y_SIZE-3){
          Posi -= 4;
          if(x < 7){
            u16 Tag = ~pTab[x];
            for(u32 y = 0; y < 9; y++) if((Tag >> y) & 1) p[Posi+y] |= Color;
          } else  for(u32 y = x-6; y < (15-x); y++) p[Posi+y] |= Color;
        }
      }
    }
    Hw.pLCD_DmaWait();
    Hw.pLCD_DmaSend((u32)p, Y_SIZE+3);
  }
  Hw.pLCD_DmaWait();
}
/*******************************************************************************
  更新信号标签
*******************************************************************************/
void UpdateTag(void)
{
  DrawTags();
  PxlPosi(MAX_X+1, MIN_Y-1);
  for(u8 y = MIN_Y-1; y < MAX_Y+2; y++) WrPxl(Palette[BLANK]); // Clear last col
  for(u16 x = MIN_X; x <= MAX_X+1; x++){
    PxlPosi(x, MIN_Y - 1);
    WrPxl(Palette[BLANK]);                    // Clear first row
    PxlPosi(x, MAX_Y + 1);
    WrPxl(Palette[BLANK]);                    // Clear last row
  }
  u16 V1C = Palette[Menu[V_1].Src];
  u8  V1n = Menu[V_1].Val+MIN_Y-2;
  PxlPosi(MIN_X-1, V1n);
  for(u8 y = 0; y < 5; y++) WrPxl(V1C);       // Left Side Tag
  PxlPosi(MAX_X+1, V1n);
  for(u8 y = 0; y < 5; y++) WrPxl(V1C);       // Right Side Tag
  
  u16 V2C = Palette[Menu[V_2].Src];
  u8  V2n = Menu[V_2].Val+MIN_Y-2;
  PxlPosi(MIN_X-1, V2n);
  for(u8 y = 0; y < 5; y++) WrPxl(V2C);       // Left Side Tag
  PxlPosi(MAX_X+1, V2n);
  for(u8 y = 0; y < 5; y++) WrPxl(V2C);       // Right Side Tag
  
  u16 T0C = Palette[Menu[V_T].Src];
  s16 T0P = (Menu[T_0].Val-Menu[XNP].Val)*30; // T0 标线位置
  if((T0P > 0)&&(T0P < X_SIZE)) for(u16 x = T0P+MIN_X-2; x < T0P+MIN_X+3; x++){
    PxlPosi(x, MIN_Y-1);
    WrPxl(T0C);                               // T0 Bottom Side Tag
    PxlPosi(x, MAX_Y+1);
    WrPxl(T0C);                               // T0 Top Side Tag
  }
  u16 T1C = Palette[Menu[T_1].Src];
  u16 T1P = Menu[T_1].Val;
  for(u16 x = T1P+MIN_X-2+0; x < T1P+MIN_X+3; x++){
    PxlPosi(x, MIN_Y-1);
    WrPxl(T1C);                               // T1 Bottom Side Tag
    PxlPosi(x, MAX_Y+1);
    WrPxl(T1C);                               // T1 Top Side Tag
  }
  u16 T2C = Palette[Menu[T_2].Src];
  u16 T2P = Menu[T_2].Val;
  for(u16 x = T2P+MIN_X-2+0; x < T2P+MIN_X+3; x++){
    PxlPosi(x, MIN_Y-1);
    WrPxl(T2C);                               // T1 Bottom Side Tag
    PxlPosi(x, MAX_Y+1);
    WrPxl(T2C);                               // T1 Top Side Tag
  }
}
/*******************************************************************************

*******************************************************************************/
void DisplayWaveForm(void)
{ 
  u16 Buf0[210], Buf1[210];
  u16 T0P = (Menu[T_0].Val-Menu[XNP].Val)*30;                // T0 标线位置
  u16 T1P = Menu[T_1].Val, T2P = Menu[T_2].Val;              // T1,T2 游标位置
  u16 V1P = Menu[V_1].Val, V2P = Menu[V_2].Val;              // V1,V2 游标位置
  u16 VtS = Menu[V_T].Src;                                   // Vt 来源
  u16 VtP = Vt[VtS];                                         // Vt 标线位置
  u16 VtC = Palette[VtS];                                    // Vt 信源颜色
  u16 GrC = Palette[GRIDC];                                  // 格线颜色
  u16 BgC = Palette[BLANK];                                  // 背景颜色
  u16 CsC = Palette[CURSR];                                  // V/T1~2 游标颜色

  u16 T0F = Menu[T_0].Flg & INVR;                            // T0 标线显示标志
  u16 T1F = Menu[T_1].Flg & INVR;                            // T1 游标显示标志
  u16 T2F = Menu[T_2].Flg & INVR;                            // T1 游标显示标志
  u16 V1F = Menu[V_1].Flg & INVR;                            // V1 游标显示标志
  u16 V2F = Menu[V_2].Flg & INVR;                            // V2 游标显示标志
  u16 VtF = Menu[V_T].Flg & INVR;                            // Vt 游标显示标志
  
  FpsCnt++;
  LCD_WrBlock(MIN_X, MIN_Y, MIN_X+X_SIZE, MIN_Y+Y_SIZE);          // 设显示区域
  for(u32 Col = X_BASE; Col <= X_SIZE; Col++){
    u16* p = (Col & 1) ? Buf1 : Buf0;                             // 切换缓冲区
    u32* p32 = (u32*)p;
    u16 Fill = (Col == X_BASE || Col == X_SIZE) ? GrC : BgC;
    for(u32 i = X_BASE; i <= Y_SIZE/2; i++) p32[i] = Fill;        // 填充底色
    p[Y_SIZE] = GrC, p[Y_BASE] = GrC;                             // 画上下框线
    if(Col%30 == 0) for(u32 y = 5;  y < 200; y +=  5) p[y] = GrC; // 画竖格线
    if(Col% 6 == 0) for(u32 x = 25; x < 200; x += 25) p[x] = GrC; // 画横格线
    
    if(Col != X_BASE && Col != X_SIZE){
      for(u32 n = 0; n < 4; n++){
        u32 m = Col*4+n;
        u32 Max[4], Min[4];
        if(Track[m] != 0xFF){                                     // 非消隐状态
          Max[n] = Track[m], Min[n] = Track[m];
          if(Track[m] > Y_BASE && Track[m] < Y_SIZE){
            if(Track[m-4] < Y_BASE+1)      Min[n] = Y_BASE+1;
            else if(Track[m-4] > Y_SIZE-1) Max[n] = Y_SIZE-1;
            else if(Track[m] > Track[m-4]) Min[n] = Track[m-4];
            else if(Track[m] < Track[m-4]) Max[n] = Track[m-4];
            if(Min[n] == Max[n]){
              if(Min[n] > Y_BASE+1) Min[n]--;
              if(Max[n] < Y_SIZE-1) Max[n]++;                     // 水平线加粗
            }
          } else {                                                // 超界处理
            if(Track[m] == Y_BASE && Track[m-4] > Y_BASE){
              Min[n] = Track[m-4], Max[n] = Track[m-4];
            }
            if(Track[m] >= Y_SIZE && Track[m-4] < Y_SIZE){
              Min[n] = Track[m-4], Max[n] = Track[m-4];
            }
          }
          u16 TrkC = Palette[n];
          if((Max[n]-Min[n]) > 5) TrkC &= 0xBDF7;                 // 亮度调整 
          if(Col != X_BASE && Col != X_SIZE && !YnHide[n]){
            for(u32 y = Min[n]; y <= Max[n]; y++){
              if(Min[n] > Y_BASE && Max[n] < Y_SIZE) p[y] |= TrkC;// 画波形轨迹
            }
          }
        }
      }
      if(Col%3 == 0) {
        if(V1F) p[V1P] |= CsC;                               // 画 V1 游标线
        if(V2F) p[V2P] |= CsC;                               // 画 V2 游标线
        if(VtF) p[VtP] |= VtC;                               // 画 Vt 游标线
      }
    } else {
      p[V1P] |= CsC, p[V1P-1] |= CsC, p[V1P+1] |= CsC;       // 画 V1 游标端点
      p[V2P] |= CsC, p[V2P-1] |= CsC, p[V2P+1] |= CsC;       // 画 V2 游标端点
      p[VtP] |= VtC, p[VtP-1] |= VtC, p[VtP+1] |= VtC;       // 画 Vt 游标端点
    }
    if(Col == X_BASE+1 || Col == X_SIZE-1){
      p[V1P] |= CsC, p[V2P] |= CsC, p[VtP] |= VtC;           // 画 V1,V2,Vt 端点
    }
    if(Col == T1P && T1F)
      for(u32 y = 1; y <= Y_SIZE; y += 3) p[y] |= CsC;       // 画 T1 游标线
    if(Col == T2P && T2F) 
      for(u32 y = 1; y <= Y_SIZE; y += 3) p[y] |= CsC;       // 画 T2 游标线
    
    if((Col == T1P) || (Col == T2P)){                        // 画 T1~2 游标端点
      p[Y_SIZE] = CsC, p[Y_SIZE-1] = CsC;
      p[Y_BASE] = CsC, p[Y_BASE+1] = CsC;
    }
    if(Col == T1P-1 || Col == T1P+1 || Col == T2P-1 || Col == T2P+1){
      p[Y_SIZE] = CsC, p[Y_BASE] = CsC;
    }
    if(Col == T0P){
      if(T0F) for(u32 y = 1; y <= Y_SIZE; y += 3) p[y] |= VtC; // 画 T0 游标线
      p[Y_SIZE] = VtC, p[Y_SIZE-1] = VtC;
      p[Y_BASE] = VtC, p[Y_BASE+1] = VtC;                      // 画 T0 游标端点
    }
    if(Col == T0P-1 || Col == T0P+1){
      p[Y_SIZE] = VtC, p[Y_BASE] = VtC;
    }
    u32 *k = (Col-Bx1)*((By2-By1)/16)+PopBuf;
    if(PopHide == 0){
      if(Col >= Bx1 && Col < Bx2){                             // 叠加弹出窗
        for(u32 i = By1; i < By2; i += 16){
          for(u32 j = 0; j < 32; j += 2){
            u16 y = i+j/2;
            if(y > Y_BASE && y < Y_SIZE) p[y] = HueTab[(*k >> j)&3]; 
          }
          k++;
        }
      }
    }
    Hw.pLCD_DmaWait();
    Hw.pLCD_DmaSend((u32)p, Y_SIZE + 1);
  }
  Hw.pLCD_DmaWait(); 
}
/*******************************************************************************
 Litimg: 缓冲区及显示窗口位置缩略图 
*******************************************************************************/
void Litimg(void)
{
  #define IMG_X0      109
  #define IMG_Y0      215

  u16 ForeGrnd = Palette[XATTR], BackGrnd = Palette[BLANK];
  RowPosi(IMG_X0, IMG_Y0);
  for(u32 n = 0; n < 24; n++){                         // 画缓冲区缩略图
    u8* p = (u8*)BUF_IMG;
    for(u32 i = 0; i < 8; i++){
      for(u32 k = 0; k < 12; k++) WrPxl(((*p >> k) & 1) ? ForeGrnd : BackGrnd);
      p++;
    }
  }
  for(u32 y = 0; y < 24; y++) WrPxl(ForeGrnd);         // End line
  RowPosi(IMG_X0, IMG_Y0);
  for(u32 y = 0; y < 24; y++) WrPxl(ForeGrnd);         // Start Line
  
  u16 ViewX = IMG_X0+2+(Menu[XNP].Val*30*192)/4096, ViewW = (360*192)/4096;
  PxlPosi(ViewX, IMG_Y0+1);
  for(u32 y = 0; y < 9; y++) WrPxl(WHT);
  LCD_WrBlock(ViewX, IMG_Y0+1, ViewX+ViewW, IMG_Y0+1);
  for(u32 x = 0; x <= ViewW; x++) WrPxl(WHT);
  LCD_WrBlock(ViewX, IMG_Y0+10, ViewX+ViewW, IMG_Y0+10);
  for(u32 x = 0; x <= ViewW; x++) WrPxl(WHT);
  PxlPosi(ViewX+ViewW, IMG_Y0+1);
  for(u32 y = 0; y < 9; y++) WrPxl(WHT);
  
  u16 T0Ptr = IMG_X0+1+(Menu[T_0].Val*30*24*8)/0x1000;
  LCD_WrBlock(T0Ptr-2, IMG_Y0+10, T0Ptr+3, IMG_Y0+11);
  ForeGrnd = Palette[Menu[V_T].Src];                  // 由触发来源选择指针颜色
  for(u32 n = 0; n < 12; ++n) WrPxl(ForeGrnd);
  LCD_WrBlock(T0Ptr,IMG_Y0,  T0Ptr+1, IMG_Y0+9);
  for(u32 n = 0; n < 20; ++n) WrPxl(ForeGrnd);        // 画触发位置指针标识
}

/******************************** END OF FILE *********************************/
