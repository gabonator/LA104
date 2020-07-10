#ifndef __Function_H__
#define __Function_H__

#define SIGN         0
#define UNSIGN       1
#define STD          2


#define ZERO         0
#define POSITIVE     1
#define NEGATIVE     2

void Rev32(u32 *pBuf);
void Rev16(u16 *pBuf);
u32  Power(u8 x, u8 y);
u32  Exp(u8 x);
u8*  SeekStr(u8 *ptr, u8 Idx);

void Delayms(u16 mS);
void Int2Str(u8 *p, s32 n, uc8 *pUnit, u8 e, u8 Mode);
void Value2Str(u8 *p, s32 n, uc8 *pUnit, s8 e, u8 Mode);
u8   Str2Byte(u8 x, u8 y);
void u16ToDec4Str(u8 *p, u16 n);
void u16ToDec5Str(u8 *p, u16 n);
void u32ToDecStr(u8 *p, u32 n);
void s16ToDec5Str(u8 *p, s16 n);
void u8ToDec3Str(u8 *p, u8 n);
void u8ToDec2Str(u8 *p, u8 n);
void u8ToDecStr(u8 *p, u8 n);
void Char2Hex(u8 *p, u8 n);
void Shor2Hex(u8 *p, u16 n);
void Char2Str(u8 *p, u8 n);
void Char2HexFullStr(u8 *p, u8 n);

u16  Sqrt32(u32 n);
void s8ToPercen(u8 *p, s8 n);
void ValuedivTen2Str(u8 *p, u16 n);
void u32ToStr_Unit(u8 *p, u32 n, uc8 *pUnit, s8 e);
void u64ToStr_Unit(u8 *p, u64 n, uc8 *pUnit, s8 e, u8 Mode);

#endif
/********************************* END OF FILE ********************************/
