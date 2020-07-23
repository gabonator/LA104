/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Bios.c
 * Description : 
 * Author      : SNAKE 
 * History     :
*******************************************************************************/
#include "MyDefine.h"
//#include "USB_lib.h"


// stubs!
u8  Save_Bmp(u16 FileNum){ return 0;}
u8  Save_Buf(u16 FileNo){ return 0;}
u8  Save_Csv(s16 FileNo){ return 0;}
u8  ReadParameter(void){ return 0;}
u8  SaveParameter(void){ return 0;}
u8  RestoreParameter(void){ return 0;}
u16 Load_File_Num(u8 Tpye){ return 0;}

void File_Num(void){}
void PrintClk(u16 x0, u16 y0, u8 Phase){}
void Clear_File_ICO(u16 x, u16 y){}
void DispFileInfo(u8 Info){}

void DiskConfig(void){}





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

__attribute__ ((section(".biosfunc"), optimize("O0")))
uint32_t __Bios(uint8_t Item, uint32_t Var) { return 0; }


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
 * Description  : 
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
 * Description  : 
 * Param        : u8 Data  
*******************************************************************************/
void ProgmIO_UARTTx(u8 Data)
{
    __Bios(EXT_UART, (u32)Data);
}
/*******************************************************************************
 * FunctionName : ProgmIO_I2CWr
 * Description  : 
 * Param        : u8 DevID 
 * Param        : u8 Reg   Ŀ
 * Param        : u8 Data  
*******************************************************************************/
void ProgmIO_I2CWr(u8 DevID, u8 Reg, u8 Data)
{
    __Bios(EXT_I2C, (DevID<<16)|(Reg<<8)|(Data));
}
/*******************************************************************************
 * FunctionName : FPGA_ReadWrite
 * Description  : 
 * Param        : u8 Num 
 * Param        : u16 Cnt 
 * Param        : u8 Cmd 
 * Param        : u16 Data 
 * Return       : u16 
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
 * Description  : 
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
 * Description  : 
 * Param        : vu32 uS 
*******************************************************************************/
void Delay_uS(vu32 uS)
{
    uS = 63 * uS / 8;
    while(uS--)
    {}
}
