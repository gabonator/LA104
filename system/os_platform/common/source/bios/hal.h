#pragma once
class CHal
{
public:
    ~CHal() = default;
    virtual void SetPixel(int x, int y, uint16_t c) = 0;
    virtual uint16_t GetPixel(int x, int y) = 0;
    virtual bool IsRunning() = 0;
    virtual char GetKey() = 0;
    // GPIO
    virtual void UartSetup(int baudrate, BIOS::GPIO::UART::EConfig config) = 0;
    virtual void UartClose() = 0;
    virtual bool UartAvailable() = 0;
    virtual uint8_t UartRead() = 0;
    virtual void UartWrite(uint8_t data) = 0;
    // SYS
    virtual void Delay(int intervalMs) = 0;
    virtual uint32_t GetTick() = 0;
    // FAT
    virtual bool FatInit() = 0;
    virtual bool FatOpen(const char* strName, ui8 nIoMode) = 0;
    virtual bool FatRead(uint8_t* pSectorData, int length) = 0;
    virtual bool FatWrite(uint8_t* pSectorData, int length) = 0;
    virtual bool FatClose() = 0;
    virtual bool FatOpenDir(char* path) = 0;
    virtual bool FatFindNext(BIOS::FAT::TFindFile* pFile) = 0;
    virtual uint32_t FatGetFileSize() = 0;
    virtual bool FatSeek(uint32_t offset) = 0;
};

extern CHal* gHal;
