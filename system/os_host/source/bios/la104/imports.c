#include "Ext_Flash.c"
#include <stdint.h>
#include <stdbool.h>
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "lcd.h"

#ifdef DISABLE_USB
void __USB_Istr(void) {}
void __CTR_HP(void) {}
#endif

extern void (* g_pfnVectors[76])(void);
extern uint32_t gGpioStatusCode;
extern uint32_t gGpioI2cSpeed;

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

enum
{
    PRDT_SN, //    8c33b088
    PRODUCT, //    LA104
    PRDTNUM, //    0104
    HDW_VER, //    1.5B
    MCU_TYP, //    STM32F103VC
    DFU_VER, //    V3.61D
    OEM_NUM, //    X
    MANUFAC, //    e-Design
    LICENCE, //    1
};

#define VIN_ST          0xFFFC0000
#define VBTYmV          0xFFFB0000

#define INIT            0xFFFF0000
#define ENBL 1
#define DSBL            0

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

void xBeep(bool b)
{
  if (b)
  {
    __Bios(BUZZDEV, ENBL);
    __Bios(BUZZDEV, 50);
  } else
  {
    __Bios(BUZZDEV, DSBL);
  }
}

void EnableUsb(bool enable)
{
  if (enable)
  {
    __Bios(USBDEV, INIT);
  } else
  {
    __Bios(USBDEV, DSBL);
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
  uint32_t keysReg =  ~__Bios(KEYnDEV, BITMAP);
  uint32_t keysMask = 0;

  // yYxX 1111  1111 DCBA 
  uint8_t encX = (keysReg >> 12) & 3;
  uint8_t encY = (keysReg >> 14) & 3;
  uint8_t keys = keysReg & 0x0f;

  static uint8_t encXold = -1, encYold = -1;
  if (encXold == -1)
  {
    encXold = encX;
    encYold = encY;
  }

  static int8_t encTable[16] = {
//     [0b0001] = 1, // 11 -> 10
     [0b0111] = 1, // 10 -> 00
     [0b1100] = 1, // 00 -> 11
     [0b0011] = -1, // 10 -> 11
//     [0b1101] = -1, // 00 -> 10
     [0b0100] = -1, // 11 -> 00
  };

  if (encX != encXold)
  {
    int8_t diff = encTable[(encXold << 2) | encX];
    if (diff>0)
      keysMask |= KeyRight;
    else if (diff<0)
      keysMask |= KeyLeft;
    encXold = encX;
  }

  if (encY != encYold)
  {
    int8_t diff = encTable[(encYold << 2) | encY];
    if (diff>0)
      keysMask |= KeyUp;
    else if (diff<0)
      keysMask |= KeyDown;
    encYold = encY;
  }

  keysMask |= keys & 0b1111; // F1..F4
  return keysMask;
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

uintptr_t GetAttribute(enum EAttribute attribute)
{
  switch (attribute)
  {
    case DeviceType: return (uintptr_t)"LA104";
    case VersionDfu: return (uintptr_t)(char*)__Bios(SYSINFO, DFU_VER);
    case VersionHardware: return (uintptr_t)(char*)__Bios(SYSINFO, HDW_VER);
    case VersionSystem: return (uintptr_t)(char*)__Bios(SYSINFO, PRODUCT);
    case VersionFpga: return (uintptr_t)(char*)0;
    case SerialNumber: return (uintptr_t)(uint32_t)__Bios(SYSINFO, PRDT_SN);
    case LicenseNumber: return (uintptr_t)0;
    case LicenseValid: return (uintptr_t)(uint32_t)__Bios(SYSINFO, LICENCE);
    case DisplayType: return (uintptr_t)(char*)"ili9341";
    case DiskType: return (uintptr_t)0;
    case BatteryVoltage: return (uintptr_t)__Bios(PWRCTRL, VBTYmV); 
    // voltage in millivolts
    // 2954 - empty, device unable to start, 3956 - fully charged
    // 4000..4680 - charging
    case Charging: return (uintptr_t)__Bios(PWRCTRL, VIN_ST); 
    case GpioStatus: return (uintptr_t)&gGpioStatusCode;
    case GpioI2cSpeed: return (uintptr_t)&gGpioI2cSpeed;
    case FlashReadRange: return (uintptr_t)gFlashReadRange;
    case FlashWriteRange: return (uintptr_t)gFlashWriteRange;
    case FlashAlertRange: return (uintptr_t)gFlashAlertRange;
    default: return 0;
  }
}
