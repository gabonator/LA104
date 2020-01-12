/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : BIOS.h  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#ifndef __BIOS_H
#define __BIOS_H

#include "system.h"


//============================= Flash space allocation =================================

#define BIN_BASE                ((u32)(0x0802C000)) // Size < 68KB  
#define PRM_BASE                BIN_BASE + 68*1024  // Size =  2KB   
#define INF_BASE                BIN_BASE + 70*1024  // Size < 10KB   
#define APP4_BASE               ((u32)(0x08024000)) // Size = 32KB  
#define APP3_BASE               ((u32)(0x0801C000)) // Size = 32KB  
#define APP2_BASE               ((u32)(0x08014000)) // Size = 32KB  
#define APP1_BASE               ((u32)(0x0800C000)) // Size = 32KB  
#define SYS_BASE                ((u32)(0x08004000)) // Size = 32KB   
#define DFU_BASE                ((u32)(0x08000000)) // Size = 16KB 

//====================== Function Set Object and Value defined ======================

#define CH_A_OFFSET       0     // A channel vertical displacement    	Value = 0~200
#define CH_B_OFFSET       1     // B channel vertical displacement    	Value = 0~200
#define BACKLIGHT         2     // backlight brightness         		Value = 0~100
#define BEEP_VOLUME       3     // buzzer volume       					Value = 0~100
#define BETTERY_DT        4     // battery voltage detector     		Value = 1: start
#define ADC_MODE          5     // ADC operating mode      				Value = 1/0
#define FIFO_CLR          6     // FIFO pointer reset 			   		Value = 1/0: W_PTR/R_PTR
  #define R_PTR           0       // FIFO read address pointer reset
  #define W_PTR           1       // FIFO write address pointer is reset
#define T_BASE_PSC        7     // prescaler value						Value = 0~65535
#define T_BASE_ARR        8     // frequency							Value = 0~65535
#define CH_A_COUPLE       9     // A channel coupling method			Value = 1/0: AC/DC
  #define DC              0
  #define AC              1
#define CH_A_RANGE       10     // A channel input range				Value = 0~5
#define CH_B_COUPLE      11     // B channel coupling method			Value = 1/0: AC/DC
#define CH_B_RANGE       12     // B channel input range				Value = 0~5
#define ANALOG_ARR       13     // analog output divider value			Value = 0~65535
#define ANALOG_PTR       14     // analog output pointer				Value = 0~65535
#define ANALOG_CNT       15     // synthetic points of a week			Value = 0~65535
#define DIGTAL_PSC       16     // pulse output prescaler value			Value = 0~65535
#define DIGTAL_ARR       17     // pulse output divider value			Value = 0~65535
#define DIGTAL_CCR       18     // pulse output duty cycle value 		Value = 0~65535
#define KEY_IF_RST       19     // timer interrupt flag reset		 	Value = 0
#define STANDBY          20     // to enter the power-down waiting		Value = 0
#define ANALOG_PSC       21	  // only implemented in Marcosin's sys version
#define FPGA_RST         31     // FPGA Reset							Value = 0

#define TRIGG_MODE       32+0  // trigger mode							Value = Mode
#define V_THRESHOLD      32+1  // voltage trigger threshold				Value = 0~200
#define T_THRESHOLD      32+2  // pulse width trigger gate limit		Value = 0~65535
#define ADC_CTRL         32+4  // ADC status							Value = 1/0 EN/DN
#define A_POSITION       32+5  // the zero position of CH_A				Value = 0~200
#define B_POSITION       32+6  // the zero position of CH_B				Value = 0~200
#define REG_ADDR         32+7  // the address determines which set of registers in the FPGA, the data read into the MCU

//==================== Function Set TRIGG_MODE of Value defined =====================
/*
CH_A Trigger source & kind select => 
0x00: by Negedge;   0x01: by Posedge;   0x02: by low level; 0x03: by high level
0x04: TL < Delta_T; 0x05: TL > Delta_T; 0x06: TH < Delta_T; 0x07: TH > Delta_T;

CH_B Trigger source & kind select =>
0x08: by Negedge;   0x09: by Posedge;   0x0A: by low level; 0x0B: by high level
0x0C: TL < Delta_T; 0x0D: TL > Delta_T; 0x0E: TH < Delta_T; 0x0F: TH > Delta_T;

CH_C Trigger source & kind select =>
0x10: by Negedge;   0x11: by Posedge;   0x12: by low level; 0x13: by high level
0x04: TL < Delta_T; 0x05: TL > Delta_T; 0x06: TH < Delta_T; 0x07: TH > Delta_T;

CH_D Trigger source & kind select =>
0x18: by Negedge;   0x19: by Posedge;   0x1A: by low level; 0x1B: by high level
0x1C: TL < Delta_T; 0x1D: TL > Delta_T; 0x1E: TH < Delta_T; 0x1F: TH > Delta_T;

0x20~0xFF  =>  Unconditional trigger
*/

#define UNCONDITION       0x20        // unconditional trigger sampling

//================ Function Set in ADC_CTRL the STANDBY Value defined ================

#define DN            0
#define EN            1

//===================== Function Set ADC_MODE of Value defined =====================

#define SEPARATE      0    // ADC sampling mode independently
#define INTERLACE     1    // ADC Alternate Sampling mode

//========================= Function Get Kind defined ============================

#define FIFO_DIGIT             0    // 16bits FIFO digital data
#define FIFO_EMPTY             1    // FIFO empty flag: 1 = empty
#define FIFO_START             2    // FIFO start flag: 1 = start
#define FIFO_FULL              3    // FIFO full flag: 1 = Full
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
#define USB_POWER              5    // USB power status: 1 = Power ON
#define V_BATTERY              6    // Battery voltage (mV)
#define VERTICAL               7    // vertical channel attribute pointer
#define HORIZONTAL             8    // horizontal channel attribute pointer
#define GLOBAL                 9    // pointer to the whole property
#define TRIGGER                10   // trigger channel attribute pointer
#define FPGA_OK                11   // FPGA configuration was successful 1 = the FPGA config the ok
#define CHARGE         12       // battery charge status
#define HDWVER         13       // device hardware version number
#define DFUVER         14       // the DFU program module version number
#define SYSVER         15       // the SYS program module version number
#define FPGAVER        16       // version number of the FPGA configuration program
#define DEVICEINFO     18       //DISK=7 (FLASH 2M,OR 8M)

#define ADC_DATA       32+0     // 0~7:ADC_CH_A 8~15:ADC_CH_B 16~17:CH_C&CH_D 
#define PRE_SAMPLING   32+1     // 0~15:Pre-sampling depth
#define ALL_SAMPLING   32+2     // 0~15:Total sampling depth
#define CH_A_MIN_MAX   32+3     // 0~7:VMIN 8~15:VMAX 
#define CH_A_V_SUM     32+4     // 0~15:CH_A voltage sum
#define CH_A_V_SSQ     32+5     // 0~15:CH_A voltage sum of squares
#define CH_A_NEDGE     32+6     // 0~15:CH_A number of edge
#define CH_A_FREQ      32+7     // 0~15:CH_A frequence
#define CH_A_PHW_MAX   32+8     // 0~15:CH_A pulse high width MAX
#define CH_A_PHW_MIN   32+9     // 0~15:CH_A pulse high width MIN
#define CH_A_PLW_MAX   32+10    // 0~15:CH_A pulse low width MAX
#define CH_A_PLW_MIN   32+11    // 0~15:CH_A pulse low width MIN

// =============================================================================

typedef struct  // hardware properties
{
  u16 LCD_X;    // horizontal screen display points
  u16 LCD_Y;    // vertical screen display points
  u16 Yp_Max;   // maximum vertical gear
  u16 Xp_Max;   // maximum level stalls
  u16 Tg_Num;   // trigger the stalls in the maximum
  u16 Yv_Max;   // maximum vertical displacement
  u16 Xt_Max;   // maximum horizontal displacement
  u16 Co_Max;   // maximum coupling mode
  u8  Ya_Num;   // the number of analog channels
  u8  Yd_Num;   // the number of digital channels
  u8  INSERT;   // start using the interpolated stalls
  u16 KpA1;     // A channel shift compensation factor
  u16 KpA2;     // A channel displacement compensation coefficient
  u16 KpB1;     // B-channel displacement compensation coefficient
  u16 KpB2;     // B channel displacement compensation coefficient
} G_attr ;

typedef struct  // vertical channel properties
{
  char STR[8];  // stall identification string
  s16 KA1;      // A channel displacement error correction factor 1
  u16 KA2;      // A channel slope error correction factor
  s16 KB1;      // B-channel displacement error correction factor 1
  u16 KB2;      // B channel slope error correction factor
  u32 SCALE;    // vertical channel scale factor
} Y_attr ;

typedef struct  // horizontal channel properties
{
  char STR[8];  // stall identification string
  s16 PSC;      // prescaler coefficient
  u16 ARR;      // frequency coefficient
  u16 CCR;      // duty cycle coefficient
  u16 KP;       // interpolation coefficients
  u32 SCALE;    // horizontal channel scale factor
} X_attr ; 

typedef struct  // chanel trigger properties
{
  char STR[8];  // trigger identification string
  u8  CHx;      // trigger channel number
  u8  CMD;      // trigger control word
} T_attr ; 

extern Y_attr *Y_Attr; 
extern X_attr *X_Attr; 
extern G_attr *G_Attr; 
extern T_attr *T_Attr; 

//==============================================================================
//                        System function entrance
//==============================================================================
 void __CTR_HP(void);     //USB_HP_Interrupt
 void __USB_Istr(void);   //USB_LP_Interrupt
 void __USB_Init(void);                      
 
 void __LCD_Initial(void);
 void __Clear_Screen(u16 Color);
 void __Point_SCR(u16 x0, u16 y0);
 void __LCD_SetPixl(u16 Color);
 u16  __LCD_GetPixl(void);  
 u16  __Get_TAB_8x14(u8 Code, u16 Row);
 void __LCD_Set_Block(u16 x1, u16 x2, u16 y1, u16 y2);
 
 void __LCD_Copy(uc16 *pBuffer, u16  NumPixel); // Send a row data to LCD
 void __LCD_Fill(u16 *pBuffer,u16 NumPixel);    // Fill number of pixel by DMA 
 void __LCD_DMA_Ready(void);                    // Wait LCD data DMA ready
 
 void __Row_Copy(uc16 *S_Buffer,u16 *T_Buffer); // Copy one row base data to buffer
 void __Row_DMA_Ready(void);                    // Wait row base data DMA ready
 
 u32  __Read_FIFO(void);                        // Read data from FIFO & Ptr+1
 void __Display_Str(u16 x0, u16 y0, u16 Color, u8 Mode, char *s);                      
 
 u32  __Input_Lic(u16 x0, u8 y0);               //Return: 32Bits Licence 
 u32  __GetDev_SN(void);                        // Get 32bits Device Serial Number

 u8   __Ident(u32 Dev_ID, u32 Proj_ID, u32 Lic_No);
 
 void __Set_Param(u8 RegAddr, u8 Parameter); 
 void __Set(u8 Object, u32 Value);
 u32  __Get(u8 Object);
 
 void __ExtFlash_PageRD(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
 void __ExtFlash_PageWR(u8* pBuffer, u32 WriteAddr);
 u8   __ReadDiskData(u8* pBuffer, u32 ReadAddr, u16 Lenght);
 u8   __ProgDiskPage(u8* pBuffer, u32 ProgAddr);
 
 u8   __FLASH_Erase(u32 Address);
 u8   __FLASH_Prog(u32 Address, u16 Data);
 void __FLASH_Unlock(void);
 void __FLASH_Lock(void);
 
 u8   __Chk_SYS(u32 Licence);
 u8*  __Chk_DFU(void);
 u8*  __Chk_HDW(void);

 u8 __OpenFileWr(u8* Buffer, char* FileName, u16* Cluster, u32* pDirAddr);
 u8 __OpenFileRd(u8* Buffer, char* FileName, u16* Cluster, u32* pDirAddr);
 u8 __ReadFileSec(u8* Buffer, u16* Cluster);
 u8 __ProgFileSec(u8* Buffer, u16* Cluster);
 u8 __CloseFile(u8* Buffer, u32 Lenght, u16* Cluster, u32* pDirAddr);
/**/
 
#endif  
/*******************************  END OF FILE  ********************************/
