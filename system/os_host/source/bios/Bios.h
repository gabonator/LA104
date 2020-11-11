#pragma once
#include "../framework/Classes.h"

namespace BIOS
{
  namespace SYS
  {
    void Init();
    uint32_t GetTick();
    int Execute(uint32_t addr);
    void Beep(int intervalMs);
    void DelayMs(int intervalMs);
    bool LoadFpga(char* path);

    enum class EAttribute {BiosVersion, CharRom, LastChar, ScreenWidth, ScreenHeight, DeviceType,
      BuildRevision, BuildDate, BuildUser, BuildSystem, 
      VersionDfu, VersionHardware, VersionSystem, VersionFpga,
      SerialNumber, LicenseNumber, LicenseValid, DisplayType, DiskType,
      DiskSectorSize, DiskSectorCount,
      GpioStatus, KeyMask, 
      FlashReadRange, FlashWriteRange, FlashAlertRange,
      BatteryVoltage, Charging,
      GpioI2cSpeed};

    uintptr_t GetAttribute(EAttribute eInternal);
  }

  namespace LCD
  {
#ifdef LA104
    const int Width = 320;
    const int Height = 240;
#elif defined(DS213)
    const int Width = 400;
    const int Height = 240;
#elif defined(DS203) || defined(DS203HD)
    const int Width = 400;
    const int Height = 240;
#endif

    void Clear(unsigned short clr);

    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char c);
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str);
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str);
    int Printf (int x, int y, unsigned short clrf, unsigned short clrb, const char* format, ...);

    void PutPixel(int x, int y, unsigned short clr);
    void PutPixel(const CPoint& cp, unsigned short clr);
    uint16_t GetPixel(int x, int y);

    void Bar(int x1, int y1, int x2, int y2, unsigned short clr);
    void Bar(const CRect& rc, unsigned short clr);
    void Rectangle(const CRect& rc, unsigned short clr);
    void RoundRect(int x1, int y1, int x2, int y2, unsigned short clr);
    void RoundRect(const CRect& rc, unsigned short clr);

    void Shadow(int x1, int y1, int x2, int y2, unsigned int nColor);

    void BufferBegin(const CRect& rc);
    void BufferWrite(uint16_t clr);
    void BufferWrite(uint16_t* buffer, int length);
    uint16_t BufferRead();
    void BufferRead(uint16_t* buffer, int length);
    void BufferEnd();

    // TODO: duplicity?
    void Pattern(int x1, int y1, int x2, int y2, const ui16 *pat, int l);
    int Draw(int x, int y, unsigned short clrf, unsigned short clrb, const char *p);
  }

  namespace KEY
  {
    enum EKey { None, Down, Up, Left, Right, F1, F2, F3, F4, Escape = F2, Enter = F1 };
    EKey GetKey();
  }

  namespace DBG
  {
    void Print (const char* format, ...);
  }

  namespace MEMORY
  {
    const int SharedBufferSize = 2048+128;

    void SetSharedBuffer(void*);
    void* GetSharedBuffer();

    void LinearStart();
    bool LinearFinish();
    bool LinearProgram( uint32_t nAddress, unsigned char* pData, int nLength );
  }

  namespace FAT
  {
    enum EIoMode {
      IoRead = 1,
      IoWrite = 2,
      IoClosed = 3
    };

    enum EResult 
    {
      EOk,
      EDiskError,
      EIntError,
      ENoFile,
      ENoPath,
      EDiskFull
    };

    enum EAttribute 
    {
      EReadOnly = 1,
      EHidden = 2,
      ESystem = 4,
      EDirectory = 0x10,
      EArchive = 0x20
    };

    struct TFindFile
    {
      ui32 nFileLength;		
      ui16 nDate;
      ui16 nTime;
      ui8 nAtrib;
      char strName[13];
    };

#if defined(DS203)
    const int SectorSize = 512;
    const int SectorCount = 4096;
#elif defined(DS203HD)
    const int SectorSize = 4096;
    const int SectorCount = 2048;
#elif defined(DS213)
    const int SectorSize = 4096;
    const int SectorCount = 2047;
#elif defined(LA104)
    const int SectorSize = 4096;
    const int SectorCount = 2048;
#endif

    const int SharedBufferSize = SectorSize;

    void SetSharedBuffer(void*);
    void* GetSharedBuffer();

    EResult Init();
    EResult Open(const char* strName, ui8 nIoMode);
    EResult Read(ui8* pSectorData);
    EResult Write(ui8* pSectorData);
    EResult Seek(ui32 lOffset);
    EResult Close(int nSize);
    EResult Close();
    ui32 GetFileSize();
	
    EResult OpenDir(char* strPath);
    EResult FindNext(TFindFile* pFile);
  }

  namespace OS
  {
    typedef void (*TInterruptHandler)(void);
    enum EInterruptVector {
      IStackTop, IReset, INMIException, IHardFaultException, IMemManageException, 
      IBusFaultException, IUsageFaultException, _Dummy1, _Dummy2,
      _Dummy3, _Dummy4, ISVC, IDebugMonitor, _Dummy5, IPendSVC, 
      ISysTick, IWWDG_IRQ, IPVD_IRQ, ITAMPER_IRQ, IRTC_IRQ, IFLASH_IRQ,
      IRCC_IRQ, IEXTI0_IRQ, IEXTI1_IRQ, IEXTI2_IRQ, IEXTI3_IRQ, IEXTI4_IRQ,
      IDMA1_Channel1_IRQ, IDMA1_Channel2_IRQ, IDMA1_Channel3_IRQ,
      IDMA1_Channel4_IRQ, IDMA1_Channel5_IRQ, IDMA1_Channel6_IRQ,
      IDMA1_Channel7_IRQ, IADC1_2_IRQ, IUSB_HP_CAN_TX_IRQ, 
      IUSB_LP_CAN_RX0_IRQ, ICAN_RX1_IRQ, ICAN_SCE_IRQ, IEXTI9_5_IRQ,
      ITIM1_BRK_IRQ, ITIM1_UP_IRQ, ITIM1_TRG_COM_IRQ, ITIM1_CC_IRQ,
      ITIM2_IRQ, ITIM3_IRQ, ITIM4_IRQ, II2C1_EV_IRQ, II2C1_ER_IRQ,
      II2C2_EV_IRQ, II2C2_ER_IRQ, ISPI1_IRQ, ISPI2_IRQ, IUSART1_IRQ,
      IUSART2_IRQ, IUSART3_IRQ, IEXTI15_10_IRQ, IRTCAlarm_IRQ, 
      IUSBWakeUp_IRQ, ITIM8_BRK_IRQ, ITIM8_UP_IRQ, ITIM8_TRG_COM_IRQ,
      ITIM8_CC_IRQ, IADC3_IRQ, IFSMC_IRQ, ISDIO_IRQ, ITIM5_IRQ,
      ISPI3_IRQ, IUART4_IRQ, IUART5_IRQ, ITIM6_IRQ, ITIM7_IRQ,
      IDMA2_Channel1_IRQ, IDMA2_Channel2_IRQ, IDMA2_Channel3_IRQ,
      IDMA2_Channel4_5_IRQ };

    void SetArgument(char* argument);
    char* GetArgument();
    bool HasArgument();
    TInterruptHandler GetInterruptVector(EInterruptVector);
    void SetInterruptVector(EInterruptVector, TInterruptHandler);
    uint32_t DisableInterrupts();
    void EnableInterrupts(uint32_t);
  }

  namespace USB
  {
    typedef void (*THandler)(void);

    void Enable();
    void Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler arrCallbacks[], THandler leaveLowPowerMode);
    void InitializeMass();
    void Disable();

    void InitializeFinish(int msk);
  }

#ifdef LA104
  namespace GPIO
  {
    enum EPin {P1, P2, P3, P4, CH1, CH2, CH3, CH4};
    enum EMode {Input = 1, Output = 2, Pwm = 4, PullUp = 8, PullDown = 16, I2c = 32, Uart = 64};
    const int AnalogRange = 1024;

    void DigitalWrite(EPin pin, bool value);
    bool DigitalRead(EPin pin);
    void AnalogWrite(EPin pin, int value);
    int AnalogRead(EPin pin);
    void PinMode(EPin pin, EMode mode);

    namespace I2C          
    {
      bool BeginTransmission(uint8_t address);
      bool RequestFrom(uint8_t address, uint8_t bytes);
      bool Write(uint8_t data);
      uint8_t Read();
      bool EndTransmission(bool stop = true);
    }

    namespace UART
    {
      enum EConfig {length8 = 0, length9 = 0x10, stopBits1 = 0, stopBits15 = 0x1, stopBits2 = 0x2,
        parityNone = 0, parityEven = 0x4, parityOdd = 0x08, flowNone = 0, flowHw = 0x20};
      enum EConfigMask {length = EConfig::length8 | EConfig::length9, stopBits = EConfig::stopBits1 | EConfig::stopBits15 |
          EConfig::stopBits2, parity = EConfig::parityNone | EConfig::parityEven | EConfig::parityOdd, flow = EConfig::flowNone | EConfig::flowHw
      };
      void Setup(int baudrate, EConfig config);
      bool Available();
      uint8_t Read();
      void Write(uint8_t);
    }
  }
#endif
	
#if defined(DS203) || defined(DS203HD) || defined(DS213)
  namespace ADC
  {
    const int NumSamples = 4096;

    union TSample 
    {
      typedef uint32_t SampleType;

      struct
      {
        uint8_t CH1 : 8;
        uint8_t CH2 : 8;
        uint8_t CH3 : 1;
        uint8_t CH4 : 1;
      };
      uint8_t CH[4];
      SampleType value;
    };

    static const TSample::SampleType NoData = 0xffffffff;

    enum class EState {
      Offline = 0,
      Online = 1,        // ready to transfer in auto mode
      Triggered = 2,     // ready to transfer
      Full = 3,          // ready to transfer, buffer full
      Busy = 4           // cant transfer now
    };

    enum class EInput {
      CH1,
      CH2,
      CH3,
      CH4
    };

    enum class ECouple {
      DC = 0,
      AC = 1
    };

    enum class EResolution {
      _50mV,  
      _100mV, 
      _200mV, 
      _500mV, 
      _1V, 
      _2V, 
      _5V, 
      _10V
    }; 

    enum class ETriggerType { 
      EdgeHL, 
      EdgeLH, 
      LevelLow, 
      LevelHigh, 
      LowerDTLow, 
      GreaterDTLow, 
      LowerDTHigh, 
      GreaterDTHigh,
      None
    };

    EState GetState();

    void Restart(int nSampleOffset);
    TSample::SampleType Get();

    int GetPointer();
    bool Enable(bool bEnable);
    bool Enabled();

    void ConfigureInput(EInput input, ECouple couple, EResolution res, int offset);
    void ConfigureTimebase(float timePerDiv);
    void ConfigureTrigger(int time, int value, ETriggerType type, EInput source);
  }

  namespace DAC
  {
    enum class EMode {
      Off, Square, Buffer, LogicHigh, LogicLow, Noise, Triangle
    };

    const int SampleMaxValue = 0xfff;

    void SetFrequency(int freqHz);
    void SetDuty(int duty);
    int GetFrequency();
    int GetDuty();
    void SetMode(EMode, uint16_t* buffer, int length);
  }

#endif
}



