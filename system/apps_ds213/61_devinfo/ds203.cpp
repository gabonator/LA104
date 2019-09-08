#include "ds203.h"

typedef uint8_t u8;
typedef uint32_t u32;
typedef const uint8_t uc8;
typedef const uint16_t uc16;
typedef volatile uint8_t vu8;
typedef volatile uint16_t vu16;
typedef volatile uint32_t vu32;

// Bios
#define BiosBase(addr) (void*)(0x08004200+addr)
u32 (*__Get)(u8, u32) = (u32 (*)(u8, u32)) BiosBase(0x55);
u32 (*__GetDev_SN)() = (u32 (*)()) BiosBase(0x3D);
/*
const void (*__CTR_HP)(void) = BiosBase(0x01);
const void (*__USB_Istr)(void) = BiosBase(0x05);
const void (*__LCD_Initial)(void) = BiosBase(0x09);
const void (*__Point_SCR)(u16, u16) = BiosBase(0x0D);
const void (*__LCD_SetPixl)(u16) = BiosBase(0x11);
const void (*__Clear_Screen)(u16) = BiosBase(0x15);
const u16 (*__Get_TAB_8x14)() = BiosBase(0x19);
const void (*__LCD_Set_Block)(u16, u16, u16, u16) = BiosBase(0x1D);
const void (*__LCD_DMA_Ready)() = BiosBase(0x21);
const void (*__LCD_Copy)(uc16*, u16) = BiosBase(0x25);
const void (*__LCD_Fill)(uc16*, u16) = BiosBase(0x29);
const void (*__Row_DMA_Ready)() = BiosBase(0x2D);
const void (*__Row_Copy)(uc16*, u16*) = BiosBase(0x31);
const u32 (*__Read_FIFO)() = BiosBase(0x35);
const u32 (*__Input_Lic)(u16, u8) = BiosBase(0x39);
const u32 (*__GetDev_SN)() = BiosBase(0x3D);
const u8 (*__Chk_SYS)(u32) = BiosBase(0x41);
const u8 (*__Ident)(u32, u32, u32) = BiosBase(0x45);
const void (*__Display_Str)(u16, u16, u16, u8, char*) = BiosBase(0x49);
const void (*__Set)(u8, u32) = BiosBase(0x4D);
//const void (*__Set_Param)() = BiosBase + 0x51;
const u32 (*__Get)(u8, u32) = BiosBase(0x55);
const void (*__ExtFlash_PageWr)(u8*, u32, u16) = BiosBase(0x59);
const void (*__ExtFlash_PageRd)(u8*, u32) = BiosBase(0x5D);
const u8 (*__ReadDiskData)(u8*, u32, u16) = BiosBase(0x61);
const u8 (*__ProgDiskPage)(u8*, u32) = BiosBase(0x65);
const u16 (*__LCD_GetPixl)() = BiosBase(0x69);
const void (*__USB_Init)() = BiosBase(0x6D);
const u8 (*__FLASH_Erase)(u32) = BiosBase(0x71);
const u8 (*__FLASH_Prog)(u32, u16) = BiosBase(0x75);
const void (*__FLASH_Unlock)() = BiosBase(0x79);
const void (*__FLASH_Lock)() = BiosBase(0x7D);
const u8* (*__Chk_DFU)() = BiosBase(0x81);
const u8* (*__Chk_HDW)() = BiosBase(0x85);
const u8 (*__OpenFileWr)(u8*, char*, u16*, u32*) = BiosBase(0x89);
const u8 (*__OpenFileRd)(u8*, char*, u16*, u32*) = BiosBase(0x8D);
const u8 (*__ReadFileSec)(u8*, u16*) = BiosBase(0x91);
const void (*__ProgFileSec)(u8*, u16*) = BiosBase(0x95);
const void (*__CloseFile)(u8*, u32, u16*, u32*) = BiosBase(0x99);
//const void (*__Row_Fill)() = BiosBase(0x9D);
*/

#define HDWVER         13       // 
#define DFUVER         14       // DFU
#define SYSVER         15       // SYS
#define FPGAVER        16       // FPGA

// LCD helpers
#define GPIO_Pin_11                ((u16)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((u16)0x1000)  /* Pin 12 selected */

#define GPIOD_BSRR  (*((vu32 *)(0x40011400+0x10)))
#define GPIOD_BRR   (*((vu32 *)(0x40011400+0x14)))

#define LCD_nRST_LOW()    GPIOD_BRR  = GPIO_Pin_11
#define LCD_nRST_HIGH()   GPIOD_BSRR = GPIO_Pin_11
#define LCD_RS_LOW()      GPIOD_BRR  = GPIO_Pin_12
#define LCD_RS_HIGH()     GPIOD_BSRR = GPIO_Pin_12

#define LCD_PORT    (*((vu16 *)(0x60000000+0x00)))

void LCD_WR_Ctrl(u16 Reg) 
{
  LCD_RS_LOW();
  LCD_PORT = Reg;        //Reg. Addr.
  LCD_RS_HIGH();
}

uint32_t LCD_RD_Type(void) 
{ 
  uint32_t LCD_Type;
  LCD_WR_Ctrl(0xEF);
  LCD_Type  = (LCD_PORT&0xFF);
  LCD_Type  = (LCD_PORT&0xFF)<<24;
  LCD_Type |= (LCD_PORT&0xFF)<<16;
  LCD_Type |= (LCD_PORT&0xFF)<<8;
  LCD_Type |= (LCD_PORT&0xFF);
  return LCD_Type;
}

// implementation
char* CDS203::DfuVersion()
{
  return (char*)__Get(DFUVER, 0);
}

char* CDS203::HardwareVersion()
{
  return (char*)__Get(HDWVER, 0);
}

char* CDS203::SystemVersion()
{
  return (char*)__Get(SYSVER, 0);
}

char* CDS203::FpgaVersion()
{
  return (char*)__Get(FPGAVER, 0);
}

char* CDS203::SerialNumber()
{                            
  static char serial[16];
  sprintf(serial, "%08x", __GetDev_SN());
  return serial;
}

char* CDS203::DisplayType()
{
/*
  LCD_RS_HIGH();
  LCD_nRST_HIGH(); 
  LCD_DelayMs(1);           //RST=1
  LCD_nRST_LOW(); 
  LCD_DelayMs(10);          //RST=0 Reset LCD
  LCD_nRST_HIGH(); 
  LCD_DelayMs(50);          //RST=1
*/
  LCD_RD_Type();
  if ( LCD_RD_Type() == 0x02049327 ) 
    return (char*)"ili9327";
  else
    return (char*)"r61509v";
}

char* CDS203::DiskType()
{
  return nullptr;
}
