#ifndef __FAT12_H
#define __FAT12_H

#include <stdint.h>
typedef uint64_t u64;

#include "STM32F10x.h"

//-------FLASH-----W25Q64BV--------------------------------------------------
#define FILE_BASE_8M    0x7000     /**/
#define ROOT_BASE_8M    0x3000     /**/ 
#define FAT_LEN_8M      0x1000
#define FAT1_BASE_8M    0x1000     /* FAT1*/
#define FAT2_BASE_8M    0x2000     /* FAT2*/
#define SEC_LEN_8M      0x1000     /*      */
#define FAT1_SEC_8M     0x1        /* FAT1   */
#define FAT2_SEC_8M     0x1        /* FAT2     */
#define FAT_END_8M      0x7FF      /*            */
                                                 
#define OK           0             /*            */
#define SEC_ERR      1             /*              */
#define FAT_ERR      2             /* FAT            */
#define OVER         3             /*                */
#define NEW          4             /*                */
#define SUM_ERR      6             /* У              
                                                     */
#define VER_ERR      1             /*                */
#define NO_FILE      2             /*                */
#define FILE_RW_ERR  3             /*                */
#define DISK_RW_ERR  4             /*                */

#define OW           0             /*                */
#define RW           1             /*                */

extern u8 Clash;

u8   ReadFileSec(u8 *Buffer, u16 *Cluster);
u8   ReadDiskData(u8 *pBuffer, u32 ReadAddr, u16 Lenght);
u8   NextCluster(u16 *Cluster);
u8   ProgFileSec(u8 *Buffer, u16 *Cluster);
u8   ProgDiskPage(u8 *Buffer, u32 ProgAddr);
u8   SeekBlank(u8 *Buffer, u16 *Cluster);
u8   SetCluster(u8 *Buffer, u16 *Cluster);
u8   OpenFileRd(u8 *Buffer, u8 *FileName, u16 *Cluster, u32 *pDirAddr);
u8   OpenFileWr(u8 *Buffer, u8 *FileName, u16 *Cluster, u32 *pDirAddr);
u8   CloseFile(u8 *Buffer, u32 Lenght, u16 *Cluster, u32 *pDirAddr);
void ExtFlash_PageWrite(u8 *pBuffer, u32 WriteAddr, u8 Mode);
int  Init_Fat_Value(void);

#endif
/********************************* END OF FILE ********************************/
