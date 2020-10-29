#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef const uint16_t uc16;
#define BEEP_VOLUME       3     // 

  #define KEY_STATUS             4    // Current keys status 
  #define K_ITEM_D_STATUS      0x0008    // 0 = Key push on
  #define K_ITEM_S_STATUS      0x0040    // 0 = Key push on
  #define KEY3_STATUS          0x0100    // 0 = Key push on
  #define KEY4_STATUS          0x0200    // 0 = Key push on
  #define K_INDEX_D_STATUS     0x0400    // 0 = Key push on
  #define K_INDEX_I_STATUS     0x0800    // 0 = Key push on
  #define K_INDEX_S_STATUS     0x1000    // 0 = Key push on
  #define KEY2_STATUS          0x2000    // 0 = Key push on
  #define KEY1_STATUS          0x4000    // 0 = Key push on
  #define K_ITEM_I_STATUS      0x8000    // 0 = Key push on

// TODO: one level of indirection could be reduced somehow!

#define BiosBase(addr) (void*)(0x08004200+addr)
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
void (*__FLASH_Unlock)() = (void (*)())BiosBase(0x79);
void (*__FLASH_Lock)() = (void (*)())BiosBase(0x7D);
