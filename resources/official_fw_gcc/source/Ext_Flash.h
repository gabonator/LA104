#ifndef __EXT_FLASH_H
#define __EXT_FLASH_H
#include "STM32F10x.h"

#define SPIx            SPI1
#define PAGE_SIZE       256 

u8   ExtFlashSendByte(u8 byte);
u8   FLASH_Prog(u32 Addr, u16 Data);
void FLASH_Erase(u32 Addr);
void ExtFlashSecWr(u8* pBuffer, u32 WriteAddr);
void ExtFlashDataRd(u8* pBuffer, u32 ReadAddr, u16 Lenght);
void ExtFlashSectorErase(u32 SecAddr);
void ExtFlashWrCtrl(u8 Cmd);

#endif

/*********************************  END OF FILE  ******************************/
