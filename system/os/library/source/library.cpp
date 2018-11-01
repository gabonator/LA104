#include "library.h"

namespace BIOS
{
  namespace SYS
  {
    uint32_t GetTick() {return 0;}
    int Execute(uint32_t addr) {return 0;}
    void Beep(int intervalMs) {}
    void DelayMs(int intervalMs) {}
  }

  namespace LCD
  {
    void Clear(unsigned short clr) {}

    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str) { return 0; }
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str) { return 0; }

    void PutPixel(int x, int y, unsigned short clr) {}
    void PutPixel(const CPoint& cp, unsigned short clr) {}

    void Bar(int x1, int y1, int x2, int y2, unsigned short clr) {}
    void Bar(const CRect& rc, unsigned short clr) {}
    void RoundRect(int x1, int y1, int x2, int y2, unsigned short clr) {}
    void RoundRect(const CRect& rc, unsigned short clr) {}

    void Shadow(int x1, int y1, int x2, int y2, unsigned int nColor) {}

    void BufferBegin(const CRect& rc, ui8 nMode) {}
    void BufferPush(ui16 clr) {}
    void BufferEnd() {}

    // TODO: duplicity?
    void Pattern(int x1, int y1, int x2, int y2, const ui16 *pat, int l) {}
    int Draw(int x, int y, unsigned short clrf, unsigned short clrb, const char *p) { return 0; }
  }

  namespace KEY
  {
    EKey GetKey() { return None; }
  }

  namespace DBG
  {
    void Print (const char* format, ...) {}
  }

  namespace MEMORY
  {
    PVOID GetSharedBuffer() { return 0; }

    void LinearStart() {}
    bool LinearFinish() { return 0; }
    bool LinearProgram( uint32_t nAddress, unsigned char* pData, int nLength ) { return 0; }
  }

  namespace FAT
  {
    PVOID GetSharedBuffer() { return 0; }

    EResult Init() { return EOk; }
    EResult Open(const char* strName, ui8 nIoMode) { return EOk; }
    EResult Read(ui8* pSectorData) { return EOk; }
    EResult Write(ui8* pSectorData) { return EOk; }
    EResult Seek(ui32 lOffset) { return EOk; }
    EResult Close(int nSize) { return EOk; }
    EResult Close() { return EOk; }
    ui32 GetFileSize() { return 0; }
	
    EResult OpenDir(char* strPath) { return EOk; }
    EResult FindNext(TFindFile* pFile) { return EOk; }
  }

  namespace GPIO
  {
    void DigitalWrite(EPin pin, bool value) {}
    bool DigitalRead(EPin pin) { return false; }
    void AnalogWrite(EPin pin, int value) {}
    int AnalogRead(EPin pin) { return 0; }
    void PinMode(EPin pin, EMode mode) {}

    namespace I2C
    {
      bool BeginTransmission(uint8_t address) {return false;}
      bool RequestFrom(uint8_t address, uint8_t bytes) {return false;}
      bool Write(uint8_t data) {return false;}
      uint8_t Read() {return 0;}
      bool EndTransmission() {return false;}
    }

    namespace UART
    {
      void Setup(int baudrate, EConfig config) {}
      bool Available() {return false;}
      uint8_t Read() {return 0;}
      void Write(uint8_t) {}
    }

  }
}

namespace GUI
{
  void Background(CRect& rc, uint32_t clr1, uint32_t clr2) {}
  void Window(const CRect& rcRect, uint16_t clr) {}
}

int sprintf(char *out, const char *format, ...) { return 0; }