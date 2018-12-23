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
    void DelayUs(int intervalUs);
  }

  namespace LCD
  {
    const int Width = 320;
    const int Height = 240;

    void Clear(unsigned short clr);

    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str);
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str);

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
    uint16_t BufferRead();
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
    PVOID GetSharedBuffer();

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

    const int SectorSize = 4096;
    PVOID GetSharedBuffer();

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

  namespace GPIO
  {
    enum EPin {P1, P2, P3, P4};                 
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
      bool EndTransmission();
    }

    namespace UART
    {
      enum EConfig {length8 = 0, length9 = 0x10, stopBits1 = 0, stopBits15 = 0x1, stopBits2 = 0x2,
        parityNone = 0, parityEven = 0x4, paritOdd = 0x08, flowNone = 0, flowHw = 0x20};
      void Setup(int baudrate, EConfig config);
      bool Available();
      uint8_t Read();
      void Write(uint8_t);
    }
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
    TInterruptHandler GetInterruptVector(EInterruptVector);
    void SetInterruptVector(EInterruptVector, TInterruptHandler);
    uint32_t DisableInterrupts();
    void EnableInterrupts(uint32_t);
  }

  namespace PRIVATE
  {
    const void* GetCharRom();
  }

  namespace USB
  {
    typedef void (*THandler)(void);

    void Enable();
    void Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
      THandler arrHandlerIn[], THandler arrHandlerOut[], THandler arrCallbacks[]);
    void InitializeMass();
    void InitializeSerial();
    void Disable();

    void InitializeFinish(int msk);
  }
}

