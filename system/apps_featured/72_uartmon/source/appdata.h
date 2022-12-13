class CApplicationData
{
public:
    enum EFormatMode { Ascii, Hex, Dec };

protected:
    int mBaudrate{9600};
    GPIO::UART::EConfig mFlags{GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityNone | GPIO::UART::stopBits1)};
    EFormatMode mRxMode{Ascii};
    EFormatMode mTxMode{Ascii};
    bool mNewLineRx{false};
    bool mNewLineTx{false};
    bool mMarkPause{false};

    int mY{-1};
    int mX{0};

    void SetPartiy(const char* p)
    {
        mFlags = GPIO::UART::EConfig(mFlags & ~GPIO::UART::EConfigMask::parity);
        if (strcmp(p, "E") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::parityEven);
        else if (strcmp(p, "O") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::parityOdd);
        else if (strcmp(p, "N") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::parityNone);
        else
            _ASSERT(0);

        ConfigureUart();
    }

    void SetStopBits(const char* p)
    {
        mFlags = GPIO::UART::EConfig(mFlags & ~GPIO::UART::EConfigMask::stopBits);
        if (strcmp(p, "1") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::stopBits1);
        else if (strcmp(p, "1.5") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::stopBits15);
        else if (strcmp(p, "2") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::stopBits2);
        else
            _ASSERT(0);

        ConfigureUart();
    }

    void SetDataBits(int n)
    {
        mFlags = GPIO::UART::EConfig(mFlags & ~GPIO::UART::EConfigMask::length);
        if (n == 8)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::length8);
        else
        if (n == 9)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::length9);
        else
            _ASSERT(0);

        ConfigureUart();
    }
    
    void SetBaudRate(int baudrate)
    {
        mBaudrate = baudrate;
        ConfigureUart();
    }
    
    int GetBaudRate()
    {
        return mBaudrate;
    }
    
    int GetDataBits()
    {
        if (mFlags & GPIO::UART::length9)
            return 9;
        return 8;
    }
    
    const char* GetParityAsString()
    {
        if (mFlags & GPIO::UART::parityEven)
            return "E";
        if (mFlags & GPIO::UART::parityOdd)
            return "O";
        return "N";
    }
    
    const char* GetStopBitsAsString()
    {
        if (mFlags & GPIO::UART::stopBits15)
            return "1.5";
        if (mFlags & GPIO::UART::stopBits2)
            return "2";
        return "1";
    }
    
    virtual void ConfigureUart() = 0;
};
