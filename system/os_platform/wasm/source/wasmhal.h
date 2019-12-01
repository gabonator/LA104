#include "../../common/source/bios/Bios.h"

extern "C"
{
extern void js_init();
extern void js_setPixel(int x, int y, int c);
extern int js_getPixel(int x, int y);
extern bool js_running();
extern int js_getKey();
extern void js_loop();
extern int js_ticks();
extern void js_flashRead(uint8_t* buff, int offset, int length);
extern void js_flashWrite(const uint8_t* buff, int offset, int length);
extern void js_setArgument(const char* arg);
}


//extern uint8_t pixels[];
//extern bool pixelsChanged;
/*
void setPixel(int x, int y, int c)
{
    y = BIOS::LCD::Height-1-y;
    assert(x >= 0 && x < BIOS::LCD::Width);
    assert(y >= 0 && y < BIOS::LCD::Height);
    
    const unsigned int offset = ( BIOS::LCD::Width * 4 * y ) + x * 4;
    pixels[ offset + 0 ] = Get565R(c);
    pixels[ offset + 1 ] = Get565G(c);
    pixels[ offset + 2 ] = Get565B(c);
    pixels[ offset + 3 ] = 255; //SDL_ALPHA_OPAQUE;
}

int getPixel(int x, int y)
{
    y = BIOS::LCD::Height-1-y;
    assert(x >= 0 && x < BIOS::LCD::Width);
    assert(y >= 0 && y < BIOS::LCD::Height);
    
    const unsigned int offset = ( BIOS::LCD::Width * 4 * y ) + x * 4;
    int r = pixels[ offset + 0 ];
    int g = pixels[ offset + 1 ];
    int b = pixels[ offset + 2 ];
    return RGB565RGB(r, g, b);
}
*/
class CWasmHal : public CHal
{
public:
  CWasmHal()
  {
    js_init();
  }
  virtual void SetPixel(int x, int y, uint16_t c) override
  {
//    setPixel(x, y, c);
    js_setPixel(x, y, c);
//    pixelsChanged = true;
  }
  virtual uint16_t GetPixel(int x, int y) override
  {
//    return getPixel(x, y);
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
/*    
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
*/

	virtual void FlashRead(uint8_t* buff, int offset, int length) override 
        { 
          js_flashRead(buff, offset, length);
        }

	virtual void FlashWrite(const uint8_t* buff, int offset, int length) override 
        {
          js_flashWrite(buff, offset, length);
        }

};
