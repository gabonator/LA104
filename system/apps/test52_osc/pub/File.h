/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  File.h Ver: 2.0                                                Author : bure
*******************************************************************************/
#ifndef __FILE_H
#define __FILE_H

  #include "Platform.h"

  #define FLASH_WAIT_TIMEOUT      100000 
  #define PAGE_0                  0 
  #define PAGE_1                  1 
  #define PAGE_2                  2 
  #define PAGE_3                  3 

  #define OK                      0 // 
  #define SEC_ERR                 1 // 
  #define FAT_ERR                 2 // FAT
  #define OVER                    3 // 
  #define NEW                     4 // 
  #define EMPT                    4 // 
  #define VER_ERR                 5 // 
  #define SUM_ERR                 6 // У
  #define RD_ERR                  8 // 
  #define WR_ERR                  9 // 
  #define DISK_ERR                10 // 

  extern uc8 F_NAME[][12];
  extern u8  TrckBuf[];
  u8  Snapshot(void);
  u8  SaveParam(void);
  u8  LoadParam(void);
  u8  SaveBuf(u8 FileNum);
  u8  SaveCsv(u8 FileNum);
  u8  SaveTrack(u8 FileNum);
  u8  LoadTrack(u8 FileNum);
  u16 SeekMaxFileNum(u8 *pName); 

#endif
/********************************* END OF FILE ********************************/
