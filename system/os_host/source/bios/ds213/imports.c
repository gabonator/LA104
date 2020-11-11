#include "../imports.h"
#include "library/STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "DS213HwDriver.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

volatile bool eepromAccessMutex = false;
uint32_t gFlashReadRange[2] = {-1, 0};
uint32_t gFlashWriteRange[2] = {-1, 0};
uint32_t gFlashAlertRange[2] = {-1, 0};

#define _SEC_SIZE         4096 // TODO: JOIN

HwDrvDef  Hw;

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
  if (b)
    *Hw.pBuz_Dev |= ENABLE;
  else
    *Hw.pBuz_Dev &= ~ENABLE;
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

  *Hw.pPwm_Bkl = 50;
  *Hw.pBuz_Vol = 50;
  *Hw.pBuz_Dev &= ~ENABLE;
}

uint8_t encoderXstate[4] = {0xff, 0xff, 0xff, 0xff};
uint8_t encoderYstate[4] = {0xff, 0xff, 0xff, 0xff};

uint32_t ProcessEncoder(int a, int b, uint8_t* state, uint32_t increment, uint32_t decrement)
{
  a |= b<<1;

  // filter out glitches
  if (a!=state[0] || a!=state[1] || a!=state[2])
  {
    state[0] = state[1];
    state[1] = state[2];
    state[2] = a;
    return 0;
  }

  // stable reading for 4ms (4 runs)
  if (state[3] == a)
    return 0;

  // dec: 00 11 10 00
  // inc: 00 10 11 00
  int code = (state[3] << 2) | a;
  state[3] = a;
  switch (code)
  {
    case 0b0011:
    case 0b1110: 
      return decrement;
    case 0b1011:
    case 0b1100: 
      return increment;
    default:
      return 0;
  }
}

uint32_t GetKeys()
{
  uint32_t keys = 0;
  keys |= !*Hw.pK1_St;
  keys |= (!*Hw.pK2_St)<<1;
  keys |= (!*Hw.pK3_St)<<2;
  keys |= (!*Hw.pK4_St)<<3;

  keys |= ProcessEncoder(*Hw.pEa_St, *Hw.pEb_St, encoderXstate, KeyRight, KeyLeft);
  keys |= ProcessEncoder(*Hw.pEc_St, *Hw.pEd_St, encoderYstate, KeyUp, KeyDown);
  return keys;
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
  gFlashWriteRange[0] = min(gFlashWriteRange[0], WriteAddr);
  gFlashWriteRange[1] = max(gFlashWriteRange[1], WriteAddr);
  if (gFlashAlertRange[0] >= gFlashAlertRange[1])
  {
    if (gFlashAlertRange[0] >= WriteAddr && WriteAddr >= gFlashAlertRange[1])
    {
      gFlashAlertRange[0] = -1;
      gFlashAlertRange[1] = 0;
    }
  }

  eepromAccessMutex = true;

  Hw.pSpiFlashSecW((uint32_t)pBuffer, WriteAddr, _SEC_SIZE);

  bool aux = eepromAccessMutex; // was this function interrupted? Needs atomic operation
  eepromAccessMutex = false;
  return aux;
}

bool ExtFlashDataRd(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Length)
{
  eepromAccessMutex = true;
  gFlashReadRange[0] = min(gFlashReadRange[0], ReadAddr);
  gFlashReadRange[1] = max(gFlashReadRange[1], ReadAddr);

  Hw.pSpiFlashRead((uint32_t)pBuffer, ReadAddr, _SEC_SIZE);

  bool aux = eepromAccessMutex;
  eepromAccessMutex = false;
  return aux;
}

uintptr_t GetAttribute(enum EAttribute attribute)
{
  switch (attribute)
  {
    case DeviceType: return (uintptr_t)"DS213";
    case VersionDfu: return (uintptr_t)(char*)Hw.pDfuVerStr;
    case VersionHardware: return (uintptr_t)(char*)Hw.pHwVerStr;
    case VersionSystem: return (uintptr_t)(char*)Hw.pProductStr;
    case VersionFpga: return (uintptr_t)(char*)Hw.pFpgaTypStr;
    case SerialNumber: return (uintptr_t)(uint32_t)Hw.ProductSN;
    case LicenseNumber: return (uintptr_t)(uint32_t) Hw.GetLicence;
    case LicenseValid: return (uintptr_t)(uint32_t) Hw.LicenceOk;
    case DisplayType: return (uintptr_t)(uint32_t)Hw.pLcdcTypStr;
    case DiskType: return (uintptr_t)(uint32_t)Hw.pDiskTypStr;
    case BatteryVoltage: return (uintptr_t)*Hw.pAdc_Vbty*375/256*8; // unverified 
    case Charging: return (uintptr_t)*Hw.pSt_Vin; // pSt_Chg
    case FlashReadRange: return (uintptr_t)gFlashReadRange;
    case FlashWriteRange: return (uintptr_t)gFlashWriteRange;
    case FlashAlertRange: return (uintptr_t)gFlashAlertRange;
    default: return 0;
  }
}
