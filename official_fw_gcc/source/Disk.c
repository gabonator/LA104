/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Disk.c
 * Description : 
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include "MyDefine.h"
#include "USB_scsi.h"
#include "USB_regs.h"
#include "USB_conf.h"
#include "USB_bot.h"
#include "USB_mem.h"
#include "USB_lib.h"
#include "USB_pwr.h"
#include "stm32f10x_flash.h"

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
u8   *SecBuf, *Var, *Data;   u32 *V32; // 磁盘工作区指针
u8   DiskBuf[SECTOR_SIZE+32+28];       // USB 磁盘工作数据区


/*******************************************************************************
 * FunctionName : Disk_Init
 * Description  : Virtual Disk initial   
 * Param        : void 
*******************************************************************************/
void Disk_Init(void)
{
    SecBuf      = (u8 *)&DiskBuf[0];
    V32         = (u32 *)&DiskBuf[SECTOR_SIZE];
    Var         = (u8 *)&DiskBuf[SECTOR_SIZE + 0x20];
    Data        = (u8 *)&DiskBuf[SECTOR_SIZE + 0x20] + SEG_DATA;
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
    ExtFlashDataRd(pBuf, Addr, SECTOR_SIZE);
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
        Offset = RdOffset * SECTOR_SIZE;
        Length = RdLength * SECTOR_SIZE;
        Var[USB_ST] = TXFR_ONGOING;
    }
    if (Var[USB_ST] == TXFR_ONGOING)
    {
        if (!V32[RD_CNT])
        {
            Disk_SecRd(SecBuf, Offset);
            UserToPMABufferCopy(SecBuf, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
            V32[RD_CNT] = SECTOR_SIZE - BULK_MAX_PACKET_SIZE;
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
        Offset = WrOffset * SECTOR_SIZE;
        Length = WrLength * SECTOR_SIZE;
        Var[USB_ST] = TXFR_ONGOING;
    }
    if (Var[USB_ST] == TXFR_ONGOING)
    {
        for (Idx = 0; V32[WR_CNT] < temp; V32[WR_CNT]++) *(u8 *)(SecBuf + V32[WR_CNT]) = Bulk_Buff[Idx++];

        Offset += Data_Len;
        Length -= Data_Len;

        if (!(Length % SECTOR_SIZE))
        {
            V32[WR_CNT] = 0;
            Disk_SecWr(SecBuf, Offset - SECTOR_SIZE);
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
 * Description  : USB连接控制
 * Param        : u8 Status  ENBL/DSBL
*******************************************************************************/
void USB_Connect(u8 Status)
{
    if(Status == DISABLE)  
    {
        while(PowerOff() != USB_SUCCESS)
        {};
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
/*********************************  END OF FILE  ******************************/
