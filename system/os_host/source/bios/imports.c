//#include "imports.h"
#include <stdint.h>
#include <stdbool.h>
#include "lib/STM32F10x_StdPeriph_Driver/inc/misc.h"

extern void (* g_pfnVectors[76])(void);

__attribute__ ((section(".biosfunc"), optimize("O0")))
uint32_t __Bios(uint8_t Item, uint32_t Var) { return 0; }

enum
{
    SYSINFO, // ????????      Var: PRDT_SN  ???????  Rtn: (u16)???????
             //                   Var: PRODUCT  ??????  Rtn: ?????????
             //                   Var: PRDTNUM  ???????  Rtn: (u16)???????
             //                   Var: SCH_VER  ?????  Rtn: ????????
             //                   Var: MCU_TYP  MCU ???  Rtn: ?????????
             //                   Var: DFU_VER  DFU ??  Rtn: ????????
             //                   Var: OEM_NUM  OEM ????  Rtn: ??????????
             //                   Var: MANUFAC ???????? Rtn: ?????????
             //                   Var: LICENCE ??????? Rtn: SUCC/FAIL
             // ????????
    NIVCPTR, // ?????????????  Var: (u16)   ??????  Rtn: SUCC/FAIL
    SYSTICK, // ?????????????  Var: (u16)uS ??????  Rtn: SUCC/FAIL
    AF_RMAP, // ?? IO ???
    PWRCTRL, // ???????          Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: VIN_ST  ??????  Rtn: ENBL/DSBL
             //                   Var: VBAT    ??????  Rtn: (u16)mV
             //                   Var: DSBL    ??????  Rtn: SUCC/FAIL
    BUZZDEV, // ?????????        Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: (MUTE~100)  ????  Rtn: SUCC/FAIL
    KEYnDEV, // ????????????    Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: BITMAP  ??????  Rtn: (u16)?????
    DELAYuS, // ??????          Var: (u32)uS ??????  Rtn: SUCC/FAIL
             // ?????????
    DISPDEV, // LCD ?????      Var: STRING  g?????  Rtn: ?????????
             //                   Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: VALUE   ??L????  Rtn: (MaxX<<16+MaxY)
             //                   Var: (0~100) ????????  Rtn: SUCC/FAIL
             //                   Var: (MCU/FPGA)?????  Rtn: SUCC/FAIL
    BLOCK_X, // ????? x ??????   Var: (x1 << 16)+x2)    Rtn: SUCC/FAIL
    BLOCK_Y, // ????? y ??????   Var: (y1 << 16)+y2)    Rtn: SUCC/FAIL
    PIXEL_X, // ???? x ???       Var: (u16)???         Rtn: SUCC/FAIL
    PIXEL_Y, // ???? y ???       Var: (u16)???         Rtn: SUCC/FAIL
    WrPIXEL, // ????????        Var: (u16)???????     Rtn: SUCC/FAIL
    RdPIXEL, // ?????L????      Var:                   Rtn: (u16)???????
    FONTPTR, //                   Var: ASCII Code        Rtn: ptr ??????
             // FPGA ??????
    FPGADEV, // FPGA ??????     Var: STRING            Rtn: ?????????
             //                   Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: CNFG    ???????  Rtn: SUCC/FAIL
             //                   Var: COMM    ???g?  Rtn: SUCC/FAIL
             //                   Var: ENBL    ?????  Rtn: SUCC/FAIL
             //                   Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
             // ??????????????
    IN_PORT, // ???????????      Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: VREF ????????? Rtn: SUCC/FAIL
             // U ???????
    USBDEV,  // USB ??'??
    U_DISK,  // USB DISK ??     Var: INIT  ??'??????  Rtn: SUCC/FAIL
             //                   Var: STRING  U ?????  Rtn: U ???????????
             //                   Var: PAGE  ??????  Rtn: SUCC/FAIL
             //                   Var: SECTOR  ????????  Rtn: (u16)????????
             //                   Var: AMOUNT  ????????  Rtn: (u16)????????
             //                   Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
             // SPI ??????
    SPI_DEV, // SPI ?????      Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
    FLSHDEV, // Flash SPI
             // ??????????????
    EXT_INP, // ??? PIO ???     Var: PIOCFG+PinDef      Rtn: SUCC
    EXT_OUT, // ??? PIO ???     Var: PIOCFG+PinDef      Rtn: SUCC
             //                   Var: PINS_OUT+Status0~3 Rtn: SUCC
             //                   Var: PINS_IN+BitMask0~3 Rtn: PinStatus 0~3
    EXT_PWM, // ??? PWM ???     Var: PWM_PSC+Var[0~15]  Rtn: SUCC
             //                   Var: PWM_ARR+Var[0~15]  Rtn: SUCC
             //                   Var: CH1_CCR+Var[0~15]  Rtn: SUCC
             //                   Var: CH2_CCR+Var[0~15]  Rtn: SUCC
             //                   Var: CH1_CTRL+ENBL/DSBL Rtn: SUCC
             //                   Var: CH2_CTRL+ENBL/DSBL Rtn: SUCC
    EXT_INF, // ??? PIO ???     Var: INIT  ??'?????? ?????????????
    EXT_SPI, // ??? PIO ???     Var: INIT  ??'??????   Rtn: SUCC/FAIL
             //                   Var: (SPI_ParamDef*)    Rtn: SUCC/FAIL
    EXT_UART,// ??? PIO ???     Var: INIT  ??'??????   Rtn: SUCC/FAIL
             //                   Var: u8 Data            Rtn: SUCC/FAIL
    EXT_I2C, // ??? PIO ???
             //                   Var: 0x00&ID&REG&DATA   Rtn: SUCC/FAIL
    EXT_RXD, // ??? PIO ???     Var: PIOCFG+PinDef      Rtn: SUCC
    EXT_TXD, // ??? PIO ???     Var: PIOCFG+PinDef      Rtn: SUCC
};

#define INIT            0xFFFF0000
#define ENBL 1
#define DSBL            0

//https://gist.github.com/iwalpola/6c36c9573fd322a268ce890a118571ca

void Set_Pixel(uint_fast16_t Color)
{
  GPIOE->ODR = (u16)Color; // LCD_PORT - GPIOE->ODR
  GPIOD->BRR = GPIO_Pin_9; // Wr - D9 
  GPIOD->BSRR = GPIO_Pin_9; // Wr - D9
}

uint16_t Get_Pixel()
{
  // TODO:
    return __Bios(RdPIXEL, 0);
}

void ExtFlash_CS_LOW(void)
{
    __Bios(FLSHDEV, DSBL);
//  GPIO_ResetBits(DISK_nSS1_PORT, DISK_nSS1_PIN);
}

void ExtFlash_CS_HIGH(void)
{
    __Bios(FLSHDEV, ENBL);
//  GPIO_SetBits(DISK_nSS1_PORT,   DISK_nSS1_PIN);
}

void Set_Block(int x1, int y1, int x2, int y2)
{
  // TODO:
  __Bios(BLOCK_X, (x2-1) | (x1 << 16));
  __Bios(BLOCK_Y, (y2-1) | (y1 << 16));
}


void Beep(bool b)
{
  if (b)
  {
    __Bios(BUZZDEV, ENBL);
    __Bios(BUZZDEV, 100);
  } else
  {
    __Bios(BUZZDEV, DSBL);
  }
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
    // TODO:
    __Bios(PIXEL_X, x);
    __Bios(PIXEL_Y, y);
}

void EnableUsb(bool enable)
{
  if (enable)
  {
    __Bios(USBDEV, INIT);         // USB
  } else
  {
    // TODO
  }
}

void HardwareInit()
{
    __Bios(PWRCTRL, INIT);        // 
    __Bios(KEYnDEV, INIT);        // 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t)g_pfnVectors - NVIC_VectTab_FLASH);
//    __Bios(NIVCPTR, 0x8000);      // 
    SysTick_Config(SystemCoreClock / 1000);
    __Bios(BUZZDEV, INIT);        // 
    __Bios(BUZZDEV, 50);
    //Beep_mS(200);
    __Bios(FLSHDEV, INIT);        // SPI
    __Bios(IN_PORT, INIT);        // DAC
}

uint32_t GetKeys()
{
  const uint32_t KEYnDEV = 6;
  const uint32_t BITMAP = 0xFFFC0000;
  return ~__Bios(KEYnDEV, BITMAP);
}

// FPGA
typedef struct
{
    uint16_t  n;
    uint8_t  *pBuf;
} TFpgaRequest;

uint32_t FPGA32(uint8_t Cmd, uint16_t Cnt, uint32_t Data)
{
  uint8_t   Buffer[10];
  uint8_t  *SpiCmd  = (uint8_t *)&Buffer[0];
  uint8_t  *SpiInfo = (uint8_t *)&Buffer[0];
  uint32_t *SpiRecord = (uint32_t *)&Buffer[1]; // WTF? ALIASING?

  TFpgaRequest Param;
  uint32_t Temp = 0;

  Param.pBuf = SpiCmd;
  Param.n = Cnt;
  *SpiCmd = Cmd;
  *SpiRecord = Data;

  __Bios(FPGADEV, (uint32_t)&Param);

  Temp  = *SpiInfo;
  Temp |= *SpiRecord << 8;

  return Temp;
}

uint16_t FPGA16(uint8_t Cmd, uint16_t Cnt, uint16_t Data)
{
  uint8_t   Buffer[10];
  uint8_t  *SpiCmd  = (uint8_t *)&Buffer[0];
  uint8_t  *SpiInfo = (uint8_t *)&Buffer[0];
  uint8_t  *SpiByte = (uint8_t *)&Buffer[1];
  uint16_t *SpiData = (uint16_t *)&Buffer[1]; // WTF? ALIASING?

  TFpgaRequest Param;

  uint16_t Temp = 0;

  Param.pBuf = SpiCmd;
  Param.n = Cnt;
  *SpiCmd = Cmd;
  *SpiData = Data;
  __Bios(FPGADEV, (uint32_t)&Param);
  Temp |= (*SpiInfo) << 8;
  Temp |= *SpiByte;

  return Temp;
}
