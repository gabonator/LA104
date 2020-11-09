#include "../include/library.h"

#ifdef ALL_PLATFORMS
#define LA104
#define DS203
#define DS213
#endif

namespace BIOS
{
  namespace SYS
  {
    uint32_t GetTick() {return 0;}
    int Execute(uint32_t addr) {return 0;}
    void Beep(int intervalMs) {}
    void DelayMs(int intervalMs) {}
    bool LoadFpga(char* path) {return true;}
    uintptr_t GetAttribute(EAttribute eInternal) { return 0; }
  }

  namespace LCD
  {
    void Clear(unsigned short clr) {}

    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char c) { return 0; }
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str) { return 0; }
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str) { return 0; }
    int Printf (int x, int y, unsigned short clrf, unsigned short clrb, const char *format, ...) { return 0; }

    void PutPixel(int x, int y, unsigned short clr) {}
    void PutPixel(const CPoint& cp, unsigned short clr) {}
    uint16_t GetPixel(int x, int y) { return 0; }

    void Bar(int x1, int y1, int x2, int y2, unsigned short clr) {}
    void Bar(const CRect& rc, unsigned short clr) {}
    void Rectangle(const CRect& rc, unsigned short clr) {}
    void RoundRect(int x1, int y1, int x2, int y2, unsigned short clr) {}
    void RoundRect(const CRect& rc, unsigned short clr) {}

    void Shadow(int x1, int y1, int x2, int y2, unsigned int nColor) {}

    void BufferBegin(const CRect& rc) {}
    void BufferWrite(uint16_t clr) {}
    void BufferWrite(uint16_t* buffer, int length) {}
    uint16_t BufferRead() {return 0;}
    void BufferRead(uint16_t* buffer, int length) {}
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
    void SetSharedBuffer(void*) {}
    void* GetSharedBuffer() { return nullptr; }

    void LinearStart() {}
    bool LinearFinish() { return 0; }
    bool LinearProgram( uint32_t nAddress, unsigned char* pData, int nLength ) { return 0; }
  }

  namespace FAT
  {
    void SetSharedBuffer(void*) {}
    void* GetSharedBuffer() { return 0; }

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

#ifdef LA104
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
      bool EndTransmission(bool) {return false;}
    }

    namespace UART
    {
      void Setup(int baudrate, EConfig config) {}
      bool Available() {return false;}
      uint8_t Read() {return 0;}
      void Write(uint8_t) {}
    }
  }
#endif

  namespace OS
  {
    void SetArgument(char* argument) {}
    char* GetArgument() {return nullptr;}
    bool HasArgument() { return false; }
    BIOS::OS::TInterruptHandler GetInterruptVector(BIOS::OS::EInterruptVector) {return nullptr;}
    void SetInterruptVector(BIOS::OS::EInterruptVector, BIOS::OS::TInterruptHandler) {}
    uint32_t DisableInterrupts() { return 0; }
    void EnableInterrupts(uint32_t) {}
  }

#if !defined(DISABLE_USB)
  namespace USB
  {
    void Enable() {}
    void Disable() {}
    void Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests, THandler arrHandlerIn[], THandler arrHandlerOut[], THandler arrCallbacks[], THandler LeaveLowPowerMode) {}
    void InitializeMass() {}
    void InitializeFinish(int imr_msk) {}
  }
#endif
                   
#if defined(DS203) || defined(DS203HD) || defined(DS213) 
  namespace ADC
  {
    BIOS::ADC::EState GetState() { return BIOS::ADC::EState::Full; }

    void Restart(int sampleOffset) {}
    BIOS::ADC::TSample::SampleType Get() { return 0;}

    int GetPointer() { return 0; }
    bool Enable(bool bEnable) { return true; }
    bool Enabled() { return true; }

    void ConfigureInput(BIOS::ADC::EInput input, BIOS::ADC::ECouple couple, BIOS::ADC::EResolution res, int offset) {}
    void ConfigureTimebase(float timePerDiv) {}
    void ConfigureTrigger(int time, int value, BIOS::ADC::ETriggerType type, BIOS::ADC::EInput source) {}
  }

  namespace DAC
  {
    void SetFrequency(int freqHz) {}
    void SetDuty(int duty) {}
    int GetFrequency() {return 0;}
    int GetDuty() {return 0;}
    void SetMode(EMode mode, uint16_t* buffer, int length) {}
  }
#endif
}

namespace GUI
{
  void Background(const CRect& rc, uint32_t clr1, uint32_t clr2) {}
  void Window(const CRect& rcRect, uint16_t clr) {}
}

int sprintf(char *out, const char *format, ...) { return 0; }
int vsprintf(char *out, const char *format, va_list args) { return 0; }
