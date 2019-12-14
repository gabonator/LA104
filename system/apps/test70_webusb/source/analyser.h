namespace ANALYSER
{



enum global
{
  RECORD_DEPTH = 3584,
  PARAM_NUM = 70,

 FPGA_BUSY=0x01,
 FPGA_FULL=0x02,
 FPGA_EMPTY=0x04,
 FPGA_PRES=0x08,
 FPGA_TRI=0x10,
  TRI_POSI=2

};

	enum
	{
		SYSINFO, NIVCPTR, SYSTICK, AF_RMAP, PWRCTRL, BUZZDEV, KEYnDEV, DELAYuS,
		DISPDEV, BLOCK_X, BLOCK_Y, PIXEL_X, PIXEL_Y, WrPIXEL, RdPIXEL, FONTPTR,
		FPGADEV, IN_PORT, USBDEV, U_DISK, SPI_DEV, FLSHDEV, EXT_INP, EXT_OUT,
		EXT_PWM, EXT_INF, EXT_SPI, EXT_UART, EXT_I2C, EXT_RXD, EXT_TXD // 27
	};

enum
{
    INTYPE = 0,
    TIMEBASE = 10, T1POSI,  T2POSI,  XPOSI,    T0POSI,   ZCPOSI,
    TRITYPE = 20,  TRICOND, TRIMASK, TRIWIDTH, THRESHOLD,
    SOURCE1 = 30,  SOURCE2, SOURCE3,
    OUTTYPE = 40,
    FILEEXIT = 50, SAVEBMP,  SAVECSV, SAVESVG, SAVEBUF,   SAVEDAT, LOADDAT,  LOADBUF,
    SAVEPAM = 60,  RSTPAM,  VOLUME,  BKLIGHT,   STANDBY, POWEROFF,
};


enum
{
    FPGA_INFO = 0x00, 
    FPGA_RST  = 0x01, 
    SMPL_CLR  = 0x02, 
    WFRM_SAVE = 0x03, 

    WINDOW_Y  = 0x04, 
    WINDOW_X  = 0x05, 
    WFRM_DRAW = 0x06, 
    FLSH_ATTR = 0x07, 

    RUL_POSN  = 0x10, 
    CH1_POSN  = 0x11, 
    CH2_POSN  = 0x12, 
    CH3_POSN  = 0x13, 
    CH4_POSN  = 0x14, 
    CH5_POSN  = 0x15, 
    CH6_POSN  = 0x16, 

    ZC_POSN   = 0x18, 
    T0_POSN   = 0x19, 
    T1_POSN   = 0x1A, 
    T2_POSN   = 0x1B, 

    TRG_KIND  = 0x20, 
    TRG_COND  = 0x21, 
    MEM_MASK  = 0x22, 
    MEM_TWTH  = 0x23, 

    MEM_PSMP  = 0x30,
    MEM_RPTR  = 0x31,
    MEM_READ  = 0x32,
                     
                      
    FPGA_ST   = 0x00,
    VER_NUM   = 0x01,
    BUSY      = 0x01,

};

typedef const uint8_t uc8;
typedef const uint32_t uc32;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t u8;

u8   Buffer[10];
u8  *SpiCmd  = (u8 *)&Buffer[0];
u8  *SpiInfo = (u8 *)&Buffer[0];
u8  *SpiByte = (u8 *)&Buffer[1];
u16 *SpiData = (u16 *)&Buffer[1];
u32 *SpiRecord = (u32 *)&Buffer[1];

uc8  TB_Str[][8] =
{
    "1.0nS ", "2.0nS ", "5.0nS ", "10nS  ",
    "20nS  ", "10nS  ", "20nS  ", "50nS  ",
    "0.1uS ", "0.2uS ", "0.5uS ", "1.0uS ",
    "2.0uS ", "5.0uS ", "10uS  ", "20uS  ",
    "50uS  ", "0.1mS ", "0.2mS ", "0.5mS "
};
uc32  gTB_Scale[20] =
{
    1,         2,           5,           1,
    2,         1,           2,           5,
    10,        20,          50,          100,
    200,       500,         1000,        2000,
    5000,      10000,       20000,       50000,
};

u16  gItemParam[PARAM_NUM] = {
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    8, 50, 230, 0,  TRI_POSI, 300,  0,  0,  0,  0,
    2,  0x0001,  0x0701,  1,  30,  0,  0,  0,  0,  0,
    0,  1,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  5,  5,  5, 30,  0,  0,  0,  0,
};


// BIOS===============
typedef struct
{
    u16  n;
    u8  *pBuf;
}SPI_ParamDef;

typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
Tfunc __Bios = (Tfunc)(0x08000100|1); 

u16 FPGA_ReadWrite(u8 Cmd, u16 Cnt, u16 Data)
{
    SPI_ParamDef  Param;

    u16 Temp = 0;

    Param.pBuf = SpiCmd;
    Param.n = Cnt;
    *SpiCmd = Cmd;
    *SpiData = Data;
    __Bios(FPGADEV, (u32)&Param);
    Temp |= (*SpiInfo) << 8;
    Temp |= *SpiByte;

    return Temp;
}

u32 FPGA_RW_4Byte(u8 Cmd, u16 Cnt, u32 Data)
{
    SPI_ParamDef  Param;
    u32 Temp = 0;

    Param.pBuf = SpiCmd;
    Param.n = Cnt;
    *SpiCmd = Cmd;
    *SpiRecord = Data;

    __Bios(FPGADEV, (u32)&Param);

    Temp  = *SpiInfo;
    Temp |= (*SpiRecord << 8);

    return Temp;
}






////////
int Configure(void)
{
    u16 PulseWidth = 0;

    FPGA_ReadWrite(TRG_COND, 1, gItemParam[TRICOND] >> 8);
    BIOS::SYS::DelayMs(1);
    FPGA_ReadWrite(MEM_MASK, 1, gItemParam[TRIMASK] >> 8);
    BIOS::SYS::DelayMs(1);
    PulseWidth = (u16)(gItemParam[TRIWIDTH] * gTB_Scale[gItemParam[TIMEBASE]]);
    FPGA_ReadWrite(MEM_TWTH, 2, PulseWidth);
    BIOS::SYS::DelayMs(1);
    FPGA_ReadWrite(TRG_KIND, 1, gItemParam[TRITYPE]);
    BIOS::SYS::DelayMs(1);
    return 0;
}

int Restart()
{
    FPGA_ReadWrite(SMPL_CLR, 1, 0);
    BIOS::SYS::DelayMs(2);
    return 0;
}

int Status()
{
    // (status >> 8) & 0xff == FPGA_TRI, delay 500
    return (u8)(FPGA_ReadWrite(FPGA_INFO, 0, 0) >> 8);
}

int Get()
{
/*
    char hex[32];
    uint32_t value = FPGA_RW_4Byte(MEM_READ, 3, 0);
    TERMINAL::Print("{hex:0x%08x}", value);
*/
    uint32_t buf[14];

    for (int i=0; i<14; i++)
      buf[i] = FPGA_RW_4Byte(MEM_READ, 3, 0);

    TERMINAL::Write((uint8_t*)buf, sizeof(buf));

//    TERMINAL::Print("%08x%08x%08x%08x%08x%08x", value[0], value[1], value[2], value[3], value[4], value[5]);
    return 0;
}
}