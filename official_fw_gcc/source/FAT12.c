/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : FAT12.c
 * Description : 文件管理系统
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include <string.h>
#include "FAT12.h"
#include "AppBios.h"
#include "Func.h"
#include "Ext_Flash.h"


typedef struct
{
    u32 FAT1_BASE;          // FAT1区开始地址
    u32 FAT2_BASE;          // FAT2区开始地址
    u32 ROOT_BASE;          // 根目录开始地址
    u32 FILE_BASE;          // 文件区开始地址
    u32 FAT_LEN;
    u32 SEC_LEN;            // 扇区长度
    u32 FAT_END;            // 链接结束
    u8  FAT1_SEC;           // FAT1扇区数
    u8  FAT2_SEC;
}FAT_InitTypeDef;

u8  Clash;

FAT_InitTypeDef FAT_V;

int Init_Fat_Value(void) 
{
    FAT_V.FAT1_BASE = FAT1_BASE_8M;
    FAT_V.FAT2_BASE = FAT2_BASE_8M;
    FAT_V.ROOT_BASE = ROOT_BASE_8M;
    FAT_V.FILE_BASE = FILE_BASE_8M;
    FAT_V.FAT1_SEC = FAT1_SEC_8M;
    FAT_V.FAT2_SEC = FAT1_SEC_8M;
    FAT_V.SEC_LEN = SEC_LEN_8M;
    FAT_V.FAT_LEN = FAT_LEN_8M;
    FAT_V.FAT_END = FAT_END_8M;

    return 0;
}

/*******************************************************************************
 * FunctionName : ReadDiskData
 * Description  : 读磁盘页面(256 Bytes)  包含USB读写冲突后重读
 * Param        : u8* pBuffer 
 * Param        : u32 ReadAddr 
 * Param        : u16 Lenght 
 * Return       : u8   
*******************************************************************************/
u8 ReadDiskData(u8 *pBuffer, u32 ReadAddr, u16 Lenght)
{
    u8 n = 0;

    while (1)
    {
        Clash = 0;
        ExtFlashDataRd(pBuffer, ReadAddr, Lenght);
        if (n++ > 6) return SEC_ERR;     // 超时出错返回
        if (Clash == 0) return OK;       // 无冲突产生则返回
    }
}

/*******************************************************************************
 * FunctionName : ProgDiskPage
 * Description  : 写磁盘页面(256 Bytes)  包含USB读写冲突后重写
 * Param        : u8* pBuffer 
 * Param        : u32 ProgAddr 
 * Return       : u8   
*******************************************************************************/
u8 ProgDiskPage(u8 *pBuffer, u32 ProgAddr)
{
    u8   n = 0;

    while (1)
    {
        Clash = 0;
        ExtFlashSecWr(pBuffer, ProgAddr);
        if (n++ > 6) return SEC_ERR;     // 超时出错返回
        if (Clash == 0) return OK;       // 无冲突产生则返回
    }
}

/*******************************************************************************
 * FunctionName : NextCluster
 * Description  : 查找下一个链接簇号后返回，当前簇号保存在指针+1的位置
 * Param        : u16* pCluster 
 * Return       : u8   
*******************************************************************************/
u8 NextCluster(u16 *pCluster)
{
    u16 FatNum;
    u32 Addr;

    Addr = FAT_V.FAT1_BASE + (*pCluster + *pCluster / 2);

    *(pCluster + 1) = *pCluster;                                   // 保存前一个簇号
    *pCluster = 0;
    if ((*(pCluster + 1) >= FAT_V.FAT_END) || (*(pCluster + 1) < 2)) return SEC_ERR;
    if (ReadDiskData((u8 *)&FatNum, Addr, 2) != OK) return SEC_ERR;
    *pCluster = (*(pCluster + 1) & 1) ? (FatNum >> 4) : (FatNum & 0xFFF); //指向下一个簇号
    return OK;
}

/*******************************************************************************
 * FunctionName : ReadFileSec
 * Description  : 读文件扇区(512 Bytes), 返回时指针指向下一个簇号， 
 *                当前簇号保存在指针+1的位置
 * Param        : u8* pBuffer 
 * Param        : u16* pCluster 
 * Return       : u8   
*******************************************************************************/
u8 ReadFileSec(u8 *pBuffer, u16 *pCluster)
{
    u32 ReadAddr = FAT_V.FILE_BASE + FAT_V.SEC_LEN * (*pCluster - 2);

    if (ReadDiskData(pBuffer, ReadAddr, FAT_V.SEC_LEN) != OK) return SEC_ERR;
    if (NextCluster(pCluster) != 0) return FAT_ERR;                 // 取下一个簇号
    return OK;
}

/*******************************************************************************
 * FunctionName : ProgFileSec
 * Description  : 写文件扇区(512/4096 Bytes)，填写当前FAT表及返回查找到的下一个簇号
 * Param        : u8* pBuffer 
 * Param        : u16* pCluster 
 * Return       : u8   
*******************************************************************************/
u8 ProgFileSec(u8 *pBuffer, u16 *pCluster)
{
    u16 Tmp;
    u32 ProgAddr = FAT_V.FILE_BASE + FAT_V.SEC_LEN * (*pCluster - 2);
    if (ProgDiskPage(pBuffer, ProgAddr) != OK) return SEC_ERR;

    if (NextCluster(pCluster) != 0) return FAT_ERR;                 // 取下一个簇号
    Tmp = *(pCluster + 1);
    if (*pCluster == 0)
    {
        *pCluster = Tmp;
        if (SeekBlank(pBuffer, pCluster) != OK) return OVER;
        if (SetCluster(pBuffer, pCluster) != OK) return SEC_ERR;
    }
    return OK;
}

/*******************************************************************************
 * FunctionName : SeekBlank
 * Description  : 查找空闲簇号，返回时指针指向下一个空闲簇号，当前簇号保存在指针+1的位置
 * Param        : u8* pBuffer 
 * Param        : u16* pCluster 
 * Return       : u8   
*******************************************************************************/
u8 SeekBlank(u8 *pBuffer, u16 *pCluster)
{
    u16  Tmp;
    u8   Buffer[2];
    u8   Tmp_Flag = 1;

    *(pCluster + 1) = *pCluster;                                    // 保存当前簇号

    for (*pCluster = 0; (*pCluster) < 4095; (*pCluster)++)
    {
        if (ReadDiskData(Buffer, FAT_V.FAT1_BASE + (*pCluster) + (*pCluster) / 2, 2) != 0) return SEC_ERR;
        Tmp = ((*pCluster) & 1) ? ((*(u16 *)Buffer) >> 4) : ((*(u16 *)Buffer) & 0xFFF);

        if ((Tmp == 0) && (Tmp_Flag == 0) && (((*pCluster)) != *(pCluster + 1)))
        {
            Tmp_Flag = 1;
            return OK;
        }
        if ((Tmp == 0) && (Tmp_Flag == 1))
        {
            *(pCluster + 2) = *pCluster;
            Tmp_Flag = 0;
        }
    }
    return OK;
}

/*******************************************************************************
 * FunctionName : SetCluster
 * Description  : 将下一簇号写入FAT表当前簇链接位，返回时指针指向下一簇号， 
 *                指针+1为当前簇号
 * Param        : u8* pBuffer 
 * Param        : u16* pCluster 
 * Return       : u8   
*******************************************************************************/
u8 SetCluster(u8 *pBuffer, u16 *pCluster)
{
    u16  Offset, i, k;
    u32  SecAddr;

    i = *(pCluster + 1);                  // 提取原当前簇号
    k = *pCluster;                        // 提取下一簇号
    Offset = i + i / 2;
    SecAddr = FAT_V.FAT1_BASE + (Offset & 0xF000);
    Offset &= 0x0FFF;
    if (ReadDiskData(pBuffer, SecAddr, FAT_V.SEC_LEN) != 0) return SEC_ERR;
    if (i & 1)
    {
        pBuffer[Offset] = (pBuffer[Offset] & 0x0F) + ((k << 4) & 0xF0);
        pBuffer[Offset + 1] = k >> 4;
    } else
    {
        pBuffer[Offset] = k & 0xFF;
        pBuffer[Offset + 1] = (pBuffer[Offset + 1] & 0xF0) + ((k >> 8) & 0x0F);
    }

    if (ProgDiskPage(pBuffer, SecAddr) != 0) return SEC_ERR;
    return OK;
}

/*******************************************************************************
 * FunctionName : OpenFileRd
 * Description  : 读模式打开文件：返回文件第一个簇号及目录项地址 
 *                或 0簇号及第一个空白目录项地址
 * Param        : u8* pBuffer 
 * Param        : u8* pFileName 
 * Param        : u16* pCluster 
 * Param        : u32* pDirAddr 
 * Return       : u8   
*******************************************************************************/
u8 OpenFileRd(u8 *pBuffer, u8 *pFileName, u16 *pCluster, u32 *pDirAddr)
{
    u16 i, n;

    *pCluster = 0;
    for (*pDirAddr = FAT_V.ROOT_BASE; *pDirAddr < FAT_V.FILE_BASE;)
    {
        if (ReadDiskData(pBuffer, *pDirAddr, FAT_V.SEC_LEN) != OK) return SEC_ERR;
        for (n = 0; n < FAT_V.SEC_LEN; n += 32)
        {
            for (i = 0; i < 11; i++)
            {
                if (pBuffer[n + i] != 0)
                {
                    if (pBuffer[n + i] != pFileName[i]) break;
                    if (i == 10)                                  // 找到文件名
                    {
                        *pCluster = *(u16 *)(pBuffer + n + 0x1A); // 文件第一个簇号
                        return OK;
                    }
                } else return NEW;               // 遇到第一个空白目录项后返回
            }
            *pDirAddr += 32;
        }
    }
    return OVER;
}

/*******************************************************************************
 * FunctionName : OpenFileWr
 * Description  : 写模式打开文件：返回文件第一个簇号及目录项地址
 * Param        : u8* pBuffer 
 * Param        : u8* pFileName 
 * Param        : u16* pCluster 
 * Param        : u32* pDirAddr 
 * Return       : u8   
*******************************************************************************/
u8 OpenFileWr(u8 *pBuffer, u8 *pFileName, u16 *pCluster, u32 *pDirAddr)
{
    u32 i, n, offset;

    i = OpenFileRd(pBuffer, pFileName, pCluster, pDirAddr);
    if (i != NEW) return i;
    else                                                     // 当前项为空白目录项
    {
        if (SeekBlank(pBuffer, pCluster) != OK) return OVER; // 若FAT表满返回
        n = *pDirAddr & 0xFFF;
        offset = *pDirAddr - n;
        if (ReadDiskData(pBuffer, offset, FAT_V.SEC_LEN) != OK) return SEC_ERR;
        for (i = 0; i < 11; i++) pBuffer[n + i] = pFileName[i]; // 创建新目录项offset +
        *(u16 *)(pBuffer + n + 0x1A) = *pCluster;
        if (ProgDiskPage(pBuffer, offset) != OK) return SEC_ERR;
        return OK;
    }
}

/*******************************************************************************
 * FunctionName : CloseFile
 * Description  : 关闭文件，将结束符写入FAT表， 
 *                将文件长度写入目录项，复制FAT1到FAT2
 * Param        : u8* pBuffer 
 * Param        : u32 Lenght 
 * Param        : u16* pCluster 
 * Param        : u32* pDirAddr 
 * Return       : u8   
*******************************************************************************/
u8 CloseFile(u8 *pBuffer, u32 Lenght, u16 *pCluster, u32 *pDirAddr)
{
    u32 n;
    n = 0xFFF;
    *pCluster = 0xFFF;
    SetCluster(pBuffer, pCluster);

    if (ReadDiskData(pBuffer, (*pDirAddr & (~n)), FAT_V.SEC_LEN) != OK) return SEC_ERR;
    *(u8 *)(pBuffer + (*pDirAddr & n) + 0x0B) = 0x20;
    *(u32 *)(pBuffer + (*pDirAddr & n) + 0x1C) = Lenght;
    if (ProgDiskPage(pBuffer, (*pDirAddr & (~n))) != OK) return SEC_ERR;
    if (ReadDiskData(pBuffer, FAT_V.FAT1_BASE, FAT_V.SEC_LEN) != OK) return SEC_ERR;
    if (ProgDiskPage(pBuffer, FAT_V.FAT2_BASE) != OK) return SEC_ERR;

    return OK;
}


/*********************************  END OF FILE  ******************************/
