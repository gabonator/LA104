/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  File.c Ver: 2.0                                                Author : bure
*******************************************************************************/
#include "File.h"
#include "Sys.h"

#define PARAM_VER    0x10   // 

u8  SaveByte(u8 *p, u8 Char);
u8  SaveRecd(u8 *p, u8 Recd);

uc8 BmpHead[56]  = { 0x42, 0x4D, 0xF8, 0xB, 0x00, 0x00, 0x00, 0x00, 
                     0x00, 0x00, 0x76, 0x0, 0x00, 0x00, 0x28, 0x00,
                     0x00, 0x00, 0x90, 0x1, 0x00, 0x00, 0xF0, 0x00,
                     0x00, 0x00, 0x01, 0x0, 0x04, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x82, 0xB, 0x00, 0x00, 0x12, 0x0B,
                     0x00, 0x00, 0x12, 0xB, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x00};
uc16 BmpColor[]  = { WHT,  CYAN, CYAN_, YEL,  YEL_, PRPL, PRPL_, GRN,   
                     GRN_, GRAY, ORNG,  BLUE, RED,  BLK,  BLK,   BLK};    
uc8 F_NAME[][12] = {"TRACK   DAT", "TRACK   DAT", "SGNAL   BUF", "SGNAL   CSV"};


/*******************************************************************************
 
*******************************************************************************/
u8 Color_Num(u16 Color)
{
  if(Color == WHT)                 return 0;
  else if((Color & CYAN )== CYAN ) return 1;
  else if((Color & YEL  )== YEL  ) return 3;
  else if((Color & PRPL )== PRPL ) return 5;
  else if((Color & GRN  )== GRN  ) return 7;
  else if((Color & CYAN_)== CYAN_) return 2;
  else if((Color & YEL_ )== YEL_ ) return 4;
  else if((Color & PRPL_)== PRPL_) return 6;
  else if((Color & GRN_ )== GRN_ ) return 8;
  else if((Color & GRAY )== GRAY ) return 9;
  else if((Color & ORNG )== ORNG ) return 10;
  else if((Color & BLUE )== BLUE ) return 11;
  else if((Color & RED  )== RED  ) return 12;
  else                             return 13;
}
/*******************************************************************************
  Save Parameter: 
*******************************************************************************/
u8 SaveParam(void)
{
   return WR_ERR;
}
/*******************************************************************************
  Load Parameter: 
*******************************************************************************/
u8 LoadParam(void)
{ 
  return RD_ERR;
}
/*******************************************************************************
  
*******************************************************************************/
u16 SeekMaxFileNum(u8 *FileName)
{
  return 0;
}
/*******************************************************************************
  BMP 
*******************************************************************************/
u8 Snapshot(void)
{
  return DISK_ERR;
}
/*******************************************************************************
  
*******************************************************************************/
u8 SaveTrack(u8 FileNum)
{
  return DISK_ERR;
}
/*******************************************************************************
  
*******************************************************************************/
u8 LoadTrack(u8 FileNum)
{
  return RD_ERR;
}
/*******************************************************************************
  
*******************************************************************************/
u8 SaveBuf(u8 FileNum)
{
  return DISK_ERR;
}
/*******************************************************************************
      
*******************************************************************************/
u8 SaveByte(u8 *p, u8 Char)
{
  return OK;
}
/*******************************************************************************
  
*******************************************************************************/
u8 SaveRecd(u8 *p, u8 Recd)
{
  u8 Str[4];
  u8To3DecStr(Str, Recd);
  if(SaveByte(p, Str[0]) != OK) return WR_ERR;
  if(SaveByte(p, Str[1]) != OK) return WR_ERR;
  if(SaveByte(p, Str[2]) != OK) return WR_ERR;
  if(SaveByte(p, ',')    != OK) return WR_ERR;
  return OK;
}
/*******************************************************************************
  
*******************************************************************************/
u8 SaveCsv(u8 FileNum)
{ 
  return DISK_ERR;
}
/******************************** END OF FILE *********************************/

