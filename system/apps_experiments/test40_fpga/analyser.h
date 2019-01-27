      uint8_t   Buffer[10];
      uint8_t  *SpiCmd  = (uint8_t *)&Buffer[0];
      uint8_t  *SpiInfo = (uint8_t *)&Buffer[0];
      uint8_t  *SpiByte = (uint8_t *)&Buffer[1];
      uint16_t *SpiData = (uint16_t *)&Buffer[1]; // WTF? ALIASING?
      uint32_t *SpiRecord = (uint32_t *)&Buffer[1]; // WTF? ALIASING?

class CBios
{
public:
  enum
  {
      SYSINFO, // ϵͳ
      NIVCPTR, // 
      SYSTICK, // ϵͳ
      AF_RMAP, // 
      PWRCTRL, // 
      BUZZDEV, // 
      KEYnDEV, // 
      DELAYuS, // ΢
      DISPDEV, // LCD 
      BLOCK_X, // 
      BLOCK_Y, // 
      PIXEL_X, // 
      PIXEL_Y, // 
      WrPIXEL, // 
      RdPIXEL, // 
      FONTPTR, //                   Var: ASCII Code        Rtn: ptr 
      FPGADEV, // FPGA 
  };
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

  enum {
    FPGA_BUSY       = 0x01,
    FPGA_FULL       = 0x02,
    FPGA_EMPTY      = 0x04,
    FPGA_PRES       = 0x08,
    FPGA_TRI        = 0x10
  };

  typedef struct
  {
      uint16_t  n;
      uint8_t  *pBuf;
  } SPI_ParamDef;

  // uint32_t [0]:Info ,[1]:Tcnt[7:0],[2]:Tcnt[13:8],[3]:Di  
  static uint32_t __Bios(uint32_t a, uint32_t b)
  {
    typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
    static Tfunc ___Bios = (Tfunc)(0x08000100|1); 
    return ___Bios(a, b);
  }

  static uint32_t FPGA_RW_4Byte(uint8_t Cmd, uint16_t Cnt, uint32_t Data)
  {
      SPI_ParamDef  Param;
      uint32_t Temp = 0;

      Param.pBuf = SpiCmd;
      Param.n = Cnt;
      *SpiCmd = Cmd;
      *SpiRecord = Data;

      __Bios(FPGADEV, (uint32_t)&Param);

      Temp  = *SpiInfo;
      Temp |= *SpiRecord << 8;

      return Temp;
  }

  static uint16_t FPGA_ReadWrite(uint8_t Cmd, uint16_t Cnt, uint16_t Data)
  {
      (void)SpiCmd; (void)SpiInfo; (void)SpiByte; (void)SpiData; (void)SpiRecord;

      SPI_ParamDef  Param;

      uint16_t Temp = 0;

      Param.pBuf = SpiCmd;
      Param.n = Cnt;
      *SpiCmd = Cmd;
      *SpiData = Data;
      __Bios(FPGADEV, (uint32_t)&Param);
      Temp |= (*SpiInfo) << 8;
      Temp |= *SpiByte;

      return Temp;
  }
};

class CAnalyser
{
public:
  enum EState { _finished, _valid, _wait, _busy, _valid2 };

  void StartSampling()
  {
      CBios::FPGA_ReadWrite(CBios::SMPL_CLR, 1, 0);
//      BIOS::SYS::DelayMs(2);
  }

  int Finished()
  {
    uint32_t SampInfo = CBios::FPGA_ReadWrite(CBios::FPGA_INFO, 0, 0); 
//(uint8_t)(CBios::FPGA_ReadWrite(CBios::FPGA_INFO, 0, 0) >> 8);
    return SampInfo;// & FPGA_TRI;
  //    }while ((SampInfo & FPGA_TRI) == 0);                 // Tri
  //  while ((SampInfo & FPGA_FULL) == 0);                // Full
  //  while ((SampInfo & FPGA_PRES) == 0);                // Ptrsamp
  }

  uint32_t Read()
  {
    return CBios::FPGA_RW_4Byte(CBios::MEM_READ, 3, 0);
  }

  EState State(uint32_t sample)
  {
    if ((sample & (CBios::FPGA_EMPTY | CBios::FPGA_FULL)) == (CBios::FPGA_EMPTY | CBios::FPGA_FULL))
    {
      return _finished;
    }

    if ((sample & CBios::FPGA_FULL) == CBios::FPGA_FULL)
    {
      // FULL
      return _valid;
    }

    if ((sample & CBios::FPGA_EMPTY) == CBios::FPGA_EMPTY)
    {
      return _wait;
    }

    if ((sample & CBios::FPGA_BUSY) == CBios::FPGA_BUSY)
    {
      return _busy;
    }

    return _valid2;
  }

  void SetTrigger()
  {
    uint16_t triggerCondition = 0; // TRICOND
    uint16_t triggerMask = 0; // TRIMASK

//uc8  TriCondStr[][4] = {  "Non", " ^`", " ==", "Any", " ^`", " ==", " ^`", " ==", };

    uint16_t triggerType = 0; // gItemParam[TRITYPE]
    uint16_t pulseWidth = 0; // (u16)(gItemParam[TRIWIDTH] * gTB_Scale[gItemParam[TIMEBASE]])

    CBios::FPGA_ReadWrite(CBios::TRG_COND, 1, triggerCondition);
    BIOS::SYS::DelayMs(1);
    CBios::FPGA_ReadWrite(CBios::MEM_MASK, 1, triggerMask);
    BIOS::SYS::DelayMs(1);
    CBios::FPGA_ReadWrite(CBios::MEM_TWTH, 2, pulseWidth);
    BIOS::SYS::DelayMs(1);
    CBios::FPGA_ReadWrite(CBios::TRG_KIND, 1, triggerType);
    BIOS::SYS::DelayMs(1);

  }
};
