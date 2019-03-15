#include "../../common/source/bios/Bios.h"

extern "C"
{
extern void js_setPixel(int x, int y, int c);
extern int js_getPixel(int x, int y);
extern bool js_running();
extern int js_getKey();
extern void js_loop();
extern int js_ticks();
}

class CWasmHal : public CHal
{
  virtual void SetPixel(int x, int y, uint16_t c) override
  {
    js_setPixel(x, y, c);
  }
  virtual uint16_t GetPixel(int x, int y) override
  {
    return js_getPixel(x, y);
  }
  virtual bool IsRunning() override
  {
    js_loop();
    return js_running();
  }
  virtual char GetKey() override
  {
    return js_getKey();
  }

    virtual void UartSetup(int baudrate, BIOS::GPIO::UART::EConfig config) override
    {
    }
    
    virtual void UartClose() override
    {
    }
    
    virtual bool UartAvailable() override
    {
        return false;
    }
    
    virtual uint8_t UartRead() override
    {
        return 0;
    }
    
    virtual void UartWrite(uint8_t data) override
    {
    }
    
    // SYS
    virtual void Delay(int intervalMs) override
    {
    }
    
    virtual uint32_t GetTick() override
    {
        return js_ticks();
    }
    
    // FAT
    virtual bool FatInit() override
    {
        return false;
    }
    
    virtual bool FatOpen(const char* strName, ui8 nIoMode) override
    {
        return false;
    }
    
    virtual bool FatRead(uint8_t* pSectorData, int length) override
    {
        return false;
    }
    
    virtual bool FatWrite(uint8_t* pSectorData, int length) override
    {
        return false;
    }
    
    virtual bool FatClose() override
    {
        return false;
    }
    
    virtual bool FatOpenDir(char* strPath) override
    {
        return false;
    }
    
    virtual bool FatFindNext(BIOS::FAT::TFindFile* pFile) override
    {
        return false;
    }
    
    virtual uint32_t FatGetFileSize() override
    {
        return false;
    }
    
    virtual bool FatSeek(uint32_t offset) override
    {
        return false;
    }
};
