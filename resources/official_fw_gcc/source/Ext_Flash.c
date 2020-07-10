/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Ext_Flash.c
 * Description : 
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include <string.h>
#include "MyDefine.h"
#include "Disk.h"
#include "AppBios.h"
#include "Ext_Flash.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_flash.h"

#define WREN       0x06     /* Write enable instruction */
#define WRDI       0x04     /* Write disable instruction */
#define READ       0x03     /* Read from Memory instruction */
#define RDSR       0x05     /* Read Status Register instruction  */
#define PP         0x02     /* Write to Memory instruction */
#define SE         0x20     /* Sector 4K Erase instruction */
#define Dummy_Byte 0xA5
#define WIP_Flag   0x01     /* Write In Progress (WIP) flag */
#define TMAX       100000   /* 超时限制*/

u8   ExtFlashReadByte(void);
u8   ExtFlashSendByte(u8 byte);
void ExtFlashWrCtrl(u8 Cmd);
void ExtFlashWaitForWrEnd(void);
void ExtFlashSectorErase(u32 SecAddr);
void ExtFlashPageProg(u8 *pBuf, u32 WrAddr, u8 CMD);

/*******************************************************************************
 * FunctionName : FLASH_Prog
 * Description  : FLASH ROM Half Word Prog
 * Param        : u32 Addr 
 * Param        : u16 Data 
 * Return       : u8   
*******************************************************************************/
u8 FLASH_Prog(u32 Addr, u16 Data)
{
    if (FLASH_WaitForLastOperation(TMAX) != FLASH_TIMEOUT) 
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    return FLASH_ProgramHalfWord(Addr, Data);
}

/*******************************************************************************
 * FunctionName : FLASH_Erase
 * Description  : FLASH ROM Page Erase
 * Param        : u32 Addr 
*******************************************************************************/
void FLASH_Erase(u32 Addr)
{
    if (Addr % FLASH_PAGE == 0)                   // FLASH Page start
    {
        if (FLASH_WaitForLastOperation(TMAX) != FLASH_TIMEOUT) 
            FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
        FLASH_ErasePage(Addr);                   // FLASH Page erase
    }
}

/*******************************************************************************
 * FunctionName : ExtFlashSectorErase
 * Description  : Ext Flash Sector Erase
 * Param        : u32 SecAddr 
*******************************************************************************/
void ExtFlashSectorErase(u32 SecAddr)
{
//  NVIC_DisableIRQ(USB_LP_IRQn);
    ExtFlashWaitForWrEnd();
    ExtFlashWrCtrl(ENABLE);
    ExtFlash_CS_LOW();
    ExtFlashSendByte(SE);
    ExtFlashSendByte((SecAddr & 0xFF0000) >> 16);   //Send high address byte
    ExtFlashSendByte((SecAddr & 0xFF00) >> 8);      //Send medium address byte
    ExtFlashSendByte(SecAddr & 0xFF);               //Send low address byte
    ExtFlash_CS_HIGH();
    ExtFlashWrCtrl(DISABLE);
//  NVIC_EnableIRQ(USB_LP_IRQn);
}
/*******************************************************************************
 * FunctionName : ExtFlashSecWr
 * Description  : 写 Ext Flash 扇区
 * Param        : u8* pBuf 
 * Param        : u32 WrAddr 
*******************************************************************************/
void ExtFlashSecWr(u8 *pBuf, u32 WrAddr)
{
    u16 SecSize = SEC_SIZE;
    u32 i, Addr = WrAddr &  (~(SecSize - 1));
    ExtFlashSectorErase(Addr);
    for (i = 0; i < SecSize; i += PAGE_SIZE)
    {
        ExtFlashPageProg(&pBuf[i], Addr + i, PP);
    }
}

/*******************************************************************************
 * FunctionName : ExtFlashPageProg
 * Description  : 写 Ext Flash 页
 * Param        : u8* pBuf 
 * Param        : u32 WrAddr 
 * Param        : u8 WrCmd 
*******************************************************************************/
void ExtFlashPageProg(u8 *pBuf, u32 WrAddr, u8 WrCmd)
{
//  NVIC_DisableIRQ(USB_LP_IRQn);
    ExtFlashWaitForWrEnd();
    ExtFlashWrCtrl(ENABLE);
    ExtFlash_CS_LOW();
    ExtFlashSendByte(WrCmd);
    ExtFlashSendByte((WrAddr & 0xFF0000) >> 16);
    ExtFlashSendByte((WrAddr & 0xFF00) >> 8);
    ExtFlashSendByte(WrAddr & 0xFF);
    for (u16 i = 0; i < PAGE_SIZE; i++) ExtFlashSendByte(~pBuf[i]);
    ExtFlash_CS_HIGH();
    ExtFlashWrCtrl(DISABLE);
//  NVIC_EnableIRQ(USB_LP_IRQn);
}

/*******************************************************************************
 * FunctionName : ExtFlashDataRd
 * Description  : Ext Flash Read Data
 * Param        : u8* pBuf 
 * Param        : u32 RdAddr 
 * Param        : u16 Lenght 
*******************************************************************************/
void ExtFlashDataRd(u8 *pBuf, u32 RdAddr, u16 Lenght)
{
//  NVIC_DisableIRQ(USB_LP_IRQn);
    ExtFlashWaitForWrEnd();
    ExtFlash_CS_LOW();
    ExtFlashSendByte(READ);
    ExtFlashSendByte((RdAddr & 0xFF0000) >> 16);
    ExtFlashSendByte((RdAddr & 0xFF00) >> 8);
    ExtFlashSendByte(RdAddr & 0xFF);
    for (u16 i = 0; i < Lenght; i++) pBuf[i] = ~ExtFlashReadByte();
    ExtFlash_CS_HIGH();
//  NVIC_EnableIRQ(USB_LP_IRQn);
}

/*******************************************************************************
 * FunctionName : ExtFlashReadByte
 * Description  : Ext Flash Read Byte
 * Param        : void 
 * Return       : u8   
*******************************************************************************/
u8 ExtFlashReadByte(void)
{
    return ExtFlashSendByte(Dummy_Byte);
}

/*******************************************************************************
 * FunctionName : ExtFlashSendByte
 * Description  : SPI_FLASH_SendByte
 * Param        : u8 Byte 
 * Return       : u8   
*******************************************************************************/
u8 ExtFlashSendByte(u8 Byte)
{
    u32 Tout = 0;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
    {
        if (Tout++ > TMAX) return 255;
    }
    SPI_I2S_SendData(SPIx, Byte);
    Tout = 0;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if (Tout++ > TMAX) return 255;
    }
    return SPI_I2S_ReceiveData(SPIx);
}

/*******************************************************************************
 * FunctionName : ExtFlashWrCtrl
 * Description  : SPI_FLASH_WriteEnable
 * Param        : u8 Cmd 
*******************************************************************************/
void ExtFlashWrCtrl(u8 Cmd)
{
    ExtFlash_CS_LOW();
    if (Cmd == ENABLE) ExtFlashSendByte(WREN);
    else              ExtFlashSendByte(WRDI);
    ExtFlash_CS_HIGH();
}

/*******************************************************************************
 * FunctionName : ExtFlashWaitForWrEnd
 * Description  : SPI_FLASH_WaitForWriteEnd
*******************************************************************************/
void ExtFlashWaitForWrEnd(void)
{
    u32 Tout = 0;
    ExtFlash_CS_LOW();
    ExtFlashSendByte(RDSR);
    while ((WIP_Flag & ExtFlashReadByte()) == SET)
    {
        if (Tout++ > TMAX) return;
    }
    ExtFlash_CS_HIGH();
}

/*********************************  END OF FILE  ******************************/
