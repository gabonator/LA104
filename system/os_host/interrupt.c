#include <stdint.h>
#include "source/usb/Disk.h"
#include "lib/STM32_USB-FS-Device_Driver/inc/usb_core.h"
#include "lib/STM32_USB-FS-Device_Driver/inc/usb_init.h"
#include "USB_istr.h"
#include "system_stm32f10x.h"
#include <stdbool.h>

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

void dbgPrint(const char* msg, ...);


volatile uint32_t gCounter = 0;
volatile uint32_t Dly_mS = 0;
volatile uint32_t gBeepCounter = 0;

//void USB_LP_CAN1_RX0_IRQHandler(void)
void USB_LP_CAN_RX0_IRQHandler(void)
{
    USB_Istr();
}

ui32  __Bios(ui8 Item, ui32 Var);

void DiskConfig(void)
{
    USB_Init();
    Disk_Init();
//    Init_Fat_Value();
}
#define BITMAP          0xFFFC0000

void Hardware_Init(void)
{

    __Bios(PWRCTRL, INIT);        // 
    __Bios(KEYnDEV, INIT);        // 
    __Bios(NIVCPTR, 0x8000);      // 
    SysTick_Config(SystemCoreClock / 1000);
    __Bios(BUZZDEV, INIT);        // 
    __Bios(BUZZDEV, 50);
    //Beep_mS(200);
    __Bios(FLSHDEV, INIT);        // SPI
    __Bios(USBDEV, INIT);         // USB
    DiskConfig();                 // 
    __Bios(IN_PORT, INIT);        // DAC
}


volatile char lastChar = 0;
char GetLastChar()
{
  char t = lastChar;
  lastChar = 0;
  return t;
}

void Beep(bool);

void SysTickHandler(void)
{
  gCounter++;
  if (Dly_mS)
    Dly_mS--;

  if (gBeepCounter)
  {
    if (!--gBeepCounter)
      Beep(false);
  }

  // keyboard
  uint16_t keysReg = ~__Bios(KEYnDEV, BITMAP);

  // yYxX 1111  1111 DCBA 
  uint8_t encX = (keysReg >> 12) & 3;
  uint8_t encY = (keysReg >> 14) & 3;
  uint8_t keys = keysReg & 0x0f;

  static uint8_t encXold = -1, encYold = -1, keysOld = -1;
  if (encXold == -1)
  {
    encXold = encX;
    encYold = encY;
    keysOld = keys;
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
      lastChar = '+'; 
    else if (diff<0)
      lastChar = '-'; 
    encXold = encX;
  }

  if (encY != encYold)
  {
    int8_t diff = encTable[(encYold << 2) | encY];
    if (diff>0)
      lastChar = '>'; 
    else if (diff<0)
      lastChar = '<'; 
    encYold = encY;
  }

  if (keysOld != keys)
  {
    if (keys & 0b0001)
      lastChar = '1';
    if (keys & 0b0010)
      lastChar = '2';
    if (keys & 0b0100)
      lastChar = '3';
    if (keys & 0b1000)
      lastChar = '4';
    keysOld = keys;
  }
}

extern void UartPushByte(uint8_t data);

void USART3_IRQHandler(void)
{
  if ((USART3->SR & USART_FLAG_RXNE) != (u16)RESET)
  {
    UartPushByte(USART_ReceiveData(USART3));
  }
}

