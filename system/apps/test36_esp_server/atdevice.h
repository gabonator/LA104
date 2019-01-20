class CATDevice
{
    long l0{0};
    CStreamSerial mStream;
    
public:
    CATDevice()
    {
        BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
        BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
        GPIO::UART::Setup(115200, GPIO::UART::EConfig::length8);
    }
    
    void Flush()
    {
        mBufferPos = 0;
        mBuffer[mBufferPos] = 0;
    }
    
    void Send(const char* msg)
    {
        Flush();
        l0 = SYS::GetTick();
        //BIOS::DBG::Print("<%s", msg);
        while (*msg)
        {
            GPIO::UART::Write(*msg++);
#ifdef __APPLE__
            SYS::DelayMs(3);
#endif
        }
    }
    
    bool Expect(const char* msg)
    {
        bool found = (strstr(GetBuffer(), msg) != nullptr);
        return found;
    }
    
    bool Timeout(int intervalMs)
    {
        bool passed = ((long)SYS::GetTick() - l0 > intervalMs);
        if (passed )
            fprintf(stdout, "{timeout %d passed}\n", intervalMs);
        return passed;
    }
    
    CDataStream& Stream()
    {
        return mStream;
    }
    
    virtual void OnRequest(char* key, char* payload) { _ASSERT(0); } // TODO: pure virtual
    //////
    static constexpr int AtBufferLength = 64;
    bool mAnyReceived{false};
    bool mPipe{false};
    char mBuffer[AtBufferLength] = {0};
    uint8_t mBufferPos{0};
    bool mRequest{false};
    bool mIgnoreGarbage{false};
    bool mProcessAll{false};
    int mIgnoreBytes{0};

bool mShowAll{false}; void ShowAll(bool b) {mShowAll = b;}
    void IgnoreBytes(int n)
    {
        // Could freeze!
        fprintf(stdout, "freeze:");
        for (int i=0; i<n; i++)
        {
            while (!GPIO::UART::Available());
            int c = GPIO::UART::Read();
            (void)c;
            fprintf(stdout, "%c", c);
        }
        fprintf(stdout, "\nfreeze-done\n");
        //mIgnoreBytes = n;
    }
    
    void ProcessAll(bool b)
    {
        mProcessAll = true;
    }
    
    void IgnoreGarbage(bool b)
    {
        mIgnoreGarbage = b;
    }

    char* GetBuffer()
    {
        return mBuffer;
    }

    void operator << (char c)
    {
if (mShowAll)
BIOS::DBG::Print("%c", c);
        fprintf(stdout, "%c", c);
        if (!mIgnoreGarbage && !mIgnoreBytes)
        {
            //BIOS::DBG::Print("%c", c);

        }
        if (mIgnoreBytes > 0)
        {
            mIgnoreBytes--;
            if (mIgnoreBytes == 0)
                fprintf(stdout, "<END OF IGNORED SECTION>\n");

            return;
        }
        
        static bool endline = false;
        
        if (endline)
        {
            mBufferPos = 0;
            mBuffer[mBufferPos] = 0;
            endline = false;
        }
        
        if (c == 0x0a)
        {
            if (mBuffer[0] == '+' && !mIgnoreGarbage)
            {
                char* delim = strstr(mBuffer, ":");
                _ASSERT(delim);
                if (delim)
                {
                    int l = strlen(mBuffer);
                    while (l > 1 && (mBuffer[l-1] == '\r' || mBuffer[l-1] == '\n'))
                        mBuffer[--l] = 0;
                    
                    delim[0] = 0;
                    OnRequest(mBuffer, delim+1);
                }
            }
            endline = true;
        }
        // notify only once one char earlier before reaching full buffer
        
        if (mBufferPos == sizeof(mBuffer)-2)
        {
            if (mIgnoreGarbage)
                return;
            
            fprintf(stdout, "Buffer overflow!\n");
        }
        
        if (mBufferPos >= sizeof(mBuffer)-1)
            return;
        
        mBuffer[mBufferPos++] = c;
        mBuffer[mBufferPos] = 0;
    }
    
    bool Transfer()
    {
        while (GPIO::UART::Available())
        {
            int c = GPIO::UART::Read();
            (*this) << c;
            if (c == '\n')
                return true;
        }
        return false;
    }
};

class CATDeviceFake
{
public:
    void Send(const char* msg)
    {
        fprintf(stdout, "<%s", msg);
    }
    bool Expect(const char* msg)
    {
        return true;
    }
    bool Timeout(int intervalMs)
    {
        return false;
    }
    bool Transfer()
    {
        return true;
    }
    char* GetBuffer()
    {
        static const char* buffer = "none";
        return (char*)buffer;
    }
    void IgnoreGarbage(bool)
    {
    }
};