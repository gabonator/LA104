/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  Func.h Ver: 2.0                                        Author : bure & Kewei
*******************************************************************************/
#ifndef __Funcs_H__
#define __Funcs_H__

  #include "Platform.h"

  #define SIGN         0
  #define UNSIGN       1
  #define STD          2

  typedef struct{
    u8 decPos;
    u8 len;
    u8 str[9];
  } I32STR_RES;

  void Mem32Cpy(u32* p, u32* q, u32 n);
  void Mem32Set(u32* p, u32 Word, u32 n);

  void Rev32(u32* pBuf);
  void Rev16(u16* pBuf);
  u32  Power(u8 x, u8 y);
  u32  Exp(u8 x);
  u8*  SeekStr(u8* ptr, u8 Idx);
  u8   Char2Nib(u8 x);

  void Int2Str(u8 *p, s32 n, uc8 *pUnit, u8 e, u8 Mode);
//void Int32String_sign(I32STR_RES *r, s32 n);
//void Int32String(I32STR_RES *r, u32 n, u32 e);
  void Value2Str(u8 *p, s32 n, uc8 *pUnit, s8 e, u8 Mode);
  u8   Str2Byte(u8 x,u8 y);
  void u16To4DecStr(u8 *p, u16 n);
  void u16To5DecStr(u8 *p, u16 n);
  void s16To5DecStr(u8 *p, s16 n);
  void u8To3DecStr (u8 *p, u8  n);
  void u8To2DecStr (u8 *p, u8  n);
  void u8To2HexStr (u8 *p, u8  n);
  void u16To4HexStr(u8 *p, u16 n);
  void u32To8HexStr(u8 *p, u32 n);
  u16  Sqrt32(u32 n);
  void s8ToPercen(u8 *p, s8 n);
  void s8To2DecStr(u8 *p, s8 n);
  void dectostr(u8 *nStr,u8* decpos, u32 Num);
  void Int32String(I32STR_RES *r, u32 n, u32 e);
  void Int32String_sign(I32STR_RES *r, s32 n);

#endif
/********************************* END OF FILE ********************************/
