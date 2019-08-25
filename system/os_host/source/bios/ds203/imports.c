#include <stdint.h>
#include <stdbool.h>
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "ds203bios.h"
//#include "BIOS.h"
//#include "lowlcd.h"

void dbgPrint(const char* msg, ...);

void Delay_mS(uint32_t mS);


extern void (* g_pfnVectors[76])(void);

void ___Set(int x, int y)
{
  __Set(x, y);
}             

void ___Int_HP()
{
  __CTR_HP();
}

void ___Int_LP()
{
  __USB_Istr();
}
    
void Set_Pixel(uint_fast16_t Color)
{
  __LCD_SetPixl(Color);
}

uint32_t ReadPixel(void)
{
  return __LCD_GetPixl();
}

void ReadStart(void)
{
}

void ReadFinish(void)
{
}

uint16_t Get_Pixel()
{
  return __LCD_GetPixl();
}

void ExtFlash_CS_LOW(void)
{
}

void ExtFlash_CS_HIGH(void)
{
}

void Set_Block(int x1, int y1, int x2, int y2)
{
  __LCD_Set_Block(x1, x2-1, y1, y2-1);
}

void xBeep(bool b)
{
  __Set(BEEP_VOLUME, b ? 100 : 0);
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
  __Point_SCR(x, y);
}

void EnableUsb(bool enable)
{
}

void HardwareInit()
{

//  LCD_Init();
//  __LCD_Initial();

  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t)g_pfnVectors - NVIC_VectTab_FLASH);
  __USB_Init();
//  SysTick_Config(SystemCoreClock / 1000);

}

uint32_t GetKeys()
{
  uint32_t status = ~__Get(KEY_STATUS, 0);
  uint32_t out = 0;

  if (status & K_INDEX_D_STATUS ) out |= KeyLeft;
  if (status & K_INDEX_I_STATUS ) out |= KeyRight;
//  if (status & K_INDEX_S_STATUS ) out |= EKeyMask::KeyF5;
  if (status & K_ITEM_D_STATUS ) out |= KeyUp;
  if (status & K_ITEM_I_STATUS ) out |= KeyDown;
//  if (status & K_ITEM_S_STATUS ) out |= EKeyMask::KeyF6;
  if (status & KEY1_STATUS ) out |= KeyF1;
  if (status & KEY2_STATUS ) out |= KeyF2;
  if (status & KEY3_STATUS ) out |= KeyF3;
  if (status & KEY4_STATUS ) out |= KeyF4;

  return out;
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
  __ProgDiskPage(pBuffer, WriteAddr);
  return 1;
}

bool ExtFlashDataRd(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Length)
{
  __ReadDiskData(pBuffer, ReadAddr, Length);
  return 1;
}
