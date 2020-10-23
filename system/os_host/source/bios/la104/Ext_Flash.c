/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Ext_Flash.c
 * Description : 
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include <stdbool.h>
#include "Ext_Flash.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_flash.h"
#include "../../framework/Types.h"

#define WREN       0x06     /* Write enable instruction */
#define WRDI       0x04     /* Write disable instruction */
#define READ       0x03     /* Read from Memory instruction */
#define RDSR       0x05     /* Read Status Register instruction  */
#define PP         0x02     /* Write to Memory instruction */
#define SE         0x20     /* Sector 4K Erase instruction */
#define Dummy_Byte 0xA5
#define WIP_Flag   0x01     /* Write In Progress (WIP) flag */
#define TMAX       100000   /* */

extern void ExtFlash_CS_LOW(void);
extern void ExtFlash_CS_HIGH(void);
volatile bool eepromAccessMutex = false;

uint32_t gFlashReadRange[2] = {-1, 0};
uint32_t gFlashWriteRange[2] = {-1, 0};
uint32_t gFlashAlertRange[2] = {-1, 0};

u8   ExtFlashReadByte(void);
u8   ExtFlashSendByte(u8 byte);
void ExtFlashWrCtrl(u8 Cmd);
void ExtFlashWaitForWrEnd(void);
void ExtFlashSectorErase(u32 SecAddr);
bool ExtFlashPageProg(u8 *pBuf, u32 WrAddr, u8 CMD);

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
 * Description  : д Ext Flash 
 * Param        : u8* pBuf 
 * Param        : u32 WrAddr 
*******************************************************************************/
bool ExtFlashSecWr(u8 *pBuf, u32 WrAddr)
{
    gFlashWriteRange[0] = min(gFlashWriteRange[0], WrAddr);
    gFlashWriteRange[1] = max(gFlashWriteRange[1], WrAddr);
    if (gFlashAlertRange[0] >= gFlashAlertRange[1])
    {
      if (gFlashAlertRange[0] >= WrAddr && WrAddr >= gFlashAlertRange[1])
      {
        gFlashAlertRange[0] = -1;
        gFlashAlertRange[1] = 0;
      }
    }
    u16 SecSize = SEC_SIZE;
    u32 i, Addr = WrAddr &  (~(SecSize - 1));
    ExtFlashSectorErase(Addr);
    for (i = 0; i < SecSize; i += PAGE_SIZE)
    {
        if (!ExtFlashPageProg(&pBuf[i], Addr + i, PP))
            return false;
    }
    return true;
}

/*******************************************************************************
 * FunctionName : ExtFlashPageProg
 * Description  : д Ext Flash ҳ
 * Param        : u8* pBuf 
 * Param        : u32 WrAddr 
 * Param        : u8 WrCmd 
*******************************************************************************/
bool ExtFlashPageProg(u8 *pBuf, u32 WrAddr, u8 WrCmd)
{
    eepromAccessMutex = true;

//  NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
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
//  NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

    bool aux = eepromAccessMutex; // was this function interrupted? Needs atomic operation
    eepromAccessMutex = false;
    return aux;
}

/*******************************************************************************
 * FunctionName : ExtFlashDataRd
 * Description  : Ext Flash Read Data
 * Param        : u8* pBuf 
 * Param        : u32 RdAddr 
 * Param        : u16 Lenght 
*******************************************************************************/
bool ExtFlashDataRd(u8 *pBuf, u32 RdAddr, u16 Lenght)
{
    eepromAccessMutex = true;
    gFlashReadRange[0] = min(gFlashReadRange[0], RdAddr);
    gFlashReadRange[1] = max(gFlashReadRange[1], RdAddr);

// disabling isr disconnects usb
//  NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    ExtFlashWaitForWrEnd();
    ExtFlash_CS_LOW();
    ExtFlashSendByte(READ);
    ExtFlashSendByte((RdAddr & 0xFF0000) >> 16);
    ExtFlashSendByte((RdAddr & 0xFF00) >> 8);
    ExtFlashSendByte(RdAddr & 0xFF);
    for (u16 i = 0; i < Lenght; i++) pBuf[i] = ~ExtFlashReadByte();
    ExtFlash_CS_HIGH();
//  NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

    bool aux = eepromAccessMutex; // was this function interrupted? Needs atomic operation
    eepromAccessMutex = false;
    return aux;
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
        if (Tout++ > TMAX) 
        {
            _ASSERT(0);
            break;
        }
    }
    ExtFlash_CS_HIGH();
}

/*********************************  END OF FILE  ******************************/
