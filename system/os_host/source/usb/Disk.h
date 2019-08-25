#ifndef __DISK_H
#define __DISK_H

#include "STM32F10x.h"
#include "UsbTypes.h"

extern u8 DiskBuf[];

void Disk_Init(void);
void Write_Memory(u32 W_Offset, u32 W_Length);
void Read_Memory (u32 R_Offset, u32 R_Length);
void USB_Connect(u8 Status);

#endif
/*********************************  END OF FILE  ******************************/

