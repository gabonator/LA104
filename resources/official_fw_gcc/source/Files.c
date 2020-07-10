/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Files.c
 * Description : 文件管理
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include "MyDefine.h"
#include "Files.h"
#include <stdlib.h>
#include "string.h"
#include "STM32F10x.h"
#include "stm32f10x_flash.h"

#define ParamAddress    0x08007800


/*
uc8  BmpHead[54] = {                               
    0X42, 0X4D, 0X76, 0X96, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X76, 0X00, 0X00, 0X00, 0X28, 0X00,
    0X00, 0X00, 0X40, 0X01, 0X00, 0X00, 0XF0, 0X00,
    0X00, 0X00, 0X01, 0X00, 0X08, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X82, 0X0B, 0X00, 0X00, 0X12, 0X0b,
    0X00, 0X00, 0X12, 0X0b, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00 };          
*/
uc8  BmpHead[54] = {  
//  文件类型    大小                    保留
    0X42, 0X4D, 0X38, 0X58, 0X02, 0X00, 0X00, 0X00, //07
//  保留        偏移                    结果大小
    0X00, 0X00, 0X36, 0X00, 0X00, 0X00, 0X28, 0X00, //15
//  宽度                    高度
    0X00, 0X00, 0X40, 0X01, 0X00, 0X00, 0XF0, 0X00, //23
//  面数                    比特数/像素 压缩   
    0X00, 0X00, 0X01, 0X00, 0X10, 0X00, 0X00, 0X00, //31
    0X00, 0X00, 0X02, 0X58, 0X02, 0X00, 0X12, 0X0b, //39
    0X00, 0X00, 0X12, 0X0b, 0X00, 0X00, 0X00, 0X00, //47
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00 };

uc16 CLK_TAB[44] = {
    0x000, 0x000, 0x000, 0x000, 0x070, 0x3FF, 0x070, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x002, 0x004, 0x008, 0x070, 0x070, 0x070, 0x080, 0x100, 0x200, 0x000,
    0x020, 0x020, 0x020, 0x020, 0x070, 0x070, 0x070, 0x020, 0x020, 0x020, 0x020,
    0x000, 0x200, 0x100, 0x080, 0x070, 0x070, 0x070, 0x008, 0x004, 0x002, 0x000 };

uc16 BMP_Color[16] = {
    WHT,  CYN, CYN_, YEL, YEL_, PUR, PUR_, GRN,
    GRN_, GRY, ORN,  BLU, RED,  LGN, DAR,  BLK };

u16  RsvdSecCnt, SectorSize, FAT_Size, FileLen;
u16  SecPerClus, DirFliePtr, DirSecNum, FAT1_Addr, FAT_Ptr;
u32  DiskStart, Hidden_Sec, Root_Addr, ClusterNum, File_Addr;
u32 *FilePtr;

/*******************************************************************************
 * FunctionName : Color_Num
 * Description  : 求出当前颜色的对应调色板编号
 * Param        : u16 Color 
 * Return       : u8   
*******************************************************************************/
u8 Color_Num(u16 Color)
{
    if(Color == WHT)                 return 0;
    else if((Color & CYN) == CYN)    return 1;
    else if((Color & CYN_) == CYN_)  return 2;
    else if((Color & YEL) == YEL)    return 3;
    else if((Color & YEL_) == YEL_)  return 4;
    else if((Color & PUR) == PUR)    return 5;
    else if((Color & PUR_) == PUR_)  return 6;
    else if((Color & GRN) == GRN)    return 7;
    else if((Color & GRN_) == GRN_)  return 8;
    else if((Color & GRY) == GRY)    return 9;
    else if((Color & ORN) == ORN)    return 10;
    else if((Color & BLU) == BLU)    return 11;
    else if((Color & RED) == RED)    return 12;
    else if((Color & LGN) == LGN)    return 13;
    else if((Color & DAR) == DAR)    return 14;
    else                              return 15;
}

/*******************************************************************************
 * FunctionName : Make_Filename
 * Description  : 打开指定扩展名的文件  
 * Param        : u16 FileNo 
 * Param        : unsigned char* FileName 
*******************************************************************************/
void Make_Filename(u16 FileNo, unsigned char *FileName)
{
    u8 Num[4];

    u8ToDec3Str(Num, FileNo);
    FileName[4] = Num[0];
    FileName[5] = Num[1];
    FileName[6] = Num[2];
}

/*******************************************************************************
 * FunctionName : Save_Bmp
 * Description  : 保存当前屏幕显示图像为BMP格式
 * Param        : u16 FileNo 
 * Return       : u8   
*******************************************************************************/
u8 Save_Bmp(u16 FileNo)
{
    u8  pFileName[12] = "IMG_    BMP";
    u16 pCluster[3];
    u32 pDirAddr[1];
    u32 Rvalue = DISK_RW_ERR;
//  s16 x, y, i = 54, j, ColorH, ColorL;
    s32 x, y, j, ColorH, ColorL;
    u32 i = 0;
    u16 Tmp = 0;
    u16 k = 0, l = 0;
    u32 length = 4096;

    USB_Connect(DISABLE);
    Make_Filename(FileNo, pFileName);
    if(OpenFileWr(DiskBuf, pFileName, pCluster, pDirAddr) != OK) return Rvalue;
    memcpy(DiskBuf, BmpHead, 54);
    i = 0x0036;                                                 // 调色板存放开始地址
/*
    for(j = 0; j < 16; ++j)                                              
    {                                                                    
        DiskBuf[j * 4 + i + 0] = (BMP_Color[j] & 0xF800) >> 8;  // Bule  
        DiskBuf[j * 4 + i + 1] = (BMP_Color[j] & 0x07E0) >> 3;  // Green&
        DiskBuf[j * 4 + i + 2] = (BMP_Color[j] & 0x001F) << 3;  // Red   
        DiskBuf[j * 4 + i + 3] = 0;                             // Alpha 
    }                                                                    
*/
/*
    for(j = 0; j < 256; j++)                                                                       
    {                                                                                              
        DiskBuf[j * 4 + i + 0] = j & 0xF8;  // Bule                                                
        DiskBuf[j * 4 + i + 1] = j & 0x7E;  // Green&                                              
        DiskBuf[j * 4 + i + 2] = j & 0x1F;  // Red                                                 
        DiskBuf[j * 4 + i + 3] = 0;                                                                
    }                                                                                              
                                                                                                   
    i = 54+256*4;                                                 // 图像数据开始存放地址
*/
    for(y = 0; y < 240; y++)
    {
        for(x = 0; x < 320; x++)
        {
            Set_Posi(x, y);
            ColorH = Read_Pixel();

            //BRG565转BGR555;
            Tmp = ColorH;
            ColorH  = (ColorH>>1)&0xFFE0;
            ColorH |= (Tmp & 0x1F);
            //BGR555z转RGB555
            Tmp = ColorH;
            ColorH = ((ColorH&0x7c00)>>10)|(Tmp & 0x03E0);
            ColorH |=  (Tmp & 0x1F)<<10;




//          Set_Posi(x + 1, y);
//          ColorL = Read_Pixel();
//          DiskBuf[i] = (Color_Num(ColorH) << 4) + Color_Num(ColorL);
//          DiskBuf[i] = ((u8)ColorH << 4 + (u8)ColorL);
//          DiskBuf[i] = (u8)(ColorH>>8);
            DiskBuf[i] = (u8)(ColorH & 0x00FF);
            i++;
            if(i >= length)
            {
                i = 0;
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR;   // 写入数据
                if(l == 0) PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (k++ >> 1) & 3); // 进度指示
                l++;
                if(l >= 2) l = 0;
            }
            DiskBuf[i] = (u8)(ColorH>>8) & 0xFF;
//          DiskBuf[i] = (u8)(ColorH & 0x00FF);;
            i++;
//          DiskBuf[i] = 0x55;
//          i++;
            if(i >= length)
            {
                i = 0;
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR;   // 写入数据
                if(l == 0) PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (k++ >> 1) & 3); // 进度指示
                l++;
                if(l >= 2) l = 0;
            }
        }
    }
/**/
    if(i != 0)                                  
    {                                           
        if(ProgFileSec(DiskBuf, pCluster) != OK)
        {return FILE_RW_ERR;}                   
    }                                           

    if(CloseFile(DiskBuf, 152 *2* 512, pCluster, pDirAddr) != OK) return FILE_RW_ERR;
    if(gItemParam[SAVEBMP] < 99) gItemParam[SAVEBMP]++;
    USB_Connect(ENABLE);
    return OK;
}


/*******************************************************************************
Save_Csv: 保存采集数据缓存区为CSV格式    输入：文件编号     返回值：0x00=成功
*******************************************************************************/
u8 Save_Csv(s16 FileNo)
{

    u8 pFileName[12] = "DATA    CSV";
    u16 pCluster[3];
    u32 pDirAddr[1];
    u32 i, k = 0, l = 0, Rvalue = DISK_RW_ERR, length;
    u8 Num[12];
    u8 count;
    u8 CH_Status = 0;
    u32 TransTime = 0;

    length = 4096;

    USB_Connect(DISABLE);
    Make_Filename(FileNo, pFileName);
    if(OpenFileWr(DiskBuf, pFileName, pCluster, pDirAddr) != OK) return Rvalue;


    memcpy(DiskBuf, "Time(nS), CH1, CH2, CH3, CH4,", 29);                // 标题栏

    k += 29;
    memcpy(&DiskBuf[k], "\r\n", 2);
    k += 2;

    for(i = TRI_START_NUM; i < RECORD_DEPTH - 3; i++)
    {
        CH_Status = TransStatusData(i);
        TransTime = TransformTime(i + 1) * 10;

        memset(Num, 0, 12);                                              // 时间
        u32ToDecStr(Num, TransTime);
        for(count = 0; count < 12; count++)
        {
            if(Num[count] == 0) break;
            DiskBuf[k++] = Num[count];

            if(k >= length)
            {
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
                PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
                k = 0;
            }
        }
        DiskBuf[k++] = 0x2c;                                             // ','
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }


        memset(Num, 0, 12);                                              // CH1状态
        Char2Hex(Num, CH_Status & 0x01);
        for(count = 0; count < 3; count++)
        {
            if(Num[count] == 0) break;
            DiskBuf[k++] = Num[count];

            if(k >= length)
            {
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
                PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
                k = 0;
            }
        }
        DiskBuf[k++] = 0x2c;
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }

        memset(Num, 0, 12);                                              // CH2状态
        Char2Hex(Num, (CH_Status >> 1) & 0x01);
        for(count = 0; count < 3; count++)
        {
            if(Num[count] == 0) break;
            DiskBuf[k++] = Num[count];

            if(k >= length)
            {
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
                PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
                k = 0;
            }
        }
        DiskBuf[k++] = 0x2c;
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }

        memset(Num, 0, 12);                                              // CH3状态
        Char2Hex(Num, (CH_Status >> 2) & 0x01);
        for(count = 0; count < 3; count++)
        {
            if(Num[count] == 0) break;
            DiskBuf[k++] = Num[count];

            if(k >= length)
            {
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
                PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
                k = 0;
            }
        }
        DiskBuf[k++] = 0x2c;
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }

        memset(Num, 0, 12);                                              // CH4状态
        Char2Hex(Num, (CH_Status >> 3) & 0x01);
        for(count = 0; count < 3; count++)
        {
            if(Num[count] == 0) break;
            DiskBuf[k++] = Num[count];

            if(k >= length)
            {
                if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
                PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
                k = 0;
            }
        }
        DiskBuf[k++] = 0x2c;
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }

        DiskBuf[k++] = 0x0d;                                             // 回车换行
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }
        DiskBuf[k++] = 0x0a;
        if(k >= length)
        {
            if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR; // 写入数据
            PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);          // 进度指示
            k = 0;
        }


    }                                                                    // 循环结束


    if(k != 0)
    {
        DiskBuf[k++] = 0x0d;
        DiskBuf[k++] = 0x0a;
        memset(&DiskBuf[k], 0x20, (length - k));
        k = 0;
        if(ProgFileSec(DiskBuf, pCluster) != OK) return FILE_RW_ERR;     // 写入数据
        PrintClk(SAVE_ICO_X0, SAVE_ICO_Y0, (l++ >> 1) & 3);              // 进度指示
    }
    if(CloseFile(DiskBuf, l * length, pCluster, pDirAddr) != OK) return DISK_RW_ERR;
    if(gItemParam[SAVECSV] < 99) gItemParam[SAVECSV]++;
    USB_Connect(ENABLE);
    return OK;
}

/*******************************************************************************
 * FunctionName : Load_File_Num
 * Description  : 
 * Param        : u8 Tpye 输入：文件类型    
 * Return       : u16 返回值：文件编号
*******************************************************************************/
u16 Load_File_Num(u8 Tpye)
{
    u16 FileNo = 0;
    u8  pFileName[12] = "FILE       ";
    u16 pCluster[3];
    u32 pDirAddr[1];

    switch(Tpye)
    {
    case SAVEBMP:
        memset(DiskBuf, 0, 4096);
        memcpy(pFileName, "IMG_    BMP", 12);
        Make_Filename(FileNo, pFileName);
        while(OpenFileRd(DiskBuf, pFileName, pCluster, pDirAddr) == OK)
        {
            FileNo++;
            Make_Filename(FileNo, pFileName);
        }
        break;
    case SAVEDAT:
        memset(DiskBuf, 0, 4096);
        memcpy(pFileName, "FILE    DAT", 12);
        Make_Filename(FileNo, pFileName);
        while(OpenFileRd(DiskBuf, pFileName, pCluster, pDirAddr) == OK)
        {
            FileNo++;
            Make_Filename(FileNo, pFileName);
        }
        break;
    case SAVEBUF:
        memset(DiskBuf, 0, 4096);
        memcpy(pFileName, "DATA    BUF", 12);
        Make_Filename(FileNo, pFileName);
        while(OpenFileRd(DiskBuf, pFileName, pCluster, pDirAddr) == OK)
        {
            FileNo++;
            Make_Filename(FileNo, pFileName);
        }
        break;
    case SAVECSV:
        memset(DiskBuf, 0, 4096);
        memcpy(pFileName, "DATA    CSV", 12);
        Make_Filename(FileNo, pFileName);
        while(OpenFileRd(DiskBuf, pFileName, pCluster, pDirAddr) == OK)
        {
            FileNo++;
            Make_Filename(FileNo, pFileName);
        }
        break;
    case SAVESVG:
        memset(DiskBuf, 0, 4096);
        memcpy(pFileName, "DATA    SVG", 12);
        Make_Filename(FileNo, pFileName);
        while(OpenFileRd(DiskBuf, pFileName, pCluster, pDirAddr) == OK)
        {
            FileNo++;
            Make_Filename(FileNo, pFileName);
        }
        break;
    }
    return FileNo;
}

/*******************************************************************************
 * FunctionName : File_Num
 * Description  : 读取文件编号
 * Param        : void 
*******************************************************************************/
void File_Num(void)
{
    gItemParam[SAVEBMP] =  Load_File_Num(SAVEBMP);
    gItemParam[SAVECSV] =  Load_File_Num(SAVECSV);
}
/*******************************************************************************
 * FunctionName : PrintClk
 * Description  : 保存进度指示
 * Param        : u16 x0 
 * Param        : u16 y0 
 * Param        : u8 Phase 
*******************************************************************************/
void PrintClk(u16 x0, u16 y0, u8 Phase)
{
    u8  j, k;
    u8  Str = 0;
    u16 Sx, Sy;
    Sx = x0 + 3;
    Sy = y0;
    Set_Posi(Sx, Sy);

    for(j = 0; j < 12; j++)
    {
        Str = Run_ICO[Phase * 12 + j];

        Set_Posi(Sx, Sy);
        Set_Pixel(GRAY);
        Set_Posi(Sx, Sy + 1);
        for(k = 0; k < 8; k++)
        {
            if(Str & 0x01) Set_Pixel(RED);
            else            Set_Pixel(GRAY);
            Str >>= 1;
        }
        Sx++;                 //显示位置水平方向+1
    }
}

/*******************************************************************************
 * FunctionName : Clear_File_ICO
 * Description  : //清除保存过程图标
 * Param        : u16 x 
 * Param        : u16 y 
*******************************************************************************/
void Clear_File_ICO(u16 x, u16 y)
{
    Disp_Str6x8(x, y, GRAY, GRAY, PRN, THIN, "   ");
}
/*******************************************************************************
 * FunctionName : DispFileInfo
 * Description  : 文件存储提示
 * Param        : u8 Info 
*******************************************************************************/
void DispFileInfo(u8 Info)
{
    u8 FnNote[][5] = {
        "O K",          /*"文件操作完成"   */
        "Err",          /*"文件版本错误"   */
        "Err",          /*"文件名不存在"   */
        "Err",          /*"文件操作错误"   */
        "Err",          /*"磁盘读写错误"   */
    };
    
    Set_Color(GRAY, RED);
    Show_Str6x8(FILE_ICO_X0, FILE_ICO_Y0, PRN, THIN, ((u8 *)&FnNote + 5 * Info));
    Delay_mS(500);
}
/*******************************************************************************
 * FunctionName : SaveParameter
 * Description  : 保存系统参数到内部Flash
 * Return       : 返回保存状态   
*******************************************************************************/
u8 SaveParameter(void)
{
    u16  i, j, k;
    u16 *ptr;

    FLASH_Unlock();
    j = FLASH_ErasePage(ParamAddress);
    if(j == FLASH_COMPLETE)
    {
        ptr = (u16 *)&DiskBuf;
        *ptr++ = 'L';                                              // 型号：LA104
        *ptr++ = 'A';
        *ptr++ = '1';
        *ptr++ = '0';
        *ptr++ = '4';
        *ptr++ = 0x0100;                                           // 版本号：V1.00
        *ptr++ = Menu.index;                                       // 当前菜单选项
        for(i = 0; i < MENU_NUM; i++) *ptr++ = gItemIndexNum[i];             // 当前项目选项
        for(i = 0; i < ITEM_NUM; i++) *ptr++ = gInSubIndexNum[IN_TYPE][i];   // 当前In子项目选项
        for(i = 0; i < ITEM_NUM; i++) *ptr++ = gOutSubIndexNum[OUT_TYPE][i]; // 当前Out子项目选项
        for(i = 0; i < PARAM_NUM; i++) *ptr++ = gItemParam[i];               // 全部项目参数
        for(i = 0; i < ITEM_NUM; i++)
        {
            for(k = 0; k < SUB_NUM; k++) *ptr++ = gInputSubParam[i][k];      // input子项目参数
            for(k = 0; k < SUB_NUM; k++) *ptr++ = gOutSubParam[i][k];        // input子项目参数
        }
        *ptr++ = 0xaa55;
        ptr = (u16 *)&DiskBuf;
        for(i = 0; i < 512; i += 2)                                         
        {
            j = FLASH_ProgramHalfWord(ParamAddress + i, *ptr++);
            if(j != FLASH_COMPLETE) break;
        }
    }
    FLASH_Lock();

    if(j == FLASH_COMPLETE) j = 0; // OK
    else                    j = 3; // Err

    return j;
}

/*******************************************************************************
 * FunctionName : ReadParameter
 * Description  : 从内部Flash读取系统参数
 * Return       : 返回读取状态：0失败，1成功    
*******************************************************************************/
u8 ReadParameter(void)
{
    u16 i, j;
    u16 *ptr;
    ptr = (u16 *)ParamAddress;
    if(*ptr++ != 'L') return 0;
    if(*ptr++ != 'A') return 0;
    if(*ptr++ != '1') return 0;
    if(*ptr++ != '0') return 0;
    if(*ptr++ != '4') return 0;
    if(*ptr++ != 0x0100) return 0;
    Menu.index = *ptr++;
    for(i = 0; i < MENU_NUM; i++) gItemIndexNum[i] = *ptr++;             // 当前项目选项
    for(i = 0; i < ITEM_NUM; i++) gInSubIndexNum[IN_TYPE][i] = *ptr++;   // 当前子项目选项
    for(i = 0; i < ITEM_NUM; i++) gOutSubIndexNum[OUT_TYPE][i] = *ptr++; // 当前子项目选项
    for(i = 0; i < PARAM_NUM; i++) gItemParam[i] = *ptr++;               // 全部项目参数
    for(i = 0; i < ITEM_NUM; i++)
    {
        for(j = 0; j < SUB_NUM; j++) gInputSubParam[i][j] = *ptr++;      // input子项目参数
        for(j = 0; j < SUB_NUM; j++) gOutSubParam[i][j] = *ptr++;        // output子项目参数
    }

    return  1;
}

/*******************************************************************************
 * FunctionName : RestoreParameter
 * Description  : 还原默认设置参数
*******************************************************************************/
u8 RestoreParameter(void)
{
    u16 i, j;

    Menu.flag = 1;
    Item.flag = 0;
    Menu.index = 0;
    Item.index = 0;
    for(i = 0; i < MENU_NUM; i++) gItemIndexNum[i] = gItemIndexNumBackup[i]; // 当前项目选项
    for(i = 0; i < ITEM_NUM; i++) gInSubIndexNum[IN_TYPE][i] = gInSubIndexNumBackup[IN_TYPE][i];     // 当前子项目选项
    for(i = 0; i < ITEM_NUM; i++) gOutSubIndexNum[OUT_TYPE][i] = gOutSubIndexNumBackup[OUT_TYPE][i]; // 当前子项目选项
    for(i = 0; i < PARAM_NUM; i++) gItemParam[i] = gItemParamBackup[i];      // 全部项目参数
    for(i = 0; i < ITEM_NUM; i++)
    {
        for(j = 0; j < SUB_NUM; j++) gInputSubParam[i][j] = gInputSubParamBackup[i][j];
        for(j = 0; j < SUB_NUM; j++) gOutSubParam[i][j] = gOutSubParamBackup[i][j];
    }

    return  0; //ok
}
/*********************************  END OF FILE  ******************************/
