// logger.h
#include "../../os_host/source/framework/BufferedIo.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

class CApplicationLogger : public CApplication
{
    CBufferedWriter mWriter;
    char mFileName[32];
    long mLastTime{0};
    bool mOpened{false};

public:
    void AttachFs()
    {
        _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
        BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    }
    
    void DetachFs()
    {
        StopLogging();
        BIOS::FAT::SetSharedBuffer(nullptr);
    }
    
    virtual bool StartLogging() override
    {
        GetNewLogName();
        mOpened = mWriter.Open(mFileName);
        _ASSERT(mOpened);
        
        mWriter << "LA104 UartMon log file, 2021 valky.eu\n";
        char info[40];
        sprintf(info, "%d bauds %d-%s-%s\n", GetBaudRate(), GetDataBits(), GetParityAsString(), GetStopBitsAsString());
        mWriter << info;
        mWriter << "================\n\n";
        
        return mOpened;
    }
    
    virtual void StopLogging() override
    {
        if (mOpened)
        {
            mWriter.Close();
            mOpened = false;
        }
    }
    
    virtual char* GetLogName() override
    {
        char* last = strrchr(mFileName, '/');
        return last ? last+1 : mFileName;
    }
    
    virtual void OnRecv(char c) override
    {
        char temp[16];

        CApplication::OnRecv(c);

        if (!mOpened)
            return;

        int32_t now = BIOS::SYS::GetTick();
        if (mLastTime && mMarkPause)
        {
            if (mRxMode == CApplicationData::EFormatMode::Hex)
                sprintf(temp, "// %d ms\n", (int)(now - mLastTime));
            else
                sprintf(temp, "<%d>\n", (int)(now - mLastTime));

            mWriter << temp;
        }
        mLastTime = now;

        switch (mRxMode)
        {
            case CApplicationData::EFormatMode::Ascii:
                mWriter << (uint8_t)c;
                break;
            case CApplicationData::EFormatMode::Dec:
                sprintf(temp, "%d ", c);
                mWriter << temp;
                break;
            case CApplicationData::EFormatMode::Hex:
                sprintf(temp, "0x%02x, ", c);
                mWriter << temp;
                break;
        }
        
    }
    
private:
    void GetCurrentPath(char* path)
    {
        // Full application path
        strcpy(path, BIOS::OS::GetArgument());

        // Strip app name
        char* last = strrchr(path, '/');
        if (last)
            *last = 0;
        else
            strcpy(path, "");
    }
    
    bool Exists(char* fileName)
    {
        if (BIOS::FAT::Open(fileName, BIOS::FAT::EIoMode::IoRead) != BIOS::FAT::EResult::EOk)
            return false;
        
        BIOS::FAT::Close();
        return true;
    }

    void GetNewLogName()
    {
        GetCurrentPath(mFileName);
        char *pName = mFileName + strlen(mFileName);

        for (int i=0; i<99; i++)
        {
            sprintf(pName, "/uart%02d.log", i);
            if (!Exists(mFileName))
                return;
        }
        _ASSERT(!"Too many files");
    }
};
