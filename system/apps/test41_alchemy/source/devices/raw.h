class CDeviceRaw : public CSampler
{
public:
    struct TConfig
    {
        bool mInvert{false};
        BIOS::GPIO::EPin mInputPin{BIOS::GPIO::EPin::P2};
    };
    
private:
    TConfig mConfig;
    
public:
    virtual bool Init() override
    {
        BIOS::GPIO::PinMode(mConfig.mInputPin, BIOS::GPIO::EMode::Input);
        //BIOS::GPIO::PinMode(mConfig.mOutputPin, BIOS::GPIO::EMode::Output);
        return true;
    }
    
    virtual void Deinit() override
    {
        BIOS::GPIO::PinMode(mConfig.mInputPin, BIOS::GPIO::EMode::Input);
        //BIOS::GPIO::PinMode(Output, BIOS::GPIO::EMode::Input);
    }

    virtual bool Read() override
    {
#ifdef EMULATED
        int nTick = BIOS::SYS::GetTick();
        int nSecond = nTick / 2000;
        static int nLastSecond = 0;
        if (nSecond == nLastSecond)
            return false;
        nLastSecond = nSecond;
        return true;
#endif
        return BIOS::GPIO::DigitalRead(mConfig.mInputPin) ^ mConfig.mInvert;
    }
    
    TConfig& Configuration()
    {
        return mConfig;
    }
};

