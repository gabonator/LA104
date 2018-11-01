#ifndef __DISK_H
#define __DISK_H

#include "STM32F10x.h"

#define SECTOR_SIZE      SEC_SIZE
#define SECTOR_CNT       SEC_AMNT

#define DISKSTR          "8MB Internal"
#define SEC_SIZE         4096
#define SEC_AMNT         2048
#define FLASH_PAGE       0x0800

extern u8 DiskBuf[];

void Disk_Init(void);
void Write_Memory(u32 W_Offset, u32 W_Length);
void Read_Memory (u32 R_Offset, u32 R_Length);
void USB_Connect(u8 Status);

#endif
/*********************************  END OF FILE  ******************************/

