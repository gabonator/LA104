#include <stdint.h>
#include <stdbool.h>
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "BIOS.h"

void Delay_mS(uint32_t mS);

extern void (* g_pfnVectors[76])(void);

#define GPIOD_BRR   (*((volatile uint32_t *)(0x40011400+0x14)))
#define GPIOD_BSRR  (*((volatile uint32_t *)(0x40011400+0x10)))
#define LCD_nRST_LOW()    GPIOD_BRR  = GPIO_Pin_11
#define LCD_nRST_HIGH()   GPIOD_BSRR = GPIO_Pin_11
#define LCD_RS_LOW()      GPIOD_BRR  = GPIO_Pin_12
#define LCD_RS_HIGH()     GPIOD_BSRR = GPIO_Pin_12
#define LCD_PORT *((volatile uint16_t*)0x60000000)

void LCD_WR_Ctrl(int Reg) 
{
  LCD_RS_LOW();
  LCD_PORT = Reg;      
  LCD_RS_HIGH();
}

void Set_Pixel(uint_fast16_t Color)
{
  LCD_PORT = Color;
//  __LCD_SetPixl(Color);
}

void ReadStart(void)
{
}

void ReadFinish(void)
{
}

uint32_t ReadPixel()
{
    uint16_t Data;
    LCD_WR_Ctrl(0x2E);
    Data  = LCD_PORT; 
    Data  = LCD_PORT; 
    LCD_WR_Ctrl(0x2C);
    return Data;
}

void Set_Block(int x1, int y1, int x2, int y2)
{
    x2--; y2--;
    LCD_WR_Ctrl(0x2A);      // Block End X Address
    LCD_PORT = (y1 >> 8); 
    LCD_PORT = (y1 & 0xFF); 
    LCD_PORT = (y2 >> 8); 
    LCD_PORT = (y2 & 0xFF); 
    LCD_WR_Ctrl(0x2B);      // Block End Y Address
    LCD_PORT = (x1 >> 8); 
    LCD_PORT = (x1 & 0xFF); 
    LCD_PORT = (x2 >> 8); 
    LCD_PORT = (x2 & 0xFF); 
    LCD_WR_Ctrl(0x2C); 
}

void xBeep(bool b)
{
  #define TIM8_CCR2   (*((vu32 *)(0x40013400+0x38)))
  TIM8_CCR2 = 100 - (b ? 100 : 0)/2;
//  __Set(BEEP_VOLUME, b ? 100 : 0);
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
    LCD_WR_Ctrl(0x2A);      // Block End X Address
    LCD_PORT = (0x00); 
    LCD_PORT = (y & 0xFF); 
    LCD_PORT = (0x00); 
    LCD_PORT = (0xEF); 
    LCD_WR_Ctrl(0x2B);      // Block End Y Address
    LCD_PORT = (x >> 8); 
    LCD_PORT = (x & 0xFF); 
    LCD_PORT = (399 >> 8); 
    LCD_PORT = (399 & 0xFF); 
    LCD_WR_Ctrl(0x2C); 

//  __Point_SCR(x, y);
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
  if (status & K_ITEM_D_STATUS ) out |= KeyDown;
  if (status & K_ITEM_I_STATUS ) out |= KeyUp;
//  if (status & K_ITEM_S_STATUS ) out |= EKeyMask::KeyF6;
  if (status & KEY1_STATUS ) out |= KeyF1;
  if (status & KEY2_STATUS ) out |= KeyF2;
  if (status & KEY3_STATUS ) out |= KeyF3;
  if (status & KEY4_STATUS ) out |= KeyF4;

  return out;
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

uintptr_t GetAttribute(enum EAttribute attribute)
{
  switch (attribute)
  {
    case DeviceType: return (uintptr_t)"DS203";
    case VersionDfu: return (uintptr_t)(char*)__Get(DFUVER, 0);
    case VersionHardware: return (uintptr_t)(char*)__Get(HDWVER, 0);
    case VersionSystem: return (uintptr_t)(char*)__Get(SYSVER, 0);
    case VersionFpga: return (uintptr_t)(char*)__Get(FPGAVER, 0);
    case SerialNumber: return (uintptr_t)(uint32_t)__GetDev_SN();
    case LicenseNumber: return (uintptr_t)0;
    case LicenseValid: return (uintptr_t)0;
    case DisplayType: return (uintptr_t)0;
    case DiskType: return (uintptr_t)0;
    case BatteryVoltage: return (uintptr_t)__Get(V_BATTERY, 0);
    case Charging: return (uintptr_t)__Get(USB_POWER, 0);
    case FlashReadRange: return (uintptr_t)0; // Uses native usb interface
    case FlashWriteRange: return (uintptr_t)0;
    case FlashAlertRange: return (uintptr_t)0;
    default: return 0;
  }
}
