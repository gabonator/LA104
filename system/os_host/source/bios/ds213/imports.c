#include <stdint.h>
#include <stdbool.h>
#include "lib/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "DS213HwDriver.h"

#define _SEC_SIZE         4096 // TODO: JOIN

HwDrvDef  Hw;

void dbgPrint(const char* msg, ...);

void Delay_mS(uint32_t mS);


extern void (* g_pfnVectors[76])(void);
                 
void Set_Pixel(uint_fast16_t Color)
{
  *Hw.pLcd_Data_W = Color;
}

uint32_t ReadPixel(void)
{
  return *Hw.pLcd_Data_R;
}

int lastBlockX1, lastBlockY1, lastBlockX2, lastBlockY2;

void ReadStart(void)
{
  Hw.pLCD_R_Block(lastBlockX1, lastBlockY1, lastBlockX2, lastBlockY2);
}

void ReadFinish(void)
{
}

uint16_t Get_Pixel()
{
  return ReadPixel();
}

void ExtFlash_CS_LOW(void)
{
}

void ExtFlash_CS_HIGH(void)
{
}

void Set_Block(int x1, int y1, int x2, int y2)
{ 
  lastBlockX1 = x1;
  lastBlockY1 = y1;
  lastBlockX2 = x2-1;
  lastBlockY2 = y2-1;
  Hw.pLCD_W_Block(x1, y1, x2-1, y2-1);
// Hw.pLCD_R_Block(x1, y1, x2, y2);
}


void xBeep(bool b)
{
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
  lastBlockX1 = x;
  lastBlockY1 = y;
  lastBlockX2 = x;
  lastBlockY2 = y;
  Hw.pLCD_W_Block(x, y, Hw.MaxLcdCol, Hw.MaxLcdRow);
}

void EnableUsb(bool enable)
{
}

void HardwareInit()
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t)g_pfnVectors - NVIC_VectTab_FLASH);
  while (SysTick_Config(SystemCoreClock / 1000));

  pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
  __BiosInit((u32)&Hw);

  Hw.pDevInit(USBDISK);
  Hw.pDevInit(USB_SCI);
  Delay_mS(500);
  Hw.pDevInit(USB_DEV);


  dbgPrint("\n\n\n\n\nsect=%d,amnt=%d\n", Hw.DiskSecSize, Hw.DiskSecAmnt);

//BIOS::DBG::Print("\n\n\n\n\nsect=%d,amnt=%d\n", Hw.DiskSecSize, Hw.DiskSecAmnt);
  #if 0
  SectorSize  = Hw.DiskSecSize;
  SectorAmnt  = Hw.DiskSecAmnt;
  V32         = (u32*)&DiskBuf[SectorSize+0x04];
  Var         = (u8*) &DiskBuf[SectorSize+0x24];
  Data        = (u8*) &DiskBuf[SectorSize+0x24]+SEG_DATA;
  V32[WR_CNT] = 0;
  V32[RD_CNT] = 0;
}

/*******************************************************************************
  Description  : USB Disk sector write  
*******************************************************************************/
void ProgDiskSect(u8 *pBuf, u32 Addr)
{
  Hw.pSpiFlashSecW((u32)&pBuf[0], Addr, SectorSize);
}

/*******************************************************************************
  Description  : Virtual Disk sector read   
*******************************************************************************/
void ReadDiskData(u8 *pBuf, u32 Addr, u16 n)
{
  Hw.pSpiFlashSecW((u32)&pBuf[0], Addr, SectorSize);
  Hw.pSpiFlashRead((u32)&pBuf[0], Addr, n);
}

#endif

}

uint32_t GetKeys()
{
  uint32_t keys = 0;
  keys |= !*Hw.pK1_St;
  keys |= (!*Hw.pK2_St)<<1;
  keys |= (!*Hw.pK3_St)<<2;
  keys |= (!*Hw.pK4_St)<<3;

  static uint32_t LastEnc 	= 0;

  uint32_t EncdInp = ((*Hw.pEa_St) << 12)|((*Hw.pEb_St) << 13)|
                ((*Hw.pEc_St) << 14)|((*Hw.pEd_St) << 15);
  #define ENC1a           0x1000
  #define ENC1b           0x2000
  #define ENC2a           0x4000
  #define ENC2b           0x8000

  uint32_t EncActP = EncdInp & ~LastEnc;
  uint32_t EncActN = ~EncdInp & LastEnc;
  LastEnc = EncdInp;

  #define ENCD_1p (1<<8)
  #define ENCD_1n (1<<9)
  #define ENCD_2p (1<<10)
  #define ENCD_2n (1<<11)

    if(EncActN & ENC1a) keys |= (EncdInp & ENC1b) ? ENCD_1p : ENCD_1n;
    if(EncActN & ENC2a) keys |= (EncdInp & ENC2b) ? ENCD_2n : ENCD_2p;
    if(EncActP & ENC1b) keys |= (EncdInp & ENC1a) ? ENCD_1p : ENCD_1n;
    if(EncActP & ENC2b) keys |= (EncdInp & ENC2a) ? ENCD_2n : ENCD_2p;

  return keys;
}

// FPGA
uint32_t FPGA32(uint8_t Cmd, uint16_t Cnt, uint32_t Data)
{
  return 0;
}

uint16_t FPGA16(uint8_t Cmd, uint16_t Cnt, uint16_t Data)
{
  return 0;
}

void USB_DevInit(void)
{
  GPIO_InitTypeDef  GPIO_Struct;
  NVIC_InitTypeDef  NVIC_Struct;

  RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
  GPIO_Struct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_11;
  GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_Struct);
  RCC->APB1ENR |= RCC_APB1Periph_USB;
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_Struct.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;
  NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Struct.NVIC_IRQChannelSubPriority        = 0;
  NVIC_Struct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_Struct);
}


bool ExtFlashSecWr(uint8_t* pBuffer, uint32_t WriteAddr)
{
  // TODO: Interruption not handled properly!
  Hw.pSpiFlashSecW((uint32_t)pBuffer, WriteAddr, _SEC_SIZE);
  return 1;
}

bool ExtFlashDataRd(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Length)
{
  // TODO: Interruption not handled properly!
  Hw.pSpiFlashRead((uint32_t)pBuffer, ReadAddr, _SEC_SIZE);
  return 1;
}
