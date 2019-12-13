#ifndef __BIOS_H
#define __BIOS_H

#include "MyDefine.h"

typedef struct
{
    u16  n;
    u8  *pBuf;
}SPI_ParamDef;

typedef struct
{
    u8 flag;
    u8 total;
    u8 index;
}MENU_Struct;

typedef struct
{
    u8 flag;
    u8 total;
    u8 index;
}ITEM_Struct;

typedef struct
{
    u8 flag;
    u8 total;
    u8 index;
}SUBITEM_Struct;

#define OFF             0
#define ON              1
#define SUCC            0
#define FAIL            1
#define HIGH            1
#define LOW             0
#define NONE            0
#define ENBL            1
#define DSBL            0
#define INIT            0xFFFF0000
#define STRING          0xFFFE0000
#define VALUE           0xFFFD0000

//----------------------
#define MCU_DRV         0xFFFB0000
#define FPGA_DRV        0xFFFA0000
#define XWDTH           280
#define YHIGH           154
#define X0              40
#define Y0              66

//--------------------- 
#define PWM1_PSC        0xFFFC0000
#define PWM1_ARR        0xFFFB0000
#define PWM1_DUTY1      0xFFFA0000
#define PWM1_DUTY2      0xFFF90000
#define PWM1_CTRL       0xFFF80000
#define PWM2_PSC        0xFFF60000
#define PWM2_ARR        0xFFF50000
#define PWM2_DUTY1      0xFFF40000
#define PWM2_DUTY2      0xFFF30000
#define PWM2_CTRL       0xFFF20000

//----------------------
#define VIN_ST          0xFFFC0000
#define VBTYmV          0xFFFB0000

//----------------------
#define BITMAP          0xFFFC0000
#define ENC1a           0x1000
#define ENC1b           0x2000
#define ENC2a           0x4000
#define ENC2b           0x8000
#define K1_HOLD         0x0001
#define K2_HOLD         0x0002
#define K3_HOLD         0x0004
#define K4_HOLD         0x0008
#define K1_ACTn         0x0010
#define K2_ACTn         0x0020
#define K3_ACTn         0x0040
#define K4_ACTn         0x0080
#define K1_ACTp         0x0100
#define K2_ACTp         0x0200
#define K3_ACTp         0x0400
#define K4_ACTp         0x0800
#define Kn_ACTn         0x00F0
#define Kn_ACTp         0x0F00
#define KEYSBIT         0x000F
#define ENCD_1p         0x1000
#define ENCD_1n         0x2000
#define ENCD_2p         0x4000
#define ENCD_2n         0x8000
#define ENCDBIT         0xF000
#define READ_RUN_ST     (((~GPIOC->IDR) & K1_PIN)? 1:0)
#define READ_KEY        (~__Bios(KEYnDEV, BITMAP) & KEYSBIT)

//----------------------
#define SIZE            0xFFFC0000
#define MCU_WR          0xFFFB0000
#define MCU_RD          0xFFFA0000
#define FPGA_MOD        0xFFF90000
#define LCDCOL          320
#define LCDROW          240
#define LOGO_X          4*8
#define LOGO_Y          160

//----------------------FPGA 
#define CNFG            0xFFFC0000
#define DONE            0xFFFA0000
#define VREF            3000
#define FPGA_BUSY       0x01
#define FPGA_FULL       0x02
#define FPGA_EMPTY      0x04
#define FPGA_PRES       0x08
#define FPGA_TRI        0x10

//#define RECORD_DEPTH    3584   // gabo
#define RECORD_DEPTH    500
#define CNTx4K          4 // gabo
#define PSMP_NUM        2
//----------------------USB DISK 

#define PAGE            0xFFFC0000
#define SECTOR          0xFFFB0000
#define AMOUNT          0xFFFA0000

extern MENU_Struct Menu;
extern ITEM_Struct Item;
extern SUBITEM_Struct SubItem;

extern u8  gSamplBuf[], gLCD_Buf[], gLCD_Backup[];
extern u8  gBeepFlag, gStandByFlag;
extern u16 gSysCnt, gFullmS, gStandbyCnt, gAutoPwrCnt;
extern u16 gTimeBase, gBatVol, gUSB_Vol;

u32  __Bios(u8 Item, u32 Var);
u8   FPGA_LCD_RW(u8 Cmd, u16 Cnt, u8 *Buf);
u16  FPGA_ReadWrite(u8 Cmd, u16 Cnt, u16 Data);
u16  FPGA_Read(u8 Cmd, u16 Cnt, u16 Data);
u32  FPGA_RW_4Byte(u8 Cmd, u16 Cnt, u32 Data);
void DiskConfig(void);
void Delay_mS(vu32 mS);
void Delay_uS(vu32 uS);
void FpgaRW(u8 Cmd, u16 n, u8 *pBuf);
void ExtFlash_CS_LOW(void);
void ExtFlash_CS_HIGH(void);
void ProgmIO_SPIWr(u16 n, u8 *pBuf);
void ProgmIO_UARTTx(u8 Data);
void ProgmIO_I2CWr(u8 DevID, u8 Reg, u8 Data);

enum
{
    SYSINFO, // ϵͳ
             //                   Var: PRODUCT  
             //                   Var: PRDTNUM  
             //                   Var: SCH_VER  Ӳ
             //                   Var: MCU_TYP  MCU 
             //                   Var: DFU_VER  DFU 
             //                   Var: OEM_NUM  OEM 
             //                   Var: MANUFAC 
             //                   Var: LICENCE 
             // ϵͳ
    NIVCPTR, // 
    SYSTICK, // ϵͳ
    AF_RMAP, // 
    PWRCTRL, // 
             //                   Var: VIN_ST  
             //                   Var: VBAT    
             //                   Var: DSBL    
    BUZZDEV, // 
             //                   Var: (MUTE~100)  
    KEYnDEV, // 
             //                   Var: BITMAP  
    DELAYuS, // ΢
             // 
    DISPDEV, // LCD 
             //                   Var: INIT  
             //                   Var: VALUE   
             //                   Var: (0~100) 
             //                   Var: (MCU/FPGA)
    BLOCK_X, // 
    BLOCK_Y, // 
    PIXEL_X, // 
    PIXEL_Y, // 
    WrPIXEL, // 
    RdPIXEL, // 
    FONTPTR, //                   Var: ASCII Code        Rtn: ptr 
             // FPGA 
    FPGADEV, // FPGA 
             //                   Var: INIT  
             //                   Var: CNFG    
             //                   Var: COMM    ͨ
             //                   Var: ENBL    
             //                   Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
             // 
    IN_PORT, // 
             //                   Var: VREF 
             // U 
    USBDEV,  // USB 
    U_DISK,  // USB DISK 
             //                   Var: STRING  U 
             //                   Var: PAGE  
             //                   Var: SECTOR  
             //                   Var: AMOUNT  
             //                   Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
             // SPI 
    SPI_DEV, // SPI 
    FLSHDEV, // Flash SPI
             // 
    EXT_INP, // 
    EXT_OUT, // 
             //                   Var: PINS_OUT+Status0~3 Rtn: SUCC
             //                   Var: PINS_IN+BitMask0~3 Rtn: PinStatus 0~3
    EXT_PWM, // 
             //                   Var: PWM_ARR+Var[0~15]  Rtn: SUCC
             //                   Var: CH1_CCR+Var[0~15]  Rtn: SUCC
             //                   Var: CH2_CCR+Var[0~15]  Rtn: SUCC
             //                   Var: CH1_CTRL+ENBL/DSBL Rtn: SUCC
             //                   Var: CH2_CTRL+ENBL/DSBL Rtn: SUCC
    EXT_INF, // 
    EXT_SPI, // 
             //                   Var: (SPI_ParamDef*)    Rtn: SUCC/FAIL
    EXT_UART,// 
             //                   Var: u8 Data            Rtn: SUCC/FAIL
    EXT_I2C, // 
             //                   Var: 0x00&ID&REG&DATA   Rtn: SUCC/FAIL
    EXT_RXD, // 
    EXT_TXD, // 
};

enum
{
    PRDT_SN, // 
    PRODUCT, // 
    PRDTNUM, // 
    HDW_VER, // Ӳ
    MCU_TYP, // MCU 
    DFU_VER, // DFU 
    OEM_NUM, // OEM 
    MANUFAC, // 
    LICENCE, // 
};

//
enum
{
    FPGA_INFO = 0x00, // ״̬
    FPGA_RST  = 0x01, // оƬ
    SMPL_CLR  = 0x02, // оƬ
    WFRM_SAVE = 0x03, // 

    WINDOW_Y  = 0x04, // 
    WINDOW_X  = 0x05, // 
    WFRM_DRAW = 0x06, // ˢ
    FLSH_ATTR = 0x07, // 

    RUL_POSN  = 0x10, // 
    CH1_POSN  = 0x11, // 
    CH2_POSN  = 0x12, // 
    CH3_POSN  = 0x13, // 
    CH4_POSN  = 0x14, // 
    CH5_POSN  = 0x15, // 
    CH6_POSN  = 0x16, // 

    ZC_POSN   = 0x18, // 
    T0_POSN   = 0x19, // 
    T1_POSN   = 0x1A, // 
    T2_POSN   = 0x1B, // 

    TRG_KIND  = 0x20, // 
    TRG_COND  = 0x21, // 
    MEM_MASK  = 0x22, // 
    MEM_TWTH  = 0x23, // 

    MEM_PSMP  = 0x30, // 
    MEM_RPTR  = 0x31, // 
    MEM_READ  = 0x32, // 
                      // [0]:Info ,[1]:Tcnt[7:0],[2]:Tcnt[13:8],[3]:Di
                      
    FPGA_ST   = 0x00, // 
    VER_NUM   = 0x01, // 
    BUSY      = 0x01, // FPGA ״̬

};
//Info״̬λ{3'd0, TrgRdy, PesRdy, Empty, Full, BusySt};
//
/*
    0: o_Trig <= 1'd0; // 
    1: o_Trig <= DiNE; // 
    2: o_Trig <= DiEQ; // 
    3: o_Trig <= 1'd1; // 
    4: o_Trig <= DfLE; // 
    5: o_Trig <= DtLE; // 
    6: o_Trig <= DfGT; // 
    7: o_Trig <= DtGT; // 
*/

#endif
/*********************************  END OF FILE  ******************************/
