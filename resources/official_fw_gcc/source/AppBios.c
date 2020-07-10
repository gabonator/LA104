/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Bios.c
 * Description : 底层硬件配置
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include "MyDefine.h"
#include "usb_lib.h"

MENU_Struct Menu;
ITEM_Struct Item;
SUBITEM_Struct SubItem;

u8   gSamplBuf[CNTx4K*4096];
u8   gLCD_Backup[LCD_BUF_WIDTH];
u8   gLCD_Buf[LCD_BUF_WIDTH];

u8   gBeepFlag = 0;
u8   gStandByFlag = 0;
u16  gSysCnt = 0;
u16  gStandbyCnt = 0;
u16  gAutoPwrCnt = 0;
u16  gFullmS = 0;
u16  gTimeBase = 0;
u16  gBatVol = 4200, gUSB_Vol;

u8  *LcdCmd  = (u8 *)&gLCD_Buf[0];
u8  *LcdInfo = (u8 *)&gLCD_Buf[0];
u8  *LcdByte = (u8 *)&gLCD_Buf[1];
u16 *LcdData = (u16 *)&gLCD_Buf[1];

u8   Buffer[10];
u8  *SpiCmd  = (u8 *)&Buffer[0];
u8  *SpiInfo = (u8 *)&Buffer[0];
u8  *SpiByte = (u8 *)&Buffer[1];
u16 *SpiData = (u16 *)&Buffer[1];
u32 *SpiRecord = (u32 *)&Buffer[1];


/*******************************************************************************
 * FunctionName : FpgaRW
 * Description  : 
 * Param        : u8 Cmd 
 * Param        : u16 n 
 * Param        : u8* pBuf 
*******************************************************************************/
void FpgaRW(u8 Cmd, u16 n, u8 *pBuf)
{
    SPI_ParamDef  Param;

    pBuf[0] = Cmd;
    Param.n    = n;
    Param.pBuf = pBuf;
    __Bios(FPGADEV, (u32)&Param);
}
/*******************************************************************************
 * FunctionName : ProgmIO_SPIWr
 * Description  : 
 * Description  : 可编程接口SPI协议输出数据
 * Param        : u16 n 
 * Param        : u8* pBuf 
*******************************************************************************/
void ProgmIO_SPIWr(u16 n, u8 *pBuf)
{
    SPI_ParamDef  Param;

    Param.n    = n;
    Param.pBuf = pBuf;
    __Bios(EXT_SPI, (u32)&Param);
}
/*******************************************************************************
 * FunctionName : ProgmIO_UARTTx
 * Description  : 可编程接口串口协议输出数据
 * Param        : u8 Data  送一个字节数据，波特率115200
*******************************************************************************/
void ProgmIO_UARTTx(u8 Data)
{
    __Bios(EXT_UART, (u32)Data);
}
/*******************************************************************************
 * FunctionName : ProgmIO_I2CWr
 * Description  : 可编程接口I2C协议输出数据
 * Param        : u8 DevID 设备ID
 * Param        : u8 Reg   目标寄存器
 * Param        : u8 Data  数据
*******************************************************************************/
void ProgmIO_I2CWr(u8 DevID, u8 Reg, u8 Data)
{
    __Bios(EXT_I2C, (DevID<<16)|(Reg<<8)|(Data));
}
/*******************************************************************************
 * FunctionName : FPGA_ReadWrite
 * Description  : 读取8位状态和8位数据
 * Param        : u8 Num 
 * Param        : u16 Cnt 
 * Param        : u8 Cmd 
 * Param        : u16 Data 
 * Return       : u16 返回的第一个字节在高8位
*******************************************************************************/
u16 FPGA_ReadWrite(u8 Cmd, u16 Cnt, u16 Data)
{
    SPI_ParamDef  Param;

    u16 Temp = 0;

    Param.pBuf = SpiCmd;
    Param.n = Cnt;
    *SpiCmd = Cmd;
    *SpiData = Data;
    __Bios(FPGADEV, (u32)&Param);
    Temp |= (*SpiInfo) << 8;
    Temp |= *SpiByte;

    return Temp;
}
/*******************************************************************************
 * FunctionName : FPGA_LCD_RW
 * Description  : 
 * Param        : u8 Cmd 
 * Param        : u16 Cnt 
 * Param        : u16 Data 
 * Return       : u8   
*******************************************************************************/
u8 FPGA_LCD_RW(u8 Cmd, u16 Cnt, u8 *Buf)
{
    SPI_ParamDef  LCD_Param;

    LCD_Param.pBuf = Buf;
    Buf[0] = Cmd;
    LCD_Param.n = Cnt;
    __Bios(FPGADEV, (u32)&LCD_Param);

    return Buf[0];
}
/*******************************************************************************
 * FunctionName : FPGA_RW_4Byte
 * Description  : 
 * Param        : u8 Cmd 
 * Param        : u16 Cnt 
 * Param        : u32 Data 
 * Return       : u32 [0]:Info ,[1]:Tcnt[7:0],[2]:Tcnt[13:8],[3]:Di  
*******************************************************************************/
u32 FPGA_RW_4Byte(u8 Cmd, u16 Cnt, u32 Data)
{
    SPI_ParamDef  Param;
    u32 Temp = 0;

    Param.pBuf = SpiCmd;
    Param.n = Cnt;
    *SpiCmd = Cmd;
    *SpiRecord = Data;

    __Bios(FPGADEV, (u32)&Param);

    Temp  = *SpiInfo;
    Temp |= (*SpiRecord << 8);

    return Temp;
}
/*******************************************************************************
 * FunctionName : FPGA_Read
 * Description  : 读取16位数据
 * Param        : u8 Num 
 * Param        : u16 Cnt 
 * Param        : u8 Cmd 
 * Param        : u16 Data 
 * Return       : u16 
*******************************************************************************/
u16 FPGA_Read(u8 Cmd, u16 Cnt, u16 Data)
{
    SPI_ParamDef  Param;
    u16 Temp = 0;

    Param.pBuf = SpiCmd;
    Param.n = Cnt;
    *SpiCmd = Cmd;
    *SpiData = Data;

    __Bios(FPGADEV, (u32)&Param);
    Temp = *SpiData;

    return Temp;
}
/*******************************************************************************
 * FunctionName : Delay_uS
 * Description  : 在 73M Hz时 100uS~1000uS 的 Delay_uS 定时误差 < 1%
 * Param        : vu32 uS 
*******************************************************************************/
void Delay_uS(vu32 uS)
{
    uS = 63 * uS / 8;
    while(uS--)
    {}
}
/*******************************************************************************
 * FunctionName : Delay_mS
 * Description  : 在72M Hz时 10mS~100mS 的 Delay_mS 定时误差约为 +0.4%
 * Param        : u32 mS 
 * Return       : u8   
*******************************************************************************/
void Delay_mS(vu32 mS)
{
    Dly_mS = mS;
    while(Dly_mS)
    {};
}
/*******************************************************************************
 * FunctionName : DiskConfig
 * Description  : 磁盘设置
 * Param        : void 
*******************************************************************************/
void NVIC_Conifg(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
 * FunctionName : DiskConfig
 * Description  : U盘初始化
 * Param        : void 
*******************************************************************************/
void DiskConfig(void)
{
    USB_Init();
    Disk_Init();
    Init_Fat_Value();
}
/*******************************************************************************
 * FunctionName : ExtFlash_CS_LOW
 * Description  : U盘SPI选片控制
 * Param        : void 
*******************************************************************************/
void ExtFlash_CS_LOW(void)
{
    __Bios(FLSHDEV, DSBL);
//  GPIO_ResetBits(DISK_nSS1_PORT, DISK_nSS1_PIN);
}
/*******************************************************************************
 * FunctionName : ExtFlash_CS_HIGH
 * Description  : U盘SPI选片控制
 * Param        : void 
*******************************************************************************/
void ExtFlash_CS_HIGH(void)
{
    __Bios(FLSHDEV, ENBL);
//  GPIO_SetBits(DISK_nSS1_PORT,   DISK_nSS1_PIN);
}

/*********************************  END OF FILE  ******************************/
