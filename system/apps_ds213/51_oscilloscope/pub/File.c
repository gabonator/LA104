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

//u8  FileBuf[4096];//, TrckBuf[1440+10];

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
#pragma message("Neni implementovane")
#if 0
  u8  Sum = 0, FileName[12]; 
  u16 *p =(u16*)FileBuf;
  u16 Tmp[2], pCluster[3];
  u32 r, pDirAddr[1];

  u32To8HexStr(FileName, GetDev_SN());
  FileName[8] = 'P', FileName[9] = 'R', FileName[10] = 'M'; 
  switch(OpenFileRd(FileBuf, FileName, pCluster, pDirAddr)){
    case OK:
      Tmp[0] = *pCluster;                                       // PRM 
      FileName[8] = 'B', FileName[9] = 'A', FileName[10] = 'K'; // ת
      r = OpenFileWr(FileBuf, FileName, pCluster, pDirAddr);
      if(r != OK) return DISK_ERR;                      
      if(ReadFileSec(FileBuf, Tmp     )!= OK) return RD_ERR;                                 
      if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;    // 
      r = CloseFile(FileBuf, 512, pCluster, pDirAddr);
      if(r != OK) return WR_ERR;                              
    case NEW:                                                   // PRM 
      FileName[8] = 'P', FileName[9] = 'R', FileName[10] = 'M';
      r = OpenFileWr(FileBuf, FileName, pCluster, pDirAddr);
      if(r != OK) return DISK_ERR;                              // 
      Mem32Set((u32*)FileBuf, 0, 128);
      *p++ =(Item << 8)+ PARAM_VER;                             // 
      for(u32 i = 0; i < ITEM_END; i++){ 
        *p++ = Menu[i].Val;
        *p++ = (Menu[i].Src << 8)+(Menu[i].Flg | UPDT);         // 
      }
      for(u32 i = 0; i < 10; i++){                              // 
        *p++ = Diff[CH_A][i], *p++ = Diff[CH_B][i];
      }
      for(u32 i = 0; i < 10; i++){                              // 
        *p++ = Gain[CH_A][i], *p++ = Gain[CH_B][i];
      }
      *p++ = Slope[CH_A], *p++ = Slope[CH_B];                   // 
      for(u32 i = 0; i < 4; i++) *p++ = (Vt[i] << 8)+Yn[i];     // 
      for(u32 i = 0; i < (SPDT-3); i++) *p++ = Pop[SVOL+i].Val; // 
      for(u32 i = 0; i < 511; i++) Sum += FileBuf[i];           // 
      FileBuf[511] = (~Sum)+ 1;
      if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;    // д
      r = CloseFile(FileBuf, 512, pCluster, pDirAddr);
      return (r != OK) ? WR_ERR : OK;
    default: return WR_ERR;
  }
#endif
  return WR_ERR;
}
/*******************************************************************************
  Load Parameter: 
*******************************************************************************/
u8 LoadParam(void)
{ 
#pragma message("Neni implementovane")
#if 0
  u8  Sum = 0, FileName[12]; 
  u16 *p =(u16*)FileBuf;
  u16 pCluster[3];
  u32 pDirAddr[1];
  
  u32To8HexStr(FileName, GetDev_SN());
  FileName[8] = 'P', FileName[9] = 'R', FileName[10] = 'M', FileName[11] = 0;
  if(OpenFileRd(FileBuf, FileName, pCluster, pDirAddr) != OK) return RD_ERR;
  if(ReadFileSec(FileBuf, pCluster)!= OK) return RD_ERR;
  if(PARAM_VER !=(*p & 0xFF)) return VER_ERR;               // 
  for(u32 i = 0; i < 512; i++) Sum += FileBuf[i];
  if(Sum != 0) return SUM_ERR;                              // У
  Item = *p++ >> 8;                                         // 
  for(u32 i = 0; i < ITEM_END; i++){                        // 
    Menu[i].Val = *p++, Menu[i].Src = *p >> 8;
    Menu[i].Flg = 0xFF & *p++;
  }
  for(u32 i = 0; i < 10; i++){                              // 
    Diff[CH_A][i] = *p++, Diff[CH_B][i] = *p++;
  }
  for(u32 i = 0; i < 10; i++){                              // 
    Gain[CH_A][i] = *p++, Gain[CH_B][i] = *p++;
  }
  Slope[CH_A] = *p++, Slope[CH_B] = *p++;                   // 
  
  for(u32 i = 0; i < 4; i++){                               // 
    Vt[i] = *p >> 8, Yn[i] = 0xFF & *p++;
  }
  for(u32 i = 0; i < (SPDT-3); i++) Pop[SVOL+i].Val = *p++; // 
  for(u32 i = 0; i < 4; i++) Pop[i].Val = SeekMaxFileNum((u8*)&F_NAME[i][0]);
#endif
  return RD_ERR;
 }
/*******************************************************************************
  
*******************************************************************************/
u16 SeekMaxFileNum(u8 *FileName)
{
#pragma message("Neni implementovane")
#if 0
  s32 Max = -1;  
  for(u32 pDir = FAT_Param.ROOT_BASE; pDir < FAT_Param.FILE_BASE;){
    ReadDiskData(FileBuf, pDir, FAT_Param.SEC_LEN);
    for(u32 n = 0; n < FAT_Param.SEC_LEN; n += 32){
      s16 Val = -1;
      u16 Same = 1;
      for(u32 i = 0; i < 11; i++){
        if((i < 5 || i > 7) && FileBuf[n+i] != FileName[i]) Same = 0;
      }
      u8 *p = &FileBuf[n+5];
      if(Same) Val = (*p-'0')*100+(*(p+1)-'0')*10+*(p+2)-'0';
      if(Max < Val) Max = Val;
      pDir += 32;
    }
  }  
  if(Max >= 999) Max = -1;     // 
  return Max+1;
#endif
  return 0;
}
/*******************************************************************************
  BMP 
*******************************************************************************/
u8 Snapshot(void)
{
#pragma message("Neni implementovane")
#if 0

  u8  NumStr[6], FileName[12] = "IMAGE   BMP";
  u16 n = SeekMaxFileNum(FileName);
  u16 pCluster[3];
  u32 pDirAddr[1];
  
  u16To5DecStr(NumStr, n);
  FileName[5] = NumStr[2], FileName[6] = NumStr[3], FileName[7] = NumStr[4];
  if(OpenFileWr(FileBuf, FileName, pCluster, pDirAddr)!=OK) return DISK_ERR;
  memcpy(FileBuf, BmpHead, 54);
  u16 i = 0x0036;                               // 
  for(u32 j = 0; j < 16; j++){
    FileBuf[j*4 +i+0]=(BmpColor[j]& 0xF800)>>8; // Bule
    FileBuf[j*4 +i+1]=(BmpColor[j]& 0x07E0)>>3; // Green
    FileBuf[j*4 +i+2]=(BmpColor[j]& 0x001F)<<3; // Red
    FileBuf[j*4 +i+3]= 0;                       // Alpha
  }
  u16 k = 0;
  i = 0x0076;                                   // ͼ
  for(u32 y = 0; y < 240; y++){
    for(u32 x = 0; x < 400; x += 2){
      LCD_RdBlock(x, y, x+1, y);
      FileBuf[i++] =(Color_Num(RdPxl()) << 4) | Color_Num(RdPxl());
      if(i >= SectorSize){
        if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR; // д
        i = 0; 
        if(y > 12){
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          PrintStr(TXT2C, CHAR, " Saveing     ");
        }
        if(y > 12) PrintClk(372, 0, (k++ >> 1)& 3);            // 
      }
    }
  }
  if(i != 0) if(ProgFileSec(FileBuf, pCluster) != OK) return WR_ERR; // д
  if(CloseFile(FileBuf, 0xBC00, pCluster, pDirAddr)!= OK) return WR_ERR;
#endif
  return 0; 
}
/*******************************************************************************
  
*******************************************************************************/
u8 SaveTrack(u8 FileNum)
{
#pragma message("Neni implementovane")
#if 0
  u8  NumStr[6], FileName[12];
  u16 pCluster[3];
  u32 pDirAddr[1];
  u16To5DecStr(NumStr, FileNum);
  memcpy(FileName, &F_NAME[0][0], 12);
  FileName[5] = NumStr[2], FileName[6] = NumStr[3], FileName[7] = NumStr[4];
  if(OpenFileWr(FileBuf, FileName, pCluster, pDirAddr)!=OK) return DISK_ERR;
  Mem32Set((u32*)FileBuf, 0, 4096/4);
  for(u32 j = 0; j < 4; j++) Track[4*359+j] = Yn[j];     // 
  Mem32Cpy((u32*)FileBuf, (u32*)Track, 360);             // 
  if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR; // д
  if(CloseFile(FileBuf, 512*4, pCluster, pDirAddr)!= OK) return WR_ERR;
  if(Pop[SWAV].Val < Pop[SWAV].Max) Pop[SWAV].Val++;
#endif
  return OK;
}
/*******************************************************************************
  
*******************************************************************************/
u8 LoadTrack(u8 FileNum)
{
#pragma message("Neni implementovane")
#if 0

  u8  NumStr[6]; 
  u8  FileName[12];
  u16 pCluster[3];
  u32 pDirAddr[1];
  u16To5DecStr(NumStr, FileNum);
  memcpy(FileName, &F_NAME[1][0], 12);
  FileName[5] = NumStr[2], FileName[6] = NumStr[3], FileName[7] = NumStr[4];
  u32 r = OpenFileRd(FileBuf, FileName, pCluster, pDirAddr);
  if(r != OK) return r;
  if(ReadFileSec(FileBuf, pCluster)!= OK) return RD_ERR;
  Mem32Cpy((u32*)Recod, (u32*)FileBuf, 360);
#endif
  return 0;
}
/*******************************************************************************
  
*******************************************************************************/
u8 SaveBuf(u8 FileNum)
{
#pragma message("Neni implementovane")
#if 0

  u8  NumStr[6], FileName[12];
  u16 pCluster[3];
  u32 pDirAddr[1];
  
  u16To5DecStr(NumStr, FileNum);
  memcpy(FileName, &F_NAME[2][0], 12);
  FileName[5] = NumStr[2], FileName[6] = NumStr[3], FileName[7] = NumStr[4];
  if(OpenFileWr(FileBuf, FileName, pCluster, pDirAddr)!=OK) return DISK_ERR;
  u32 St = 0;
  while(~St & FULL){
    St = FPGA_CtrlRW(CH_A, SMPL_ST) | FPGA_CtrlRW(CH_B, SMPL_ST);
  }
  FPGA_DataWr(A_C_CH, SMPL_RPTR, 0);
  FPGA_DataWr(B_D_CH, SMPL_RPTR, 0); // 
  for(u32 i = 0; i < 4; i++){
    u8* p = FileBuf;
    for(u32 n = 0; n <1024; n++){
      u32 SmplAC = FPGA_SmplRd(A_C_CH), SmplBD = FPGA_SmplRd(B_D_CH);
      *p++ = SmplAC >> 8, *p++ = SmplBD >> 8;
      *p++ = SmplAC &  1, *p++ = SmplBD &  1;
    }
    if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR; // д
  }
  Mem32Set((u32*)FileBuf, 0, 1024);
  u16* p =(u16*)FileBuf;
  for(u32 i = 0; i < ITEM_END; i++){
    if(i == RNA || i == RNB || i == TIM || i == T_0 || i == XNP){
      *p++ =  Menu[i].Val;
      *p++ = (Menu[i].Src << 8)+(Menu[i].Flg | UPDT);      // 
    }
  }
  for(u32 i = 0; i < 4; i++) *p++ = (Vt[i] << 8)+Yn[i];    // 
  if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;   // д
  if(CloseFile(FileBuf, 0x4200, pCluster, pDirAddr)!= OK) return WR_ERR;
  if(Pop[SBUF].Val < Pop[SBUF].Max) Pop[SBUF].Val++;
#endif
  return 0;
}
/*******************************************************************************
      
*******************************************************************************/
u8 SaveByte(u8 *p, u8 Char)
{
#pragma message("Neni implementovane")
#if 0

  u16 pCluster[3];
  *p = Char;
  if(++p == FileBuf+SectorSize){
    if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR; // д
    p = FileBuf;
  }
#endif
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
#pragma message("Neni implementovane")
#if 0

  u8  NumStr[6], Num[4], Ch[4], FileName[12]; 
  u32 k = 0, n = 0;
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  u16To5DecStr(NumStr, FileNum);
  memcpy(FileName, &F_NAME[3][0], 12);
  FileName[5] = NumStr[2], FileName[6] = NumStr[3], FileName[7] = NumStr[4];
  if(OpenFileWr(FileBuf, FileName, pCluster, pDirAddr)!=OK) return DISK_ERR;

  memcpy(FileBuf,     "CH_A", 4);     k+= 4;                            
  memcpy(&FileBuf[k], (u8*)Menu[CPA].STR+ 8*Menu[CPA].Val, 4); k += 4;
  memcpy(&FileBuf[k], (u8*)Menu[RNA].STR+ 8*Menu[RNA].Val, 4); k += 4;                    
  memcpy(&FileBuf[k], ",CH_B", 5);    k += 5;                       
  memcpy(&FileBuf[k], (u8*)Menu[CPB].STR+ 8*Menu[CPB].Val, 4); k += 4;                   
  memcpy(&FileBuf[k], (u8*)Menu[RNB].STR+ 8*Menu[RNB].Val, 4); k += 4;                    
  memcpy(&FileBuf[k], ",CH_C", 5);    k += 5;                          
  memcpy(&FileBuf[k], ",CH_D", 5);    k += 5;                           
  memcpy(&FileBuf[k], ",T_BASE ", 8); k += 8;                        
  memcpy(&FileBuf[k], (u8*)Menu[TIM].STR + 8 * Menu[TIM].Val, 5);  k+= 5;                   
  FileBuf[k] = 0x0D;  k++;                                       
  FileBuf[k] = 0x0A;  k++;

  u32 St = 0, Len = 4096;
  while(~St & FULL){
    St = FPGA_CtrlRW(CH_A, SMPL_ST) | FPGA_CtrlRW(CH_B, SMPL_ST);
  }
  FPGA_DataWr(A_C_CH, SMPL_RPTR, 0);
  FPGA_DataWr(B_D_CH, SMPL_RPTR, 0); // 
  for(u32 i = 0; i < 4096; i++){
    u32 SmplAC = FPGA_SmplRd(A_C_CH), SmplBD = FPGA_SmplRd(B_D_CH);
    Ch[0] = SmplAC >> 8, Ch[1] = SmplBD >> 8;
    Ch[2] = SmplAC &  1, Ch[3] = SmplBD &  1;
    for(u32 j = 0; j < 4; j++){
      u8To3DecStr(Num, Ch[j]);
      for(u32 Count = 0; Count < 3; Count++){
        if(Num[Count] == 0) break;
        FileBuf[k++] = Num[Count];
        if(k >= Len){
          if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR; // д
          PrintClk(354, 0, (n++ >> 1)& 3);
          k = 0;
        }
      }
      FileBuf[k++] = 0x2c;
      if(k >= Len){
        if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;   // д
        PrintClk(354, 0, (n++ >> 1)& 3);
        k = 0;
      }
    }
    FileBuf[k++] = 0x0d;
    if(k >= Len){
      if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;     // д
      PrintClk(354, 0, (n++ >> 1)& 3);
      k = 0;
    }
    FileBuf[k++] = 0x0a;
    if(k >= Len){
      if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;     // д
      PrintClk(354, 0, (n++ >> 1)& 3);
      k = 0;
    }
  }
  if(k != 0){
    FileBuf[k++]=0x0d;
    FileBuf[k++]=0x0a;
    memset(&FileBuf[k],0,(Len-k));
    k=0;
    if(ProgFileSec(FileBuf, pCluster)!= OK) return WR_ERR;       // д
    PrintClk(354, 0, (n++ >> 1)& 3);  
  }
  if(CloseFile(FileBuf, n*Len, pCluster, pDirAddr)!= OK) return WR_ERR;
  if(Pop[SCSV].Val < Pop[SCSV].Max) Pop[SCSV].Val++;
#endif
  return OK;
}
/******************************** END OF FILE *********************************/

