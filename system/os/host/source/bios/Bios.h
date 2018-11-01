#pragma once
#include "../framework/Classes.h"

namespace BIOS
{
  namespace SYS
  {
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

    enum { // TODO: named enum
      // BufferBegin mode
      BfXY = 1,
      BfYX = 2
    };

    void Clear(unsigned short clr);

    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str);
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str);

    void PutPixel(int x, int y, unsigned short clr);
    void PutPixel(const CPoint& cp, unsigned short clr);

    void Bar(int x1, int y1, int x2, int y2, unsigned short clr);
    void Bar(const CRect& rc, unsigned short clr);
    void RoundRect(int x1, int y1, int x2, int y2, unsigned short clr);
    void RoundRect(const CRect& rc, unsigned short clr);

    void Shadow(int x1, int y1, int x2, int y2, unsigned int nColor);

    void BufferBegin(const CRect& rc, ui8 nMode);
    void BufferPush(ui16 clr);
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
}