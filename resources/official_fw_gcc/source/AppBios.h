#ifndef __BIOS_H
#define __BIOS_H

#include "STM32F10x.h"

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

//----------------------显示设备相关宏定义---------------------------
#define MCU_DRV         0xFFFB0000
#define FPGA_DRV        0xFFFA0000
#define XWDTH           280
#define YHIGH           154
#define X0              40
#define Y0              66

//--------------------- 扩展测试信号端口相关宏定义-------------------
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

//----------------------电源控制及测量设备相关宏定义-----------------
#define VIN_ST          0xFFFC0000
#define VBTYmV          0xFFFB0000

//----------------------按键类设备相关宏定义-------------------------
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

//----------------------显示设备相关宏定义---------------------------
#define SIZE            0xFFFC0000
#define MCU_WR          0xFFFB0000
#define MCU_RD          0xFFFA0000
#define FPGA_MOD        0xFFF90000
#define LCDCOL          320
#define LCDROW          240
#define LOGO_X          4*8
#define LOGO_Y          160

//----------------------FPGA 控制相关宏定义--------------------------
#define CNFG            0xFFFC0000
#define DONE            0xFFFA0000
#define VREF            3000
#define FPGA_BUSY       0x01
#define FPGA_FULL       0x02
#define FPGA_EMPTY      0x04
#define FPGA_PRES       0x08
#define FPGA_TRI        0x10

#define RECORD_DEPTH    3584   // 有效变化3584
#define CNTx4K          4
#define PSMP_NUM        2
//----------------------USB DISK 相关宏定义--------------------------

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
    SYSINFO, // 系统综合信息      Var: PRDT_SN  产品串号  Rtn: (u16)产品串号
             //                   Var: PRODUCT  产品型号  Rtn: 型号字串指针
             //                   Var: PRDTNUM  产品编号  Rtn: (u16)产品编号
             //                   Var: SCH_VER  硬件版本  Rtn: 版本字串指针
             //                   Var: MCU_TYP  MCU 型号  Rtn: 型号字串指针
             //                   Var: DFU_VER  DFU 版本  Rtn: 版本字串指针
             //                   Var: OEM_NUM  OEM 编号  Rtn: 编号字串指针
             //                   Var: MANUFAC 出品商信息 Rtn: 信息字串指针
             //                   Var: LICENCE 许可证状态 Rtn: SUCC/FAIL
             // 系统控制类
    NIVCPTR, // 中断矢量基址设置  Var: (u16)   矢量基址  Rtn: SUCC/FAIL
    SYSTICK, // 系统嘀嗒中断设置  Var: (u16)uS 中断时长  Rtn: SUCC/FAIL
    AF_RMAP, // 设备 IO 重新定位  Var: (u32)   AF REMAP  Rtn: SUCC/FAIL
    PWRCTRL, // 电源控制          Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: VIN_ST  外接电源  Rtn: ENBL/DSBL
             //                   Var: VBAT    电池电压  Rtn: (u16)mV
             //                   Var: DSBL    电源关闭  Rtn: SUCC/FAIL
    BUZZDEV, // 蜂鸣器接口        Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: (MUTE~100)  音量  Rtn: SUCC/FAIL
    KEYnDEV, // 按键类输入设备    Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: BITMAP  读取键位  Rtn: (u16)键位状态
    DELAYuS, // 微秒延迟          Var: (u32)uS 延迟时长  Rtn: SUCC/FAIL
             // 显示控制类
    DISPDEV, // LCD 显示设备      Var: STRING  模块型号  Rtn: 型号字串指针
             //                   Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: VALUE   屏幕规格  Rtn: (MaxX<<16+MaxY)
             //                   Var: (0~100) 背光亮度  Rtn: SUCC/FAIL
             //                   Var: (MCU/FPGA)驱动源  Rtn: SUCC/FAIL
    BLOCK_X, // 显示块 x 域设置   Var: (x1 << 16)+x2)    Rtn: SUCC/FAIL
    BLOCK_Y, // 显示块 y 域设置   Var: (y1 << 16)+y2)    Rtn: SUCC/FAIL
    PIXEL_X, // 像素 x 位置       Var: (u16)位置         Rtn: SUCC/FAIL
    PIXEL_Y, // 像素 y 位置       Var: (u16)位置         Rtn: SUCC/FAIL
    WrPIXEL, // 显示像素写        Var: (u16)像素颜色     Rtn: SUCC/FAIL
    RdPIXEL, // 读取屏幕像素      Var:                   Rtn: (u16)像素颜色
    FONTPTR, //                   Var: ASCII Code        Rtn: ptr 字串指针
             // FPGA 控制类
    FPGADEV, // FPGA 设备控制     Var: STRING            Rtn: 型号字串指针
             //                   Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: CNFG    上电配置  Rtn: SUCC/FAIL
             //                   Var: COMM    通信模式  Rtn: SUCC/FAIL
             //                   Var: ENBL    接口片选  Rtn: SUCC/FAIL
             //                   Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
             // 采样输入控制类
    IN_PORT, // 逻辑输入控制      Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: VREF 逻辑参考电压 Rtn: SUCC/FAIL
             // U 盘控制类
    USBDEV,  // USB 初始化
    U_DISK,  // USB DISK 设备     Var: INIT  初始化设置  Rtn: SUCC/FAIL
             //                   Var: STRING  U 盘规格  Rtn: U 盘规格字串指针
             //                   Var: PAGE  存储页尺寸  Rtn: SUCC/FAIL
             //                   Var: SECTOR  扇区容量  Rtn: (u16)扇区容量
             //                   Var: AMOUNT  扇区数量  Rtn: (u16)扇区数量
             //                   Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
             // SPI 控制类
    SPI_DEV, // SPI 设备接口      Var: (SPI_ParamDef*)   Rtn: SUCC/FAIL
    FLSHDEV, // Flash SPI
             // 扩展测试接口控制类
    EXT_INP, // 扩展 PIO 接口     Var: PIOCFG+PinDef      Rtn: SUCC
    EXT_OUT, // 扩展 PIO 接口     Var: PIOCFG+PinDef      Rtn: SUCC
             //                   Var: PINS_OUT+Status0~3 Rtn: SUCC
             //                   Var: PINS_IN+BitMask0~3 Rtn: PinStatus 0~3
    EXT_PWM, // 扩展 PWM 接口     Var: PWM_PSC+Var[0~15]  Rtn: SUCC
             //                   Var: PWM_ARR+Var[0~15]  Rtn: SUCC
             //                   Var: CH1_CCR+Var[0~15]  Rtn: SUCC
             //                   Var: CH2_CCR+Var[0~15]  Rtn: SUCC
             //                   Var: CH1_CTRL+ENBL/DSBL Rtn: SUCC
             //                   Var: CH2_CTRL+ENBL/DSBL Rtn: SUCC
    EXT_INF, // 扩展 PIO 接口     Var: INIT  初始化设置 设置为浮空输入
    EXT_SPI, // 扩展 PIO 接口     Var: INIT  初始化设置   Rtn: SUCC/FAIL
             //                   Var: (SPI_ParamDef*)    Rtn: SUCC/FAIL
    EXT_UART,// 扩展 PIO 接口     Var: INIT  初始化设置   Rtn: SUCC/FAIL
             //                   Var: u8 Data            Rtn: SUCC/FAIL
    EXT_I2C, // 扩展 PIO 接口
             //                   Var: 0x00&ID&REG&DATA   Rtn: SUCC/FAIL
    EXT_RXD, // 扩展 PIO 接口     Var: PIOCFG+PinDef      Rtn: SUCC
    EXT_TXD, // 扩展 PIO 接口     Var: PIOCFG+PinDef      Rtn: SUCC
};

enum
{
    PRDT_SN, // 产品串号
    PRODUCT, // 产品型号字符串
    PRDTNUM, // 产品编号
    HDW_VER, // 硬件版本字符串
    MCU_TYP, // MCU 型号字符串
    DFU_VER, // DFU 版本字符串
    OEM_NUM, // OEM 编号字符串
    MANUFAC, // 出品商信息字符串
    LICENCE, // 许可证授权状态
};

//函数 void FpgaRW(u8 Item, u16 n, u8* p) Item 的调用说明
enum
{
    FPGA_INFO = 0x00, // 状态及固件版本号  n = 1, Rtn p[0] = St, p[1]=未知数据, p[2]=0, p[3] = VerNum
    FPGA_RST  = 0x01, // 芯片内部逻辑复位  n = 0, p* = Dummny
    SMPL_CLR  = 0x02, // 芯片内部逻辑复位  n = 1, p* = Dummny
    WFRM_SAVE = 0x03, // 设置波形曲线数据  n = Row+1, p = &DataBytes

    WINDOW_Y  = 0x04, // 设置显示窗口高度  n = 1, p[1]   = Y Size
    WINDOW_X  = 0x05, // 设置显示窗口宽度  n = 2, p[1~2] = X Size
    WFRM_DRAW = 0x06, // 刷新波形显示图像  n = 0, p[1] = Col, p[2~3] = Row
    FLSH_ATTR = 0x07, // 设置闪烁属性标志  n = 1, p[1~2] = Attr

    RUL_POSN  = 0x10, // 设置 RUL 表尺位置  n = 1, p[1] = RUL Position
    CH1_POSN  = 0x11, // 设置 CH1 波形位置  n = 1, p[1] = CH1 Position
    CH2_POSN  = 0x12, // 设置 CH2 波形位置  n = 1, p[1] = CH2 Position
    CH3_POSN  = 0x13, // 设置 CH3 波形位置  n = 1, p[1] = CH3 Position
    CH4_POSN  = 0x14, // 设置 CH4 波形位置  n = 1, p[1] = CH4 Position
    CH5_POSN  = 0x15, // 设置 CH5 包络位置  n = 1, p[1] = CH5 Position
    CH6_POSN  = 0x16, // 设置 CH6 包络位置  n = 1, p[1] = CH6 Position

    ZC_POSN   = 0x18, // 设置 ZC 标线位置  n = 2, p[1~2] = ZC Position
    T0_POSN   = 0x19, // 设置 T0 标线位置  n = 2, p[1~2] = T0 Position
    T1_POSN   = 0x1A, // 设置 T1 标线位置  n = 2, p[1~2] = T1 Position
    T2_POSN   = 0x1B, // 设置 T2 标线位置  n = 2, p[1~2] = T2 Position

    TRG_KIND  = 0x20, // 设置 触发类型  n = 1, p[1]   = Trigger Kind
    TRG_COND  = 0x21, // 设置 触发条件  n = 1, p[1]   = Trigger COND
    MEM_MASK  = 0x22, // 设置 触发屏蔽  n = 1, p[1]   = Trigger Mask
    MEM_TWTH  = 0x23, // 设置 触发脉宽  n = 2, p[1~2] = Trigger Twth

    MEM_PSMP  = 0x30, // 设置 预采记录深度n = 1, p[1]   缺省值为9
    MEM_RPTR  = 0x31, // 设置 记录读指针  n = 2, p[1~2] = Record Rptr
    MEM_READ  = 0x32, // 读出 记录数据    n = 3, Rtn p[1~3] = Record Data
                      // [0]:Info ,[1]:Tcnt[7:0],[2]:Tcnt[13:8],[3]:Di
                      
    FPGA_ST   = 0x00, // 返回的 FPGA 状态字节的位置
    VER_NUM   = 0x01, // 返回的 FPGA 固件版本号的位置
    BUSY      = 0x01, // FPGA 状态字节 St 中的 “波形图像刷新” 标志位

};
//Info状态位{3'd0, TrgRdy, PesRdy, Empty, Full, BusySt};
//触发类型
/*
    0: o_Trig <= 1'd0; // 禁止触发
    1: o_Trig <= DiNE; // 输入组合逻辑的状态不符预设条件触发
    2: o_Trig <= DiEQ; // 输入组合逻辑的状态符合预设条件触发
    3: o_Trig <= 1'd1; // 任意触发
    4: o_Trig <= DfLE; // 输入不符条件的持续宽度小于阈值触发
    5: o_Trig <= DtLE; // 输入符合条件的持续宽度小于阈值触发
    6: o_Trig <= DfGT; // 输入不符条件的持续宽度大于阈值触发
    7: o_Trig <= DtGT; // 输入符合条件的持续宽度大于阈值触发
*/

#endif
/*********************************  END OF FILE  ******************************/
