/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : file.h  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#ifndef __FILE_H
#define __FILE_H

#include "system.h"

//#define FLASH_WAIT_TIMEOUT      100000 
//#define PAGE_0                  0 
//#define PAGE_1                  1 
//#define PAGE_2                  2 
//#define PAGE_3                  3 

#define OK           0            // operation is completed
//#define SEC_ERR      1            // sectors read and write errors
//#define FAT_ERR      2            // FAT table to read and write errors
#define OVER         3            // operations overflow
#define NEW          4            // new directory entry
#define EMPT         4            // file does not exist
#define VER_ERR      5            // wrong version
#define SUM_ERR      6            // checksum error
#define RD_ERR       8            // sectors read error
#define WR_ERR       9            // sector write error
#define DISK_ERR     10           // disk error

#define FLASH_8M    1
#define FLASH_2M    0
#define FLASH_2M_SECTOR_SIZE 512
#define FLASH_8M_SECTOR_SIZE 4096
#define LARGEST_SECTOR_SIZE 4096
#define BUFFER_SIZE 16384
extern u8 flash_mode;
extern u16 SectorSize;
extern u8  FileBuff[1600];
extern u8  SaveShortBuffXpos;
extern u8  Versions;
extern u16 TempPar[74];
extern u16 ArbtSampleNumber;
extern u32 UartFileSize;
//extern u8  SecBuff[LARGEST_SECTOR_SIZE];
extern u8  SecBuff[0x1800];
extern char Label[15][13];
extern char Ext[4];
extern u16  DirRange;
extern char SelectedFileName[9];
extern char ConfigFileName[9];
extern char LastAccessedConfig[9];
extern u8  Edited;
extern u8  BufferRestore;
extern u16 CSVposition;

u8 Save_Bmp(u8 FileNum);
u8 Save_Dat(u8 FileNum);
u8 Load_Dat(u8 FileNum);
u8 Save_Buf(u8 FileNum);
u8 Load_Buf(u8 FileNum);
u8 Load_Bmp(u8 FileNum);
u8 Save_Csv(u8 FileNum);
u8 Make_Filename(u8 FileNum,char* FileName);
void reset_parameter(void);
u8 Load_Param(u8 FileNum);
u8 Save_Param(u8 FileNum);
u8 Load_Arbt(u8 FileNum);
u8 Load_Uart(u8 FileNum);
u8 Save_Img(void);
u8 ReadDir(char *ext);
void FileMessage(u8 i);
void UpdateFileMenu(void);
u8 DeleteFile(u8* pBuffer,char* FileName);
void save_parameter(void);

#endif
/********************************* END OF FILE ********************************/
