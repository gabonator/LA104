#ifndef __GUI_H
#define __GUI_H

#include "MyDefine.h"

//======================== 
#define PRN       0x00
#define INV       0x10   
#define SYMB      0x20   
#define VOID      0x40   

#define THIN      0x00   
#define BOLD      0x01   

#define LCD_ROW   320    
#define LCD_COL   240    

#define RD        0
#define WR        1

//====================== 
#define CYN        0xFFE0  /* 0*/
#define PUR        0xF81F  /* 1*/
#define YEL        0x07FF  /* 2*/
#define GRN        0x07E0  /* 3*/
#define CYN_       0xBDE0  /* 4*/
#define PUR_       0xB817  /* 5*/
#define YEL_       0x05F7  /* 6*/
#define GRN_       0x05E0  /* 7*/
#define ORN        0x051F  /* 8*/
#define BLK        0x0000  /* 9*/
#define WHT        0xFFFF  /* 10*/
#define BLU        0xFC10  /* 11*/
#define RED        0x001F  /* 12*/
#define GRY        0x7BEF  /* 13*/
#define LGN        0x27E4  /* 14*/
#define DAR        0x39E7  /* 15*/
#define GRAY       0x7BEF
#define RED_       0x631F
#define SW_IN_COL  DAR
#define SW_COL     CYN_

u16  Read_Pixel(void);
u16  Read_Pixel_Color(u16 x, u16 y);
u16  Get_TAB_6x8(u8 Code, u16 Row);
u16  Get_Mini6x8(u8 Code, u16 Row);
u16  Get_TAB_8x14(u8 Code, u16 Row);
void Set_Posi(u16 x, u16 y);
void Set_Pixel(u16 Color);
void Add_Pixel_Color(u16 x, u16 y, u16 Color);
void Disp_Str8x14(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, const char *Str);
//void Disp_Str8x14(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, const u8 *Str);
//void Disp_Str8x14(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, const u8 Str[]);
void Disp_Char8x14(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Code);
void Disp_Str6x8(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Style, const char *Str);
void Disp_Str7x9(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Style, const char *Str);
void Set_Block(u16 x1, u16 y1, u16 x2, u16 y2);
void Clr_Scrn(u16 Color);
u8   Show_Run_ICO(u16 x, u16 y, u16 Fg, u16 Bg, u8 Mode, u8 Cnt);
void RW_Row_Color(u16 x, u16 y, u16 Height, u16 *Buf, u8 Mode);
void Show_Str6x8(u16 x0, u16 y0, u8 Mode, u8 Style, const char *Str);
void Set_Color(u16 Bg, u16 Fg);

#endif