#include <stdint.h>
#include <stdbool.h>
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "BIOS.h"

const uint32_t firmwareCrc = 0x6ab02021;

extern unsigned long _estack;
extern unsigned long _addressRamBegin;
extern unsigned long _addressRamEnd;
extern unsigned long _addressRomBegin;
extern unsigned long _addressRomEnd;
const uint32_t* _systemMemoryRanges[] = {&_addressRomBegin, &_addressRomEnd, 
  &_addressRamBegin, &_addressRamEnd, &_estack};

void Delay_mS(uint32_t mS);

extern void (* g_pfnVectors[76])(void);

#define GPIOD_BRR   (*((volatile uint32_t *)(0x40011400+0x14)))
#define GPIOD_BSRR  (*((volatile uint32_t *)(0x40011400+0x10)))
#define LCD_nRST_LOW()    GPIOD_BRR  = GPIO_Pin_11
#define LCD_nRST_HIGH()   GPIOD_BSRR = GPIO_Pin_11
#define LCD_RS_LOW()      GPIOD_BRR  = GPIO_Pin_12
#define LCD_RS_HIGH()     GPIOD_BSRR = GPIO_Pin_12
#define LCD_PORT *((volatile uint16_t*)0x60000000)

// Dummy flash read: forces a D-Code bus stall (2 wait-state fetch from flash)
// which delays the CPU long enough for the preceding GPIO write to reach
// the pin before the next FSMC transaction begins. DSB alone is insufficient
// because it doesn't stall the D-Code bus; mov.w immediates don't either.
#define LCD_BUS_STALL()   ((void)(*(volatile uint32_t*)0x08000000))

#define DMA2_CH1_CCR   (*((volatile uint32_t*)0x40020408))
#define DMA2_CH1_CNDTR (*((volatile uint32_t*)0x4002040C))
#define DMA2_CH1_CPAR  (*((volatile uint32_t*)0x40020410))
#define DMA2_CH1_CMAR  (*((volatile uint32_t*)0x40020414))

#define LCD_ID_ILI9327  0x02049327

#ifndef DS203_BIOS_LCD

static uint32_t lcd_id;

// Write command byte (RS LOW, write, RS HIGH)
void LCD_WR_Ctrl(int Reg)
{
  LCD_BUS_STALL();
  LCD_RS_LOW();
  LCD_PORT = Reg;
  LCD_BUS_STALL();
  LCD_RS_HIGH();
}

static void LCD_WR_Data(uint16_t Data)
{
  LCD_PORT = Data;
}

// ILI932x register write: command + data in one call
static void LCD_WR_Reg(uint16_t cmd, uint16_t data)
{
  LCD_WR_Ctrl(cmd);
  LCD_WR_Data(data);
}

static void LCD_DMA_Wait(void)
{
  DMA2_CH1_CCR |= 1;
  while (DMA2_CH1_CNDTR != 0);
}

static uint32_t LCD_ReadID(void)
{
  LCD_WR_Ctrl(0xEF);
  (void)LCD_PORT;  // dummy
  volatile uint16_t r0 = LCD_PORT;
  volatile uint16_t r1 = LCD_PORT;
  volatile uint16_t r2 = LCD_PORT;
  volatile uint16_t r3 = LCD_PORT;
  return (r0 << 24) | ((r1 & 0xFF) << 16) | ((r2 & 0xFF) << 8) | (r3 & 0xFF);
}

static void LCD_Init_ILI9327(void)
{
  LCD_WR_Ctrl(0xE9); LCD_WR_Data(0x20);
  LCD_WR_Ctrl(0x11); // Sleep Out
  Delay_mS(100);

  LCD_WR_Ctrl(0xD1); LCD_WR_Data(0x00); LCD_WR_Data(0x5E); LCD_WR_Data(0x17);
  LCD_WR_Ctrl(0xD0); LCD_WR_Data(0x07); LCD_WR_Data(0x07); LCD_WR_Data(0x0C);
  LCD_WR_Ctrl(0x36); LCD_WR_Data(0x40); // MADCTL: column address order reversed
  LCD_WR_Ctrl(0x0C); LCD_WR_Data(0x55);
  LCD_WR_Ctrl(0x3A); LCD_WR_Data(0x55); // 16-bit color

  LCD_WR_Ctrl(0xC1); LCD_WR_Data(0x10); LCD_WR_Data(0x10); LCD_WR_Data(0x02); LCD_WR_Data(0x02);
  LCD_WR_Ctrl(0xC0); LCD_WR_Data(0x00); LCD_WR_Data(0x35); LCD_WR_Data(0x00);
                       LCD_WR_Data(0x00); LCD_WR_Data(0x01); LCD_WR_Data(0x02);
  LCD_WR_Ctrl(0xC5); LCD_WR_Data(0x01);
  LCD_WR_Ctrl(0xD2); LCD_WR_Data(0x01); LCD_WR_Data(0x22);

  // Gamma
  LCD_WR_Ctrl(0xC8); LCD_WR_Data(0x01); LCD_WR_Data(0x73); LCD_WR_Data(0x37);
                       LCD_WR_Data(0x00); LCD_WR_Data(0x0F); LCD_WR_Data(0x02);
                       LCD_WR_Data(0x04); LCD_WR_Data(0x40); LCD_WR_Data(0x67);
                       LCD_WR_Data(0x00); LCD_WR_Data(0x02); LCD_WR_Data(0x0F);
                       LCD_WR_Data(0x08); LCD_WR_Data(0x80); LCD_WR_Data(0x00);

  LCD_WR_Ctrl(0x29); // Display ON
  LCD_WR_Ctrl(0x2C); // Memory Write
}

static void LCD_Init_ILI932x(void)
{
  LCD_WR_Reg(0x00, 0x00);    // Start oscillation
  LCD_WR_Data(0x00); LCD_WR_Data(0x00); LCD_WR_Data(0x00);
  Delay_mS(10);

  LCD_WR_Reg(0x600, 0x01);   // NL = 1
  Delay_mS(10);

  LCD_WR_Reg(0x6F0, 0x40);
  Delay_mS(10);

  LCD_WR_Reg(0x400, 0x6200);
  LCD_WR_Reg(0x08, 0x0808);
  LCD_WR_Reg(0x01, 0x0100);
  LCD_WR_Reg(0x02, 0x0100);
  LCD_WR_Reg(0x03, 0x1030);
  LCD_WR_Reg(0x09, 0x0001);
  LCD_WR_Reg(0x0C, 0x0000);
  LCD_WR_Reg(0x90, 0x8000);
  LCD_WR_Reg(0x0F, 0x0000);
  LCD_WR_Reg(0x10, 0x0016);
  LCD_WR_Reg(0x11, 0x0101);
  LCD_WR_Reg(0x12, 0x0000);
  LCD_WR_Reg(0x13, 0x0001);
  LCD_WR_Reg(0x100, 0x0330);
  LCD_WR_Reg(0x101, 0x0237);
  LCD_WR_Reg(0x103, 0x0F00);
  LCD_WR_Reg(0x280, 0x6B00);
  LCD_WR_Reg(0x102, 0xC1B0);
  Delay_mS(100);

  // Gamma
  LCD_WR_Reg(0x300, 0x0C00);
  LCD_WR_Reg(0x301, 0x5A0B);
  LCD_WR_Reg(0x302, 0x0906);
  LCD_WR_Reg(0x303, 0x1017);
  LCD_WR_Reg(0x304, 0x2300);
  LCD_WR_Reg(0x305, 0x1700);
  LCD_WR_Reg(0x306, 0x6309);
  LCD_WR_Reg(0x307, 0x0C09);
  LCD_WR_Reg(0x308, 0x100C);
  LCD_WR_Reg(0x309, 0x2232);

  // Window
  LCD_WR_Reg(0x210, 0x0000);
  LCD_WR_Reg(0x211, 0x00EF);
  LCD_WR_Reg(0x212, 0x0000);
  LCD_WR_Reg(0x213, 0x018F);
  LCD_WR_Reg(0x500, 0x0000);
  LCD_WR_Reg(0x501, 0x0000);
  LCD_WR_Reg(0x502, 0x0005);
  LCD_WR_Reg(0x401, 0x0001);
  LCD_WR_Reg(0x404, 0x0000);
  Delay_mS(100);

  LCD_WR_Reg(0x07, 0x0100);
  Delay_mS(100);

  LCD_WR_Reg(0x200, 0x0000);
  LCD_WR_Reg(0x201, 0x0000);

  // Start GRAM write (RS LOW for 0x202 command, RS HIGH stays for data)
  LCD_WR_Ctrl(0x202);
}

void LCD_Init(void)
{
  // Reset sequence
  LCD_RS_HIGH();
  LCD_nRST_HIGH();
  Delay_mS(1);
  LCD_nRST_LOW();
  Delay_mS(10);
  LCD_nRST_HIGH();
  Delay_mS(50);

  lcd_id = LCD_ReadID();

  if (lcd_id == LCD_ID_ILI9327)
    LCD_Init_ILI9327();
  else
    LCD_Init_ILI932x();

  // Init DMA2 Channel 1 for LCD fills
  DMA2_CH1_CPAR = 0x60000000;
}

void Set_Pixel(uint_fast16_t Color)
{
  LCD_PORT = Color;
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
  if (lcd_id == LCD_ID_ILI9327)
  {
    LCD_WR_Ctrl(0x2E);
    Data = LCD_PORT; // dummy
    Data = LCD_PORT;
    LCD_WR_Ctrl(0x2C);
  }
  else
  {
    Data = LCD_PORT; // dummy
    Data = LCD_PORT;
  }
  return Data;
}

void Set_Block(int x1, int y1, int x2, int y2)
{
  if (lcd_id == LCD_ID_ILI9327)
  {
    x2--; y2--;
    LCD_WR_Ctrl(0x2A);      // Column address (Y axis)
    LCD_PORT = (y1 >> 8);
    LCD_PORT = (y1 & 0xFF);
    LCD_PORT = (y2 >> 8);
    LCD_PORT = (y2 & 0xFF);
    LCD_WR_Ctrl(0x2B);      // Page address (X axis)
    LCD_PORT = (x1 >> 8);
    LCD_PORT = (x1 & 0xFF);
    LCD_PORT = (x2 >> 8);
    LCD_PORT = (x2 & 0xFF);
    LCD_WR_Ctrl(0x2C);      // Memory Write
  }
  else
  {
    LCD_WR_Reg(0x200, y1);
    LCD_WR_Reg(0x201, x1);
    // Start GRAM write
    LCD_WR_Ctrl(0x202);
  }
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
  LCD_DMA_Wait();
  if (lcd_id == LCD_ID_ILI9327)
  {
    LCD_WR_Ctrl(0x2A);      // Column address (Y axis)
    LCD_PORT = 0x00;
    LCD_PORT = (y & 0xFF);
    LCD_PORT = 0x00;
    LCD_PORT = 0xEF;         // y_end = 239
    LCD_WR_Ctrl(0x2B);      // Page address (X axis)
    LCD_PORT = (x >> 8);
    LCD_PORT = (x & 0xFF);
    LCD_PORT = 0x01;
    LCD_PORT = 0x8F;         // x_end = 399
    LCD_WR_Ctrl(0x2C);      // Memory Write
  }
  else
  {
    LCD_WR_Reg(0x200, y);
    LCD_WR_Reg(0x201, x);
    LCD_WR_Ctrl(0x202);
  }
}

#else // DS203_BIOS_LCD — delegate to BIOS

void LCD_WR_Ctrl(int Reg)
{
  LCD_RS_LOW();
  LCD_PORT = Reg;
  LCD_RS_HIGH();
}

void Set_Pixel(uint_fast16_t Color)
{
  __LCD_SetPixl(Color);
}

void ReadStart(void)
{
}

void ReadFinish(void)
{
}

uint32_t ReadPixel()
{
  return __LCD_GetPixl();
}

void Set_Block(int x1, int y1, int x2, int y2)
{
  __LCD_Set_Block(x1, x2-1, y1, y2-1);
}

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
{
  __Point_SCR(x, y);
}

#endif // DS203_BIOS_LCD

void xBeep(bool b)
{
  #define TIM8_CCR2   (*((vu32 *)(0x40013400+0x38)))
  TIM8_CCR2 = 100 - (b ? 100 : 0)/2;
}

void EnableUsb(bool enable)
{
}

void HardwareInit()
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t)g_pfnVectors - NVIC_VectTab_FLASH);
#ifndef DS203_BIOS_LCD
  LCD_Init();
#endif
  __USB_Init();
  xBeep(false);
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
    case LicenseNumber: return (uintptr_t)(*(volatile uint32_t*)0x08003804);
    case LicenseValid: return -1; // no way to check validity
#ifndef DS203_BIOS_LCD
    case DisplayType: return (uintptr_t)(lcd_id == LCD_ID_ILI9327 ? "ILI9327" : "ILI932x");
#else
    case DisplayType: return (uintptr_t)"BIOS";
#endif
    case DiskType: return (uintptr_t)0;
    case BatteryVoltage: return (uintptr_t)__Get(V_BATTERY, 0);
    case Charging: return (uintptr_t)__Get(USB_POWER, 0);
    case FlashReadRange: return (uintptr_t)0; // Uses native usb interface
    case FlashWriteRange: return (uintptr_t)0;
    case FlashAlertRange: return (uintptr_t)0;
    case FirmwareChecksum: return (uintptr_t)&firmwareCrc;
    default: return 0;
  }
}
