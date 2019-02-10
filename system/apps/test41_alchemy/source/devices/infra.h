#include "infra/ir_tx.h"

class CInfrared
{
  static IrCode irCode;
  BIOS::OS::TInterruptHandler pOldTimerHandler;

public:
  void Init()
  {
    ir_tx_setup();
    pOldTimerHandler = BIOS::OS::GetInterruptVector(BIOS::OS::ITIM1_UP_IRQ);
    BIOS::OS::SetInterruptVector(BIOS::OS::ITIM1_UP_IRQ, ir_irq);
    ir_tx_start();
  }

  void Deinit()
  {
    ir_tx_stop();
    BIOS::OS::SetInterruptVector(BIOS::OS::ITIM1_UP_IRQ, pOldTimerHandler);
  }

  void Send(uint16_t* pData, int nLength)
    {
        irCode.repeatCount = 1;
        irCode.gap = 0;
        irCode.codeLength = nLength;
        irCode.code = pData;
        ir_tx_send(&irCode);
    }
    
  void Send(CArray<uint16_t>& sequence)
  {
    irCode.repeatCount = 1;
    irCode.gap = 0;
    irCode.codeLength = sequence.GetSize();
    irCode.code = sequence.GetData();
    ir_tx_send(&irCode);
  }
};

IrCode CInfrared::irCode;

class CDeviceInfra : public CSampler
{
    CInfrared mDevice;
    
public:
    struct TConfig
    {
        bool mInvert{false};
        BIOS::GPIO::EPin mInputPin{BIOS::GPIO::EPin::P2};
        BIOS::GPIO::EPin mOutputPin{BIOS::GPIO::EPin::P4};
    };
    
private:
    TConfig mConfig;
    
public:
    virtual bool Init() override
    {
        BIOS::GPIO::PinMode(mConfig.mInputPin, BIOS::GPIO::EMode::Input);
        _ASSERT(mConfig.mOutputPin == BIOS::GPIO::EPin::P4); // hw timer!
        //BIOS::GPIO::PinMode(mConfig.mOutputPin, BIOS::GPIO::EMode::Output);
        return true;
    }
    
    virtual void Deinit() override
    {
        BIOS::GPIO::PinMode(mConfig.mInputPin, BIOS::GPIO::EMode::Input);
        BIOS::GPIO::PinMode(mConfig.mOutputPin, BIOS::GPIO::EMode::Input);
    }
    
    virtual bool Read() override
    {
#ifdef __APPLE__
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
    
    virtual void Send(uint16_t* pData, int nLength)
    {
        mDevice.Init();
        mDevice.Send(pData, nLength);
        BIOS::SYS::DelayMs(200);
        mDevice.Deinit();
    }
};

