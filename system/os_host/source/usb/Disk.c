//TODO: MOVE IT!!!

/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Disk.c
 * Description : 
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include "Disk.h"
#include "msdusb_scsi.h"
#include <usb_regs.h>
#include "msdusb_conf.h"
#include "msdusb_bot.h"
#include <usb_mem.h>
#include <USB_lib.h>
#include "../../library/COMMON/commonusb_pwr.h"
#include "../../library/STM32F10x_StdPeriph_Driver/inc/stm32f10x_flash.h"
#include "../bios/imports.h"
#include "../bios/fat.h"

#define TXFR_IDLE      0
#define TXFR_ONGOING   1

#define W_ADDR         0
#define ADDR           1
#define H_ADDR         2
#define OFFSET         3
#define SEC_CNT        4
#define COUNT          5
#define RD_CNT         6
#define WR_CNT         7

#define USB_ST         0
#define SEG_KIND       1
#define SEG_LEN        2
#define SEG_SUM        3
#define SEG_TMP        4
#define SEG_ST         5
#define DATA_CNT       6
#define F_TYPE         7
#define F_FLAG         8
#define SEG_DATA       9              

void Disk_SecWr(u8 *pBuf, u32 DiskAddr);
void Disk_SecRd(u8 *pBuf, u32 DiskAddr);
u8   Char2Nib(u8 x);
u8   *SecBuf, *Var, *Data;   u32 *V32; // 
// Join with BIOS::FAT::gSharedBuffer to save 4kB
u8   DiskBuf[BIOS_FAT_SectorSize+32+28];       // USB 
extern bool gUsbDidWriteToDisk;

/*******************************************************************************
 * FunctionName : Disk_Init
 * Description  : Virtual Disk initial   
 * Param        : void 
*******************************************************************************/
void Disk_Init(void)
{
    SecBuf      = (u8 *)&DiskBuf[0];
    V32         = (u32 *)&DiskBuf[BIOS_FAT_SectorSize];
    Var         = (u8 *)&DiskBuf[BIOS_FAT_SectorSize + 0x20];
    Data        = (u8 *)&DiskBuf[BIOS_FAT_SectorSize + 0x20] + SEG_DATA;
    V32[WR_CNT] = 0;
    V32[RD_CNT] = 0;
}

/*******************************************************************************
 * FunctionName : Disk_SecWr
 * Description  : Virtual Disk sector write  
 * Param        : u8* pBuf 
 * Param        : u32 Addr 
*******************************************************************************/
void Disk_SecWr(u8 *pBuf, u32 Addr)
{
    gUsbDidWriteToDisk = true;
    ExtFlashSecWr(pBuf, Addr);
}

/*******************************************************************************
 * FunctionName : Disk_SecRd
 * Description  : Virtual Disk sector read   
 * Param        : u8* pBuf 
 * Param        : u32 Addr 
*******************************************************************************/
void Disk_SecRd(u8 *pBuf, u32 Addr)
{
    ExtFlashDataRd(pBuf, Addr, BIOS_FAT_SectorSize);
}

/*******************************************************************************
 * FunctionName : Read_Memory
 * Description  : Handle the Read operation from the USB disk.      
 * Param        : u32 RdOffset 
 * Param        : u32 RdLength 
*******************************************************************************/
void Read_Memory(u32 RdOffset, u32 RdLength)
{
    static u32 Offset, Length, Block;

    if (Var[USB_ST] == TXFR_IDLE)
    {
        Offset = RdOffset * BIOS_FAT_SectorSize;
        Length = RdLength * BIOS_FAT_SectorSize;
        Var[USB_ST] = TXFR_ONGOING;
    }
    if (Var[USB_ST] == TXFR_ONGOING)
    {
        if (!V32[RD_CNT])
        {
            Disk_SecRd(SecBuf, Offset);
            UserToPMABufferCopy(SecBuf, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
            V32[RD_CNT] = BIOS_FAT_SectorSize - BULK_MAX_PACKET_SIZE;
            Block       = BULK_MAX_PACKET_SIZE;
        } else
        {
            UserToPMABufferCopy(SecBuf + Block, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
            V32[RD_CNT] -= BULK_MAX_PACKET_SIZE;
            Block       += BULK_MAX_PACKET_SIZE;
        }
        SetEPTxCount(ENDP1, BULK_MAX_PACKET_SIZE);
        SetEPTxStatus(ENDP1, EP_TX_VALID);
        Offset += BULK_MAX_PACKET_SIZE;
        Length -= BULK_MAX_PACKET_SIZE;

        CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
    }
    if (Length == 0)
    {
        V32[RD_CNT] = 0;
        Block       = 0;
        Offset      = 0;
        Bot_State   = BOT_DATA_IN_LAST;
        Var[USB_ST] = TXFR_IDLE;
    }
}

/*******************************************************************************
 * FunctionName : Write_Memory
 * Description  : Handle the Write operation to the USB disk.  
 * Param        : u32 WrOffset 
 * Param        : u32 WrLength 
*******************************************************************************/
void Write_Memory(u32 WrOffset, u32 WrLength)
{
    static u32 Offset, Length;
    u32 Idx, temp =  V32[WR_CNT] + 64;

    if (Var[USB_ST] == TXFR_IDLE)
    {
        Offset = WrOffset * BIOS_FAT_SectorSize;
        Length = WrLength * BIOS_FAT_SectorSize;
        Var[USB_ST] = TXFR_ONGOING;
    }
    if (Var[USB_ST] == TXFR_ONGOING)
    {
        for (Idx = 0; V32[WR_CNT] < temp; V32[WR_CNT]++) *(u8 *)(SecBuf + V32[WR_CNT]) = Bulk_Buff[Idx++];

        Offset += Data_Len;
        Length -= Data_Len;

        if (!(Length % BIOS_FAT_SectorSize))
        {
            V32[WR_CNT] = 0;
            Disk_SecWr(SecBuf, Offset - BIOS_FAT_SectorSize);
        }

        CSW.dDataResidue -= Data_Len;
        SetEPRxStatus(ENDP2, EP_RX_VALID);
    }
    if ((Length == 0) || (Bot_State == BOT_CSW_Send))
    {
        V32[WR_CNT] = 0;
        Set_CSW(CSW_CMD_PASSED, SEND_CSW_ENABLE);
        Var[USB_ST] = TXFR_IDLE;
    }
}

/*******************************************************************************
 * FunctionName : USB_Connect
 * Description  : USB
 * Param        : u8 Status  ENBL/DSBL
*******************************************************************************/
#if 0
void USB_Connect(u8 Status)
{
    if(Status == DISABLE)  
    {
        while(PowerOff() != USB_SUCCESS)
        {};
//        BIOS::SYS:Delay(200);
        Delay_mS(200);
        GPIOA->CRH    |= 0x00033000;             // USB DN & DP Pins Out_Mode
        GPIOA->CRH    &= 0xFFF33FFF;  
    }
    else                          
    {
        GPIOA->CRH |= 0x000BB000;                // USB DN & DP Pins AF_Mode
        GPIOA->CRH &= 0xFFFBBFFF;
        USB_Init();
    }
}
#endif
/*********************************  END OF FILE  ******************************/
