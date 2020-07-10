/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : GUI.c
 * Description : 图像显示模块
 * Author      : 
 * History     :
*******************************************************************************/
#include "AppBios.h"
#include "GUI.h"
#include "Font.h"
#include "Interrupt.h"

u16 Background, Foreground;

/*******************************************************************************
 * FunctionName : SetPosition
 * Description  : 设置显示的位置
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
*******************************************************************************/
void Set_Posi(u16 x, u16 y)
{
    __Bios(PIXEL_X, x);
    __Bios(PIXEL_Y, y);
}

/*******************************************************************************
 * FunctionName : SetPixel
 * Description  : 设置像素颜色
 * Param        : u16 Color 
*******************************************************************************/
void Set_Pixel(u16 Color)
{
    __Bios(WrPIXEL, Color);
}

/*******************************************************************************
 * FunctionName : ReadPixel
 * Description  : 读取像素颜色
 * Return       : u16 返回颜色值  
*******************************************************************************/
u16 Read_Pixel(void)
{
    u16 Color = 0;
    Color =  __Bios(RdPIXEL, 0);

    return Color;
}

/*******************************************************************************
 * FunctionName : Add_Pixel_Color
 * Description  : 指定位置设置颜色
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
 * Param        : u16 Color 设置颜色
*******************************************************************************/
void Add_Pixel_Color(u16 x, u16 y, u16 Color)
{
    __Bios(PIXEL_X, x);
    __Bios(PIXEL_Y, y);
    __Bios(WrPIXEL, Color);
}

/*******************************************************************************
 * FunctionName : Read_Pixel_Color
 * Description  : 读取指定位置颜色
 * Param        : u16 x 坐标 
 * Param        : u16 y 坐标
 * Return       : u16 返回颜色值  
*******************************************************************************/
u16 Read_Pixel_Color(u16 x, u16 y)
{
    u16 Color = 0;

    __Bios(PIXEL_X, x);
    __Bios(PIXEL_Y, y);
    Color = __Bios(RdPIXEL, 0);

    return Color;
}

/*******************************************************************************
 * FunctionName : Set_Block
 * Description  : 指定显示块
 * Param        : u16 x1 坐标
 * Param        : u16 y1 坐标
 * Param        : u16 x2 坐标
 * Param        : u16 y2 坐标
*******************************************************************************/
void Set_Block(u16 x1, u16 y1, u16 x2, u16 y2)
{
    __Bios(BLOCK_X, (x1 << 16) + x2);
    __Bios(BLOCK_Y, (y1 << 16) + y2);
}


/*******************************************************************************
 * FunctionName : Clr_Scrn
 * Description  : 清LCD显示器屏幕
 * Param        : u16 Color 
*******************************************************************************/
void Clr_Scrn(u16 Color)
{
    u32 i;
    Set_Posi(0, 0);
    for(i = 0; i < LCD_COL * LCD_ROW; i++) Set_Pixel(Color);
}

/*******************************************************************************
 * FunctionName : Disp_Str8x14
 * Description  : 指定位置显示带颜色字符串
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
 * Param        : u16 Foreground  前景色
 * Param        : u16 Background  背景色
 * Param        : u8  Mode PRN/INV/SYMB/VOID 正常/反衬/图块/虚体   
 * Param        : u8* Str 
*******************************************************************************/
void Disp_Str8x14(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 *Str)
{
    u8  Code;
    u16 i, j, k;
    u16 Sx, Sy;

    Sx = x;
    Sy = y;
    while(*Str != 0)
    {
        Code = *Str++;
        for(i = 0; i < 8; ++i)
        {
            if(Mode & SYMB);
            else            k = Get_TAB_8x14(Code, i);
            if(Mode & VOID) k &= (i % 2) ? 0xAAAA : 0x5555;
            Set_Posi(Sx, Sy);
            for(j = 0; j < 14; ++j)
            {
                if(k & 4) Set_Pixel((Mode & INV) ? Bg : Fg);
                else      Set_Pixel((Mode & INV) ? Fg : Bg);
                k >>= 1;
            }
            Sx++;
        }
    }
}

/*******************************************************************************
 * FunctionName : Disp_Char8x14
 * Description  : 指定位置显示带颜色字符
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
 * Param        : u16 Fg：  Foreground 前景色
 * Param        : u16 Bg：  Background 背景色
 * Param        : u8  Mode PRN/INV/SYMB/VOID 正常/反衬/图块/虚体  
 * Param        : u8  Code 
*******************************************************************************/
void Disp_Char8x14(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Code)
{
    u16 i, j, k;
    u16 Sx, Sy;

    Sx = x;
    Sy = y;
    for(i = 0; i < 8; ++i)
    {
        if(Mode & SYMB);
        else            k = Get_TAB_8x14(Code, i);
        if(Mode & VOID) k &= (i % 2) ? 0xAAAA : 0x5555;
        Set_Posi(Sx, Sy);
        for(j = 0; j < 14; ++j)
        {
            if(k & 4) Set_Pixel((Mode & INV) ? Bg : Fg);
            else      Set_Pixel((Mode & INV) ? Fg : Bg);
            k >>= 1;
        }
        Sx++;
    }
}

/*******************************************************************************
 * FunctionName : Disp_Str6x8
 * Description  : 显示6X8字符 
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
 * Param        : u16 Fg：  Foreground 前景色
 * Param        : u16 Bg：  Background 背景色
 * Param        : u8  Mode  Mode PRN/INV/SYMB/VOID 正常/反衬/图块/虚体  
 * Param        : u8  Style THIN/BOLD 细体/粗体
 * Param        : u8* Str 
*******************************************************************************/
void Disp_Str6x8(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Style, u8 *Str)
{
    u8  Code, Width;
    u16 i, j, k;
    u16 Sx, Sy;

    Sx = x;
    Sy = y;
    Set_Posi(Sx, Sy);
    if(Style == BOLD) Width = 6;
    else              Width = 7;
    while(*Str != 0)
    {
        Code = *Str++;
        for(i = (Style ? 0 : 1); i < Width; ++i)
        {
            if(Style == BOLD)
            {
                if(!(Mode & SYMB)) k = Get_TAB_6x8(Code, i);
                else               k = SYMB6x8[(Code - '!') * 6 + i];
            }
            else
            {
                if(!(Mode & SYMB)) k = Get_Mini6x8(Code, i);
                else               k = SYMB6x8[(Code - '!') * 6 + i];
            }
            Set_Posi(Sx, Sy);
            Set_Pixel((Mode & INV) ? Fg : Bg);
            Set_Posi(Sx, Sy + 1);
            for(j = 0; j < 9; ++j)  // j<8时不空行，j<10空两行   即上下各空两行
            {
                if(Style == BOLD)
                {
                    if(k & 0x01) Set_Pixel((Mode & INV) ? Bg : Fg);
                    else         Set_Pixel((Mode & INV) ? Fg : Bg);
                    k >>= 1;
                }
                else
                {
                    if(k & 0x80) Set_Pixel((Mode & INV) ? Bg : Fg);
                    else         Set_Pixel((Mode & INV) ? Fg : Bg);
                    k <<= 1;
                }
            }
            Sx++;                  // 显示位置水平方向+1
        }
    }

}

/*******************************************************************************
 * FunctionName : Disp_Str7x9
 * Description  : 7X9像素字体
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
 * Param        : u16 Fg：  Foreground 前景色
 * Param        : u16 Bg：  Background 背景色
 * Param        : u8  Mode  Mode PRN/INV/SYMB/VOID 正常/反衬/图块/虚体  
 * Param        : u8  Style THIN/BOLD 细体/粗体
 * Param        : u8* Str 
*******************************************************************************/
void Disp_Str7x9(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Style, u8 *Str)
{
    u8  Code, Width;
    u16 i, j, k;
    u16 Sx, Sy;

    Sx = x;
    Sy = y;
    Set_Posi(Sx, Sy);
    if(Style == BOLD) Width = 6;
    else              Width = 7;
    while(*Str != 0)
    {
        Code = *Str++;
        for(i = 0; i < Width; ++i)
        {
            if(Style == BOLD)
            {
                if(!(Mode & SYMB)) k = Get_TAB_6x8(Code, i);
                else               k = SYMB6x8[(Code - '!') * 6 + i];
            }
            else
            {
                if(!(Mode & SYMB)) k = Get_Mini6x8(Code, i);
                else               k = SYMB6x8[(Code - '!') * 6 + i];
            }
            Set_Posi(Sx, Sy);
            Set_Pixel((Mode & INV) ? Fg : Bg);
            Set_Posi(Sx, Sy + 1);
            for(j = 0; j < 9; ++j)  // j<8时不空行，j<10空两行   即上下各空两行
            {
                if(Style == BOLD)
                {
                    if(k & 0x01) Set_Pixel((Mode & INV) ? Bg : Fg);
                    else         Set_Pixel((Mode & INV) ? Fg : Bg);
                    k >>= 1;
                }
                else
                {
                    if(k & 0x80) Set_Pixel((Mode & INV) ? Bg : Fg);
                    else         Set_Pixel((Mode & INV) ? Fg : Bg);
                    k <<= 1;
                }
            }
            Sx++;                  // 显示位置水平方向+1
        }
    }
}
/*******************************************************************************
 * FunctionName : Get_TAB_8x14
 * Description  : 取字符
 * Param        : u8  Code 
 * Param        : u16 Row 
 * Return       : u16   
*******************************************************************************/
u16 Get_TAB_8x14(u8 Code, u16 Row)
{
    if((Code < 0x21) || (Code > 0x7E))  return 0;
    else if(Row < 8) return Font8x14[((Code - 0X21) * 8) + Row];
    else             return 0;
}

/*******************************************************************************
 * FunctionName : Get_TAB_6x8
 * Description  : 取粗体字符
 * Param        : u8  Code 
 * Param        : u16 Row 
 * Return       : u16   
*******************************************************************************/
u16 Get_TAB_6x8(u8 Code, u16 Row)
{
    if((Code < 0x21) || (Code > 0x7E))  return 0;
    else if(Row < 6) return CHAR6x8[((Code - 0X21) * 6) + Row];
    else             return 0;
}

/*******************************************************************************
 * FunctionName : Get_Mini6x8
 * Description  : 取细体字符
 * Param        : u8  Code 
 * Param        : u16 Row 
 * Return       : u16   
*******************************************************************************/
u16 Get_Mini6x8(u8 Code, u16 Row)
{
    if((Code < 0x21) || (Code > 0x7E))  return 0;
    else if(Row < 6) return MiniChar6x8[((Code - 0X21) * 6) + Row];
    else              return 0;
}

/*******************************************************************************
 * FunctionName : Show_Run_ICO
 * Description  : 显示采集图标
 * Param        : u16 x 
 * Param        : u16 y 
 * Param        : u16 Fg 
 * Param        : u16 Bg 
 * Param        : u8  Mode 
*******************************************************************************/
u8 Show_Run_ICO(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Cnt)
{
    u8  j, k;
    u8  Str = 0;
    u16 Sx, Sy;

    Sx = x;
    Sy = y;
    Set_Posi(Sx, Sy);

    for(j = 0; j < 12; j++)
    {
        Str = Run_ICO[Cnt * 12 + j];

        Set_Posi(Sx, Sy);
        Set_Pixel((Mode & INV) ? Fg : Bg);
        Set_Posi(Sx, Sy + 1);
        for(k = 0; k < 8; k++)
        {
            if(Str & 0x01) Set_Pixel((Mode & INV) ? Bg : Fg);
            else            Set_Pixel((Mode & INV) ? Fg : Bg);
            Str >>= 1;
        }
        Sx++;                 //显示位置水平方向+1
    }

    if(Cnt < 7) Cnt++;
    else     Cnt = 0;

    Delay_mS(50);

    return Cnt;
}
/*******************************************************************************
 * FunctionName : RW_Row_Color
 * Description  : 读写列颜色
 * Param        : u16 x 坐标
 * Param        : u16 y 坐标
 * Param        : u16  Height 高度
 * Param        : u16* Buf 
 * Param        : u8   Mode；RD/WR 读/写 
*******************************************************************************/
void RW_Row_Color(u16 x, u16 y, u16 Height, u16 *Buf, u8 Mode)
{
    u16 i = 0;
    for(i = 0; i < Height; i++)
    {
        Set_Posi(x, y + i);
        if(Mode == WR)
        {
            Set_Pixel(*Buf++);
        }
        else
        {

            *Buf++ = Read_Pixel();
        }
    }
}

/*******************************************************************************
 * FunctionName : Set_Color
 * Description  : 设置前后背景色
 * Param        : u16 Bg 背景色
 * Param        : u16 Fg 前景色
*******************************************************************************/
void Set_Color(u16 Bg, u16 Fg)
{
    Background = Bg;
    Foreground = Fg;
}

/*******************************************************************************
 * FunctionName : Show_Str6x8
 * Description  : 
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u8  Mode  Mode PRN/INV/SYMB/VOID 正常/反衬/图块/虚体  
 * Param        : u8  Style THIN/BOLD 细体/粗体
 * Param        : u8* Str 
*******************************************************************************/
void Show_Str6x8(u16 x0, u16 y0, u8 Mode, u8 Style, u8 *Str)
{
    u8  Code, Width;
    u16 i, j, k;
    u16 Sx, Sy;

    Sx = x0;
    Sy = y0;
    Set_Posi(Sx, Sy);

    if(Style) Width = 6;
    else      Width = 7;
    while(*Str != 0)
    {
        Code = *Str++;

        for(i = (Style ? 0 : 1); i < Width; ++i)
        {
            if(Style == BOLD)
            {
                if(!(Mode & SYMB)) k = Get_TAB_6x8(Code, i);
                else                k = SYMB6x8[(Code - '!') * 6 + i];
            }
            else
            {
                if(!(Mode & SYMB)) k = Get_Mini6x8(Code, i);
                else                k = SYMB6x8[(Code - '!') * 6 + i];
            }
            Set_Posi(Sx, Sy);
            Set_Pixel((Mode & INV) ? Foreground : Background);
            Set_Posi(Sx, Sy + 1);
            for(j = 0; j < 9; ++j)  //j<8时不空行，j<10空两行   即上下各空两行
            {
                if(Style == BOLD)
                {
                    if(k & 0x01) Set_Pixel((Mode & INV) ? Background : Foreground);
                    else          Set_Pixel((Mode & INV) ? Foreground : Background);
                    k >>= 1;
                }
                else
                {
                    if(k & 0x80) Set_Pixel((Mode & INV) ? Background : Foreground);
                    else          Set_Pixel((Mode & INV) ? Foreground : Background);
                    k <<= 1;
                }
            }
            Sx++;                 //显示位置水平方向+1
        }
    }

}
